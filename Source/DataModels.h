#pragma once
#include "Head.h"
#include "Waveforms.h"
#include "Tuning.h"
#include "CircularBuffer.h"
#include "Stats.h"
#include "DCT.h"
#include "ProcessData.h"


/*
* 
0. Shared Structs
1. Scribe Model
2. AudioParam Model
3. Calculations Model

*/

// 0.Shared Structs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



//scribe specific
struct Tuning
{
    const float refFreq = 440; //(Hz) concert tuning
    const float semitone = std::pow(2, 1.0 / 12.0); //12 Tone equal temperament
    const int octaveSize = 12;

    const int lowExp = -57; // -57 = C0
    const int highExp = 50; // 50 = B8; aliasing exist in octaves 7 to 8;
};

struct Audio
{
    static float historyTime() { return 0.0625f; }

    struct DownSample
    {
        const float srate = 8000;

        const int samples = srate * Audio::historyTime(); //size of history not blocksize

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
    
    void updateFundamental();


    bool isInitialized = false; //don't run PluginProcessor ready state loop without this set to true

    Tuning tuning;
    Audio audio;

    fvec frequencies = fvec(1 + tuning.highExp - tuning.lowExp, 0);

    fvec weights     = fvec(frequencies.size(), 0);
    
    fvec timeVector  = fvec(audio.ds.samples, 0);

    const int harmonics = 32;
    const int freqHistSize = 600; //aiming for 60fps * 10 seconds of data
    const float fps = 60;

    cmatrix freqMatrix = cmatrix(frequencies.size(), cvec(audio.ds.samples, std::complex<float>(0, 0)));
    
    std::vector<cmatrix> harmonicMatrix = 
        std::vector<cmatrix>(
            frequencies.size(),
            cmatrix(harmonics, cvec(audio.ds.samples, std::complex<float>(0, 0)))
            );

    std::unique_ptr<Buffer<float> > signalHistory;

    Buffer<std::vector<float> > freqHistA = Buffer<std::vector<float> >(freqHistSize, fvec(frequencies.size(), 0));
    Buffer<std::vector<float> > freqHistB = Buffer<std::vector<float> >(freqHistSize, fvec(frequencies.size(), 0));

    fvec historyDS = fvec(audio.ds.samples, 0.0001f);

};



// 2. AudioParam data model~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~
struct AudioParams
{
    float octaveRatio = 4;
    bool hoverInfoOn = true;
    bool record = false;
    int historyIndex = 0;
};

// 3. Calculations Data Model~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct Calculations
{

};