{
  /*****************************************************************/
  // Prepare the canvas (modified to include energy/time spectrum)
 
  gROOT->LoadMacro("palette.cxx");
  SetRainbowPalette();
  gStyle->SetOptStat("ne");
  
  TCanvas *MuSunCanvas = (TCanvas *) gROOT->GetListOfCanvases()->At(0);
  MuSunCanvas->Divide(4,4);

  

  /*********************** Pad 1 ***********************************/
  MuSunCanvas->cd(1);
  TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronAmplitudeVsChannel");
  TH1 *hHistEgy_py = hHistEgy->ProjectionY("Chan1",1,1,"");
  hHistEgy_py->Draw();

  //TH1 *hHistEgy_px = hHistEgy->ProjectionX();
  // printf("Channel # %i\n",hHistEgy->GetXaxis()->FindBin(3.0));
  //printf("Energy %d\n",hHistEgy->GetYaxis()->FindBin(3.0));

  
  /*********************** Pad 2 ***********************************/
  MuSunCanvas->cd(2);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py1 = hHistEgy->ProjectionY("Chan2",2,2,"");
  hHistEgy_py1->Draw();
 
  /*********************** Pad 3 ***********************************/
  MuSunCanvas->cd(3);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py2 = hHistEgy->ProjectionY("Chan3",3,3,"");
  hHistEgy_py2->Draw();

  /*********************** Pad 4 ***********************************/
  MuSunCanvas->cd(4);
  // TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py3 = hHistEgy->ProjectionY("Chan4",4,4,"");
  hHistEgy_py3->Draw();
  
  /****************************************************************/
  MuSunCanvas->cd(5);
  // TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py4 = hHistEgy->ProjectionY("Chan5",5,5,"");
  hHistEgy_py4->Draw();

  MuSunCanvas->cd(6);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py5 = hHistEgy->ProjectionY("Chan6",6,6,"");
  hHistEgy_py5->Draw();
  
  /****************************************************************/


  MuSunCanvas->cd(7);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py6 = hHistEgy->ProjectionY("Chan7",7,7,"");
  hHistEgy_py6->Draw();
  /* pad5->cd();
  TH1 *hNPulse = (TH1*) getHist("hNeutronPulse");
  hNPulse->Draw();
  */
  /*************************************************************/
  MuSunCanvas->cd(8);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistEgy_py7 = hHistEgy->ProjectionY("Chan8",8,8,"");
  hHistEgy_py7->Draw();
  
  /**********************TIME SPECTRA*************************/
  /***********************************************************/

  MuSunCanvas->cd(9);
  TH2 *hHistTy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py = hHistTy->ProjectionY("ChanT1",1,1,"");
  hHistTy_py->Draw();

  //TH1 *hHistEgy_px = hHistEgy->ProjectionX();
  // printf("Channel # %i\n",hHistEgy->GetXaxis()->FindBin(3.0));
  //printf("Time %d\n",hHistTy->GetYaxis()->FindBin(3.0));

  
  /*********************** Pad 2 ***********************************/
  MuSunCanvas->cd(10);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronEnergyVsChannel");
  TH1 *hHistTy_py1 = hHistTy->ProjectionY("ChanT2",2,2,"");
  hHistTy_py1->Draw();
 
  
  /*********************** Pad 3 ***********************************/
  MuSunCanvas->cd(11);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py2 = hHistTy->ProjectionY("ChanT3",3,3,"");
  hHistTy_py2->Draw();

  /*********************** Pad 4 ***********************************/
  MuSunCanvas->cd(12);
  // TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py3 = hHistTy->ProjectionY("ChanT4",4,4,"");
  hHistTy_py3->Draw();
  
  /****************************************************************/
  MuSunCanvas->cd(13);
  // TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py4 = hHistTy->ProjectionY("ChanT5",5,5,"");
  hHistTy_py4->Draw();

  MuSunCanvas->cd(14);
  //TH2 *hHistEgy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py5 = hHistTy->ProjectionY("ChanT6",6,6,"");
  hHistTy_py5->Draw();
  
  /****************************************************************/


  MuSunCanvas->cd(15);
  //  TH2 *hHistTy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py6 = hHistTy->ProjectionY("ChanT7",7,7,"");
  hHistTy_py6->Draw();
  
  /*************************************************************/
  MuSunCanvas->cd(16);
  //TH2 *hHistTy = (TH2*) getHist("hFadcNeutronTimeVsChannel");
  TH1 *hHistTy_py7 = hHistTy->ProjectionY("ChanT8",8,8,"");
  hHistTy_py7->Draw();


} 
 
