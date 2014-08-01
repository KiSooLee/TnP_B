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
void TnPEffTrgDraw_etabin3_1st_for_B();

// From here you need to set up your environments.
bool MC_ = false; // if the data set is real data, it should be false
bool Cent_ = false; // if the data is only one MinBias file, it shoud be false
const int maxFile_ = 1; // the number of data files, usually it should be 4 but 1 in the case of MinBias
int TrgMode_ = 1; // 1 = L1, 2 = L2

char *outfile_ = "Jpsi_pPb_RD_MuTrgNew2etabin3CS_1st_CBGpExp_Eff_for_B_test.root"; // L1 or L2
//char *outfile_ = "Jpsi_pPb_MC_MuTrgNew2etabin3CS_1st_CBGpExp_Eff_for_B_test.root"; // L1 or L2
string dir_suffix = "_cbGaussPlusExpo"; // depends on which fit function is used for : eg) _gaussExpo, _gaussPol2

void TnPEffTrgDraw_etabin3_1st_for_B() {
    gROOT->Macro("rootlogon.C");

    char *files[maxFile_] = {
        "tnp_pPb_Ana_MuTrgNew2etabin3CS_CBGpExp_1st_Run_All_for_B_test.root",
//        "tnp_pPb_Ana_MuTrgNew2etabin3CS_CBGpExp_MC_All_for_B_test.root",
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
        RooDataSet *daTrgEta1bin;
        daTrgEta1bin = (RooDataSet*)f->Get("MuonTrgNew2etabin3CS/PAMu3_1bin_pt_eta/fit_eff");

        TGraphAsymmErrors *Trg_1bin_pt_eta = plotEffEta(daTrgEta1bin, 1);

        double Trg[4];
        Trg_1bin_pt_eta->GetPoint(0,Trg[0],Trg[1]);
        Trg[2] = Trg_1bin_pt_eta->GetErrorYhigh(0);
        Trg[3] = Trg_1bin_pt_eta->GetErrorYlow(0);

        RooDataSet *daTrgPt;
        if(TrgMode_ == 1) daTrgPt = (RooDataSet*)f->Get("MuonTrgNew2etabin3CS/PAMu3_pt/fit_eff");
        if(TrgMode_ == 2) daTrgPt = (RooDataSet*)f->Get("MuonTrgNew2/HLTL2_pt/fit_eff");
        TGraphAsymmErrors *Trg_pt = plotEffPt(daTrgPt, 1);

        /*
        RooDataSet *daTrgD = (RooDataSet*)f->Get("MuonTrg/HLTL1_pt_eta/fit_eff");
        TString twoDName1 = "Trg_2d";
        TH2F *Trg_2d = plotEff2D(daTrgD, twoDName1);
        */


        char legs[512];
        TString pic_name_png, pic_name_pdf;


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
        sprintf(legs,"HLT_HIL1DoubleMuOpen : %0.3f^{ +%0.4f}_{ -%0.4f}",Trg[1],Trg[2],Trg[3]);
        leg2->AddEntry(Trg_pt,legs,"pl");
        leg2->Draw("same");

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_Trg_New2_etabin3CS_Mu3_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_Trg_New2_etabin3CS_Mu3_" + middle_name + "_pt_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_Trg_New2_etabin3CS_1st_Mu3_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_Trg_New2_etabin3CS_1st_Mu3_" + middle_name + "_pt_for_B_test.pdf";
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

//        Trg_eta->SetName(Trgna1);
        Trg_1bin_pt_eta->SetName(Trgna2);
        Trg_pt->SetName(Trgna3);
        //Trg_2d->SetName(Trgna4);

        outfile->cd();
//        Trg_eta->Write();
        Trg_pt->Write();
        Trg_1bin_pt_eta->Write();
        //Trg_2d->Write();
    }

    // Comparing Plots as the Centralities

//    TH1F *hPadEta = new TH1F("hPadEta","",6,-2.4,2.4);
//    TH1F *hPadPt = new TH1F("hPadPt","",12,0,30);

    //c2->cd();


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

