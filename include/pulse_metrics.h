#ifndef WAVEFORMMANAGER_2_H
#define WAVEFORMMANAGER_2_H

#include <utility> // for std::pair
#include <limits>  // for std::numeric_limits

#include "TH1D.h"
#include "TSpectrum.h"

class WaveformManager_2 {
public:
    WaveformManager_2();
    ~WaveformManager_2();

    std::pair<double, double> FindMaxNegativePeak(TH1D* histogram);
    void CutWaveform(TH1D*& histogram, double peakPosition, double cutValue);
    std::pair<double, double> FindMaxPositivePeak(TH1D*& histogram, double peak_start_x);
    int FindStartBin(TH1D* histogram, int peakBin, double cutValue);
    int FindEndBin(TH1D* histogram, int peakBin, double cutValue);
    
private:
    Long64_t iWaveform;
};

#endif // WAVEFORMMANAGER_2_H
