#include "ofApp.h"
#include "ofxTimeMeasurements.h"

//--------------------------------------------------------------
const int SOM_WIDTH = 256;
const int SOM_HEIGHT = 256;
void ofApp::setup(){
  ofxTimeMeasurements::instance()->setEnabled(true);

  double minInstance[3] = { 0, 0, 0 };
  double maxInstance[3] = { 1.0, 1.0, 1.0 };
  som.setFeaturesRange(3, minInstance, maxInstance);
  som.setMapSize(SOM_WIDTH, SOM_HEIGHT); // can go to 3 dimensions
  som.setInitialLearningRate(0.1);
  som.setNumIterations(3000);
  som.setup();
  
  somImage.allocate(SOM_WIDTH, SOM_HEIGHT, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){
  TS_START("update-audoanalysis");
  audioDataProcessorPtr->update();
  TS_STOP("update-audoanalysis");

  if (audioDataProcessorPtr->isDataValid()) {
    TS_START("update-som");
    float s = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::pitch, 700.0, 1300.0);
    float t = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::rootMeanSquare, 400.0, 4000.0, false);
    float u = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::spectralKurtosis, 0.0, 25.0);
//    float s = audioDataProcessorPtr->getNormalisedScalarValue(ofxAudioAnalysisClient::AnalysisScalar::spectralCentroid, 0.4, 6.0);
    double instance[3] = {
      static_cast<double>(s),
      static_cast<double>(t),
      static_cast<double>(u)
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
//  ofBackground(0);
  somImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
