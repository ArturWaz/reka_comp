/*
 * EmotivEpocController.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Artur
 */

#ifndef EmotivEpocController_H_
#define EmotivEpocController_H_


#include "EmotivEpocEngine.h"




class EmotivEpocController: public EmotivEpocEngine {



public:

    EmotivEpocController(float bufferSize);
    ~EmotivEpocController();


    void userAddedEvent(const int userID){}
    void userRemovedEvent(const int userID){}
    void cognitivActionEvent(const int userID, EpocCognitivAction actionType, float actionPower, float time){}
    void cognitivControllerEvent(const int userID, EpocCognitivEvent eventType){}

};


#endif

