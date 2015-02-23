/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <thread>
#include <Hand.h>

using namespace std;

bool readControl(Hand&hand);

int main(){

    Hand hand(6,10000);

    cout<<"Command (finger,state): \n";
    hand.continuousDataTransmitter();
    while(readControl(hand));

    return 0;
}

bool readControl(Hand&hand){
    char finger;
    int state;
    cin>>finger;
    if (finger == 'q') return false;
    cin>>state;

    switch (finger){
        case 't': // thumb
            if (state == 1) hand.thumbOpen();
            else if (state == 2) hand.thumbClose();
            break;
        case 'a':
            hand.thumbPWM(uint8_t(state));
            break;
        case 'i': // index
            if (state == 1) hand.indexOpen();
            else if (state == 2) hand.indexClose();
            break;
        case 'm': // middle
            if (state == 1) hand.middleOpen();
            else if (state == 2) hand.middleClose();
            break;
        case 'r': // ring
            if (state == 1) hand.ringOpen();
            else if (state == 2) hand.ringClose();
            break;
        case 'p': // pinky
            if (state == 1) hand.pinkyOpen();
            else if (state == 2) hand.pinkyClose();
            break;
        case 'h':
            if (state == 1) hand.openHand();
            else if(state == 2) hand.closeHand();
            break;
        case 'T':
            if (state == 2) hand.turnOffMotors();
            break;
        default:
            return true;
    }
    return true;
}