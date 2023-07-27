#include "WaveformManager_2.h"

#include <utility> // for std::pair
#include <limits>  // for std::numeric_limits

#include "TH1D.h"
#include "TSpectrum.h"


struct WaveformData {
    Double_t peakHeight;
    Double_t peak_time;
    Double_t avgBaseline;
    Double_t pulse_start;
    Double_t pulse_end;
    Long64_t waveformID;
};

int main()
{
    TFile *file = new TFile("data.root", "RECREATE"); // Create a new ROOT file to save the data

    TDirectory* dirCase1 = file->mkdir("Case1");
    TDirectory* dirCase2 = file->mkdir("Case2");
    TDirectory* dirCase3 = file->mkdir("Case3");
    TDirectory* dirCase4 = file->mkdir("Case4");

    WaveformData dataCase1, dataCase2, dataCase3, dataCase4;
    
    TTree* treeCase1 = new TTree("Tree1", "Data from Case 1");
    treeCase1->Branch("WaveformData", &dataCase1, "peakHeight/D:peak_time:avgBaseline:pulse_start:pulse_end:waveformID/L");

    TTree* treeCase2 = new TTree("Tree2", "Data from Case 2");
    treeCase2->Branch("WaveformData", &dataCase2, "peakHeight/D:peak_time:avgBaseline:pulse_start:pulse_end:waveformID/L");
  
    TTree* treeCase3 = new TTree("Tree3", "Data from Case 3");
    treeCase3->Branch("WaveformData", &dataCase3, "peakHeight/D:peak_time:avgBaseline:pulse_start:pulse_end:waveformID/L");

    TTree* treeCase4 = new TTree("Tree4", "Data from Case 4");
    treeCase4->Branch("WaveformData", &dataCase4, "peakHeight/D:peak_time:avgBaseline:pulse_start:pulse_end:waveformID/L");

    const std::string m_file_name = "/sbnd/app/users/moriarty/workdir_2/my_larsoft/WF_processing/muon_hitdumper.root";

    DataLoader loader(m_file_name);

    for (int i = 0; i < 11223; ++i) {
        TH1D* hist = loader.loadWaveform(i);
        int tpc = loader.getTPC(i);
        int plane = loader.getPlane(i);

        int composite = tpc * 10 + plane;

        double baseline = CalculateBaseline(hist,0,100);
        WaveformManager_2* wm = new WaveformManager_2(0, hist); 

        switch (composite) {
            case 1:  // tpc = 0, plane = 1
                dirCase1->cd();

                std::pair<double, double> negativePeak1 = wm->FindMaxNegativePeak(hist);
                dataCase1.peakHeight = negativePeak1.second;
                dataCase1.peak_time = negativePeak1.first;
                dataCase1.avgBaseline = baseline;
                dataCase1.pulse_start = wm->FindStartBin(hist, negativePeak1.first, baseline);
                dataCase1.pulse_end = wm->FindEndBin(hist, negativePeak1.first, baseline);
                dataCase1.waveformID = i;

                treeCase1->Fill();

                dirCase2->cd();

                std::pair<double, double> positivePeak1 = wm->FindMaxPositivePeak(hist, negativePeak1.first);
                dataCase2.peakHeight = positivePeak1.second;
                dataCase2.peak_time = positivePeak1.first;
                dataCase2.avgBaseline = baseline;
                dataCase2.pulse_start = wm->FindStartBin(hist, positivePeak1.first, baseline);
                dataCase2.pulse_end = wm->FindEndBin(hist, positivePeak1.first, baseline);
                dataCase2.waveformID = i;

                treeCase2->Fill();
                break;

            case 11:  // tpc = 1, plane = 1
                dirCase3->cd();

                std::pair<double, double> negativePeak2 = wm->FindMaxNegativePeak(hist);
                dataCase3.peakHeight = negativePeak2.second;
                dataCase3.peak_time = negativePeak2.first;
                dataCase3.avgBaseline = baseline;
                dataCase3.pulse_start = wm->FindStartBin(hist, negativePeak2.first, baseline);
                dataCase3.pulse_end = wm->FindEndBin(hist, negativePeak2.first, baseline);
                dataCase3.waveformID = i;

                treeCase3->Fill();

                dirCase4->cd();

                std::pair<double, double> positivePeak2 = wm->FindMaxPositivePeak(hist, negativePeak2.first);
                dataCase4.peakHeight = positivePeak2.second;
                dataCase4.peak_time = positivePeak2.first;
                dataCase4.avgBaseline = baseline;
                dataCase4.pulse_start = wm->FindStartBin(hist, positivePeak2.first, baseline);
                dataCase4.pulse_end = wm->FindEndBin(hist, positivePeak2.first, baseline);
                dataCase4.waveformID = i;

                treeCase4->Fill();
                break;

            default:
                // Handle unexpected cases
                break;
        }

        delete wm;
    }

    file->Write();
    file->Close();

    return 0;
}
