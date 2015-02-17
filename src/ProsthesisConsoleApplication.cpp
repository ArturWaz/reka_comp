#include "ProsthesisConsoleApplication.h"
#include <iostream>
#include <vector>
#include <conio.h>
#include <sstream>


ProsthesisConsoleApplication::ProsthesisConsoleApplication(): EmotivEpocController(2.0f), out(std::cout), userPrompt("command: ") {

}

ProsthesisConsoleApplication::~ProsthesisConsoleApplication() {

}

int ProsthesisConsoleApplication::applicationStart() {
    bool quit = false;

    showAvailableCommands();

    while (!quit){

        if (_kbhit()) {
            switch (handleUserInput()) {
                case 0:
                    break;
                case -1:
                    quit = true;
                    break;
                case 1:
                    out << "ERROR:   Bad number of arguments.\n";
                    break;
                case 2:
                    out << "ERROR:   Bad argument/s format.\n";
                    break;
                case 3:
                    out << "ERROR:   Bad user argument.\n";
                    break;
                case 9:
                    out << "ERROR:   Command does not exist.\n";
                    break;
                default:
                    out << "ERROR:   Undefined error.\n";
                    break;
            }
        }



    }

    return 0;
}


void ProsthesisConsoleApplication::userAddedEvent(const unsigned int userID) {
    EmotivEpocController::userAddedEvent(userID);
}

void ProsthesisConsoleApplication::userRemovedEvent(const unsigned int userID) {
    EmotivEpocController::userRemovedEvent(userID);
}

void ProsthesisConsoleApplication::cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time) {
    EmotivEpocController::cognitivActionEvent(userID, actionType, actionPower, time);
}

void ProsthesisConsoleApplication::cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType) {
    EmotivEpocController::cognitivControllerEvent(userID, eventType);
}


#define CONSOLE_APP_COMMAND_setCognitivActions "setCognitivActions"
#define CONSOLE_APP_COMMAND_configureAction "configureAction"
#define CONSOLE_APP_COMMAND_trainingStart "trainingStart"
#define CONSOLE_APP_COMMAND_trainingAccept "trainingAccept"
#define CONSOLE_APP_COMMAND_trainingReject "trainingReject"
#define CONSOLE_APP_COMMAND_trainingErase "trainingErase"
#define CONSOLE_APP_COMMAND_recordDataStart "recordDataStart"
#define CONSOLE_APP_COMMAND_recordDataStop "recordDataStop"
#define CONSOLE_APP_COMMAND_setUsername "setUserName"
#define CONSOLE_APP_COMMAND_profileSave "profileSave"
#define CONSOLE_APP_COMMAND_profileLoad "profileLoad"



void ProsthesisConsoleApplication::showAvailableCommands() {
    out << "\n"
            "\t Available commands: \n"
            "\n"
            "\t\t [user]   - id of connected headset\n"
            "\t\t [action] - available actions: \"push\",\"pull\",\"lift\",\"drop\",\"left\",\"right\",\"rotate_left\",\"rotate_right\",\"rotate_clockwise\",\"rotate_counter_clockwise\",\"rotate_forwards\",\"rotate_reverse\",\"disappear\"\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_setCognitivActions<<"  [user]  [action]  [action]  ...    - set actions which will be recognize\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_configureAction<<"     [user]  [action]                   - set action to manage\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingStart<<"       [user]                             - start train chosen action\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingAccept<<"      [user]                             - accept last training\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingReject<<"      [user]                             - reject last training\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingErase<<"       [user]                             - clear training data\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_recordDataStart<<"     [user]                             - start data recording\n"
            "\t\t "<<CONSOLE_APP_COMMAND_recordDataStop<<"      [user]                             - stop data recording\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_setUsername<<"         [user]  [name]                     - set username\n"
            "\t\t "<<CONSOLE_APP_COMMAND_profileSave<<"         [user]                             - save user profile\n"
            "\t\t "<<CONSOLE_APP_COMMAND_profileLoad<<"         [user]                             - load user profile\n"
            "\t\t help                                                   - show available commands\n"
            "\t\t exit                                                   - shutdown application\n"
            "\n\n";
    out<<userPrompt;
}


int ProsthesisConsoleApplication::handleUserInput() {
    int outputFlag = 0;
    char c = (char)_getch();

    if (c == '\r') {
        std::cout << std::endl;
        std::string command;

        const size_t len = inputBuffer.length();
        command.reserve(len);

        inputBuffer.clear();

        outputFlag = parseCommand(command);

    }
    else {
        if (c == '\b') { // Backspace key
            if (inputBuffer.length()) {
                putchar(c);
                putchar(' ');
                putchar(c);
                inputBuffer.erase(inputBuffer.end()-1);
            }
        }
        else {
            inputBuffer.append(1,c);
            std::cout << c;
        }
    }
    return outputFlag;
}

void ProsthesisConsoleApplication::splitCommand(std::string const &input, std::vector<std::string> &tokens){
    tokens.clear();
    std::stringstream ss(input);
    std::string oneToken;

    while (ss >> oneToken) {
        try {
            tokens.push_back(oneToken);
        }
        catch (const std::exception&) {}
    }
}

int ProsthesisConsoleApplication::parseCommand(std::string const &command) {

    if (command.size() < 1) return 0;

    std::vector<std::string> strings;
    splitCommand(command, strings);


    if (strings[0] == "exit"){
        return -1;
    }
    if (strings[0] == "help"){
        showAvailableCommands();
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_setCognitivActions){
        // todo
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_configureAction){
        if (strings.size() != 2) return 1;
        // todo
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingStart){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingStart(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingAccept){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingAccept(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingReject) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingReject(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingErase) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionEraseData(userID)) return 3;
        return 0;
    }

    return 9;
}