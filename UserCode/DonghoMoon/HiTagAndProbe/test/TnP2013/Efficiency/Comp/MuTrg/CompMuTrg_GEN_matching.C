//////////////////////////////////////////////////////////////////////////////////
//This code compare RD and MC("and" or "or" pp).                                //
//                                                                              //
//                                                             2013-12-21       //
//                                                             by KiSoo Lee     //
//////////////////////////////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TNtuple.h>

#include <TCanvas.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLatex.h>

#include <TMath.h>
#include <TSystem.h>
#include <TStyle.h>
#include <Riostream.h>
#include <TString.h>
#include <TInterpreter.h>
#include <TBrowser.h>

void CalEffErr(TGraph *a, double *b);
TGraphAsymmErrors *getEff(TH1F *h1, TH1F *h2);
TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2);
void formatTGraph(TGraph* a, int b, int c, int d);
void formatTCanv(TCanvas* a);
void formatTLeg(TLegend* a);
void formatTH1F(TH1* a, int b, int c, int d);

void CompMuTrg()
{
    gROOT->Macro("rootlogon.C");
    gStyle->SetOptStat(0);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetTitleYOffset(1.0);
/*
    TFile *f1 = new TFile("Jpsi_pPb_RD_MuTrgNew2Rfb_CBGpExp_1st_Run_Eff_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuTrgNew2Rfb_CBGpExp_1st_Run_Eff_test.root", "READ");
*/

    TFile *f1 = new TFile("Jpsi_pPb_RD_MuTrgNew2CS_CBGpExp_1st_Run_Eff_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuTrgNew2CS_CBGpExp_1st_Run_Eff_test.root", "READ");

/*
    TFile *f1 = new TFile("Jpsi_pPb_RD_MuTrgNew2CS_CBGpExp_1st_Run_Eff_Mu3_for_B_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuTrgNew2CS_CBGpExp_1st_Run_Eff_Mu3_for_B_test.root", "READ");
*/
    TGraph *gTrg1Eta = (TGraph*) f1->Get("Trg_eta_All");
    TGraph *gTrg1IntEta = (TGraph*) f1->Get("Trg_1bin_pt_eta_All");
    TGraph *gTrg1Pt = (TGraph*) f1->Get("Trg_pt_All");
    TGraph *gTrg2Eta = (TGraph*) f2->Get("Trg_eta_All");
    TGraph *gTrg2IntEta = (TGraph*) f2->Get("Trg_1bin_pt_eta_All");
    TGraph *gTrg2Pt = (TGraph*) f2->Get("Trg_pt_All");
    TGraph *gTrg1CNT = (TGraph*) f1->Get("pCNT");
    TGraph *gTrg1MBCNT = (TGraph*) f1->Get("pMBCNT");
    TGraph *gTrg2CNT = (TGraph*) f2->Get("pCNT");
    TGraph *gTrg2MBCNT = (TGraph*) f2->Get("pMBCNT");
    
    TCanvas *c1 = new TCanvas("c1","",700,600);
    TH1F *hPad1 = new TH1F("hPad1","",15,-2.4,1.93);
    TH1F *hPad2 = new TH1F("hPad2","",12,0,30);
    TH1F *hPad3 = new TH1F("hPad3","",10,0,100);
    
    hPad1->GetYaxis()->CenterTitle();
    hPad2->GetYaxis()->CenterTitle();
    hPad3->GetYaxis()->CenterTitle();

    double Trg1Eta[4];
    CalEffErr(gTrg1IntEta, Trg1Eta);    
    double Trg2Eta[4];
    CalEffErr(gTrg2IntEta, Trg2Eta);    
    
    TLatex *lt1 = new TLatex();
    lt1->SetNDC();
    
    formatTH1F(hPad1,1,1,2);
    formatTH1F(hPad2,1,1,1);
    formatTH1F(hPad3,1,1,5);

    formatTGraph(gTrg1Eta,859,21,2);
    gTrg1Eta->SetMarkerSize(1.2);
    formatTGraph(gTrg2Eta,809,20,2);
    gTrg2Eta->SetMarkerSize(1.4);
    
    hPad1->SetMaximum(1.05);
    hPad1->SetMinimum(0.00);

    hPad1->GetYaxis()->SetTitleOffset(1.2);
    hPad2->GetYaxis()->SetTitleOffset(1.2);
    hPad3->GetYaxis()->SetTitleOffset(1.2);

    hPad1->Draw();
    gTrg1Eta->Draw("pz same");
    gTrg2Eta->Draw("pz same");
    
    char legs[512];
    TLegend *leg1 = new TLegend(0.29,0.17,0.63,0.40);
    leg1->SetFillStyle(0);
    leg1->SetFillColor(0);
    leg1->SetBorderSize(0);
    leg1->SetTextSize(0.040);
    leg1->SetHeader("Trigger Efficiency");
    sprintf(legs,"MC (PYTHIA+EvtGen): %.3f^{ + %.4f}_{ - %.4f}", Trg2Eta[0], Trg2Eta[1], Trg2Eta[2]);
    leg1->AddEntry(gTrg2Eta,legs,"pl");
    sprintf(legs,"Data(pPb): %.3f^{ + %.4f}_{ - %.4f}", Trg1Eta[0], Trg1Eta[1], Trg1Eta[2]);
    leg1->AddEntry(gTrg1Eta,legs,"pl");
    leg1->Draw();
    lt1->SetTextSize(0.035);
/*
    lt1->DrawLatex(0.50,0.48,"6.5 < p_{T}^{J/#psi} < 30.0, 1.47 < |y| < 2.4");
    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.47, -2.4 < #eta^{#mu} < 1.47");
*/
/*
    lt1->DrawLatex(0.20,0.54,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.20,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && ((-2.4 < y^{J/#psi} < -1.97)||(1.03 < y^{J/#psi} < 1.46)))");
*/

    lt1->DrawLatex(0.40,0.50,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.40,0.44,"(0.0 < p_{T}^{J/#psi} < 30.0 && -2.4 < y^{J/#psi} < -1.97) ||");
    lt1->DrawLatex(0.40,0.38,"(3.0 < p_{T}^{J/#psi} < 30.0 && 1.03 < y^{J/#psi} < 1.93)");


    lt1->SetTextSize(0.05);
//    lt1->DrawLatex(0.50,0.7,"CMS Preliminary");
    lt1->DrawLatex(0.50,0.53,"pPb  #sqrt{s_{NN}} = 5 TeV");
    lt1->SetTextSize(0.04);
/*
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_eta_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_eta_L1_test.pdf");
*/

    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_eta_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_eta_L1_test.pdf");

/*
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_eta_Mu3_for_B_test.png");
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_eta_Mu3_for_B_test.pdf");
*/
    formatTGraph(gTrg1Pt,859,21,1);
    gTrg1Pt->SetMarkerSize(1.2);
    formatTGraph(gTrg2Pt,809,20,1);
    gTrg2Pt->SetMarkerSize(1.4);
    
    hPad2->SetMinimum(0.20);
    hPad2->SetMaximum(1.05);
    hPad2->Draw();
    gTrg1Pt->Draw("pz same");
    gTrg2Pt->Draw("pz same");
    
    TLegend *leg2 = new TLegend(0.29,0.17,0.63,0.40,NULL,"brNDC");
    leg2->SetFillStyle(0);
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.040);
    leg2->SetHeader("Trigger Efficiency");
    sprintf(legs,"MC (PYTHIA+EvtGen): %.3f^{ + %.4f}_{ - %.4f}", Trg2Eta[0], Trg2Eta[1], Trg2Eta[2]);
    leg2->AddEntry(gTrg2Pt,legs,"pl");
    sprintf(legs,"Data(pPb): %.3f^{ + %.4f}_{ - %.4f}", Trg1Eta[0], Trg1Eta[1], Trg1Eta[2]);
    leg2->AddEntry(gTrg1Pt,legs,"pl");
    leg2->Draw();
    lt1->SetTextSize(0.035);
/*
    lt1->DrawLatex(0.50,0.48,"6.5 < p_{T}^{J/#psi} < 30.0, 1.47 < |y| < 2.4");
    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.47, -2.4 < #eta^{#mu} < 1.47");
*/
/*
    lt1->DrawLatex(0.20,0.54,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.20,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && ((-2.4 < y^{J/#psi} < -1.97)||(1.03 < y^{J/#psi} < 1.46)))");
*/

    lt1->DrawLatex(0.40,0.60,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.40,0.54,"(0.0 < p_{T}^{J/#psi} < 30.0 && -2.4 < y^{J/#psi} < -1.97) ||");
    lt1->DrawLatex(0.40,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && 1.03 < y^{J/#psi} < 1.93)");


    lt1->SetTextSize(0.05);
//    lt1->DrawLatex(0.50,0.7,"CMS Preliminary");
    lt1->DrawLatex(0.50,0.63,"pPb  #sqrt{s_{NN}} = 5 TeV");
    lt1->SetTextSize(0.04);
/*
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_pt_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_pt_L1_test.pdf");
*/

    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_pt_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_pt_L1_test.pdf");

/*
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_pt_Mu3_for_B_test.png");
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_pt_Mu3_for_B_test.pdf");
*/
    formatTGraph(gTrg1CNT,859,21,1);
    gTrg1CNT->SetMarkerSize(1.4);
    formatTGraph(gTrg1MBCNT,859,25,1);
    gTrg1MBCNT->SetMarkerSize(1.4);
    formatTGraph(gTrg2CNT,809,20,1);
    gTrg2CNT->SetMarkerSize(1.4);
    formatTGraph(gTrg2MBCNT,809,24,1);
    gTrg2MBCNT->SetMarkerSize(1.4);
    
    hPad3->SetMinimum(0.50);
    hPad3->SetMaximum(1.05);
    hPad3->GetXaxis()->SetTitle("Centrality Bin");
    hPad3->Draw();
    gTrg1CNT->Draw("pz same");
    gTrg1MBCNT->Draw("pz same");
    gTrg2MBCNT->Draw("pz same");
    
    TLegend *leg3 = new TLegend(0.29,0.17,0.63,0.40);
    leg3->SetFillStyle(0);
    leg3->SetFillColor(0);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.040);
    leg3->SetHeader("Trigger Efficiency");
    sprintf(legs,"MC (PYTHIA+EvtGen): %.3f^{ + %.4f}_{ - %.4f}", Trg2Eta[0], Trg2Eta[1], Trg2Eta[2]);
    leg3->AddEntry(gTrg2MBCNT,legs,"pl");
    sprintf(legs,"Data(pPb): %.3f^{ + %.4f}_{ - %.4f}", Trg1Eta[0], Trg1Eta[1], Trg1Eta[2]);
    leg3->AddEntry(gTrg1MBCNT,legs,"pl");
    leg3->Draw();
    lt1->SetTextSize(0.035);
/*
    lt1->DrawLatex(0.50,0.48,"6.5 < p_{T}^{J/#psi} < 30.0, 1.47 < |y| < 2.4");
    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.47, -2.4 < #eta^{#mu} < 1.47");
*/
/*
    lt1->DrawLatex(0.20,0.54,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.20,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && ((-2.4 < y^{J/#psi} < -1.97)||(1.03 < y^{J/#psi} < 1.46)))");
*/

    lt1->DrawLatex(0.40,0.60,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.40,0.54,"(0.0 < p_{T}^{J/#psi} < 30.0 && -2.4 < y^{J/#psi} < -1.97) ||");
    lt1->DrawLatex(0.40,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && 1.03 < y^{J/#psi} < 1.93)");


    lt1->SetTextSize(0.05);
//    lt1->DrawLatex(0.50,0.7,"CMS Preliminary");
    lt1->DrawLatex(0.50,0.63,"pPb  #sqrt{s_{NN}} = 5 TeV");
    lt1->SetTextSize(0.04);
/*
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_CNT_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2Rfb_Comp_pPb_MC_CNT_L1_test.pdf");
*/
/*
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_CNT_L1_test.png");
    c1->SaveAs("Jpsi_TrgNew2CS_Comp_pPb_MC_CNT_L1_test.pdf");
*/
}
//(TH1, color, style, pt, eta, rapidity)
void formatTH1F(TH1* a, int b, int c, int d){
    a->SetLineWidth(2);
    a->SetLineStyle(c);
    a->SetMarkerSize(2);
    a->SetLineColor(b);
    a->SetMarkerColor(b);
    a->GetYaxis()->SetTitle("Single #mu Efficiency");
    if(d == 1){	
        a->GetXaxis()->SetTitle("p_{T}^{#mu} (GeV/c)"); 
    }else if(d == 2){
        a->GetXaxis()->SetTitle("#eta^{#mu}"); 
    }else if(d == 3){
        a->GetXaxis()->SetTitle("rapidity^{#mu}"); 
    }else if(d == 4){
        a->GetXaxis()->SetTitle("Centrality");
    }else if(d == 5){
        a->GetXaxis()->SetTitle("Centrality (%)");
    }
    a->GetXaxis()->CenterTitle(true);
}     

void formatTLeg(TLegend* a){
    
    a->SetFillStyle(0);  
    a->SetFillColor(0); 
    a->SetBorderSize(0);
    a->SetTextSize(0.03);
    
}

void formatTCanv(TCanvas* a){
    a->SetBorderSize(2);
    a->SetFrameFillColor(0);
    a->cd();
    a->SetGrid(1);
    a->SetTickx();
    a->SetTicky();
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
    a->GetXaxis()->SetTitleOffset(1.0);
    a->GetYaxis()->SetTitle("Efficiency");
    //a->GetXaxis()->CenterTitle();
    if(d == 1){	
        a->GetXaxis()->SetTitle("p_{T} (GeV/c)"); 
    }else if(d == 2){
        a->GetXaxis()->SetTitle("eta"); 
    }else if(d == 3){
        a->GetXaxis()->SetTitle("rapidity"); 
    }
    a->GetYaxis()->SetRangeUser(0,1);
    a->GetXaxis()->SetRangeUser(0,10);
    a->GetYaxis()->SetLabelSize(0.05);
    a->GetYaxis()->SetTitleSize(0.05);
    a->GetYaxis()->SetTitleOffset(1.3);
    
    
}

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2)
{
    TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
    gEfficiency->BayesDivide(h1,h2);
    return gEfficiency;
}

TGraphAsymmErrors *getEff(TH1F *h1, TH1F *h2)
{
    
    h1->Sumw2();
    h2->Sumw2();
    
    TGraphAsymmErrors *result = calcEff(h1,h2);
    return result;
}
void CalEffErr(TGraph *a, double *b){
    const int nbins = 100;
    double x[nbins], y[nbins];
    double sum = 0, errHighSum = 0, errLowSum = 0, sqSumHigh = 0, sqSumLow = 0;
    //double b[3] = 0;
    
    int nBins = a->GetN();
    for(int i=0;i<a->GetN();i++){
        a->GetPoint(i,x[i],y[i]);
        //cout<<"Eff x = "<<x[i]<<" y = "<<y[i]<<endl;
        double eHigh = a->GetErrorYhigh(i);
        double eLow = a->GetErrorYlow(i);
        //cout<<"Err high = "<<eHigh<<" low = "<<eLow<<endl;
        sum += y[i];
        errHighSum += eHigh;
        sqSumHigh += eHigh*eHigh;
        errLowSum += eLow;
        sqSumLow += eLow*eLow;
    }
    b[0] = sum/nBins;
    b[1] = sqrt(sqSumHigh)/nBins;
    b[2] = sqrt(sqSumLow)/nBins;
    //cout<<"b1 : "<<b[0]<<", b2 : "<<b[1]<<", b3 : "<<b[2]<<endl;
    
    cout<<b[0]<<"^{"<<b[1]<<"}_{"<<b[2]<<"}"<<endl;
    //return b[3];
}

