import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    

#PDFName = "cbPlusExpo"
#PDFName = "gaussPlusExpo"
PDFName = "cbPlusPoly"

process.TagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    # IO parameters:
    InputFileNames = cms.vstring("/afs/cern.ch/work/k/kilee/tutorial/cms538_pPb/src/UserCode/HiTagAndProbe/test/TnP2013/ProdMerge/MC/20140730_B/tnp_pA_MC_Prod_Merge_for_B_GEN_matching_20140730.root"),
    InputDirectoryName = cms.string("MuonIDNew2CS"),
    InputTreeName = cms.string("fitter_tree"),
    OutputFileName = cms.string("tnp_pPb_Ana_MuIdNew2CS_CBpPoly_MC_for_B_GEN_matching_test.root"),
    #numbrer of CPUs to use for fitting
    NumCPU = cms.uint32(1),
    # specifies wether to save the RooWorkspace containing the data for each bin and
    # the pdf object with the initial and final state snapshots
    #binnedFit = cms.bool(True),
    #binsForFit = cms.uint32(30),
    #binsForMassPlots = cms.uint32(50),
    binsForMassPlots = cms.uint32(50),
    binnedFit = cms.bool(True),
    binsForFit = cms.uint32(50),
    
    # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
    Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "2.6", "3.5", "GeV/c^{2}"),
        pt = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
        p = cms.vstring("Probe p", "0", "1000", "GeV/c"),
        eta = cms.vstring("Probe #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
        tag_pt = cms.vstring("Tag p_{T}", "2.6", "1000", "GeV/c"),
    ),
    # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
    Categories = cms.PSet(
        #mcTrue = cms.vstring("MC true", "dummy[true=1,false=0]"),
        PassingTrk = cms.vstring("PassingTrk", "dummy[true=1,false=0]"),
    ),

    # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
    # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
    PDFs = cms.PSet(
      cbPlusExpo = cms.vstring(
        "CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.5], alpha[2.0, 0.2, 4.0], n[4, 0.5, 100.])",
        #"CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.02,0.02,0.1], alpha[1.0, 0.2, 3.0], n[4, 0.5, 100.])",
        "Exponential::backgroundPass(mass, lp[0,-5,5])",
        "Exponential::backgroundFail(mass, lf[0,-5,5])",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      cbPlusPoly = cms.vstring(
        "CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.02, 0.01, 0.1], alpha[2.0, 1.0, 5.0], n[1, 0.5, 100.])",
        #"CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.02, 0.01, 0.1], alpha[1.0, 0.5, 4.0], n[1, 0.5, 100.])",
        "Chebychev::backgroundPass(mass, {cPass[0,-1.0,1.0], cPass2[0,-1.0,1.0]})",
        "Chebychev::backgroundFail(mass, {cFail[0,-1.0,1.0], cFail2[0,-1.0,1.0]})",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      gaussPlusExpo = cms.vstring(
        "Gaussian::signal(mass, mean[3.1,3.0,3.2], sigma[0.02, 0.01, 0.1])",
        #"Gaussian::signal(mass, mean[3.1,3.0,3.2], sigma[0.04])",
        "Chebychev::backgroundPass(mass, {cPass[0,-1.0,1.0], cPass2[0,-1.0,1.0]})",
        "Chebychev::backgroundFail(mass, {cFail[0,-1.0,1.0], cFail2[0,-1.0,1.0]})",
        #"Exponential::backgroundPass(mass, lp[0,-5,5])",
        #"Exponential::backgroundFail(mass, lf[0,-5,5])",  # same slope, they're both muons
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
    ),
    # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
    # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
    Efficiencies = cms.PSet(
        #the name of the parameter set becomes the name of the directory
        PassingTrk_pt = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PassingTrk","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0, 1.5, 3.0, 4.5, 6.0, 9.0, 20, 30),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        PassingTrk_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PassingTrk","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                eta = cms.vdouble(-2.4, -1.5, -0.5, 0.5, 1.5, 2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        PassingTrk_1bin_eta_pt = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PassingTrk","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0,30),
                eta = cms.vdouble(-2.4,2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
    )
)

process.fitness = cms.Path(
    process.TagProbeFitTreeAnalyzer
)

