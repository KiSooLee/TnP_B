#include <TROOT.h>
#include <TFile.h>
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
void TnPDrawMass_etabin2_1st_for_B();

int Mode_ = 1; // 1 - Trg, 2 - Trk, 3 - MuId
const int maxfile_ = 1; 
const int ptBins = 7;
TString Cat1 = "RD"; // MC or RD
TString Cat2 = "Trg"; // Trg, Trk or MuId 
TString Cat3 = "Trigger"; // Trigger, Inner Tracker or Muon Id
TString data_set = "Data Set : Prompt Reco pPb 2013"; // ReReco, Prompt Reco or MC weighted
int TrgBit_ = 1; // 1 - L1DoubleMuOpen, 2 - L1DoubleMuHighQ, 3 - L2DoubleMu3

TString outfile_ = "Jpsi_pPb_RD_MuTrgNew2etabin2CS_CBGpExp_1st_Run_Eff_Mass_Plots_total_for_B_test.ps";

void TnPDrawMass_etabin2_1st_for_B() {
    gROOT->Macro("rootlogon.C");

    char *infile;
    char *files[maxfile_] = {
        "../../Ana/RD/MuTrg/tnp_pPb_Ana_MuTrgNew2etabin2CS_CBGpExp_1st_Run_for_B_test.root",
    };

    TString outname_in, outname_mid, outname_out;
    TString middle_name, middle_name2;
    int Mode = 0;

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetTitle(0);

    gStyle->SetPaperSize(20,26);
    TCanvas *c1 = new TCanvas();
    Int_t type = 112;
    TString out1 = outfile_ + "[";
    c1->Print(out1);

    for(int l = 0; l < maxfile_; l++){

        infile = files[l];
        TFile *f = new TFile(infile);
        //TFile finput(infile);
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


        outname_in = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps[";
        outname_mid = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps";
        outname_out = "Jpsi_" + Cat1 + "_" + Cat2 + "_Mass_" + middle_name + ".ps]";
        

        cout<<" Out Name : "<<outname_in<<" "<<outname_mid<<" "<<outname_in<<endl;
        

        TString tot_dir, dir_pt, dir_eta, dir_suffix, intg;

        TString ptbins[ptBins] = {"pt_bin0_", "pt_bin1_", "pt_bin2_", "pt_bin3_", "pt_bin4_",
                                 "pt_bin5_", "pt_bin6_"};

        if(Mode_ == 1) {
            if(TrgBit_ == 1){
                dir_pt = "MuonTrgNew2etabin2CS/PAMu3_pt/";
                intg = "MuonTrgNew2etabin2CS/PAMu3_1bin_pt_eta/eta_bin0__pt_bin0_";
                dir_suffix = "_cbCaussPlusExpo";
            }else if(TrgBit_ == 2){
                dir_pt = "MuonTrgNew/HLTL1HighQ_pt/";
                dir_eta = "MuonTrgNew/HLTL1HighQ_eta/";
                dir_suffix = "_cbPlusExpo";
                intg = "MuonTrgNew/HLTL1HighQ_1bin_pt_eta/pt_bin0__eta_bin0_";
            }else if(TrgBit_ == 3){
                dir_pt = "MuonTrgNew/HLTL2_pt/";
                dir_eta = "MuonTrgNew/HLTL2_eta/";
                dir_suffix = "_cbPlusExpo";
                intg = "MuonTrgNew/HLTL2_1bin_pt_eta/pt_bin0__eta_bin0_";
            }
        }else if (Mode_ == 2){
            dir_pt = "MuonTrketabin2/isTrk_pt/";
            intg = "MuonTrketabin2/isTrk_1bin_pt_eta/eta_bin0__eta_bin0_";
            dir_suffix = "_twoGaussPlusPoly4";
        }else if (Mode_ == 3){
            dir_pt = "MuonIDNew2etabin2CS/PassingTrk_pt/";
            intg = "/MuonIDNew2etabin2CS/PassingTrk_1bin_eta_pt/eta_bin0__pt_bin0_";
            dir_suffix = "_cbPlusPoly";

        }
        ///*

        cout<<" Total directory : "<<tot_dir<<endl;
        gStyle->SetPaperSize(20,26);

        Int_t type = 112;

        c1->Print(outname_in);
        int cnt = 0;

        TCanvas *tmp1 = new TCanvas();
        tmp1->cd();

        TString title;
        title = "J/#psi " + Cat3 + " Efficiency Heavy Ion TnP Result (" + middle_name2 + ")";

        TLatex fitInfo;
        fitInfo.SetTextAlign(13);
        fitInfo.SetTextSize(0.05);
        fitInfo.DrawLatex(0.05,0.95, title);
        fitInfo.DrawLatex(0.05,0.90, data_set);
        fitInfo.SetTextSize(0.04);
        fitInfo.DrawLatex(0.05,0.80, "FitFunction Condition");
        fitInfo.SetTextSize(0.03);
        if(Mode_ == 1){
            fitInfo.DrawLatex(0.1,0.60,"CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.05,0.01,0.1], alpha[1.0, 0.2, 2.0], n[2, 0.5, 100.])");
            fitInfo.DrawLatex(0.1,0.55,"Exponential::backgroundPass(mass, lp[0,-5,5])");
            fitInfo.DrawLatex(0.1,0.50,"Exponential::backgroundFail(mass, lf[0,-5,5])");
        }else if(Mode_ == 2) {
            fitInfo.DrawLatex(0.1,0.70,"Gaussian::G1(mass, mean, sigma1[0.15,0.05,0.25])");
            fitInfo.DrawLatex(0.1,0.65,"Gaussian::G2(mass, mean, sigma2[0.02,0.01,0.1])");
            fitInfo.DrawLatex(0.1,0.60,"SUM::signal(coef[0.1,0,1]*G1,G2)");
            fitInfo.DrawLatex(0.1,0.55,"Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-0.5,0.5]})");
            fitInfo.DrawLatex(0.1,0.50,"Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-0.5,0.5]})");
        }else{
            fitInfo.DrawLatex(0.1,0.60,"CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.05,0.01,0.1], alpha[1.0, 0.2, 3.0], n[1, 1.0, 100.])");
            fitInfo.DrawLatex(0.1,0.55,"Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-0.5,0.5]})");
            fitInfo.DrawLatex(0.1,0.50,"Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-0.5,0.5]})");
        }
        fitInfo.SetTextSize(0.04);
        fitInfo.DrawLatex(0.05,0.40,"Bin Information");
        fitInfo.SetTextSize(0.03);
        fitInfo.DrawLatex(0.1,0.30,"#eta : -2.4,-1.97,-1.72,-1.47,-1.22,-0.97,-0.72,0.47,0.22,0.03,0.28,0.53,0.78,1.03,1.46");
        fitInfo.DrawLatex(0.1,0.25,"p_{T} [GeV/c] : 6.5,7.5,8.5,9.5,11,14,30");
        c1 = (TCanvas *)tmp1->Clone();
        c1->Print(outname_mid);
        c1->Print(outfile_);

        TString mid_title = "Centrality : (" + middle_name2 + ")";
        TString leg_title = Cat2 + " " + Cat3 + " Efficiency (" + middle_name2 + ")";

        if(Mode < 1) {
            // pt
            for(int j = 0; j < ptBins; j++){
                cout<<" dir_pt : "<<dir_pt<<", ptbins : "<<ptbins[j]<<", dir_suffix : "<<dir_suffix<<endl;
                TString tot_dir = dir_pt + ptbins[j] + dir_suffix;
                f->cd(tot_dir);
                cout<<" tot_dir : "<<tot_dir<<endl;
                TDirectory *root_dir = gDirectory;
                TIter rootnextkey( root_dir->GetListOfKeys() );
                root_dir->cd();
                TKey *rootkey;
                TCanvas *ctmp = new TCanvas();
                ctmp->cd();


                TLatex l;
                l.SetTextAlign(13);
                l.SetTextSize(0.06);
                l.DrawLatex(0.1,0.8,mid_title);
                l.DrawLatex(0.1,0.6,"Bin : ");
                l.SetTextSize(0.04);
                l.DrawLatex(0.1,0.5,tot_dir);
                ctmp->Update();
                c1 = (TCanvas *)ctmp->Clone();
                c1->Print(outname_mid);
                c1->Print(outfile_);

                while( rootkey = (TKey*)rootnextkey() )
                {
                    TObject *rootobj = rootkey->ReadObj();
                    TDirectory *rdir = gDirectory;
                    TIter rdirnextkey(rdir->GetListOfKeys());
                    rdir->cd();
                    TKey *dir_key;
                    while( dir_key = (TKey*)rdirnextkey())
                    {
                        if (rootobj->IsA()->InheritsFrom("TCanvas")){
                            c1 = (TCanvas *)rootobj;
                            c1->Print(outname_mid);
                            c1->Print(outfile_);
                            cnt++;
                            cout<<"Count : "<<cnt<<endl;
                            if(cnt > 0) break;
                        }
                    }
                }
            }
			}

        tot_dir = intg + dir_suffix;
        f->cd(tot_dir);
        cout<<" tot_dir : "<<tot_dir<<endl;
        TDirectory *root_dir = gDirectory;
        TIter rootnextkey( root_dir->GetListOfKeys() );
        root_dir->cd();
        TKey *rootkey;
        TCanvas *ctmp = new TCanvas();
        ctmp->cd();


        TLatex l;
        l.SetTextAlign(13);
        l.SetTextSize(0.06);
        l.DrawLatex(0.1,0.8,mid_title);
        l.DrawLatex(0.1,0.6,"Bin : ");
        l.SetTextSize(0.04);
        l.DrawLatex(0.1,0.5,tot_dir);
        ctmp->Update();
        c1 = (TCanvas *)ctmp->Clone();
        c1->Print(outname_mid);
        c1->Print(outfile_);

        while( rootkey = (TKey*)rootnextkey() )
        {
            TObject *rootobj = rootkey->ReadObj();
            TDirectory *rdir = gDirectory;
            TIter rdirnextkey(rdir->GetListOfKeys());
            rdir->cd();
            TKey *dir_key;
            while( dir_key = (TKey*)rdirnextkey())
            {
                if (rootobj->IsA()->InheritsFrom("TCanvas")){
                    c1 = (TCanvas *)rootobj;
                    c1->Print(outname_mid);
                    c1->Print(outfile_);
                    cnt++;
                    cout<<"Count : "<<cnt<<endl;
                    if(cnt > 0) break;
                }
            }
        }
    }
    c1->Print(outname_out);
    TString out2 = outfile_ + "]";
    c1->Print(out2);


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

//    const int xnbins = xAx->getBinning().numBins();
//    const int ynbins = yAx->getBinning().numBins();

    //double xbins[] = {-2.4, -1.6, -0.8, 0.0, 0.8, 1.6, 2.4};
    //double ybins[] = {0, 2, 3, 5, 8, 10, 20};

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

