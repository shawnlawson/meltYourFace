#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // TODO add control to switch between mic and system audio
    fft.setup(2048);

    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofBackground(66,66,66);

    //initialize the video grabber
    vidGrabber.setVerbose(true);
    vidGrabber.setup(320,240);

    //store the width and height for convenience
    int width = vidGrabber.getWidth();
    int height = vidGrabber.getHeight();

    //add one vertex to the mesh for each pixel
    for (int y = 0; y < height; y++){
        for (int x = 0; x<width; x++){
            mainMesh.addVertex(ofPoint(x,y,0));    // mesh index = x + y*width
            // this replicates the pixel array within the camera bitmap...
            mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
        }
    }

    for (int y = 0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            mainMesh.addIndex(x+y*width);                // 0
            mainMesh.addIndex((x+1)+y*width);            // 1
            mainMesh.addIndex(x+(y+1)*width);            // 10

            mainMesh.addIndex((x+1)+y*width);            // 1
            mainMesh.addIndex((x+1)+(y+1)*width);        // 11
            mainMesh.addIndex(x+(y+1)*width);            // 10
        }
    }

    //this is an annoying thing that is used to flip the camera
    cam.setScale(1,-1,1);


    //this determines how much we push the meshes out
    extrusionAmount = 70.0;
}

//--------------------------------------------------------------
void ofApp::update(){
    fft.update();
    float value = fft.getBins()[50]; // TODO we want this to be the amplitude
//    float value = fft.getAmplitude();

    //grab a new frame
    vidGrabber.update();

    //update the mesh if we have a new frame
    if (vidGrabber.isFrameNew()){
        for (int i=0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++){

            ofFloatColor sampleColor(vidGrabber.getPixels()[i*3]/255.f,                // r
                                     vidGrabber.getPixels()[i*3+1]/255.f,            // g
                                     vidGrabber.getPixels()[i*3+2]/255.f);            // b

            //now we get the vertex at this position
            ofVec3f tmpVec = mainMesh.getVertex(i);

            //melt a little if the sound is loud enough
//            if (value > 0.06) { // microphone
            if (value > 0.02) { // loopback
                int yInitial = tmpVec.y;
                tmpVec.y += (sampleColor.getBrightness() * value * 6) / 3;
                tmpVec.y = (int)tmpVec.y % (int)vidGrabber.getHeight(); // make the bottom pixels jump to the top
                tmpVec.z += (tmpVec.y - yInitial) / 5;
//                tmpVec.y += (sampleColor.getBrightness()) / 8;
            }
            mainMesh.setVertex(i, tmpVec);

            mainMesh.setColor(i, sampleColor);
        }
    }

    //vertically center the camera
    float rotateAmount = 0;

    //move the camera around the mesh
    ofVec3f camDirection(0,0,1);
    ofVec3f centre(vidGrabber.getWidth()/2.f,vidGrabber.getHeight()/2.f, 255/2.f);
    ofVec3f camDirectionRotated = camDirection.getRotated(rotateAmount, ofVec3f(1,0,0));
    ofVec3f camPosition = centre + camDirectionRotated * extrusionAmount;

    cam.setPosition(camPosition);
    cam.lookAt(centre);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //we have to disable depth testing to draw the video frame
    ofDisableDepthTest();
    //    vidGrabber.draw(20,20);

    //but we want to enable it to show the mesh
    ofEnableDepthTest();
    cam.begin();

    //You can either draw the mesh or the wireframe
    // mainMesh.drawWireframe();
    mainMesh.drawFaces();
    cam.end();

    //draw framerate for fun
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(msg, 10, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 'f':
            ofToggleFullscreen();
            break;
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