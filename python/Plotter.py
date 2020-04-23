import os,ROOT
import mylib
import canvas_margin
import tdrstyle
import CMS_lumi, tdrstyle
import math
from array import array

class SampleGroup:
  def __init__(self, Name="", Type='', Samples=[], Color=0, Style=1, TLatexAlias="", LatexAlias=""):
    self.Name = Name
    self.Type = Type
    self.Samples = Samples
    self.Color = Color
    self.Style = Style
    self.TLatexAlias = TLatexAlias
    self.LatexAlias = LatexAlias
  def Print(self):
    print 'Sample group name = '+self.Name
    print '  Type = '+self.Type
    print '  Samples = ',
    print self.Samples
    print '  Color = '+str(self.Color)
    print '  TLatexAlias = '+str(self.TLatexAlias)
    print '  LatexAlias = '+str(self.LatexAlias)

class Variable:
  def __init__(self, Name, TLatexAlias, Unit):
    self.Name = Name
    self.TLatexAlias = TLatexAlias
    self.Unit = Unit
  def Print(self):
    print '(%s, %s, %s)' % (self.Name, self.TLatexAlias, self.Unit)

class Region:
  def __init__(self, Name, PrimaryDataset, DrawData=True, Logy=-1, TLatexAlias=""):
    self.Name = Name
    self.PrimaryDataset = PrimaryDataset
    self.DrawData = DrawData
    self.Logy = Logy
    self.TLatexAlias = TLatexAlias
  def Print(self):
    print '(%s, %s, DrawData=%s, Logy=%f, %s)'%(self.Name, self.PrimaryDataset, self.DrawData, self.Logy, self.TLatexAlias)

class Plotter:

  def __init__(self):

    self.DataYear = 2016

    self.SampleGroups = []
    self.RegionsToDraw = []
    self.VariablesToDraw = []
    self.Signals = []

    self.Systematics = []
    self.InputDirectory = ""
    self.Filename_prefix = ""
    self.Filename_suffix = ""
    self.Filename_skim = ""
    self.OutputDirectory = ""

    self.ScaleMC = False

  def PrintBorder(self):
    print '--------------------------------------------------------------------------'
  def PrintSamples(self):
    self.PrintBorder()
    print '[Plotter.PrintSamples()] Printing samples'
    for s in self.SampleGroups:
      s.Print()
    self.PrintBorder()

  def PrintRegions(self):
    self.PrintBorder()
    print '[Plotter.PrintRegions()] Printing regions to be drawn'
    for s in self.RegionsToDraw:
      s.Print()
    self.PrintBorder()

  def PrintVariables(self):
    self.PrintBorder()
    print '[Plotter.PrintVariables()] Printing variables to be drawn'
    for s in self.VariablesToDraw:
      s.Print()
    self.PrintBorder()

  def SetBinningFilepath(self, RebinFilepath, XaxisFilepath, YaxisFilepath):
    self.RebinFilepath = RebinFilepath
    self.XaxisFilepath = XaxisFilepath
    self.YaxisFilepath = YaxisFilepath
  def ReadBinningInfo(self, Region):
    ## Rebin
    Rebins = dict()
    for line in open(self.RebinFilepath).readlines():
      words = line.split()
      if Region!=words[0]:
        continue
      Rebins[words[1]] = int(words[2])
    ## xaxis
    XaxisRanges = dict()
    for line in open(self.XaxisFilepath).readlines():
      words = line.split()
      if Region!=words[0]:
        continue
      XaxisRanges[words[1]] = [float(words[2]), float(words[3])]

    return Rebins, XaxisRanges
  def Rebin(self, hist, region, var, nRebin):
    if var=='WRCand_Mass':
      return mylib.RebinWRMass(hist, region, self.DataYear,False)
    else:
      if nRebin>0:
        hist.Rebin(nRebin)
        return hist
      else:
        return hist

  def ZeroDataCheckCut(self,var,xlow,xhigh):
    ## TODO
    return False
  def Draw(self):

    ROOT.gErrorIgnoreLevel = ROOT.kFatal

    tdrstyle.setTDRStyle()
    ROOT.TH1.AddDirectory(False)

    for Region in self.RegionsToDraw:

      print '## Drawing '+Region.Name

      ## Read binning data
      Rebins, XaxisRanges = self.ReadBinningInfo(Region.Name)

      ## Input/Output directotry
      Indir = self.InputDirectory
      Outdir = self.OutputDirectory+'/'+Region.Name+'/'
      if self.ScaleMC:
        Outdir = self.OutputDirectory+'/ScaleMC/'+Region.Name+'/'
      print '##   Outputs => '+Outdir
      os.system('mkdir -p '+Outdir)

      ## Data file
      f_Data = ROOT.TFile(Indir+self.Filename_prefix+self.Filename_skim+'_data_'+Region.PrimaryDataset+self.Filename_suffix+'.root')

      ## Loop over variables

      for Variable in self.VariablesToDraw:

        ## BinInfo
        nRebin = Rebins[Variable.Name]
        xMin = XaxisRanges[Variable.Name][0]
        xMax = XaxisRanges[Variable.Name][1]
        yMax = -999

        ## xtitle
        xtitle = Variable.TLatexAlias
        if Variable.Name=="WRCand_Mass":
          if "Resolved" in Region.Name:
            xtitle = "m_{lljj} (GeV)"
          else:
            xtitle = "m_{lJ} (GeV)"

        ## Save hists
        ## For legend later..
        HistsToDraw = dict()

        ## Get data first
        h_Data = f_Data.Get(Region.Name+'/'+Variable.Name+'_'+Region.Name)

        ## Make overflow
        h_Data.GetXaxis().SetRangeUser(xMin,xMax)
        h_Data = mylib.MakeOverflowBin(h_Data)

        ## Rebin
        h_Data = self.Rebin(h_Data, Region.Name, Variable.Name, nRebin)
        ## Copy data axis
        dataAxis = h_Data.GetXaxis()
        nBin = dataAxis.GetNbins()
        xBins = [dataAxis.GetBinLowEdge(1)]
        for ix in range(0,nBin):
          xBins.append( dataAxis.GetBinUpEdge(ix+1) )
        xBins = array("d",xBins)

        ### Att data histogram
        h_Data.SetMarkerStyle(20)
        h_Data.SetMarkerSize(1.2)
        h_Data.SetMarkerColor(ROOT.kBlack)
        h_Data.SetLineColor(ROOT.kBlack)

        ## Prepare canvas

        c1 = ROOT.TCanvas('c1', '', 800, 800)

        c1_up = ROOT.TPad("c1_up", "", 0, 0.25, 1, 1)
        c1_down = ROOT.TPad("c1_down", "", 0, 0, 1, 0.25)
        c1, c1_up, c1_down = canvas_margin.canvas_margin(c1, c1_up, c1_down)
        c1.Draw()
        c1_up.Draw()
        c1_down.Draw()

        c1_up.cd()
        if Region.Logy>0:
          c1_up.SetLogy(True)

        c1.cd()

        latex_CMSPriliminary = ROOT.TLatex()
        latex_Lumi = ROOT.TLatex()

        latex_CMSPriliminary.SetNDC()
        latex_Lumi.SetNDC()
        latex_CMSPriliminary.SetTextSize(0.035)
        latex_CMSPriliminary.DrawLatex(0.15, 0.96, "#font[62]{CMS} #font[42]{#it{#scale[0.8]{Preliminary}}}")

        latex_Lumi.SetTextSize(0.035)
        latex_Lumi.SetTextFont(42)
        latex_Lumi.DrawLatex(0.73, 0.96, mylib.TotalLumi(float(self.DataYear))+" fb^{-1} (13 TeV)")

        c1_up.cd()
        h_dummy_up = ROOT.TH1D('h_dumy_up', '', nBin, xBins)
        h_dummy_up.GetXaxis().SetRangeUser(xMin, xMax)
        if nRebin>0:
          binsize = h_dummy_up.GetXaxis().GetBinUpEdge(1)-h_dummy_up.GetXaxis().GetBinLowEdge(1)
          str_binsize = '%d'%(binsize)
          if binsize!=int(binsize):
            str_binsize = '%1.2f'%(binsize)
          h_dummy_up.GetYaxis().SetTitle('Events / '+str_binsize+' '+Variable.Unit)
        else:
          h_dummy_up.GetYaxis().SetTitle('Events / bin')
        if Variable.Name=='WRCand_Mass':
          h_dummy_up.GetYaxis().SetTitle('Events / bin')
        h_dummy_up.Draw("histsame")

        c1_down.cd()
        h_dummy_down = ROOT.TH1D('h_dumy_down', '', nBin, xBins)
        h_dummy_down.GetYaxis().SetRangeUser(0.,2.0)
        h_dummy_down.SetNdivisions(504,"Y")
        h_dummy_down.GetXaxis().SetRangeUser(xMin, xMax)
        h_dummy_down.GetXaxis().SetTitle(xtitle)
        h_dummy_down.GetYaxis().SetRangeUser(0.5,1.6)
        h_dummy_down.GetYaxis().SetTitle("#frac{Data}{Sim.}")
        h_dummy_down.SetFillColor(0)
        h_dummy_down.SetMarkerSize(0)
        h_dummy_down.SetMarkerStyle(0)
        h_dummy_down.SetLineColor(ROOT.kWhite)
        h_dummy_down.Draw("axis")

        g1_x = [-9000, 9000]
        g1_y = [1, 1]
        g1 = ROOT.TGraph(2, array("d", g1_x ), array("d", g1_y ))
        g1.Draw("same")

        h_dummy_up, h_dummy_down = canvas_margin.hist_axis(h_dummy_up, h_dummy_down)

        ## Loop over samples
        ## For Legend, save 
        HistsForLegend = []
        stack_Bkgd = ROOT.THStack("stack_Bkgd", "")
        h_Bkgd = 0
        ## Save systematic
        SystematicUps = dict()
        SystematicDowns = dict()

        ## Loop over systematics
        for Syst in self.Systematics:

          h_Bkgd_ForSyst = 0
          dirName = Region.Name

          if Syst!="Central":
            dirName = "Syst_"+Syst+"_"+Region.Name

          for SampleGroup in self.SampleGroups:
            Color = SampleGroup.Color
            LegendAdded = False
            for Sample in SampleGroup.Samples:
              f_Sample = ROOT.TFile(Indir+self.Filename_prefix+self.Filename_skim+'_'+Sample+self.Filename_suffix+'.root')
              h_Sample = f_Sample.Get(dirName+'/'+Variable.Name+'_'+dirName)
              if not h_Sample:
                continue

              ## Make overflow
              h_Sample.GetXaxis().SetRangeUser(xMin,xMax)
              h_Sample = mylib.MakeOverflowBin(h_Sample)

              h_Sample = self.Rebin(h_Sample, Region.Name, Variable.Name, nRebin)
              h_Sample.SetLineColor(Color)
              h_Sample.SetLineWidth(0)
              h_Sample.SetFillColor(Color)

              if Syst=="Central":

                stack_Bkgd.Add(h_Sample)
                if not h_Bkgd:
                  h_Bkgd = h_Sample.Clone()
                else:
                  h_Bkgd.Add(h_Sample)

                HistsToDraw[Sample] = h_Sample.Clone()
                if not LegendAdded:
                  HistsForLegend.append( [HistsToDraw[Sample],SampleGroup.TLatexAlias] )
                  LegendAdded = True

              else:

                if not h_Bkgd_ForSyst:
                  h_Bkgd_ForSyst = h_Sample.Clone()
                else:
                  h_Bkgd_ForSyst.Add(h_Sample)

            ##==>End Sample loop
          ##==>End SampleGroup loop

          if Syst!="Central":
            if Syst.endswith('Up'):
              SystematicUps[Syst[:-2]] = h_Bkgd_ForSyst
            elif Syst.endswith('Down'):
              SystematicDowns[Syst[:-4]] = h_Bkgd_ForSyst
            else:
              print 'WTF is this Syst?? : '+Syst

        ##==>End Syst loop
        #print SystematicUps
        #print SystematicDowns

        ## TODO Corr/Uncorr
        ## Syst Up/Down . Max/Min
        h_Bkgd_TotErr_Max = h_Bkgd.Clone()
        h_Bkgd_TotErr_Min = h_Bkgd.Clone()
        for SystKey in SystematicUps:
          #print 'SystKey = '+SystKey
          h_Up = SystematicUps[SystKey]
          h_Down = SystematicDowns[SystKey]
          for ix in range(0,h_Bkgd.GetXaxis().GetNbins()):
            x_l = h_Bkgd.GetXaxis().GetBinLowEdge(ix+1)
            x_r = h_Bkgd.GetXaxis().GetBinUpEdge(ix+1)
            y_Central = h_Bkgd.GetBinContent(ix+1)
            y_Up = h_Up.GetBinContent(ix+1)
            y_Down = h_Down.GetBinContent(ix+1)
            ## -.
            y_Max = max( max(y_Central,y_Up), y_Down)
            y_Min = min( min(y_Central,y_Up), y_Down)
            #print '[%d,%d] : %f, (Max,Min) = (%f,%f)'%(x_l,x_r,y_Central,y_Up,y_Down)

            ## Update Max
            err_Max_Current = h_Bkgd_TotErr_Max.GetBinError(ix+1)
            err_Max_ToAdd = y_Max-y_Central
            err_Max_Updated = math.sqrt( err_Max_Current*err_Max_Current + err_Max_ToAdd*err_Max_ToAdd )
            h_Bkgd_TotErr_Max.SetBinError(ix+1, err_Max_Updated)
            ## Update Min
            err_Min_Current = h_Bkgd_TotErr_Min.GetBinError(ix+1)
            err_Min_ToAdd = y_Central-y_Min
            err_Min_Updated = math.sqrt( err_Min_Current*err_Min_Current + err_Min_ToAdd*err_Min_ToAdd )
            h_Bkgd_TotErr_Min.SetBinError(ix+1, err_Min_Updated)
        ##==>End Systematic loop

        ## hist.gr
        gr_Bkgd_TotErr = mylib.GetAsymmError(h_Bkgd_TotErr_Max,h_Bkgd_TotErr_Min)

        gr_Data = ROOT.TGraphAsymmErrors(h_Data)

        err_up_tmp = []
        err_down_tmp = []
        alpha = 1. - 0.6827
        for i in range(0, gr_Data.GetN()):
          N = gr_Data.GetY()[i]

          L = 0.                                          if (N==0.) else (ROOT.Math.gamma_quantile(alpha/2.,N,1.))
          U = ( ROOT.Math.gamma_quantile_c(alpha,N+1,1) ) if (N==0.) else (ROOT.Math.gamma_quantile_c(alpha/2.,N+1.,1.))

          if N!=0:
            gr_Data.SetPointEYlow(i, N-L )
            gr_Data.SetPointEYhigh(i, U-N )
            err_down_tmp.append(N-L)
            err_up_tmp.append(U-N)
            if Variable.Name!="WRCand_Mass":
              gr_Data.SetPointEXlow(i, 0)
              gr_Data.SetPointEXhigh(i, 0)
          else:
            zerodata_err_low = 0.1
            zerodata_err_high = 1.8

            xlow = gr_Data.GetX()[i]-gr_Data.GetEXlow()[i]
            xhigh = gr_Data.GetX()[i]+gr_Data.GetEXhigh()[i]
            if self.ZeroDataCheckCut(Variable.Name,xlow,xhigh):
              zerodata_err_low = 0.
              zerodata_err_high = 0.

            gr_Data.SetPointEYlow(i, zerodata_err_low)
            gr_Data.SetPointEYhigh(i, zerodata_err_high)
            err_down_tmp.append(zerodata_err_low)
            err_up_tmp.append(zerodata_err_high)
            if Variable.Name!="WRCand_Mass":
              gr_Data.SetPointEXlow(i, 0)
              gr_Data.SetPointEXhigh(i, 0)

        ## Legend
        lg = 0
        if Region.DrawData:
          ## No signal
          if len(self.Signals)==0:
            lg = ROOT.TLegend(0.60, 0.45, 0.92, 0.90)
          ## With Signal
          else:
            lg = ROOT.TLegend(0.48, 0.46, 0.91, 0.90)
        else:
          lg = ROOT.TLegend(0.50, 0.50, 0.88, 0.90)
        lg.SetBorderSize(0)
        lg.SetFillStyle(0)

        lg.AddEntry(gr_Bkgd_TotErr, "Stat.+syst. uncert.", "f")
        ## dummy graph for legend..
        ## this is because h_Data does not have horizontal error bars,
        ## and gr_data does not have points
        gr_Data_dummy = ROOT.TGraphAsymmErrors(gr_Data)
        gr_Data_dummy.SetMarkerStyle(20)
        gr_Data_dummy.SetMarkerSize(1.2)
        dataLegendGOption="ep"
        if Variable.Name=="WRCand_Mass":
          dataLegendGOption="lpe"
        if Region.DrawData:
          lg.AddEntry(gr_Data_dummy, "Data", dataLegendGOption)
        else:
          lg.AddEntry(gr_Data_dummy, "Total background", dataLegendGOption)
        for i_lg in range(0,len(HistsForLegend)):
          h_lg = HistsForLegend[ len(HistsForLegend)-1-i_lg ][0]
          tlatexaliax = HistsForLegend[ len(HistsForLegend)-1-i_lg ][1]
          lg.AddEntry(h_lg,tlatexaliax,"f")
        c1_up.cd()
        lg.Draw()

        ## Get Y maximum
        yMax = max( yMax, mylib.GetMaximum(gr_Data) )
        yMax = max( yMax, mylib.GetMaximum(gr_Bkgd_TotErr) )
        ## Yaxis range
        yMin = 0.
        yMaxScale = 1.2
        if Region.Logy>0:
          yMaxScale = 10
          yMin = Region.Logy
        h_dummy_up.GetYaxis().SetRangeUser( yMin, yMaxScale*yMax )
        ## Exception control
        if (Variable.Name=="WRCand_Mass") and ("_SR" in Region.Name) and ("EMu" not in Region.Name):
          if ("Region.Name" in Region.Name):
            h_dummy_up.GetYaxis().SetRangeUser( 1E-1, yMaxScale*yMax )
          else:
            h_dummy_up.GetYaxis().SetRangeUser( 1, yMaxScale*yMax )
        if (Variable.Name=="ZCand_Mass") and ("_DYCR" in Region.Name):
          h_dummy_up.GetYaxis().SetRangeUser(10, 1E8)
        if (Variable.Name=="ZCand_Pt") and ("_DYCR" in Region.Name) and ("Boosted" in Region.Name):
          h_dummy_up.GetYaxis().SetRangeUser(10, 2E5)

        ## Draw up
        c1_up.cd()
        stack_Bkgd.Draw("histsame")

        gr_Bkgd_TotErr.SetMarkerColor(0)
        gr_Bkgd_TotErr.SetMarkerSize(0)
        gr_Bkgd_TotErr.SetFillStyle(3013)
        gr_Bkgd_TotErr.SetFillColor(ROOT.kBlack)
        gr_Bkgd_TotErr.SetLineColor(0)
        gr_Bkgd_TotErr.Draw("sameE2")

        gr_Data.SetLineWidth(2)
        gr_Data.SetMarkerSize(0.)
        gr_Data.SetMarkerColor(ROOT.kBlack)
        gr_Data.SetLineColor(ROOT.kBlack)
        h_Data.Draw("phistsame")
        gr_Data.Draw("p0same")

        h_dummy_up.Draw("axissame")

        ## Draw down
        c1_down.cd()

        ## values must be set later
        h_Data_Ratio = h_Data.Clone('h_Data_Ratio')
        ## BinContent set by divide here, but errors must be set later
        tmp_h_Data_Ratio = h_Data.Clone()
        tmp_h_Data_Ratio.Divide(h_Bkgd)
        gr_Data_Ratio = ROOT.TGraphAsymmErrors(tmp_h_Data_Ratio)
        gr_Data_Ratio.SetName('gr_Data_Ratio')
        gr_Data_Ratio.SetLineWidth(2)
        gr_Data_Ratio.SetMarkerSize(0.)
        gr_Data_Ratio.SetLineColor(ROOT.kBlack)
        ## values must be set later, but BinContent will be simply 1
        gr_Bkgd_Ratio = gr_Bkgd_TotErr.Clone('gr_Bkgd_Ratio')

        for i in range(1,h_Data_Ratio.GetXaxis().GetNbins()+1):

          ## FIXME for zero? how?
          if h_Bkgd.GetBinContent(i)!=0:

            ## ratio point
            ## BinContent = Data/Bkgd
            ## BinError = DataError/Bkgd
            h_Data_Ratio.SetBinContent( i, h_Data_Ratio.GetBinContent(i) / h_Bkgd.GetBinContent(i) )
            h_Data_Ratio.SetBinError ( i, h_Data_Ratio.GetBinError(i) / h_Bkgd.GetBinContent(i) )

            if err_down_tmp[i-1]!=0.:
              gr_Data_Ratio.SetPointEYlow(i-1, err_down_tmp[i-1] / h_Bkgd.GetBinContent(i) )
              gr_Data_Ratio.SetPointEYhigh(i-1, err_up_tmp[i-1] / h_Bkgd.GetBinContent(i))
              if Variable.Name!="WRCand_Mass":
                gr_Data_Ratio.SetPointEXlow(i-1, 0)
                gr_Data_Ratio.SetPointEXhigh(i-1, 0)
            else:
              gr_Data_Ratio.SetPointEYlow(i-1, 0)
              gr_Data_Ratio.SetPointEYhigh(i-1, 1.8 / h_Bkgd.GetBinContent(i))
              if Variable.Name!="WRCand_Mass":
                gr_Data_Ratio.SetPointEXlow(i-1, 0)
                gr_Data_Ratio.SetPointEXhigh(i-1, 0)

            ## ratio allerr
            ## BinContent = 1
            ## BinError = Bkgd(Stat+Syst)Error/Bkgd

            gr_Bkgd_Ratio.SetPoint(i-1,h_Bkgd.GetXaxis().GetBinCenter(i), 1.)
            gr_Bkgd_Ratio.SetPointEYhigh( i-1, gr_Bkgd_Ratio.GetErrorYhigh(i-1) / h_Bkgd.GetBinContent(i) )
            gr_Bkgd_Ratio.SetPointEYlow( i-1,  gr_Bkgd_Ratio.GetErrorYlow(i-1) / h_Bkgd.GetBinContent(i) )

          elif h_Bkgd.GetBinContent(i)==0. and h_Data_Ratio.GetBinContent(i)==0.:

            h_Data_Ratio.SetBinContent( i, 0 )
            h_Data_Ratio.SetBinError ( i, 0 )
            gr_Data_Ratio.SetPoint(i-1, 0, 0)
            gr_Data_Ratio.SetPointEYlow(i-1, 0)
            gr_Data_Ratio.SetPointEYhigh(i-1, 0)

            gr_Bkgd_Ratio.SetPoint(i-1,h_Bkgd.GetXaxis().GetBinCenter(i), 1.)
            gr_Bkgd_Ratio.SetPointEYhigh( i-1, 0. )
            gr_Bkgd_Ratio.SetPointEYlow( i-1, 0. )

            if Variable.Name!="WRCand_Mass":
              gr_Data_Ratio.SetPointEXlow(i-1, 0)
              gr_Data_Ratio.SetPointEXhigh(i-1, 0)

          ## If bkgd <= 0
          else:
            this_max_ratio = 20.0
            this_data = h_Data_Ratio.GetBinContent(i)
            this_data_err = h_Data_Ratio.GetBinError(i)

            h_Data_Ratio.SetBinContent( i, this_max_ratio )
            h_Data_Ratio.SetBinError ( i, this_data_err*this_max_ratio/this_data )

            tmp_x = ROOT.Double(0.)
            tmp_y = ROOT.Double(0.)
            gr_Data_Ratio.GetPoint(i-1, tmp_x, tmp_y)
            gr_Data_Ratio.SetPoint(i-1, tmp_x, this_max_ratio)
            gr_Data_Ratio.SetPointEYlow(i-1, err_down_tmp[i-1]*this_max_ratio/this_data)
            gr_Data_Ratio.SetPointEYhigh(i-1, err_up_tmp[i-1]*this_max_ratio/this_data)

            gr_Bkgd_Ratio.SetPoint(i-1,h_Bkgd.GetXaxis().GetBinCenter(i), 1.)
            gr_Bkgd_Ratio.SetPointEYhigh( i-1, 0. )
            gr_Bkgd_Ratio.SetPointEYlow( i-1, 0. )

            if Variable.Name!="WRCand_Mass":
              gr_Data_Ratio.SetPointEXlow(i-1, 0)
              gr_Data_Ratio.SetPointEXhigh(i-1, 0)
        ##==>End bin loop

        gr_Bkgd_Ratio.SetMarkerColor(0)
        gr_Bkgd_Ratio.SetMarkerSize(0)
        gr_Bkgd_Ratio.SetFillStyle(3013)
        gr_Bkgd_Ratio.SetFillColor(ROOT.kBlack)
        gr_Bkgd_Ratio.SetLineColor(0)
        gr_Bkgd_Ratio.Draw("sameE2")

        h_Data_Ratio.Draw("p9histsame")
        gr_Data_Ratio.Draw("p0same")

        ## TLatex
        c1.cd()
        channelname = ROOT.TLatex()
        channelname.SetNDC()
        channelname.SetTextSize(0.037)
        channelname.DrawLatex(0.2, 0.88, Region.TLatexAlias)

        ## Save
        c1.SaveAs(Outdir+Variable.Name+'_'+Region.Name+'.pdf')
        print Variable.Name+'_'+Region.Name+'.pdf ==> Saved'
      

        c1.Close()
        del h_dummy_up
        del h_dummy_down
        del HistsToDraw


      ##==>End Variable loop

      f_Data.Close()
