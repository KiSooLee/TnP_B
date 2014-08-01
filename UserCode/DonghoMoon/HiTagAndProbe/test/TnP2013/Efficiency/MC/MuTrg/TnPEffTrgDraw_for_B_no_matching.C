#include <iostream>
#include <TSystem.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TPave.h>
#include <TText.h>
#include <fstream>
#include <sstream>
#include <string.h>

#include <TROOT.h>
#include <TFile.h>
#include <TGraphAsymmErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>

#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooArgSet.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>


using namespace RooFit;
using namespace std;


// Function Define
TH2F *plotEff2D(RooDataSet *a, TString b);
TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa);
TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa);
void formatTH1F(TH1* a, int b, int c, int d);
void formatTGraph(TGraph* a, int b, int c, int d);
void formatTLeg(TLegend* a);
void TnPEffTrgDraw_for_B ();

// From here you need to set up your environments.
bool MC_ = false; // if the data set is real data, it should be false
bool Cent_ = false; // if the data is only one MinBias file, it shoud be false
const int maxFile_ = 1; // the number of data files, usually it should be 4 but 1 in the case of MinBias
int TrgMode_ = 1; // 1 = L1, 2 = L2

char *outfile_ = "Jpsi_pPb_RD_MuTrgNew2CS_CBGpExp_1st_Run_Eff_Mu3_for_B_test.root"; // L1 or L2
//char *outfile_ = "Jpsi_pPb_MC_MuTrgNew2CS_CBGpExp_1st_Run_Eff_Mu3_for_B_test.root"; // L1 or L2
string dir_suffix = "_cbGaussPlusExpo"; // depends on which fit function is used for : eg) _gaussExpo, _gaussPol2

void TnPEffTrgDraw_for_B() {
    gROOT->Macro("rootlogon.C");

    char *files[maxFile_] = {
        "tnp_pPb_Ana_MuTrgNew2CS_CBGpExp_1st_Run_All_Sgl_for_B_test.root"
//        "tnp_pPb_Ana_MuTrgNew2CS_CBGpExp_MC_All_Sgl_for_B_test.root"
    };

    TString outname_in, outname_mid, outname_out;
    TString middle_name, middle_name2;
    TString leg_title; 
    int Mode = 0;
    TFile *outfile = new TFile(outfile_,"RECREATE");

    for(int l = 0; l < maxFile_; l++){

        char *infile = files[l];
        TFile *f = new TFile(infile);

        Mode = l;

        if(Mode == 0){ 
            middle_name = "All";
            middle_name2 = "MinBias";
        }else if(Mode == 1){ 
            middle_name = "0005";
            middle_name2 = "0 - 5 %";
        }else if(Mode == 2){ 
            middle_name = "0510";
            middle_name2 = "5 - 10 %";
        }else if(Mode == 3){ 
            middle_name = "1020";
            middle_name2 = "10 - 20 %";
        }else if(Mode == 4){ 
            middle_name = "2030";
            middle_name2 = "20 - 30 %";
        }else if(Mode == 5){ 
            middle_name = "3040";
            middle_name2 = "30 - 40 %";
        }else if(Mode == 6){ 
            middle_name = "4050";
            middle_name2 = "40 - 50 %";
        }else if(Mode == 7){ 
            middle_name = "5060";
            middle_name2 = "50 - 60 %";
        }else if(Mode == 8){ 
            middle_name = "60100";
            middle_name2 = "60 - 100 %";
        }

        gROOT->SetStyle("Plain");
        gStyle->SetOptStat(0);
        gStyle->SetTitle(0);


        // if you have only one file, should change the number 4 -> 1

        TString mid_title = "Centrality : (" + middle_name2 + ")";
        if(!MC_) leg_title = "Data Trigger Efficiency (" + middle_name2 + ")";
        if(MC_) leg_title = "MC Trigger Efficiency (" + middle_name2 + ")";

        TCanvas *c1 = new TCanvas("c1","",120,20,800,600);

        f->cd();
        RooDataSet *daTrgEta, *daTrgEta1bin;
        if(TrgMode_ == 1){
            daTrgEta = (RooDataSet*)f->Get("MuonTrgNew2CS/PAMu3_eta/fit_eff");
            daTrgEta1bin = (RooDataSet*)f->Get("MuonTrgNew2CS/PAMu3_1bin_pt_eta/fit_eff");
        }else{
            daTrgEta = (RooDataSet*)f->Get("MuonTrgNew2/HLTL2_eta/fit_eff");
            daTrgEta1bin = (RooDataSet*)f->Get("MuonTrgNew2/HLTL2_1bin_pt_eta/fit_eff");
        }
        TGraphAsymmErrors *Trg_eta = plotEffEta(daTrgEta, 1);
        TGraphAsymmErrors *Trg_1bin_pt_eta = plotEffEta(daTrgEta1bin, 1);

        double Trg[4];
        Trg_1bin_pt_eta->GetPoint(0,Trg[0],Trg[1]);
        Trg[2] = Trg_1bin_pt_eta->GetErrorYhigh(0);
        Trg[3] = Trg_1bin_pt_eta->GetErrorYlow(0);

        RooDataSet *daTrgPt;
        if(TrgMode_ == 1) daTrgPt = (RooDataSet*)f->Get("MuonTrgNew2CS/PAMu3_pt/fit_eff");
        if(TrgMode_ == 2) daTrgPt = (RooDataSet*)f->Get("MuonTrgNew2/HLTL2_pt/fit_eff");
        TGraphAsymmErrors *Trg_pt = plotEffPt(daTrgPt, 1);

        /*
        RooDataSet *daTrgD = (RooDataSet*)f->Get("MuonTrg/HLTL1_pt_eta/fit_eff");
        TString twoDName1 = "Trg_2d";
        TH2F *Trg_2d = plotEff2D(daTrgD, twoDName1);
        */

        Trg_eta->GetXaxis()->SetLabelSize(0.05);
        Trg_eta->GetYaxis()->SetLabelSize(0.05);
        Trg_eta->GetXaxis()->SetTitleSize(0.05);
        Trg_eta->GetYaxis()->SetTitleSize(0.05);
        Trg_eta->GetXaxis()->SetTitleOffset(0.9);
        Trg_eta->GetYaxis()->SetTitleOffset(0.8);
        Trg_eta->SetMarkerColor(kRed+2);
        Trg_eta->Draw("apz");

        char legs[512];
        TLegend *leg1 = new TLegend(0.3054774,0.1567832,0.5429648,0.3858042,NULL,"brNDC");
        leg1->SetFillStyle(0);
        leg1->SetFillColor(0);
        leg1->SetBorderSize(0);
        leg1->SetTextSize(0.04);
        leg1->SetHeader(leg_title);
        sprintf(legs,"HLT_HIL1DoubleMuOpen : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1],Trg[2],Trg[3]);
        leg1->AddEntry(Trg_eta,legs,"pl");
        leg1->Draw("same");
        TString pic_name_png, pic_name_pdf;

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_Trg_New2CS_Mu3_" + middle_name + "_eta_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_Trg_New2CS_Mu3_" + middle_name + "_eta_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_Trg_New2CS_1st_run_Mu3_" + middle_name + "_eta_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_Trg_New2CS_1st_run_Mu3_" + middle_name + "_eta_for_B_test.pdf";
        }
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);

        Trg_pt->GetXaxis()->SetLabelSize(0.05);
        Trg_pt->GetYaxis()->SetLabelSize(0.05);
        Trg_pt->GetXaxis()->SetTitleSize(0.05);
        Trg_pt->GetYaxis()->SetTitleSize(0.05);
        Trg_pt->GetXaxis()->SetTitleOffset(0.9);
        Trg_pt->GetYaxis()->SetTitleOffset(0.8);
        Trg_pt->SetMarkerColor(kRed+2);
        Trg_pt->Draw("apz");

        TLegend *leg2 = new TLegend(0.3454774,0.2167832,0.5429648,0.4458042,NULL,"brNDC");
        leg2->SetFillStyle(0);
        leg2->SetFillColor(0);
        leg2->SetBorderSize(0);
        leg2->SetTextSize(0.04);
        leg2->SetHeader(leg_title);
//        sprintf(legs,"HLT_HIL1DoubleMuOpen : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1],Trg[2],Trg[3]);
        sprintf(legs,"HLT_PASingleMu3 : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1],Trg[2],Trg[3]);
        leg2->AddEntry(Trg_pt,legs,"pl");
        leg2->Draw("same");

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_Trg_New2CS_Mu3_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_Trg_New2CS_Mu3_" + middle_name + "_pt_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_Trg_New2CS_1st_run_Mu3_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_Trg_New2CS_1st_run_Mu3_" + middle_name + "_pt_for_B_test.pdf";
        }
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);

        /*
        Trg_2d->Draw();
        pic_name_png = "Jpsi_MC_Trg_" + middle_name + "_2d.png";
        pic_name_pdf = "Jpsi_MC_Trg_" + middle_name + "_2d.pdf";
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);
        c1->Print("Jpsi_MC_Trg_Eff_total.ps");
        */

        TString Trgna1, Trgna2, Trgna3, Trgna4;

        Trgna1 = "Trg_eta_" + middle_name; 
        Trgna2 = "Trg_1bin_pt_eta_" + middle_name; 
        Trgna3 = "Trg_pt_" + middle_name; 
        Trgna4 = "Trg_2d_" + middle_name; 

        Trg_eta->SetName(Trgna1);
        Trg_1bin_pt_eta->SetName(Trgna2);
        Trg_pt->SetName(Trgna3);
        //Trg_2d->SetName(Trgna4);

        outfile->cd();
        Trg_eta->Write();
        Trg_pt->Write();
        Trg_1bin_pt_eta->Write();
        //Trg_2d->Write();
    }

    // Comparing Plots as the Centralities

    TH1F *hPadEta = new TH1F("hPadEta","",15,-2.4,2.4);
    TH1F *hPadPt = new TH1F("hPadPt","",12,0,30);

    TCanvas *c2 = new TCanvas("c2","",120,20,800,600);
    //c2->cd();

    TGraphAsymmErrors *gTrg[maxFile_];
    TGraphAsymmErrors *gTrgEta[maxFile_];
    TGraphAsymmErrors *gTrgPt[maxFile_];

    double Trg[maxFile_][4];
    for(int i = 0; i < maxFile_; i++){

        TFile finput(files[i]);
        finput.cd();
        RooDataSet *dataTrg, *dataTrgEta, *dataTrgPt;
        if(TrgMode_ == 1){
            dataTrg = (RooDataSet*)finput.Get("MuonTrgNew2CS/PAMu3_1bin_pt_eta/fit_eff");
            dataTrgEta = (RooDataSet*)finput.Get("MuonTrgNew2CS/PAMu3_eta/fit_eff");
            dataTrgPt = (RooDataSet*)finput.Get("MuonTrgNew2CS/PAMu3_pt/fit_eff");
        }else{
            dataTrg = (RooDataSet*)finput.Get("MuonTrgNew2/HLTL2_1bin_pt_eta/fit_eff");
            dataTrgEta = (RooDataSet*)finput.Get("MuonTrgNew2/HLTL2_eta/fit_eff");
            dataTrgPt = (RooDataSet*)finput.Get("MuonTrgNew2/HLTL2_pt/fit_eff");
        }

        gTrg[i] = plotEffEta(dataTrg, 0);
        gTrgEta[i] = plotEffEta(dataTrgEta, 0);
        gTrgPt[i] = plotEffPt(dataTrgPt, 0);

        gTrg[i]->GetPoint(0,Trg[i][0],Trg[i][1]);
        Trg[i][2] = gTrg[i]->GetErrorYhigh(0);
        Trg[i][3] = gTrg[i]->GetErrorYlow(0);
    }

    TGraphAsymmErrors *pCNT = new TGraphAsymmErrors(3);
    TGraphAsymmErrors *pMBCNT = new TGraphAsymmErrors();

    if(Cent_){
        pCNT->SetPoint(0,10,Trg[1][1]);
        pCNT->SetPointError(0,0,0,Trg[1][3],Trg[1][2]);

        pCNT->SetPoint(1,20,Trg[2][1]);
        pCNT->SetPointError(1,0,0,Trg[2][3],Trg[2][2]);

        pCNT->SetPoint(2,30,Trg[3][1]);
        pCNT->SetPointError(2,0,0,Trg[3][3],Trg[3][2]);

        pCNT->SetPoint(3,40,Trg[4][1]);
        pCNT->SetPointError(3,0,0,Trg[4][3],Trg[4][2]);

        pCNT->SetPoint(4,50,Trg[4][1]);
        pCNT->SetPointError(3,0,0,Trg[4][3],Trg[4][2]);

        pCNT->SetPoint(5,60,Trg[5][1]);
        pCNT->SetPointError(3,0,0,Trg[5][3],Trg[5][2]);

        pCNT->SetPoint(6,70,Trg[6][1]);
        pCNT->SetPointError(3,0,0,Trg[6][3],Trg[6][2]);

        pCNT->SetPoint(7,80,Trg[7][1]);
        pCNT->SetPointError(3,0,0,Trg[7][3],Trg[7][2]);

        pCNT->SetPoint(8,90,Trg[8][1]);
        pCNT->SetPointError(3,0,0,Trg[8][3],Trg[8][2]);

        pCNT->SetMarkerColor(kRed+2);
        pCNT->SetMarkerStyle(20);
    }
    pMBCNT->SetPoint(0,113.0558,Trg[0][1]);
    pMBCNT->SetPointError(0,0,0,Trg[0][3],Trg[0][2]);

    pMBCNT->SetMarkerColor(kRed+2);
    pMBCNT->SetMarkerStyle(24);

    gStyle->SetOptStat(0);
    TH1F *hPad = new TH1F("hPad","",40,0,40);
    hPad->SetTitle("");
    formatTH1F(hPad,1,1,4);

    hPad->Draw();
    hPad->GetYaxis()->SetTitle("Efficiency");
    hPad->GetXaxis()->SetTitle("Centrality bin");
    if(Cent_) pCNT->Draw("pz same");
    pMBCNT->Draw("pz same");


    char legs[512];
    TLegend *leg1 = new TLegend(0.369849,0.1853147,0.6809045,0.4527972,NULL,"brNDC"); // Size 0.03
    leg1->SetFillStyle(0);
    leg1->SetFillColor(0);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.03);
    leg1->SetHeader(leg_title);
    if(Cent_) leg1->AddEntry(pCNT,"HLT_HIL1DoubleMuOpen","pl");
    sprintf(legs,"HLT_HIL1DoubleMuOpen (MinBias) : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[0][1],Trg[0][2],Trg[0][3]);
    leg1->AddEntry(pMBCNT,legs,"pl");
    leg1->Draw("same");

    if(MC_){
        if(TrgMode_ == 1){
/*
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trg_New3_L1_CNT.png");
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trg_New3_L1_CNT.pdf");
*/
        }else{
/*
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trg_New3_L2_CNT.png");
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trg_New3_L2_CNT.pdf");
*/
        }
    }else{
        if(TrgMode_ == 1){
/*
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2Rfb_1st_run_L1_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2Rfb_1st_run_L1_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2Rfb_2nd_run_L1_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2Rfb_2nd_run_L1_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2CS_1st_run_L1_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2CS_1st_run_L1_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2CS_2nd_run_L1_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_TrgNew2CS_2nd_run_L1_CNT_test.pdf");
*/
        }else{
/*
            c2->SaveAs("Jpsi_pPb_for_B_RD_TrgNew2_L2_CNT.png");
            c2->SaveAs("Jpsi_pPb_for_B_RD_TrgNew2_L2_CNT.pdf");
*/
        }
    }

    if(Cent_) {
        gTrgEta[1]->SetMarkerStyle(20);
        gTrgEta[1]->SetMarkerColor(634);
        gTrgEta[1]->GetXaxis()->SetTitle("#eta");;
        gTrgEta[1]->GetXaxis()->CenterTitle();;
        gTrgEta[2]->SetMarkerStyle(23);
        gTrgEta[2]->SetMarkerColor(602);
        gTrgEta[3]->SetMarkerStyle(24);
        gTrgEta[3]->SetMarkerColor(618);
        gTrgEta[4]->SetMarkerStyle(25);
        gTrgEta[4]->SetMarkerColor(620);
        gTrgEta[5]->SetMarkerStyle(26);
        gTrgEta[5]->SetMarkerColor(621);
        gTrgEta[6]->SetMarkerStyle(27);
        gTrgEta[6]->SetMarkerColor(622);
        gTrgEta[7]->SetMarkerStyle(28);
        gTrgEta[7]->SetMarkerColor(623);
        gTrgEta[8]->SetMarkerStyle(29);
        gTrgEta[8]->SetMarkerColor(624);
        gTrgEta[0]->SetMarkerStyle(21);
        gTrgEta[0]->SetMarkerColor(802);

        formatTH1F(hPadEta,1,1,2);
        hPadEta->Draw();
        gTrgEta[1]->SetMinimum(0.5);
        gTrgEta[1]->SetMaximum(1.05);
        gTrgEta[1]->Draw("pz same");
        gTrgEta[2]->Draw("pz same");
        gTrgEta[3]->Draw("pz same");
        gTrgEta[4]->Draw("pz same");
        gTrgEta[5]->Draw("pz same");
        gTrgEta[6]->Draw("pz same");
        gTrgEta[7]->Draw("pz same");
        gTrgEta[8]->Draw("pz same");
        gTrgEta[0]->Draw("pz same");

        TLegend *leg2 = new TLegend(0.4899497,0.1678322,0.7876884,0.4947552,NULL,"brNDC"); // Size 0.03
        leg2->SetFillStyle(0);
        leg2->SetFillColor(0);
        leg2->SetBorderSize(0);
        leg2->SetTextSize(0.04);
        if(MC_) leg2->SetHeader("MC Weighted HLT_HIL1DoubleMuOpen ");
        if(!MC_) leg2->SetHeader("RD HLT_HIL1DoubleMuOpen ");
        sprintf(legs,"0 - 5 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1][1],Trg[1][2],Trg[1][3]);
        leg2->AddEntry(gTrgEta[1],legs,"pl");
        sprintf(legs,"5 - 10 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[2][1],Trg[2][2],Trg[2][3]);
        leg2->AddEntry(gTrgEta[2],legs,"pl");
        sprintf(legs,"10 - 20 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[3][1],Trg[3][2],Trg[3][3]);
        leg2->AddEntry(gTrgEta[3],legs,"pl");
        sprintf(legs,"20 - 30 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[4][1],Trg[4][2],Trg[4][3]);
        leg2->AddEntry(gTrgEta[4],legs,"pl");
        sprintf(legs,"30 - 40 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[5][1],Trg[5][2],Trg[5][3]);
        leg2->AddEntry(gTrgEta[5],legs,"pl");
        sprintf(legs,"40 - 50 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[6][1],Trg[6][2],Trg[6][3]);
        leg2->AddEntry(gTrgEta[6],legs,"pl");
        sprintf(legs,"50 - 60 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[7][1],Trg[7][2],Trg[7][3]);
        leg2->AddEntry(gTrgEta[7],legs,"pl");
        sprintf(legs,"60 - 100 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[8][1],Trg[8][2],Trg[8][3]);
        leg2->AddEntry(gTrgEta[8],legs,"pl");
        sprintf(legs,"MinBias : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[0][1],Trg[0][2],Trg[0][3]);
        leg2->AddEntry(gTrgEta[0],legs,"pl");
        leg2->Draw("same");

        if(MC_){
            if(TrgMode_ == 1){
/*
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL1_New3_eta_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL1_New3_eta_CNT.pdf");
*/
            }else{
/*
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL2_New3_eta_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL2_New3_eta_CNT.pdf");
*/
            }
        }else{
            if(TrgMode_ == 1){
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_1st_run_eta_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_1st_run_eta_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_2nd_run_eta_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_2nd_run_eta_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_1st_run_eta_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_1st_run_eta_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_2nd_run_eta_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_2nd_run_eta_CNT_test.pdf");
*/
            }else{
/*
                c2->SaveAs("Jpsi_pPb_for_B_RD_HLTL2_New2_eta_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_RD_HLTL2_New2_eta_CNT.pdf");
*/
            }
        }

        gTrgPt[1]->SetMarkerStyle(20);
        gTrgPt[1]->SetMarkerColor(634);
        gTrgPt[1]->GetXaxis()->SetTitle("p_{T} [GeV/c]");;
        gTrgPt[1]->GetXaxis()->CenterTitle();;
        gTrgPt[2]->SetMarkerStyle(23);
        gTrgPt[2]->SetMarkerColor(602);
        gTrgPt[3]->SetMarkerStyle(24);
        gTrgPt[3]->SetMarkerColor(618);
        gTrgPt[4]->SetMarkerStyle(25);
        gTrgPt[4]->SetMarkerColor(620);
        gTrgPt[5]->SetMarkerStyle(26);
        gTrgPt[5]->SetMarkerColor(621);
        gTrgPt[6]->SetMarkerStyle(27);
        gTrgPt[6]->SetMarkerColor(622);
        gTrgPt[7]->SetMarkerStyle(28);
        gTrgPt[7]->SetMarkerColor(623);
        gTrgPt[8]->SetMarkerStyle(29);
        gTrgPt[8]->SetMarkerColor(624);
        gTrgPt[0]->SetMarkerStyle(21);
        gTrgPt[0]->SetMarkerColor(802);

        formatTH1F(hPadPt,1,1,1);
        hPadPt->Draw();
        gTrgPt[1]->SetMinimum(0.5);
        gTrgPt[1]->SetMaximum(1.05);
        gTrgPt[1]->Draw("pz same");
        gTrgPt[2]->Draw("pz same");
        gTrgPt[3]->Draw("pz same");
        gTrgPt[4]->Draw("pz same");
        gTrgPt[5]->Draw("pz same");
        gTrgPt[6]->Draw("pz same");
        gTrgPt[7]->Draw("pz same");
        gTrgPt[8]->Draw("pz same");
        gTrgPt[0]->Draw("pz same");

        TLegend *leg3 = new TLegend(0.4899497,0.1678322,0.7876884,0.4947552,NULL,"brNDC"); // Size 0.03
        leg3->SetFillStyle(0);
        leg3->SetFillColor(0);
        leg3->SetBorderSize(0);
        leg3->SetTextSize(0.04);
        if(MC_) leg3->SetHeader("MC Weighted HLT_HIL1DoubleMuOpen ");
        if(!MC_) leg3->SetHeader("RD HLT_HIL1DoubleMuOpen ");
        sprintf(legs,"0 - 5 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1][1],Trg[1][2],Trg[1][3]);
        leg3->AddEntry(gTrgPt[1],legs,"pl");
        sprintf(legs,"5 - 10 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[2][1],Trg[2][2],Trg[2][3]);
        leg3->AddEntry(gTrgPt[2],legs,"pl");
        sprintf(legs,"10 - 20 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[3][1],Trg[3][2],Trg[3][3]);
        leg3->AddEntry(gTrgPt[3],legs,"pl");
        sprintf(legs,"20 - 30 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[4][1],Trg[4][2],Trg[4][3]);
        leg3->AddEntry(gTrgPt[4],legs,"pl");
        sprintf(legs,"30 - 40 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[5][1],Trg[5][2],Trg[5][3]);
        leg3->AddEntry(gTrgPt[5],legs,"pl");
        sprintf(legs,"40 - 50 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[6][1],Trg[6][2],Trg[6][3]);
        leg3->AddEntry(gTrgPt[6],legs,"pl");
        sprintf(legs,"50 - 60 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[7][1],Trg[7][2],Trg[7][3]);
        leg3->AddEntry(gTrgPt[7],legs,"pl");
        sprintf(legs,"60 - 100 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[8][1],Trg[8][2],Trg[8][3]);
        leg3->AddEntry(gTrgPt[8],legs,"pl");
        sprintf(legs,"MinBias : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[0][1],Trg[0][2],Trg[0][3]);
        leg3->AddEntry(gTrgPt[0],legs,"pl");
        leg3->Draw("same");

        if(MC_){
            if(TrgMode_ == 1){
/*
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL1_New2_pt_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL1_New2_pt_CNT.pdf");
*/
            }else{
/*
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL2_New2_pt_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_MC_HLTL2_New2_pt_CNT.pdf");
*/
            }
        }else{
            if(TrgMode_ == 1){
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_1st_run_pt_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_1st_run_pt_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_2nd_run_pt_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2Rfb_2nd_run_pt_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_1st_run_pt_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_1st_run_pt_CNT_test.pdf");
*/
/*
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_2nd_run_pt_CNT_test.png");
                c2->SaveAs("Jpsi_pPb_RD_HLTL1_New2CS_2nd_run_pt_CNT_test.pdf");
*/
            }else{
/*
                c2->SaveAs("Jpsi_pPb_for_B_RD_HLTL2_New2_pt_CNT.png");
                c2->SaveAs("Jpsi_pPb_for_B_RD_HLTL2_New2_pt_CNT.pdf");
*/
            }
        }

    }

    outfile->cd();

    if(Cent_){
        gTrgPt[1]->SetName("gTrg0005Pt");
        gTrgPt[2]->SetName("gTrg0510Pt");
        gTrgPt[3]->SetName("gTrg1020Pt");
        gTrgPt[4]->SetName("gTrg2030Pt");
        gTrgPt[5]->SetName("gTrg3040Pt");
        gTrgPt[6]->SetName("gTrg4050Pt");
        gTrgPt[7]->SetName("gTrg5060Pt");
        gTrgPt[8]->SetName("gTrg60100Pt");
        gTrgEta[1]->SetName("gTrg0005Eta");
        gTrgEta[2]->SetName("gTrg0510Eta");
        gTrgEta[3]->SetName("gTrg1020Eta");
        gTrgEta[4]->SetName("gTrg2030Eta");
        gTrgEta[5]->SetName("gTrg3040Eta");
        gTrgEta[6]->SetName("gTrg4050Eta");
        gTrgEta[7]->SetName("gTrg5060Eta");
        gTrgEta[8]->SetName("gTrg60100Eta");

        gTrgPt[1]->Write();
        gTrgPt[2]->Write();
        gTrgPt[3]->Write();
        gTrgPt[4]->Write();
        gTrgPt[5]->Write();
        gTrgPt[6]->Write();
        gTrgPt[7]->Write();
        gTrgPt[8]->Write();
        gTrgEta[1]->Write();
        gTrgEta[2]->Write();
        gTrgEta[3]->Write();
        gTrgEta[4]->Write();
        gTrgEta[5]->Write();
        gTrgEta[6]->Write();
        gTrgEta[7]->Write();
        gTrgEta[8]->Write();
    }
    gTrgPt[0]->SetName("gMBTrgPt");
    gTrgEta[0]->SetName("gMBTrgEta");
    gTrg[0]->SetName("gMBTrg");

    pCNT->SetName("pCNT");
    pMBCNT->SetName("pMBCNT");

    gTrgPt[0]->Write();
    gTrgEta[0]->Write();
    gTrg[0]->Write();

    pCNT->Write();
    pMBCNT->Write();

    outfile->Close();
    //f->Close();
    //files->Close();

}

void formatTH1F(TH1* a, int b, int c, int d){
    a->SetLineWidth(2);
    a->SetLineStyle(c);
    a->SetMarkerSize(2);
    a->SetLineColor(b);
    a->SetMarkerColor(b);
    a->GetYaxis()->SetTitle("Efficiency");
    if(d == 1){
        a->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    }else if(d == 2){
        a->GetXaxis()->SetTitle("#eta");
    }else if(d == 3){
        a->GetXaxis()->SetTitle("rapidity");
    }else if(d == 4){
        a->GetXaxis()->SetTitle("Centrality");
    }
    a->GetXaxis()->CenterTitle(true);
    a->GetXaxis()->SetLabelSize(0.05);
    a->GetXaxis()->SetTitleSize(0.05);
    a->GetXaxis()->SetTitleOffset(0.9);
    a->GetYaxis()->SetLabelSize(0.05);
    a->GetYaxis()->SetTitleSize(0.05);
    a->GetYaxis()->SetTitleOffset(0.8);

}

void formatTLeg(TLegend* a){

    a->SetFillStyle(0);
    a->SetFillColor(0);
    a->SetBorderSize(0);
    a->SetTextSize(0.03);

}


void formatTGraph(TGraph* a, int b, int c, int d)
{

    a->SetMarkerStyle(c);
    a->SetMarkerColor(b);
    a->SetMarkerSize(1.0);
    a->SetLineColor(b);
    a->SetLineWidth(1);
    a->GetXaxis()->SetLabelSize(0.05);
    a->GetXaxis()->SetTitleSize(0.06);
    a->GetXaxis()->SetTitleOffset(0.9);
    a->GetYaxis()->SetTitle("Efficiency");
    a->GetXaxis()->CenterTitle();
    if(d == 1){ 
        a->GetXaxis()->SetTitle("p_{T} (GeV/c)"); 
    }else if(d == 2){ 
        a->GetXaxis()->SetTitle("eta"); 
    }else if(d == 3){ 
        a->GetXaxis()->SetTitle("rapidity"); 
    }else if(d == 4){
        a->GetXaxis()->SetTitle("Centrality");
    }   
    a->GetYaxis()->SetRangeUser(0,1);
    a->GetXaxis()->SetRangeUser(0,10);
    a->GetYaxis()->SetLabelSize(0.05);
    a->GetYaxis()->SetTitleSize(0.05);
    a->GetYaxis()->SetTitleOffset(0.9);


}

TGraphAsymmErrors *plotEffEta(RooDataSet *a, int aa){
    const RooArgSet *set = a->get();
    RooRealVar *xAx = (RooRealVar*)set->find("eta");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

    const int nbins = xAx->getBinning().numBins();

    double tx[nbins], txhi[nbins], txlo[nbins];
    double ty[nbins], tyhi[nbins], tylo[nbins];

    for (int i=0; i<nbins; i++) {
        a->get(i);
        ty[i] = eff->getVal();
        tx[i] = xAx->getVal();
        txhi[i] = fabs(xAx->getErrorHi());
        txlo[i] = fabs(xAx->getErrorLo()); 
        tyhi[i] = fabs(eff->getErrorHi());
        tylo[i] = fabs(eff->getErrorLo()); 
    }

    cout<<"NBins : "<<nbins<<endl;

    const double *x = tx;
    const double *xhi = txhi;
    const double *xlo = txlo;
    const double *y = ty;
    const double *yhi = tyhi;
    const double *ylo = tylo;

    TGraphAsymmErrors *b = new TGraphAsymmErrors();
    if(aa == 1) {*b = TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
    if(aa == 0) {*b = TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}
    b->SetMaximum(1.1);
    b->SetMinimum(0.0);
    b->SetMarkerStyle(20);
    b->SetMarkerColor(kRed+2);
    b->SetMarkerSize(1.0);
    b->SetTitle("");
    b->GetXaxis()->SetTitleSize(0.05);
    b->GetYaxis()->SetTitleSize(0.05);
    b->GetXaxis()->SetTitle("#eta");
    b->GetYaxis()->SetTitle("Efficiency");
    b->GetXaxis()->CenterTitle();
    //b->Draw("apz");

    for (int i=0; i<nbins; i++) {
        cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
    }

    return b;

}

TGraphAsymmErrors *plotEffPt(RooDataSet *a, int aa){
    const RooArgSet *set = a->get();
    RooRealVar *xAx = (RooRealVar*)set->find("pt");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

    const int nbins = xAx->getBinning().numBins();

    double tx[nbins], txhi[nbins], txlo[nbins];
    double ty[nbins], tyhi[nbins], tylo[nbins];

    for (int i=0; i<nbins; i++) {
        a->get(i);
        ty[i] = eff->getVal();
        tx[i] = xAx->getVal();
        txhi[i] = fabs(xAx->getErrorHi());
        txlo[i] = fabs(xAx->getErrorLo()); 
        tyhi[i] = fabs(eff->getErrorHi());
        tylo[i] = fabs(eff->getErrorLo()); 
    }

    cout<<"NBins : "<<nbins<<endl;

    const double *x = tx;
    const double *xhi = txhi;
    const double *xlo = txlo;
    const double *y = ty;
    const double *yhi = tyhi;
    const double *ylo = tylo;

    TGraphAsymmErrors *b = new TGraphAsymmErrors();
    if(aa == 1) {*b = TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
    if(aa == 0) {*b = TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}
    b->SetMaximum(1.1);
    b->SetMinimum(0.0);
    b->SetMarkerStyle(20);
    b->SetMarkerColor(kRed+2);
    b->SetMarkerSize(1.0);
    b->SetTitle("");
    b->GetXaxis()->SetTitleSize(0.05);
    b->GetYaxis()->SetTitleSize(0.05);
    b->GetXaxis()->SetTitle("p_{T} [GeV/c]");
    b->GetYaxis()->SetTitle("Efficiency");
    b->GetXaxis()->CenterTitle();
    //b->Draw("apz");

    for (int i=0; i<nbins; i++) {
        cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
    }

    return b;

}

TH2F *plotEff2D(RooDataSet *a, TString b){
    const RooArgSet *set = a->get();
    RooRealVar *yAx = (RooRealVar*)set->find("pt");
    RooRealVar *xAx = (RooRealVar*)set->find("eta");
    RooRealVar *eff = (RooRealVar*)set->find("efficiency");

    //const int xnbins = xAx->getBinning().numBins();
    //const int ynbins = yAx->getBinning().numBins();

    const double *xvbins = xAx->getBinning().array();
    const double *yvbins = yAx->getBinning().array();

    TH2F* h = new TH2F(b, "", xAx->getBinning().numBins(), xvbins, yAx->getBinning().numBins(), yvbins);

    gStyle->SetPaintTextFormat("5.2f");
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetPalette(1);
    h->SetOption("colztexte");
    h->GetZaxis()->SetRangeUser(-0.001,1.001);
    h->SetStats(kFALSE);
    h->GetYaxis()->SetTitle("p_{T} [GeV/c]");
    h->GetXaxis()->SetTitle("#eta");
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetTitleOffset(0.8);
    h->GetXaxis()->SetTitleOffset(0.9);
    for(int i=0; i<a->numEntries(); i++){
        a->get(i);
        h->SetBinContent(h->FindBin(xAx->getVal(), yAx->getVal()), eff->getVal());
        h->SetBinError(h->FindBin(xAx->getVal(), yAx->getVal()), (eff->getErrorHi()-eff->getErrorLo())/2.);
    }

    return h;

}

