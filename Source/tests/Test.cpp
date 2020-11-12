#pragma once
#include "ModelInstances.h"
#include "Debug.h"

#define PRINT 1


int main()
{

    float srate = 44100;
    float blockSize = 128;

    pitchfork.initialize(srate, blockSize);

    fvec signal0 = importCsv("input/_test_a3.csv", srate);

    #if PRINT == 1
    printRows(pitchfork.frequencies, "output/_0_freqs.csv");
    printColumn(*pitchfork.timeVector.get(), "output/_0_timeVector.csv");
    printMatrixReal(*pitchfork.freqMatrix.get(), "output/_0_cmatrix.csv",0);
    printColumn(pitchfork.timeVectorDS, "output/_0_timeVectorDS.csv");
    printMatrixReal(pitchfork.freqMatrixDS, "output/_0_cmatrixDS.csv",0);

    #endif

    int loops = (signal0.size() / pitchfork.audio.blockSize);
    int start = 0;
    int end = 0;

    for(int i = 0; i < loops; i++)
    {
        fvec block(pitchfork.audio.blockSize, 0);
        int offset = i * pitchfork.audio.blockSize;
        for(int i = 0; i < pitchfork.audio.blockSize; i++)
        {
            //continually pushing to the down sample history gives a really bad signal
            //even with filtering, so use the full history and down sample from there always
            //pitchfork.signalHistory.get()->push( 
            //    SMA(pitchfork.signalHistory->lastValue(),signal0[i + offset], 
            //    pitchfork.audio.ds.srate/pitchfork.audio.srate ));
            pitchfork.signalHistory.get()->push(signal0[i + offset]);
        }
        
        fvec trueSignal = pitchfork.signalHistory.get()->toOrderedVec();

        for(int i = 0; i < pitchfork.historyDS.size(); i++)
        {
            pitchfork.historyDS[i] = trueSignal[i * pitchfork.audio.ds.factor];
        }


        pitchfork.updateFundamental(params);
        

        
#if PRINT == 1

        fvec values{
            pitchfork.maxIndex
            };
            

        printRows( trueSignal, "output/_2_history.csv");
        printRows( pitchfork.historyDS, "output/_2_historyDS.csv");
        printRows( pitchfork.freqWeights, "output/_2_weights.csv");
        printRows(pitchfork.harmonicWeights, "output/_2_hweights.csv");
        printRows(values, "output/_2_values.csv");
        //

#elif PRINT == 2

    std::cout<<(float)f0ind <<", "<<
            midiNum <<", "<<
            comy <<", "<<
            ampDiff <<", "<<
            metric <<", "<<
            (float)message.on <<", "<<
            (float)message.onVel <<", "<<
            (float)message.off <<", "<<
            (float)message.offVel <<", "<<
            (float) message.send <<", "<<
            midiSwitch.notes.current <<", "<<
            midiSwitch.notes.prev <<", "<<
            (float)midiSwitch.state <<"\n";
#endif
    }
    return 0;
}

#undef PRINT
