/*
 * Hand.cpp
 *
 *  Created on: Jan 30, 2015
 *      Author: Artur
 */

#include "Hand.h"


Hand::Hand(int portNumber): portCOM(portNumber,9600) {}

Hand::~Hand() {}
