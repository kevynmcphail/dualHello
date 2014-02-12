#include "testApp.h"
using namespace ofxCv;
using namespace cv;

//-- Globals ---------------------------------------------------
bool drawCircles = true;
bool drawRects = false;
bool expressionSet = false;

int nObjects = 50;
int rectColor = 0xD62D14;
int circleColor = 0xA2D927;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);

    //Initialize box2D
    box2d.init();
    box2d.setGravity(0,0);
    box2d.createBounds();
    box2d.setFPS(30.0);
    box2d.registerGrabbing();

    //init Camera //
    cam.initGrabber(640, 480);
	tracker.setup();
	tracker.setRescale(.5);

    classifier.load("expressions");

    for(int i=0; i<=nObjects; i++){
        int xPos = ofRandom(100, 380);
        int yPos = ofRandom(100, 540);
        int rad = ofRandom(1,30);
        //circles
        circles.push_back(ofPtr<ofxBox2dCircle>(new ofxBox2dCircle));
        circles.back().get()->setPhysics(3.0, 0.53, 0.1);
        circles.back().get()->setup(box2d.getWorld(), xPos, yPos, rad);
        //rectangles
        rects.push_back(ofPtr<ofxBox2dRect>(new ofxBox2dRect));
        rects.back().get()->setPhysics(3.0, 0.53, 0.1);
        rects.back().get()->setup(box2d.getWorld(), xPos, yPos, rad*2, rad*2);
    }
}

//--------------------------------------------------------------
void testApp::update(){
    box2d.update();

    cam.update();
	if(cam.isFrameNew()) {
		if(tracker.update(toCv(cam))) {
			classifier.classify(tracker);
		}
	}
	//Change gravity based on expression
	//int expressions = classifier.size();
	int expression = classifier.getPrimaryExpression();
	cout<<expression;
    if ((expression==0)){
        box2d.setGravity(0,-20);
        drawRects = false;
        drawCircles = true;
        circleColor = 0xA2D927;
        cout<<"up";
    }
    if ((expression==1)){
        box2d.setGravity(0,20);
        drawCircles = false;
        drawRects = true;
        rectColor = 0xD62D14;
        cout<<"down";
    }
    if ((expression==2)){
        box2d.setGravity(20,0);
        drawCircles = false;
        drawRects = true;
        rectColor = 0x612851;
        cout<<"down";
    }
    if ((expression==3)){
        box2d.setGravity(-20,0);
        drawCircles = true;
        drawRects = false;
        circleColor = 0x2AA9D9;
        cout<<"down";
    }
}

//--------------------------------------------------------------
void testApp::draw(){

    box2d.drawGround();

    //Cam tracker
    ofSetColor(255);
	cam.draw(0, 0);
	tracker.draw();

    if (drawCircles==true){
        for(int i=0; i<circles.size(); i++){
            ofFill();
            ofSetHexColor(circleColor);
            circles[i].get()->draw();
        }
    }
    if (drawRects==true){
        for(int i=0; i<rects.size(); i++){
            ofFill();
            ofSetHexColor(rectColor);
            rects[i].get()->draw();
        }
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key =='a'){
        classifier.addExpression();
        classifier.addSample(tracker);
        classifier.save("expressions");
        expressionSet = true;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
