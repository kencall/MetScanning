import FWCore.ParameterSet.Config as cms

##____________________________________________________________________________||
process = cms.Process("SKIM")

##____________________________________________________________________________||
process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.load("Configuration.StandardSequences.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")


##____________________________________________________________________________||
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc', '')

##____________________________________________________________________________||
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring("root://eoscms.cern.ch//store/data/Run2015A/ZeroBias1/RECO/PromptReco-v1/000/246/930/00000/2E8CE084-930B-E511-88E0-02163E0145E7.root")
    )

##____________________________________________________________________________||
process.out = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string('skim.root'),
    SelectEvents   = cms.untracked.PSet( SelectEvents = cms.vstring('p') ),
    outputCommands = cms.untracked.vstring(
        'keep *'
        )
    )

##____________________________________________________________________________||
process.options   = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

##____________________________________________________________________________||
process.load('RecoMET.METFilters.CSCTightHaloFilter_cfi')

##____________________________________________________________________________||
process.ApplyBaselineHBHENoiseFilter = cms.EDFilter('BooleanFlagFilter',
    inputLabel = cms.InputTag('HBHENoiseFilterResultProducer','HBHENoiseFilterResult'),
    reverseDecision = cms.bool(False)
)

## select events with high caloMET 
process.caloMETSelector = cms.EDFilter(
    "CandViewSelector",
    src = cms.InputTag("caloMet"),
#    src = cms.InputTag("caloMetM"), #muon corrected
    cut = cms.string( "pt()>150" )
    )

process.caloMETCounter = cms.EDFilter(
    "CandViewCountFilter",
    src = cms.InputTag("caloMETSelector"),
    minNumber = cms.uint32(1),
    )

##____________________________________________________________________________||
process.p = cms.Path(
    process.CSCTightHaloFilter*
    process.ApplyBaselineHBHENoiseFilter*
    process.caloMETSelector*
    process.caloMETCounter
    )

process.e1 = cms.EndPath(
    process.out
    )

##____________________________________________________________________________||
