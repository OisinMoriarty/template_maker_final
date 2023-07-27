#include "waveform_utils.h"
#include "TFile.h"
#include "TCanvas.h"

std::map<Long64_t, TH1F*> createHistogramsFromTree(TTree* tree, WaveformData& data) {
    std::map<Long64_t, TH1F*> histograms;

    Long64_t nEntries = tree->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        Long64_t nBins = data.pulse_end - data.pulse_start + 1;
        TH1F* hist = new TH1F(("waveform" + std::to_string(i)).c_str(), ("Waveform " + std::to_string(i)).c_str(), nBins, data.pulse_start, data.pulse_end);
        histograms[data.waveformID] = hist;
    }

    return histograms;
}

void createAveragedHistogram(const std::map<Long64_t, TH1F*>& histograms, const double peakTime, const std::string& outputFileName) {
    TH1F* avgHist = nullptr;

    for (const auto& pair : histograms) {
        TH1F* hist = pair.second;
        hist->GetXaxis()->SetRangeUser(peakTime - hist->GetNbinsX()/2, peakTime + hist->GetNbinsX()/2);
        
        if (avgHist == nullptr) {
            avgHist = (TH1F*)hist->Clone(("average_" + outputFileName).c_str());
        } else {
            avgHist->Add(hist);
        }
    }

    if (avgHist != nullptr) {
        avgHist->Scale(1.0/histograms.size());

        TFile outFile((outputFileName + ".root").c_str(), "RECREATE");
        avgHist->Write();
        outFile.Close();

        TCanvas canvas("canvas", "canvas", 800, 600);
        avgHist->Draw();
        canvas.SaveAs((outputFileName + ".pdf").c_str());

        delete avgHist;
    }

    return;
}
