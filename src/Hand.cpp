/*
 * Hand.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include <thread>
#include <iostream>
#include <bitset>
#include "Hand.h"
#include "Config.h"
#include "DefineFunctions.h"


Hand::Hand(int portNumber): portCOM(portNumber,9600), ifContinuous(false) {
    portCOM.open();
    packet[0] = 0x0F;
    packet[1] = 0xF0;
}

Hand::~Hand() {}

void Hand::sendData() {
    if (ifContinuous) return;
    uint8_t packet[2];
    // mutex lock
    packet[0] = Hand::packet[0];
    packet[1] = Hand::packet[1];
    // mutex unlock
    //portCOM.sendBlock(packet,2);
    portCOM.sendByte(packet[0]); portCOM.sendByte(packet[1]);
}

void dataSender(Hand *hand){
    uint8_t packet[2];
    uint8_t finger, state;
    while (true){
        try {
            hand->mtx.lock();
            packet[0] = hand->packet[0];
            packet[1] = hand->packet[1];
            hand->mtx.unlock();

            hand->portCOM.sendByte(packet[0]);
            hand->portCOM.sendByte(packet[1]);


            finger = hand->portCOM.readByte();
            state = hand->portCOM.readByte();

            std::bitset<8> x(finger);
            std::bitset<8> y(state);

//            std::cout << "finger: " << x << ", state: " << y << std::endl;

            if (finger != packet[0] && state != packet[1]){
//                ERROR_COM(hand->portCOM.getPortNumber(), "Bad feedback packet.");
                SLEEP_MS(10);
                continue;
            }

            SLEEP_MS(100);
        } catch(bool e) {}
    }
}


void Hand::continuousDataTransmitter() {
    ifContinuous = true;
    continuousDataSender = std::thread(dataSender, this);
    continuousDataSender.detach();
}

void Hand::setPacket(uint8_t finger, uint8_t state) {
    finger = 0x81 | finger;
    mtx.lock();
    packet[0] = finger;
    packet[1] = state;
    mtx.unlock();
}


void Hand::openHand() {
    setPacket((1<<PINKY_OC) | (1<< RING_OC) | (1<< MIDDLE_OC) | (1<< INDEX_OC) | (1<< THUMB_OC), OPEN);
}

void Hand::closeHand() {
    setPacket((1<<PINKY_OC)| (1<< RING_OC) | (1<< MIDDLE_OC) | (1<< INDEX_OC) | (1<< THUMB_OC), CLOSE);
}


void Hand::pinkyOpen() { setPacket(1<< PINKY_OC, OPEN); }
void Hand::pinkyClose() { setPacket(1<< PINKY_OC, CLOSE); }

void Hand::ringOpen() { setPacket(1<< RING_OC, OPEN); }
void Hand::ringClose() { setPacket(1<< RING_OC, CLOSE);  }

void Hand::indexOpen() { setPacket(1<< INDEX_OC, OPEN); }
void Hand::indexClose() { setPacket(1<< INDEX_OC, CLOSE); }

void Hand::middleOpen() { setPacket(1<< MIDDLE_OC, OPEN); }
void Hand::middleClose() { setPacket(1<< MIDDLE_OC, CLOSE); }

void Hand::thumbOpen() { setPacket(1<< THUMB_OC, OPEN); }
void Hand::thumbClose() { setPacket(1<< THUMB_OC, CLOSE); }

void Hand::thumbLeft() { setPacket(1<<THUMB_LRM, LEFT); }
void Hand::thumbRight() { setPacket(1<<THUMB_LRM, RIGHT); }
void Hand::thumbMid() { setPacket(1<<THUMB_LRM, MIDDLE); }

void Hand::turnOffMotors() { setPacket(0x81,0x81); }
