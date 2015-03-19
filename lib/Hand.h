/*
 * Hand.h
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#ifndef HAND_H_
#define HAND_H_

#include <mutex>
#include "PortCOM.h"

class Hand {

    PortCOM portCOM;
    uint8_t packet[2];

    std::thread continuousDataSender;
    std::mutex mtx;
    bool ifContinuous;
    bool ifChanged;

    const uint64_t turnOffTime; // [ms] time after the motors will be turned off

    friend void dataSender(Hand *);

public:

    Hand(int portNumber, uint64_t turnOffTime);
    ~Hand();

    void sendData();
    void continuousDataTransmitter(); // with testing of read data

    void setPacket(uint8_t finger, uint8_t state);

    void turnOffMotors();

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

    void thumbPWM(uint8_t pwm);

};

#endif /* HAND_H_ */
