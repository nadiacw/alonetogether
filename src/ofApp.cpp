#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //SERIAL SETUP
    mySerial.listDevices();
    vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList(); //Every port
    
    
    //serial port must match what was declared in arduino Code ... 9600
    
    // for each serial connection found
    for (int i = 0; i < deviceList.size(); i++) {
        // Plant 1
        if(deviceList[i].getDeviceName().find("tty.banana-DevB") == 0) {
            while(!serialList[0].setup(i, 9600))
            {
                serialList[0].setup(i, 9600);
                cout << "thorolf connected! at serial port " << i <<endl;
            }
            //serialList[0].flush();
        }
        // Plant 2
        else if(deviceList[i].getDeviceName().find("tty.HC-06-DevB") == 0) {
            while(!serialList[1].setup(i, 9600))
            {
                serialList[1].setup(i, 9600);
                cout << "svamp connected! at serial port " << i <<endl;
            }
            //serialList[1].flush();
        }
        
        for (int i=0; i<2; i++) {
            serialList[i].flush();
        }
        plantsConnected = true;
    }
    //END SERIAL
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    //SERIAL PART
    
    if(plantsConnected){
        
        // Plant 1
         if(serialList[0].available() >= NUM_MSG_BYTES){
                 int val = serialList[0].readByte();
                 byteReceived = val;
         }
        
//        if(byteReceived == 1){
//            cout << "plant 1 touched!" << endl;
//        }
//        cout << "message from plant 1: " << byteReceived << endl;
//        cout << "------------------------" << endl;
        
        serialList[0].writeByte('0');
        if(byteReceived == 1){
            cout << "thorolf touched!" << endl;
            // Plant touched! Send message to other plant!
            serialList[1].writeByte('1');
        }
        else {
            serialList[1].writeByte('0');
        }
        
//
        serialList[0].flush();

//    // FOR EACH PLANT
//    for (int i=0; i<2; i++) {
//
//        //If recieved the flag from Arduino ... i.e "a"
//        if(serialList[i].available() >= NUM_MSG_BYTES){
//
//            //go through all received bytes
//            for(int j=0; j<NUM_MSG_BYTES; j++){
//
//                //read this byte
//                int val = serialList[i].readByte();
//
//                //store it in the array at index i
//                bytesReceived[j] = val;
//            }
//
//            numMsgRecvd++;
//
//
//            //Prints to port so you can read the vlues being recieved
//            cout << "message from plant " << i <<": bytesReceived[0] = " << bytesReceived[0] << endl;
//            //cout << "message from plant " << i <<": bytesReceived[1] = " << bytesReceived[1] << endl;
//            cout << "------------------------"<<endl;
//
//            //flush super important it clears the buffer
//            // if buffer gets full you can't keep receiving values
//            serialList[i].flush();
//
//            //Sending flag to Ardunio so that arduino knows to send the current values
//            //mySerial.writeByte('A'); //request another message!
//        }
//    }
             
    }
//    //END SERIAL
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'a'){
        serialList[1].writeByte('0');
        ofSetBackgroundColor(0,0,255);
    } else if (key == 's'){
        serialList[1].writeByte('1');
        ofSetBackgroundColor(0,255,255);
    }
    else if (key == 'z'){
        serialList[0].writeByte('0');
        ofSetBackgroundColor(225,0,255);
    } else if (key == 'x'){
        serialList[0].writeByte('1');
        ofSetBackgroundColor(0,255,0);
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
