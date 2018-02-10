#include "ofApp.h"
#include "Header.h"
#include <iomanip>

#define _USE_MATH_DEFINES
#include <math.h>


//--------------------------------------------------------------
void ofApp::setup() {

	int Fbo_Memory_x = 960;
	int Fbo_Memory_y = 540;
	int serial_com_no = 4;
	int set_fps = 60;

	int fluid_Fbo_Memory_x = 960;
	int fluid_Fbo_Memory_y = 540;

	if (XML.load("mySettings.xml")) {
		Fbo_Memory_x = XML.getValue<int>("//Fbo_Memory_x");
		Fbo_Memory_y = XML.getValue<int>("//Fbo_Memory_y");

		fluid_Fbo_Memory_x = XML.getValue<int>("//fluid_Fbo_Memory_x");
		fluid_Fbo_Memory_y = XML.getValue<int>("//fluid_Fbo_Memory_y");

		serial_com_no = XML.getValue<int>("//serial_com_no");
		set_fps = XML.getValue<int>("//set_fps");
		cout << " Fbo_Memory_x " << Fbo_Memory_x << endl;
		cout << " Fbo_Memory_y " << Fbo_Memory_y << endl;
		
		cout << " fluid_Fbo_Memory_x " << fluid_Fbo_Memory_x << endl;
		cout << " fluid_Fbo_Memory_y " << fluid_Fbo_Memory_y << endl;

		cout << " serial_com_no " << serial_com_no << endl;
		cout << " set_fps " << set_fps << endl;
	}
	else {

		XML.addChild("SETTING");
		XML.addValue("Fbo_Memory_x", ofToString(Fbo_Memory_x));
		XML.addValue("Fbo_Memory_y", ofToString(Fbo_Memory_y));

		XML.addValue("fluid_Fbo_Memory_x", ofToString(fluid_Fbo_Memory_x));
		XML.addValue("fluid_Fbo_Memory_y", ofToString(fluid_Fbo_Memory_y));

		XML.addValue("serial_com_no", ofToString(serial_com_no));
		XML.addValue("set_fps", ofToString(set_fps));
		XML.save("mySettings.xml");

	}

	ofEnableAlphaBlending();
	ofSetCircleResolution(64);

	//変数の初期化
	currentFrame = 0;

	//画像のリストアップ
	long nn_max = 0;
	string fnamelist[2048];
	string dir, ff, ext;
	dir = ".\\data\\images\\";
	ext = "gif";
	get_filelist(dir, fnamelist, nn_max, ext);
	max_frame = nn_max;

	//連番がふられた画像を順番に読み込み
	for (int i = 0; i < max_frame; i++) {
		//ファイル名を一時的に格納する文字列
		char char1[2048];
		sprintf(char1, "images/%s", fnamelist[i].c_str());
		myImage_A[i].loadImage(char1);
		cout << char1 << ":load" << endl;
	}


	//背景の指定
	ofBackground(255, 255, 255);

	bShowHelp = true;

	ofSetVerticalSync(true);
	ofSetFrameRate(set_fps);

	if(set_fps==0){
	ofSetVerticalSync(false);
	ofSetFrameRate(0);
	}

	myFbo.allocate(Fbo_Memory_x, Fbo_Memory_y);
	myGlitch.setup(&myFbo);

	switch_flg = false;

	char port[11];
	sprintf(port, "\\\\.\\COM%d", serial_com_no);

	//シリアルポートを開く
	serial.setup(port, 9600);

	//時間計測
	time_s = ofGetElapsedTimeMillis();
	myGlitch_key = 5;

	myImage_view.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_left.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_right.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_left2.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_right2.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く

	myImage_sub_right_switch.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_left_switch.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く

	myImage_sub_right_switch2.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_right_switch3.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_left_switch2.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く
	myImage_sub_left_switch3.setAnchorPercent(0.5, 0.5);	//表示位置を中心に置く


	gui.setup();
	gui.add(doFullScreen.set("fullscreen (F)", false));
	gui.add(guiHide.set("guiHide", false));
	gui.add(invert_flg.set("invert", false));
	gui.add(graph_view.set("graph", false));
	gui.add(mouse_flg.set("mouse", true));
	gui.add(fluid_flg.set("fluid", true));
	gui.add(Glitch_flg.set("Glitch", true));

	gui.add(fluid_fall.setup("fluid_fall", 1.0, 1, 20));
	gui.add(fluid_num.setup("fluid_num", 2.0, 1, 2));

	gui.add(p1.setup("fluid_speed", 2.0, 0, 5.0));
	gui.add(p2.setup("fluid_circle_x", 0.4, 0, 1.0));
	gui.add(p3.setup("fluid_circle_y", 0.4, 0, 1.0));
	gui.add(p4.setup("fluid_power", 5.0, 0, 20.0));

	gui.add(p12.setup("fluid_diss", 0.98, 0.8, 1.0));
	gui.add(p13.setup("fluid_vel", 0.85, 0.8, 1.0));
	gui.add(p14.setup("fluid_Gravity", -0.25, -1.0, 1.0));


	gui.add(p5.setup("wait_delay", 0.5, 0, 1.0));
	gui.add(p6.setup("contrast", 0.5, 0, 1.0));
	gui.add(p7.setup("scale_main", 0.7, 0, 1.0));
	gui.add(p8.setup("scale_sub", 0.25, 0, 1.0));
	gui.add(p9.setup("acc_rate", 0.1, 0, 0.5));
	gui.add(p10.setup("thresh_acc", 50, 0, 1000));
	gui.add(p11.setup("thresh_delay", 1.0, 0, 1.0));

	//gui.add(p15.setup("Plot_max", 100, 0, 1000));
	//gui.add(p16.setup("Plot_min", 0, 0, 1000));
	//gui.add(p17.setup("Plot_speed", 1.0, 0, 10.0));



	//フルスクリーンの指定
	doFullScreen.addListener(this, &ofApp::setFullScreen);

	gui.loadFromFile("gui_settings.xml");


	//fluid
	fluid.allocate(fluid_Fbo_Memory_x, fluid_Fbo_Memory_y, 0.5);
	fluid.dissipation = p12;
	fluid.velocityDissipation = p13;
	fluid.setGravity(ofVec2f(0.0, p14));

	fluid.begin();
	ofSetColor(0, 0, 255);
	fluid.setUseObstacles(false);
	fluid.end();


	int x = ofGetWidth() - 300;
	int y = 0;

	plotter1 = new ofxDatGuiValuePlotter("raw data", 0, 500);
	plotter1->setSpeed(2.0f);
	plotter1->setDrawMode(ofxDatGuiGraph::LINES);
	plotter2 = new ofxDatGuiValuePlotter("filterd ", 0, 500);
	plotter2->setSpeed(2.0f);
	plotter2->setDrawMode(ofxDatGuiGraph::LINES);

	component = plotter1;
	component->setPosition(x, y);
	components.push_back(component);
	component = plotter2;
	component->setPosition(x, y + 80);
	components.push_back(component);


}

//--------------------------------------------------------------

void ofApp::myGlitch_on(int key) {
	


	//	if (key == 1) myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
	//	else if (key == 2) myGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
	if (key == 1) myGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
	else if (key == 2) myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
	else if (key == 3) myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
	else myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);


}

void ofApp::myGlitch_off(int &key) {

	//	if (key == 1) myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
	//	else if (key == 2) myGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
	if (key == 1) myGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
	else if (key == 2) myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
	else if (key == 3) myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
	else myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);

	//key = rand() % (3 + 1);

}

void ofApp::switch_ofImage(double aa, ofImage &pre_image, ofImage &next_image, ofImage &output) {

	unsigned char *pixels_1 = pre_image.getPixels().getData();
	unsigned char *pixels_2 = next_image.getPixels().getData();
	unsigned char *pixels_o = output.getPixels().getData();

	//高さと幅の取得
	int w1 = pre_image.getWidth();
	int h1 = pre_image.getHeight();

	int w2 = next_image.getWidth();
	int h2 = next_image.getHeight();


	for (int i = 0; i < h1; i++) {
		for (int j = 0; j < w1; j++) {
			if ((j < w2) && (i < h2)) {
				int r1 = pixels_1[i * 3 * w1 + j * 3];		//red
				int g1 = pixels_1[i * 3 * w1 + j * 3 + 1];	//green
				int b1 = pixels_1[i * 3 * w1 + j * 3 + 2];	//blue

				int r2 = pixels_2[i * 3 * w2 + j * 3];		//red
				int g2 = pixels_2[i * 3 * w2 + j * 3 + 1];	//green
				int b2 = pixels_2[i * 3 * w2 + j * 3 + 2];	//blue

				pixels_o[i * 3 * w1 + j * 3] = aa*r2 + (1.0 - aa)*r1;		//red
				pixels_o[i * 3 * w1 + j * 3 + 1] = aa*g2 + (1.0 - aa)*g1;	//green
				pixels_o[i * 3 * w1 + j * 3 + 2] = aa*b2 + (1.0 - aa)*b1;	//blue
			}
		}
	}
	output.update();

}


void ofApp::fade_ofImage(ofImage &image, double aa) {

	unsigned char *pixels_1 = image.getPixels().getData();

	//高さと幅の取得
	int w1 = image.getWidth();
	int h1 = image.getHeight();

	for (int i = 0; i < h1; i++) {
		for (int j = 0; j < w1; j++) {
			int r1 = pixels_1[i * 3 * w1 + j * 3];		//red
			int g1 = pixels_1[i * 3 * w1 + j * 3 + 1];	//green
			int b1 = pixels_1[i * 3 * w1 + j * 3 + 2];	//blue

			//int r2 = (255 - r1)*aa;
			//int g2 = (255 - g1)*aa;
			//int b2 = (255 - b1)*aa;
			//if (r2 > 255)r2 = 0;
			//if (g2 > 255)g2 = 0;
			//if (b2 > 255)b2 = 0;
			//if (r2 < 0)r2 = 0;
			//if (g2 < 0)g2 = 0;
			//if (b2 < 0)b2 = 0;
				//pixels_1[i * 3 * w1 + j * 3] = r2 + 255;		//red
				//pixels_1[i * 3 * w1 + j * 3 + 1] = g2 + 255;	//green
				//pixels_1[i * 3 * w1 + j * 3 + 2] = b2+255;	//blue

			pixels_1[i * 3 * w1 + j * 3] = aa*r1 + (1.0 - aa) * 255.0;		//red
			pixels_1[i * 3 * w1 + j * 3 + 1] = aa*g1 + (1.0 - aa) * 255.0;	//green
			pixels_1[i * 3 * w1 + j * 3 + 2] = aa*b1 + (1.0 - aa) * 255.0;	//blue
		}
	}
	image.update();

}


void ofApp::update() {

	wait_switch = p5;			//キーボードからの応答遅延時間
	contrast = p6;				//サブウィンドウの輝度
	scale_main = p7;			//メインウィンドウのサイズ
	scale_sub = p8;			//サブウィンドウのサイズ


	int nextFrame = currentFrame + 1;
	int preFrame = currentFrame - 1;
	if (nextFrame >= max_frame) nextFrame = 0;
	if (preFrame < 0) preFrame = max_frame - 1;

	int next_nextFrame = nextFrame + 1;
	int pre_preFrame = preFrame - 1;
	if (next_nextFrame >= max_frame) next_nextFrame = 0;
	if (pre_preFrame < 0) pre_preFrame = max_frame - 1;

	int next_next_nextFrame = next_nextFrame + 1;
	if (next_next_nextFrame >= max_frame) next_next_nextFrame = 0;


	static int check_flg = switch_flg;
	if (switch_flg) {	//切り返しの動作
		if (check_flg == false) {
			time_s = ofGetElapsedTimeMillis();
		}

		long int now_time = ofGetElapsedTimeMillis();
		switch_flg_rate = (now_time - time_s) / (wait_switch*1000.0);
		if ((switch_flg_rate > 1.0)) {
			switch_flg_rate = 0.0;
			switch_flg = 0;
			currentFrame++;
			if (currentFrame + 1 >= max_frame) currentFrame = 0;
			myGlitch_off(myGlitch_key);
		}
		else {
			myGlitch_on(myGlitch_key);
		}
	}


	//濃淡を逆転する
	if (invert_flg) {
		myGlitch.setFx(OFXPOSTGLITCH_INVERT, true);
	}
	else {
		myGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
	}

	check_flg = switch_flg;
	//double contrast = 0.5;

	myImage_view.clone(myImage_A[currentFrame]);
	myImage_sub_left.clone(myImage_A[nextFrame]);
	myImage_sub_right.clone(myImage_A[preFrame]);
	myImage_sub_left2.clone(myImage_A[next_nextFrame]);
	myImage_sub_right2.clone(myImage_A[pre_preFrame]);
	fade_ofImage(myImage_sub_left, contrast);
	fade_ofImage(myImage_sub_right, contrast);
	fade_ofImage(myImage_sub_left2, contrast);
	fade_ofImage(myImage_sub_right2, contrast);


	myFbo.begin();
	ofClear(255, 255, 255, 255);
	//double scale_main = 0.7;
	//double scale_sub = 0.25;

	float winW = myFbo.getWidth(), winH = myFbo.getHeight();
	//float winW = myFbo.getWidth(), winH = myFbo.getHeight();
	float imgW = myImage_view.getWidth();
	float imgH = myImage_view.getHeight();

	float ratioW = winW / imgW,
		ratioH = winH / imgH;

	if (ratioW < ratioH)
		ratioMin = ratioW;
	else
		ratioMin = ratioH;

	int shift_size_w = winW / 4.0;
	int shift_size_h = winH/3.0;

	double center_x_0 = winW / 2;
	double center_y_0 = winH / 2;

	double left_x_0 = winW / 2 - shift_size_w / 2;
	double left_y_0 = winH / 2 - shift_size_h;

	double left_x_1 = left_x_0 - shift_size_w;
	double left_y_1 = left_y_0;


	double right_x_0 = winW / 2 + shift_size_w / 2;
	double right_y_0 = winH / 2 + shift_size_h;

	double right_x_1 = right_x_0 + shift_size_w;
	double right_y_1 = right_y_0;


	if (switch_flg) {	//切り返しの動作
		int next_nextFrame = nextFrame + 1;
		int pre_preFrame = preFrame - 1;
		if (next_nextFrame >= max_frame) next_nextFrame = 0;
		if (pre_preFrame < 0) pre_preFrame = max_frame - 1;

		double left_scale = switch_flg_rate*scale_main + (1.0 - switch_flg_rate)*scale_sub;
		double right_scale = switch_flg_rate*scale_sub + (1.0 - switch_flg_rate)*scale_main;

		double left_x = switch_flg_rate*center_x_0 + (1.0 - switch_flg_rate)*left_x_0;
		double left_y = switch_flg_rate*center_y_0 + (1.0 - switch_flg_rate)*left_y_0;
		double left_w = ratioMin*imgW*left_scale;
		double left_h = ratioMin*imgH*left_scale;

		double right_x = switch_flg_rate*right_x_0 + (1.0 - switch_flg_rate)*center_x_0;
		double right_y = switch_flg_rate*right_y_0 + (1.0 - switch_flg_rate)*center_y_0;
		double right_w = ratioMin*imgW*right_scale;
		double right_h = ratioMin*imgH*right_scale;

		double left2_x = switch_flg_rate*left_x_0 + (1.0 - switch_flg_rate)*left_x_1;
		double left2_y = switch_flg_rate*left_y_0 + (1.0 - switch_flg_rate)*left_y_1;
		double left2_w = ratioMin*imgW*scale_sub;
		double left2_h = ratioMin*imgH*scale_sub;

		double right2_x = switch_flg_rate*right_x_1 + (1.0 - switch_flg_rate)*right_x_0;
		double right2_y = switch_flg_rate*right_y_1 + (1.0 - switch_flg_rate)*right_y_0;
		double right2_w = ratioMin*imgW*scale_sub;
		double right2_h = ratioMin*imgH*scale_sub;

		myImage_sub_right_switch.clone(myImage_view);

		fade_ofImage(myImage_sub_right_switch, (contrast));
		myImage_sub_left_switch.clone(myImage_sub_left);

		myImage_sub_right_switch2.clone(myImage_sub_right);
		myImage_sub_left_switch2.clone(myImage_sub_left2);

		myImage_sub_left_switch3.clone(myImage_A[next_next_nextFrame]);

		fade_ofImage(myImage_sub_left_switch3, (switch_flg_rate*contrast));

		myImage_sub_left_switch3.draw(left_x_1, left_y_1, ratioMin*imgW*scale_sub, ratioMin*imgH*scale_sub);

		myImage_sub_left_switch.draw(left_x, left_y, left_w, left_h);
		myImage_sub_right_switch.draw(right_x, right_y, right_w, right_h);

		myImage_sub_left_switch2.draw(left2_x, left2_y, left2_w, left2_h);
		myImage_sub_right_switch2.draw(right2_x, right2_y, right2_w, right2_h);

	}
	else {

		myImage_view.draw(center_x_0, center_y_0, ratioMin*imgW*scale_main, ratioMin*imgH*scale_main);
		myImage_sub_right.draw(right_x_0, right_y_0, ratioMin*imgW*scale_sub, ratioMin*imgH*scale_sub);
		myImage_sub_left.draw(left_x_0, left_y_0, ratioMin*imgW*scale_sub, ratioMin*imgH*scale_sub);

		myImage_sub_right2.draw(right_x_1, right_y_1, ratioMin*imgW*scale_sub, ratioMin*imgH*scale_sub);
		myImage_sub_left2.draw(left_x_1, left_y_1, ratioMin*imgW*scale_sub, ratioMin*imgH*scale_sub);

	}


	myFbo.end();

	//シリアル通信で加速度センサの読み取り
	string str = ofxGetSerialString(serial, '\n');
	static double val_old = 0;
	static double val_ave = 0;

	double acc_rate = p9;				//加速度の加重平均
	double thresh_acc = p10;			//加速度センサの検出閾値
	double thresh_delay = p11;			//加速度センサの応答遅延時間

	double value = 0;
	if (str.length() > 0) {
		value = std::atof(str.c_str());					// 数値化
		val_ave = (1 - acc_rate)*val_ave + acc_rate *value;		//移動平均をとる
		val_old = value;

		long int time_now = ofGetElapsedTimeMillis();			//時間を計測する
		int delay = time_now - time_s;
		if ((fabs(value - val_ave) > thresh_acc) && (delay > thresh_delay * 1000)) {		//前回よりも時間がたっておらず、加速度入力があれば
			switch_flg = true;
			time_s = ofGetElapsedTimeMillis();
		}
		acc_data_raw = value;
		acc_data_filtered = fabs(value - val_ave);
		plotter1->setValue(acc_data_raw);
		plotter2->setValue(acc_data_filtered);
	}

	double fluid_speed = p1;
	double fluid_circle_x = p2;
	double fluid_circle_y = p3;
	double fluid_power = p4;

	int width = fluid.getWidth();
	int height = fluid.getHeight();

	double x = width * fluid_circle_x * sin(fluid_speed*ofGetElapsedTimef()) + width / 2.0;
	double y = height * fluid_circle_y * cos(fluid_speed*ofGetElapsedTimef()) + height / 2.0;


	ofPoint m1 = ofPoint(x, y);
	x = width * fluid_circle_x * sin(fluid_speed*ofGetElapsedTimef() + M_PI) + width / 2.0;
	y = height * fluid_circle_y * cos(fluid_speed*ofGetElapsedTimef() + M_PI) + height / 2.0;

	ofPoint d1 = (m1 - oldM1)*10.0;
	oldM1 = m1;
	ofPoint c1 = ofPoint(width * 0.5, height * 0.5) - m1;
	c1.normalize();

	ofPoint m2 = ofPoint(x, y);
	ofPoint d2 = (m2 - oldM2)*10.0;
	oldM2 = m2;
	ofPoint c2 = ofPoint(width * 0.5, height * 0.5) - m2;
	c2.normalize();

	
	fluid.addTemporalForce(m1, d1, ofFloatColor(c1.x, c1.y, 0.5)*sin(ofGetElapsedTimef()), fluid_power);
	if(fluid_num>= 2)
		fluid.addTemporalForce(m2, d2, ofFloatColor(c2.x, c2.y, 0.5)*sin(ofGetElapsedTimef()), fluid_power);



	fluid.dissipation = p12;
	fluid.velocityDissipation = p13;
	fluid.setGravity(ofVec2f(0.0, p14));

	static int fluid_cnt = 0;
	int fluid_fall_int = fluid_fall;
	fluid_cnt++;
	if (fluid_flg) {
		if(fluid_cnt%fluid_fall_int ==0)	fluid.update();
	}

	for (int i = 0; i < components.size(); i++) components[i]->update();

	int xx = ofGetWidth() - 300;
	int yy = 0;
	double speed = p11;

	double max = p9;
	double min = p10;
	components[0]->setPosition(xx, yy);
	components[1]->setPosition(xx, yy + 80);


}

//--------------------------------------------------------------
void ofApp::draw() {
	/* Apply effects */
	if (Glitch_flg )myGlitch.generateFx();

	/* draw effected view */
	ofSetColor(255);	//背景色の指定

	myFbo.draw(0, 0, ofGetWidth(), ofGetHeight());
	if (fluid_flg) fluid.draw(0,0,ofGetWidth(), ofGetHeight());

	if (!graph_view) {
		for (int i = 0; i < components.size(); i++) components[i]->draw();
	}

	
	if (!guiHide) {
		gui.draw();


		string info = "";
		info += "HannyaShingyo\n";
		info += "H key : Hide or show \n";
		info += "space/z:next\n";
		info += "1 - 0 : Apply glitch effects.\n";
		info += "q - u : Apply color remap effects.\n";
		
		info += "acc_data_raw___:";
		info += std::to_string(acc_data_raw);
		info += "\n";

		info += "acc_data_filter:";
		info += std::to_string(acc_data_filtered);
		info += "\n";

		info += "fps:";
		info += ofToString(ofGetFrameRate());
		info += "\n";


		ofSetColor(240, 240, 240);
		ofRect(30, ofGetHeight() - 120, 300, 120);
		ofSetColor(0);
		ofDrawBitmapString(info, 40, ofGetHeight() - 100);
		ofSetColor(255);


	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, true);
	if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW, true);
	if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER, true);
	if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, true);
	if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST, true);
	if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE, true);
	if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE, true);
	if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, true);
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL, true);
	if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT, true);

	if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, true);
	if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, true);
	if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, true);
	if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, true);
	if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, true);
	if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, true);
	if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, true);

	if (key == 'z') switch_flg = true;
	if (key == 32) switch_flg = true;
	if (key == 'x') {
		currentFrame++;
		if (currentFrame >= max_frame)currentFrame = 0;
		//page_status = 0;

	}
	if (key == 'h') {
		guiHide = !guiHide;
	}
	if (key == 'f') {
		doFullScreen.set(!doFullScreen.get());
	}

	if (key == 's') {
		gui.saveToFile("gui_settings.xml");
	}
	if (key == 'l') {
		gui.loadFromFile("gui_settings.xml");
	}



}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == '1') myGlitch.setFx(OFXPOSTGLITCH_CONVERGENCE, false);
	if (key == '2') myGlitch.setFx(OFXPOSTGLITCH_GLOW, false);
	if (key == '3') myGlitch.setFx(OFXPOSTGLITCH_SHAKER, false);
	if (key == '4') myGlitch.setFx(OFXPOSTGLITCH_CUTSLIDER, false);
	if (key == '5') myGlitch.setFx(OFXPOSTGLITCH_TWIST, false);
	if (key == '6') myGlitch.setFx(OFXPOSTGLITCH_OUTLINE, false);
	if (key == '7') myGlitch.setFx(OFXPOSTGLITCH_NOISE, false);
	if (key == '8') myGlitch.setFx(OFXPOSTGLITCH_SLITSCAN, false);
	if (key == '9') myGlitch.setFx(OFXPOSTGLITCH_SWELL, false);
	if (key == '0') myGlitch.setFx(OFXPOSTGLITCH_INVERT, false);
	if (key == 'q') myGlitch.setFx(OFXPOSTGLITCH_CR_HIGHCONTRAST, false);
	if (key == 'w') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUERAISE, false);
	if (key == 'e') myGlitch.setFx(OFXPOSTGLITCH_CR_REDRAISE, false);
	if (key == 'r') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENRAISE, false);
	if (key == 't') myGlitch.setFx(OFXPOSTGLITCH_CR_BLUEINVERT, false);
	if (key == 'y') myGlitch.setFx(OFXPOSTGLITCH_CR_REDINVERT, false);
	if (key == 'u') myGlitch.setFx(OFXPOSTGLITCH_CR_GREENINVERT, false);


	//if (key == 'a') switch_flg = false;

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	if(mouse_flg)switch_flg = true;

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}




string ofApp::ofxGetSerialString(ofSerial &serial, char until) {
	static string str;
	stringstream ss;
	char ch;
	int ttl = 1000;
	while ((ch = serial.readByte()) > 0 && ttl-- > 0 && ch != until) {
		ss << ch;
	}
	str += ss.str();
	if (ch == until) {
		string tmp = str;
		str = "";
		return ofxTrimString(tmp);
	}
	else {
		return "";
	}
}
// trim trailing spaces
string ofApp::ofxTrimStringRight(string str) {
	size_t endpos = str.find_last_not_of(" \t\r\n");
	return (string::npos != endpos) ? str.substr(0, endpos + 1) : str;
}

// trim trailing spaces
string ofApp::ofxTrimStringLeft(string str) {
	size_t startpos = str.find_first_not_of(" \t\r\n");
	return (string::npos != startpos) ? str.substr(startpos) : str;
}

string ofApp::ofxTrimString(string str) {
	return ofxTrimStringLeft(ofxTrimStringRight(str));;
}
