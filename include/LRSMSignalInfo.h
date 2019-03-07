#ifndef LRSMSignalInfo_h
#define LRSMSignalInfo_h

class LimitResult{
public:
  TString region;
  double limit_obs, limit_exp, limit_exp_1sdUp, limit_exp_1sdDn, limit_exp_2sdUp, limit_exp_2sdDn;
};

class LRSMSignalInfo{
public :

  double mass_WR, mass_Z, mass_N, xsec;
  vector<LimitResult> LimitResults;
  TString prod_channel;
  TString lep_channel;
  TString generator;
  TString FileName;

  void SetNames();

  TString GetFileName();
  TString GetLegendAlias();
  TString GetTEXName();
  LRSMSignalInfo();

  void Print();

  //==== Get All Mass Info
  void GetMassMaps();
  void GetMassMapsPlot();
  void GetMassMapsSigDist();
  int NTotalMass;
  map< double, vector<double> > maps_WR_to_N;
  map< double, vector<double> > maps_N_to_WR;

  bool operator==(LRSMSignalInfo b);

private:
  TString filename, legendalias, texname;

};


LRSMSignalInfo::LRSMSignalInfo(){
  NTotalMass = 0;
}

void LRSMSignalInfo::SetNames(){

  if(generator=="aMCNLO"){
    if(prod_channel=="pair"){
      filename = "HNPairToJJJJ_"+lep_channel+"_ZP"+TString::Itoa(mass_Z,10)+"_N"+TString::Itoa(mass_N,10)+"_WR"+TString::Itoa(mass_WR,10);
      legendalias = "m_{Z'} = "+TString::Itoa(mass_Z,10)+" GeV, m_{N} = "+TString::Itoa(mass_N,10);
      texname = "SignalPair"+lep_channel+"Z"+TString::Itoa(mass_Z,10)+"N"+TString::Itoa(mass_N,10);
    }
    if(prod_channel=="SchWR"){
      filename = "WR_"+lep_channel+"JJ_WR"+TString::Itoa(mass_WR,10)+"_N"+TString::Itoa(mass_N,10);
      legendalias = "m_{W_{R}} = "+TString::Itoa(mass_WR,10)+" GeV, m_{N} = "+TString::Itoa(mass_N,10);
      texname = "SignalSchWR"+lep_channel+"WR"+TString::Itoa(mass_WR,10)+"N"+TString::Itoa(mass_N,10);
    }
  }

}

TString LRSMSignalInfo::GetFileName(){
  return filename;
}

TString LRSMSignalInfo::GetLegendAlias(){
  return legendalias;
}

TString LRSMSignalInfo::GetTEXName(){
  return texname;
}

void LRSMSignalInfo::GetMassMaps(){

  vector<double> m_WRs = {
400, 400, 1000, 1000, 1000, 1600, 1600, 1600, 1600, 2200, 2200, 2200, 2200, 2200, 2800, 2800, 2800, 2800, 2800, 2800, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 
  };
  vector<double> m_Ns = {
100, 300, 100, 400, 900, 100, 400, 1000, 1500, 100, 400, 1000, 1600, 2100, 100, 400, 1000, 1600, 2200, 2700, 100, 400, 1000, 1600, 2200, 2800, 3300, 100, 400, 1000, 1600, 2200, 2800, 3400, 3900, 100, 400, 1000, 1600, 2200, 2800, 3400, 4000, 4500, 100, 400, 1000, 1600, 2200, 2800, 3400, 4000, 4600, 5100, 100, 400, 1000, 1600, 2200, 2800, 3400, 4000, 4600, 5200, 5700, 100, 400, 1000, 1600, 2200, 2800, 3400, 4000, 4600, 5200, 5800, 6300, 100, 400, 1000, 1600, 2200, 2800, 3400, 4000, 4600, 5200, 5800, 6400, 6900, 
  };

  maps_WR_to_N.clear();
  maps_N_to_WR.clear();
  NTotalMass = 0;

  for(unsigned int i=0; i<m_WRs.size(); i++){

    double m_WR = m_WRs.at(i);
    double m_N = m_Ns.at(i);

    maps_WR_to_N[m_WR].push_back( m_N );
    maps_N_to_WR[m_N].push_back( m_WR );
    NTotalMass++;

  }

}

void LRSMSignalInfo::GetMassMapsPlot(){

  vector<double> m_WRs = {
400, 400, 1000, 1000, 1000, 3400, 3400, 3400, 6400, 6400, 6400, 
  };
  vector<double> m_Ns = {
100, 300, 100, 400, 900, 100, 2200, 3300, 100, 3400, 6300, 
  };

  maps_WR_to_N.clear();
  maps_N_to_WR.clear();
  NTotalMass = 0;

  for(unsigned int i=0; i<m_WRs.size(); i++){

    double m_WR = m_WRs.at(i);
    double m_N = m_Ns.at(i);

    maps_WR_to_N[m_WR].push_back( m_N );
    maps_N_to_WR[m_N].push_back( m_WR );
    NTotalMass++;

  }

}

void LRSMSignalInfo::GetMassMapsSigDist(){

  vector<double> m_WRs = {
400, 400, 1000, 1000, 1000, 1600, 1600, 1600, 1600, 2200, 2200, 2200, 2200, 2200, 2800, 2800, 2800, 2800, 2800, 3400, 3400, 3400, 3400, 3400, 4000, 4000, 4000, 4000, 4000, 4600, 4600, 4600, 4600, 4600, 5200, 5200, 5200, 5200, 5200, 5800, 5800, 5800, 5800, 5800, 6400, 6400, 6400, 6400, 6400, 7000, 7000, 7000, 7000, 7000, 
  };
  vector<double> m_Ns = {
100, 300, 100, 400, 900, 100, 400, 1000, 1500, 100, 400, 1000, 1600, 2100, 100, 400, 1000, 1600, 2700, 100, 400, 1000, 1600, 3300, 100, 400, 1000, 1600, 3900, 100, 400, 1000, 1600, 4500, 100, 400, 1000, 1600, 5100, 100, 400, 1000, 1600, 5700, 100, 400, 1000, 1600, 6300, 100, 400, 1000, 1600, 6900, 
  };

  maps_WR_to_N.clear();
  maps_N_to_WR.clear();
  NTotalMass = 0;

  for(unsigned int i=0; i<m_WRs.size(); i++){

    double m_WR = m_WRs.at(i);
    double m_N = m_Ns.at(i);

    maps_WR_to_N[m_WR].push_back( m_N );
    maps_N_to_WR[m_N].push_back( m_WR );
    NTotalMass++;

  }

}

bool LRSMSignalInfo::operator== (LRSMSignalInfo b){

  if( this->prod_channel!=b.prod_channel ) return false;
  if( this->lep_channel!=b.lep_channel ) return false;
  if( this->generator!=b.generator ) return false;
  if( this->mass_WR!=b.mass_WR ) return false;
  if( this->mass_Z!=b.mass_Z ) return false;
  if( this->mass_N!=b.mass_N ) return false;

  return true;

}

void LRSMSignalInfo::Print(){

  cout << "[" << GetLegendAlias() << "]" <<  endl;
  cout << "Xsec = " << xsec << endl;
  for(unsigned int i=0; i<LimitResults.size(); i++){
    LimitResult m = LimitResults.at(i);
    cout << "  " << m.region << "\t" << m.limit_exp_2sdDn << "\t" << m.limit_exp_1sdDn << "\t" << m.limit_exp << "\t" << m.limit_exp_1sdUp << "\t" << m.limit_exp_2sdUp << endl;
  }

}

#endif