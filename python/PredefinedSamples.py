import os,ROOT
from Plotter import SampleGroup

## DY
SampleGroup_DY_2016 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2016,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+jets',
  LatexAlias='ZJets'
)
SampleGroup_DY_2017 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2017,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+jets',
  LatexAlias='ZJets'
)
SampleGroup_DY_2018 = SampleGroup(
  Name='DY',
  Type='Bkgd',
  Samples=['DYJets_MG_HT'],
  Year = 2018,
  Color=ROOT.kYellow,
  Style=1,
  TLatexAlias='Z+jets',
  LatexAlias='ZJets'
)

## ttbar
SampleGroup_ttbar_2016 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2016,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)
SampleGroup_ttbar_2017 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2017,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)
SampleGroup_ttbar_2018 = SampleGroup(
  Name='ttbar',
  Type='Bkgd',
  Samples=['TTLX_powheg'],
  Year = 2018,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='ttbar'
)

SampleGroup_TT_TW_2016 = SampleGroup(
  Name='TT_TW',
  Type='Bkgd',
  Samples=['TT_TW'],
  Year = 2016,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}+tW',
  LatexAlias='TT\_TW'
)
SampleGroup_TT_TW_2017 = SampleGroup(
  Name='TT_TW',
  Type='Bkgd',
  Samples=['TT_TW'],
  Year = 2017,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}+tW',
  LatexAlias='TT_TW'
)
SampleGroup_TT_TW_2018 = SampleGroup(
  Name='TT_TW',
  Type='Bkgd',
  Samples=['TT_TW'],
  Year = 2018,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}+tW',
  LatexAlias='TT\_TW'
)

## others
SampleGroup_Others_2016 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2016,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)
SampleGroup_Others_2017 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2017,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)
SampleGroup_Others_2018 = SampleGroup(
  Name='Others',
  Type='Bkgd',
  Samples=['Others'],
  Year = 2018,
  Color=870,
  Style=1,
  TLatexAlias='Other backgrounds',
  LatexAlias='Others'
)

## WJets_MG_HT
SampleGroup_WJets_2016 = SampleGroup(
  Name='WJets_MG_HT',
  Type='Bkgd',
  Samples=['WJets_MG_HT'],
  Year = 2016,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='WJets_MG_HT'
)
SampleGroup_WJets_2017 = SampleGroup(
  Name='WJets_MG_HT',
  Type='Bkgd',
  Samples=['WJets_MG_HT'],
  Year = 2017,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='WJets_MG_HT'
)
SampleGroup_WJets_2018 = SampleGroup(
  Name='WJets_MG_HT',
  Type='Bkgd',
  Samples=['WJets_MG_HT'],
  Year = 2018,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='WJets_MG_HT'
)

## NonPrompt
SampleGroup_NonPrompt_2016 = SampleGroup(
  Name='NonPrompt',
  Type='Bkgd',
  Samples=['NonPrompt'],
  Year = 2016,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='Nonprompt',
  LatexAlias='NonPrompt'
)
SampleGroup_NonPrompt_2017 = SampleGroup(
  Name='NonPrompt',
  Type='Bkgd',
  Samples=['NonPrompt'],
  Year = 2017,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='Nonprompt',
  LatexAlias='NonPrompt'
)
SampleGroup_NonPrompt_2018 = SampleGroup(
  Name='NonPrompt',
  Type='Bkgd',
  Samples=['NonPrompt'],
  Year = 2018,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='Nonprompt',
  LatexAlias='NonPrompt'
)

## Error from shape
SampleGroup_total_background_2016 = SampleGroup(
  Name='total_background',
  Type='Bkgd',
  Samples=['total_background'],
  Year = 2016,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='total_background'
)
SampleGroup_total_background_2017 = SampleGroup(
  Name='total_background',
  Type='Bkgd',
  Samples=['total_background'],
  Year = 2017,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='total_background'
)
SampleGroup_total_background_2018 = SampleGroup(
  Name='total_background',
  Type='Bkgd',
  Samples=['total_background'],
  Year = 2018,
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='total_background'
)
SampleGroup_total_background_YearCombined = SampleGroup(
  Name='total_background',
  Type='Bkgd',
  Samples=['total_background'],
  Year = 'YearCombined',
  Color=ROOT.kGreen+1,
  Style=1,
  TLatexAlias='W+Jets',
  LatexAlias='total_background'
)

## TTLL
SampleGroup_TTLL_2016 = SampleGroup(
  Name='TTLL',
  Type='Bkgd',
  Samples=['TTLL_powheg'],
  Year = 2016,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='tt'
)
SampleGroup_TTLL_2017 = SampleGroup(
  Name='TTLL',
  Type='Bkgd',
  Samples=['TTLL_powheg'],
  Year = 2017,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='tt'
)
SampleGroup_TTLL_2018 = SampleGroup(
  Name='TTLL',
  Type='Bkgd',
  Samples=['TTLL_powheg'],
  Year = 2018,
  Color=ROOT.kRed,
  Style=1,
  TLatexAlias='t#bar{t}',
  LatexAlias='tt'
)


## TW
SampleGroup_TW_2016 = SampleGroup(
  Name='TW',
  Type='Bkgd',
  Samples=['SingleTop_tW_antitop_NoFullyHad','SingleTop_tW_top_NoFullyHad'],
  Year = 2016,
  Color=ROOT.kGray,
  Style=1,
  TLatexAlias='tW',
  LatexAlias='tW'
)
SampleGroup_TW_2017 = SampleGroup(
  Name='TW',
  Type='Bkgd',
  Samples=['SingleTop_tW_antitop_NoFullyHad','SingleTop_tW_top_NoFullyHad'],
  Year = 2017,
  Color=ROOT.kGray,
  Style=1,
  TLatexAlias='tW',
  LatexAlias='tW'
)
SampleGroup_TW_2018 = SampleGroup(
  Name='TW',
  Type='Bkgd',
  Samples=['SingleTop_tW_antitop_NoFullyHad','SingleTop_tW_top_NoFullyHad'],
  Year = 2018,
  Color=ROOT.kGray,
  Style=1,
  TLatexAlias='tW',
  LatexAlias='tW'
)
