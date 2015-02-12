/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <Socket.h>
#include <map>
#include <sstream>
#include "Hand.h"

using namespace std;

bool readControl(Hand&hand);





int main(){

    int userID;


    std::string receiverHost = "localhost";
    int startSendPort = 6868;
    SocketClient socket(receiverHost, startSendPort, UDP);



    std::ostringstream os;

    int actionType	= 0;
    float				actionPower = 0.65;
    const float 		timestamp 	= 3.454;

    os << static_cast<int>(actionType) << "," << static_cast<int>(actionPower*100.0f);


    while (true) socket.SendBytes(os.str());


//    Hand hand(6,10000);
//    cout<<"Command (finger,state): \n";
//    hand.continuousDataTransmitter();
//    while(readControl(hand));

    return 0;
}

bool readControl(Hand&hand){
    char finger, state;
    cin>>finger;
    if (finger == 'q') return false;
    cin>>state;

    switch (finger){
        case 't': // thumb
            if (state == 'o') hand.thumbOpen();
            else if (state == 'c') hand.thumbClose();
            else if (state == 'm') hand.thumbMid();
            else if (state == 'l') hand.thumbLeft();
            else if (state == 'r') hand.thumbRight();
            break;
        case 'i': // index
            if (state == 'o') hand.indexOpen();
            else if (state == 'c') hand.indexClose();
            break;
        case 'm': // middle
            if (state == 'o') hand.middleOpen();
            else if (state == 'c') hand.middleClose();
            break;
        case 'r': // ring
            if (state == 'o') hand.ringOpen();
            else if (state == 'c') hand.ringClose();
            break;
        case 'p': // pinky
            if (state == 'o') hand.pinkyOpen();
            else if (state == 'c') hand.pinkyClose();
            break;
        case 'h':
            if (state == 'o') hand.openHand();
            else if(state == 'c') hand.closeHand();
            break;
        case 'T':
            if (state == 'o') hand.turnOffMotors();
            break;
        default:
            return true;
    }
    hand.sendData();
    return true;
}