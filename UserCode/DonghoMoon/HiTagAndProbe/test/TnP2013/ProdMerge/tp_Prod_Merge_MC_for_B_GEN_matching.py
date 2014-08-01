import FWCore.ParameterSet.Config as cms

process = cms.Process("TnPProd")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True),
    SkipEvent = cms.untracked.vstring('ProductNotFound'),
)
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'STARTHI53_V25::All'

from HeavyIonsAnalysis.Configuration.CommonFunctions_cff import *
overrideCentrality(process)

process.HeavyIonGlobalParameters = cms.PSet(
        centralitySrc = cms.InputTag("pACentrality"),
        centralityVariable = cms.string("HFtowersPlusTrunc"),
        nonDefaultGlauberModel = cms.string("Hijing"),
        pPbRunFlip = cms.untracked.unit32(99999999)
)

process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')

process.load("RecoHI.HiCentralityAlgos.CentralityFilter_cfi")
#process.centralityFilter.selectedBins = [0,1,2,3] # 0 - 10 %
#process.centralityFilter.selectedBins = [4,5,6,7] # 10 - 20 %
#process.centralityFilter.selectedBins = [8,9,10,11,12,13,14,15,16,17,18,19] # 20 - 50 %
#process.centralityFilter.selectedBins = [20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39] # 50 - 100 %
#process.centralityFilter.selectedBins = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39] # MinBias
process.centralityFilter.selectedBins = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99] # 0 - 100 %

process.source = cms.Source("PoolSource", 
#    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
#        "tnp_Regit_Skim.root",
    ),
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1),
)    

TRACK_CUTS = "track.hitPattern.trackerLayersWithMeasurement > 5 && track.normalizedChi2 < 1.8 && track.hitPattern.pixelLayersWithMeasurement > 1 && abs(dB) < 3 && abs(track.dz) < 30"
GLB_CUTS_1 = "isTrackerMuon && muonID('TrackerMuonArbitrated') && muonID('TMOneStationTight')"
GLB_CUTS_2 = "isTrackerMuon"
QUALITY_CUTS_1 =  GLB_CUTS_1 + ' && ' + TRACK_CUTS
QUALITY_CUTS_2 =  GLB_CUTS_2 + ' && ' + TRACK_CUTS

IN_ACCEPTANCE = '[((abs(eta) <= 1.3 && pt > 3.3) || (1.3 < abs(eta) <= 2.2 && p > 2.9) || (2.2 < abs(eta) <= 2.4 && pt > 0.8))]' ## pp

#trigger efficiency probes

#trigger efficiency pairs

#tracking efficiency probes

#tracking efficiency pairs

#MuId efficiency probes

#MuId efficiency pairs

# Make the fit tree and save it in the "MuonID" directory
process.MuonTrgNew2CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTrig2"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    flags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    tagVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    tagFlags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    pairVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    pairFlags = cms.PSet(
        TRK = cms.string("isTrackerMuon"),
    ),
    isMC = cms.bool(True),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuons2MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuons2"),
)

process.MuonTrgNew2etabin1CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTrigetabin1"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    flags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    tagVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    tagFlags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    pairVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    pairFlags = cms.PSet(
        TRK = cms.string("isTrackerMuon"),
    ),
    isMC = cms.bool(True),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsetabin1MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsetabin1"),
)

process.MuonTrgNew2etabin2CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTrigetabin2"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    flags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    tagVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    tagFlags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    pairVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    pairFlags = cms.PSet(
        TRK = cms.string("isTrackerMuon"),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsetabin2MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsetabin2"),
)

process.MuonTrgNew2etabin3CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTrigetabin3"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    flags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    tagVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y   = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    tagFlags = cms.PSet(
        HLTL1HighQ = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMu0_HighQ_v1',1,0).empty()"),
        HLTL1Open = cms.string("!triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v1',1,0).empty()"),
        HLTL2 = cms.string("!triggerObjectMatchesByPath('HLT_PAL2DoubleMu3_v1',1,0).empty()"),
        PAMu3 = cms.string("!triggerObjectMatchesByPath('HLT_PAMu3_v1',1,0).empty()"),
    ),
    pairVariables = cms.PSet(
        pt  = cms.string("pt"),
        phi  = cms.string("phi"),
        eta = cms.string("eta"),
        abseta = cms.string("abs(eta)"),
        y = cms.string("rapidity"),
        absy = cms.string("abs(rapidity)"),
    ),
    pairFlags = cms.PSet(
        TRK = cms.string("isTrackerMuon"),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsetabin3MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsetabin3"),
)

#tracking efficiency fit tree
process.MuonTrk2 = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsSta2"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
      phi = cms.string("phi"),
    ),
    flags = cms.PSet(
#        isGlb = cms.string(QUALITY_CUTS_2),
         isTrk = cms.string(QUALITY_CUTS_2),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    tagFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
       isTrk = cms.string(QUALITY_CUTS_2),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
      isTrk = cms.string(QUALITY_CUTS_2),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsSta2MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsSta2"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonTrketabin1 = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsStaetabin1"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
      phi = cms.string("phi"),
    ),
    flags = cms.PSet(
#        isGlb = cms.string(QUALITY_CUTS_2),
         isTrk = cms.string(QUALITY_CUTS_2),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    tagFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
       isTrk = cms.string(QUALITY_CUTS_2),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
      isTrk = cms.string(QUALITY_CUTS_2),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsStaetabin1MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsStaetabin1"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonTrketabin2 = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsStaetabin2"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
      phi = cms.string("phi"),
    ),
    flags = cms.PSet(
#        isGlb = cms.string(QUALITY_CUTS_2),
         isTrk = cms.string(QUALITY_CUTS_2),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    tagFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
       isTrk = cms.string(QUALITY_CUTS_2),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
      isTrk = cms.string(QUALITY_CUTS_2),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonStaetabin2sMCMatchSta"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsStaetabin2"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonTrketabin3 = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsStaetabin3"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
      phi = cms.string("phi"),
    ),
    flags = cms.PSet(
#        isGlb = cms.string(QUALITY_CUTS_2),
         isTrk = cms.string(QUALITY_CUTS_2),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    tagFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
       isTrk = cms.string(QUALITY_CUTS_2),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
#      isGlb = cms.string(QUALITY_CUTS_2),
      isTrk = cms.string(QUALITY_CUTS_2),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsStaetabin3MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsStaetabin3"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonIDNew2CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTracks"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    flags = cms.PSet(
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
         PassingTrk = cms.string(QUALITY_CUTS_1),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
    ),
    tagFlags = cms.PSet(
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
         PassingTrk = cms.string(QUALITY_CUTS_1),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
#      PassingGlb = cms.string(QUALITY_CUTS_1),
         PassingTrk = cms.string(QUALITY_CUTS_1),
    ),
    isMC = cms.bool(True),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsTrk2MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsTrk2"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonIDNew2etabin1CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTracksetabin1"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    flags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
    ),
    tagFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch")
    probeMatches  = cms.InputTag("probeMuonsTrketabin1MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsTrketabin1"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonIDNew2etabin2CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTracksetabin2"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    flags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
    ),
    tagFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsTrketabin2MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsTrketabin2"),
#    eventWeight=cms.double(15091.8108)
)

process.MuonIDNew2etabin3CS = cms.EDAnalyzer("TagProbeFitTreeProducer",
    tagProbePairs = cms.InputTag("tpPairsTracksetabin3"),
    arbitration   = cms.string("OneProbe"),
    variables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
    ),
    flags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    tagVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
    ),
    tagFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
      #PassingGlb = cms.InputTag("tkPassingGlb"),
    ),
    pairVariables = cms.PSet(
      pt  = cms.string("pt"),
      eta = cms.string("eta"),
      abseta = cms.string("abs(eta)"),
      y = cms.string("rapidity"),
      absy = cms.string("abs(rapidity)"),
    ),  
    pairFlags = cms.PSet(
        PassingTrk = cms.string(QUALITY_CUTS_1),
#      PassingGlb = cms.string(QUALITY_CUTS_1),
    ),
    isMC = cms.bool(False),
    tagMatches = cms.InputTag("tagMuonsSglTrgMCMatch"),
    probeMatches  = cms.InputTag("probeMuonsTrketabin3MCMatch"),
    motherPdgId = cms.int32(23),
    makeMCUnbiasTree = cms.bool(True),
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes     = cms.InputTag("probeMuonsTrketabin3"),
#    eventWeight=cms.double(15091.8108)
)

process.tnpSimpleSequence = cms.Sequence(

         process.MuonTrgNew2CS *
        process.MuonTrgNew2etabin1CS *
        process.MuonTrgNew2etabin2CS *
        process.MuonTrgNew2etabin3CS *

    process.MuonTrk2 *
    process.MuonTrketabin1 *
    process.MuonTrketabin2 *
    process.MuonTrketabin3 *

    process.MuonIDNew2CS *
    process.MuonIDNew2etabin1CS *
    process.MuonIDNew2etabin2CS *
    process.MuonIDNew2etabin3CS
)

process.tagAndProbe = cms.Path(
#    process.centralityFilter *
    process.tnpSimpleSequence
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("tnp_pA_MC_Prod_Merge_for_B_GEN_matching_test.root"))
