
#include "DataModels.h"

void Pitchfork::initialize(float srate, float blockSize)
{

    audio.srate = srate;
    audio.blockSize = blockSize;
    audio.samples = audio.historyTime() * audio.srate + 1;

    audio.ds.factor = (audio.srate / audio.ds.srate) + 0.5;
    audio.ds.blockSize = audio.blockSize / audio.ds.factor;
    

    setETFrequencies(
        frequencies,
        tuning.refFreq,
        tuning.semitone,
        tuning.lowExp,
        tuning.highExp);

    

    

    signalHistory.reset(new Buffer<float>(audio.samples, 0.0f));
    freqMatrix.reset(new cmatrix(frequencies.size(), cvec(signalHistory->size, std::complex<float>(0, 0))));
    timeVector.reset(new fvec(signalHistory->size, 0));

    harmonicMatrix.reset(
        new std::vector<cmatrix>(
        frequencies.size(),
        cmatrix(harmonics, cvec(audio.samples, std::complex<float>(0, 0)))
        )
    );

    setTimeVector(timeVectorDS, audio.ds.srate);
    setComplexMatrix(freqMatrixDS, frequencies, timeVectorDS);
    setHarmonicMatrix(harmonicMatrixDS, frequencies, timeVectorDS);
    
    setTimeVector(*timeVector.get(), audio.srate);
    setComplexMatrix(*freqMatrix.get(), frequencies, *timeVector.get());
    setHarmonicMatrix(*harmonicMatrix.get(), frequencies, *timeVector.get());

    fpsBlocks = srate / (blockSize * fps);

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



void Pitchfork::updateFundamental(const AudioParams& params) 
{
    freqWeights = dct(
        freqMatrixDS, historyDS,
        0, frequencies.size(),
        0, historyDS.size());

    absMaxNormalize(freqWeights);

    maxIndex = maxArg(freqWeights);

    if(maxIndex > 12)
    {
       maxIndex =  freqWeights[maxIndex] / freqWeights[maxIndex - 12] < params.octaveRatio ? maxIndex - 12 : maxIndex;
    }

    //32nd harmonic = 5th octave, so I remove 5 octaves (5 * 12) from the frequency range
    //anything below this limit can use the downsampled signal
    //anything above this limit must use the normal signal

    if(maxIndex < frequencies.size() - 5*12)
    {
        harmonicWeights = dct(
        harmonicMatrixDS[maxIndex], historyDS,
        0, frequencies.size(),
        0, historyDS.size());

        //get rid of freqs that exceed nyquist limit;
        for(int i = 0; i < harmonics; i ++)
        {
            harmonicWeights[i] = frequencies[maxIndex] *(i+1) >= audio.ds.srate ? 0 : harmonicWeights[i];
        }
    }
    else
    {
        harmonicWeights = dct(
            harmonicMatrix->at(maxIndex), signalHistory->toOrderedVec(),
            0, harmonics,
            0, historyDS.size()); 
            //historyDS.size() is intentional here
            //frequencies at this range (c3 and up) don't need the full signal

        //get rid of freqs that exceed nyquist limit;
        for(int i = 0; i < harmonics; i ++)
        {
            harmonicWeights[i] = frequencies[maxIndex] *(i+1) >= audio.srate ? 0 : harmonicWeights[i];
        }
    }

    
    absMaxNormalize(harmonicWeights);
    

}


