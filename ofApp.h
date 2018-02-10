#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdlib> // std::atoi, std::strtol

#include "ofMain.h"
#include "ofxPostGlitch.h"
#include "ofxGui.h"
#include "ofxFluid.h"
#include "ofxDatGui.h"


#define FRAMENUM 1024 //ì«Ç›çûÇﬁâÊëúÇÃñáêî

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void myGlitch_off(int &num);
	void myGlitch_on(int num);
	void switch_ofImage(double aa, ofImage &pre_image, ofImage &next_image, ofImage &output);

	bool	bShowHelp;
	void ofApp::fade_ofImage(ofImage &image, double aa);
	ofImage myImage_view;
	ofImage myImage_sub_left;
	ofImage myImage_sub_right;
	ofImage myImage_sub_left2;
	ofImage myImage_sub_right2;


	ofImage myImage_sub_left_switch;
	ofImage myImage_sub_right_switch;
	ofImage myImage_sub_left_switch2;
	ofImage myImage_sub_left_switch3;
	ofImage myImage_sub_right_switch2;
	ofImage myImage_sub_right_switch3;

	ofImage myImage_A[FRAMENUM];
	int currentFrame;
	double	ratioMin;
	int max_frame;
	ofParameter<bool>	doFullScreen;
	void				setFullScreen(bool& _value) { ofSetFullscreen(_value); }

	ofFbo			myFbo;

	ofTexture		texture;
	ofxPostGlitch	myGlitch;
	int switch_flg;

	double switch_flg_rate;


	ofSerial serial;

	string ofxGetSerialString(ofSerial &serial, char until);
	string ofxTrimStringRight(string str);
	string ofxTrimStringLeft(string str);
	string ofxTrimString(string str);
	//		string str;

	long int  time_s;
	int myGlitch_key;


	ofXml XML;
	ofxFluid fluid;

	//ê›íËÉpÉâÉÅÅ[É^
	double wait_switch ;
	double contrast ;
	double scale_main ;
	double scale_sub;

	double acc_data_raw;
	double acc_data_filtered;

	ofVec2f oldM1;
	ofVec2f oldM2;


	//GUI

	ofxFloatSlider radius;
	ofxPanel gui;
	//bool bHide;
	ofParameter<bool> invert_flg;
	ofParameter<bool> graph_view;
	ofParameter<bool> guiHide;
	ofParameter<bool> mouse_flg;
	ofParameter<bool> fluid_flg;
	ofParameter<bool> Glitch_flg;

	ofxFloatSlider p1, p2, p3, p4, p5, p6, p7, p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20;
	ofxFloatSlider fluid_fall;
	ofxFloatSlider fluid_num;

	ofxDatGuiValuePlotter* plotter1;
	ofxDatGuiValuePlotter* plotter2;
	vector<ofxDatGuiComponent*> components;
	ofxDatGuiComponent* component;


};
