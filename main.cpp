/*
 * main.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <iostream>
#include <thread>
#include <Hand.h>
#include "DefineFunctions.h"

using namespace std;

bool readControl(Hand&hand);

//void threadTest(int nb){
//    long i = 0;
//    while(true){
//        std::cout << "Thread: " << nb << ", is working, iter: " << i++ << std::endl;
//        SLEEP_MS(1000);
//    }
//}

int main(){

    Hand hand(6);

    cout<<"Command (finger,state): \n";
    hand.continuousDataTransmitter();
    while(readControl(hand));

    return 0;
}

bool readControl(Hand&hand){
    //cout<<"Command (finger,state): ";
    char finger, state;
    cin>>finger;
    if (finger == 'q') return false;
    cin>>state;

    //cout<<finger<<" "<<state<<endl;

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