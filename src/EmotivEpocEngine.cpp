

#include <cstdint>
#include <iostream>
#include <emotiv/edk.h>
#include "EmotivEpocEngine.h"

using namespace std;



static uint8_t EmotivEpocEngineID = 0;



void EmotivEpocEngine::init(const char*address, int port) {
    if (EmotivEpocEngineID){
        std::cerr << "\n\n\tERROR: Only one EmotivEpocEngine class could exist in application. \n\n";
        std::exit(1);
    }
    ++EmotivEpocEngineID;

    event = EE_EmoEngineEventCreate();
    state = EE_EmoStateCreate();

    if (address == NULL) {
        if (EE_EngineConnect() != EDK_OK) throw "Emotive Engine start up failed.";
    }
    else {
        if (EE_EngineRemoteConnect(address, port) != EDK_OK) throw "Emotive Engine start up failed.";
    }

    if (bufferSize != 0.0f){
        DataHandle hData = EE_DataCreate();
        EE_DataSetBufferSizeInSec(bufferSize);
    }

}

EmotivEpocEngine::EmotivEpocEngine(float bufferSize): bufferSize(bufferSize) {
    init(NULL,0);
}

EmotivEpocEngine::EmotivEpocEngine(const char*IPaddress, int port, float bufferSize): bufferSize(bufferSize) {
    init(IPaddress,port);
}

EmotivEpocEngine::~EmotivEpocEngine() {
    EE_EngineDisconnect();
    EE_EmoStateFree(state);
    EE_EmoEngineEventFree(event);
    if (bufferSize != 0.0f) EE_DataFree(data);
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
            userAdded(userID);
            break;
        case EE_UserRemoved:
            userRemoved(userID);
            break;
        case EE_EmoStateUpdated:
            EE_EmoEngineEventGetEmoState(event, EmotivEpocEngine::state);
            cognitivAction(userID, CognitivAction(ES_CognitivGetCurrentAction(EmotivEpocEngine::state)), ES_CognitivGetCurrentActionPower(EmotivEpocEngine::state), ES_GetTimeFromStart(EmotivEpocEngine::state));
            break;
        case EE_CognitivEvent:
            cognitivEvent(userID, CognitivEvent(EE_CognitivEventGetType(event)));
            break;
    }

    return;
}
