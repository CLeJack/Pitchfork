#include "DCT.h"

// Discrete Customized Fourier Transform

void dct(fvec& weights, const cmatrix& matrix, const fvec& signal, 
int rowi, int rowf, int indi, int indf)
{
    rowi = std::max(rowi,0);
    rowf = std::min(rowf, (int)matrix.size());
    for(int row = rowi; row < rowf; row++)
    {
        std::complex<float> total(0.0f,0.0f);
        
        for(int sample = indi; sample < indf; sample++)
        {   
            total += signal[sample] * matrix[row][sample];
        }
        
        weights[row] = std::abs(total);
    }
}


fvec dct(const cmatrix& matrix, const fvec& signal, 
                int rowi, int rowf, int indi, int indf)
{
    rowi = std::max(rowi,0);
    rowf = std::min(rowf, (int)matrix.size());
    fvec weights(matrix.size(),0);
    for(int row = rowi; row < rowf; row++)
    {
        std::complex<float> total(0.0f,0.0f);
        
        for(int sample = indi; sample < indf; sample++)
        {   
            total += signal[sample] * matrix[row][sample];
        }
        
        weights[row] = std::abs(total);
    }
    return weights;
}

fvec dct(const cmatrix& matrix, const fvec& signal)
{   // can pass exMatrix with exWeights

    int rowf = matrix.size();
    int indf = signal.size();

    return dct( matrix, signal, 0, rowf, 0, indf);
}


void setComplexMatrix(cmatrix& matrix, const fvec& freqs, const fvec& timeVector)
{
    std::complex<float> iunit(0,1);
    for(int  row = 0; row < freqs.size(); row++)
    {
        float omega = 2 * MY_PI * freqs[row];
        

        for(int col = 0; col < timeVector.size(); col++)
        {
            matrix[row][col] = std::exp(iunit * (omega * timeVector[col]));
        }
    }
}

void setHarmonicMatrix(std::vector<cmatrix>& matrix, const fvec& freqs, const fvec& timeVector, int harmonics)
{
    //f0 = fundamnetal harmonic
    fvec harmonicVector(harmonics,0);
    for(int row = 0; row < freqs.size(); row++)
    {
        for(int h = 1; h < harmonics + 1; h++)
        {
            harmonicVector[h] = freqs[row] * h;
        }
        setComplexMatrix(matrix[row], harmonicVector, timeVector);
    }
}