#pragma once
#include "ofMain.h"
#include "ofFbo.h"
#include "ofxBox2d.h"

#include "opencv2/opencv.hpp"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void mouseEntered(int x, int y) {};
	void mouseExited(int x, int y) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	ofxBox2d box2d;

	ofFbo fbo;
	ofFbo mask;
	ofImage frame_img;
	
	int deg_max;

	vector<shared_ptr<ofxBox2dCircle>> circles;
	vector<ofColor> circles_color;
	vector<float> circles_life;

	cv::Mat frame;
	cv::VideoCapture cap;
};