#include "ofApp.h"
#include "ofxTimeMeasurements.h"

//--------------------------------------------------------------
const int SOM_WIDTH = 64;
const int SOM_HEIGHT = 64;
void ofApp::setup(){
  ofxTimeMeasurements::instance()->setEnabled(false);
  
  ofSetRandomSeed(0);

  double minInstance[3] = { 0, 0, 0 };
  double maxInstance[3] = { 1.0, 1.0, 1.0 };
  som.setFeaturesRange(3, minInstance, maxInstance);
  som.setMapSize(SOM_WIDTH, SOM_HEIGHT); // can go to 3 dimensions
  som.setInitialLearningRate(0.01);
  som.setNumIterations(20000);
  som.setup();
  
  somImage.allocate(SOM_WIDTH, SOM_HEIGHT, OF_IMAGE_COLOR);
  
//    audioAnalysisClientPtr = std::make_shared<ofxAudioAnalysisClient::LocalGistClient>(ofToDataPath("violin.wav"));
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

  if (som.getCurrentIteration() < som.getNumIterations()) {
    TS_START("update-som");
    float u = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::complexSpectralDifference, 0.0, 100.0);
    float v = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::spectralCrest, 0.0, 100.0);
    float w = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::zeroCrossingRate, 0.0, 20.0);
    
    double instance[3] = {
      static_cast<double>(u),
      static_cast<double>(v),
      static_cast<double>(w)
    };
    som.updateMap(instance);
    TS_STOP("update-som");
    
    TS_START("update-som-image");
    for (int i = 0; i < SOM_WIDTH; i++) {
      for (int j = 0; j < SOM_HEIGHT; j++) {
        double * c = som.getMapAt(i,j);
        ofFloatColor col(c[0], c[1], c[2]);
        somImage.setColor(i, j, col);
      }
    }
    somImage.update();
    TS_STOP("update-som-image");
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  somImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::exit(){
  audioAnalysisClientPtr->closeStream();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (audioAnalysisClientPtr->keyPressed(key)) return;
  if (key == 'S') {
    ofSaveImage(somImage, ofFilePath::getUserHomeDir()+"/Documents/musicsom/snapshot-"+ofGetTimestampString()+".png", OF_IMAGE_QUALITY_BEST);
  }
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
