#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
/*
	ofGLWindowSettings settings;
	settings.setGLVersion(4,3);
	settings.width = 1024;
	settings.height = 768;
	settings.windowMode = OF_WINDOW;
	ofCreateWindow(settings);
*/


	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 800, OF_WINDOW);			// <-------- setup the GL context
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
