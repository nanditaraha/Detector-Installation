TH1 *fitM(TH2 *h, bool scaled, char *name)
{
  TF1 *mFunc = new TF1("mFunc", "gaus(0) + gaus(3)");

  TH1 *outHist = new TH1D(name, name, h->GetXaxis()->GetNbins(), 
			  h->GetXaxis()->GetBinLowEdge(1),
			  h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->GetNbins() + 1));

  for(int i = 0; i < h->GetNbinsX(); i++) {
    TH1 *slice = h->ProjectionY("slice", i, i);

    // find primary peak
    double maximumHeight = slice->GetMaximum();
    int maximumBin = slice->GetMaximumBin();
    int nbins = slice->GetNbinsX();
  
    if(scaled) { 
      mFunc->SetParameters(maximumHeight, 0.12, 0.01, maximumHeight, 0.22, 0.02);
    } else {
      double scale = h->GetXaxis()->GetBinCenter(i);
      mFunc->SetParameters(maximumHeight, 0.1*scale, 0.03*scale, maximumHeight, 0.2*scale, 0.03*scale);
    }

    slice->Fit("mFunc", "L");
    c1->Update();
    c1->Modified();

    double m = fabs(mFunc->GetParameter(4) - mFunc->GetParameter(1)) / (fabs(mFunc->GetParameter(5)) + fabs(mFunc->GetParameter(2)))/2.35;

    if(m < 0.3 || m > 3) m = 0;
    if(mFunc->GetParameter(0) < 1 || mFunc->GetParameter(3) < 1) m = 0;
    if(!gMinuit->fCstatu->Contains("CONVERGED")) m = 0;
    if(slice->GetEntries() < 200) m = 0;

    outHist->SetBinContent(i, m);

  }  
}
