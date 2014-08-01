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

void CompMuId_etabin_1st()
{
    gROOT->Macro("rootlogon.C");
    gStyle->SetOptStat(0);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetTitleYOffset(1.0);
/*
    TFile *f1 = new TFile("Jpsi_pPb_RD_MuIdNew2etabin1Rfb_CBpPoly_1st_Run_Eff_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuIdNew2etabin1Rfb_CBpPoly_1st_Run_Eff_test.root", "READ");
*/

    TFile *f1 = new TFile("Jpsi_pPb_RD_MuIdNew2etabin1CS_CBpPoly_1st_Run_Eff_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuIdNew2etabin1CS_CBpPoly_1st_Run_Eff_test.root", "READ");

/*
    TFile *f1 = new TFile("Jpsi_pPb_RD_MuIdNew2etabin1CS_CBpPoly_1st_Run_Eff_for_B_test.root", "READ");
    TFile *f2 = new TFile("Jpsi_pPb_MC_MuIdNew2etabin1CS_CBpPoly_1st_Run_Eff_for_B_test.root", "READ");
*/

    TGraph *gMuId1Pt = (TGraph*) f1->Get("MuId_pt_All");
    TGraph *gMuId2Pt = (TGraph*) f2->Get("MuId_pt_All");
    
    TCanvas *c1 = new TCanvas("c1","",700,600);
    TH1F *hPad2 = new TH1F("hPad2","",12,0,30);
    
    hPad2->GetYaxis()->CenterTitle();

    TLatex *lt1 = new TLatex();
    lt1->SetNDC();
    
    formatTH1F(hPad2,1,1,1);

    hPad2->GetYaxis()->SetTitleOffset(1.2);

    
    char legs[512];
  
    formatTGraph(gMuId1Pt,859,21,1);
    gMuId1Pt->SetMarkerSize(1.2);
    formatTGraph(gMuId2Pt,809,20,1);
    gMuId2Pt->SetMarkerSize(1.4);
    
    hPad2->SetMinimum(0.50);
    hPad2->SetMaximum(1.05);
    hPad2->Draw();
    gMuId1Pt->Draw("pz same");
    gMuId2Pt->Draw("pz same");
    
    TLegend *leg2 = new TLegend(0.29,0.17,0.63,0.40,NULL,"brNDC");
    leg2->SetFillStyle(0);
    leg2->SetFillColor(0);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.040);
    leg2->SetHeader("Muon ID Efficiency");
    sprintf(legs,"MC (PYTHIA+EvtGen)");
    leg2->AddEntry(gMuId2Pt,legs,"pl");
    sprintf(legs,"Data(pPb)");
    leg2->AddEntry(gMuId1Pt,legs,"pl");
    leg2->Draw();
    lt1->SetTextSize(0.035);
/*
    lt1->DrawLatex(0.20,0.54,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.20,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && ((-2.4 < y^{J/#psi} < -1.97)||(1.03 < y^{J/#psi} < 1.46)))");
*/

    lt1->DrawLatex(0.40,0.60,"(6.5 < p_{T}^{J/#psi} < 30.0 && -1.97 < y^{J/#psi} < 1.03) ||");
    lt1->DrawLatex(0.40,0.54,"(0.0 < p_{T}^{J/#psi} < 30.0 && -2.4 < y^{J/#psi} < -1.97) ||");
    lt1->DrawLatex(0.40,0.48,"(3.0 < p_{T}^{J/#psi} < 30.0 && 1.03 < y^{J/#psi} < 1.93)");


    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.93, -2.4 < #eta^{#mu} < -0.8");
//    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.93, -0.8 < #eta^{#mu} < 0.8");
//    lt1->DrawLatex(0.50,0.42,"-2.4 < y^{J/#psi} < 1.93, 0.8 < #eta^{#mu} < 1.93");

    lt1->SetTextSize(0.05);
//    lt1->DrawLatex(0.50,0.7,"CMS Preliminary");
    lt1->DrawLatex(0.50,0.63,"pPb  #sqrt{s_{NN}} = 5 TeV");
    lt1->SetTextSize(0.04);

/*
    c1->SaveAs("Jpsi_MuIdNew2etabin1Rfb_Comp_pPb_MC_pt_test.png");
    c1->SaveAs("Jpsi_MuIdNew2etabin1Rfb_Comp_pPb_MC_pt_test.pdf");
*/

    c1->SaveAs("Jpsi_MuIdNew2etabin1CS_Comp_pPb_MC_pt_test.png");
    c1->SaveAs("Jpsi_MuIdNew2etabin1CS_Comp_pPb_MC_pt_test.pdf");

/*
    c1->SaveAs("Jpsi_MuIdNew2etabin1CS_Comp_pPb_MC_pt_for_B_test.png");
    c1->SaveAs("Jpsi_MuIdNew2etabin1CS_Comp_pPb_MC_pt_for_B_test.pdf");
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

