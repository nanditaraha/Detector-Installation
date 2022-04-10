#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TMinuit.h"
#include "TMath.h"

TH1D *hchannel;
TH1D *hclosestGondola;
TH1D *hnsamples;
TH1D *hpeakSample;
TH1D *hpeakTime;
TH1D *hped;
TH1D *hpedSpread;
TH1D *hslowIntegral20;
TH1D *hslowIntegral25;
TH1D *hslowIntegral30;
TH1D *hslowIntegral35;
TH1D *hslowIntegral40;
TH1D *htotalIntegral;
//TH1D *htotalIntegral0;
//TH1D *htotalIntegral1;
//TH1D *htotalIntegral2;

TH2D *htotalSlow20Integral;
TH2D *htotalSlow25Integral;
//TH2D *htotalSlow30Integral;
TH2F *htotalSlow30Integral;
TH2D *htotalSlow35Integral;
TH2D *htotalSlow40Integral;

void readNeutronTree(int run, int det)
{
  char name[80];
  sprintf(name, "run%05d.root", run);

  // det=0;

  TFile *sig_file = new TFile(name);
  
  TTree *sig_tree = (TTree*)sig_file->Get("ndet");  

  Int_t n;
  sig_tree->SetBranchAddress("_n_", &n);

  Int_t    channel[20000], closestGondola[20000], nsamples[20000], peakSample[20000];
  Double_t peakTime[20000], ped[20000], pedSpread[20000], slowIntegral20[20000], slowIntegral25[20000], 
           slowIntegral30[20000], slowIntegral35[20000], slowIntegral40[20000], totalIntegral[20000];

  sig_tree->SetBranchAddress("channel", &channel);
  sig_tree->SetBranchAddress("closestGondola", &closestGondola);
  sig_tree->SetBranchAddress("nsamples", &nsamples);
  sig_tree->SetBranchAddress("peakSample", &peakSample);
  sig_tree->SetBranchAddress("peakTime", &peakTime);
  sig_tree->SetBranchAddress("ped", &ped);
  sig_tree->SetBranchAddress("pedSpread", &pedSpread);
  sig_tree->SetBranchAddress("slowIntegral20", &slowIntegral20);
  sig_tree->SetBranchAddress("slowIntegral25", &slowIntegral25);
  sig_tree->SetBranchAddress("slowIntegral30", &slowIntegral30);
  sig_tree->SetBranchAddress("slowIntegral35", &slowIntegral35);
  sig_tree->SetBranchAddress("slowIntegral40", &slowIntegral40);
  sig_tree->SetBranchAddress("totalIntegral", &totalIntegral);

   
  htotalIntegral = new TH1D("htotalIntegral", "htotalIntegral", 1000, 0, 10000);
  //htotalIntegral0 = new TH1D("htotalIntegral0", "htotalIntegral0", 1000, 0, 10000);
  //htotalIntegral1 = new TH1D("htotalIntegral1", "htotalIntegral1", 1500, 0, 15000);
  //htotalIntegral2 = new TH1D("htotalIntegral2", "htotalIntegral2", 1000, 0, 10000);
  hchannel = new TH1D("hchannel", "hchannel", 10, 0, 10);
  hclosestGondola = new TH1D("hclosestGondola", "hClosestGondola", 20, 0, 20);
  hnsamples = new TH1D("hnsamples", "hnsamples", 100, 0, 100);
  hpeakSample = new TH1D("hpeakSample", "hpeakSample", 100, 0, 4000);
  hpeakTime = new TH1D("hpeakTime", "hpeakTime", 10000, 0, 1e9);
  hped = new TH1D("hped", "hped", 1000, 0, 4000);
  hpedSpread = new TH1D("hpedSpread", "hpedSpread", 100, -50, 50);
  hslowIntegral20 = new TH1D("hslowIntegral20", "hslowIntegral20", 1100, -1000, 10000);
  hslowIntegral25 = new TH1D("hslowIntegral25", "hslowIntegral25", 1100, -1000, 10000);
  hslowIntegral30 = new TH1D("hslowIntegral30", "hslowIntegral30", 1100, -1000, 10000);
  hslowIntegral35 = new TH1D("hslowIntegral35", "hslowIntegral35", 1100, -1000, 10000);
  hslowIntegral40 = new TH1D("hslowIntegral40", "hslowIntegral40", 1100, -1000, 10000);
 

  htotalSlow20Integral = new TH2D("htotalSlow30Integral", "htotalSlow30Integral", 
				  1000, 0, 10000, 1100, -1000, 10000);
  htotalSlow25Integral = new TH2D("htotalSlow30Integral", "htotalSlow30Integral", 
				  1000, 0, 10000, 1100, -1000, 10000);
  htotalSlow30Integral = new TH2F("htotalSlow30Integral", "htotalSlow30Integral", 
				  300., 0., 3000., 330., -300.,3000.);
  //  htotalSlow30Integral = new TH2D("htotalSlow30Integral", "htotalSlow30Integral",
  //			  1000, 0, 10000, 1100, -1000, 10000);

  htotalSlow35Integral = new TH2D("htotalSlow30Integral", "htotalSlow30Integral", 
				  1000, 0, 10000, 1100, -1000, 10000);
  htotalSlow40Integral = new TH2D("htotalSlow30Integral", "htotalSlow30Integral", 
				  1000, 0, 10000, 1100, -1000, 10000);
  htotalToSlow30Integral = new TH2D("htotalToSlow30Integral", "htotalSlow30Integral",
                                  1000, 0, 10000, 1100, -10000, 10000);


  Int_t nevent = sig_tree->GetEntries();
  for (Int_t i=0;i<nevent;i++) {
      sig_tree->GetEntry(i);           
      //printf("event:%d n hits %d\n",i,n);

      for (Int_t j=0;j<n;j++){
	
	//printf("Whats channel[j] %d\n",channel[0]);
	//det = j;
        hchannel->Fill(channel[j]);
	
	if (channel[j] == det )
	  {
	printf("event %d hit %d ped %f peakTime %f peakSample %d totalIntegral %f\n",i,n,ped[j],peakTime[j], peakSample[j],totalIntegral[j]);
	    
	    hclosestGondola->Fill(closestGondola[j]);
	    hnsamples->Fill(nsamples[j]);
	    hpeakSample->Fill(peakSample[j]);
	    hpeakTime->Fill(peakTime[j]);
	    htotalIntegral->Fill(totalIntegral[j]);
	    hpedSpread->Fill(pedSpread[j]);
	    hslowIntegral20->Fill(slowIntegral20[j]);
	    hslowIntegral25->Fill(slowIntegral25[j]);
	    hslowIntegral30->Fill(slowIntegral30[j]);
	    hslowIntegral35->Fill(slowIntegral35[j]);
	    hslowIntegral40->Fill(slowIntegral40[j]);
	    htotalSlow20Integral->Fill(totalIntegral[j],slowIntegral20[j]);
	    htotalSlow25Integral->Fill(totalIntegral[j],slowIntegral25[j]);
	    htotalSlow30Integral->Fill(totalIntegral[j]*0.3,slowIntegral30[j]*0.3);
	    htotalSlow35Integral->Fill(totalIntegral[j],slowIntegral35[j]);
	    htotalSlow40Integral->Fill(totalIntegral[j],slowIntegral40[j]);
	    htotalToSlow30Integral->Fill(slowIntegral30[j]/totalIntegral[j],slowIntegral30[j]);

	    
	  }
	  
      }
   }
}

TGraphErrors *gh, *ga;

void gainPlot(){

  Double_t peakHeight[6]={1444,1544,1699,1896,2120,2438};
  Double_t dpeakHeight[6]={2,2,3,4,5,5};
  Double_t peakArea[6]={1173,1669,2285,3165,4287,5655};
  Double_t dpeakArea[6]={7,11,13,14,28,30};
  Double_t highVoltage[6]={1650,1700,1750,1800,1850,1900};
  Int_t n = 6;

  gh = new TGraphErrors(n,highVoltage,peakHeight,0,dpeakHeight);
  ga = new TGraphErrors(n,highVoltage,peakArea,0,dpeakArea);

  gh->SetTitle("peak height versus high voltage");
  ga->SetTitle("peak area versus high voltage");
  gh->SetMarkerColor(kRed);
  gh->SetMarkerSize(0.4);
  gh->SetMarkerStyle(8);
  ga->SetMarkerColor(kBlue);
  ga->SetMarkerSize(0.4);
  ga->SetMarkerStyle(8);

}
