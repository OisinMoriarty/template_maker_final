#ifndef WAVEFORM_UTILS_H
#define WAVEFORM_UTILS_H

#include "TTree.h"
#include "TH1F.h"
#include <map>

struct WaveformData {
    Double_t peakHeight;
    Double_t peak_time;
    Double_t avgBaseline;
    Double_t pulse_start;
    Double_t pulse_end;
    Long64_t waveformID;
};

// Function prototypes
std::map<Long64_t, TH1F*> createHistogramsFromTree(TTree* tree, WaveformData& data);
void createAveragedHistogram(const std::map<Long64_t, TH1F*>& histograms, const double peakTime, const std::string& outputFileName);

#endif // WAVEFORM_UTILS_H
