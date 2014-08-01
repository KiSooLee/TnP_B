import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    

#PDFName = "twoGaussPlusExPoly6"
#PDFName = "twoGaussPlusPoly6"
PDFName = "twoGaussPlusPoly4"
#PDFName = "twoGaussPlusPoly3"
#PDFName = "gaussPlusPoly"
#PDFName = "gaussPlusExp"

process.TagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    # IO parameters:
    InputFileNames = cms.vstring("/afs/cern.ch/work/k/kilee/tutorial/cms538_pPb/src/UserCode/HiTagAndProbe/test/TnP2013/ProdMerge/MC/20140730_B/tnp_pA_MC_Prod_Merge_for_B_GEN_matching_20140730.root"),
    InputDirectoryName = cms.string("MuonTrketabin2"),
    InputTreeName = cms.string("fitter_tree"),
    OutputFileName = cms.string("tnp_pPb_Ana_MuTrketabin2_2GpP4_for_B_GEN_matching_test.root"),
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(True),
    binsForMassPlots = cms.uint32(50),
    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(50),
    
    # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
    Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "2", "5", "GeV/c^{2}"),
        pt = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
        p = cms.vstring("Probe p", "0", "1000", "GeV/c"),
        eta = cms.vstring("Probe #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
        tag_pt = cms.vstring("Tag p_{T}", "0", "1000", "GeV/c"),
        #pair_pt = cms.vstring("Pair p_{T}", "6.5", "30", "GeV/c"),
    ),

    # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
    Categories = cms.PSet(
        #mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),
        isTrk = cms.vstring("isTrk", "dummy[true=1,false=0]"),
    ),
    # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
    # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
    PDFs = cms.PSet(
      gaussPlusPoly = cms.vstring(
        "Gaussian::signal(mass, mean[3.1,3.0,3.2], sigma[0.200])",
        #"Gaussian::signal(mass, mean[3.1,3.0,3.2], sigma[0.15,0.05,0.25])",
        "Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-0.5,0.5], cPass3[0,-0.5,0.5]})",
        "Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-0.5,0.5], cFail3[0,-0.5,0.5]})",
        #"Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-0.5,0.5]})",
        #"Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-0.5,0.5]})",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      twoGaussPlusPoly3 = cms.vstring(
        "Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,1.00])",
        #"Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.30])",
        "Gaussian::signal2(mass, mean1[3.7,3.5,3.9], sigma2[0.15,0.05,1.00])",
        #"Gaussian::signal2(mass, mean1[3.1,3.0,3.2], sigma2[0.15,0.05,0.50])",
        #"Gaussian::signal2(mass, mean, sigma2[0.15,0.05,0.30])",
        "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
        "Chebychev::backgroundPass(mass, {cPass[0,-0.5,0.5], cPass2[0,-1.0,1.0], cPass3[0,-0.5,0.5]})",
        "Chebychev::backgroundFail(mass, {cFail[0,-0.5,0.5], cFail2[0,-1.0,1.0], cFail3[0,-0.5,0.5]})",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      oneGaussPlusPoly6 = cms.vstring(
        "Gaussian::signal(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.250])",
        #"Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.250])",
        #"Gaussian::signal2(mass, mean2[3.15,3.0,3.3], sigma2[0.10,0.05,0.250])",
        #"SUM::signal(vfrac[0.5,0.01,1.0]*signal1,signal2)",
        "Chebychev::backgroundPass1(mass,{cP1[-0.06,-1.0,1.0],cP2[-0.12,-1.0,1.0],cP3[-0.03,-1.0,1.0],cP4[0.019,-1.0,1.0],cP5[-0.0004,-1.0,1.0],cP6[-0.01,-1.0,1.0]})",
        "Exponential::backgroundPass2(mass, lp[0,-0.1,0.1])",
        "SUM::backgroundPass(b1frac[0.5,0.01,1.0]*backgroundPass1,backgroundPass2)",
        #"Chebychev::backgroundPass(mass,{cP1[0,-1.0,1.0],cP2[0,-1.0,1.0],cP3[0,-1.0,1.0],cP4[0,-1.0,1.0],cP5[0,-1.0,1.0],cP6[0,-1.0,1.0]})",
        "Chebychev::backgroundFail1(mass,{cF1[-0.06,-1.0,1.0],cF2[-0.12,-1.0,1.0],cF3[-0.03,-1.0,1.0],cF4[0.019,-1.0,1.0],cF5[-0.0004,-1.0,1.0],cF6[-0.01,-1.0,1.0]})",
        "Exponential::backgroundFail2(mass, lf[0,-0.1,0.1])",
        "SUM::backgroundFail(b2frac[0.5,0.01,1.0]*backgroundFail1,backgroundFail2)",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      twoGaussPlusExPoly6 = cms.vstring(
        "Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.250])",
        "Gaussian::signal2(mass, mean2[3.15,3.0,3.3], sigma2[0.10,0.05,0.250])",
        "SUM::signal(vfrac[0.5,0.01,1.0]*signal1,signal2)",
        "Chebychev::backgroundPass1(mass,{cP1[-0.06,-1.0,1.0],cP2[-0.12,-1.0,1.0],cP3[-0.03,-1.0,1.0],cP4[0.019,-1.0,1.0],cP5[-0.0004,-1.0,1.0],cP6[-0.01,-1.0,1.0]})",
        "Exponential::backgroundPass2(mass, lp[0,-0.1,0.1])",
        "SUM::backgroundPass(b1frac[0.5,0.01,1.0]*backgroundPass1,backgroundPass2)",
        "Chebychev::backgroundFail1(mass,{cF1[-0.06,-1.0,1.0],cF2[-0.12,-1.0,1.0],cF3[-0.03,-1.0,1.0],cF4[0.019,-1.0,1.0],cF5[-0.0004,-1.0,1.0],cF6[-0.01,-1.0,1.0]})",
        "Exponential::backgroundFail2(mass, lf[0,-0.1,0.1])",
        "SUM::backgroundFail(b2frac[0.5,0.01,1.0]*backgroundFail1,backgroundFail2)",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      twoGaussPlusPoly6 = cms.vstring(
        "Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.250])",
        "Gaussian::signal2(mass, mean2[3.15,3.0,3.3], sigma2[0.10,0.05,0.250])",
        "SUM::signal(vfrac[0.5,0.01,1.0]*signal1,signal2)",
#        "Chebychev::backgroundPass(mass,{cP1[-0.06,-1.0,1.0],cP2[-0.12,-1.0,1.0],cP3[-0.03,-1.0,1.0],cP4[0.019,-1.0,1.0],cP5[-0.0004,-1.0,1.0],cP6[-0.01,-1.0,1.0]})",
#        "Chebychev::backgroundFail(mass,{cF1[-0.06,-1.0,1.0],cF2[-0.12,-1.0,1.0],cF3[-0.03,-1.0,1.0],cF4[0.019,-1.0,1.0],cF5[-0.0004,-1.0,1.0],cF6[-0.01,-1.0,1.0]})",
        "Chebychev::backgroundPass(mass,{cP1[-0.48,-1.0,1.0],cP2[-0.03,-1.0,1.0],cP3[-0.07,-1.0,1.0],cP4[0.06,-1.0,1.0],cP5[-0.01,-1.0,1.0],cP6[-0.02,-1.0,1.0]})",
        "Chebychev::backgroundFail(mass,{cF1[-0.48,-1.0,1.0],cF2[-0.03,-1.0,1.0],cF3[-0.07,-1.0,1.0],cF4[0.06,-1.0,1.0],cF5[-0.01,-1.0,1.0],cF6[-0.02,-1.0,1.0]})",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      twoGaussPlusPoly4 = cms.vstring(
        "Gaussian::signal1(mass, mean[3.1,3.0,3.2], sigma[0.10,0.05,0.250])",
        "Gaussian::signal2(mass, mean2[3.15,3.0,3.3], sigma2[0.10,0.05,0.250])",
        "SUM::signal(vfrac[0.5,0.01,1.0]*signal1,signal2)",
        "Chebychev::backgroundPass(mass,{cP1[-0.5,-1.0,1.0],cP2[-0.1,-1.0,1.0],cP3[0.2,-1.0,1.0],cP4[-0.05,-1.0,1.0]})",
        "Chebychev::backgroundFail(mass,{cF1[-0.5,-1.0,1.0],cF2[-0.1,-1.0,1.0],cF3[0.2,-1.0,1.0],cF4[-0.05,-1.0,1.0]})",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
    ),
    # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
    # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
    Efficiencies = cms.PSet(
        #the name of the parameter set becomes the name of the directory
        isTrk_pt = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("isTrk","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0, 1.5, 3.0, 4.5, 6.0, 9.0, 20, 30),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        isTrk_1bin_pt_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("isTrk","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0,30),
                eta = cms.vdouble(-0.8, 0.8),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
    )
)

process.fitness = cms.Path(
    process.TagProbeFitTreeAnalyzer
)

