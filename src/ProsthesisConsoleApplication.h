/*
 * ProsthesisConsoleApplication.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Artur
 */

#ifndef ProsthesisConsoleApplication_H_
#define ProsthesisConsoleApplication_H_


#include "EmotivEpocController.h"
#include <ostream>
#include <string>




class ProsthesisConsoleApplication: private EmotivEpocController {
    std::ostream &out;
    std::string inputBuffer;
    char const *userPrompt;

    virtual void userAddedEvent(const unsigned int userID);
    virtual void userRemovedEvent(const unsigned int userID);
    virtual void cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time);
    virtual void cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType);

public:

    ProsthesisConsoleApplication();
    ~ProsthesisConsoleApplication();


    int applicationStart();


    int handleUserInput();
    int parseCommand(std::string const &);
    void splitCommand(std::string const &, std::vector<std::string> &);

    void showAvailableCommands();
    void prompt();

};


#endif