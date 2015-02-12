/*
 * EmotivEpocEngine.h
 *
 *  Created on: Feb 2, 2015
 *      Author: Artur
 */

#ifndef EmotivEpocEngine_H_
#define EmotivEpocEngine_H_




enum CognitivAction {
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

enum CognitivEvent {
    CognitivNoEvent = 0, CognitivTrainingStarted, CognitivTrainingSucceeded,
    CognitivTrainingFailed, CognitivTrainingCompleted, CognitivTrainingDataErased,
    CognitivTrainingRejected, CognitivTrainingReset,
    CognitivAutoSamplingNeutralCompleted, CognitivSignatureUpdated
};



class EmotivEpocEngine {

    void *event; // engine event handler
    void *state; // state handler

    void *data; // raw signal data acquisition handler
    float bufferSize; // length of data buffer in seconds

    void init(const char*address, int port);
    EmotivEpocEngine(EmotivEpocEngine &){}

protected:

    inline float getBufferSize() { return bufferSize; }

public:

    EmotivEpocEngine(float bufferSize); // if bufferSize is 0.0f, then data wont be collected
    EmotivEpocEngine(const char*IPaddress, int port, float bufferSize);
    ~EmotivEpocEngine();

    void getNextEvent();

    virtual void userAdded(const int userID){}
    virtual void userRemoved(const int userID){}
    virtual void cognitivAction(const int userID, CognitivAction actionType, float actionPower, float time){}
    virtual void cognitivEvent(const int userID, CognitivEvent eventType){}

    void takeSamplesFromBuffer(const int userID){
        if (bufferSize == 0.0f) return;
        if (EE_DataUpdateHandle(0, data) != EDK_OK) return;
        unsigned int numberOfSamplesTaken = 0;
        if (EE_DataGetNumberOfSample(data,&numberOfSamplesTaken) != EDK_OK) return;
        if (numberOfSamplesTaken) return;

        double **data = new double*[numberOfSamplesTaken];
        for (int i = 0; i < 0; ++i)
            data[i] = new double[0];
//        EE_DataGetMultiChannels(EmotivEpocEngine::data, EE_DataChannel_t channels[], unsigned int nChannels, data, numberOfSamplesTaken);

    }
};


#endif