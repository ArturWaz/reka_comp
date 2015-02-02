/*
 * Hand.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */


#include "Hand.h"
#include "Config.h"


Hand::Hand(int portNumber): portCOM(portNumber,9600) {
    portCOM.open();
    packet[0] = 0x0F;
    packet[1] = 0xF0;
}

Hand::~Hand() {}

void Hand::sendData() {
    portCOM.sendBlock(packet,2);
}


void Hand::setPacket(uint8_t finger, uint8_t state) {
    packet[0] = 0x81 | finger;
    packet[1] = state;
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
