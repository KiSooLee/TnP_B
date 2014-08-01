import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )    

PDFName = "cbGaussPlusExpo"
#PDFName = "gaussPlusExpo"
#PDFName = "cbPlusExpo"

process.TagProbeFitTreeAnalyzer = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    # IO parameters:
    InputFileNames = cms.vstring("/afs/cern.ch/work/k/kilee/tutorial/cms538_pPb/src/UserCode/HiTagAndProbe/test/TnP2013/ProdMerge/MC/20140730_B/tnp_pA_MC_Prod_Merge_for_B_no_matching_20140730.root"),
    InputDirectoryName = cms.string("MuonTrgNew2CS"),
    InputTreeName = cms.string("fitter_tree"),
    OutputFileName = cms.string("tnp_pPb_Ana_MuTrgNew2CS_CBGpExp_MC_for_B_no_matching_test.root"),
    #numbrer of CPUs to use for fitting
    NumCPU = cms.uint32(1),
    # specifies wether to save the RooWorkspace containing the data for each bin and
    # the pdf object with the initial and final state snapshots
    SaveWorkspace = cms.bool(True),
    binsForMassPlots = cms.uint32(52),
    #WeightVariable = cms.string("weight"),
    
    # defines all the real variables of the probes available in the input tree and intended for use in the efficiencies
    Variables = cms.PSet(
        mass = cms.vstring("Tag-Probe Mass", "2.6", "3.5", "GeV/c^{2}"),
        pt = cms.vstring("Probe p_{T}", "0", "1000", "GeV/c"),
#        p = cms.vstring("Probe p", "0", "1000", "GeV/c"),
        eta = cms.vstring("Probe #eta", "-2.5", "2.5", ""),
        abseta = cms.vstring("Probe |#eta|", "0", "2.5", ""),
        tag_pt = cms.vstring("Tag p_{T}", "2.6", "1000", "GeV/c"),
        #weight = cms.vstring("weight","0.0","10000.0",""),
    ),

    # defines all the discrete variables of the probes available in the input tree and intended for use in the efficiency calculations
    Categories = cms.PSet(
        HLTL1Open = cms.vstring("HLTL1Open", "dummy[true=1,false=0]"),
#        HLTL1HighQ = cms.vstring("HLTL1HighQ", "dummy[true=1,false=0]"),
        HLTL2 = cms.vstring("HLTL2", "dummy[true=1,false=0]"),
        PAMu3 = cms.vstring("PAMu3", "dummy[true=1,false=0]"),
    ),

    # defines all the PDFs that will be available for the efficiency calculations; uses RooFit's "factory" syntax;
    # each pdf needs to define "signal", "backgroundPass", "backgroundFail" pdfs, "efficiency[0.9,0,1]" and "signalFractionInPassing[0.9]" are used for initial values  
    PDFs = cms.PSet(
      cbPlusExpo = cms.vstring(
        "CBShape::signal(mass, mean[3.1,3.0,3.2], sigma[0.05,0.01,0.1], alpha[1.0, 0.2, 3.0], n[2, 0.5, 100.])",
        "Exponential::backgroundPass(mass, lp[0,-5,5])",
        "Exponential::backgroundFail(mass, lf[0,-5,5])",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
      cbGaussPlusExpo = cms.vstring(
        "CBShape::signal1(mass, mean[3.1,3.0,3.2], sigma1[0.01,0.01,0.1], alpha[0.5, 0.2, 3.0], n[2, 0.5, 100.])",
		  "Gaussian::signal2(mass, mean[3.1, 3.0, 3.2], sigma2[0.04,0.01,0.1])",
		  "SUM::signal(signal1,vFrac[0.8,0,1]*signal2)",
        "Exponential::backgroundPass(mass, lp[0,-5,5])",
        "Exponential::backgroundFail(mass, lf[0,-5,5])",
        "efficiency[0.9,0,1]",
        "signalFractionInPassing[0.9]"
      ),
    ),
    # defines a set of efficiency calculations, what PDF to use for fitting and how to bin the data;
    # there will be a separate output directory for each calculation that includes a simultaneous fit, side band subtraction and counting. 
    Efficiencies = cms.PSet(
        #the name of the parameter set becomes the name of the directory
        PAMu3_pt = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PAMu3","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0, 1.5, 3.0, 4.5, 6.0, 9.0, 20, 30),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        PAMu3_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PAMu3","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                eta = cms.vdouble(-2.4, -1.5, -0.5, 0.5, 1.5, 2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        PAMu3_1bin_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("PAMu3","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                eta = cms.vdouble(-2.4, 2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        PAMu3_1bin_pt_eta = cms.PSet(
          EfficiencyCategoryAndState = cms.vstring("PAMu3","true"),
          UnbinnedVariables = cms.vstring("mass"),
          BinnedVariables = cms.PSet(
              pt = cms.vdouble(0., 30),
              eta = cms.vdouble(-2.4, 2.4),
          ),  
          BinToPDFmap = cms.vstring(PDFName)
        ),
        ## HLT L2 DoubleMu3
        HLTL2_pt = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("HLTL2","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                pt = cms.vdouble(0.0, 1.5, 3.0, 4.5, 6.0, 9.0, 20, 30),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        HLTL2_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("HLTL2","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                eta = cms.vdouble(-2.4, -1.5, -0.5, 0.5, 1.5, 2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        HLTL2_1bin_eta = cms.PSet(
            EfficiencyCategoryAndState = cms.vstring("HLTL2","true"),
            UnbinnedVariables = cms.vstring("mass"),
            BinnedVariables = cms.PSet(
                eta = cms.vdouble(-2.4, 2.4),
            ),
            BinToPDFmap = cms.vstring(PDFName)
        ),
        HLTL2_1bin_pt_eta = cms.PSet(
          EfficiencyCategoryAndState = cms.vstring("HLTL2","true"),
          UnbinnedVariables = cms.vstring("mass"),
          BinnedVariables = cms.PSet(
              pt = cms.vdouble(0.0, 30),
              eta = cms.vdouble(-2.4, 2.4),
          ),  
          BinToPDFmap = cms.vstring(PDFName)
        ),
    )
)

process.fitness = cms.Path(
    process.TagProbeFitTreeAnalyzer
)
