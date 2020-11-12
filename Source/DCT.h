#pragma once

#include "Head.h"

// Discrete Customized Fourier Transform

void dct(fvec& weights, const cmatrix& matrix, const fvec& signal, 
                int rowi, int rowf, int indi, int indf);

fvec dct(const cmatrix& matrix, const fvec& signal, 
                int rowi, int rowf, int indi, int indf);

fvec dct(const cmatrix& matrix, const fvec& signal);

void setComplexMatrix(cmatrix& matrix, const fvec& freqs, const fvec& timeVector);

void setHarmonicMatrix(std::vector<cmatrix>& matrix, const fvec& freqs, const fvec& timeVector, int harmonics = 32);