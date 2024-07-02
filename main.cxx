#include <iostream>
#include <string>
#include <vector>
#include <iterator>

#include "TString.h"
#include "TFile.h"
#include "TGraphErrors.h"

int main(int argc, char** argv) {
    // List of arguments (4):
    // :1: root file path
    // :2: task tag, like stat or sys
    // :3: centrality
    // :4: mode, 0 for rapidity scan and 1 for pT scan

    // note, please name the files list:
    // -> path/stat.y0p5.root 

    const char* path = argv[1];
    const char* tag = argv[2];
    int cent = TString(argv[3]).Atoi();
    int mode = TString(argv[4]).Atoi();

    std::cout << "[LOG] Path: " << path << std::endl;
    std::cout << "[LOG] Task tag: " << tag << std::endl;
    std::cout << "[LOG] Centrality: " << cent << std::endl;
    if (mode == 1) {
        std::cout << "[LOG] Mode: pT" << std::endl;
    } else {
        std::cout << "[LOG] Mode: y" << std::endl;
    }

    std::vector<std::string> files;

    const int nfs = 10;
    int nFiles;
    const int nCums = 61;
    TFile* tfs[nfs];
    TGraphErrors* tgs[nCums];
    double x[nCums][nfs] = { 0.0 };
    double y[nCums][nfs] = { 0.0 };
    double xerr[nCums][nfs] = { 0.0 };
    double yerr[nCums][nfs] = { 0.0 };

    std::vector<std::string> cumTags = {
        "Pro_C1", "Pro_C2", "Pro_C3", "Pro_C4", "Pro_C5", "Pro_C6", // 6
        "Pro_R21", "Pro_R31", "Pro_R32", "Pro_R42", "Pro_R51", "Pro_R62", // 6
        "Pro_k1", "Pro_k2", "Pro_k3", "Pro_k4", "Pro_k5", "Pro_k6", // 6
        "Pro_k21", "Pro_k31", "Pro_k41", "Pro_k51", "Pro_k61", // 5
        "Pbar_C1", "Pbar_C2", "Pbar_C3", "Pbar_C4", "Pbar_C5", "Pbar_C6", // 6
        "Pbar_R21", "Pbar_R31", "Pbar_R32", "Pbar_R42", "Pbar_R51", "Pbar_R62", // 6
        "Pbar_k1", "Pbar_k2", "Pbar_k3", "Pbar_k4", "Pbar_k5", "Pbar_k6", // 6
        "Pbar_k21", "Pbar_k31", "Pbar_k41", "Pbar_k51", "Pbar_k61", // 5
        "Netp_C1", "Netp_ppb", "Netp_C2", "Netp_C3", "Netp_C4", "Netp_C5", "Netp_C6", // 7
        "Netp_R21", "Netp_R2s", "Netp_R31", "Netp_R32", "Netp_R3s", // 5
        "Netp_R42", "Netp_R51", "Netp_R62" // 3
    };

    if (mode == 0) { // rapidity mode
        files = std::vector<std::string>(
            {"y0p1", "y0p2", "y0p3", "y0p4", "y0p5", "y0p6"}
        );
        nFiles = 6;
        double xarr[6] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 };

        int iFile = 0;
        for (auto iter : files) {
            tfs[iFile] = new TFile(
                Form(
                    "%s/%s.%s.root", 
                    path, tag, iter.c_str()
                )
            );
            int iCum = 0;
            for (auto iterc : cumTags) {
                auto tg = (TGraphErrors*)tfs[iFile]->Get(iterc.c_str());
                x[iCum][iFile] = xarr[iFile];
                xerr[iCum][iFile] = 0.0;
                y[iCum][iFile] = tg->GetPointY(cent);
                yerr[iCum][iFile] = tg->GetErrorY(cent);
                iCum ++;
            }
            iFile ++;
        }

        TFile* tfout = new TFile(Form("outputs/%s.cent%d.y.root", tag, cent), "recreate");
        tfout->cd();
        int iCum = 0;
        for (auto iterc : cumTags) {
            tgs[iCum] = new TGraphErrors(nFiles, x[iCum], y[iCum], xerr[iCum], yerr[iCum]);
            tgs[iCum]->SetName(iterc.c_str());
            tgs[iCum]->Write();
            iCum ++;
        };
        tfout->Close();
        std::cout << "[LOG] Rapidity scan finished!" << std::endl;
    } else if (mode == 1) { // pT scan
        files = std::vector<std::string>(
            {"pt0p8", "pt1p0", "pt1p2", "pt1p4", "pt1p6", "pt1p8", "y0p5"}
        );
        nFiles = 7;
        double xarr[7] = { 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2.0 };

        int iFile = 0;
        for (auto iter : files) {
            tfs[iFile] = new TFile(
                Form(
                    "%s/%s.%s.root", 
                    path, tag, iter.c_str()
                )
            );
            int iCum = 0;
            for (auto iterc : cumTags) {
                auto tg = (TGraphErrors*)tfs[iFile]->Get(iterc.c_str());
                x[iCum][iFile] = xarr[iFile];
                xerr[iCum][iFile] = 0.0;
                y[iCum][iFile] = tg->GetPointY(cent);
                yerr[iCum][iFile] = tg->GetErrorY(cent);
                iCum ++;
            }
            iFile ++;
        }

        TFile* tfout = new TFile(Form("outputs/%s.cent%d.pt.root", tag, cent), "recreate");
        tfout->cd();
        int iCum = 0;
        for (auto iterc : cumTags) {
            tgs[iCum] = new TGraphErrors(nFiles, x[iCum], y[iCum], xerr[iCum], yerr[iCum]);
            tgs[iCum]->SetName(iterc.c_str());
            tgs[iCum]->Write();
            iCum ++;
        };
        tfout->Close();
        std::cout << "[LOG] pT scan finished!" << std::endl;

    }

    return 0;
}