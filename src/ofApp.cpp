#include "ofApp.h"
#include "ofxTimeMeasurements.h"

SomPalette::SomPalette(int width_, int height_, float initialLearningRate_, int numIterations_) :
width { width_ },
height { height_ }
{
  setThreadName("SomPalette");
  
  som.setInitialLearningRate(initialLearningRate_);
  som.setNumIterations(numIterations_);
  
  double minInstance[3] = { 0, 0, 0 };
  double maxInstance[3] = { 1.0, 1.0, 1.0 };
  som.setFeaturesRange(3, minInstance, maxInstance);
  som.setMapSize(width, height); // can go to 3 dimensions
  
  som.setup();
    
  startThread();
}

SomPalette::~SomPalette() {
  newInstanceData.close();
  waitForThread(true);
}

void SomPalette::addInstanceData(SomInstanceDataT& instanceData) {
  if (isIterating()) newInstanceData.send(instanceData);
}

// TODO: Make sure we can't be overwhelmed if producer fills queue faster than we consume (e.g. could just do the SOM not the pixels)
void SomPalette::threadedFunction() {
  SomInstanceDataT instanceData;
  ofPixels p;
  p.allocate(width, height, OF_IMAGE_COLOR);

  while (newInstanceData.receive(instanceData)) {
    som.updateMap(instanceData.data());
    for (int i = 0; i < width; i++) {
      for (int j = 0; j < height; j++) {
        double* c = som.getMapAt(i, j);
        ofFloatColor col(c[0], c[1], c[2]);
        p.setColor(i, j, col);
      }
    }
    newPalettePixels.send(std::move(p));
  }
}

void SomPalette::update() {
  isNewPalettePixelsReady = false;
  while (newPalettePixels.tryReceive(pixels)) {
    isNewPalettePixelsReady = true;
  }
  if (isNewPalettePixelsReady) {
    if (!paletteTexture.isAllocated()) paletteTexture.allocate(pixels);
    paletteTexture.loadData(pixels);
  }
}

bool SomPalette::keyPressed(int key) {
  if (key == 'S' && paletteTexture.isAllocated()) {
    ofPixels p;
    paletteTexture.readToPixels(p);
    ofSaveImage(p, ofFilePath::getUserHomeDir()+"/Documents/som/snapshot-"+ofGetTimestampString()+".png", OF_IMAGE_QUALITY_BEST);
    return true;
  }
  return false;
}

void SomPalette::draw() {
  if (paletteTexture.isAllocated()) {
    paletteTexture.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
  }
}

//--------------------------------------------------------------
void ofApp::setup(){
  ofxTimeMeasurements::instance()->setEnabled(false);
  
//  ofSetRandomSeed(0);

    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("violin.wav"));
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("trombone.wav"));
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("Nightsong.wav"));
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("Treganna.wav"));
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("bells-descending-peal.wav"));
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("violin-tune.wav"));
  //  audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>();
    audioDataProcessorPtr = std::make_shared<ofxAudioData::Processor>(audioAnalysisClientPtr);
}

//--------------------------------------------------------------
void ofApp::update(){
  TS_START("update-audoanalysis");
  audioDataProcessorPtr->update();
  TS_STOP("update-audoanalysis");

  TS_START("update-som");
  float u = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::complexSpectralDifference, 0.0, 100.0);
  float v = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::spectralCrest, 0.0, 100.0);
  float w = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::zeroCrossingRate, 0.0, 20.0);
  
  std::array<double, 3> instance {
    static_cast<double>(u),
    static_cast<double>(v),
    static_cast<double>(w)
  };
  somPalette.addInstanceData(instance);
  somPalette.update();
  TS_STOP("update-som");
}

//--------------------------------------------------------------
void ofApp::draw(){
  somPalette.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
  audioAnalysisClientPtr->closeStream();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (audioAnalysisClientPtr->keyPressed(key)) return;
  if (somPalette.keyPressed(key)) return;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
