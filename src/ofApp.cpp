#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    mySerial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
    
    //serial port must match what was declared in arduino Code ... 9600
    mySerial.setup(5, 9600);
    
    mySerial.flush(); //flush the serial port once before we start
    //END SERIAL

}

//--------------------------------------------------------------
void ofApp::update(){
    
    //SERIAL PART
    //If recieved the flag from Arduino ... i.e "a"
    if(mySerial.available() >= NUM_MSG_BYTES){
        
        //go through all received bytes
        for(int i=0; i<NUM_MSG_BYTES; i++){
            
            //read this byte
            int val = mySerial.readByte();
            
            //store it in the array at index i
            bytesReceived[i] = val;
        }
        
        numMsgRecvd++;
        
        
        //Prints to port so you can read the vlues being recieved
        cout << "bytesReceived[0] = " << bytesReceived[0] << endl;
        cout << "bytesReceived[1] = " << bytesReceived[1] << endl;
        cout << "------------------------"<<endl;
        
        //flush super important it clears the buffer
        // if buffer gets full you can't keep receiving values
        mySerial.flush();
        
        //Sending flag to Ardunio so that arduino knows to send the current values
        //mySerial.writeByte('A'); //request another message!
    }
    //END SERIAL

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 'a'){
        mySerial.writeByte('0');
        ofSetBackgroundColor(0,0,255);
    } else if (key == 's'){
        mySerial.writeByte('1');
        ofSetBackgroundColor(0,255,255);
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
