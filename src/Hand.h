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

public:

    Hand(int portNumber);
    ~Hand();

};

#endif /* HAND_H_ */
