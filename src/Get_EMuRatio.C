#include "canvas_margin.h"
#include "mylib.h"

void Get_EMuRatio(int xxx=0){

  bool DrawCompPlot = false;

  setTDRStyle();

  TString Year = "2016";
  if(xxx==1) Year = "2017";

  gStyle->SetOptStat(0);

  TH1::SetDefaultSumw2(true);
  TH1::AddDirectory(kFALSE);

  TString WORKING_DIR = getenv("PLOTTER_WORKING_DIR");
  TString dataset = getenv("CATANVERSION");
  TString ENV_PLOT_PATH = getenv("PLOT_PATH");

  TString base_filepath = WORKING_DIR+"/rootfiles/"+dataset+"/Regions/"+Year+"/";
  TString base_plotpath = ENV_PLOT_PATH+"/"+dataset+"/EMuRatio/"+Year+"/";

  if( !gSystem->mkdir(base_plotpath+"/Comparison/", kTRUE) ){
    cout
    << "###################################################" << endl
    << "Directoy " << base_plotpath << " is created" << endl
    << "###################################################" << endl
    << endl;
  }

  //==== MCFR Samples

  vector<TString> sym_samples, asym_samples;

  if(Year=="2016"){

    sym_samples = {
      "TTLL_powheg",
      "TTLJ_powheg",
      "TTLX_powheg",
    };

    asym_samples = {
"DYJets",
"DYJets10to50",
"WJets_MG",
"WW_pythia",
"WZ_pythia",
"ZZ_pythia",
    };

  }

  if(Year=="2017"){

    sym_samples = {
      "TTLL_powheg",
      "TTLJ_powheg",
      "TTLX_powheg",
    };

    asym_samples = {
"DYJets",
"DYJets10to50_MG",
"TTG",
"TTLJ_powheg", "TTJJ_powheg",
"WJets_MG",
"WWW",
"WWZ",
"WW_pythia",
"WZZ",
"WZ_pythia",
"ZZZ",
"ZZ_pythia",
"ttW",
"ttZ",
    };

  }

  //==== Variables

  vector<TString> vars = {
    "NEvent",
    "ZCand_Mass",
    "WRCand_Mass",
  };

  vector<TString> xtitles = {
    "# of events",
    "m(ll) (GeV)",
    "m(W_{R}) (GeV)",
  };

  vector<int> rebins = {
    1,
    100,
    -1,
  };

  vector<TString> vars_to_draw = {
    "NEvent", "nPileUp", "nPV",
    "Lepton_0_Pt", "Lepton_0_Eta", "Lepton_0_TrkRelIso",
    "Lepton_1_Pt", "Lepton_1_Eta", "Lepton_1_TrkRelIso",
    "dPhi_ll",
    "Jet_0_Pt", "Jet_0_Eta",
    "Jet_1_Pt", "Jet_1_Eta",
    "HNFatJet_Pt", "HNFatJet_Eta", "HNFatJet_Mass", "HNFatJet_SDMass",
    "ZCand_Pt", "ZCand_Mass", "dR_ll",
    "MET", "HT",
    "MT",
    "Jet_Size", "NBJets",
    "NCand_Mass", "WRCand_Mass",
    "NCand_Pt", "WRCand_Pt",
    "LSFFatJet_Size",
  };

  TFile *file_TTLL = new TFile(base_filepath+"/HNWRAnalyzer_TTLL_powheg.root");
  TFile *file_TTLJ = new TFile(base_filepath+"/HNWRAnalyzer_TTLJ_powheg.root");

  //==== First, obtain the ee,mm / em ratio from MC

  for(unsigned int it_sym_sample=0; it_sym_sample<sym_samples.size(); it_sym_sample++){

    TString sym_sample = sym_samples.at(it_sym_sample);

    TFile *file = new TFile(base_filepath+"/HNWRAnalyzer_"+sym_sample+".root");

    vector< vector<double> > ratios_for_each_SR;

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      for(unsigned int it_var=0; it_var<vars.size(); it_var++){

        TString var = vars.at(it_var);
        TString xtitle = xtitles.at(it_var);
        int rebin = rebins.at(it_var);

        TString region_EE = "HNWR_SingleElectron_Resolved_SR";
        TString region_MM = "HNWR_SingleMuon_Resolved_SR";
        TString region_EM = "HNWR_EMu_Resolved_SR";

        if(it_SR==1){
          region_EE = "HNWR_SingleElectron_Boosted_SR"; // isolated el + elJet
          region_MM = "HNWR_SingleElectron_Boosted_SR"; // dummy
          region_EM = "HNWR_SingleMuon_EMu_Boosted_CR"; // isolated mu + elJet
        }
        if(it_SR==2){
          region_EE = "HNWR_SingleMuon_Boosted_SR"; // dummy
          region_MM = "HNWR_SingleMuon_Boosted_SR"; // isolated mu + muJet
          region_EM = "HNWR_SingleElectron_EMu_Boosted_CR"; // isolated el + elJet
        }

        TH1D *hist_EE = (TH1D *)file->Get(region_EE+"/"+var+"_"+region_EE);
        TH1D *hist_MM = (TH1D *)file->Get(region_MM+"/"+var+"_"+region_MM);
        TH1D *hist_EM = (TH1D *)file->Get(region_EM+"/"+var+"_"+region_EM);

        if(rebin>0){
          hist_EE->Rebin(rebin);
          hist_MM->Rebin(rebin);
          hist_EM->Rebin(rebin);
        }
        else{

          if(var=="WRCand_Mass"){
            vector<double> vec_bins;
            if(it_SR==0) vec_bins = {0, 200, 400, 600, 800, 1000, 1500, 2000, 8000};
            else vec_bins = {0, 200, 400, 600, 800, 1000, 8000};
            const int n_bin = vec_bins.size()-1;
            double ptArray[n_bin+1];
            for(int zzz=0;zzz<vec_bins.size();zzz++) ptArray[zzz] = vec_bins.at(zzz);
            hist_EE = (TH1D *)hist_EE->Rebin(n_bin, hist_EE->GetName(), ptArray);
            hist_MM = (TH1D *)hist_MM->Rebin(n_bin, hist_MM->GetName(), ptArray);
            hist_EM = (TH1D *)hist_EM->Rebin(n_bin, hist_EM->GetName(), ptArray);

          }

        }

        hist_EE->Divide(hist_EM);
        hist_MM->Divide(hist_EM);

        TCanvas *c1 = new TCanvas("c1", "", 600, 600);
        canvas_margin(c1);
        c1->cd();

        hist_EE->SetLineColor(kRed);
        hist_MM->SetLineColor(kBlue);
        hist_EE->SetLineWidth(3);
        hist_MM->SetLineWidth(3);

        if(it_SR==0){
          hist_EE->Draw("histe1");
          hist_axis(hist_EE);
          hist_MM->Draw("histsamee1");

          hist_EE->GetYaxis()->SetRangeUser(0., 1.5);
          hist_EE->GetXaxis()->SetTitle(xtitle);
          hist_EE->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_EE, "ee/e#mu", "l");
          lg->AddEntry(hist_MM, "#mu#mu/e#mu", "l");
          lg->Draw();
        }
        else if(it_SR==1){
          hist_EE->Draw("histe1");
          hist_axis(hist_EE);

          hist_EE->GetYaxis()->SetRangeUser(0., 1.5);
          hist_EE->GetXaxis()->SetTitle(xtitle);
          hist_EE->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_EE, "ee/e#mu", "l");
          lg->Draw();
        }
        else if(it_SR==2){
          hist_MM->Draw("histe1");
          hist_axis(hist_MM);
        
          hist_MM->GetYaxis()->SetRangeUser(0., 1.5);
          hist_MM->GetXaxis()->SetTitle(xtitle);
          hist_MM->GetYaxis()->SetTitle("Ratio");
          TLegend *lg = new TLegend(0.6, 0.8, 0.9, 0.9);
          lg->AddEntry(hist_MM, "#mu#mu/e#mu", "l");
          lg->Draw();
        }

        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".pdf");
        c1->SaveAs(base_plotpath+"/Ratios_"+SR+"_"+var+"_"+sym_sample+".png");

        if(var=="NEvent"){
          cout << sym_sample << "\t" << SR << "\t" << hist_EE->GetBinContent(1) << "\t" << hist_MM->GetBinContent(1) << endl;

          vector<double> ratios = { hist_EE->GetBinContent(1), hist_MM->GetBinContent(1) };
          ratios_for_each_SR.push_back( ratios );

        }

        c1->Close();

      } // END variable loop


    }

    //==== Ratio obtained with this sym_sample
    //==== Now make shape

    TFile *outfile = new TFile(base_filepath+"/HNWRAnalyzer_EMuMethod_"+sym_sample+".root","RECREATE");

    for(int it_SR=0; it_SR<3; it_SR++){

      TString SR = "Resolved";
      if(it_SR==1) SR = "Boosted_elFatJet";
      if(it_SR==2) SR = "Boosted_muFatJet";

      vector<double> ratios = ratios_for_each_SR.at(it_SR);

      cout << "@@@@ Making " << SR << " plots.." << endl;

      TString filename_DataEMu = "HNWRAnalyzer_data_SingleMuon.root";
      TString region_EE = "HNWR_SingleElectron_Resolved_SR";
      TString region_MM = "HNWR_SingleMuon_Resolved_SR";
      TString region_EM = "HNWR_EMu_Resolved_SR";

      if(it_SR==1){
        filename_DataEMu = "HNWRAnalyzer_data_SingleMuon.root";
        region_EE = "HNWR_SingleElectron_Boosted_SR"; // isolated el + elJet
        region_MM = "HNWR_SingleElectron_Boosted_SR"; // dummy
        region_EM = "HNWR_SingleMuon_EMu_Boosted_CR"; // isolated mu + elJet
      }
      if(it_SR==2){
        filename_DataEMu = "HNWRAnalyzer_data_SingleElectron.root";
        region_EE = "HNWR_SingleMuon_Boosted_SR"; // dummy
        region_MM = "HNWR_SingleMuon_Boosted_SR"; // isolated mu + muJet
        region_EM = "HNWR_SingleElectron_EMu_Boosted_CR"; // isolated el + elJet
      }

      TFile *file_EMu = new TFile(base_filepath+"/"+filename_DataEMu);
      if(it_SR==0){
        outfile->mkdir(region_EE);
        outfile->mkdir(region_MM);
      }
      if(it_SR==1){
        outfile->mkdir(region_EE);
      }
      if(it_SR==2){
        outfile->mkdir(region_MM);
      }

      for(unsigned it_var=0; it_var<vars_to_draw.size(); it_var++){

        TString var = vars_to_draw.at(it_var);

        TH1D *hist_EMu = (TH1D *)file_EMu->Get(region_EM+"/"+var+"_"+region_EM);
        if(!hist_EMu) continue;

        int n_rebin = 1;
        double x_min = -9999;
        double x_max = 9999;

        if(var=="WRCand_Mass"){
          n_rebin = 100;
        }

        //==== Subtract non-TT
        for(unsigned int it_asym=0; it_asym<asym_samples.size(); it_asym++){
          TFile *file_asym = new TFile(base_filepath+"/HNWRAnalyzer_"+asym_samples.at(it_asym)+".root");
          TH1D *hist_asym = (TH1D *)file_asym->Get(region_EM+"/"+var+"_"+region_EM);
          if(!hist_asym){
            file_asym->Close();
            continue;
          }
          hist_EMu->Add(hist_asym,-1.);
          file_asym->Close();
        }

        if(it_SR==0){
          TH1D *hist_Pred_EE = (TH1D *)hist_EMu->Clone();
          TH1D *hist_Pred_MM = (TH1D *)hist_EMu->Clone();

          hist_Pred_EE->Scale(ratios.at(0));
          hist_Pred_EE->SetName(var+"_"+region_EE);
          outfile->cd(region_EE);
          hist_Pred_EE->Write();
          outfile->cd();

          hist_Pred_MM->Scale(ratios.at(1));
          hist_Pred_MM->SetName(var+"_"+region_MM);
          outfile->cd(region_MM);
          hist_Pred_MM->Write();
          outfile->cd();


          //==== Make comparison plot

          if(!DrawCompPlot) continue;

          //==== 1) EE

          //==== observed (cloneed from TTLL)
          TH1D *hist_Obs_EE = (TH1D *)file_TTLL->Get(region_EE+"/"+var+"_"+region_EE);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_EE){

            //==== Add TTLJ
            TH1D *hist_TTLJ_EE = (TH1D *)file_TTLJ->Get(region_EE+"/"+var+"_"+region_EE);
            if(hist_TTLJ_EE) hist_Obs_EE->Add(hist_TTLJ_EE);

            //==== rebin
            hist_Pred_EE->Rebin(n_rebin);
            hist_Obs_EE->Rebin(n_rebin);

            //==== canvas
            TCanvas *c_comp_EE = new TCanvas("c_comp_EE", "", 800, 800);
            TPad *c1_up_EE;
            TPad *c1_down_EE;
            c1_up_EE = new TPad("c1_EE", "", 0, 0.25, 1, 1);
            c1_down_EE = new TPad("c1_down_EE", "", 0, 0, 1, 0.25);
            canvas_margin(c_comp_EE, c1_up_EE, c1_down_EE);

            c1_up_EE->Draw();
            c1_down_EE->Draw();

            //==== up

            c1_up_EE->cd();
            c1_up_EE->SetLogy();

            //==== Pred
            hist_Pred_EE->SetFillColor(kOrange);
            hist_Pred_EE->SetLineColor(kOrange);
            hist_Pred_EE->Draw("hist");

            TH1D *hist_Pred_err_EE = (TH1D *)hist_Pred_EE->Clone();
            hist_Pred_err_EE->SetMarkerColor(0);
            hist_Pred_err_EE->SetMarkerSize(0);
            hist_Pred_err_EE->SetFillStyle(3013);
            hist_Pred_err_EE->SetFillColor(kBlack);
            hist_Pred_err_EE->SetLineColor(0);
            hist_Pred_err_EE->Draw("sameE2");

            hist_Pred_EE->GetYaxis()->SetRangeUser(1., 10.*max( GetMaximum(hist_Pred_EE), GetMaximum(hist_Obs_EE) ) );

            //==== Obs
            hist_Obs_EE->SetMarkerStyle(20);
            hist_Obs_EE->SetMarkerSize(1.2);

            TGraphAsymmErrors *gr_Obs_EE = new TGraphAsymmErrors(hist_Obs_EE);
            for(int i=0; i<gr_Obs_EE->GetN(); ++i){
              gr_Obs_EE->SetPointEXlow(i, 0);
              gr_Obs_EE->SetPointEXhigh(i, 0);
            }
            gr_Obs_EE->SetLineWidth(2.0);
            gr_Obs_EE->SetMarkerSize(0.);
            gr_Obs_EE->SetMarkerColor(kBlack);
            gr_Obs_EE->SetLineColor(kBlack);
            hist_Obs_EE->Draw("phistsame");
            gr_Obs_EE->Draw("p0same");


            //==== down

            c1_down_EE->cd();

            TH1D *tmp_ratio_point_EE = (TH1D *)hist_Obs_EE->Clone();
            tmp_ratio_point_EE->Divide(hist_Pred_EE);
            TGraphAsymmErrors *gr_ratio_point_EE = new TGraphAsymmErrors(tmp_ratio_point_EE);
            gr_ratio_point_EE->SetLineWidth(2.0);
            gr_ratio_point_EE->SetMarkerSize(0.);
            gr_ratio_point_EE->SetLineColor(kBlack);

            TH1D *hist_empty_bottom_EE = (TH1D *)hist_Obs_EE->Clone();
            hist_empty_bottom_EE->GetYaxis()->SetRangeUser(0.,2.0);
            hist_empty_bottom_EE->SetNdivisions(504,"Y");
            hist_empty_bottom_EE->GetYaxis()->SetRangeUser(0.5,1.5);
            hist_empty_bottom_EE->GetXaxis()->SetTitle(var);
            hist_empty_bottom_EE->GetYaxis()->SetTitle("#frac{Obs.}{Pred.}");
            hist_empty_bottom_EE->SetFillColor(0);
            hist_empty_bottom_EE->SetFillStyle(0);
            hist_empty_bottom_EE->SetMarkerSize(0);
            hist_empty_bottom_EE->SetMarkerStyle(0);
            hist_empty_bottom_EE->SetLineColor(kWhite);
            hist_empty_bottom_EE->Draw("axis");
            hist_axis(hist_Pred_EE, hist_empty_bottom_EE);

            TH1D *hist_ratio_EE = (TH1D *)hist_Obs_EE->Clone();
            TH1D *hist_ratio_err_EE = (TH1D *)hist_Obs_EE->Clone();
            for(int i=1; i<=hist_ratio_EE->GetXaxis()->GetNbins(); i++){
              //==== FIXME for zero? how?
              if(hist_Pred_EE->GetBinContent(i)!=0){

                //==== ratio point
                //==== BinContent = Data/Bkgd
                //==== BinError = DataError/Bkgd
                hist_ratio_EE->SetBinContent( i, hist_ratio_EE->GetBinContent(i) / hist_Pred_EE->GetBinContent(i) );
                hist_ratio_EE->SetBinError ( i, hist_ratio_EE->GetBinError(i) / hist_Pred_EE->GetBinContent(i) );
                gr_ratio_point_EE->SetPointEXlow(i-1, 0);
                gr_ratio_point_EE->SetPointEXhigh(i-1, 0);

                hist_ratio_err_EE->SetBinContent( i, 1.);
                hist_ratio_err_EE->SetBinError( i, hist_Pred_EE->GetBinError(i) / hist_Pred_EE->GetBinContent(i) );


              }
              else{
                hist_ratio_EE->SetBinContent( i, 0 );
                hist_ratio_EE->SetBinError ( i, 0 );
                gr_ratio_point_EE->SetPoint(i-1, 0, 0);
                gr_ratio_point_EE->SetPointEYlow(i-1, 0);
                gr_ratio_point_EE->SetPointEXlow(i-1, 0);
                gr_ratio_point_EE->SetPointEYhigh(i-1, 0);
                gr_ratio_point_EE->SetPointEXhigh(i-1, 0);
                hist_ratio_err_EE->SetBinContent( i, 1.);
                hist_ratio_err_EE->SetBinError( i, 0.);
              }
            }

            hist_ratio_err_EE->SetFillColor(kGray);
            hist_ratio_err_EE->SetMarkerSize(0);
            hist_ratio_err_EE->SetMarkerStyle(0);
            hist_ratio_err_EE->SetLineColor(kWhite);
            hist_ratio_err_EE->Draw("E2same");

            hist_ratio_EE->Draw("p9histsame");
            gr_ratio_point_EE->Draw("p0same");

            c_comp_EE->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_EE+"_"+sym_sample+".pdf");
            c_comp_EE->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_EE+"_"+sym_sample+".png");
            c_comp_EE->Close();
          }

          //==== 2) MM

          //==== observed (clonMMd from TTLL)
          TH1D *hist_Obs_MM = (TH1D *)file_TTLL->Get(region_MM+"/"+var+"_"+region_MM);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_MM){

            //==== Add TTLJ
            TH1D *hist_TTLJ_MM = (TH1D *)file_TTLJ->Get(region_MM+"/"+var+"_"+region_MM);
            if(hist_TTLJ_MM) hist_Obs_MM->Add(hist_TTLJ_MM);

            //==== rebin
            hist_Pred_MM->Rebin(n_rebin);
            hist_Obs_MM->Rebin(n_rebin);

            //==== canvas
            TCanvas *c_comp_MM = new TCanvas("c_comp_MM", "", 800, 800);
            TPad *c1_up_MM;
            TPad *c1_down_MM;
            c1_up_MM = new TPad("c1_MM", "", 0, 0.25, 1, 1);
            c1_down_MM = new TPad("c1_down_MM", "", 0, 0, 1, 0.25);
            canvas_margin(c_comp_MM, c1_up_MM, c1_down_MM);

            c1_up_MM->Draw();
            c1_down_MM->Draw();

            //==== up

            c1_up_MM->cd();
            c1_up_MM->SetLogy();

            //==== Pred
            hist_Pred_MM->SetFillColor(kOrange);
            hist_Pred_MM->SetLineColor(kOrange);
            hist_Pred_MM->Draw("hist");

            TH1D *hist_Pred_err_MM = (TH1D *)hist_Pred_MM->Clone();
            hist_Pred_err_MM->SetMarkerColor(0);
            hist_Pred_err_MM->SetMarkerSize(0);
            hist_Pred_err_MM->SetFillStyle(3013);
            hist_Pred_err_MM->SetFillColor(kBlack);
            hist_Pred_err_MM->SetLineColor(0);
            hist_Pred_err_MM->Draw("sameE2");

            hist_Pred_MM->GetYaxis()->SetRangeUser(1., 10.*max( GetMaximum(hist_Pred_MM), GetMaximum(hist_Obs_MM) ) );

            //==== Obs
            hist_Obs_MM->SetMarkerStyle(20);
            hist_Obs_MM->SetMarkerSize(1.2);

            TGraphAsymmErrors *gr_Obs_MM = new TGraphAsymmErrors(hist_Obs_MM);
            for(int i=0; i<gr_Obs_MM->GetN(); ++i){
              gr_Obs_MM->SetPointEXlow(i, 0);
              gr_Obs_MM->SetPointEXhigh(i, 0);
            }
            gr_Obs_MM->SetLineWidth(2.0);
            gr_Obs_MM->SetMarkerSize(0.);
            gr_Obs_MM->SetMarkerColor(kBlack);
            gr_Obs_MM->SetLineColor(kBlack);
            hist_Obs_MM->Draw("phistsame");
            gr_Obs_MM->Draw("p0same");


            //==== down

            c1_down_MM->cd();

            TH1D *tmp_ratio_point_MM = (TH1D *)hist_Obs_MM->Clone();
            tmp_ratio_point_MM->Divide(hist_Pred_MM);
            TGraphAsymmErrors *gr_ratio_point_MM = new TGraphAsymmErrors(tmp_ratio_point_MM);
            gr_ratio_point_MM->SetLineWidth(2.0);
            gr_ratio_point_MM->SetMarkerSize(0.);
            gr_ratio_point_MM->SetLineColor(kBlack);

            TH1D *hist_empty_bottom_MM = (TH1D *)hist_Obs_MM->Clone();
            hist_empty_bottom_MM->GetYaxis()->SetRangeUser(0.,2.0);
            hist_empty_bottom_MM->SetNdivisions(504,"Y");
            hist_empty_bottom_MM->GetXaxis()->SetTitle(var);
            hist_empty_bottom_MM->GetYaxis()->SetTitle("#frac{Obs.}{Pred.}");
            hist_empty_bottom_MM->SetFillColor(0);
            hist_empty_bottom_MM->SetFillStyle(0);
            hist_empty_bottom_MM->SetMarkerSize(0);
            hist_empty_bottom_MM->SetMarkerStyle(0);
            hist_empty_bottom_MM->SetLineColor(kWhite);
            hist_empty_bottom_MM->Draw("axis");
            hist_axis(hist_Pred_MM, hist_empty_bottom_MM);

            TH1D *hist_ratio_MM = (TH1D *)hist_Obs_MM->Clone();
            TH1D *hist_ratio_err_MM = (TH1D *)hist_Obs_MM->Clone();
            for(int i=1; i<=hist_ratio_MM->GetXaxis()->GetNbins(); i++){
              //==== FIXME for zero? how?
              if(hist_Pred_MM->GetBinContent(i)!=0){

                //==== ratio point
                //==== BinContent = Data/Bkgd
                //==== BinError = DataError/Bkgd
                hist_ratio_MM->SetBinContent( i, hist_ratio_MM->GetBinContent(i) / hist_Pred_MM->GetBinContent(i) );
                hist_ratio_MM->SetBinError ( i, hist_ratio_MM->GetBinError(i) / hist_Pred_MM->GetBinContent(i) );
                gr_ratio_point_MM->SetPointEXlow(i-1, 0);
                gr_ratio_point_MM->SetPointEXhigh(i-1, 0);

                hist_ratio_err_MM->SetBinContent( i, 1.);
                hist_ratio_err_MM->SetBinError( i, hist_Pred_MM->GetBinError(i) / hist_Pred_MM->GetBinContent(i) );


              }
              else{
                hist_ratio_MM->SetBinContent( i, 0 );
                hist_ratio_MM->SetBinError ( i, 0 );
                gr_ratio_point_MM->SetPoint(i-1, 0, 0);
                gr_ratio_point_MM->SetPointEYlow(i-1, 0);
                gr_ratio_point_MM->SetPointEXlow(i-1, 0);
                gr_ratio_point_MM->SetPointEYhigh(i-1, 0);
                gr_ratio_point_MM->SetPointEXhigh(i-1, 0);
                hist_ratio_err_MM->SetBinContent( i, 1.);
                hist_ratio_err_MM->SetBinError( i, 0.);
              }
            }

            hist_ratio_err_MM->SetFillColor(kGray);
            hist_ratio_err_MM->SetMarkerSize(0);
            hist_ratio_err_MM->SetMarkerStyle(0);
            hist_ratio_err_MM->SetLineColor(kWhite);
            hist_ratio_err_MM->Draw("E2same");

            hist_ratio_MM->Draw("p9histsame");
            gr_ratio_point_MM->Draw("p0same");

            c_comp_MM->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_MM+"_"+sym_sample+".pdf");
            c_comp_MM->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_MM+"_"+sym_sample+".png");
            c_comp_MM->Close();
          }

        }
        if(it_SR==1){
          TH1D *hist_Pred_EE = (TH1D *)hist_EMu->Clone();

          hist_Pred_EE->Scale(ratios.at(0));
          hist_Pred_EE->SetName(var+"_"+region_EE);
          outfile->cd(region_EE);
          hist_Pred_EE->Write();
          outfile->cd();

          //==== Make comparison plot

          if(!DrawCompPlot) continue;

          //==== 1) EE

          //==== observed (cloneed from TTLL)
          TH1D *hist_Obs_EE = (TH1D *)file_TTLL->Get(region_EE+"/"+var+"_"+region_EE);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_EE){

            //==== Add TTLJ
            TH1D *hist_TTLJ_EE = (TH1D *)file_TTLJ->Get(region_EE+"/"+var+"_"+region_EE);
            if(hist_TTLJ_EE) hist_Obs_EE->Add(hist_TTLJ_EE);

            //==== rebin
            hist_Pred_EE->Rebin(n_rebin);
            hist_Obs_EE->Rebin(n_rebin);

            //==== canvas
            TCanvas *c_comp_EE = new TCanvas("c_comp_EE", "", 800, 800);
            TPad *c1_up_EE;
            TPad *c1_down_EE;
            c1_up_EE = new TPad("c1_EE", "", 0, 0.25, 1, 1);
            c1_down_EE = new TPad("c1_down_EE", "", 0, 0, 1, 0.25);
            canvas_margin(c_comp_EE, c1_up_EE, c1_down_EE);

            c1_up_EE->Draw();
            c1_down_EE->Draw();

            //==== up

            c1_up_EE->cd();
            c1_up_EE->SetLogy();

            //==== Pred
            hist_Pred_EE->SetFillColor(kOrange);
            hist_Pred_EE->SetLineColor(kOrange);
            hist_Pred_EE->Draw("hist");

            TH1D *hist_Pred_err_EE = (TH1D *)hist_Pred_EE->Clone();
            hist_Pred_err_EE->SetMarkerColor(0);
            hist_Pred_err_EE->SetMarkerSize(0);
            hist_Pred_err_EE->SetFillStyle(3013);
            hist_Pred_err_EE->SetFillColor(kBlack);
            hist_Pred_err_EE->SetLineColor(0);
            hist_Pred_err_EE->Draw("sameE2");

            hist_Pred_EE->GetYaxis()->SetRangeUser(1., 10.*max( GetMaximum(hist_Pred_EE), GetMaximum(hist_Obs_EE) ) );

            //==== Obs
            hist_Obs_EE->SetMarkerStyle(20);
            hist_Obs_EE->SetMarkerSize(1.2);

            TGraphAsymmErrors *gr_Obs_EE = new TGraphAsymmErrors(hist_Obs_EE);
            for(int i=0; i<gr_Obs_EE->GetN(); ++i){
              gr_Obs_EE->SetPointEXlow(i, 0);
              gr_Obs_EE->SetPointEXhigh(i, 0);
            }
            gr_Obs_EE->SetLineWidth(2.0);
            gr_Obs_EE->SetMarkerSize(0.);
            gr_Obs_EE->SetMarkerColor(kBlack);
            gr_Obs_EE->SetLineColor(kBlack);
            hist_Obs_EE->Draw("phistsame");
            gr_Obs_EE->Draw("p0same");


            //==== down

            c1_down_EE->cd();

            TH1D *tmp_ratio_point_EE = (TH1D *)hist_Obs_EE->Clone();
            tmp_ratio_point_EE->Divide(hist_Pred_EE);
            TGraphAsymmErrors *gr_ratio_point_EE = new TGraphAsymmErrors(tmp_ratio_point_EE);
            gr_ratio_point_EE->SetLineWidth(2.0);
            gr_ratio_point_EE->SetMarkerSize(0.);
            gr_ratio_point_EE->SetLineColor(kBlack);

            TH1D *hist_empty_bottom_EE = (TH1D *)hist_Obs_EE->Clone();
            hist_empty_bottom_EE->GetYaxis()->SetRangeUser(0.,2.0);
            hist_empty_bottom_EE->SetNdivisions(504,"Y");
            hist_empty_bottom_EE->GetYaxis()->SetRangeUser(0.5,1.5);
            hist_empty_bottom_EE->GetXaxis()->SetTitle(var);
            hist_empty_bottom_EE->GetYaxis()->SetTitle("#frac{Obs.}{Pred.}");
            hist_empty_bottom_EE->SetFillColor(0);
            hist_empty_bottom_EE->SetFillStyle(0);
            hist_empty_bottom_EE->SetMarkerSize(0);
            hist_empty_bottom_EE->SetMarkerStyle(0);
            hist_empty_bottom_EE->SetLineColor(kWhite);
            hist_empty_bottom_EE->Draw("axis");
            hist_axis(hist_Pred_EE, hist_empty_bottom_EE);

            TH1D *hist_ratio_EE = (TH1D *)hist_Obs_EE->Clone();
            TH1D *hist_ratio_err_EE = (TH1D *)hist_Obs_EE->Clone();
            for(int i=1; i<=hist_ratio_EE->GetXaxis()->GetNbins(); i++){
              //==== FIXME for zero? how?
              if(hist_Pred_EE->GetBinContent(i)!=0){

                //==== ratio point
                //==== BinContent = Data/Bkgd
                //==== BinError = DataError/Bkgd
                hist_ratio_EE->SetBinContent( i, hist_ratio_EE->GetBinContent(i) / hist_Pred_EE->GetBinContent(i) );
                hist_ratio_EE->SetBinError ( i, hist_ratio_EE->GetBinError(i) / hist_Pred_EE->GetBinContent(i) );
                gr_ratio_point_EE->SetPointEXlow(i-1, 0);
                gr_ratio_point_EE->SetPointEXhigh(i-1, 0);

                hist_ratio_err_EE->SetBinContent( i, 1.);
                hist_ratio_err_EE->SetBinError( i, hist_Pred_EE->GetBinError(i) / hist_Pred_EE->GetBinContent(i) );


              }
              else{
                hist_ratio_EE->SetBinContent( i, 0 );
                hist_ratio_EE->SetBinError ( i, 0 );
                gr_ratio_point_EE->SetPoint(i-1, 0, 0);
                gr_ratio_point_EE->SetPointEYlow(i-1, 0);
                gr_ratio_point_EE->SetPointEXlow(i-1, 0);
                gr_ratio_point_EE->SetPointEYhigh(i-1, 0);
                gr_ratio_point_EE->SetPointEXhigh(i-1, 0);
                hist_ratio_err_EE->SetBinContent( i, 1.);
                hist_ratio_err_EE->SetBinError( i, 0.);
              }
            }

            hist_ratio_err_EE->SetFillColor(kGray);
            hist_ratio_err_EE->SetMarkerSize(0);
            hist_ratio_err_EE->SetMarkerStyle(0);
            hist_ratio_err_EE->SetLineColor(kWhite);
            hist_ratio_err_EE->Draw("E2same");

            hist_ratio_EE->Draw("p9histsame");
            gr_ratio_point_EE->Draw("p0same");

            c_comp_EE->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_EE+"_"+sym_sample+".pdf");
            c_comp_EE->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_EE+"_"+sym_sample+".png");
            c_comp_EE->Close();
          }

        }
        if(it_SR==2){
          TH1D *hist_Pred_MM = (TH1D *)hist_EMu->Clone();

          hist_Pred_MM->Scale(ratios.at(1));
          hist_Pred_MM->SetName(var+"_"+region_MM);
          outfile->cd(region_MM);
          hist_Pred_MM->Write();
          outfile->cd();

          //==== Make comparison plot

          if(!DrawCompPlot) continue;

          //==== 1) MM

          //==== observed (clonMMd from TTLL)
          TH1D *hist_Obs_MM = (TH1D *)file_TTLL->Get(region_MM+"/"+var+"_"+region_MM);
          //==== trkiso etc.. sometimes there is no histogram
          if(hist_Obs_MM){

            //==== Add TTLJ
            TH1D *hist_TTLJ_MM = (TH1D *)file_TTLJ->Get(region_MM+"/"+var+"_"+region_MM);
            if(hist_TTLJ_MM) hist_Obs_MM->Add(hist_TTLJ_MM);

            //==== rebin
            hist_Pred_MM->Rebin(n_rebin);
            hist_Obs_MM->Rebin(n_rebin);

            //==== canvas
            TCanvas *c_comp_MM = new TCanvas("c_comp_MM", "", 800, 800);
            TPad *c1_up_MM;
            TPad *c1_down_MM;
            c1_up_MM = new TPad("c1_MM", "", 0, 0.25, 1, 1);
            c1_down_MM = new TPad("c1_down_MM", "", 0, 0, 1, 0.25);
            canvas_margin(c_comp_MM, c1_up_MM, c1_down_MM);

            c1_up_MM->Draw();
            c1_down_MM->Draw();

            //==== up

            c1_up_MM->cd();
            c1_up_MM->SetLogy();

            //==== Pred
            hist_Pred_MM->SetFillColor(kOrange);
            hist_Pred_MM->SetLineColor(kOrange);
            hist_Pred_MM->Draw("hist");

            TH1D *hist_Pred_err_MM = (TH1D *)hist_Pred_MM->Clone();
            hist_Pred_err_MM->SetMarkerColor(0);
            hist_Pred_err_MM->SetMarkerSize(0);
            hist_Pred_err_MM->SetFillStyle(3013);
            hist_Pred_err_MM->SetFillColor(kBlack);
            hist_Pred_err_MM->SetLineColor(0);
            hist_Pred_err_MM->Draw("sameE2");

            hist_Pred_MM->GetYaxis()->SetRangeUser(1., 10.*max( GetMaximum(hist_Pred_MM), GetMaximum(hist_Obs_MM) ) );

            //==== Obs
            hist_Obs_MM->SetMarkerStyle(20);
            hist_Obs_MM->SetMarkerSize(1.2);

            TGraphAsymmErrors *gr_Obs_MM = new TGraphAsymmErrors(hist_Obs_MM);
            for(int i=0; i<gr_Obs_MM->GetN(); ++i){
              gr_Obs_MM->SetPointEXlow(i, 0);
              gr_Obs_MM->SetPointEXhigh(i, 0);
            }
            gr_Obs_MM->SetLineWidth(2.0);
            gr_Obs_MM->SetMarkerSize(0.);
            gr_Obs_MM->SetMarkerColor(kBlack);
            gr_Obs_MM->SetLineColor(kBlack);
            hist_Obs_MM->Draw("phistsame");
            gr_Obs_MM->Draw("p0same");


            //==== down

            c1_down_MM->cd();

            TH1D *tmp_ratio_point_MM = (TH1D *)hist_Obs_MM->Clone();
            tmp_ratio_point_MM->Divide(hist_Pred_MM);
            TGraphAsymmErrors *gr_ratio_point_MM = new TGraphAsymmErrors(tmp_ratio_point_MM);
            gr_ratio_point_MM->SetLineWidth(2.0);
            gr_ratio_point_MM->SetMarkerSize(0.);
            gr_ratio_point_MM->SetLineColor(kBlack);

            TH1D *hist_empty_bottom_MM = (TH1D *)hist_Obs_MM->Clone();
            hist_empty_bottom_MM->GetYaxis()->SetRangeUser(0.,2.0);
            hist_empty_bottom_MM->SetNdivisions(504,"Y");
            hist_empty_bottom_MM->GetXaxis()->SetTitle(var);
            hist_empty_bottom_MM->GetYaxis()->SetTitle("#frac{Obs.}{Pred.}");
            hist_empty_bottom_MM->SetFillColor(0);
            hist_empty_bottom_MM->SetFillStyle(0);
            hist_empty_bottom_MM->SetMarkerSize(0);
            hist_empty_bottom_MM->SetMarkerStyle(0);
            hist_empty_bottom_MM->SetLineColor(kWhite);
            hist_empty_bottom_MM->Draw("axis");
            hist_axis(hist_Pred_MM, hist_empty_bottom_MM);

            TH1D *hist_ratio_MM = (TH1D *)hist_Obs_MM->Clone();
            TH1D *hist_ratio_err_MM = (TH1D *)hist_Obs_MM->Clone();
            for(int i=1; i<=hist_ratio_MM->GetXaxis()->GetNbins(); i++){
              //==== FIXME for zero? how?
              if(hist_Pred_MM->GetBinContent(i)!=0){

                //==== ratio point
                //==== BinContent = Data/Bkgd
                //==== BinError = DataError/Bkgd
                hist_ratio_MM->SetBinContent( i, hist_ratio_MM->GetBinContent(i) / hist_Pred_MM->GetBinContent(i) );
                hist_ratio_MM->SetBinError ( i, hist_ratio_MM->GetBinError(i) / hist_Pred_MM->GetBinContent(i) );
                gr_ratio_point_MM->SetPointEXlow(i-1, 0);
                gr_ratio_point_MM->SetPointEXhigh(i-1, 0);

                hist_ratio_err_MM->SetBinContent( i, 1.);
                hist_ratio_err_MM->SetBinError( i, hist_Pred_MM->GetBinError(i) / hist_Pred_MM->GetBinContent(i) );


              }
              else{
                hist_ratio_MM->SetBinContent( i, 0 );
                hist_ratio_MM->SetBinError ( i, 0 );
                gr_ratio_point_MM->SetPoint(i-1, 0, 0);
                gr_ratio_point_MM->SetPointEYlow(i-1, 0);
                gr_ratio_point_MM->SetPointEXlow(i-1, 0);
                gr_ratio_point_MM->SetPointEYhigh(i-1, 0);
                gr_ratio_point_MM->SetPointEXhigh(i-1, 0);
                hist_ratio_err_MM->SetBinContent( i, 1.);
                hist_ratio_err_MM->SetBinError( i, 0.);
              }
            }

            hist_ratio_err_MM->SetFillColor(kGray);
            hist_ratio_err_MM->SetMarkerSize(0);
            hist_ratio_err_MM->SetMarkerStyle(0);
            hist_ratio_err_MM->SetLineColor(kWhite);
            hist_ratio_err_MM->Draw("E2same");

            hist_ratio_MM->Draw("p9histsame");
            gr_ratio_point_MM->Draw("p0same");

            c_comp_MM->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_MM+"_"+sym_sample+".pdf");
            c_comp_MM->SaveAs(base_plotpath+"/Comparison/"+SR+"_"+var+"_"+region_MM+"_"+sym_sample+".png");
            c_comp_MM->Close();
          }

        } // END if SR2

      } // END vars_to_draw

    } // END Loop SR

    outfile->Close();

  } // END sys_sample loop




}














