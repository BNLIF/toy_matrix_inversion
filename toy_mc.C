// Example ROOT script for processing Daya Bay data trees
//
//  This example uses the simple 'GetLeaf()' approach for accessing
//  event data.  It is easier to understand, but can be slightly
//  slower.  See 'dybTreeDraw.C' for the simplest approach.  See
//  'dybTreeSetBranch.C' if you want speed.
//   
//   Usage:
//   root [0] .L dybTreeGetLeaf.C+
//   root [1] dybTreeGetLeaf("recon*.root")
//
// Created by: dandwyer@caltech.edu  2011/10/11   
#include <TROOT.h>
#include "TApplication.h"
#include "Rtypes.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <fstream>
#include "math.h"
#include "stdio.h"
#include "TH1F.h"
#include "TString.h"
#include <vector>
#include "TChain.h"
#include "TFile.h"
#include "TEventList.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TCut.h"
#include "TLeaf.h"
#include "TH2F.h"
#include "TMath.h"
#include "TRandom.h"

using namespace::std;

Double_t calc_u(Double_t x, Double_t y){
  Double_t u = sqrt(3.)/2.*y - 0.5*x;
  return u;
}
Double_t calc_v(Double_t x, Double_t y){
  Double_t v = sqrt(3.)/2.*y + 0.5*x;
  return v;
}
Double_t calc_w(Double_t x, Double_t y){
  Double_t w = x;
  return w;
}
Double_t calc_xw(Double_t w){
  Double_t x = w;
  return x;
}
Double_t calc_xuv(Double_t u, Double_t v){
  Double_t x = (v-u);
  return x;
}
Double_t calc_yuv(Double_t u, Double_t v){
  Double_t y = (u+v)/sqrt(3.);
  return y;
}

Int_t map_wire_dis(Double_t dis){
  Int_t wire;
  if (dis > 0) {
    wire = dis/0.003 + 250;
  }else{
    wire = dis/0.003 + 250;
  }
  return wire;
}

Double_t map_dis_wire(Int_t wire){
  Double_t dis = 0.003 * (wire - 250);
  return dis;
}



//void toy_mc(){
int
main (Int_t argc, char *argv[])
{

  TFile *file = new TFile("result.root","RECREATE");
  TTree *T = new TTree("T","T");
  T->SetDirectory(file);
  Int_t nhit;
  T->Branch("nrealhit",&nhit,"nrealhit/I");
  Double_t xr[100],yr[100],charge[100];
  Int_t ur[100],vr[100],wr[100];
  T->Branch("xr",xr,"xr[nrealhit]/D");
  T->Branch("yr",yr,"yr[nrealhit]/D");
  T->Branch("charge",charge,"charge[nrealhit]/D");
  
  T->Branch("ur",ur,"ur[nrealhit]/I");
  T->Branch("vr",vr,"vr[nrealhit]/I");
  T->Branch("wr",wr,"wr[nrealhit]/I");
  


  
  Int_t nu = 0;
  Int_t nv = 0;
  Int_t nw = 0;
  T->Branch("nw",&nw,"nw/I");
  T->Branch("nu",&nu,"nu/I");
  T->Branch("nv",&nv,"nv/I");
  Int_t uf[500],vf[500],wf[500];
  T->Branch("uf",uf,"uf[nu]/I");
  T->Branch("vf",vf,"vf[nv]/I");
  T->Branch("wf",wf,"wf[nw]/I");
  Double_t ufc[500],vfc[500],wfc[500];
  T->Branch("ufc",ufc,"ufc[nu]/D");
  T->Branch("vfc",vfc,"vfc[nv]/D");
  T->Branch("wfc",wfc,"wfc[nw]/D");

  Int_t nc = 0;
  T->Branch("nc",&nc,"nc/I");
  Double_t xc[1000],yc[1000];
  Int_t ucc[1000],vcc[1000],wcc[1000];

  T->Branch("xc",xc,"xc[nc]/D");
  T->Branch("yc",yc,"yc[nc]/D");
  
  T->Branch("ucc",ucc,"ucc[nc]/I");
  T->Branch("vcc",vcc,"vcc[nc]/I");
  T->Branch("wcc",wcc,"wcc[nc]/I");




  for (Int_t qx = 0;qx!=100000;qx++){
    
    if (qx%1000 ==0) cout << qx << endl;
    Double_t u[500], v[500], w[500];
    for (Int_t i=0;i!=500;i++){
      u[i] = 0.;
      v[i] = 0.;
      w[i] = 0.;
    }
    
    nhit = gRandom->Uniform(1,20);
    //nhit = 2;
    //cout << "Real hits: " << nhit << endl;
    
    // simulate real hits
    for (Int_t i=0;i!=nhit;i++){
      Double_t xp = gRandom->Uniform(-0.5,0.5);
      Double_t yp = gRandom->Uniform(-0.5,0.5);
      
      Double_t up = calc_u(xp,yp);
      Double_t vp = calc_v(xp,yp);
      Double_t wp = calc_w(xp,yp);
      
      Int_t uc = map_wire_dis(up);
      Int_t vc = map_wire_dis(vp);
      Int_t wc = map_wire_dis(wp);

      xr[i] = xp;
      yr[i] = yp;
      ur[i] = uc;
      vr[i] = vc;
      wr[i] = wc;

      charge[i] = gRandom->Uniform(5,30);
      
      
      if (uc>=0&&uc<500) u[uc]+= charge[i];
      if (vc>=0&&vc<500) v[vc]+= charge[i];
      if (wc>=0&&wc<500) w[wc]+= charge[i];
      //cout << "Simu " << xp << " " << yp <<  " " << wc << endl;
      // cout << wp << endl;
      //cout << uc << " " << vc << " " << wc << endl;
    }
    
    
    //find out all the wires fired
    nu = 0;
    nv = 0;
    nw = 0;
    
    
    for (Int_t i=0;i!=500;i++){
      if (u[i]>0){
	uf[nu] = i;
	ufc[nu] = u[i];
	nu++;
      }
      if (v[i]>0){
	vf[nv] = i;
	vfc[nv] = v[i];
	nv++;
      }
      if (w[i]>0){
	wf[nw] = i;
	wfc[nw] = w[i];
	nw++;
      }
    }
    
    // cout << "Fired wire: u,v,w ";
    // cout << nu << " " << nv << " " << nw << endl;
    
    // cout << uf[0] << " " << uf[1] << endl;
    // cout << vf[0] << " " << vf[1] << endl;
    // cout << wf[0] << " " << wf[1] << endl;
    
    
    // find out all the false hits
    // 
    nc = 0;
    
    
    // loop over all u wires
    for (Int_t i=0;i!=nu;i++){
      // loop over all v wires
      Double_t up = map_dis_wire(uf[i]);
      for (Int_t j=0;j!=nv;j++){
	Double_t vp = map_dis_wire(vf[j]);
	
	Int_t flag = 0;
	for (Int_t k1=0;k1!=3;k1++){
	  for (Int_t k2=0;k2!=3;k2++){
	    Double_t xp = calc_xuv(up-0.0015+0.0015*k1,vp-0.0015+0.0015*k2);
	    Double_t yp = calc_yuv(up-0.0015+0.0015*k1,vp-0.0015+0.0015*k2);
	    Double_t wp = calc_w(xp,yp);
	    Int_t wc = map_wire_dis(wp);
	    //cout << xp << " " << uf[i] << " " << vf[j] << " " << wc << endl;
	    // cout << wc << " " << wp << endl;
	    if (fabs(xp)<0.51&&fabs(yp)<0.51){
	      if (w[wc]>0){
		xc[nc] = xp;
		yc[nc] = yp;

		ucc[nc] = i;
		vcc[nc] = j;
		
		for (Int_t k3 =0;k3!=nw;k3++){
		  if (wf[k3]==wc){
		    wcc[nc] = k3;
		    break;
		  }
		}
		
		flag = 1;
	      }
	    }
	  }
	}
	if (flag==1){
	  nc ++;
	}
	
      }
    }
    T->Fill();
    // cout << "Potential hits: " << nc << endl;
    // if (nc==1) break;
  }

  file->Write();
  file->Close();
  

}
