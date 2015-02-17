

#include <cstdint>
#include <iostream>
#include "emotiv/edk.h"
#include "EmotivEpocEngine.h"

using namespace std;


static uint8_t EmotivEpocEngineID = 0;



void EmotivEpocEngine::init(const char*address, int port) {
    if (EmotivEpocEngineID){
        std::cerr << "\n\n\tERROR: Only one EmotivEpocEngine class could exist in application. \n\n";
        std::exit(1);
    }
    ++EmotivEpocEngineID;

    if (address == NULL) {
        if (EE_EngineConnect() != EDK_OK) throw "Emotive Engine start up failed.";
    }
    else {
        if (EE_EngineRemoteConnect(address, port) != EDK_OK) throw "Emotive Engine start up failed.";
    }


    event = EE_EmoEngineEventCreate();
    state = EE_EmoStateCreate();
    if (bufferSize > 0.0f){
        data = EE_DataCreate();
        EE_DataSetBufferSizeInSec(bufferSize);
    }
}

EmotivEpocEngine::EmotivEpocEngine(float bufferSize): bufferSize(bufferSize), state(NULL), event(NULL), data(NULL) {
    init(NULL,0);
}

EmotivEpocEngine::EmotivEpocEngine(const char*IPaddress, int port, float bufferSize): bufferSize(bufferSize), state(NULL), event(NULL), data(NULL) {
    init(IPaddress,port);
}

EmotivEpocEngine::~EmotivEpocEngine() {
    EE_EngineDisconnect();
    EE_EmoStateFree(state);
    EE_EmoEngineEventFree(event);
    EE_DataFree(data);
    --EmotivEpocEngineID;
}

void EmotivEpocEngine::getNextEvent() {
    int state = EE_EngineGetNextEvent(event);
    unsigned int userID;

    if (state == EDK_NO_EVENT) return;
    if (state != EDK_OK) throw "Internal error in Emotiv Engine.";

    EE_Event_t eventType = EE_EmoEngineEventGetType(event);
    EE_EmoEngineEventGetUserId(event, &userID);

    switch (eventType){
        case EE_UserAdded:
            userAddedEvent(userID);
            break;
        case EE_UserRemoved:
            userRemovedEvent(userID);
            break;
        case EE_EmoStateUpdated:
            EE_EmoEngineEventGetEmoState(event, EmotivEpocEngine::state);
            cognitivActionEvent(userID, EpocCognitivAction(ES_CognitivGetCurrentAction(EmotivEpocEngine::state)), ES_CognitivGetCurrentActionPower(EmotivEpocEngine::state), ES_GetTimeFromStart(EmotivEpocEngine::state));
            break;
        case EE_CognitivEvent:
            cognitivControllerEvent(userID, EpocCognitivEvent(EE_CognitivEventGetType(event)));
            break;
        default:
            break;
    }

    return;
}

bool EmotivEpocEngine::dataAcqusitionEnable(unsigned int userID) {
    return EE_DataAcquisitionEnable(userID,true) == EDK_OK;
}

bool EmotivEpocEngine::dataAcqusitionDisable(unsigned int userID) {
    return EE_DataAcquisitionEnable(userID,false) == EDK_OK;
}

DataPacket::DataPacket(unsigned int numberOfSamples): nSamples(numberOfSamples) {
    for (int i = 0; i < 25; ++i)
        packet[i] = new double[numberOfSamples];
}

DataPacket::~DataPacket() {
    for (int i = 0; i < 25; ++i) {
        free(packet[i]);
    }
}

void DataPacket::writeDataToStream(std::ostream &ostream1) {
    for (int j = 0; j < nSamples; ++j) {
        for (int i = 0; i < 25; ++i) {
            ostream1 << packet[i][j];
            if (i < 24) ostream1 << ',';
        }
        ostream1 << std::endl;
    }
}

DataPacket *EmotivEpocEngine::takeSamplesFromBuffer(const unsigned int userID) {
    if (!data) return NULL;

    bool dataAcquisitionEnabled;
    EE_DataAcquisitionIsEnabled(userID, &dataAcquisitionEnabled);
    if (dataAcquisitionEnabled) return NULL;

    if (EE_DataUpdateHandle(userID, data)) throw "Problem with data handler.";
    unsigned int numberOfSamplesTaken;
    if (EE_DataGetNumberOfSample(data,&numberOfSamplesTaken))  throw "Problem with data handler.";

    if (!numberOfSamplesTaken) return NULL;

    EE_DataChannel_t channels[] = {
            ED_COUNTER, ED_INTERPOLATED, ED_RAW_CQ,
            ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
            ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
            ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX,
            ED_GYROY, ED_TIMESTAMP, ED_ES_TIMESTAMP, ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER,
            ED_SYNC_SIGNAL
    };

    DataPacket *packet = new DataPacket(numberOfSamplesTaken);
    EE_DataGetMultiChannels(EmotivEpocEngine::data, channels, 25, packet->pointer(), numberOfSamplesTaken);

    return packet;
}

bool EmotivEpocEngine::loadUserProfile(unsigned int userID, const char *filename) {
    return EE_LoadUserProfile(userID, filename) == EDK_OK;
}

bool EmotivEpocEngine::saveUserProfile(unsigned int userID, const char *filename) {
    return EE_LoadUserProfile(userID, filename) == EDK_OK;
}

void EmotivEpocEngine::userAddedEvent(const unsigned int userID) { std::cout<<"Added users: "<<userID<<std::endl; }

void EmotivEpocEngine::userRemovedEvent(const unsigned int userID) { std::cout<<"Removed users: "<<userID<<std::endl; }


void EmotivEpocEngine::cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time) {
    std::cout<<"Action users: "<<userID<<", action type: "<<int(actionType)<<", action power: "<<actionPower*100.0f<<"."<<std::endl;
}


void EmotivEpocEngine::cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType) {
    std::cout<<"Cognitiv event users: "<<userID<<", cognitiv event type: "<<int(eventType)<<"."<<std::endl;
}
