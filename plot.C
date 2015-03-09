void plot(){
  TFile *file = new TFile("result.root");
  TTree *T = (TTree*)file->Get("T");
  TCanvas *c1 = new TCanvas("c1","c1",1200,800);
  c1->Divide(2,2);
  c1->cd(1);
  TH1F *h1 = new TH1F("h1","h1",50,0.,50);
  TH1F *h2 = new TH1F("h2","h2",50,0.,50);
  T->Project("h1","nrealhit");
  T->Project("h2","nrealhit","nw+nv+nu>nc");
  h1->Draw();
  h2->Draw("same");
  h2->SetLineColor(2);
  h1->SetXTitle("# of real hits");
  h1->SetTitle("Degeneracy");
  TLegend *le1 = new TLegend(0.5,0.15,0.89,0.6);
  le1->AddEntry(h1,"Simulated","l");
  le1->AddEntry(h2,"Pass nWire>nHit cut","l");
  le1->Draw();

  
  c1->cd(2);
  TH2F *h3 = new TH2F("h3","h3",50,0.,50,150,0.,150);
  
  T->Project("h3","nw+nv+nu:nrealhit");
  h3->Draw("COLZ");
  h3->SetXTitle("# of real hits");
  h3->SetYTitle("nWire");
  h3->SetTitle();

  c1->cd(3);
  TH2F *h4 = new TH2F("h4","h4",50,0.,50,100,0.,1000);
  T->Project("h4","nc:nrealhit");
  h4->Draw("COLZ");
  h4->SetTitle();
  h4->SetXTitle("# of real hits");
  h4->SetYTitle("# of potential hits");

  c1->cd(4);
  TH2F *h5 = new TH2F("h5","h5",50,0.,150.,50,0.,150.);
  
  T->Project("h5","nc:nw+nv+nu");
  h5->Draw("COLZ");
  h5->SetXTitle("nWire");
  h5->SetYTitle("# of potential hits");
  TLine *l1 = new TLine(0.,0.,150,150);
  l1->Draw();
  l1->SetLineWidth(2.5);
}
