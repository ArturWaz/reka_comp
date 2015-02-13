/*
 * EmotivEpocController.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Artur
 */

#ifndef EmotivEpocController_H_
#define EmotivEpocController_H_


#include <map>
#include <fstream>
#include "EmotivEpocEngine.h"
#include "Socket.h"



class User: public SocketClient {
    char *username;
public:
    std::ofstream outputfile;

    User(int port);
    ~User();

    inline const char *getName() { return username; }
    void setName(const char *username);

};


class EmotivEpocController: public EmotivEpocEngine {

    std::map<unsigned int, User> user;

    friend void dataRecorder(EmotivEpocController*,float,const char*);

public:

    EmotivEpocController(float bufferSize);
    ~EmotivEpocController();

    unsigned int getUserID(const char*username);

    void turnOnThtreadedRecorder();
    void startRecordUser(unsigned int userID);
    void stopRecording();


    virtual void userAddedEvent(const unsigned int userID);
    virtual void userRemovedEvent(const unsigned int userID);
    virtual void cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time);
    virtual void cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType);

};


#endif

