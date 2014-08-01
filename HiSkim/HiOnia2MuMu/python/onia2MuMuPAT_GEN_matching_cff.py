import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.helpers import *

def onia2MuMuPAT(process, GlobalTag, MC=False, HLT='HLT', Filter=True):
    # Setup the process
    process.options = cms.untracked.PSet(
        wantSummary = cms.untracked.bool(True),
        # fileMode = cms.untracked.string('MERGE'),
    )
    process.load("FWCore.MessageService.MessageLogger_cfi")
    process.MessageLogger.cerr.FwkReport.reportEvery = 100
    process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
    process.load("Configuration.StandardSequences.Reconstruction_cff")
    process.load("Configuration.StandardSequences.MagneticField_cff")
    process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
    process.GlobalTag.globaltag = GlobalTag

    # Drop the DQM stuff on input
    process.source = cms.Source("PoolSource",
        inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*"),
        fileNames = cms.untracked.vstring()
    )

    # Scraping filter
    process.scrapingFilter = cms.EDFilter("FilterOutScraping",
        applyfilter = cms.untracked.bool(True),
        debugOn = cms.untracked.bool(False),
        numtrack = cms.untracked.uint32(10),
        thresh = cms.untracked.double(0.25)
    )

    IN_ACCEPTANCE = '((abs(eta) <= 1.3 && pt > 3.3) || (1.3 < abs(eta) <= 2.2 && p > 2.9) || (2.2 < abs(eta) <= 2.4 && pt > 0.8))'

    # Merge muons, calomuons in a single collection for T&P
    process.mergedMuons = cms.EDProducer("CaloMuonMerger",
        muons     = cms.InputTag("muons"), 
        muonsCut = cms.string(""),
        mergeCaloMuons = cms.bool(True),  ### NEEDED TO RUN ON AOD
        caloMuons = cms.InputTag("calomuons"),
        caloMuonsCut = cms.string(IN_ACCEPTANCE),
        minCaloCompatibility = cms.double(0.6),
        mergeTracks = cms.bool(False),
        tracks = cms.InputTag("generalTracks"),
	tracksCut = cms.string(IN_ACCEPTANCE),
    )


    # Prune generated particles to muons and their parents
    process.genMuons = cms.EDProducer("GenParticlePruner",
        #src = cms.InputTag("genParticles"),
        src = cms.InputTag("hiGenParticles"),
        select = cms.vstring(
            "drop  *  ",                     # this is the default
            "++keep abs(pdgId) = 13",        # keep muons and their parents
            "drop pdgId == 21 && status = 2" # remove intermediate qcd spam carrying no flavour info
        )
    )

    # Make PAT Muons
    process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
    from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import addMCinfo, changeRecoMuonInput, useL1MatchingWindowForSinglets, changeTriggerProcessName, switchOffAmbiguityResolution
    # with some customization
    if MC:
        addMCinfo(process)
        # since we match inner tracks, keep the matching tight and make it one-to-one
        process.muonMatch.maxDeltaR = 0.05
        process.muonMatch.resolveByMatchQuality = True
        process.muonMatch.matched = "genMuons"
    changeTriggerProcessName(process, HLT)
    switchOffAmbiguityResolution(process) # Switch off ambiguity resolution: allow multiple reco muons to match to the same trigger muon
    #useL1MatchingWindowForSinglets(process)

    process.muonL1Info.maxDeltaR = 0.3
    process.muonL1Info.fallbackToME1 = True
    process.muonMatchHLTL1.maxDeltaR = 0.3
    process.muonMatchHLTL1.fallbackToME1 = True
    process.muonMatchHLTL2.maxDeltaR = 0.3
    process.muonMatchHLTL2.maxDPtRel = 10.0
    process.muonMatchHLTL3.maxDeltaR = 0.1
    process.muonMatchHLTL3.maxDPtRel = 10.0
    process.muonMatchHLTCtfTrack.maxDeltaR = 0.1
    process.muonMatchHLTCtfTrack.maxDPtRel = 10.0
    process.muonMatchHLTTrackMu.maxDeltaR = 0.1
    process.muonMatchHLTTrackMu.maxDPtRel = 10.0


    # Make a sequence
    process.patMuonSequence = cms.Sequence(
        process.hltOniaHI *
        process.PAcollisionEventSelection *
        process.genMuons *
        process.patMuonsWithTriggerSequence
    )
    if not MC:
        process.patMuonSequence.remove(process.genMuons)
      
    # Make dimuon candidates
    process.onia2MuMuPatTrkTrk = cms.EDProducer('HiOnia2MuMuPAT',
        muons = cms.InputTag("patMuonsWithTrigger"),
        beamSpotTag = cms.InputTag("offlineBeamSpot"),
        primaryVertexTag = cms.InputTag("offlinePrimaryVertices"),
        # At least one muon must pass this selection
        higherPuritySelection = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        # BOTH muons must pass this selection
        lowerPuritySelection  = cms.string("(isGlobalMuon || isTrackerMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
        dimuonSelection  = cms.string(""), ## The dimuon must pass this selection before vertexing
        addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
        addMuonlessPrimaryVertex = cms.bool(True), ## Embed the primary vertex re-made from all the tracks except the two muons
        addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
        resolvePileUpAmbiguity = cms.bool(True)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
    )

    # check if there is at least one (inclusive) tracker+tracker di-muon
    process.onia2MuMuPatTrkTrkFilter = cms.EDFilter("CandViewCountFilter",
            src = cms.InputTag('onia2MuMuPatTrkTrk'),
            minNumber = cms.uint32(1),
    )

    process.onia2MuMuPatGlbGlb = cms.EDProducer('HiOnia2MuMuPAT',
            muons = cms.InputTag("patMuonsWithTrigger"),
            beamSpotTag = cms.InputTag("offlineBeamSpot"),
            primaryVertexTag = cms.InputTag("hiSelectedVertex"),
            # At least one muon must pass this selection
            higherPuritySelection = cms.string("(isGlobalMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
            # BOTH muons must pass this selection
            lowerPuritySelection  = cms.string("(isGlobalMuon || (innerTrack.isNonnull && genParticleRef(0).isNonnull)) && abs(innerTrack.dxy)<4 && abs(innerTrack.dz)<35"),
            dimuonSelection  = cms.string(""), ## The dimuon must pass this selection before vertexing
            addCommonVertex = cms.bool(True), ## Embed the full reco::Vertex out of the common vertex fit
            addMuonlessPrimaryVertex = cms.bool(True), ## Embed the primary vertex re-made from all the tracks except the two muons
            addMCTruth = cms.bool(MC),      ## Add the common MC mother of the two muons, if any
            resolvePileUpAmbiguity = cms.bool(True)   ## Order PVs by their vicinity to the J/psi vertex, not by sumPt                            
    )

    # check if there is at least one (inclusive) tracker+tracker di-muon
    process.onia2MuMuPatGlbGlbFilter = cms.EDFilter("CandViewCountFilter",
            src = cms.InputTag('onia2MuMuPatGlbGlb'),
            minNumber = cms.uint32(1),
    )

    # the onia2MuMu path
    process.Onia2MuMuPAT = cms.Path(
        process.patMuonSequence *
        process.onia2MuMuPatGlbGlb *
        process.onia2MuMuPatTrkTrk *
        process.onia2MuMuPatTrkTrkFilter *
        process.onia2MuMuPatGlbGlbFilter
    )
    #process.Onia2MuMuPAT = cms.Path(process.patMuonSequence)

    # Make Tag and Probe pairs for efficiency measurements
    TRACK_CUTS = "(track.hitPattern.trackerLayersWithMeasurement > 5 && track.normalizedChi2 < 1.8 && track.hitPattern.pixelLayersWithMeasurement > 1 && abs(dB) < 3 && abs(track.dz) < 30)"
    GLB_CUTS = "(isTrackerMuon && muonID('TrackerMuonArbitrated') && muonID('TMOneStationTight'))"
    QUALITY_CUTS =  "(" + GLB_CUTS + ' && ' + TRACK_CUTS + ")"

    process.tagMuonsSglTrg = cms.EDFilter("PATMuonSelector",
                                       src = cms.InputTag("patMuonsWithTrigger"),
                                       cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE + " && (!triggerObjectMatchesByPath('HLT_PAMu3_v*',1,0).empty() || !triggerObjectMatchesByPath('HLT_PAMu7_v*',1,0).empty() || !triggerObjectMatchesByPath('HLT_PAMu12_v*',1,0).empty())")
                                       )

    process.tagMuonsDblTrg = cms.EDFilter("PATMuonSelector",
                                       src = cms.InputTag("patMuonsWithTrigger"),
                                       cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE + " && !triggerObjectMatchesByPath('HLT_PAL1DoubleMuOpen_v*',1,0).empty()")
                                       )

    # produce patMuons that use the STA momentum information
    process.patMuonsWithTriggerSta = cms.EDProducer("RedefineMuonP4FromTrackPAT",
                                                    src   = cms.InputTag("patMuonsWithTrigger"),
                                                    track = cms.string("outer")
                                                    )

    # must be STA, so we can measure inner tracking efficiency
    ### probes for inner tracking efficiency
    process.probeMuonsSta = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTriggerSta"),
                                         cut = cms.string("outerTrack.isNonnull")
                                         )

    process.probeMuonsSta2 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTriggerSta"),
                                         cut = cms.string("outerTrack.isNonnull"+"&&"+"-2.4 < eta && eta < 2.4")
                                         )

    process.probeMuonsStaetabin1 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTriggerSta"),
                                         cut = cms.string("outerTrack.isNonnull"+"&&"+"-2.4 < eta && eta < -0.8")
                                         )

    process.probeMuonsStaetabin2 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTriggerSta"),
                                         cut = cms.string("outerTrack.isNonnull"+"&&"+"-0.8 < eta && eta < 0.8")
                                         )

    process.probeMuonsStaetabin3 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTriggerSta"),
                                         cut = cms.string("outerTrack.isNonnull"+"&&"+"0.8 < eta && eta < 2.4")
                                         )

    ### pairs for inner tracking efficiency
    process.tpPairsSta = cms.EDProducer("CandViewShallowCloneCombiner",
                                        cut = cms.string('2.0 < mass < 5.0'),
                                        decay = cms.string('tagMuonsSglTrg@+ probeMuonsSta@-')
                                        )

    process.tpPairsSta2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                        cut = cms.string('2.0 < mass < 5.0'),
                                        decay = cms.string('tagMuonsSglTrg@+ probeMuonsSta2@-')
                                        )

    process.tpPairsStaetabin1 = cms.EDProducer("CandViewShallowCloneCombiner",
                                        cut = cms.string('2.0 < mass < 5.0'),
                                        decay = cms.string('tagMuonsSglTrg@+ probeMuonsStaetabin1@-')
                                        )

    process.tpPairsStaetabin2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                        cut = cms.string('2.0 < mass < 5.0'),
                                        decay = cms.string('tagMuonsSglTrg@+ probeMuonsStaetabin2@-')
                                        )

    process.tpPairsStaetabin3 = cms.EDProducer("CandViewShallowCloneCombiner",
                                        cut = cms.string('2.0 < mass < 5.0'),
                                        decay = cms.string('tagMuonsSglTrg@+ probeMuonsStaetabin3@-')
                                        )

    # must be a GLB to measure trigger efficiency
    ### probe muons for trigger efficiency
    process.probeMuons = cms.EDFilter("PATMuonSelector",
                                      src = cms.InputTag("patMuonsWithTrigger"),
                                      cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE)
                                      )

    process.probeMuons2 = cms.EDFilter("PATMuonSelector",
                                      src = cms.InputTag("patMuonsWithTrigger"),
                                      cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE+'&&'+'-2.4 < eta && eta < 2.4')
                                      )

    process.probeMuonsetabin1 = cms.EDFilter("PATMuonSelector",
                                      src = cms.InputTag("patMuonsWithTrigger"),
                                      cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE+'&&'+'-2.4 < eta && eta < -0.8')
                                      )

    process.probeMuonsetabin2 = cms.EDFilter("PATMuonSelector",
                                      src = cms.InputTag("patMuonsWithTrigger"),
                                      cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE+'&&'+'-0.8 < eta && eta < 0.8')
                                      )

    process.probeMuonsetabin3 = cms.EDFilter("PATMuonSelector",
                                      src = cms.InputTag("patMuonsWithTrigger"),
                                      cut = cms.string(QUALITY_CUTS + ' && ' + IN_ACCEPTANCE+'&&'+'0.8 < eta && eta < 2.4')
                                      )

    ### pairs for trigger efficiency
    process.tpPairsTrig = cms.EDProducer("CandViewShallowCloneCombiner",
                                     cut = cms.string('2.6 < mass < 4.0'),
                                     decay = cms.string('tagMuonsSglTrg@+ probeMuons@-')
                                     )

    process.tpPairsTrig2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                     cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                     decay = cms.string('tagMuonsSglTrg@+ probeMuons2@-')
                                     )

    process.tpPairsTrigetabin1 = cms.EDProducer("CandViewShallowCloneCombiner",
                                     cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                     decay = cms.string('tagMuonsSglTrg@+ probeMuonsetabin1@-')
                                     )

    process.tpPairsTrigetabin2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                     cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                     decay = cms.string('tagMuonsSglTrg@+ probeMuonsetabin2@-')
                                     )

    process.tpPairsTrigetabin3 = cms.EDProducer("CandViewShallowCloneCombiner",
                                     cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                     decay = cms.string('tagMuonsSglTrg@+ probeMuonsetabin3@-')
                                     )

    # must be tracker muon, so we can measure the muon reco efficiency
    # since we're using tracker muons this year, we should use calo muons as probes here
    ### probe muons for muon ID efficiency
    process.probeMuonsTrk = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTrigger"),
                                         cut = cms.string("isCaloMuon && " + TRACK_CUTS)
                                         )

    process.probeMuonsTrk2 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTrigger"),
                                         cut = cms.string("isCaloMuon && " + TRACK_CUTS + "&&" +"-2.4 < eta && eta < 2.4")
                                         )

    process.probeMuonsTrketabin1 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTrigger"),
                                         cut = cms.string("isCaloMuon && " + TRACK_CUTS + "&&" +"-2.4 < eta && eta < -0.8")
                                         )

    process.probeMuonsTrketabin2 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTrigger"),
                                         cut = cms.string("isCaloMuon && " + TRACK_CUTS + "&&" +"-0.8 < eta && eta < 0.8")
                                         )

    process.probeMuonsTrketabin3 = cms.EDFilter("PATMuonSelector",
                                         src = cms.InputTag("patMuonsWithTrigger"),
                                         cut = cms.string("isCaloMuon && " + TRACK_CUTS + "&&" +"0.8 < eta && eta < 2.4")
                                         )

    ### pairs for muon ID efficiency
    process.tpPairsTracks = cms.EDProducer("CandViewShallowCloneCombiner",
                                           cut = cms.string('2.6 < mass < 4.0'),
                                           decay = cms.string('tagMuonsSglTrg@+ probeMuonsTrk@-')
                                           )

    process.tpPairsTracks2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                           cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                           decay = cms.string('tagMuonsSglTrg@+ probeMuonsTrk2@-')
                                           )

    process.tpPairsTracksetabin1 = cms.EDProducer("CandViewShallowCloneCombiner",
                                           cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                           decay = cms.string('tagMuonsSglTrg@+ probeMuonsTrketabin1@-')
                                           )

    process.tpPairsTracksetabin2 = cms.EDProducer("CandViewShallowCloneCombiner",
                                           cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                           decay = cms.string('tagMuonsSglTrg@+ probeMuonsTrketabin2@-')
                                           )

    process.tpPairsTracksetabin3 = cms.EDProducer("CandViewShallowCloneCombiner",
                                           cut = cms.string('2.6 < mass < 4.0'+'&&'+'pt > 3.0 && -2.4 < y && y < 2.4'),
                                           decay = cms.string('tagMuonsSglTrg@+ probeMuonsTrketabin3@-')
                                           )

    # check if there is at least one Tag and Probe pair
    ### filter for inner tracking efficiency
    process.tpPairsStaFilter = cms.EDFilter("CandViewCountFilter",
                                            src = cms.InputTag('tpPairsSta'),
                                            minNumber = cms.uint32(1),
                                            )

    process.tpPairsSta2Filter = cms.EDFilter("CandViewCountFilter",
                                            src = cms.InputTag('tpPairsSta2'),
                                            minNumber = cms.uint32(1),
                                            )

    process.tpPairsStaetabin1Filter = cms.EDFilter("CandViewCountFilter",
                                            src = cms.InputTag('tpPairsStaetabin1'),
                                            minNumber = cms.uint32(1),
                                            )

    process.tpPairsStaetabin2Filter = cms.EDFilter("CandViewCountFilter",
                                            src = cms.InputTag('tpPairsStaetabin2'),
                                            minNumber = cms.uint32(1),
                                            )

    process.tpPairsStaetabin3Filter = cms.EDFilter("CandViewCountFilter",
                                            src = cms.InputTag('tpPairsStaetabin3'),
                                            minNumber = cms.uint32(1),
                                            )

    ### filter for trigger efficiency
    process.tpPairsTrigFilter = cms.EDFilter("CandViewCountFilter",
                                             src = cms.InputTag('tpPairsTrig'),
                                             minNumber = cms.uint32(1),
                                             )

    process.tpPairsTrig2Filter = cms.EDFilter("CandViewCountFilter",
                                             src = cms.InputTag('tpPairsTrig2'),
                                             minNumber = cms.uint32(1),
                                             )

    process.tpPairsTrigetabin1Filter = cms.EDFilter("CandViewCountFilter",
                                             src = cms.InputTag('tpPairsTrigetabin1'),
                                             minNumber = cms.uint32(1),
                                             )

    process.tpPairsTrigetabin2Filter = cms.EDFilter("CandViewCountFilter",
                                             src = cms.InputTag('tpPairsTrigetabin2'),
                                             minNumber = cms.uint32(1),
                                             )

    process.tpPairsTrigetabin3Filter = cms.EDFilter("CandViewCountFilter",
                                             src = cms.InputTag('tpPairsTrigetabin3'),
                                             minNumber = cms.uint32(1),
                                             )

    ### filter for Muon ID efficiency
    process.tpPairsTracksFilter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag('tpPairsTracks'),
                                               minNumber = cms.uint32(1),
                                               )
    
    process.tpPairsTracks2Filter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag('tpPairsTracks2'),
                                               minNumber = cms.uint32(1),
                                               )
    
    process.tpPairsTracksetabin1Filter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag('tpPairsTracksetabin1'),
                                               minNumber = cms.uint32(1),
                                               )
    
    process.tpPairsTracksetabin2Filter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag('tpPairsTracksetabin2'),
                                               minNumber = cms.uint32(1),
                                               )
    
    process.tpPairsTracksetabin3Filter = cms.EDFilter("CandViewCountFilter",
                                               src = cms.InputTag('tpPairsTracksetabin3'),
                                               minNumber = cms.uint32(1),
                                               )
    
    # the Tag and Probe path
    ### path for inner tracking efficiency
    process.tnpSta = cms.Sequence(
        process.probeMuonsSta *
        process.tpPairsSta *
        process.tpPairsStaFilter
        )

    process.tnpSta2 = cms.Sequence(
        process.probeMuonsSta2 *
        process.tpPairsSta2 *
        process.tpPairsSta2Filter
        )

    process.tnpStaetabin1 = cms.Sequence(
        process.probeMuonsStaetabin1 *
        process.tpPairsStaetabin1 *
        process.tpPairsStaetabin1Filter
        )

    process.tnpStaetabin2 = cms.Sequence(
        process.probeMuonsStaetabin2 *
        process.tpPairsStaetabin2 *
        process.tpPairsStaetabin2Filter
        )

    process.tnpStaetabin3 = cms.Sequence(
        process.probeMuonsStaetabin3 *
        process.tpPairsStaetabin3 *
        process.tpPairsStaetabin3Filter
        )

    ### path for trigger efficiency
    process.tnpTrig = cms.Sequence(
        process.probeMuons *
        process.tpPairsTrig *
        process.tpPairsTrigFilter
        )

    process.tnpTrig2 = cms.Sequence(
        process.probeMuons2 *
        process.tpPairsTrig2 *
        process.tpPairsTrig2Filter
        )

    process.tnpTrigetabin1 = cms.Sequence(
        process.probeMuonsetabin1 *
        process.tpPairsTrigetabin1 *
        process.tpPairsTrigetabin1Filter
        )

    process.tnpTrigetabin2 = cms.Sequence(
        process.probeMuonsetabin2 *
        process.tpPairsTrigetabin2 *
        process.tpPairsTrigetabin2Filter
        )

    process.tnpTrigetabin3 = cms.Sequence(
        process.probeMuonsetabin3 *
        process.tpPairsTrigetabin3 *
        process.tpPairsTrigetabin3Filter
        )

    ### path for Muon ID efficiency
    process.tnpMuID = cms.Sequence(
        process.probeMuonsTrk *
        process.tpPairsTracks *
        process.tpPairsTracksFilter
        )

    process.tnpMuID2 = cms.Sequence(
        process.probeMuonsTrk2 *
        process.tpPairsTracks2 *
        process.tpPairsTracks2Filter
        )

    process.tnpMuIDetabin1 = cms.Sequence(
        process.probeMuonsTrketabin1 *
        process.tpPairsTracksetabin1 *
        process.tpPairsTracksetabin1Filter
        )

    process.tnpMuIDetabin2 = cms.Sequence(
        process.probeMuonsTrketabin2 *
        process.tpPairsTracksetabin2 *
        process.tpPairsTracksetabin2Filter
        )

    process.tnpMuIDetabin3 = cms.Sequence(
        process.probeMuonsTrketabin3 *
        process.tpPairsTracksetabin3 *
        process.tpPairsTracksetabin3Filter
        )

    # inner track reco efficiency
    process.TagAndProbeSta = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.patMuonsWithTriggerSta *
        process.tnpSta
    )

    process.TagAndProbeSta2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.patMuonsWithTriggerSta *
        process.tnpSta2
    )

    process.TagAndProbeStaetabin1 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.patMuonsWithTriggerSta *
        process.tnpStaetabin1
    )

    process.TagAndProbeStaetabin2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.patMuonsWithTriggerSta *
        process.tnpStaetabin2
    )

    process.TagAndProbeStaetabin3 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.patMuonsWithTriggerSta *
        process.tnpStaetabin3
    )

    # muon reco and ID efficiency
    process.TagAndProbeMuID = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpMuID
    )

    process.TagAndProbeMuID2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpMuID2
    )

    process.TagAndProbeMuIDetabin1 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpMuIDetabin1
    )

    process.TagAndProbeMuIDetabin2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpMuIDetabin2
    )

    process.TagAndProbeMuIDetabin3 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpMuIDetabin3
    )

    # muon trigger efficiency
    process.TagAndProbeTrig = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpTrig
    )
    
    process.TagAndProbeTrig2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpTrig2
    )
    
    process.TagAndProbeTrigetabin1 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpTrigetabin1
    )
    
    process.TagAndProbeTrigetabin2 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpTrigetabin2
    )
    
    process.TagAndProbeTrigetabin3 = cms.Path(
        process.patMuonSequence *
        process.tagMuonsSglTrg *
        process.tnpTrigetabin3
    )
    
    if MC:
        process.tagMuonsDblTrgMCMatch = process.muonMatch.clone(src = "tagMuonsDblTrg")
        process.tagMuonsSglTrgMCMatch = process.muonMatch.clone(src = "tagMuonsSglTrg")

        process.probeMuonsStaMCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsSta") # inner tracking eff
        process.probeMuonsSta2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsSta2") # inner tracking eff
        process.probeMuonsStaetabin1MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsStaetabin1") # inner tracking eff
        process.probeMuonsStaetabin2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsStaetabin2") # inner tracking eff
        process.probeMuonsStaetabin3MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsStaetabin3") # inner tracking eff

        process.probeMuonsTrkMCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsTrk") # Muon reco and ID eff
        process.probeMuonsTrk2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsTrk2") # Muon reco and ID eff
        process.probeMuonsTrketabin1MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsTrketabin1") # Muon reco and ID eff
        process.probeMuonsTrketabin2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsTrketabin2") # Muon reco and ID eff
        process.probeMuonsTrketabin3MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsTrketabin3") # Muon reco and ID eff

        process.probeMuonsMCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuons") # muon trigger eff
        process.probeMuons2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuons2") # muon trigger eff
        process.probeMuonsetabin1MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsetabin1") # muon trigger eff
        process.probeMuonsetabin2MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsetabin2") # muon trigger eff
        process.probeMuonsetabin3MCMatch = process.tagMuonsSglTrgMCMatch.clone(src = "probeMuonsetabin3") # muon trigger eff

        process.TagAndProbeSta.replace(process.tpPairsSta, process.tagMuonsSglTrgMCMatch * process.probeMuonsStaMCMatch * process.tpPairsSta)
        process.TagAndProbeSta2.replace(process.tpPairsSta2, process.tagMuonsSglTrgMCMatch * process.probeMuonsSta2MCMatch * process.tpPairsSta2)
        process.TagAndProbeStaetabin1.replace(process.tpPairsStaetabin1, process.tagMuonsSglTrgMCMatch * process.probeMuonsStaetabin1MCMatch * process.tpPairsStaetabin1)
        process.TagAndProbeStaetabin2.replace(process.tpPairsStaetabin2, process.tagMuonsSglTrgMCMatch * process.probeMuonsStaetabin2MCMatch * process.tpPairsStaetabin2)
        process.TagAndProbeStaetabin3.replace(process.tpPairsStaetabin3, process.tagMuonsSglTrgMCMatch * process.probeMuonsStaetabin3MCMatch * process.tpPairsStaetabin3)

        process.TagAndProbeMuID.replace(process.tpPairsTracks, process.tagMuonsSglTrgMCMatch * process.probeMuonsTrkMCMatch * process.tpPairsTracks)
        process.TagAndProbeMuID2.replace(process.tpPairsTracks2, process.tagMuonsSglTrgMCMatch * process.probeMuonsTrk2MCMatch * process.tpPairsTracks2)
        process.TagAndProbeMuIDetabin1.replace(process.tpPairsTracksetabin1, process.tagMuonsSglTrgMCMatch * process.probeMuonsTrketabin1MCMatch * process.tpPairsTracksetabin1)
        process.TagAndProbeMuIDetabin2.replace(process.tpPairsTracksetabin2, process.tagMuonsSglTrgMCMatch * process.probeMuonsTrketabin2MCMatch * process.tpPairsTracksetabin2)
        process.TagAndProbeMuIDetabin3.replace(process.tpPairsTracksetabin3, process.tagMuonsSglTrgMCMatch * process.probeMuonsTrketabin3MCMatch * process.tpPairsTracksetabin3)

        process.TagAndProbeTrig.replace(process.tpPairsTrig, process.tagMuonsSglTrgMCMatch * process.probeMuonsMCMatch * process.tpPairsTrig)
        process.TagAndProbeTrig2.replace(process.tpPairsTrig2, process.tagMuonsSglTrgMCMatch * process.probeMuons2MCMatch * process.tpPairsTrig2)
        process.TagAndProbeTrigetabin1.replace(process.tpPairsTrigetabin1, process.tagMuonsSglTrgMCMatch * process.probeMuonsetabin1MCMatch * process.tpPairsTrigetabin1)
        process.TagAndProbeTrigetabin2.replace(process.tpPairsTrigetabin2, process.tagMuonsSglTrgMCMatch * process.probeMuonsetabin2MCMatch * process.tpPairsTrigetabin2)
        process.TagAndProbeTrigetabin3.replace(process.tpPairsTrigetabin3, process.tagMuonsSglTrgMCMatch * process.probeMuonsetabin3MCMatch * process.tpPairsTrigetabin3)
    
    # output
    process.load('Configuration.EventContent.EventContent_cff')
    
    process.outOnia2MuMu = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('onia2MuMuPAT.root'),
        outputCommands =  cms.untracked.vstring('drop *'),
                                                
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('Onia2MuMuPAT') ) if Filter else cms.untracked.PSet()
    )

    process.outOnia2MuMu.outputCommands.extend(cms.untracked.vstring(
        'keep *_mergedtruth_*_*',                              # tracking particles and tracking vertices for hit by hit matching
        'keep *_genParticles_*_*',                             # generated particles
        'keep *_genMuons_*_Onia2MuMuPAT',                      # generated muons and parents
        'keep patMuons_patMuonsWithTrigger_*_Onia2MuMuPAT',    # All PAT muons including matches to triggers
        'keep patCompositeCandidates_*__Onia2MuMuPAT',         # PAT di-muons
        'keep *_offlinePrimaryVertices_*_*',                   # Primary vertices: you want these to compute impact parameters
        'keep *_offlineBeamSpot_*_*',                          # Beam spot: you want this for the same reason                                   
        'keep edmTriggerResults_TriggerResults_*_*',           # HLT info, per path (cheap)
        'keep l1extraL1MuonParticles_hltL1extraParticles_*_*', # L1 info (cheap)
        'keep l1extraL1MuonParticles_l1extraParticles_*_*',    # L1 info (cheap)
        'keep *_offlinePrimaryVertices_*_*',
        'keep *_pACentrality_*_*',
        'keep *_generalTracks_*_*',
        'keep *_standAloneMuons_*_*')
                            ),

    process.outTnP = cms.OutputModule("PoolOutputModule",
        fileName = cms.untracked.string('tnp.root'),
        outputCommands = cms.untracked.vstring('drop *',
            #'keep *_genParticles_*_*',                             # generated particles
            'keep *_hiGenParticles_*_*',                            # embedded particles
            'keep *_genMuons_*_Onia2MuMuPAT',                      # generated muons and parents
            'keep *_tagMuonsDblTrgMCMatch__Onia2MuMuPAT',                # tagMuons MC matches for efficiency
            'keep *_tagMuonsSglTrgMCMatch__Onia2MuMuPAT',                # tagMuons MC matches for efficiency

            'keep *_probeMuonsMCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuons2MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsetabin1MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsetabin2MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsetabin3MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency

            'keep *_probeMuonsStaMCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsSta2MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsStaetabin1MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsStaetabin2MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency
            'keep *_probeMuonsStaetabin3MCMatch__Onia2MuMuPAT',              # probeMuons MC matches for efficiency

            'keep *_probeMuonsTrkMCMatch__Onia2MuMuPAT',              # probeTacks MC matches for efficiency
            'keep *_probeMuonsTrk2MCMatch__Onia2MuMuPAT',              # probeTacks MC matches for efficiency
            'keep *_probeMuonsTrketabin1MCMatch__Onia2MuMuPAT',              # probeTacks MC matches for efficiency
            'keep *_probeMuonsTrketabin2MCMatch__Onia2MuMuPAT',              # probeTacks MC matches for efficiency
            'keep *_probeMuonsTrketabin3MCMatch__Onia2MuMuPAT',              # probeTacks MC matches for efficiency

            'keep patMuons_patMuonsWithTriggerSta__Onia2MuMuPAT',    # All PAT muons including matches to triggers
            'keep patMuons_tagMuonsDblTrg__Onia2MuMuPAT',                # tagMuons for efficiency
            'keep patMuons_tagMuonsSglTrg__Onia2MuMuPAT',                # tagMuons for efficiency

            'keep patMuons_probeMuonsSta__Onia2MuMuPAT',           # probeMuons for efficiency
            'keep patMuons_probeMuonsSta2__Onia2MuMuPAT',           # probeMuons for efficiency
            'keep patMuons_probeMuonsStaetabin1__Onia2MuMuPAT',           # probeMuons for efficiency
            'keep patMuons_probeMuonsStaetabin2__Onia2MuMuPAT',           # probeMuons for efficiency
            'keep patMuons_probeMuonsStaetabin3__Onia2MuMuPAT',           # probeMuons for efficiency

            'keep patMuons_probeMuonsTrk__Onia2MuMuPAT',                    # probeTracks for efficiency
            'keep patMuons_probeMuonsTrk2__Onia2MuMuPAT',                    # probeTracks for efficiency
            'keep patMuons_probeMuonsTrketabin1__Onia2MuMuPAT',                    # probeTracks for efficiency
            'keep patMuons_probeMuonsTrketabin2__Onia2MuMuPAT',                    # probeTracks for efficiency
            'keep patMuons_probeMuonsTrketabin3__Onia2MuMuPAT',                    # probeTracks for efficiency

            'keep patMuons_probeMuons__Onia2MuMuPAT',              # probeMuons for efficiency
            'keep patMuons_probeMuons2__Onia2MuMuPAT',              # probeMuons for efficiency
            'keep patMuons_probeMuonsetabin1__Onia2MuMuPAT',              # probeMuons for efficiency
            'keep patMuons_probeMuonsetabin2__Onia2MuMuPAT',              # probeMuons for efficiency
            'keep patMuons_probeMuonsetabin3__Onia2MuMuPAT',              # probeMuons for efficiency

            'keep recoCompositeCandidates_tpPairsSta__Onia2MuMuPAT',        # RECO di-muons, tpPairs for tracking efficiency
            'keep recoCompositeCandidates_tpPairsSta2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for tracking efficiency
            'keep recoCompositeCandidates_tpPairsStaetabin1__Onia2MuMuPAT',        # RECO di-muons, tpPairs for tracking efficiency
            'keep recoCompositeCandidates_tpPairsStaetabin2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for tracking efficiency
            'keep recoCompositeCandidates_tpPairsStaetabin3__Onia2MuMuPAT',        # RECO di-muons, tpPairs for tracking efficiency

            'keep recoCompositeCandidates_tpPairsTracks__Onia2MuMuPAT',        # RECO di-muons, tpPairs for muon ID and reco efficiency
            'keep recoCompositeCandidates_tpPairsTracks2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for muon ID and reco efficiency
            'keep recoCompositeCandidates_tpPairsTracksetabin1__Onia2MuMuPAT',        # RECO di-muons, tpPairs for muon ID and reco efficiency
            'keep recoCompositeCandidates_tpPairsTracksetabin2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for muon ID and reco efficiency
            'keep recoCompositeCandidates_tpPairsTracksetabin3__Onia2MuMuPAT',        # RECO di-muons, tpPairs for muon ID and reco efficiency

            'keep recoCompositeCandidates_tpPairsTrig__Onia2MuMuPAT',        # RECO di-muons, tpPairs for trigger efficiency
            'keep recoCompositeCandidates_tpPairsTrig2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for trigger efficiency
            'keep recoCompositeCandidates_tpPairsTrigetabin1__Onia2MuMuPAT',        # RECO di-muons, tpPairs for trigger efficiency
            'keep recoCompositeCandidates_tpPairsTrigetabin2__Onia2MuMuPAT',        # RECO di-muons, tpPairs for trigger efficiency
            'keep recoCompositeCandidates_tpPairsTrigetabin3__Onia2MuMuPAT',        # RECO di-muons, tpPairs for trigger efficiency

            'keep *_offlinePrimaryVertices_*_*',                   # Primary vertices: you want these to compute impact parameters
            'keep *_offlineBeamSpot_*_*',                          # Beam spot: you want this for the same reason   
            'keep edmTriggerResults_TriggerResults_*_*',           # HLT info, per path (cheap)
            'keep l1extraL1MuonParticles_hltL1extraParticles_*_*', # L1 info (cheap)
            'keep l1extraL1MuonParticles_l1extraParticles_*_*',    # L1 info (cheap)
            'keep *_pACentrality_*_*',
            'keep *_generalTracks_*_*',
            'keep *_standAloneMuons_*_*'                           # standAloneMuon track collection, to be on the safe side
        ),
        SelectEvents = cms.untracked.PSet( SelectEvents = cms.vstring('TagAndProbeSta','TagAndProbeSta2','TagAndProbeStaetabin1','TagAndProbeStaetabin2','TagAndProbeStaetabin3','TagAndProbeMuID','TagAndProbeMuID2','TagAndProbeMuIDetabin1','TagAndProbeMuIDetabin2','TagAndProbeMuIDetabin3','TagAndProbeTrig','TagAndProbeTrig2','TagAndProbeTrigetabin1','TagAndProbeTrigetabin2','TagAndProbeTrigetabin3') ) if Filter else cms.untracked.PSet()
    )

    process.e = cms.EndPath(process.outOnia2MuMu + process.outTnP)

