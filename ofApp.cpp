#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofSetCircleResolution(60);
	ofSetLineWidth(2.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->box2d.init();
	this->box2d.setGravity(0, 5);
	//this->box2d.createGround(ofVec2f(0, ofGetWidth()), ofVec2f(ofGetWidth(), ofGetHeight()));
	this->box2d.createBounds();
	this->box2d.setFPS(60);
	this->box2d.registerGrabbing();

	this->cap = cv::VideoCapture("move.mp4");
	this->frame_img.allocate(this->cap.get(CV_CAP_PROP_FRAME_WIDTH), this->cap.get(CV_CAP_PROP_FRAME_HEIGHT), OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->frame_img.getHeight(), this->frame_img.getWidth(), CV_MAKETYPE(CV_8UC3, this->frame_img.getPixels().getNumChannels()), this->frame_img.getPixels().getData(), 0);

	this->mask.allocate(this->frame_img.getWidth(), this->frame_img.getHeight(), GL_RGB);
	this->mask.begin();
	ofFill();
	ofColor(255);
	ofDrawCircle(ofGetWidth() / 2, ofGetHeight() / 3 * 2 - 190, 145);
	this->mask.end();

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
	this->deg_max = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 4 == 0 && !ofGetMousePressed()){
		float radius = 145;
		float power = 150;
		float tmp_deg = 0;

		for (int deg = 0; deg < this->deg_max; deg += 8) {
			tmp_deg = deg + ofGetFrameNum() * 3;

			this->circles.push_back(shared_ptr<ofxBox2dCircle>(new ofxBox2dCircle));
			this->circles.back().get()->setPhysics(3.0, 0.53, 0.1);
			this->circles.back().get()->setup(this->box2d.getWorld(), radius * cos(tmp_deg * DEG_TO_RAD) + ofGetWidth() / 2, radius * sin(tmp_deg * DEG_TO_RAD) + ofGetHeight() / 3 * 2, 3);
			this->circles.back().get()->addForce(ofVec2f(power * cos((tmp_deg + 270)  * DEG_TO_RAD), power * sin((tmp_deg + 270) * DEG_TO_RAD)), 1.0);

			ofColor color = ofColor(255, 255, 0);
			this->circles_color.push_back(color);

			this->circles_life.push_back(255);
		}

		if (this->deg_max < 340) {
			this->deg_max += 5;
		}
	}
	
	if(ofGetMousePressed() && this->deg_max >= 0){
		this->deg_max -= 3;
		if (this->deg_max < 280){
			this->deg_max = 0;
		}
	}

	for (int i = 0; i < this->circles.size(); i++) {
		if (this->circles_life[i] < 0) {
			this->circles[i].get()->destroy();
			this->circles.erase(this->circles.begin() + i);
			this->circles_color.erase(this->circles_color.begin() + i);
			this->circles_life.erase(this->circles_life.begin() + i);
		}
	}

	this->box2d.update();

	this->fbo.begin();
	ofFill();
	ofClear(0);

	if (this->deg_max > 280) {
		this->cap >> this->frame;
		if (this->frame.empty()) {
			this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
			this->frame = cv::Mat(this->frame_img.getHeight(), this->frame_img.getWidth(), CV_MAKETYPE(CV_8UC3, this->frame_img.getPixels().getNumChannels()), this->frame_img.getPixels().getData(), 0);
			this->cap >> this->frame;
		}
	
		cv::cvtColor(this->frame, this->frame, CV_RGB2BGR);
		ofSetColor(255, ofMap(this->deg_max, 280, 300, 0, 128));
		this->frame_img.update();
		this->frame_img.getTextureReference().setAlphaMask(this->mask.getTexture());
		this->frame_img.draw(0, 190);

	}

	for (int i = 0; i < this->circles.size(); i++) {
		ofSetColor(this->circles_color[i], this->circles_life[i] + 128);
		//ofDrawCircle(this->circles[i].get()->getPosition(), 3);
		ofDrawLine(this->circles[i].get()->getPosition(), this->circles[i].get()->getPosition() - this->circles[i].get()->getVelocity());

		this->circles_life[i] -= 10.f;
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->fbo.draw(0, 0);
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(512, 512, OF_WINDOW);
	ofRunApp(new ofApp());
}