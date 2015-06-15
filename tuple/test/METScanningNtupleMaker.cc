#include "MetScanning/tuple/test/METScanningNtupleMaker.h"


METScanningNtupleMaker::METScanningNtupleMaker(const edm::ParameterSet& iConfig) {
  

  //the input tags
   inputTagCaloMET_ = iConfig.getParameter<edm::InputTag>("caloMET");
   inputTagPFCaloMET_ = iConfig.getParameter<edm::InputTag>("pfCaloMET");
   inputTagPFClusterMET_ = iConfig.getParameter<edm::InputTag>("pfClusterMET");
   inputTagEcalPFClusters_ = iConfig.getParameter<edm::InputTag>("EcalPFClusterCollection");
   inputTagHBHEPFClusters_ = iConfig.getParameter<edm::InputTag>("HBHEPFClusterCollection");
   inputTagHOPFClusters_ = iConfig.getParameter<edm::InputTag>("HOPFClusterCollection");
   inputTagHFPFClusters_ = iConfig.getParameter<edm::InputTag>("HFPFClusterCollection");
   inputTagTracks_ = iConfig.getParameter<edm::InputTag>("tracksCollection");
   inputTagCSC_ = iConfig.getParameter<edm::InputTag>("CSCfilter");
   inputTagHBHER1_ = iConfig.getParameter<edm::InputTag>("HBHEfilterR1");
   inputTagHBHER2L_ = iConfig.getParameter<edm::InputTag>("HBHEfilterR2L");
   inputTagHBHER2T_ = iConfig.getParameter<edm::InputTag>("HBHEfilterR2T");


  // The root tuple
  outputfile_ = iConfig.getParameter<std::string>("rootOutputFile"); 
  tf1 = new TFile(outputfile_.c_str(), "RECREATE");  
  s = new TTree("tree","tree");




  //basic informations ==========================
  s->Branch("run",&run,"run/l");  
  s->Branch("lumi",&lumiBlock,"lumi/l");  
  s->Branch("event",&event,"event/l");  
  s->Branch("time",&time,"time/l");

  s->Branch("filter_csc",&filtercsc,"filter_csc/O");
  s->Branch("filter_hbher1",&filterhbher1,"filter_hbher1/O");
  s->Branch("filter_hbher2l",&filterhbher2l,"filter_hbher2l/O");
  s->Branch("filter_hbher2t",&filterhbher2t,"filter_hbher2t/O");

  //METs ========================================
  s->Branch("caloMETPt",&caloMETPt,"caloMETPt/F");  
  s->Branch("caloMETPhi",&caloMETPhi,"caloMETPhi/F"); 
  s->Branch("caloMETSumEt",&caloMETSumEt,"caloMETSumEt/F");  

  s->Branch("pfCaloMETPt",&pfCaloMETPt,"pfCaloMETPt/F");  
  s->Branch("pfCaloMETPhi",&pfCaloMETPhi,"pfCaloMETPhi/F"); 
  s->Branch("pfCaloMETSumEt",&pfCaloMETSumEt,"pfCaloMETSumEt/F");  

  s->Branch("pfClusterMETPt",&pfClusterMETPt,"pfClusterMETPt/F");  
  s->Branch("pfClusterMETPhi",&pfClusterMETPhi,"pfClusterMETPhi/F"); 
  s->Branch("pfClusterMETSumEt",&pfClusterMETSumEt,"pfClusterMETSumEt/F"); 
  
  //clusters ==========================================
  s->Branch("pfClusterEcal_energy",&pfClusterEcal_energy);
  s->Branch("pfClusterEcal_eta",&pfClusterEcal_eta);
  s->Branch("pfClusterEcal_phi",&pfClusterEcal_phi);

  s->Branch("pfClusterHBHE_energy",&pfClusterHBHE_energy);
  s->Branch("pfClusterHBHE_eta",&pfClusterHBHE_eta);
  s->Branch("pfClusterHBHE_phi",&pfClusterHBHE_phi);

  s->Branch("pfClusterHO_energy",&pfClusterHO_energy);
  s->Branch("pfClusterHO_eta",&pfClusterHO_eta);
  s->Branch("pfClusterHO_phi",&pfClusterHO_phi);

  s->Branch("pfClusterHF_energy",&pfClusterHF_energy);
  s->Branch("pfClusterHF_eta",&pfClusterHF_eta);
  s->Branch("pfClusterHF_phi",&pfClusterHF_phi);
  
  //tracks ============================================
  s->Branch("track_pt",&track_pt);
  s->Branch("track_eta",&track_eta);
  s->Branch("track_phi",&track_phi);
  

}



METScanningNtupleMaker::~METScanningNtupleMaker() { 
  
  tf1->cd();
  s->Write();
  tf1->Write();
  tf1->Close();  
}



void 
METScanningNtupleMaker::beginRun(const edm::Run& run, 
				  const edm::EventSetup & es) { }


void 
METScanningNtupleMaker::analyze(const Event& iEvent, 
				 const EventSetup& iSetup) {
  
  irun  = iEvent.id().run();
  ievent  = iEvent.id().event();
  ilumiBlock = iEvent.id().luminosityBlock();
  itime = iEvent.time();

  run = (size_t)irun;
  event = (size_t)ievent;
  lumiBlock = (size_t)ilumiBlock;
  time = (size_t)((iEvent.time().value())>>32);

  
  //get filters
  Handle<bool> ifiltercsc;
  iEvent.getByLabel(inputTagCSC_, ifiltercsc);
  filtercsc = *ifiltercsc;

  Handle<bool> ifilterhbher1;
  iEvent.getByLabel(inputTagHBHER1_, ifilterhbher1);
  filterhbher1 = *ifilterhbher1;

  Handle<bool> ifilterhbher2l;
  iEvent.getByLabel(inputTagHBHER2L_, ifilterhbher2l);
  filterhbher2l = *ifilterhbher2l;

  Handle<bool> ifilterhbher2t;
  iEvent.getByLabel(inputTagHBHER2T_, ifilterhbher2t);
  filterhbher2t = *ifilterhbher2t;

  // get METs
  Handle<reco::CaloMETCollection> caloMET;
  iEvent.getByLabel(inputTagCaloMET_, caloMET);

  Handle<reco::PFMETCollection> pfCaloMET;
  iEvent.getByLabel(inputTagPFCaloMET_, pfCaloMET);

  Handle<reco::PFClusterMETCollection> pfClusterMET;
  iEvent.getByLabel(inputTagPFClusterMET_, pfClusterMET);
  
  //get PFClusters
  Handle<reco::PFClusterCollection> pfClustersEcal;
  iEvent.getByLabel(inputTagEcalPFClusters_,pfClustersEcal);

  Handle<reco::PFClusterCollection> pfClustersHBHE;
  iEvent.getByLabel(inputTagHBHEPFClusters_,pfClustersHBHE);

  Handle<reco::PFClusterCollection> pfClustersHO;
  iEvent.getByLabel(inputTagHOPFClusters_,pfClustersHO);

  Handle<reco::PFClusterCollection> pfClustersHF;
  iEvent.getByLabel(inputTagHFPFClusters_,pfClustersHF);

  //get tracks
  Handle<reco::TrackCollection> tracks;
  iEvent.getByLabel(inputTagTracks_,tracks);


  
  //clear vectors
  pfClusterEcal_energy.clear();
  pfClusterEcal_eta.clear();
  pfClusterEcal_phi.clear();

  pfClusterHBHE_energy.clear();
  pfClusterHBHE_eta.clear();
  pfClusterHBHE_phi.clear();

  pfClusterHO_energy.clear();
  pfClusterHO_eta.clear();
  pfClusterHO_phi.clear();

  pfClusterHF_energy.clear();
  pfClusterHF_eta.clear();
  pfClusterHF_phi.clear();

  track_pt.clear();
  track_eta.clear();
  track_phi.clear();

  


  //================================================================

  //METs =======================
  caloMETPt = caloMET->begin()->pt();
  caloMETPhi = caloMET->begin()->phi();
  caloMETSumEt = caloMET->begin()->sumEt();

  pfCaloMETPt = pfCaloMET->begin()->pt();
  pfCaloMETPhi = pfCaloMET->begin()->phi();
  pfCaloMETSumEt = pfCaloMET->begin()->sumEt();

  pfClusterMETPt = pfClusterMET->begin()->pt();
  pfClusterMETPhi = pfClusterMET->begin()->phi();
  pfClusterMETSumEt = pfClusterMET->begin()->sumEt();

  //ECAL clusters
  for( size_t ibc=0; ibc<pfClustersEcal->size(); ++ibc ) {
    reco::PFClusterRef bcRef( pfClustersEcal, ibc );
    pfClusterEcal_energy.push_back( bcRef->energy() );
    pfClusterEcal_eta.push_back( bcRef->eta() );
    pfClusterEcal_phi.push_back( bcRef->phi() );
  }

  //HBHE clusters
  for( size_t ibc=0; ibc<pfClustersHBHE->size(); ++ibc ) {
    reco::PFClusterRef bcRef( pfClustersHBHE, ibc );
    pfClusterHBHE_energy.push_back( bcRef->energy() );
    pfClusterHBHE_eta.push_back( bcRef->eta() );
    pfClusterHBHE_phi.push_back( bcRef->phi() );
  }

  //HO clusters
  for( size_t ibc=0; ibc<pfClustersHO->size(); ++ibc ) {
    reco::PFClusterRef bcRef( pfClustersHO, ibc );
    pfClusterHO_energy.push_back( bcRef->energy() );
    pfClusterHO_eta.push_back( bcRef->eta() );
    pfClusterHO_phi.push_back( bcRef->phi() );
  }

  //HF clusters
  for( size_t ibc=0; ibc<pfClustersHF->size(); ++ibc ) {
    reco::PFClusterRef bcRef( pfClustersHF, ibc );
    pfClusterHF_energy.push_back( bcRef->energy() );
    pfClusterHF_eta.push_back( bcRef->eta() );
    pfClusterHF_phi.push_back( bcRef->phi() );
  }


  //tracks
  for( size_t ibc=0; ibc<tracks->size(); ++ibc ) {
    reco::TrackRef trkRef( tracks, ibc );
    track_pt.push_back( trkRef->pt() );
    track_eta.push_back( trkRef->eta() );
    track_phi.push_back( trkRef->phi() );
  }


  //tree filling ===========================
  s->Fill();

}


DEFINE_FWK_MODULE(METScanningNtupleMaker);
