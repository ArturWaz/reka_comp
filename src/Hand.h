/*
 * Hand.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef HAND_H_
#define HAND_H_

#include "PortCOM.h"

class Hand {

    PortCOM portCOM;

    uint8_t packet[2];

public:

    Hand(int portNumber);
    ~Hand();

    void sendData();

    void setPacket(uint8_t finger, uint8_t state);

    void openHand();
    void closeHand();

    void pinkyOpen();
    void pinkyClose();

    void ringOpen();
    void ringClose();

    void indexOpen();
    void indexClose();

    void middleOpen();
    void middleClose();

    void thumbOpen();
    void thumbClose();

    void thumbLeft();
    void thumbRight();
    void thumbMid();

};

#endif /* HAND_H_ */
