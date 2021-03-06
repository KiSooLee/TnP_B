# for the list of used tags please see:
# https://twiki.cern.ch/twiki/bin/view/CMS/Onia2MuMuSamples

import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

# set up process
process = cms.Process("Onia2MuMuPAT")

# setup 'analysis'  options
options = VarParsing.VarParsing ('analysis')

# setup any defaults you want
#options.inputFiles = '/store/group/phys_heavyions/dileptons/MC2013/5TeV/Jpsi/MC_Jpsi_FEVTSIM_0.root'
options.outputFile = 'pA_B_MC_TnP_skim_GEN_matching_test.root'

options.maxEvents = -1 # -1 means all events

# get and parse the command line arguments
options.parseArguments()

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'STARTHI53_V25::All' ### B

# Common offline event selection
process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

# pile up rejection
process.load('Appeltel.RpPbAnalysis.PAPileUpVertexFilter_cff')

# Centrality for pPb
process.load('RecoHI.HiCentralityAlgos.HiCentrality_cfi')

# HLT dimuon trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltOniaHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltOniaHI.HLTPaths = ["HLT_PAL1DoubleMuOpen_v*",
                              "HLT_PAL1DoubleMu0_HighQ_v*",
                              "HLT_PAL2DoubleMu3_v*",
                              "HLT_PAMu3_v*",
                              "HLT_PAMu7_v*",
                              "HLT_PAMu12_v*"
                              ]
process.hltOniaHI.throw = False
process.hltOniaHI.andOr = True
process.hltOniaHI.TriggerResultsTag = cms.InputTag("TriggerResults","","HLT")


from HiSkim.HiOnia2MuMu.onia2MuMuPAT_GEN_matching_cff import *
onia2MuMuPAT(process, GlobalTag=process.GlobalTag.globaltag, MC=True, HLT="HLT", Filter=False)

#process.onia2MuMuPatTrkTrk.addMuonlessPrimaryVertex = False
#process.onia2MuMuPatTrkTrk.resolvePileUpAmbiguity = False

from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import changeRecoMuonInput
changeRecoMuonInput(process, "mergedMuons")

process.patMuonSequence = cms.Sequence(
#    process.hltOniaHI *
    process.PAcollisionEventSelection *
#    process.pileupVertexFilterCutGplus * ### exclude this when you analize B
    process.pACentrality_step *
    process.mergedMuons *
    process.genMuons *
    process.patMuonsWithTriggerSequence
    )

process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.source.fileNames = cms.untracked.vstring(
    options.inputFiles
    )

# filter on lumisections
#from HiSkim.HiOnia2MuMu.goodLumiSectionListHI_cfi import *
#process.source.lumisToProcess = goodLumisToProcess

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(options.maxEvents) )
process.outTnP.fileName = cms.untracked.string( options.outputFile )

#process.pACentrality.producePixelhits = cms.bool(False)

process.e = cms.EndPath(process.outTnP)

process.schedule = cms.Schedule(process.TagAndProbeTrig,
                                process.TagAndProbeTrig2,
                                process.TagAndProbeTrigetabin1,
                                process.TagAndProbeTrigetabin2,
                                process.TagAndProbeTrigetabin3,
                                process.TagAndProbeSta,
                                process.TagAndProbeSta2,
                                process.TagAndProbeStaetabin1,
                                process.TagAndProbeStaetabin2,
                                process.TagAndProbeStaetabin3,
                                process.TagAndProbeMuID,
                                process.TagAndProbeMuID2,
                                process.TagAndProbeMuIDetabin1,
                                process.TagAndProbeMuIDetabin2,
                                process.TagAndProbeMuIDetabin3,
                                process.e)


