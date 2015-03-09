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

void test(){
  for (Int_t i=0;i!=10;i++){
    Double_t dis = gRandom->Uniform(-0.5,0.5);
    Int_t wc1 = map_wire_dis(dis);
    dis = map_dis_wire(wc1); 
    Int_t wc2 = map_wire_dis(dis);
    cout << wc1 << " " << wc2 << endl;
  }
}
