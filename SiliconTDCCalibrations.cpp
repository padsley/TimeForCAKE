{
  TFile *fin = TFile::Open("SiliconTime_BeamData.root");
  TH2F *h2D = (TH2F*)fin->FindObjectAny("h2D");
  
  h2D->Draw("col");
  
  TFile *fout = new TFile("SiliconTDCCalibrations.root","RECREATE");
  
  ofstream output;
  output.open("SiliconTDCCalibrations.dat");
  
  output << "double SiliconTDCOffsets[64] = {";
  
  for(int i=1;i<=64;i++)
  {
    TH1D *hproj = h2D->ProjectionX("_px",i,i);
    hproj->Draw();
    
    int MaxBin = hproj->GetMaximumBin();
    float MaxBinPos = hproj->GetBinCenter(MaxBin);
    
    TF1 *fitty = new TF1("fitty","gaus",MaxBinPos-100,MaxBinPos+100);
    fitty->SetParameter(1,MaxBinPos);
    hproj->Fit(fitty,"R");
    
    output << fitty->GetParameter(1);
    if(i<64)output << "," << "\n";
    cout << i-1 << "\t" << MaxBinPos << "\n";
  }
  output << "};" << endl;
  output.close();
}