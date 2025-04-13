#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAudioAnalysisClient.h"
#include "ofxAudioData.h"
#include "ofxSelfOrganizingMap.h"

// The doubles need to be normalised 0.0..1.0
using SomInstanceDataT = std::array<double, 3>;

class SomPalette: public ofThread {
public:
  SomPalette(int width_, int height_, float initialLearningRate, int numIterations);
  ~SomPalette();
  bool isIterating() { return som.getCurrentIteration() < som.getNumIterations(); }
  void addInstanceData(SomInstanceDataT& instanceData);
  void update(); // move pixels into a GL texture on main thread
  bool keyPressed(int key);
  void draw();
protected:
  void threadedFunction() override;
private:
  int width, height;
  ofxSelfOrganizingMap som;

  ofThreadChannel<SomInstanceDataT> newInstanceData;
  ofThreadChannel<ofPixels> newPalettePixels;
  bool isNewPalettePixelsReady;
  ofPixels pixels; // non-GL, used in the threaded function

  ofTexture paletteTexture;
};

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
  
  std::shared_ptr<ofxAudioAnalysisClient::LocalGistClient> audioAnalysisClientPtr;
  std::shared_ptr<ofxAudioData::Processor> audioDataProcessorPtr;
  
  SomPalette somPalette { 256, 256, 0.015, 10000 };
};
