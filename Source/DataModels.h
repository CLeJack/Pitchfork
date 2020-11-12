#pragma once
#include "Head.h"
#include "Waveforms.h"
#include "Tuning.h"
#include "CircularBuffer.h"
#include "Stats.h"
#include "DCT.h"
#include "ProcessData.h"




// 0.Shared Structs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct AudioParams
{
    float octaveRatio = 4;
    int historyIndex = 0;
    bool hoverOn = true;
    bool recordA = false;
    bool recordB = false;
    
};

//scribe specific
struct Tuning
{
    const float refFreq = 440; //(Hz) concert tuning
    const float semitone = std::pow(2, 1.0 / 12.0); //12 Tone equal temperament
    const int octaveSize = 12;

    const int lowExp = -57; // -57 = C0
    const int highExp = 50 - 12; // 50 = B8;
};

struct Audio
{
    static float historyTime() { return 0.0625f; }

    struct DownSample
    {
        const float srate = 8820;

        const int samples = srate * Audio::historyTime() + 1; //size of history not blocksize

        //for use with DCT
        //full signal start calculates octave 0 which isn't needed
        //half signal starts calculations on octave 1 which should be bass guitar range
        const int signalStart = 0;

        int factor = 0;
        float blockSize = 0;
    };

    int samples = 0; //size of history not blocksize
    float srate = 44100;
    int blockSize = 128;
    DownSample ds;
};

//~~~~~~~~~~~~~~~~~~~~~~



// 1. Pitchfork program data model~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct Pitchfork {

    void initialize(float srate, float blockSize);//set all required non-const variables here
    bool detectsPropertyChange(float srate, float blockSize);
    
    void updateFundamental(const AudioParams& params);


    bool isInitialized = false; //don't run PluginProcessor ready state loop without this set to true

    Tuning tuning;
    Audio audio;

    fvec frequencies = fvec(1 + tuning.highExp - tuning.lowExp, 0);
    fvec timeVectorDS  = fvec(audio.ds.samples, 0);

    const int harmonics = 32;
    const int freqHistSize = 600; //aiming for 60fps * 10 seconds of data
    const float fps = 60;
    int frameCounter = 0;
    int fpsBlocks = 1;
    float maxIndex = 0;

    
    cmatrix freqMatrixDS = cmatrix(frequencies.size(), cvec(audio.ds.samples, std::complex<float>(0, 0))); //don't forget to switch this back to audio.ds.samples if downsampling
    
    std::vector<cmatrix> harmonicMatrixDS = 
        std::vector<cmatrix>(
            frequencies.size(),
            cmatrix(harmonics, cvec(audio.ds.samples, std::complex<float>(0, 0))) //don't forget to switch this back to audio.ds.samples if downsampling
            );
    

    //Since I can't rely on a constant downsample rate, the matrix size is now known ahead of time and must be computed
    //during initialization with the signalHistory

    std::unique_ptr<Buffer<float> > signalHistory;
    std::unique_ptr<cmatrix> freqMatrix;
    std::unique_ptr<std::vector<cmatrix> > harmonicMatrix;
    std::unique_ptr<std::vector<float> > timeVector;

    fvec freqWeights = fvec(frequencies.size(), 0);
    fvec harmonicWeights = fvec(harmonics, 0);

    Buffer<std::vector<float> > freqHistA = Buffer<std::vector<float> >(freqHistSize, fvec(frequencies.size(), 0));
    Buffer<std::vector<float> > freqHistB = Buffer<std::vector<float> >(freqHistSize, fvec(frequencies.size(), 0));

    Buffer<std::vector<float> > harmHistA = Buffer<std::vector<float> >(freqHistSize, fvec(harmonics, 0));
    Buffer<std::vector<float> > harmHistB = Buffer<std::vector<float> >(freqHistSize, fvec(harmonics, 0));

    fvec historyDS = fvec(audio.ds.samples, 0.0001f);

};