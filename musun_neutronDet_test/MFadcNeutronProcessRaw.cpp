/***********************************************************************\
Name:           MFadcNeutronProcessRaw.cpp
Created by:     Nandita Raha
Contents:       Module to take FADC data and extract neutron information.
\***********************************************************************/

#include "MFadcNeutronProcessRaw.h"
#include "TFadcPulseRawSample.h"
#include "TMucapData.h"
#include "TMusunFadcPulse.h"

/*--------Static Parameters--------------------------------------------*/
static TH2 *hEnergyVsChannel ;
static TH2 *hTimeVsChannel ;
static TH2 *hAmplitudeVsChannel ;
static TH2 *hPedestalVsChannel ;
static TH1 *hNeutronPulse ;

/*--------Module Declaration-------------------------------------------*/

INT MFadcNeutronProcessRaw_init(void);
INT MFadcNeutronProcessRaw(EVENT_HEADER*, void*);
int ProcessNeutronFadcData(EVENT_HEADER *pheader, void *pevent);
void printBankToFile(unsigned char *raw, int bank_size, char *bank_name);
int processIsland (int t0, int nsamples, int *samples, int channel);
int timestamp;

extern TMucapData *gData;
extern HNDLE hDB;

ANA_MODULE MFadcNeutronProcessRaw_module = {
  "MFadcNeutronProcessRaw",    /* module name           */  
  "Serdar Kizilgul",           /* author                */
  MFadcNeutronProcessRaw,      /* event routine         */
  NULL,                        /* BOR routine           */
  NULL,                        /* EOR routine           */
  MFadcNeutronProcessRaw_init,         /* init routine          */
  NULL,                        /* exit routine          */
  NULL,                        /* parameter structure   */
  0,                           /* structure size        */
  NULL,                        /* initial parameters    */
};

/*--------Init Routine-------------------------------------------------*/
INT MFadcNeutronProcessRaw_init(void){
  // Initialize histogr char name[80], title[80];
  hEnergyVsChannel = new TH2D("hFadcNeutronEnergyVsChannel","Pulse Energies",8,-0.5, 7.5, 1000, 0, 10000);
  hTimeVsChannel = new TH2D("hFadcNeutronTimeVsChannel","Pulse Time",8,-0.5, 7.5, 10000, 0, 2.0e8);

  hAmplitudeVsChannel = new TH2D("hFadcNeutronAmplitudeVsChannel","Pulse Amplitude",8,-0.5, 7.5, 1000, 0, 5000);
  hPedestalVsChannel = new TH2D("hFadcNeutronPedestalVsChannel","Pedestal",8,-0.5, 7.5, 1000, 0, 5000);
  hNeutronPulse = new TH1D("hNeutronPulse","Pulse",60,0,60);
  return SUCCESS;
}




/*--------Event Routine------------------------------------------------*/

INT MFadcNeutronProcessRaw(EVENT_HEADER *pheader, void *pevent)
{
  //printf("MFadcNeutronProcesRaw:: serial number is %d\n", pheader->serial_number);
  char bank_name[8];
  
  bool foundFadcData = false;

  for(int fadc=0; fadc<kMaxFadcs; fadc++) {    
    fadc_word *fadc_bank;
    sprintf(bank_name, "ADC%d", fadc);
    int fadc_bank_size = bk_locate(pevent, bank_name, (DWORD *) &fadc_bank);
    if(fadc_bank_size > 0){
      // We found some old FADC banks, let's process them and we're done
      foundFadcData = true;  
    }
  }
  if(!foundFadcData){
    ProcessNeutronFadcData(pheader, pevent);
    //printf("MFadcNeutronProcessRaw:: ProcessNewFadcData returned\n");
  }
  // printf("MFadcNeutronProcessRaw-success\n");
  return SUCCESS;
}



int ProcessNeutronFadcData(EVENT_HEADER *pheader, void *pevent){
  //printf("MFadcNeutronProcessRaw:: Processing neutron process raw \n");
  // Get block number from event header
  int block_number = ((EVENT_HEADER *) pevent - 1)->serial_number;

  // open the output bank
  FADC_info *Fadc_Stats_Array;
  bk_create (pevent, "FDCR", TID_DWORD, (DWORD *) & Fadc_Stats_Array);
  int FDCRcounter = 0;

/////////////////////////////////////////////////////////////////////////////////
  // unpack the raw data
  extern HNDLE hDB;
  int PSIrun=-1;
  int size=sizeof(int);
  db_get_value(hDB,0,"Runinfo/Run number", &PSIrun, &size, TID_INT,1);
  int runnumber = DeterminePSIRun(PSIrun);

  //numUnpacked = 0;

  static int first = 1;
 
  for (int i = 0; i <= 8; i++){
    //int board, channel;
    //board = anodes_map[i - 1].board;
    //channel = anodes_map[i - 1].channel;
    
    //   if (board < 0 || channel < 0)
    //   continue;

    char name[80];
    unsigned char *raw;
       
    if ( i == 0 ) { sprintf (name, "Naff"); }
    if ( i == 1 ) { sprintf (name, "Nbff"); }
    if ( i == 2 ) { sprintf (name, "Ncff"); }
    if ( i == 3 ) { sprintf (name, "Ndff"); }
    if ( i == 4 ) { sprintf (name, "Neff"); }
    if ( i == 5 ) { sprintf (name, "Nfff"); }
    if ( i == 6 ) { sprintf (name, "Ngff"); }
    if ( i == 7 ) { sprintf (name, "Nhff"); }
    
    int bank_size = bk_locate (pevent, name, &raw);
    bank_size /= 10;

    // printBankToFile(raw,bank_size,name);   }

    int islandSamples[8192];
    int islandNsamples = 0;
    int islandT0 = -1;
    bool firstIsland = true;
 
    // loop through words to build up "islands"
    int lastTimestamp = 0;
    //printf("Bank size for bank %s is %d\n", name, bank_size);

    if ( bank_size < 10 ) 
    continue ;

    for(int j = 0; j < bank_size; j++) {
     
      // data format:
      //
      //  bits 
      //  78-52       timestamp
      //  51-48       overflow
      //  47-36       sampleB0
      //  35-24       sampleA0
      //  23-12       sampleB1
      //  11-0        sampleA1
      //printf ("%.8x\t%.8x\n",raw[j*10+0],raw[j*10+1]);
      //printf ("%.8x\t%.8x\n",raw[j*10+2],raw[j*10+3]);
      //printf ("%.8x\t%.8x\n",raw[j*10+4],raw[j*10+5]);
      //printf ("%.8x\t%.8x\n",raw[j*10+6],raw[j*10+7]);
      //printf ("%.8x\t%.8x\n",raw[j*10+8],raw[j*10+9]);

      timestamp =  (raw[j*10+0] << 20) |
                       (raw[j*10+1] << 12) |
                       (raw[j*10+2] << 4) |
                       (raw[j*10+3] >> 4);

      bool overflowB0 = ((raw[j*10+3] & 0x08) != 0);
      bool overflowA0 = ((raw[j*10+3] & 0x04) != 0);
      bool overflowB1 = ((raw[j*10+3] & 0x02) != 0);
      bool overflowA1 = ((raw[j*10+3] & 0x01) != 0);

      int sampleB0 = (overflowB0 << 12) |
                     (raw[j*10+4] << 4) |
                     (raw[j*10+5] >> 4);  
      int sampleA0 = (overflowA0 << 12) |
                     ((raw[j*10+5] & 0xf) << 8) |
                     (raw[j*10+6]);  
      int sampleB1 = (overflowB1 << 12) |
                     (raw[j*10+7] << 4) |
                     (raw[j*10+8] >> 4);  
      int sampleA1 = (overflowA1 << 12) |
                     ((raw[j*10+8] & 0xf) << 8) |
                     (raw[j*10+9]);  
      
      
      //hTimeVsChannel->Fill(i,timestamp);

      printf ("MFadcNeutronProcessRaw:: Timestamp test=%d\n",timestamp);

     if(timestamp < lastTimestamp) {
       printf("MFadcNeutronProcessRaw:: Time ordering error!\n");
       printf("MFadcNeutronProcessRaw:: Tsst=%d %d\n",timestamp,lastTimestamp);
     }

      if(timestamp != lastTimestamp + 1) {
        if(!firstIsland) {
          int stop = processIsland(islandT0*2, islandNsamples, islandSamples, i);
	  //printf("MFadcNeutronProcessRaw:: test=%d\n",stop);
          
          if(stop) {
             return 1;
          }
        }
        islandT0 = timestamp; 
        islandNsamples = 0;
        firstIsland = false;
      }
      lastTimestamp = timestamp;

      islandSamples[islandNsamples++] = sampleA1;
      islandSamples[islandNsamples++] = sampleB1;
      islandSamples[islandNsamples++] = sampleA0;
      islandSamples[islandNsamples++] = sampleB0;
    }

    //    if(islandNsamples != 30) {
      //printf("MFadcNeutronProcessRaw:: %d :: process Island is called >>>>>>>, number of samples:: %d\n", i, islandNsamples);
      FDCRcounter +=
      processIsland(islandT0*2, islandNsamples, islandSamples, i);
      //}
  }

//////////////////////////////////////////////////////////////////////////////

  bk_close (pevent, Fadc_Stats_Array + FDCRcounter);
  
  return 1;
}


int processIsland(int t0, int nsamples, int *samples, int channel)
{
  int FDCRcounter = 0 ;

  TH1 *hPedTemp = new TH1D ("pedTemp", "pedTemp", 4096, -0.5, 4095.5);

  double sum = 0;
  int min = 4097;
  int max = -1;

  double binWidth = 1000./100.00;
  //TH1D *waveform = new TH1D("waveform", title, nsamples, -0.5*binWidth, (nsamples-0.5)*binWidth);

  for(int i = 0; i < 1000; i++) {
    hNeutronPulse->SetBinContent(i+1,0);
  }

  for(int i = 0; i < nsamples; i++) {
    sum += samples[i];
    if(samples[i] < min) min = samples[i];
    if(samples[i] > max) max = samples[i];
    //printf("MFadcNeutronProcessRaw:: .........%d\n", samples[i]);
    hPedTemp->Fill(samples[i]) ;
    hNeutronPulse->SetBinContent(i+1,samples[i]);
  }
  int mb = hPedTemp->GetMaximumBin();
  //printf("MFadcNeutronProcessRaw:: .....pedestal %d :: maxval %d\n",mb,max);
  
  int ampLimit = 50 ;
  double energyguess = sum - nsamples*mb ;
  //if ( max - mb > ampLimit ) {
    hEnergyVsChannel->Fill(channel, energyguess);
    hAmplitudeVsChannel->Fill(channel, max-mb);
    hPedestalVsChannel->Fill(channel, mb);
    hTimeVsChannel->Fill(channel,t0); 

    //}

  delete hPedTemp;
  return 0;
}



