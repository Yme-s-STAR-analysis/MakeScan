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
    // :4: var mode, [rap] [y] [rapidity] or [pt] [pT]
    // :5: scan mode, 0 for traditional (accumulative), 1 for shifting (differential)
    // :6: highest order, 4 or 6

    // note, please name the files list:
    // -> path/stat.y0p5.root 

    const char* path = argv[1];
    const char* tag = argv[2];
    int cent = TString(argv[3]).Atoi();
    std::string modeStr(argv[4]);
    int mode;
    int scan = TString(argv[5]).Atoi();

    std::cout << "[LOG] Path: " << path << std::endl;
    std::cout << "[LOG] Task tag: " << tag << std::endl;
    std::cout << "[LOG] Centrality: " << cent << std::endl;
    if (modeStr == "rap" || modeStr == "y" || modeStr == "rapidity") { mode = 0; }
    else if (modeStr == "pT" || modeStr == "pt") { mode = 1; }
    else { mode = -1; }
    if (mode == 0) {
        std::cout << "[LOG] Mode: y" << std::endl;
    } else if (mode == 1) {
        std::cout << "[LOG] Mode: pT" << std::endl;
    } else {
        std::cout << "[ERROR] Invalid mode code (" << modeStr << ")" << std::endl;
        return -1;
    }
    if (scan == 0) {
        std::cout << "[LOG] Accumulative scan" << std::endl;
    } else if (scan == 1) {
        std::cout << "[LOG] Differential scan" << std::endl;
    } else {
        std::cout << "[ERROR] Invalid scan code (" << scan << ")" << std::endl;
        return -1;
    }

    std::vector<std::string> files;

    const int nfs = 10;
    int nFiles;
    const int nCums = 100;
    TFile* tfs[nfs];
    TGraphErrors* tgs[nCums];
    double x[nCums][nfs] = { 0.0 };
    double y[nCums][nfs] = { 0.0 };
    double xerr[nCums][nfs] = { 0.0 };
    double yerr[nCums][nfs] = { 0.0 };

    std::vector<std::string> cumTags;
    
    double hOrder = TString(argv[6]).Atoi();
    if (hOrder == 4) {
        std::cout << "[LOG] In the scan, up to 4th order will be considered." << std::endl;
    } else if (hOrder == 6) {
        std::cout << "[LOG] In the scan, up to 6th order will be considered." << std::endl;
    } else {
        std::cout << "[ERROR] Invalid highest order (" << argv[6] << ")" << std::endl;
        return -1;
    }

    if (hOrder == 6) {
        cumTags = std::vector<std::string>({
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
            "Netp_R42", "Netp_R51", "Netp_R62" // 3,
            #ifdef __CBWCALTER__
            // some more, with CBWC method2
            ,"Pro_R21r", "Pro_R31r", "Pro_R32r", "Pro_R42r", "Pro_R51r", "Pro_R62r", // 6
            "Pro_k21r", "Pro_k31r", "Pro_k41r", "Pro_k51r", "Pro_k61r", // 5
            "Pbar_R21r", "Pbar_R31r", "Pbar_R32r", "Pbar_R42r", "Pbar_R51r", "Pbar_R62r", // 6
            "Pbar_k21r", "Pbar_k31r", "Pbar_k41r", "Pbar_k51r", "Pbar_k61r", // 5
            "Netp_R21r", "Netp_R2sr", "Netp_R31r", "Netp_R32r", "Netp_R3sr", // 5
            "Netp_R42r", "Netp_R51r", "Netp_R62r" // 3
            #endif
        });
    } else {
        cumTags = std::vector<std::string>({
            "Pro_C1", "Pro_C2", "Pro_C3", "Pro_C4", // 4
            "Pro_R21", "Pro_R31", "Pro_R32", "Pro_R42", // 4
            "Pro_k1", "Pro_k2", "Pro_k3", "Pro_k4",  // 4
            "Pro_k21", "Pro_k31", "Pro_k41", // 3
            "Pbar_C1", "Pbar_C2", "Pbar_C3", "Pbar_C4", // 4
            "Pbar_R21", "Pbar_R31", "Pbar_R32", "Pbar_R42", // 4
            "Pbar_k1", "Pbar_k2", "Pbar_k3", "Pbar_k4", // 4
            "Pbar_k21", "Pbar_k31", "Pbar_k41", // 3
            "Netp_C1", "Netp_ppb", "Netp_C2", "Netp_C3", "Netp_C4", // 5
            "Netp_R21", "Netp_R2s", "Netp_R31", "Netp_R32", "Netp_R3s", "Netp_R42" // 6
            #ifdef __CBWCALTER__
            // some more, with CBWC method2
            ,"Pro_R21r", "Pro_R31r", "Pro_R32r", "Pro_R42r", // 4
            "Pro_k21r", "Pro_k31r", "Pro_k41r", // 3
            "Pbar_R21r", "Pbar_R31r", "Pbar_R32r", "Pbar_R42r", // 4
            "Pbar_k21r", "Pbar_k31r", "Pbar_k41r", // 3
            "Netp_R21r", "Netp_R2sr", "Netp_R31r", "Netp_R32r", "Netp_R3sr", "Netp_R42r" // 6
            #endif
        });
    }

    if (mode == 0 && scan == 0) { // rapidity + accumulative
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
                y[iCum][iFile] = double(tg->GetPointY(cent));
                yerr[iCum][iFile] = double(tg->GetErrorY(cent));
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
        std::cout << "[LOG] Accumulative rapidity scan finished!" << std::endl;

    } else if (mode == 1 && scan == 0) { // pT + accumulative
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
                y[iCum][iFile] = double(tg->GetPointY(cent));
                yerr[iCum][iFile] = double(tg->GetErrorY(cent));
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
        std::cout << "[LOG] Accumulative pT scan finished!" << std::endl;

    } else if (mode == 0 && scan == 1) { // rapidity differential
        files = std::vector<std::string>(
            {"y0p1", "yf0p2", "yf0p3", "yf0p4", "yf0p5", "yf0p6"}
        );
        nFiles = 6;
        double xarr[6] = { 0.05, 0.15, 0.25, 0.35, 0.45, 0.55 };

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
                y[iCum][iFile] = double(tg->GetPointY(cent));
                yerr[iCum][iFile] = double(tg->GetErrorY(cent));
                iCum ++;
            }
            iFile ++;
        }

        TFile* tfout = new TFile(Form("outputs/%s.cent%d.yf.root", tag, cent), "recreate");
        tfout->cd();
        int iCum = 0;
        for (auto iterc : cumTags) {
            tgs[iCum] = new TGraphErrors(nFiles, x[iCum], y[iCum], xerr[iCum], yerr[iCum]);
            tgs[iCum]->SetName(iterc.c_str());
            tgs[iCum]->Write();
            iCum ++;
        };
        tfout->Close();
        std::cout << "[LOG] Differential rapidity scan finished!" << std::endl;

    } else if (mode == 1 && scan == 1) { // pT differential
        files = std::vector<std::string>(
            {"ptf0p6", "ptf0p8", "ptf1p0", "ptf1p2", "ptf1p4", "ptf1p6", "ptf1p8", "ptf2p0"}
        );
        nFiles = 8;
        double xarr[8] = { 0.5, 0.7, 0.9, 1.1, 1.3, 1.5, 1.7, 1.9 };

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
                y[iCum][iFile] = double(tg->GetPointY(cent));
                yerr[iCum][iFile] = double(tg->GetErrorY(cent));
                iCum ++;
            }
            iFile ++;
        }

        TFile* tfout = new TFile(Form("outputs/%s.cent%d.ptf.root", tag, cent), "recreate");
        tfout->cd();
        int iCum = 0;
        for (auto iterc : cumTags) {
            tgs[iCum] = new TGraphErrors(nFiles, x[iCum], y[iCum], xerr[iCum], yerr[iCum]);
            tgs[iCum]->SetName(iterc.c_str());
            tgs[iCum]->Write();
            iCum ++;
        };
        tfout->Close();
        std::cout << "[LOG] Differential pT scan finished!" << std::endl;
    }

    return 0;
}
