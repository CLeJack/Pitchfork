
#include "DataModels.h"

void Pitchfork::initialize(float srate, float blockSize)
{

    audio.srate = srate;
    audio.blockSize = blockSize;
    audio.samples = audio.historyTime() * audio.srate + 1;

    audio.ds.factor = audio.srate / audio.ds.srate;
    audio.ds.blockSize = audio.blockSize / audio.ds.factor;


    setETFrequencies(
        frequencies,
        tuning.refFreq,
        tuning.semitone,
        tuning.lowExp,
        tuning.highExp);

    setTimeVector(timeVector, audio.ds.srate);

    setComplexMatrix(freqMatrix, frequencies, timeVector);

    signalHistory.reset(new Buffer<float>(audio.samples, 0.0f));

    isInitialized = true;
}

bool Pitchfork::detectsPropertyChange(float srate, float blockSize) 
{
    if (audio.srate != srate || audio.blockSize != blockSize) 
    {
        isInitialized = false;
    }
    return isInitialized;
}



void Pitchfork::updateFundamental() 
{
    auto signal = signalHistory->toOrderedVec();
    weights = dct(
        freqMatrix, signal,
        0, frequencies.size(),
        0, signal.size());

    absMaxNormalize(weights);


    

}


