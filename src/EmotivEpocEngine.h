/*
 * EmotivEpocEngine.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Artur
 */

#ifndef EmotivEpocEngine_H_
#define EmotivEpocEngine_H_

#include <ostream>


enum EpocCognitivAction {
    ACTION_NEUTRAL				    = 0x0001,
    ACTION_PUSH						= 0x0002,
    ACTION_PULL						= 0x0004,
    ACTION_LIFT						= 0x0008,
    ACTION_DROP						= 0x0010,
    ACTION_LEFT						= 0x0020,
    ACTION_RIGHT					= 0x0040,
    ACTION_ROTATE_LEFT				= 0x0080,
    ACTION_ROTATE_RIGHT				= 0x0100,
    ACTION_ROTATE_CLOCKWISE			= 0x0200,
    ACTION_ROTATE_COUNTER_CLOCKWISE	= 0x0400,
    ACTION_ROTATE_FORWARDS			= 0x0800,
    ACTION_ROTATE_REVERSE			= 0x1000,
    ACTION_DISAPPEAR				= 0x2000
};

enum EpocCognitivEvent {
    COGNITIV_NO_EVENT = 0, COGNITIV_TRAINING_STARTED, COGNITIV_TRAINING_SUCCEEDED,
    COGNITIV_TRAINING_FAILED, COGNITIV_TRAINING_COMPLETED, COGNITIV_TRAINING_DATA_ERASED,
    COGNITIV_TRAINING_REJECTED, COGNITIV_TRAINING_RESET,
    COGNITIV_AUTOSAMPLING_NEUTRAL_COMPLETED, COGNITIV_SIGNATURE_UPDATED
};


enum EpocDataChannel {
    COUNTER = 0, INTERPOLATED, RAW_CQ,
    AF3, F7, F3, FC5, T7,
    P7, O1, O2, P8, T8,
    FC6, F4, F8, AF4, GYROX,
    GYROY, TIMESTAMP, ES_TIMESTAMP, FUNC_ID, FUNC_VALUE, MARKER,
    SYNC_SIGNAL
};



class DataPacket {

    double *packet[25];
    unsigned int nSamples;

public:

    DataPacket(unsigned int numberOfSamples);
    ~DataPacket();

    inline unsigned int numberOfSamples() { return nSamples; }
    inline const double&operator()(EpocDataChannel channel, unsigned int sampleNumber) const { return packet[channel][sampleNumber]; }
    inline double **pointer(){ return packet; }

    void writeDataToStream(std::ostream&);
};


class EmotivEpocEngine {

    void *event; // engine event handler
    void *state; // state handler

    void *data; // raw signal data acquisition handler
    float bufferSize; // length of data buffer in seconds

    void init(const char*address, int port);
    EmotivEpocEngine(EmotivEpocEngine &){}
    void operator=(EmotivEpocEngine&){}

public:

    EmotivEpocEngine(float bufferSize); // if bufferSize is 0.0f, then data wont be collected
    EmotivEpocEngine(const char*IPaddress, int port, float bufferSize);
    ~EmotivEpocEngine();


    // PROFILE MANAGEMENT
    bool loadUserProfile(unsigned int userID, const char*filename);
    bool saveUserProfile(unsigned int userID, const char*filename);


    // RAW EEG READING
    inline float getBufferSize() { return bufferSize; }
    bool dataAcqusitionEnable(unsigned int userID);
    bool dataAcqusitionDisable(unsigned int userID);
    DataPacket *takeSamplesFromBuffer(const unsigned int userID); // return NULL if the packet was not read


    // EVENT ACQUISITION
    void getNextEvent();
    virtual void userAddedEvent(const unsigned int userID);
    virtual void userRemovedEvent(const unsigned int userID);
    virtual void cognitivActionEvent(const unsigned int userID, EpocCognitivAction actionType, float actionPower, float time);
    virtual void cognitivControllerEvent(const unsigned int userID, EpocCognitivEvent eventType);


    // COGNITIV CONTROL
    bool setCognitivActions(unsigned int userID, unsigned long actions);
    bool setCognitivActions(unsigned int userID, EpocCognitivAction *actions, unsigned int numberOfActions);
    bool setCurrentAction(unsigned int userID, EpocCognitivAction action);
    bool currentActionEraseData(unsigned int userID);
    bool currentActionTrainingStart(unsigned int userID);
    bool currentActionTrainingAccept(unsigned int userID);
    bool currentActionTrainingReject(unsigned int userID);

};


#endif