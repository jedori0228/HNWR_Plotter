import os,ROOT
import math
from array import array

def AddHistograms(h_original, h_toAdd, option=''):
  if option=="L":

    h_out = h_original.Clone()
    for ix in range(0, h_original.GetXaxis().GetNbins()):
      y_original = h_original.GetBinContent(ix+1)
      e_original = h_original.GetBinError(ix+1)

      y_toAdd = h_toAdd.GetBinContent(ix+1)
      e_toAdd = h_toAdd.GetBinError(ix+1)

      h_out.SetBinContent(ix+1, y_original+y_toAdd)
      h_out.SetBinError(ix+1, e_original+e_toAdd)

    return h_out

  else:
    h_original.Add(h_toAdd)
    return h_original

def TotalLumi(DataYear):

  if DataYear==2016:
    return "35.92"
  if DataYear==2017:
    return "41.53"
  if DataYear==2018:
    return "59.74"
  if DataYear<0:
    return "137"
  else:
    print "[mylib.py, TotalLumi()] Wrong DataYear : %d"%DataYear
    return "35.9"

def LumiError(DataYear):

  if DataYear==2016:
    return 0.025
  elif DataYear==2017:
    return 0.023
  elif DataYear==2018:
    return 0.025
  else:
    print "[mylib.py, LumiError()] Wrong DataYear : %d"%DataYear
    return 0.

def MakeOverflowBin(hist):

  #### GetXaxis().SetRangeUser() should be done first

  #### 0     1                                          n_bin_origin
  ####     |===|===|===|===|===|===|===|===|===|===|===|===|===|
  ####           bin_first        bin_last
  ####             |===|===|===|===|===|
  #### |   |                                                   |   |
  #### under                                                   over
  #### flow                                                    flow
  #### |<=========>|                   |<=========================>|
  ####  all underflow                           all overflow

  #### Original NBins
  n_bin_origin = hist.GetXaxis().GetNbins()
  #### Changed NBins
  bin_first = hist.GetXaxis().GetFirst()
  bin_last = hist.GetXaxis().GetLast()
  n_bin_inrange = bin_last-bin_first+1

  x_first_lowedge = hist.GetXaxis().GetBinLowEdge(bin_first)
  x_last_upedge = hist.GetXaxis().GetBinUpEdge(bin_last)

  Allunderflows = hist.Integral(0, bin_first-1)
  Allunderflows_error = hist.GetBinError(0)
  Allunderflows_error = Allunderflows_error*Allunderflows_error
  for i in range(1, bin_first-1 +1):
    Allunderflows_error += (hist.GetBinError(i))*(hist.GetBinError(i))
  Allunderflows_error = math.sqrt(Allunderflows_error)

  Alloverflows = hist.Integral(bin_last+1, n_bin_origin+1)
  Alloverflows_error = hist.GetBinError(n_bin_origin+1)
  Alloverflows_error = Alloverflows_error*Alloverflows_error
  for i in range(bin_last+1, n_bin_origin +1):
    Alloverflows_error += (hist.GetBinError(i))*(hist.GetBinError(i))
  Alloverflows_error = math.sqrt(Alloverflows_error)

  #### Make X-bin array
  temp_xbins = [] ## size = n_bin_inrange+1
  counter=0
  for i in range(bin_first,bin_last +1):
    temp_xbins.append( hist.GetXaxis().GetBinLowEdge(i) )
    counter += 1
  temp_xbins.append( hist.GetXaxis().GetBinUpEdge(bin_last) )

  hist_out = ROOT.TH1D(hist.GetName()+'tmp', hist.GetTitle(), n_bin_inrange, array("d",temp_xbins))
  for i in range(1,n_bin_inrange+1):
    this_content = hist.GetBinContent(bin_first-1+i)
    this_error = hist.GetBinError(bin_first-1+i)
    #print '[%d,%d] : %f'%(hist_out.GetXaxis().GetBinLowEdge(i),hist_out.GetXaxis().GetBinUpEdge(i),this_content)

    #### underflows
    if i==1:
      this_content += Allunderflows
      this_error = math.sqrt( this_error*this_error + Allunderflows_error*Allunderflows_error )

    #### overflows
    if i==n_bin_inrange:
      this_content += Alloverflows
      this_error = math.sqrt( this_error*this_error + Alloverflows_error*Alloverflows_error )

    hist_out.SetBinContent(i, this_content)
    hist_out.SetBinError(i, this_error)

  hist_out.SetName(hist.GetName())
  return hist_out

def RebinWRMass(hist, region, DataYear):

  lastbin = hist.GetXaxis().GetNbins()
  vec_bins = [800, 1000, 1200, 1400, 1600, 2000, 2400, 2800, 3200, 8000]
  if "Boosted" in region:
    vec_bins = [800, 1000, 1200, 1500, 1800, 8000]

  #if ('LowWR' in region) or ('DYCR' in region):
  if ('LowWR' in region):
    tmp_vec_bins = [0, 200, 300, 400, 500, 600, 700, 800]
    for b in vec_bins:
      tmp_vec_bins.append(b)
    vec_bins = tmp_vec_bins

  n_bin = len(vec_bins)-1
  hist = hist.Rebin(n_bin, hist.GetName(), array("d", vec_bins) )
  return hist

def GetMaximum(a, ErrorScale=1.):

  NX = a.GetN()

  maxval = -9999.
  for i in range(0,NX):

    x = ROOT.Double(0.)
    y = ROOT.Double(0.)

    a.GetPoint(i, x, y)
    yerr_low  = a.GetErrorYlow(i)
    yerr_high = a.GetErrorYhigh(i)

    if (y+ErrorScale*yerr_high > maxval):
      maxval = y+ErrorScale*yerr_high

  return maxval

def GetAsymmError(MC_stacked_allerr_Up, MC_stacked_allerr_Down):

  NBin = MC_stacked_allerr_Up.GetXaxis().GetNbins()
  x = []
  x_lerr = []
  x_rerr = []
  y = []
  y_lerr = []
  y_rerr = []

  for i in range(0,NBin):

    x.append( MC_stacked_allerr_Up.GetXaxis().GetBinCenter(i+1) )
    x_lerr.append( x[i] - MC_stacked_allerr_Up.GetXaxis().GetBinLowEdge(i+1) )
    x_rerr.append( MC_stacked_allerr_Up.GetXaxis().GetBinUpEdge(i+1) - x[i] )

    y.append( MC_stacked_allerr_Up.GetBinContent(i+1) )
    y_lerr.append( MC_stacked_allerr_Down.GetBinError(i+1) )
    y_rerr.append( MC_stacked_allerr_Up.GetBinError(i+1) )

  out = ROOT.TGraphAsymmErrors(NBin, array("d", x), array("d", y),  array("d", x_lerr), array("d", x_rerr), array("d", y_lerr), array("d", y_rerr))
  return out

def GetDYNormSF(DataYear, channel):

  DYNorm = 1.
  DYNorm_err = 0.

  int_channel = -1 ## 0 : ee, 1 : mm
  if "Electron" in channel:
    int_channel = 0
  elif "Muon" in channel:
    int_channel = 1
  elif "_EMu_" in channel:
    return 1., 0.
  else:
    print 'Wrong channel name '+channel
    exit()

  int_region = -1 ## 0 : Resolved, 1 : Boosted
  if "Resolved" in channel:
    int_region = 0;
  elif "Boosted" in channel:
    int_region = 1;
  else:
    print 'Wrong region name '+channel
    exit()

  #print '%s -> int_channel = %d, int_region = %d'%(channel,int_channel,int_region)

  if DataYear==2016:
    if int_channel==0:
      if int_region==0:
        DYNorm = 0.9885
        DYNorm_err = 0.0232365
      elif int_region==1:
        DYNorm = 0.786894
        DYNorm_err = 0.0335847
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 0.983116
        DYNorm_err = 0.0221691
      elif int_region==1:
        DYNorm = 0.650332
        DYNorm_err = 0.0302755
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()
  elif DataYear==2017:
    if int_channel==0:
      if int_region==0:
        DYNorm = 1.04708
        DYNorm_err = 0.024024
      elif int_region==1:
        DYNorm = 1.02889
        DYNorm_err = 0.0427265
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 1.05824
        DYNorm_err = 0.0235158
      elif int_region==1:
        DYNorm = 0.930729
        DYNorm_err = 0.0395311
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()
  elif DataYear==2018:
    if int_channel==0:
      if int_region==0:
        DYNorm = 0.974142
        DYNorm_err = 0.0217522
      elif int_region==1:
        DYNorm = 0.905253
        DYNorm_err = 0.0331565
      else:
        print "Wrong DY Norm"
        exit()
    elif int_channel==1:
      if int_region==0:
        DYNorm = 1.00625
        DYNorm_err = 0.0219334
      elif int_region==1:
        DYNorm = 0.793592
        DYNorm_err = 0.0308831
      else:
        print "Wrong DY Norm"
        exit()
    else:
      print "Wrong DY Norm"
      exit()

  return DYNorm, DYNorm_err

  #if 'DYCR' in channel:
  #  return DYNorm, DYNorm_err
  #  return DYNorm, 0.30*DYNorm
  #else:
  #  return DYNorm, 0.30*DYNorm

def GetSignalXsec(filepath, mWR, mN):

  lines = open(filepath).readlines()
  for line in lines:
    words = line.split()
    if ( int(words[0])==int(mWR) ) and ( int(words[1])==int(mN) ):
      return float(words[2])
  print 'Xsec not found for mWR=%d, mN=%d'%(mWR,mN)

def GetKFactor(mWR, mN, Year=2016, lepch=0):

  ##==== lepch : 0=ee, 1=mm

  WORKING_DIR = os.environ["PLOTTER_WORKING_DIR"]
  dataset =  os.environ["CATANVERSION"]
  ENV_PLOT_PATH =  os.environ["PLOT_PATH"]

  massstring = "WR"+str(mWR)+"_N"+str(mN)

  ##==== get averaged kfactor
  lines = open(WORKING_DIR+"/data/"+dataset+"/"+str(Year)+"/AveragedKFactor.txt").readlines()
  this_avg_kfactor = -1.;
  for line in lines:
    words = line.strip('\n').split()
    this_mass = words[0]
    avg_k_ee = float(words[1])
    avg_k_mm = float(words[2])

    if massstring==this_mass:
      if lepch==0:
        this_avg_kfactor = avg_k_ee;
      elif lepch==1:
        this_avg_kfactor = avg_k_mm;
      else:
        print "[mylib.py][GetKFactor] Wrong lepch = "+str(lepch)
      break

  if this_avg_kfactor<0:
    print "[mylib.h][GetKFactor] this_avg_kfactor = "+str(this_avg_kfactor)

  return this_avg_kfactor


