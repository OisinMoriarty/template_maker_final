#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "WaveformTools.h"

int main() {
    // define the cuts here
    std::string cutCase1 = "peakHeight < 50 && avgBaseline > 10";
    std::string cutCase2 = "peakHeight < 50 && avgBaseline > 10";
    std::string cutCase3 = "peakHeight < 50 && avgBaseline > 10";
    std::string cutCase4 = "peakHeight < 50 && avgBaseline > 10";

    std::array<std::string, 4> cuts = {cutCase1, cutCase2, cutCase3, cutCase4};

    TFile* inFile = TFile::Open("data.root", "READ");
    if (!inFile || inFile->IsZombie()) {
        std::cout << "Error: Cannot open file " << "data.root" << std::endl;
        return;
    }

    WaveformTools tools;

    for (int i = 0; i < 4; ++i) {
        // Load the TTree corresponding to the case.
        TTree* tree = (TTree*)inFile->Get(("Tree" + std::to_string(i+1)).c_str());

        // Apply the cut to the tree and get the new tree.
        TTree* cutTree = tools.applyCutToTree(tree, cuts[i]);

        // Create histograms from the tree.
        auto histograms = tools.createHistogramsFromTree(cutTree);

        // Create the averaged histogram.
        TH1D* averagedHistogram = tools.createAveragedHistogram(histograms);

        // Save the averaged histogram as a PDF.
        TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
        averagedHistogram->Draw();
        canvas->SaveAs(("averagedHistogramCase" + std::to_string(i+1) + ".pdf").c_str());
        delete canvas;

        delete averagedHistogram;
    }

    inFile->Close();
    delete inFile;

    return 0;
}
