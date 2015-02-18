
#include "EmotivEpocController.h"
#include <cstring>
#include <ctime>
#include <DefineFunctions.h>
#include <iostream>
#include <sstream>


EmotivEpocController::EmotivEpocController(float bufferSize): EmotivEpocEngine(bufferSize), threadedReading(false), userID(INFINITE) {

}

EmotivEpocController::~EmotivEpocController() {
    if (threadedReading) {
        outputCOGfile.close();
        threadedReading = false;
        recordData.join();
    }
}

void EmotivEpocController::userAddedEvent(const unsigned int userID) {
//    EmotivEpocEngine::userAddedEvent(userID);
    users.insert(std::pair<unsigned int, User>(userID, User(userID)));
}

void EmotivEpocController::userRemovedEvent(const unsigned int userID) {
//    EmotivEpocEngine::userRemovedEvent(userID);
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return;
    if (iter != users.end()) users.erase(iter);
}

void EmotivEpocController::cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time) {
//    EmotivEpocEngine::cognitivActionEvent(userID, actionType, actionPower, time);
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return;
    if (outputCOGfile.is_open() && userID == EmotivEpocController::userID){
        outputCOGfile<<time<<','<<int(actionType)<<','<<actionPower<<std::endl;
    }

    std::ostringstream os;
    os << static_cast<int>(actionType) << "," << static_cast<int>(actionPower*100.0f);
    iter->second.SendBytes(os.str());
}

void EmotivEpocController::cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType) {
//    EmotivEpocEngine::cognitivControllerEvent(userID, eventType);
}

User::User(unsigned int port): SocketClient("localhost",port+20000,UDP), username(NULL) {}

User::~User() {
    if (username) free(username);
}

void User::setName(const char *username) {
    if (User::username) delete []User::username;
    User::username = new char[strlen(username)+1];
    strcpy(User::username,username);
}

unsigned int EmotivEpocController::getUserID(const char *username) {
    if (!username) throw "Name has to be given.";
    std::map<unsigned int, User>::iterator iter = users.begin();
    while (iter != users.end()){
        if (strcmp(username,iter->second.getName())) return iter->first;
        ++iter;
    }
    throw "There is no users with given name.";
}






void dataRecorder(EmotivEpocController *controller, std::string fileName, float bufferSizeInSeconds, unsigned int userID) {
    controller->EmotivEpocEngine::dataAcqusitionEnable(userID);
    std::ofstream outputStream;
    bufferSizeInSeconds *= 1000/2;

    outputStream.open(fileName, std::ofstream::trunc);
    outputStream << "COUNTER,INTERPOLATED,RAW_CQ,AF3,F7,F3,FC5,T7,P7,O1,O2,P8,T8,FC6,F4,F8,AF4,GYROX,GYROY,TIMESTAMP,ES_TIMESTAMP,FUNC_ID,FUNC_VALUE,MARKER,SYNC_SIGNAL" << std::endl;

    while(controller->threadedReading){
        SLEEP_MS(int(bufferSizeInSeconds));

        DataPacket *dataPacket = controller->EmotivEpocEngine::takeSamplesFromBuffer(userID);
        if (dataPacket == NULL) continue;

        dataPacket->writeDataToStream(outputStream);
        delete dataPacket;
    }
    controller->EmotivEpocEngine::dataAcqusitionDisable(userID);
    outputStream.close();
}



bool EmotivEpocController::startRecordUser(unsigned int userID) {
    if (threadedReading) return false;

    if (getBufferSize() <= 0.0f) return false;

    // GENERATE FILE NAME
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return false;



    char filename[100] = "undefinedUsername";
    unsigned int filenameLength = 17;
    if (iter->second.getName() != NULL) {
        filenameLength = (int)strlen(iter->second.getName());
        memcpy(filename, iter->second.getName(), filenameLength);
    }

    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo;
    timeinfo = localtime(&rawtime);
    std::strftime(&(filename[filenameLength]),100,"_%Y.%m.%d_%H.%M.%S_",timeinfo); // "_%F_%H-%M-%S"
    filenameLength = (unsigned int)strlen(filename);


    // TURN ON COG RECORDER
    strcat(filename,"COG.csv");
    outputCOGfile.open(filename, std::ofstream::trunc);
    outputCOGfile << "TIME,ACTION,POWER\n";
    EmotivEpocController::userID = userID;

    // TURN ON EEG RECORDER
    strcpy(&(filename[filenameLength]),"EEG.csv");
    threadedReading = true;
    recordData = thread(dataRecorder,this,std::string(filename), getBufferSize(),iter->first);

    return true;
}

bool EmotivEpocController::stopRecording(unsigned int userID) {
    if (!threadedReading) return false;

    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return false;

    // TURN OFF COG RECORDER
    outputCOGfile.close();
    EmotivEpocController::userID = INFINITE;

    // TURN OFF EEG RECORDER
    threadedReading = false;
    recordData.join();

    return true;
}

bool EmotivEpocController::setUserName(unsigned int userID, const char *username) {
    if (username == NULL) return false;
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return false;
    iter->second.setName(username);
    return true;
}

bool EmotivEpocController::saveUserProfile(unsigned int userID) {
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return false;
    if (iter->second.getName() == NULL)
        return EmotivEpocEngine::saveUserProfile(userID,"unnamedUser.emu");
    std::string str(iter->second.getName());
    str += ".emu";
    return EmotivEpocEngine::saveUserProfile(userID,str.c_str());
}

bool EmotivEpocController::loadUserProfile(unsigned int userID) {
    std::map<unsigned int, User>::iterator iter = users.find(userID);
    if (iter == users.end()) return false;
    if (iter->second.getName() == NULL)
        return EmotivEpocEngine::loadUserProfile(userID,"unnamedUser.emu");
    std::string str(iter->second.getName());
    str += ".emu";
    return EmotivEpocEngine::loadUserProfile(userID,str.c_str());
}
