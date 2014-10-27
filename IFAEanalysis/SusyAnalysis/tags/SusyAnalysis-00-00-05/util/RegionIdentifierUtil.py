#!/usr/bin/python

import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True

import os, subprocess, sys, commands, math

class AnalysisDictionary(object):
    '''This class identifies the different analyses regions. string is of the type Stop-A6-SR'''
    def __init__(self):
        self.dictionary = {}
        self.weights = {}
        self.baseCuts = {}

        self.stringDictionary()

    def stringDictionary(self):
        '''Add here the definitions of the preselection cuts, region cuts and channel cuts'''

        selectionCuts = {}
        channelCuts = {}
        weights = {}


        #--- StopRz to top neutralino (Razor)
        self.baseCuts['StopRz'] = ['passPreselectionCuts', 'abs(eta1)<2.5', 'abs(eta2)<2.5', '((abs(eta1)>2.0 && pt1<100.) || (j1_chf>0.02 || (j1_chf>0.05 && j1_emf<0.9)))', '((abs(eta2)>2.0 && pt2<100.) || (j2_chf>0.02 || (j2_chf>0.05 && j2_emf<0.9)))', 'pt2>50.']

        dphijm2 = ['dPhi_min>0.2*TMath::Pi()']
        altwobtags = ['n_bjets>1', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.7892'] #it is actually at least 2...
        lepveto = ['e_N==0', 'm_N==0']

	realMETtrk = ['met_trk>30.','dPhi_met_mettrk<TMath::Pi()/2']	

        #dummy now (Njet moved to each region definition, to cover 'fit' regions)
	selectionCuts['StopRz_E1'] = ['n_jets>1'] 
        

	# ------ 2tag signal region ('optimization')   [adding one cut at a time]     
	channelCuts['StopRz_SRopt0']  = lepveto + altwobtags + dphijm2 + ['n_jets>4', 'met>150'] #[, 'mct>150.', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']

	channelCuts['StopRz_SRopt1']  = channelCuts['StopRz_SRopt0'] + ['pt1>130.']  

	channelCuts['StopRz_SRopt2']  = channelCuts['StopRz_SRopt1'] + ['pt1>130.'] +  realMETtrk

	channelCuts['StopRz_SRopt3a']  = channelCuts['StopRz_SRopt2'] + ['MT_bcl_met>175.']

        channelCuts['StopRz_SR0']  = channelCuts['StopRz_SRopt3a'] + ['R*R>0.7']

        #HIGH DeltaM
        channelCuts['StopRz_SRH1']  = channelCuts['StopRz_SR0'] + ['MT_j2_met>350.']
        channelCuts['StopRz_SRH2']  = channelCuts['StopRz_SR0'] + ['MR>400.']
        channelCuts['StopRz_SRH3']  = channelCuts['StopRz_SR0'] + ['met>350.']
                        
        #LOW DeltaM


	# ------ control regions  
        twotag  = ['n_bjets==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.7892'] #it is actually at least 2...
        onetag = ['n_bjets==1', 'tag_MV1_1>0.7892']
        zerotag = ['n_bjets==0']

        CRw_base = ['((e_N==1 && m_N==0) || (e_N==0 && m_N==1 && (30.<m_MT && m_MT<100.)))', 'dPhi_min>0.4', '((m_N==1 && met>150.) || (e_N==1 && met_ecorr>150.))', 'pt1>130.']
        CRzmm_base = ['e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)', 'dPhi_min>0.4', 'met>100.', 'pt1>50.']
        CRtop_base = ['((e_N==1 && m_N==1 && m_EM>50.) || ((e_N==1 && m_N==0) || (e_N==0 && m_N==1)))', 'dPhi_min>0.4', 'met>150', 'pt1>130.']

        Zmmpt = ['m_Zpt>120.']
        
        # -- 2-tag control regions 
	channelCuts['StopRz_CRw2TAG'] = CRw_base + twotag + ['n_jets>4']
       	channelCuts['StopRz_CRzmm2TAG'] = CRzmm_base + twotag + Zmmpt + ['n_jets>4']
        channelCuts['StopRz_CRtop2TAG'] = CRtop_base + twotag + ['n_jets>4']
        
        # -- 1-tag control region
	channelCuts['StopRz_CRw1TAG'] = CRw_base + onetag + ['n_jets>4']
       	channelCuts['StopRz_CRzmm1TAG'] = CRzmm_base + onetag + Zmmpt + ['n_jets>4']

	channelCuts['StopRz_CRw1TAGhmjj'] = CRw_base + onetag + ['DiJet_Mass>500.','n_jets>4']
       	channelCuts['StopRz_CRzmm1TAGhmjj'] = CRzmm_base + onetag + Zmmpt + ['DiJet_Mass>500.','n_jets>4']

        # -- 0-tag control regions
 	channelCuts['StopRz_CRw0TAG'] = CRw_base + zerotag + ['n_jets>4']
       	channelCuts['StopRz_CRzmm0TAG'] = CRzmm_base + zerotag + Zmmpt + ['n_jets>4']

        # -- 0-tag control regions
 	channelCuts['StopRz_CRwNOTAG'] = CRw_base + ['n_jets>4']
       	channelCuts['StopRz_CRzmmNOTAG'] = CRzmm_base + Zmmpt + ['n_jets>4']

        # ------ looser njet regions for btag-fraction fit
 	channelCuts['StopRz_CRw2TAGfit'] = CRw_base + twotag + ['n_jets>1']
       	channelCuts['StopRz_CRzmm2TAGfit'] = CRzmm_base + twotag + Zmmpt + ['n_jets>1']
 	channelCuts['StopRz_CRwfit'] = CRw_base  + ['n_jets>1']
       	channelCuts['StopRz_CRzmmfit'] = CRzmm_base + Zmmpt + ['n_jets>1']
        
        # --- just for BosonPtRW (?)
        channelCuts['StopRz_CRwmnPRETAG'] = CRw_base + ['n_jets>4']

        #        weights['StopRz'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))', 'ttbar_weight', 'btag_weight1*(btag_weight_total/btag_weight1)']
        weights['StopRz'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))', 'ttbar_weight', 'btag_weight_total']


        #--- Stop to top neutralino analysis (StopTL)

        self.baseCuts['StopTL'] = ['passPreselectionCuts', 'abs(eta1)<2.5', 'abs(eta2)<2.5', '((abs(eta1)>2.0 && pt1<100.) || (j1_chf>0.02 || (j1_chf>0.05 && j1_emf<0.9)))', '((abs(eta2)>2.0 && pt2<100.) || (j2_chf>0.02 || (j2_chf>0.05 && j2_emf<0.9)))', 'pt2>80.']

        dphijm2 = ['dPhi_min>0.2*TMath::Pi()',]
        dphijm1 = ['dPhi_min>0.1*TMath::Pi()']
        twobtags = ['n_bjets>1', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.7892'] #it is actually at least 2...
        onebtag = ['n_bjets==1', 'tag_MV1_1>0.7892'] #strictly one
        res_jets = ['n_jets>5', 'pt6>35.', 'MT_bcl_met>175.'] + twobtags
        lepveto = ['e_N==0', 'm_N==0']
        tauveto = ['n_taujets==0']
        antilepveto = ['(e_N+m_N)==1','((e_N==1 && e_pt>35.) || (m_N==1 && m_pt>35.))']

	selectionCuts['StopTL_P1'] = ['pt2>0.'] #dummy thing for now!

	commonSRsel = ['met_trk>30','dPhi_met_mettrk<TMath::Pi()/3']

	# ------ signal regions (fully resolved)       
	channelCuts['StopTL_SRA1'] = lepveto + tauveto + commonSRsel + dphijm2 + res_jets + ['met>150.', 'm_top_had1<225.', 'm_top_had2<250.']
	channelCuts['StopTL_SRA2'] = lepveto + tauveto + commonSRsel + dphijm2 + res_jets + ['met>250.', 'm_top_had1<225.', 'm_top_had2<250.']
	channelCuts['StopTL_SRA3'] = lepveto + tauveto + commonSRsel + dphijm2 + res_jets + ['met>200.'] + self.inrange('m_top_had1',75.,200.) + self.inrange('m_top_had2',50.,250.) + ['MT_min_jet_met>50.' ] 
	channelCuts['StopTL_SRA4'] = lepveto + tauveto + commonSRsel + dphijm2 + res_jets + ['met>300.'] + self.inrange('m_top_had1',50.,250.) + self.inrange('m_top_had2',50.,400.) + ['MT_min_jet_met>50.']
	channelCuts['StopTL_SRA5'] = lepveto + tauveto + commonSRsel + dphijm2 + res_jets + ['met>350.'] + self.inrange('m_top_had1',50.,250.) + self.inrange('m_top_had2',50.,400.) + ['MT_min_jet_met>50.']

	# ------ signal regions (partially resolved)       
	channelCuts['StopTL_SRB4j'] = lepveto + twobtags + ['mtasym12<0.5', 'n_jets==4', 'pt4>35.', 'MT_lcl_met>175.', 'MT_jsoft_met>280', 'm0_antikt12>80.'] + self.inrange('m1_antikt12',60.,200.) + ['m0_antikt08>50.', 'met>325.']
	channelCuts['StopTL_SRB5j'] = lepveto + twobtags + ['mtasym12<0.5', 'n_jets==5', 'pt5>35.', 'MT_lcl_met>175.', 'm0_antikt12>80.'] + self.inrange('m1_antikt12',60.,200.) + ['m0_antikt08>50.', 'met>325.']
        # this merges the previous two
	channelCuts['StopTL_SRB1'] = lepveto + twobtags + ['mtasym12<0.5'] + self.ORcuts('(n_jets==4 && pt4>35.)','(n_jets==5 && pt5>35.)') + ['MT_lcl_met>175.'] + self.ORcuts('n_jets==5','MT_jsoft_met>280')+self.inrange('m1_antikt12',60.,200.)+['m0_antikt12>80.','m0_antikt08>50.', 'met>325.']

	channelCuts['StopTL_SRB2'] = lepveto + twobtags + ['mtasym12>0.5', 'n_jets==5', 'pt5>35.', 'pt2>100.', 'pt0_antikt12>350.', 'MT_lcl_met>125.']+self.inrange('m0_antikt12',140.,500.)+self.inrange('m0_antikt08',70.,300.)+['met/sqrt(HT)>17.', 'met>400.']

        # ------ control regions 

        #*** ttbar
        t1tn_goodCRTel = self.ANDcuts(['e_N>0','e_pt>35.','e_tight','e_etiso30/e_pt<0.18','e_ptiso30/e_pt<0.16'])
        t1tn_goodCRTmu = self.ANDcuts(['m_N>0','m_pt>35.','m_etiso30/m_pt<0.18','m_ptiso30/m_pt<0.12'])
        antilepveto = self.ORcuts(t1tn_goodCRTel, t1tn_goodCRTmu)

        #(fully resolved)       
        channelCuts['StopTL_CRTopA'] = antilepveto + dphijm1 + twobtags + self.ORcuts('e_N==0','m_N==0') + ['(n_jets+e_N+m_N)>5','pt1>80.','(e_pt>80. || m_pt>80. || pt2>80.)','pt5>35.','((e_N && (e_MT>40. && e_MT<120.) || (m_N && (m_MT>40. && m_MT<120.))','(m_top_had1<600 || m_top_had2<600)','met>150.','MT_bcl_met>125.']

        #(partially resolved)       
        # CHECK NOT SURE THEY APPLY THE A_mt cut here!!
        # common (njets>2)
        channelCuts['StopTL_CRTopB2jinc'] = antilepveto + dphijm1 + twobtags + self.ORcuts('e_N==0','m_N==0') + ['mtasym12<0.5', '(n_jets+e_N+m_N)>2', 'pt1>80.']+self.ORcuts('e_pt>80.','m_pt>80.','pt2>80.')+['pt3>35.']+self.ORcuts(self.ANDcuts('e_N',self.inrange('e_MT',40.,120.)), self.ANDcuts('m_N',self.inrange('m_MT',40.,120.)))+['met>150.','pt1_antikt12>0.']
        # exclusive njets==4
        channelCuts['StopTL_CRTopB4j'] = channelCuts['StopTL_CRTopB2jinc']+self.ORcuts('e_N==0','m_N==0')+['(n_jets+e_N+m_N)==4','pt3>35.']
        channelCuts['StopTL_CRTopB5j'] = channelCuts['StopTL_CRTopB2jinc']+self.ORcuts('e_N==0','m_N==0')+['(n_jets+e_N+m_N)==5','pt4>35.']

        #this merges the two above
        channelCuts['StopTL_CRTopB1'] = channelCuts['StopTL_CRTopB2jinc']+self.ORcuts('e_N==0','m_N==0')+self.ORcuts(self.ANDcuts('(n_jets+e_N+m_N)==4','pt3>35.'),self.ANDcuts('(n_jets+e_N+m_N)==5','pt4>35.'))

        channelCuts['StopTL_CRTopB2'] = antilepveto + dphijm1 + twobtags + self.ORcuts('e_N==0','m_N==0') + ['mtasym12>0.5', '(n_jets+(e_N+m_N))==5','pt1>80.']+self.ORcuts('e_pt>80.','m_pt>80.','pt2>80.')+['pt4>35.']+self.ORcuts(self.ANDcuts('e_N',self.inrange('e_MT',40.,120.)), self.ANDcuts('m_N', self.inrange('m_MT',40.,120.)))+['dPhi_b1_b2>0.2*TMath::Pi()','met>100.','MT_bcl_met>150.','MT_bfar_met>125.']

        #*** Z(vv)jets
        t1tn_goodCRTdiel = self.ANDcuts('e_N==2','m_N==0','e_pt>25.','e_tight','e_etiso30/e_pt<0.18','e_ptiso30/e_pt<0.16')
        t1tn_goodCRTdimu = self.ANDcuts('m_N==2','e_N==0','m_pt>25.','m2_pt>10.','m_etiso30/m_pt<0.18','m_ptiso30/m_pt<0.12','m2_etiso30/m2_pt<0.18','m2_ptiso30/m2_pt<0.12')
        t1tn_goodCRTdilep = self.ORcuts(t1tn_goodCRTdiel, t1tn_goodCRTdimu)
    

        # the first is the inclusive of the 3 below (n_jets>=2)
        # channelCuts['StopTL_CRZeefit'] = t1tn_goodCRTdimu + ['met_mu<50.','e_M>86.','e_M<96.','met>70.','n_jets>=2'] ##met_mu cut can't be applied till we get lepton triggers 
        channelCuts['StopTL_CRZeefit'] = t1tn_goodCRTdiel + self.inrange('e_M',86.,96.)+['met_ecorr>70.','n_jets>=2','n_bjets==2']
        channelCuts['StopTL_CRZee4j']  = channelCuts['StopTL_CRZeefit'] + ['n_jets==4']
        channelCuts['StopTL_CRZee5j']  = channelCuts['StopTL_CRZeefit'] + ['n_jets==5']
        channelCuts['StopTL_CRZee6j']  = channelCuts['StopTL_CRZeefit'] + ['n_jets==6']

        # the first is the inclusive of the 3 below (n_jets>=2)
        # channelCuts['StopTL_CRZmmfit'] = t1tn_goodCRTdimu + ['met_mu<50.','m_ll>86.','m_ll<96.','met>70.','n_jets>=2'] ##met_mu cut can't be applied till we get lepton triggers 
        channelCuts['StopTL_CRZmmfit'] = t1tn_goodCRTdimu + self.inrange('m_M',86.,96.) + ['met>70.','n_jets>=2','n_bjets==2'] 
        channelCuts['StopTL_CRZmm4j']  = channelCuts['StopTL_CRZmmfit'] + ['n_jets==4']
        channelCuts['StopTL_CRZmm5j']  = channelCuts['StopTL_CRZmmfit'] + ['n_jets==5']
        channelCuts['StopTL_CRZmm6j']  = channelCuts['StopTL_CRZmmfit'] + ['n_jets==6']

        # the first is the inclusive of the 3 below (n_jets>=2)
        channelCuts['StopTL_CRZfit'] = self.ORcuts(channelCuts['StopTL_CRZeefit'], channelCuts['StopTL_CRZmmfit'])
        channelCuts['StopTL_CRZ4j']  = self.ORcuts(channelCuts['StopTL_CRZee4j'], channelCuts['StopTL_CRZmm4j'])
        channelCuts['StopTL_CRZ5j']  = self.ORcuts(channelCuts['StopTL_CRZee5j'], channelCuts['StopTL_CRZmm5j'])
        channelCuts['StopTL_CRZ6j']  = self.ORcuts(channelCuts['StopTL_CRZee6j'], channelCuts['StopTL_CRZmm6j'])

        #*** Wjets
        #(partially resolved)       
        channelCuts['StopTL_CRWB4j'] = antilepveto + dphijm1 + onebtag + ['mtasym12<0.5', '(n_jets+(e_N+m_N))==4', 'pt1>80.','(e_pt>80. || m_pt>80. || pt2>80.)','pt3>35.','((e_N && (e_MT>40. && e_MT<120.) || (m_N && (m_MT>40. && m_MT<120.))','met>150.','pt1_antikt12>0.','m0_antikt12<40']
        channelCuts['StopTL_CRWB5j'] = antilepveto + dphijm1 + onebtag + ['mtasym12<0.5', '(n_jets+(e_N+m_N))==5', 'pt1>80.','(e_pt>80. || m_pt>80. || pt2>80.)','pt4>35.','((e_N && (e_MT>40. && e_MT<120.) || (m_N && (m_MT>40. && m_MT<120.))','met>150.','pt1_antikt12>0.','m0_antikt12<40']
        #this merges the two above
        channelCuts['StopTL_CRWB1'] = antilepveto + dphijm1 + onebtag + ['mtasym12<0.5', '((n_jets+(e_N+m_N))==4 || (n_jets+(e_N+m_N))==5)','pt1>80.','(e_pt>80. || m_pt>80. || pt2>80.)','pt3>35.','((n_jets+(e_N+m_N))==4 || pt4>35.)','((e_N && (e_MT>40. && e_MT<120.) || (m_N && (m_MT>40. && m_MT<120.))','met>150.','pt1_antikt12>0.','m0_antikt12<40']


        weights['StopTL'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', 'ttbar_weight', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))']

        #--- Stop to charm neutralino analysis

#        self.baseCuts['Stop'] = ['passPreselectionCuts', 'abs(eta1)<2.8', 'j1_chf>0.02', 'j1_emf>0.1', 'j1_fsm<0.8', 'JetsFailBCHMedium==0']
        self.baseCuts['Stop'] = ['passPreselectionCuts', 'abs(eta1)<2.8', 'j1_chf>0.02', 'j1_emf>0.1', 'j1_fsm<0.8']

        selectionCuts['Stop_A0'] = ['met>150.', 'met<220.', 'pt1>150', 'pt1<220.', 'n_jets<=3']
        selectionCuts['Stop_A1'] = ['met>220.', 'pt1>220.', 'n_jets<=3']
        selectionCuts['Stop_A2'] = ['met>280.', 'pt1>280.', 'n_jets<=3']
        selectionCuts['Stop_A3'] = ['met>340.', 'pt1>340.', 'n_jets<=3']
        selectionCuts['Stop_A3QCD'] = ['met>300.', 'pt1>300.', 'n_jets<=3']
        selectionCuts['Stop_A4'] = ['met>450.', 'pt1>450.', 'n_jets<=3']
        selectionCuts['Stop_A5'] = ['met>170.', 'pt1>220.', 'n_jets<=3']
        selectionCuts['Stop_A6'] = ['met>220.', 'pt1>280.', 'n_jets<=3']
        selectionCuts['Stop_A7'] = ['met>280.', 'pt1>340.', 'n_jets<=3']
        selectionCuts['Stop_A8'] = ['met>340.', 'pt1>450.', 'n_jets<=3']
        selectionCuts['Stop_A9'] = ['met>550.', 'pt1>550.', 'n_jets<=3']
        selectionCuts['Stop_A10'] = ['met>600.', 'pt1>600.', 'n_jets<=3']
        selectionCuts['Stop_B0'] = ['met>150.', 'met<220.', 'pt1>150', 'pt1<220.', 'n_jets>=3']
        selectionCuts['Stop_B1'] = ['met>220.', 'pt1>220.', 'n_jets>=3']
        selectionCuts['Stop_B2'] = ['met>280.', 'pt1>280.', 'n_jets>=3']
        selectionCuts['Stop_B3'] = ['met>340.', 'pt1>340.', 'n_jets>=3']
        selectionCuts['Stop_B4'] = ['met>450.', 'pt1>450.', 'n_jets>=3']
        selectionCuts['Stop_B5'] = ['met>170.', 'pt1>220.', 'n_jets>=3']
        selectionCuts['Stop_B6'] = ['met>220.', 'pt1>280.', 'n_jets>=3']
        selectionCuts['Stop_B7'] = ['met>280.', 'pt1>340.', 'n_jets>=3']
        selectionCuts['Stop_B8'] = ['met>340.', 'pt1>450.', 'n_jets>=3']

        channelCuts['Stop_SR']    = ['dPhi_min>0.4', 'e_N==0' ,'m_N==0']
        channelCuts['Stop_CRele'] = ['dPhi_min>0.4', 'e_N==1' ,'m_N==0']
        channelCuts['Stop_CRwmn'] = ['dPhi_min>0.4', 'e_N==0', 'm_N==1', '(30.<m_MT && m_MT<100.)']
        channelCuts['Stop_CRzmm'] = ['dPhi_min>0.4', 'e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)']
        channelCuts['Stop_CRtop'] = ['dPhi_min>0.4', 'e_N==1', 'm_N==1']
        channelCuts['Stop_CRqcd'] = ['dPhi_min<0.4', 'e_N==0', 'm_N==0']

        weights['Stop'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', 'ttbar_weight', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))']


        self.baseCuts['Stop13'] = ['passPreselectionCuts', 'abs(eta1)<2.8', 'j1_chf>0.02', 'j1_emf>0.1', 'j1_fsm<0.8']

        selectionCuts['Stop13_A0'] = ['met>150.', 'met<220.', 'pt1>150', 'pt1<220.', 'n_jets<=3']
        selectionCuts['Stop13_A1'] = ['met>220.', 'pt1>220.', 'n_jets<=3']
        selectionCuts['Stop13_A2'] = ['met>280.', 'pt1>280.', 'n_jets<=3']
        selectionCuts['Stop13_A3'] = ['met>340.', 'pt1>340.', 'n_jets<=3']
        selectionCuts['Stop13_A3QCD'] = ['met>300.', 'pt1>300.', 'n_jets<=3']
        selectionCuts['Stop13_A4'] = ['met>450.', 'pt1>450.', 'n_jets<=3']
        selectionCuts['Stop13_A5'] = ['met>170.', 'pt1>220.', 'n_jets<=3']
        selectionCuts['Stop13_A6'] = ['met>220.', 'pt1>280.', 'n_jets<=3']
        selectionCuts['Stop13_A7'] = ['met>280.', 'pt1>340.', 'n_jets<=3']
        selectionCuts['Stop13_A8'] = ['met>340.', 'pt1>450.', 'n_jets<=3']
        selectionCuts['Stop13_A9'] = ['met>550.', 'pt1>550.', 'n_jets<=3']
        selectionCuts['Stop13_A10'] = ['met>600.', 'pt1>600.', 'n_jets<=3']
        selectionCuts['Stop13_B0'] = ['met>150.', 'met<220.', 'pt1>150', 'pt1<220.', 'n_jets>=3']
        selectionCuts['Stop13_B1'] = ['met>220.', 'pt1>220.', 'n_jets>=3']
        selectionCuts['Stop13_B2'] = ['met>280.', 'pt1>280.', 'n_jets>=3']
        selectionCuts['Stop13_B3'] = ['met>340.', 'pt1>340.', 'n_jets>=3']
        selectionCuts['Stop13_B4'] = ['met>450.', 'pt1>450.', 'n_jets>=3']
        selectionCuts['Stop13_B5'] = ['met>170.', 'pt1>220.', 'n_jets>=3']
        selectionCuts['Stop13_B6'] = ['met>220.', 'pt1>280.', 'n_jets>=3']
        selectionCuts['Stop13_B7'] = ['met>280.', 'pt1>340.', 'n_jets>=3']
        selectionCuts['Stop13_B8'] = ['met>340.', 'pt1>450.', 'n_jets>=3']

        channelCuts['Stop13_SR']    = ['dPhi_min>0.4', 'e_N==0' ,'m_N==0']
        channelCuts['Stop13_CRele'] = ['dPhi_min>0.4', 'e_N==1' ,'m_N==0']
        channelCuts['Stop13_CRwmn'] = ['dPhi_min>0.4', 'e_N==0', 'm_N==1', '(30.<m_MT && m_MT<100.)']
        channelCuts['Stop13_CRzmm'] = ['dPhi_min>0.4', 'e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)']
        channelCuts['Stop13_CRtop'] = ['dPhi_min>0.4', 'e_N==1', 'm_N==1']
        channelCuts['Stop13_CRqcd'] = ['dPhi_min<0.4', 'e_N==0', 'm_N==0']

        weights['Stop13'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', 'PDF_w', 'ttbar_weight', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))']


        #--- Sbottom to bottom neutralino analysis

        self.baseCuts['Sbottom'] = ['passPreselectionCuts', 'abs(eta1)<2.5', 'abs(eta2)<2.5', '((abs(eta1)>2.0 && pt1<100.) || (j1_chf>0.02 || (j1_chf>0.05 && j1_emf<0.9)))', '((abs(eta2)>2.0 && pt2<100.) || (j2_chf>0.02 || (j2_chf>0.05 && j2_emf<0.9)))', 'pt2>50.', 'pt3<50.', 'met/(met+pt1+pt2)>0.25']

	selectionCuts['Sbottom_A'] = ['n_jets>1']
        
	
	# ------ 2tag signal region        
	channelCuts['Sbottom_SR150']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>150.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR200']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>200.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR250']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>250.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR300']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>300.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR350']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>350.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR400']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>400.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR450']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>450.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']

	# ------------------- binned SRs
	
	channelCuts['Sbottom_SR0_150']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct<150.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']
	channelCuts['Sbottom_SR150_250']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>150.', 'mct<250.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']	
	channelCuts['Sbottom_SR250_350']    = ['e_N==0' , 'm_N==0', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'mct>250.', 'mct<350.', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'met>150.', 'pt1>130.']	

	# ------ 1tag signal region        
	#channelCuts['Sbottom_SR1TAG150']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>150.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG200']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>200.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG250']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>250.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG300']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>300.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG350']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>350.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG400']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>400.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	#channelCuts['Sbottom_SR1TAG450']    = ['e_N==0' , 'm_N==0', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'DiJet_Mass>200.', 'mct>450.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']
	
	# ------ control regions   
	channelCuts['Sbottom_CRw2TAG150'] = ['((e_N==1 && m_N==0) || (e_N==0 && m_N==1 && (30.<m_MT && m_MT<100.)))', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'mct>150.', 'pt1>130.', 'met>150.']
       	channelCuts['Sbottom_CRzmm2TAG150'] = ['e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'dPhi_min>0.4', 'mct>150.', 'met>100.', 'pt1>50.']
        channelCuts['Sbottom_CRtop2TAG150'] = ['((e_N==1 && m_N==1 && m_EM>50.) || (((e_N==1 && m_N==0) || (e_N==0 && m_N==1)) && DiJet_Mass<=200.))', 'n_bjets_80eff==2', 'tag_MV1_1>0.7892', 'tag_MV1_2>0.3511', 'dPhi_min>0.4', 'pt1>130.', 'met>150.', 'mct>150.']


	#channelCuts['Sbottom_CRw1TAG150'] = ['((e_N==1 && m_N==0) || (e_N==0 && m_N==1 && (30.<m_MT && m_MT<100.)))', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'mct>150.', 'pt1>130.', 'met>150.']
       	#channelCuts['Sbottom_CRzmm1TAG150'] = ['e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'dPhi_min>0.4', 'mct>150.', 'met>100.', 'pt1>50.']
        #channelCuts['Sbottom_CRtop1TAG150'] = ['((e_N==1 && m_N==1 && m_EM>50.) || (((e_N==1 && m_N==0) || (e_N==0 && m_N==1)) && DiJet_Mass<=200.))', 'n_bjets==1', '(tag_MV1_1>0.7892 || tag_MV1_2>0.7892)', 'dPhi_min>0.4', 'pt1>130.', 'met>150.', 'mct>150.']
    
	channelCuts['Sbottom_CRw0TAG150'] = ['((e_N==1 && m_N==0) || (e_N==0 && m_N==1 && (30.<m_MT && m_MT<100.)))', 'n_bjets_80eff==0', 'dPhi_min>0.4', 'DiJet_Mass>200.', 'mct>150.', 'pt1>130.', 'met>150.']
       	channelCuts['Sbottom_CRzmm0TAG150'] = ['e_N==0', 'm_N==2', '(66.<m_M && m_M<116.)', 'n_bjets_80eff==0', 'dPhi_min>0.4', 'mct>150.', 'met>100.', 'pt1>50.']

        # ------ no tagging control regions
        channelCuts['Sbottom_CRwmnPRETAG150'] = ['e_N==0', 'm_N==1', '(30.<m_MT && m_MT<100.)', 'DiJet_Mass>200.', 'mct>150.', 'dPhi_min>0.4', 'met>150.', 'pt1>130.']


        weights['Sbottom'] = ['w', 'FileWeight', 'MC_w', 'pileup_w', '((e_SF*(e_N>0))+(e_antiSF*(e_N==0)))', '((m_SF*(m_N>0))+(m_antiSF*(m_N==0)))', 'btag_weight1*(btag_weight_total_80eff/btag_weight_80eff1)', 'ttbar_weight']

        
        for analysis in self.baseCuts.keys():
            for selection in selectionCuts.keys():
                for channel in channelCuts.keys():
                    self.dictionary["%s_%s_%s"%(analysis, selection.split('_')[1], channel.split('_')[1])] = self.baseCuts[analysis] + selectionCuts[selection] + channelCuts[channel]

        #--- Remove n_jets cut from CRqcd
        for analysis in self.baseCuts.keys():
            for selection in selectionCuts.keys():
                for channel in channelCuts.keys():
                    if 'CRqcd' in channel:
                        tempListOfCuts = []
                        for cut in self.dictionary["%s_%s_%s"%(analysis, selection.split('_')[1], channel.split('_')[1])]:
                            if 'n_jets' not in cut:
                                tempListOfCuts.append(cut)
                        self.dictionary["%s_%s_%s"%(analysis, selection.split('_')[1], channel.split('_')[1])] = tempListOfCuts
        
            

        self.weights = weights


    def returnCutsList(self, identifier):
        identifier = identifier.split('_')
        analysis = identifier[0]
        region = identifier[1]
        channel = identifier[2]
        return self.dictionary["%s_%s_%s" % (analysis, region, channel)]

    def returnWeightsList(self, identifier):
        return self.weights[identifier]

    def returnBaseCutsList(self, identifier):
        return self.baseCuts[identifier]

    def returnCutsStr(self, identifier):
        identifier = identifier.split('_')
        analysis = identifier[0]
        region = identifier[1]
        channel = identifier[2]
        return '(' + ' && '.join(self.dictionary["%s_%s_%s" % (analysis, region, channel)]) + ')'

    def returnWeightsStr(self, identifier):
        return '(' + '*'.join(self.weights[identifier]) + ')'

    def returnMetCut(self, identifier):
        identifier = identifier.split('_')
        analysis = identifier[0]
        region = identifier[1]
        channel = identifier[2]
        for cut in self.dictionary["%s_%s_%s" % (analysis, region, channel)]:
            if 'met>' in cut:
                return cut.replace('met>','').replace('.','')

    def returnPt1Cut(self, identifier):
        identifier = identifier.split('_')
        analysis = identifier[0]
        region = identifier[1]
        channel = identifier[2]
        for cut in self.dictionary["%s_%s_%s" % (analysis, region, channel)]:
            if 'pt1>' in cut:
                return cut.replace('pt1>','').replace('.','')

    def returnQCDweight(self, identifier, sampleName):
        identifier = identifier.split('_')
        analysis = identifier[0]
        region = identifier[1]
        channel = identifier[2]
        if 'QCD' not in sampleName:
            return ['1.']
        elif analysis=='Stop':
            if region=='A0':
                #BCHcut return ['0.00409588']
                return ['0.00446657']
            if region=='A3':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A3QCD':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A3001':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A30011':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A30012':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A4':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A6':
                #BCHcut return ['0.00361002']
                return ['0.00501327']
            if region=='A8':
                #BCHcut return ['0.00361002']
                return ['0.00501327']
            if region=='A9':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A10':
                #BCHcut return ['0.00340547']
                return ['0.00576958']
            if region=='A61':
                #BCHcut return ['0.00361002']
                return ['0.00501327']
            if region=='A62':
                #BCHcut return ['0.00361002']
                return ['0.00501327']
            
        elif analysis=='Stop13': #--- Not used, but just in case it's a copy of the values before
            if region=='A0':
                return ['0.00446657']
            if region=='A3':
                return ['0.00576958']
            if region=='A3QCD':
                return ['0.00576958']
            if region=='A4':
                return ['0.00576958']
            if region=='A6':
                return ['0.00501327']
            if region=='A8':
                return ['0.00501327']
            if region=='A9':
                return ['0.00576958']
            if region=='A10':
                return ['0.00576958']


    def ORcuts(self, *arg):
        '''Returns a single string list after OR-ing all selections for all channels (lists) or cuts (strings).
        AND is assumed for all cuts within the same channel.
        example1: 
        ch1=['mycut1','mycut2']
        ch2=['mycut3']
        
        ORcuts(ch1,ch2) = ['((mycut2 && mycut3) || (mycut2))']

        
        example2:
        cut1='mycut1'
        cut2='mycut2'

        ORcuts(cut1, cut2) = ['(mycut1 || mycut2)']
        '''
        
        merged=[]
        for c in arg:
            merged += ['%s' % self.ANDcuts(c)[0]]
        
        return ['(%s)' % ' || '.join(merged)]
        

    def ANDcuts(self, *arg):
        '''Returns a single string list after AND-ing all selections for all channels (lists) or cuts (strings).
        you can even mix the two.
        
        example1: 
        ch1=['mycut1','mycut2']
        ch2=['mycut3']

        ANDcuts(ch1,ch2) = ['(mycut1 && mycut2 && mycut3)']

        example2: 
        cut1='mycut1'
        cut2='mycut2'
        cut3='mycut3'
        
        ANDcuts(cut1,cut2,cut3) = ['(mycut1 && mycut2 && mycut3)']
        '''
        clist=[]
        for c in arg:
            if isinstance(c,basestring):
                clist.append(c)
            else:
                clist += c #arg[0]
              
        return ['(%s)' % ' && '.join(clist)]


    def inrange(self, var,low,high):
        '''Returns list of cuts, for var (string) between low and high.
        example:
        
        inrange('met',50.,150.) = ['met>50.','met<150.']
        '''
        return self.ANDcuts('%s>%s' % (var, str(low)), '%s<%s' % (var, str(high)))


myanal = AnalysisDictionary()
#print ' && '.join(myanal.returnCutsList('StopRz_E1_CRtop2TAG'))
#print ' && '.join(myanal.returnCutsList('StopRz_E1_CRw0TAG'))
#print ' && '.join(myanal.returnCutsList('StopRz_E1_CRw1TAG'))
#print myanal.returnWeightsStr('StopRz')                                                                                                                                            #   
#print ' && '.join(myanal.returnCutsList('StopRz_E1_CRzmm0TAG'))
#print myanal.returnCutsList('StopTL_P1_CRTopA')
#print myanal.returnCutsList('StopTL_P2_CRZmmfit')
#print myanal.returnCutsList('Stop_A6_SR')
#print myanal.returnWeightsList('Stop')
#print myanal.returnCutsStr('Stop_A6_SR')
#print myanal.returnWeightsStr('Stop')
##print myanal.returnMetCut('Stop_A6_SR')
##print myanal.returnPt1Cut('Stop_A6_SR')
#print myanal.returnCutsList('Stop_A6_CRqcd')
#print myanal.returnWeightsList('Stop')
#print myanal.returnCutsStr('Stop_A6_CRqcd')
#print myanal.returnWeightsStr('Stop')
