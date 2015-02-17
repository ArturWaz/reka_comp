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
#include <thread>
#include <mutex>
#include <string>
#include "EmotivEpocEngine.h"
#include "Socket.h"



class User: public SocketClient {
    char *username;
    void operator=(User&u){}
public:

    User(unsigned int port);
    User(User const &u): SocketClient(u) {
        if (u.username == NULL)
            username = NULL;
        else {
            username = (char *) malloc(strlen(u.username) + 1);
            memcpy(username, u.username, strlen(u.username) + 1);
        }
    }
    ~User();

    inline const char *getName() { return username; }
    void setName(const char *username);

};


class EmotivEpocController: public EmotivEpocEngine {

    std::map<unsigned int, User> users;

    // only one user could be recorded
    std::ofstream outputCOGfile;
    std::thread recordData;
    bool threadedReading; // marker, true if threaded reading is turn on

    friend void dataRecorder(EmotivEpocController*,std::string,float,unsigned int);

public:

    EmotivEpocController(float bufferSize);
    ~EmotivEpocController();

    unsigned int getUserID(const char*username);
    bool setUserName(unsigned int userID, const char *username);

    inline bool isRecording() const { return threadedReading; }

    bool startRecordUser(unsigned int userID);
    bool stopRecording(unsigned int userID);


    virtual void userAddedEvent(const unsigned int userID);
    virtual void userRemovedEvent(const unsigned int userID);
    virtual void cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time);
    virtual void cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType);

};


#endif

