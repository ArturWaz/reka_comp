
#include "EmotivEpocController.h"
#include <cstring>
#include <ctime>
#include <DefineFunctions.h>
#include <thread>
#include <mutex>


EmotivEpocController::EmotivEpocController(float bufferSize): EmotivEpocEngine(bufferSize) {

}

EmotivEpocController::~EmotivEpocController() {

}

void EmotivEpocController::userAddedEvent(const unsigned int userID) {
//    EmotivEpocEngine::userAddedEvent(userID);
    user.insert(std::pair<unsigned int, User>(userID, User(20000+userID)));
}

void EmotivEpocController::userRemovedEvent(const unsigned int userID) {
//    EmotivEpocEngine::userRemovedEvent(userID);
    std::map<unsigned int, User>::iterator iter = user.find(userID);
    if (iter != user.end()) user.erase(iter);
}

void EmotivEpocController::cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time) {
//    EmotivEpocEngine::cognitivActionEvent(userID, actionType, actionPower, time);
    std::map<unsigned int, User>::iterator iter = user.find(userID);
    if (iter->second.outputfile.is_open()){
        iter->second.outputfile<<time<<','<<int(actionType)<<','<<actionPower<<std::endl;
    }
}

void EmotivEpocController::cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType) {
//    EmotivEpocEngine::cognitivControllerEvent(userID, eventType);
}

User::User(int port): SocketClient("localhost",port,UDP), username(NULL) {}

User::~User() {
    if (username) delete []username;
}

void User::setName(const char *username) {
    if (User::username) delete []User::username;
    User::username = new char[strlen(username)+1];
    strcpy(User::username,username);
}

unsigned int EmotivEpocController::getUserID(const char *username) {
    if (!username) throw "Name has to be given.";
    std::map<unsigned int, User>::iterator iter = user.begin();
    while (iter != user.end()){
        if (strcmp(username,iter->second.getName())) return iter->first;
        ++iter;
    }
    throw "There is no user with given name.";
}





std::thread EmotivEpocController_recordData;
std::mutex EmotivEpocController_recordDataMutex;
unsigned int EmotivEpocController_userID = INFINITE;
char *EmotivEpocController_fileName = NULL;

void dataRecorder(EmotivEpocController *controller, float bufferSizeInSeconds) { // todo check if this working
    bool changed = true;
    std::ofstream outputStream;
    bufferSizeInSeconds *= 1000/2;
    while(true){
        SLEEP_MS(int(bufferSizeInSeconds));
        EmotivEpocController_recordDataMutex.lock();
        if (EmotivEpocController_userID == INFINITE){
            changed = true;
            if (outputStream.is_open()) outputStream.close();
            continue;
        }
        EmotivEpocController_recordDataMutex.unlock();

        if (changed){

            outputStream.open(EmotivEpocController_fileName, std::ofstream::trunc);

            outputStream << "COUNTER,INTERPOLATED,RAW_CQ,AF3,F7,F3,FC5,T7,P7,O1,O2,P8,T8,FC6,F4,F8,AF4,GYROX,GYROY,TIMESTAMP,ES_TIMESTAMP,FUNC_ID,FUNC_VALUE,MARKER,SYNC_SIGNAL" << std::endl;
            changed = false;
        }

        DataPacket *dataPacket = controller->takeSamplesFromBuffer(EmotivEpocController_userID);

        dataPacket->writeDataToStream(outputStream);

        delete dataPacket;
    }
}


void EmotivEpocController::turnOnThtreadedRecorder() {
    if (getBufferSize() <= 0.0f) return;
    EmotivEpocController_recordData = std::thread(dataRecorder, this);
    EmotivEpocController_recordData.detach();
}

void EmotivEpocController::startRecordUser(unsigned int userID) { // todo finish !!!

    // GENERATE FILE NAME
    std::map<unsigned int, User>::iterator iter = user.find(userID);
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime (buffer,80,"_%F_%H-%M-%S",timeinfo);

    std::string filename(EmotivEpocController_fileName);
    filename += string(buffer);

    // TURN ON COG RECORDER
    // todo open file

    // TURN ON EEG RECORDER
    EmotivEpocController_recordDataMutex.lock();
    EmotivEpocController_userID = userID;
    if (EmotivEpocController_fileName) delete []EmotivEpocController_fileName;
    EmotivEpocController_fileName = new char[strlen(       )+1];
    strcpy(EmotivEpocController_fileName,        );
    EmotivEpocController_recordDataMutex.unlock();
}

void EmotivEpocController::stopRecording() {

    // TURN OFF COG RECORDER
    // todo close file

    // TURN OFF EEG RECORDER
    EmotivEpocController_recordDataMutex.lock();
    EmotivEpocController_userID = INFINITE;
    if (EmotivEpocController_fileName) delete []EmotivEpocController_fileName;
    EmotivEpocController_fileName = NULL;
    EmotivEpocController_recordDataMutex.unlock();
}

