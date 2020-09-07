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
  void GetMassMapsNoLSFCutPlot();
  void GetMassMapsSigDist();
  void GetMassMapsFitShape();
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

  if(generator=="MGFastSim"){
    if(prod_channel=="SchWR"){

      filename = "WRtoNLtoLLJJ_WR"+TString::Itoa(mass_WR,10)+"_N"+TString::Itoa(mass_N,10);
      legendalias = "(m_{W_{R}}, m_{N}) = ("+TString::Itoa(mass_WR,10)+", "+TString::Itoa(mass_N,10)+") GeV";
      texname = "SignalSchWR"+lep_channel+"WR"+TString::Itoa(mass_WR,10)+"N"+TString::Itoa(mass_N,10);
    }

  }
  else if(generator=="aMCNLO"){
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
//200, 
//400, 400, 400, 
//600, 600, 600, 600, 
800, 800, 800, 800, 800, 
1000, 1000, 1000, 1000, 1000, 1000, 
1200, 1200, 1200, 1200, 1200, 1200, 1200, 
1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 
1600, 1600, 1600, 1600, 1600, 1600, 1600, 1600, 1600, 
1800, 1800, 1800, 1800, 1800, 1800, 1800, 1800, 1800, 1800, 
2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 2000, 
2200, 2200, 2200, 2200, 2200, 2200, 2200, 2200, 2200, 2200, 2200, 2200, 
2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 2400, 
2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 2600, 
2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 2800, 
3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 
3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 
3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 3400, 
3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 3600, 
3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 3800, 
4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 
4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 4200, 
4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 4400, 
4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 4600, 
4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 4800, 
5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 
5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 5200, 
5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 5400, 
///*
5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 5600, 
5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 5800, 
6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 
/*
6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 6200, 
6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 6400, 
6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 6600, 
6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 6800, 
7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 
*/
  };
  vector<double> m_Ns = {
//100, 
//100, 200, 300, 
//100, 200, 400, 500, 
100, 200, 400, 600, 700, 
100, 200, 400, 600, 800, 900, 
100, 200, 400, 600, 800, 1000, 1100, 
100, 200, 400, 600, 800, 1000, 1200, 1300, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 1900, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2100, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2300, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 2900, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3100, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3300, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 3900, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4100, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4300, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 4900, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5100, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5300, 
///*
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 5900, 
/*
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6100, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6300, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6400, 6500, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6400, 6600, 6700, 
100, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000, 2200, 2400, 2600, 2800, 3000, 3200, 3400, 3600, 3800, 4000, 4200, 4400, 4600, 4800, 5000, 5200, 5400, 5600, 5800, 6000, 6200, 6400, 6600, 6800, 6900, 
*/
  };

/*
  m_WRs = {
5200,
  };
  m_Ns = {
2600,
  };
*/

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
5000, 5000,
  };
  vector<double> m_Ns = {
3000, 100,
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

void LRSMSignalInfo::GetMassMapsNoLSFCutPlot(){

  vector<double> m_WRs = {
5000, 5000, 5000,
  };
  vector<double> m_Ns = {
100, 200, 400,
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

void LRSMSignalInfo::GetMassMapsFitShape(){

  vector<double> m_WRs = {
5000, 5000,
  };
  vector<double> m_Ns = {
100, 3000,
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
