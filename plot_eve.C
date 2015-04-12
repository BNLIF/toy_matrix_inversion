#include "TVectorD.h"
#include "TMatrixD.h"

void plot_eve(Int_t neve = 3){
  gStyle->SetOptStat(0);

  TFile *file = new TFile("result.root");
  TTree *T = (TTree*)file->Get("T");

  Int_t nhit;
  T->SetBranchAddress("nrealhit",&nhit);
  Double_t xr[100],yr[100],charge[100];
  Int_t ur[100],vr[100],wr[100];
  T->SetBranchAddress("xr",xr);
  T->SetBranchAddress("yr",yr);
  T->SetBranchAddress("charge",charge);
  
  T->SetBranchAddress("ur",ur);
  T->SetBranchAddress("vr",vr);
  T->SetBranchAddress("wr",wr);
  

  Int_t nu = 0;
  Int_t nv = 0;
  Int_t nw = 0;
  T->SetBranchAddress("nw",&nw);
  T->SetBranchAddress("nu",&nu);
  T->SetBranchAddress("nv",&nv);
  Int_t uf[500],vf[500],wf[500];
  T->SetBranchAddress("uf",uf);
  T->SetBranchAddress("vf",vf);
  T->SetBranchAddress("wf",wf);
  Double_t ufc[500],vfc[500],wfc[500];
  T->SetBranchAddress("ufc",ufc);
  T->SetBranchAddress("vfc",vfc);
  T->SetBranchAddress("wfc",wfc);

  Int_t nc = 0;
  T->SetBranchAddress("nc",&nc);
  Double_t xc[1000],yc[1000];
  T->SetBranchAddress("xc",xc);
  T->SetBranchAddress("yc",yc);
  
  Int_t ucc[1000],vcc[1000],wcc[1000];
  T->SetBranchAddress("ucc",ucc);
  T->SetBranchAddress("vcc",vcc);
  T->SetBranchAddress("wcc",wcc);

  T->GetEntry(neve);
  
  //cout << nc << " " << nu << " " << nv << " " << nw << endl;
  

  // potential hit as well as wire charge 
  TVectorD vphit(nc);
  TVectorD vwire(nu+nv+nw);

  TMatrixD *A  = new TMatrixD(nu+nv+nw,nc);
  TMatrixD *AT = new TMatrixD(nc,nu+nv+nw);
  TMatrixD *Vy_invert = new TMatrixD(nu+nv+nw,nu+nv+nw);
  TMatrixD *MB = new TMatrixD(nc,nc);
  TMatrixD *FM = new TMatrixD(nc,nu+nv+nw);

  Double_t uncer = 0.05;
  Double_t fill_charge;
  // start to fill in everything ... 
  for (Int_t i=0;i!=nu;i++){
    fill_charge = gRandom->Gaus(ufc[i],ufc[i]*uncer);
    vwire[i] = fill_charge;
    (*Vy_invert)(i,i) = 1./pow(fill_charge*uncer,2);
  }
  for (Int_t i=0;i!=nv;i++){
    fill_charge = gRandom->Gaus(vfc[i],vfc[i]*uncer);
    vwire[i+nu] = fill_charge;
    (*Vy_invert)(i+nu,i+nu) = 1./pow(fill_charge*uncer,2);
  }
  for (Int_t i=0;i!=nw;i++){
    fill_charge = gRandom->Gaus(wfc[i],wfc[i]*uncer);
    vwire[i+nu+nv] = fill_charge;
    (*Vy_invert)(i+nu+nv,i+nu+nv) = 1./pow(fill_charge*uncer,2);
  }
  //Vy_invert->Draw("COLZ");
 
  // // Now construct A 
  for (Int_t i=0;i!=nc;i++){
    (*A)(ucc[i],i) = 1; // u
    (*A)(nu+vcc[i],i) = 1; // v
    (*A)(nu+nv+wcc[i],i) = 1; // w
  }
  
  // //A->Draw("COLZ");
  AT->Transpose(*A);
   
   *MB = (*AT) * (*Vy_invert) * (*A);
   
   cout << "Determinant: " << MB->Determinant() << endl;

   TCanvas *c1 = new TCanvas("c1","c1",1200,800);
   c1->Divide(2,2);
   c1->cd(1);
   MB->Draw("COLZ");
   //MB->Print();

   c1->cd(3);
   TMatrixDEigen *Eigen = new TMatrixDEigen(*MB);
   TMatrixD EigenValue = Eigen->GetEigenValues();
   TMatrixD *trans = new TMatrixD(nc,nc);
   TMatrixD *transT = new TMatrixD(nc,nc);
   

   *trans = Eigen->GetEigenVectors();
   transT->Transpose(*trans);
   EigenValue.Draw("COLZ");
   //EigenValue(21,21) = 0.001;
   //EigenValue(22,22) = 0.001;
   //does not work by changing the eigenvalue ... 

   c1->cd(1);
   trans->Draw("COLZ");

   c1->cd(4);
   trans->Draw("COLZ");
  // trans->Print();

   *MB = (*trans) *EigenValue * (*transT);
   //MB->Print();
   cout << "Determinant: " << MB->Determinant() << endl;

   MB->Invert();
   
   

   *FM = (*MB) * (*AT) * (*Vy_invert);

   // FM->Draw("COLZ");
   vphit = (*FM) * (vwire);


   c1->cd(2);
   TH2F *h1 = new TH2F("h1","h1",100,-0.5,0.5,100,-0.5,0.5);
   TH2F *h2 = new TH2F("h2","h2",100,-0.5,0.5,100,-0.5,0.5);

   for (Int_t i=0;i!=nhit;i++){
     h2->Fill(xr[i],yr[i]-0.02,Int_t(charge[i]*10)/10.);
   }
   h2->Draw("TEXT");

   h2->SetTitle(Form("# of real hits: %d, # of potential hits: %d",nhit,nc));
   //h1->Reset();
   for (Int_t i=0;i!=nc;i++){
     h1->Fill(xc[i],yc[i],Int_t(vphit[i]*10)/10.);
   }
   gStyle->SetPalette(1);
   h1->Draw("COLZsame");
   h1->GetZaxis()->SetRangeUser(0,vphit.Max()+1);

   
   c1->cd(4);
   h1->Draw();
   TGraph *g2 = new TGraph(nc,xc,yc);
   g2->Draw("*same");
   g2->SetMarkerStyle(21);
   g2->SetMarkerColor(2);
   

   

   TGraph *g1 = new TGraph(nhit,xr,yr);
   g1->Draw("*same");
   g1->SetMarkerStyle(20);
   g1->SetMarkerColor(1);
   


   // cout << nc << " " << nw + nv + nu << endl;

   // for (Int_t i=0;i!=nc;i++){
   //   cout << i << " " << EigenValue(i,i) << " " << xc[i] << " " << yc[i] << " " << vphit[i] << " " << fabs((*trans)(i,55)) << endl;
   // }
   // cout << endl;
   

   // // for (Int_t i=0;i!=nhit;i++){
   // //   cout << xr[i] << " " << yr[i] << " " << charge[i] << endl;
   // // }
   // // cout << endl;
   


   // // //test ... 
   // // for (Int_t i=0;i!=nu+nv+nw;i++){
   // //   cout << vwire[i] << " ";
   // // }
   // // cout << endl;

   // // A->Print();
   // // //test 
  
   
}
