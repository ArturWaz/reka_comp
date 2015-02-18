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
    out << userPrompt;

    while (!quit){

        if (_kbhit()) quit = userInputErrors(handleUserInput());

        getNextEvent();

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
#define CONSOLE_APP_COMMAND_setCognitivActions_short "sa"
#define CONSOLE_APP_COMMAND_configureAction "configureAction"
#define CONSOLE_APP_COMMAND_configureAction_short "ca"
#define CONSOLE_APP_COMMAND_trainingStart "trainingStart"
#define CONSOLE_APP_COMMAND_trainingStart_short "ts"
#define CONSOLE_APP_COMMAND_trainingAccept "trainingAccept"
#define CONSOLE_APP_COMMAND_trainingAccept_short "ta"
#define CONSOLE_APP_COMMAND_trainingReject "trainingReject"
#define CONSOLE_APP_COMMAND_trainingReject_short "tr"
#define CONSOLE_APP_COMMAND_trainingErase "trainingErase"
#define CONSOLE_APP_COMMAND_trainingErase_short "te"
#define CONSOLE_APP_COMMAND_recordDataStart "recordDataStart"
#define CONSOLE_APP_COMMAND_recordDataStart_short "rb"
#define CONSOLE_APP_COMMAND_recordDataStop "recordDataStop"
#define CONSOLE_APP_COMMAND_recordDataStop_short "re"
#define CONSOLE_APP_COMMAND_setUsername "setUserName"
#define CONSOLE_APP_COMMAND_setUsername_short "pn"
#define CONSOLE_APP_COMMAND_profileSave "profileSave"
#define CONSOLE_APP_COMMAND_profileSave_short "ps"
#define CONSOLE_APP_COMMAND_profileLoad "profileLoad"
#define CONSOLE_APP_COMMAND_profileLoad_short "pl"



void ProsthesisConsoleApplication::showAvailableCommands() {
    out << "\n"
            "\t Available commands: \n"
            "\n"
            "\t\t [user]   - id of connected headset\n"
            "\t\t [action] - available actions: \"push\",\"pull\",\"lift\",\"drop\",\"left\",\"right\",\"rotate_left\",\"rotate_right\",\"rotate_clockwise\",\"rotate_counter_clockwise\",\"rotate_forwards\",\"rotate_reverse\",\"disappear\"\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_setCognitivActions<<" ("<<CONSOLE_APP_COMMAND_setCognitivActions_short<<")  [user]  [action]  [action]  ...    - set actions which will be recognize\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_configureAction<<" ("<<CONSOLE_APP_COMMAND_configureAction_short<<")     [user]  [action]                   - set action to manage\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingStart<<" ("<<CONSOLE_APP_COMMAND_trainingStart_short<<")       [user]                             - start train chosen action\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingAccept<<" ("<<CONSOLE_APP_COMMAND_trainingAccept_short<<")      [user]                             - accept last training\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingReject<<" ("<<CONSOLE_APP_COMMAND_trainingReject_short<<")      [user]                             - reject last training\n"
            "\t\t "<<CONSOLE_APP_COMMAND_trainingErase<<" ("<<CONSOLE_APP_COMMAND_trainingErase_short<<")       [user]                             - clear training data\n"
            "\n"
            "\t\t "<<CONSOLE_APP_COMMAND_setUsername<<" ("<<CONSOLE_APP_COMMAND_setUsername_short<<")         [user]  [name]                     - set username\n"
            "\t\t "<<CONSOLE_APP_COMMAND_profileSave<<" ("<<CONSOLE_APP_COMMAND_profileSave_short<<")         [user]                             - save user profile\n"
            "\t\t "<<CONSOLE_APP_COMMAND_profileLoad<<" ("<<CONSOLE_APP_COMMAND_profileLoad_short<<")         [user]                             - load user profile\n"
            "\t\t "<<CONSOLE_APP_COMMAND_recordDataStart<<" ("<<CONSOLE_APP_COMMAND_recordDataStart_short<<")     [user]                             - start data recording\n"
            "\t\t "<<CONSOLE_APP_COMMAND_recordDataStop<<" ("<<CONSOLE_APP_COMMAND_recordDataStop_short<<")      [user]                             - stop data recording\n"
            "\n"
            "\t\t help (h)                                                    - show available commands\n"
            "\t\t exit (quit)                                                 - shutdown application\n"
            "\n\n";
//    out<<userPrompt;
}


int ProsthesisConsoleApplication::handleUserInput() {
    int outputFlag = -2;
    char c = (char)_getch();

    if (c == '\r') {
        std::cout << std::endl;
        std::string command;

        const size_t len = inputBuffer.length();
        command.reserve(len);

        for (size_t i=0; i < len; i++) {
            command.append(1,inputBuffer.at(i));
        }

        inputBuffer.clear();

        if (len > 0)
            outputFlag = parseCommand(command);
        else
            out << userPrompt;

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

    std::vector<std::string> strings;
    splitCommand(command, strings);


    if (strings[0] == "exit" || strings[0] == "quit"){
        return -1;
    }
    if (strings[0] == "help"){
        showAvailableCommands();
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_setCognitivActions || strings[0] == CONSOLE_APP_COMMAND_setCognitivActions_short){
        // todo
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_configureAction || strings[0] == CONSOLE_APP_COMMAND_configureAction_short){
        if (strings.size() != 2) return 1;
        // todo
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingStart || strings[0] == CONSOLE_APP_COMMAND_trainingStart_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingStart(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingAccept || strings[0] == CONSOLE_APP_COMMAND_trainingAccept_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingAccept(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingReject || strings[0] == CONSOLE_APP_COMMAND_trainingReject_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionTrainingReject(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingErase || strings[0] == CONSOLE_APP_COMMAND_trainingErase_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!currentActionEraseData(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_recordDataStart || strings[0] == CONSOLE_APP_COMMAND_recordDataStart_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!startRecordUser(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_recordDataStop || strings[0] == CONSOLE_APP_COMMAND_recordDataStop_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!stopRecording(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_setUsername || strings[0] == CONSOLE_APP_COMMAND_setUsername_short) {
        if (strings.size() != 3) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!setUserName(userID,strings[2].c_str())) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_profileLoad || strings[0] == CONSOLE_APP_COMMAND_profileLoad_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!loadUserProfile(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_profileSave || strings[0] == CONSOLE_APP_COMMAND_profileSave_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::out_of_range& oor) { return 2; }
        if (!saveUserProfile(userID)) return 3;
        return 0;
    }

    return 9;
}

bool ProsthesisConsoleApplication::userInputErrors(int i) {
    switch (i) {
        case -2:
            break;
        case 0:
            out << userPrompt;
            break;
        case -1:
            return true;
        case 1:
            out << "ERROR:   Bad number of arguments.\n" << userPrompt;
            break;
        case 2:
            out << "ERROR:   Bad argument/s format.\n" << userPrompt;
            break;
        case 3:
            out << "ERROR:   Bad argument/s.\n" << userPrompt;
            break;
        case 9:
            out << "ERROR:   Command does not exist.\n" << userPrompt;
            break;
        default:
            out << "ERROR:   Undefined error.\n" << userPrompt;
            break;
    }
    return false;
}
