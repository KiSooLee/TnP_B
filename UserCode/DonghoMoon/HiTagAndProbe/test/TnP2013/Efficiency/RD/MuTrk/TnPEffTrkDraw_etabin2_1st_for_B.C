#include <iostream>
#include <TSystem.h>
#include <TTree.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TPave.h>
#include <TText.h>
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
void TnPEffTrkDraw_etabin2_1st_for_B ();

// From here you need to set up your environments.
bool MC_ = false; // if the data set is real data, it should be false
bool Cent_ = false; // if the data is only one MinBias file, it shoud be false
const int maxFile_ = 1; // the number of data files, usually it should be 4 but 1 in the case of MinBias

char *outfile_ = "Jpsi_pPb_RD_MuTrketabin2_CBpPoly_1st_Run_Eff_for_B_test.root";
string dir_suffix = "_gaussPlusPoly"; // depends on which fit function is used for : eg) _gaussExpo, _gaussPol2

void TnPEffTrkDraw_etabin2_1st_for_B () {
    gROOT->Macro("rootlogon.C");

    // input your tnp_Ana_*.root files instead of "_input_file_"
    char *files[maxFile_] = {
        "../../Ana/RD/MuTrk/tnp_pPb_Ana_MuTrketabin2_2GpP4_1st_Run_for_B_test.root",
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
            middle_name = "5050";
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
        if(!MC_) leg_title = "Data Inner Tracking Efficiency (" + middle_name2 + ")";
        if(MC_) leg_title = "MC Inner Tracking Efficiency (" + middle_name2 + ")";

        TCanvas *c1 = new TCanvas("c1","",120,20,800,600);
        f->cd();

        RooDataSet *daTrkEta1bin = (RooDataSet*)f->Get("MuonTrketabin2/isTrk_1bin_pt_eta/fit_eff");

        TGraphAsymmErrors *Trk_1bin_pt_eta = plotEffEta(daTrkEta1bin, 1);

        double Trk[4];
        Trk_1bin_pt_eta->GetPoint(0,Trk[0],Trk[1]);
        Trk[2] = Trk_1bin_pt_eta->GetErrorYhigh(0);
        Trk[3] = Trk_1bin_pt_eta->GetErrorYlow(0);

        RooDataSet *daTrkPt = (RooDataSet*)f->Get("MuonTrketabin2/isTrk_pt/fit_eff");
        TGraphAsymmErrors *Trk_pt = plotEffPt(daTrkPt, 1);

        /*
        RooDataSet *daTrk2D = (RooDataSet*)f->Get("MuonTrk/is_Trk_pt_eta/fit_eff");
        TString twoDName1 = "Trk_2d";
        TH2F *Trk_2d = plotEff2D(daTrk2D, twoDName1);
        */

        char legs[512];
        TString pic_name_png, pic_name_pdf;


        Trk_pt->GetXaxis()->SetLabelSize(0.05);
        Trk_pt->GetYaxis()->SetLabelSize(0.05);
        Trk_pt->GetXaxis()->SetTitleSize(0.05);
        Trk_pt->GetYaxis()->SetTitleSize(0.05);
        Trk_pt->GetXaxis()->SetTitleOffset(0.9);
        Trk_pt->GetYaxis()->SetTitleOffset(0.8);
        Trk_pt->SetMarkerColor(kRed+2);
        Trk_pt->Draw("apz");

        TLegend *leg2 = new TLegend(0.3454774,0.2167832,0.5429648,0.4458042,NULL,"brNDC");
        leg2->SetFillStyle(0);
        leg2->SetFillColor(0);
        leg2->SetBorderSize(0);
        leg2->SetTextSize(0.04);
        leg2->SetHeader(leg_title);
        sprintf(legs,"Inner Tracking Efficiency: %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[1],Trk[2],Trk[3]);
        leg2->AddEntry(Trk_pt,legs,"pl");
        leg2->Draw("same");

        if(MC_){
            pic_name_png = "Jpsi_pPb_MC_Trketabin2_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_MC_Trketabin2_" + middle_name + "_pt_for_B_test.pdf";
        }else{
            pic_name_png = "Jpsi_pPb_RD_Trketabin2_1st_run_" + middle_name + "_pt_for_B_test.png";
            pic_name_pdf = "Jpsi_pPb_RD_Trketabin2_1st_run_" + middle_name + "_pt_for_B_test.pdf";
        }
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);

        /*
        Trk_2d->Draw();
        pic_name_png = "Jpsi_pPb_MC_Trk_" + middle_name + "_2d.png";
        pic_name_pdf = "Jpsi_pPb_MC_Trk_" + middle_name + "_2d.pdf";
        c1->SaveAs(pic_name_png);
        c1->SaveAs(pic_name_pdf);
        c1->Print("Jpsi_pPb_MC_Trk_Eff_total.ps");
        */

        TString Trkna1, Trkna2, Trkna3, Trkna4;

        Trkna1 = "Trk_eta_" + middle_name; 
        Trkna2 = "Trk_1bin_pt_eta_" + middle_name; 
        Trkna3 = "Trk_pt_" + middle_name; 
        Trkna4 = "Trk_2d_" + middle_name; 

        Trk_1bin_pt_eta->SetName(Trkna2);
        Trk_pt->SetName(Trkna3);
        //Trk_2d->SetName(Trkna4);

        outfile->cd();
        Trk_pt->Write();
        Trk_1bin_pt_eta->Write();
        //Trk_2d->Write();
    }

    // Comparing Plots as the Centralities

    TH1F *hPadEta = new TH1F("hPadEta","",15,-2.4,2.4);
    TH1F *hPadPt = new TH1F("hPadPt","",12,0,30);

    TCanvas *c2 = new TCanvas("c2","",120,20,800,600);
    //c2->cd();

    TGraphAsymmErrors *gTrk[maxFile_];
    TGraphAsymmErrors *gTrkPt[maxFile_];

    double Trk[maxFile_][4];
    for(int i = 0; i < maxFile_; i++){

        TFile finput(files[i]);
        finput.cd();

        RooDataSet *dataTrkPt = (RooDataSet*)finput.Get("MuonTrketabin2/isTrk_pt/fit_eff");

        gTrkPt[i] = plotEffPt(dataTrkPt, 0);

    }

    TGraphAsymmErrors *pCNT = new TGraphAsymmErrors(3);
    TGraphAsymmErrors *pMBCNT = new TGraphAsymmErrors();

    if(Cent_){
        pCNT->SetPoint(0,10,Trk[1][1]);
        pCNT->SetPointError(0,0,0,Trk[1][3],Trk[1][2]);

        pCNT->SetPoint(1,20,Trk[2][1]);
        pCNT->SetPointError(1,0,0,Trk[2][3],Trk[2][2]);

        pCNT->SetPoint(2,30,Trk[3][1]);
        pCNT->SetPointError(2,0,0,Trk[3][3],Trk[3][2]);

        pCNT->SetPoint(3,40,Trk[4][1]);
        pCNT->SetPointError(3,0,0,Trk[4][3],Trk[4][2]);

        pCNT->SetPoint(4,50,Trk[5][1]);
        pCNT->SetPointError(4,0,0,Trk[5][3],Trk[5][2]);

        pCNT->SetPoint(5,60,Trk[6][1]);
        pCNT->SetPointError(5,0,0,Trk[6][3],Trk[6][2]);

        pCNT->SetPoint(6,70,Trk[7][1]);
        pCNT->SetPointError(6,0,0,Trk[7][3],Trk[7][2]);

        pCNT->SetPoint(7,80,Trk[8][1]);
        pCNT->SetPointError(7,0,0,Trk[8][3],Trk[8][2]);

        pCNT->SetMarkerColor(kRed+2);
        pCNT->SetMarkerStyle(20);
    }
    pMBCNT->SetPoint(0,113.0528,Trk[0][1]);
    pMBCNT->SetPointError(0,0,0,Trk[0][3],Trk[0][2]);

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
    if(Cent_) leg1->AddEntry(pCNT,"Inner Tracking Efficiency","pl");
    sprintf(legs,"Inner Tracking Efficiency: %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[0][1],Trk[0][2],Trk[0][3]);
    leg1->AddEntry(pMBCNT,legs,"pl");
    leg1->Draw("same");

    if(MC_){
/*
        c2->SaveAs("Jpsi_pPb_for_B_MC_Trk_CNT.png");
        c2->SaveAs("Jpsi_pPb_for_B_MC_Trk_CNT.pdf");
*/
    }else{
/*
        c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_CNT_test.pdf");
*/
/*
        c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_CNT_test.pdf");
*/
/*
        c2->SaveAs("Jpsi_pPb_RD_Trketabin3_1st_run_CNT_test.png");
        c2->SaveAs("Jpsi_pPb_RD_Trketabin3_1st_run_CNT_test.pdf");
*/
    }

    if(Cent_) {
        gTrkPt[1]->SetMarkerStyle(20);
        gTrkPt[1]->SetMarkerColor(634);
        gTrkPt[1]->GetXaxis()->SetTitle("p_{T} [GeV/c]");;
        gTrkPt[1]->GetXaxis()->CenterTitle();;
        gTrkPt[2]->SetMarkerStyle(23);
        gTrkPt[2]->SetMarkerColor(602);
        gTrkPt[3]->SetMarkerStyle(24);
        gTrkPt[3]->SetMarkerColor(618);
        gTrkPt[4]->SetMarkerStyle(25);
        gTrkPt[4]->SetMarkerColor(620);
        gTrkPt[5]->SetMarkerStyle(26);
        gTrkPt[5]->SetMarkerColor(621);
        gTrkPt[6]->SetMarkerStyle(27);
        gTrkPt[6]->SetMarkerColor(622);
        gTrkPt[7]->SetMarkerStyle(28);
        gTrkPt[7]->SetMarkerColor(623);
        gTrkPt[8]->SetMarkerStyle(29);
        gTrkPt[8]->SetMarkerColor(624);
        gTrkPt[0]->SetMarkerStyle(21);
        gTrkPt[0]->SetMarkerColor(802);

        formatTH1F(hPadPt,1,1,1);
        hPadPt->Draw();
        gTrkPt[1]->SetMinimum(0.5);
        gTrkPt[1]->SetMaximum(1.05);
        gTrkPt[1]->Draw("pz same");
        gTrkPt[2]->Draw("pz same");
        gTrkPt[3]->Draw("pz same");
        gTrkPt[4]->Draw("pz same");
        gTrkPt[5]->Draw("pz same");
        gTrkPt[6]->Draw("pz same");
        gTrkPt[7]->Draw("pz same");
        gTrkPt[8]->Draw("pz same");
        gTrkPt[0]->Draw("pz same");

        TLegend *leg3 = new TLegend(0.4899497,0.1678322,0.7876884,0.4947552,NULL,"brNDC"); // Size 0.03
        leg3->SetFillStyle(0);
        leg3->SetFillColor(0);
        leg3->SetBorderSize(0);
        leg3->SetTextSize(0.04);
        leg3->SetHeader(leg_title);
        sprintf(legs,"0 - 5 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[1][1],Trk[1][2],Trk[1][3]);
        leg3->AddEntry(gTrkPt[1],legs,"pl");
        sprintf(legs,"5 - 10 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[2][1],Trk[2][2],Trk[2][3]);
        leg3->AddEntry(gTrkPt[2],legs,"pl");
        sprintf(legs,"10 - 20 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[3][1],Trk[3][2],Trk[3][3]);
        leg3->AddEntry(gTrkPt[3],legs,"pl");
        sprintf(legs,"20 - 30 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[4][1],Trk[4][2],Trk[4][3]);
        leg3->AddEntry(gTrkPt[4],legs,"pl");
        sprintf(legs,"30 - 40 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[5][1],Trk[5][2],Trk[5][3]);
        leg3->AddEntry(gTrkPt[5],legs,"pl");
        sprintf(legs,"40 - 50 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[6][1],Trk[6][2],Trk[6][3]);
        leg3->AddEntry(gTrkPt[6],legs,"pl");
        sprintf(legs,"50 - 60 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[7][1],Trk[7][2],Trk[7][3]);
        leg3->AddEntry(gTrkPt[7],legs,"pl");
        sprintf(legs,"60 - 100 %% : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[8][1],Trk[8][2],Trk[8][3]);
        leg3->AddEntry(gTrkPt[8],legs,"pl");
        sprintf(legs,"MinBias : %0.3f^{ +%0.4f}_{ -%0.4f}",Trk[0][1],Trk[0][2],Trk[0][3]);
        leg3->AddEntry(gTrkPt[0],legs,"pl");
        leg3->Draw("same");

        if(MC_){
/*
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trk_pt_CNT.png");
            c2->SaveAs("Jpsi_pPb_for_B_MC_Trk_pt_CNT.pdf");
*/
        }else{
/*
            c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_pt_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_Trketabin2_1st_run_pt_CNT_test.pdf");
*/
/*
            c2->SaveAs("Jpsi_pPb_RD_Trketabin3_1st_run_pt_CNT_test.png");
            c2->SaveAs("Jpsi_pPb_RD_Trketabin3_1st_run_pt_CNT_test.pdf");
*/
        }

    }

    outfile->cd();

    if(Cent_){
        gTrkPt[1]->SetName("gTrk0005Pt");
        gTrkPt[2]->SetName("gTrk0510Pt");
        gTrkPt[3]->SetName("gTrk1020Pt");
        gTrkPt[4]->SetName("gTrk2030Pt");
        gTrkPt[5]->SetName("gTrk3040Pt");
        gTrkPt[6]->SetName("gTrk4050Pt");
        gTrkPt[7]->SetName("gTrk5060Pt");
        gTrkPt[8]->SetName("gTrk60100Pt");

        gTrkPt[1]->Write();
        gTrkPt[2]->Write();
        gTrkPt[3]->Write();
        gTrkPt[4]->Write();
        gTrkPt[5]->Write();
        gTrkPt[6]->Write();
        gTrkPt[7]->Write();
        gTrkPt[8]->Write();
    }
    gTrkPt[0]->SetName("gMBTrkPt");

    pCNT->SetName("pCNT");
    pMBCNT->SetName("pMBCNT");

    gTrkPt[0]->Write();

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

