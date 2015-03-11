#include "ProsthesisConsoleApplication.h"
#include <iostream>
#include <conio.h>
#include <sstream>




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



#define CONSOLE_APP_ACTION_NEUTRAL "neutral"
#define CONSOLE_APP_ACTION_PUSH "push"
#define CONSOLE_APP_ACTION_PULL "pull"
#define CONSOLE_APP_ACTION_LIFT "lift"
#define CONSOLE_APP_ACTION_DROP "drop"
#define CONSOLE_APP_ACTION_LEFT "left"
#define CONSOLE_APP_ACTION_RIGHT "right"
#define CONSOLE_APP_ACTION_ROTATE_LEFT "rotate_right"
#define CONSOLE_APP_ACTION_ROTATE_RIGHT "rotate_left"
#define CONSOLE_APP_ACTION_ROTATE_CLOCKWISE "rotate_clockwise"
#define CONSOLE_APP_ACTION_ROTATE_COUNTER_CLOCKWISE "rotate_counter_clockwise"
#define CONSOLE_APP_ACTION_ROTATE_FORWARDS "rotate_forwards"
#define CONSOLE_APP_ACTION_ROTATE_REVERSE "rotate_reverse"
#define CONSOLE_APP_ACTION_DISAPPEAR "dissapear"




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
    out << "\n\tAdded new user (headset), id: " << userID << std::endl << userPrompt;
}

void ProsthesisConsoleApplication::userRemovedEvent(const unsigned int userID) {
    EmotivEpocController::userRemovedEvent(userID);
    out << "\n\tUser removed, id: " << userID << std::endl << userPrompt;
}

void ProsthesisConsoleApplication::cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time) {
    EmotivEpocController::cognitivActionEvent(userID, actionType, actionPower, time);
    // todo prosthesis control
}

void ProsthesisConsoleApplication::cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType) {
    EmotivEpocController::cognitivControllerEvent(userID, eventType);
    if (eventType == COGNITIV_NO_EVENT) return;
    if (eventType == COGNITIV_TRAINING_STARTED) {
        out << "\n\tTraining started for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_SUCCEEDED) {
        out << "\n\tTraining succeeded for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_FAILED) {
        out << "\n\tTraining failde for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_COMPLETED) {
        out << "\n\tTraining completed for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_DATA_ERASED) {
        out << "\n\tTraining data erased for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_REJECTED) {
        out << "\n\tTraining rejected for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_TRAINING_RESET) {
        out << "\n\tTraining reset for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_AUTOSAMPLING_NEUTRAL_COMPLETED) {
        out << "\n\tNeutral autosampling completed for user: " << userID << std::endl << userPrompt;
        return;
    }
    if (eventType == COGNITIV_SIGNATURE_UPDATED) {
        out << "\n\tSignature updated for user: " << userID << std::endl << userPrompt;
        return;
    }
}




void ProsthesisConsoleApplication::showAvailableCommands() {
    out << "\n"
            "\n"
            "\t Available commands: \n"
            "\n"
            "\t\t (...)    - alternative command\n"
            "\t\t [user]   - id of connected headset\n"
            "\t\t [action] - available actions: \""<< CONSOLE_APP_ACTION_NEUTRAL <<"\",\""<< CONSOLE_APP_ACTION_PUSH <<"\",\""<< CONSOLE_APP_ACTION_PULL <<"\",\""<< CONSOLE_APP_ACTION_LIFT <<"\",\""<< CONSOLE_APP_ACTION_DROP <<"\",\""<< CONSOLE_APP_ACTION_LEFT <<"\",\""<< CONSOLE_APP_ACTION_RIGHT <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_LEFT <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_RIGHT <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_CLOCKWISE <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_COUNTER_CLOCKWISE <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_FORWARDS <<"\",\""<< CONSOLE_APP_ACTION_ROTATE_REVERSE <<"\",\""<< CONSOLE_APP_ACTION_DISAPPEAR <<"\"\n"
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
    if (strings[0] == "help" || strings[0] == "h"){
        showAvailableCommands();
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_setCognitivActions || strings[0] == CONSOLE_APP_COMMAND_setCognitivActions_short){
        if (strings.size() < 3) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        unsigned long actions = 0;
        for (int i = 2; i < strings.size(); ++i) {
            unsigned long action = parseCognitivAction(strings[i]);
            if (action == 0) return 3;
            actions += action;
        }
        if (!setCognitivActions(userID,actions)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_configureAction || strings[0] == CONSOLE_APP_COMMAND_configureAction_short){
        if (strings.size() != 3) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        unsigned long actions = parseCognitivAction(strings[2]);
        if (actions == 0) return 3;
        if (!setCurrentAction(userID,EpocCognitivAction(actions))) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingStart || strings[0] == CONSOLE_APP_COMMAND_trainingStart_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!currentActionTrainingStart(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingAccept || strings[0] == CONSOLE_APP_COMMAND_trainingAccept_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!currentActionTrainingAccept(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingReject || strings[0] == CONSOLE_APP_COMMAND_trainingReject_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!currentActionTrainingReject(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_trainingErase || strings[0] == CONSOLE_APP_COMMAND_trainingErase_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!currentActionEraseData(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_recordDataStart || strings[0] == CONSOLE_APP_COMMAND_recordDataStart_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!startRecordUser(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_recordDataStop || strings[0] == CONSOLE_APP_COMMAND_recordDataStop_short){
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!stopRecording(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_setUsername || strings[0] == CONSOLE_APP_COMMAND_setUsername_short) {
        if (strings.size() != 3) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!setUserName(userID,strings[2].c_str())) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_profileLoad || strings[0] == CONSOLE_APP_COMMAND_profileLoad_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
        if (!loadUserProfile(userID)) return 3;
        return 0;
    }
    if (strings[0] == CONSOLE_APP_COMMAND_profileSave || strings[0] == CONSOLE_APP_COMMAND_profileSave_short) {
        if (strings.size() != 2) return 1;
        unsigned int userID;
        try { userID = std::stoul(strings.at(1),nullptr); } catch(const std::invalid_argument& oor) { return 2; }
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





unsigned int ProsthesisConsoleApplication::parseCognitivAction(string const &aConst) {
    if (aConst == CONSOLE_APP_ACTION_NEUTRAL) return ACTION_NEUTRAL;
    if (aConst == CONSOLE_APP_ACTION_PUSH) return ACTION_PUSH;
    if (aConst == CONSOLE_APP_ACTION_PULL) return ACTION_PULL;
    if (aConst == CONSOLE_APP_ACTION_LIFT) return ACTION_LIFT;
    if (aConst == CONSOLE_APP_ACTION_DROP) return ACTION_DROP;
    if (aConst == CONSOLE_APP_ACTION_LEFT) return ACTION_LEFT;
    if (aConst == CONSOLE_APP_ACTION_RIGHT) return ACTION_RIGHT;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_LEFT) return ACTION_ROTATE_LEFT;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_RIGHT) return ACTION_ROTATE_RIGHT;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_CLOCKWISE) return ACTION_ROTATE_CLOCKWISE;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_COUNTER_CLOCKWISE) return ACTION_ROTATE_COUNTER_CLOCKWISE;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_FORWARDS) return ACTION_ROTATE_FORWARDS;
    if (aConst == CONSOLE_APP_ACTION_ROTATE_REVERSE) return ACTION_ROTATE_REVERSE;
    if (aConst == CONSOLE_APP_ACTION_DISAPPEAR) return ACTION_DISAPPEAR;
    return 0;
}
