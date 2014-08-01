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
void TnPEffMuIdDraw_for_B ();

// From here you need to set up your environments.
bool MC_ = false; // if the data set is real data, it should be false
bool Cent_ = false; // if the data is only one MinBias file, it shoud be false
const int maxFile_ = 1; // the number of data files, usually it should be 4 but 1 in the case of MinBias

char *outfile_ = "Jpsi_pPb_RD_MuIdNew2CS_CBpPoly_1st_Run_Eff_for_B_test.root";
//char *outfile_ = "Jpsi_pPb_MC_MuIdNew2CS_CBpPoly_1st_Run_Eff_for_B_test.root";
string dir_suffix = "_CBPlusPoly"; // depends on which fit function is used for : eg) _gaussExpo, _gaussPol2

void TnPEffMuIdDraw_for_B() {
    gROOT->Macro("rootlogon.C");

    char *files[maxFile_] = {
        "tnp_pPb_Ana_MuIdNew2CS_CBpPoly_1st_Run_All_for_B_test.root",
//        "tnp_pPb_Ana_MuIdNew2CS_CBpPoly_1st_MC_for_B_test.root",
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
        if(!MC_) leg_title = "Data MuId Efficiency (" + middle_name2 + ")";
        if(MC_) leg_title = "MC MuId Efficiency (" + middle_name2 + ")";

        TCanvas *c1 = new TCanvas("c1","",120,20,800,600);

        f->cd();

        RooDataSet *daMuIdEta = (RooDataSet*)f->Get("MuonIDNew2CS/PassingTrk_eta/fit_eff");
        RooDataSet *daMuIdEta1bin = (RooDataSet*)f->Get("MuonIDNew2CS/PassingTrk_1bin_eta_pt/fit_eff");

        TGraphAsymmErrors *MuId_eta = plotEffEta(daMuIdEta, 1);
        TGraphAsymmErrors *MuId_1bin_pt_eta = plotEffEta(daMuIdEta1bin, 1);

        double Id[4];
        MuId_1bin_pt_eta->GetPoint(0,Id[0],Id[1]);
        Id[2] = MuId_1bin_pt_eta->GetErrorYhigh(0);
        Id[3] = MuId_1bin_pt_eta->GetErrorYlow(0);

        RooDataSet *daMuIdPt = (RooDataSet*)f->Get("MuonIDNew2CS/PassingTrk_pt/fit_eff");
        TGraphAsymmErrors *MuId_pt = plotEffPt(daMuIdPt, 1);

        /*
        RooDataSet *daMuId2D = (RooDataSet*)f->Get("MuonID/PassingTrk_pt_eta/fit_eff");
        TString twoDName1 = "MuId_2d";
        TH2F *MuId_2d = plotEff2D(daMuId2D, twoDName1);
        */

        MuId_eta->GetXaxis()->SetLabelSize(0.05);
        MuId_eta->GetYaxis()->SetLabelSize(0.05);
        MuId_eta->GetXaxis()->SetTitleSize(0.05);
        MuId_eta->GetYaxis()->SetTitleSize(0.05);
        MuId_eta->GetXaxis()->SetTitleOffset(0.9);
        MuId_eta->GetYaxis()->SetTitleOffset(0.8);
        MuId_eta->SetMarkerColor(kRed+2);
        MuId_eta->Draw("apz");

        char legs[512];
        TLegend *leg1 = new TLegend(0.3054774,0.1567832,0.5429648,0.3858042,NULL,"brNDC");
        leg1->SetFillStyle(0);
        leg1->SetFillColor(0);
        leg1->SetBorderSize(0);
        leg1->SetTextSize(0.04);
        leg1->SetHeader(leg_title);
        sprintf(legs,"MuId Efficiency: %0.3f^{ +%0.4f}_{ -%0.4f}",Id[1],Id[2],Id[3]);
        leg1->AddEntry(MuId_eta,legs,"pl");
        leg1->Draw("same");
        TString pic_name_png, pic_name_pdf;

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_MuIdNew2CS_" + middle_name + "_eta_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_MuIdNew2CS_" + middle_name + "_eta_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_MuId_New2CS_1st_run_" + middle_name + "_eta_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_MuId_New2CS_1st_run_" + middle_name + "_eta_for_B_test.pdf";
        }
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);

        MuId_pt->GetXaxis()->SetLabelSize(0.05);
        MuId_pt->GetYaxis()->SetLabelSize(0.05);
        MuId_pt->GetXaxis()->SetTitleSize(0.05);
        MuId_pt->GetYaxis()->SetTitleSize(0.05);
        MuId_pt->GetXaxis()->SetTitleOffset(0.9);
        MuId_pt->GetYaxis()->SetTitleOffset(0.8);
        MuId_pt->SetMarkerColor(kRed+2);
        MuId_pt->Draw("apz");

        TLegend *leg2 = new TLegend(0.3454774,0.2167832,0.5429648,0.4458042,NULL,"brNDC");
        leg2->SetFillStyle(0);
        leg2->SetFillColor(0);
        leg2->SetBorderSize(0);
        leg2->SetTextSize(0.04);
        leg2->SetHeader(leg_title);
        sprintf(legs,"MuId Efficiency: %0.3f^{ +%0.4f}_{ -%0.4f}",Id[1],Id[2],Id[3]);
        leg2->AddEntry(MuId_pt,legs,"pl");
        leg2->Draw("same");

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_MuIdNew2CS_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_MuIdNew2CS_" + middle_name + "_pt_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_MuIdNew2CS_1st_run_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_MuIdNew2CS_1st_run_" + middle_name + "_pt_for_B_test.pdf";
        }
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);

        /*
        MuId_2d->Draw();
        pic_name_png = "Psi2s_MC_MuId_" + middle_name + "_2d.png";
        pic_name_pdf = "Psi2s_MC_MuId_" + middle_name + "_2d.pdf";
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);
        c1->Print("Psi2s_MC_MuId_Eff_total.ps");
        */

        TString Idna1, Idna2, Idna3, Idna4;

        Idna1 = "MuId_eta_" + middle_name; 
        Idna2 = "MuId_1bin_pt_eta_" + middle_name; 
        Idna3 = "MuId_pt_" + middle_name; 
        Idna4 = "MuId_2d_" + middle_name; 

        MuId_eta->SetName(Idna1);
        MuId_1bin_pt_eta->SetName(Idna2);
        MuId_pt->SetName(Idna3);
        //MuId_2d->SetName(Idna4);

        outfile->cd();
        MuId_eta->Write();
        MuId_pt->Write();
        MuId_1bin_pt_eta->Write();
        //MuId_2d->Write();
    }

    // Comparing Plots as the Centralities

    TH1F *hPadEta = new TH1F("hPadEta","",6,-2.4,2.4);
    TH1F *hPadPt = new TH1F("hPadPt","",6,0,20);

    TCanvas *c2 = new TCanvas("c2","",120,20,800,600);
    //c2->cd();

    TGraphAsymmErrors *gMuId[maxFile_];
    TGraphAsymmErrors *gMuIdEta[maxFile_];
    TGraphAsymmErrors *gMuIdPt[maxFile_];

    double MuId[9][4];
    for(int i = 0; i < maxFile_; i++){

        TFile finput(files[i]);
        finput.cd();

        RooDataSet *dataMuId = (RooDataSet*)finput.Get("MuonIDNew2CS/PassingTrk_1bin_eta_pt/fit_eff");
        RooDataSet *dataMuIdEta = (RooDataSet*)finput.Get("MuonIDNew2CS/PassingTrk_eta/fit_eff");
        RooDataSet *dataMuIdPt = (RooDataSet*)finput.Get("MuonIDNew2CS/PassingTrk_pt/fit_eff");


        gMuId[i] = plotEffEta(dataMuId, 0);
        gMuIdEta[i] = plotEffEta(dataMuIdEta, 0);
        gMuIdPt[i] = plotEffPt(dataMuIdPt, 0);

        gMuId[i]->GetPoint(0,MuId[i][0],MuId[i][1]);
        MuId[i][2] = gMuId[i]->GetErrorYhigh(0);
        MuId[i][3] = gMuId[i]->GetErrorYlow(0);
    }

    TGraphAsymmErrors *pCNT = new TGraphAsymmErrors(3);
    TGraphAsymmErrors *pMBCNT = new TGraphAsymmErrors();

    if(Cent_){
        pCNT->SetPoint(0,10,MuId[1][1]);
        pCNT->SetPointError(0,0,0,MuId[1][3],MuId[1][2]);

        pCNT->SetPoint(1,20,MuId[2][1]);
        pCNT->SetPointError(1,0,0,MuId[2][3],MuId[2][2]);

        pCNT->SetPoint(2,30,MuId[3][1]);
        pCNT->SetPointError(2,0,0,MuId[3][3],MuId[3][2]);

        pCNT->SetPoint(3,40,MuId[4][1]);
        pCNT->SetPointError(3,0,0,MuId[4][3],MuId[4][2]);

        pCNT->SetPoint(4,50,MuId[5][1]);
        pCNT->SetPointError(4,0,0,MuId[5][3],MuId[5][2]);

        pCNT->SetPoint(5,60,MuId[6][1]);
        pCNT->SetPointError(5,0,0,MuId[6][3],MuId[6][2]);

        pCNT->SetPoint(6,70,MuId[7][1]);
        pCNT->SetPointError(6,0,0,MuId[7][3],MuId[7][2]);

        pCNT->SetPoint(7,80,MuId[8][1]);
        pCNT->SetPointError(7,0,0,MuId[8][3],MuId[8][2]);

        pCNT->SetMarkerColor(kRed+2);
        pCNT->SetMarkerStyle(20);
    }
    pMBCNT->SetPoint(0,113.0558,MuId[0][1]);
    pMBCNT->SetPointError(0,0,0,MuId[0][3],MuId[0][2]);

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
    if(Cent_) leg1->AddEntry(pCNT,"MuId Efficiency","pl");
    sprintf(legs,"MuId Efficiency: %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[0][1],MuId[0][2],MuId[0][3]);
    leg1->AddEntry(pMBCNT,legs,"pl");
    leg1->Draw("same");

    if(MC_){
/*
        c2->SaveAs("Jpsi_pPb_MC_MuId_New_CNT.png");
        c2->SaveAs("Jpsi_pPb_MC_MuId_New_CNT.pdf");
*/
    }else{
/*
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_CNT_test.pdf");
*/
/*
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_CNT_test.pdf");
*/
/*
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_CNT_test.pdf");
*/
/*
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_CNT_test.pdf");
*/
    }

    if(Cent_) {
        gMuIdEta[1]->SetMarkerStyle(20);
        gMuIdEta[1]->SetMarkerColor(634);
        gMuIdEta[1]->GetXaxis()->SetTitle("#eta");;
        gMuIdEta[1]->GetXaxis()->CenterTitle();;
        gMuIdEta[2]->SetMarkerStyle(23);
        gMuIdEta[2]->SetMarkerColor(602);
        gMuIdEta[3]->SetMarkerStyle(24);
        gMuIdEta[3]->SetMarkerColor(618);
        gMuIdEta[4]->SetMarkerStyle(25);
        gMuIdEta[4]->SetMarkerColor(620);
        gMuIdEta[5]->SetMarkerStyle(26);
        gMuIdEta[5]->SetMarkerColor(621);
        gMuIdEta[6]->SetMarkerStyle(27);
        gMuIdEta[6]->SetMarkerColor(622);
        gMuIdEta[7]->SetMarkerStyle(28);
        gMuIdEta[7]->SetMarkerColor(623);
        gMuIdEta[8]->SetMarkerStyle(29);
        gMuIdEta[8]->SetMarkerColor(624);
        gMuIdEta[0]->SetMarkerStyle(21);
        gMuIdEta[0]->SetMarkerColor(802);

        formatTH1F(hPadEta,1,1,2);
        hPadEta->Draw();
        gMuIdEta[1]->SetMinimum(0.5);
        gMuIdEta[1]->SetMaximum(1.05);
        gMuIdEta[1]->Draw("pz same");
        gMuIdEta[2]->Draw("pz same");
        gMuIdEta[3]->Draw("pz same");
        gMuIdEta[4]->Draw("pz same");
        gMuIdEta[5]->Draw("pz same");
        gMuIdEta[6]->Draw("pz same");
        gMuIdEta[7]->Draw("pz same");
        gMuIdEta[8]->Draw("pz same");
        gMuIdEta[0]->Draw("pz same");

        TLegend *leg2 = new TLegend(0.4899497,0.1678322,0.7876884,0.4947552,NULL,"brNDC"); // Size 0.03
        leg2->SetFillStyle(0);
        leg2->SetFillColor(0);
        leg2->SetBorderSize(0);
        leg2->SetTextSize(0.04);
        leg2->SetHeader(leg_title);
        sprintf(legs,"0 - 5 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[1][1],MuId[1][2],MuId[1][3]);
        leg2->AddEntry(gMuIdEta[1],legs,"pl");
        sprintf(legs,"5 - 10 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[2][1],MuId[2][2],MuId[2][3]);
        leg2->AddEntry(gMuIdEta[2],legs,"pl");
        sprintf(legs,"10 - 20 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[3][1],MuId[3][2],MuId[3][3]);
        leg2->AddEntry(gMuIdEta[3],legs,"pl");
        sprintf(legs,"20 - 30 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[4][1],MuId[4][2],MuId[4][3]);
        leg2->AddEntry(gMuIdEta[4],legs,"pl");
        sprintf(legs,"30 - 40 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[5][1],MuId[5][2],MuId[5][3]);
        leg2->AddEntry(gMuIdEta[5],legs,"pl");
        sprintf(legs,"40 - 50 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[6][1],MuId[6][2],MuId[6][3]);
        leg2->AddEntry(gMuIdEta[6],legs,"pl");
        sprintf(legs,"50 - 60 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[7][1],MuId[7][2],MuId[7][3]);
        leg2->AddEntry(gMuIdEta[7],legs,"pl");
        sprintf(legs,"60 - 100 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[8][1],MuId[8][2],MuId[8][3]);
        leg2->AddEntry(gMuIdEta[8],legs,"pl");
        sprintf(legs,"MinBias : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[0][1],MuId[0][2],MuId[0][3]);
        leg2->AddEntry(gMuIdEta[0],legs,"pl");
        leg2->Draw("same");

        if(MC_){
/*
            c2->SaveAs("Jpsi_pPb_MC_MuId_New_eta_CNT.png");
            c2->SaveAs("Jpsi_pPb_MC_MuId_New_eta_CNT.pdf");
*/
        }else{
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_eta_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_eta_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_eta_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_eta_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_eta_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_eta_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_eta_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_eta_CNT_test.pdf");
*/
        }

        gMuIdPt[1]->SetMarkerStyle(20);
        gMuIdPt[1]->SetMarkerColor(634);
        gMuIdPt[1]->GetXaxis()->SetTitle("p_{T} [GeV/c]");;
        gMuIdPt[1]->GetXaxis()->CenterTitle();;
        gMuIdPt[2]->SetMarkerStyle(23);
        gMuIdPt[2]->SetMarkerColor(602);
        gMuIdPt[3]->SetMarkerStyle(24);
        gMuIdPt[3]->SetMarkerColor(618);
        gMuIdPt[4]->SetMarkerStyle(25);
        gMuIdPt[4]->SetMarkerColor(620);
        gMuIdPt[5]->SetMarkerStyle(26);
        gMuIdPt[5]->SetMarkerColor(621);
        gMuIdPt[6]->SetMarkerStyle(27);
        gMuIdPt[6]->SetMarkerColor(622);
        gMuIdPt[7]->SetMarkerStyle(28);
        gMuIdPt[7]->SetMarkerColor(623);
        gMuIdPt[8]->SetMarkerStyle(29);
        gMuIdPt[8]->SetMarkerColor(624);
        gMuIdPt[0]->SetMarkerStyle(21);
        gMuIdPt[0]->SetMarkerColor(802);

        formatTH1F(hPadPt,1,1,1);
        hPadPt->Draw();
        gMuIdPt[1]->SetMinimum(0.5);
        gMuIdPt[1]->SetMaximum(1.05);
        gMuIdPt[1]->Draw("pz same");
        gMuIdPt[2]->Draw("pz same");
        gMuIdPt[3]->Draw("pz same");
        gMuIdPt[4]->Draw("pz same");
        gMuIdPt[5]->Draw("pz same");
        gMuIdPt[6]->Draw("pz same");
        gMuIdPt[7]->Draw("pz same");
        gMuIdPt[8]->Draw("pz same");
        gMuIdPt[0]->Draw("pz same");

        TLegend *leg3 = new TLegend(0.4899497,0.1678322,0.7876884,0.4947552,NULL,"brNDC"); // Size 0.03
        leg3->SetFillStyle(0);
        leg3->SetFillColor(0);
        leg3->SetBorderSize(0);
        leg3->SetTextSize(0.04);
        leg3->SetHeader(leg_title);
        sprintf(legs,"0 - 5 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[1][1],MuId[1][2],MuId[1][3]);
        leg3->AddEntry(gMuIdPt[1],legs,"pl");
        sprintf(legs,"5 - 10 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[2][1],MuId[2][2],MuId[2][3]);
        leg3->AddEntry(gMuIdPt[2],legs,"pl");
        sprintf(legs,"10 - 20 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[3][1],MuId[3][2],MuId[3][3]);
        leg3->AddEntry(gMuIdPt[3],legs,"pl");
        sprintf(legs,"20 - 30 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[4][1],MuId[4][2],MuId[4][3]);
        leg3->AddEntry(gMuIdPt[4],legs,"pl");
        sprintf(legs,"30 - 40 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[5][1],MuId[5][2],MuId[5][3]);
        leg3->AddEntry(gMuIdPt[5],legs,"pl");
        sprintf(legs,"40 - 50 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[6][1],MuId[6][2],MuId[6][3]);
        leg3->AddEntry(gMuIdPt[6],legs,"pl");
        sprintf(legs,"50 - 60 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[7][1],MuId[7][2],MuId[7][3]);
        leg3->AddEntry(gMuIdPt[7],legs,"pl");
        sprintf(legs,"60 - 100 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[8][1],MuId[8][2],MuId[8][3]);
        leg3->AddEntry(gMuIdPt[8],legs,"pl");
        sprintf(legs,"MinBias : %0.3f^{ +%0.4f}_{ -%0.4f}",MuId[0][1],MuId[0][2],MuId[0][3]);
        leg3->AddEntry(gMuIdPt[0],legs,"pl");
        leg3->Draw("same");

        if(MC_){
/*
            c2->SaveAs("Jpsi_pPb__MC_MuId_New_pt_CNT.png");
            c2->SaveAs("Jpsi_pPb__MC_MuId_New_pt_CNT.pdf");
*/
        }else{
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_1st_run_pt_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2Rfb_2nd_run_pt_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_1st_run_pt_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_MuIdNew2CS_2nd_run_pt_CNT_test.pdf");
*/
        }

    }

    outfile->cd();

    if(Cent_){
        gMuIdPt[1]->SetName("gCNT0005Pt");
        gMuIdPt[2]->SetName("gCNT0510Pt");
        gMuIdPt[3]->SetName("gCNT1020Pt");
        gMuIdPt[4]->SetName("gCNT2030Pt");
        gMuIdPt[5]->SetName("gCNT3040Pt");
        gMuIdPt[6]->SetName("gCNT4050Pt");
        gMuIdPt[7]->SetName("gCNT5060Pt");
        gMuIdPt[8]->SetName("gCNT60100Pt");
        gMuIdEta[1]->SetName("gCNT0005Eta");
        gMuIdEta[2]->SetName("gCNT0510Eta");
        gMuIdEta[3]->SetName("gCNT1020Eta");
        gMuIdEta[4]->SetName("gCNT2030Eta");
        gMuIdEta[5]->SetName("gCNT3040Eta");
        gMuIdEta[6]->SetName("gCNT4050Eta");
        gMuIdEta[7]->SetName("gCNT5060Eta");
        gMuIdEta[8]->SetName("gCNT60100Eta");

        gMuIdPt[1]->Write();
        gMuIdPt[2]->Write();
        gMuIdPt[3]->Write();
        gMuIdPt[4]->Write();
        gMuIdPt[5]->Write();
        gMuIdPt[6]->Write();
        gMuIdPt[7]->Write();
        gMuIdPt[8]->Write();
        gMuIdEta[1]->Write();
        gMuIdEta[2]->Write();
        gMuIdEta[3]->Write();
        gMuIdEta[4]->Write();
        gMuIdEta[5]->Write();
        gMuIdEta[6]->Write();
        gMuIdEta[7]->Write();
        gMuIdEta[8]->Write();
    }
    gMuIdPt[0]->SetName("gMBCNTPt");
    gMuIdEta[0]->SetName("gMBCNTEta");
    gMuId[0]->SetName("gMBCNT");

    pCNT->SetName("pCNT");
    pMBCNT->SetName("pMBCNT");

    gMuIdPt[0]->Write();
    gMuIdEta[0]->Write();
    gMuId[0]->Write();

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

