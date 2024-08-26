#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAudioAnalysisClient.h"
#include "ofxAudioData.h"
#include "ofxSelfOrganizingMap.h"

class ofApp : public ofBaseApp {
  
public:
  void setup() override;
  void update() override;
  void draw() override;
  void exit() override;
  
  void keyPressed(int key) override;
  void keyReleased(int key) override;
  void mouseMoved(int x, int y ) override;
  void mouseDragged(int x, int y, int button) override;
  void mousePressed(int x, int y, int button) override;
  void mouseReleased(int x, int y, int button) override;
  void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
  void mouseEntered(int x, int y) override;
  void mouseExited(int x, int y) override;
  void windowResized(int w, int h) override;
  void dragEvent(ofDragInfo dragInfo) override;
  void gotMessage(ofMessage msg) override;
  
  std::shared_ptr<ofxAudioAnalysisClient::FileClient> audioAnalysisClientPtr { std::make_shared<ofxAudioAnalysisClient::FileClient>("Jam-20240402-094851837") };
//  std::shared_ptr<ofxAudioAnalysisClient::FileClient> audioAnalysisClientPtr { std::make_shared<ofxAudioAnalysisClient::FileClient>("Jam-20240517-155805463") };
  std::shared_ptr<ofxAudioData::Processor> audioDataProcessorPtr { std::make_shared<ofxAudioData::Processor>(audioAnalysisClientPtr) };

  ofxSelfOrganizingMap som;
  ofImage somImage;
};
