/**
   Authors
   Valerio Consorti <valerio.consorti@cern.ch>

   Purpose: provide a set of scaling factor to reweight W,Z
   and ttbar AlpGen samples from the central value
   to the value that they would assume variating
   factorization or renormalization scale in MC 
   generator. For W also some MLM matching parameter
   variation is available.

   - Version 1.0
**/

#include "SusyAnalysis/ScaleVariatioReweighter.hpp"

using namespace ScaleVariatioReweighter;

double ScaleVariatioReweighter::GetWeight(int sampleId, variation var, mcperiod mc){
  // NUMBERS FOR MC11
  if(mc == mc11) {
    if(var==None) return 1.;
    double Np0_baseline=669.60*1.24347;
    double Np1_baseline=134.52*1.24347;
    double Np2_baseline=40.706*1.24347;
    double Np3_baseline=11.262*1.24347;
    double Np4_baseline=2.8447*1.24347;
    double Np5_baseline=0.75691*1.24347;

    //Z->ll
    /* --------------------------------------
       /  LO xsec from AMI normalized to the tot cross section:
       /  1069 pb * 3 flavours (NNLO)
       /--------------------------------------*/
    //Np0:    Zee           Zmumu               Ztautau             Znunu_filt1jet   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    if( sampleId==107650 || sampleId==107660 || sampleId==107670 || sampleId==107710 || sampleId==116946 || sampleId==116948    || sampleId==116940  ){
      if( var==ktfacUP ) return 611.87*1.39838/Np0_baseline;
      if( var==ktfacDOWN ) return 612.86*1.28321/Np0_baseline;
      if( var==qfacUP ) return 658.31*1.27776/Np0_baseline;
      if( var==qfacDOWN ) return 571.57*1.40407/Np0_baseline;
    }
    //Np1:    Zee               Zmumu               Ztautau             Znunu_filt1jet      Znunu_susyfilt   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    else if(sampleId==107651 || sampleId==107661 || sampleId==107671 || sampleId==107711 || sampleId==144192 || sampleId==116947 || sampleId==116949    || sampleId==116941 ){
      if( var==ktfacUP ) return 112.41*1.39838/Np1_baseline;
      if( var==ktfacDOWN ) return 147.18*1.28321/Np1_baseline;
      if( var==qfacUP ) return 128.25*1.27776/Np1_baseline;
      if( var==qfacDOWN ) return 131.63*1.40407/Np1_baseline;
    }
    //Np2:    Zee               Zmumu               Ztautau             Znunu_filt1jet      Znunu_susyfilt   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    else if(sampleId==107652 || sampleId==107662 || sampleId==107672 || sampleId==107712 || sampleId==144193 || sampleId==116252 || sampleId==116262    || sampleId==116942  ){
      if( var==ktfacUP ) return 30.499*1.39838/Np2_baseline;
      if( var==ktfacDOWN ) return 50.923*1.28321/Np2_baseline;
      if( var==qfacUP ) return 36.876*1.27776/Np2_baseline;
      if( var==qfacDOWN ) return 42.112*1.40407/Np2_baseline;
    }
    //Np3:    Zee               Zmumu               Ztautau             Znunu_filt1jet      Znunu_susyfilt   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    else if(sampleId==107653 || sampleId==107663 || sampleId==107673 || sampleId==107713 || sampleId==144194 || sampleId==116253 || sampleId==116263    || sampleId==116943  ){
      if( var==ktfacUP ) return 7.5203*1.39838/Np3_baseline;
      if( var==ktfacDOWN ) return 16.155*1.28321/Np3_baseline;
      if( var==qfacUP ) return 10.0*1.27776/Np3_baseline;
      if( var==qfacDOWN ) return 12.022*1.40407/Np3_baseline;
    }
    //Np4:    Zee               Zmumu               Ztautau             Znunu_filt1jet      Znunu_susyfilt   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    else if(sampleId==107654 || sampleId==107664 || sampleId==107674 || sampleId==107714 || sampleId==144195 || sampleId==116254 || sampleId==116264    || sampleId==116944  ){
      if( var==ktfacUP ) return 1.7130*1.39838/Np4_baseline;
      if( var==ktfacDOWN ) return 4.5332*1.28321/Np4_baseline;
      if( var==qfacUP ) return 2.4632*1.27776/Np4_baseline;
      if( var==qfacDOWN ) return 3.1289*1.40407/Np4_baseline;
    }
    //Np5:    Zee               Zmumu               Ztautau             Znunu_filt1jet   Zee_Mll10to40Pt15Pt5 Zmumu_Mll10to40Pt15Pt5  Ztautau_Mll10to40Pt15Pt5
    else if(sampleId==107655 || sampleId==107665 || sampleId==107675 || sampleId==107715 || sampleId==116255 || sampleId==116265    || sampleId==116945  ){
      if( var==ktfacUP ) return 0.4425*1.39838/Np5_baseline;
      if( var==ktfacDOWN ) return 1.4152*1.28321/Np5_baseline;
      if( var==qfacUP ) return 0.72051*1.27776/Np5_baseline;
      if( var==qfacDOWN ) return 0.89404*1.40407/Np5_baseline;
    }
	
    //W->lnu
    /* --------------------------------------
       /  LO xsec from AMI normalized to the tot cross section:
       /  10460 pb * 3 flavours (NNLO)
       /--------------------------------------*/
    Np0_baseline=6932.4*1.1953;
    Np1_baseline=1305.9*1.1953;
    Np2_baseline=378.07*1.1953;
    Np3_baseline=101.85*1.1953;
    Np4_baseline=25.720*1.1953;
    Np5_baseline=6.9999*1.1953;
  
    //Np0:    Wenu               Wmunu                Wtaunu
    if(sampleId==107680 || sampleId==107690 || sampleId==107700 ){
      if( var==ptminUP ) return 7350.*1.2054/Np0_baseline;
      if( var==ptminDOWN ) return 6878.*1.19448/Np0_baseline;
      if( var==IqoptUP ) return 6894.*1.20012/Np0_baseline;
      if( var==IqoptDOWN ) return 6890.*1.19683/Np0_baseline;
      if( var==ScalesUP ) return 7530.*1.15439/Np0_baseline;
      if( var==ScalesDOWN ) return 6366.*1.20409/Np0_baseline;
      if( var==qfacUPktfacDOWN ) return 7530.*1.07903/Np0_baseline;
      if( var==qfacDOWNktfacUP ) return 6368.*1.31086/Np0_baseline;
    }
    //Np1:  Wenu                Wmunu               Wtaunu              Wenu_susyfilt       Wmunu_susyfilt      Wtaunu_susyfilt
    else if(sampleId==107681 || sampleId==107691 || sampleId==107701 || sampleId==144196 || sampleId==144200 || sampleId==144204 ){
      if( var==ptminUP ) return 996.7*1.2054/Np1_baseline;
      if( var==ptminDOWN ) return 1328.*1.19448/Np1_baseline;
      if( var==IqoptUP ) return 1299.*1.20012/Np1_baseline;
      if( var==IqoptDOWN ) return 1309.*1.19683/Np1_baseline;
      if( var==ScalesUP ) return 1160.*1.15439/Np1_baseline;
      if( var==ScalesDOWN ) return 1546.*1.20409/Np1_baseline;
      if( var==qfacUPktfacDOWN ) return 1506.*1.07903/Np1_baseline;
      if( var==qfacDOWNktfacUP ) return 1181.*1.31086/Np1_baseline;
    }
    //Np2:  Wenu                Wmunu               Wtaunu              Wenu_susyfilt       Wmunu _susyfilt     Wtaunu_susyfilt
    else if(sampleId==107682 || sampleId==107692 || sampleId==107702 || sampleId==144197 || sampleId==144201 || sampleId==144205 ){
      if( var==ptminUP ) return 256.1*1.2054/Np2_baseline;
      if( var==ptminDOWN ) return 401.7*1.19448/Np2_baseline;
      if( var==IqoptUP ) return 382.8*1.20012/Np2_baseline;
      if( var==IqoptDOWN ) return 393.7*1.19683/Np2_baseline;
      if( var==ScalesUP ) return 285.5*1.15439/Np2_baseline;
      if( var==ScalesDOWN ) return 542.4*1.20409/Np2_baseline;
      if( var==qfacUPktfacDOWN ) return 466.*1.07903/Np2_baseline;
      if( var==qfacDOWNktfacUP ) return 327.1*1.31086/Np2_baseline;
    }
    //Np3:  Wenu                Wmunu               Wtaunu              Wenu_susyfilt       Wmunu _susyfilt     Wtaunu_susyfilt
    else if(sampleId==107683 || sampleId==107693 || sampleId==107703 || sampleId==144198 || sampleId==144202 || sampleId==144206 ){
      if( var==ptminUP ) return 59.46*1.2054/Np3_baseline;
      if( var==ptminDOWN ) return 112.1*1.19448/Np3_baseline;
      if( var==IqoptUP ) return 105.*1.20012/Np3_baseline;
      if( var==IqoptDOWN ) return 109.7*1.19683/Np3_baseline;
      if( var==ScalesUP ) return 67.28*1.15439/Np3_baseline;
      if( var==ScalesDOWN ) return 168.3*1.20409/Np3_baseline;
      if( var==qfacUPktfacDOWN ) return 141.2*1.07903/Np3_baseline;
      if( var==qfacDOWNktfacUP ) return 80.38*1.31086/Np3_baseline;
    }
    //Np4:  Wenu                Wmunu               Wtaunu              Wenu_susyfilt       Wmunu _susyfilt     Wtaunu_susyfilt
    else if(sampleId==107684 || sampleId==107694 || sampleId==107704 || sampleId==144199 || sampleId==144203 || sampleId==144207 ){
      if( var==ptminUP ) return 12.52*1.2054/Np4_baseline;
      if( var==ptminDOWN ) return 28.55*1.19448/Np4_baseline;
      if( var==IqoptUP ) return 27.17*1.20012/Np4_baseline;
      if( var==IqoptDOWN ) return 28.64*1.19683/Np4_baseline;
      if( var==ScalesUP ) return 14.62*1.15439/Np4_baseline;
      if( var==ScalesDOWN ) return 48.75*1.20409/Np4_baseline;
      if( var==qfacUPktfacDOWN ) return 38.63*1.07903/Np4_baseline;
      if( var==qfacDOWNktfacUP ) return 18.33*1.31086/Np4_baseline;
    }
    //Np5:    Wenu               Wmunu                Wtaunu
    else if(sampleId==107685 || sampleId==107695 || sampleId==107705 ){
      if( var==ptminUP ) return 2.861*1.2054/Np5_baseline;
      if( var==ptminDOWN ) return 8.585*1.19448/Np5_baseline;
      if( var==IqoptUP ) return 7.8090*1.20012/Np5_baseline;
      if( var==IqoptDOWN ) return 8.693*1.19683/Np5_baseline;
      if( var==ScalesUP ) return 3.685*1.15439/Np5_baseline;
      if( var==ScalesDOWN ) return 15.64*1.20409/Np5_baseline;
      if( var==qfacUPktfacDOWN ) return 12.04*1.07903/Np5_baseline;
      if( var==qfacDOWNktfacUP ) return 4.675*1.31086/Np5_baseline;
    }
	
    //top AlpGen lnln 
    /* --------------------------------------
       /  Tot cross section ttbar full leptonic:
       /  166.78 * (3*0.108)^2  = 17.5079 pb
       /--------------------------------------*/
    Np0_baseline=3.4704*1.68452;
    Np1_baseline=3.4051*1.68452;
    Np2_baseline=2.1093*1.68452;
    Np3_baseline=0.94698*1.68452;
    Np4_baseline=0.33409*1.68452;
    Np5_baseline=0.12753*1.68452;
	
    if(sampleId==105890){
      if( var==ktfacUP ) return 3.42*1.93265/Np0_baseline;
      if( var==ktfacDOWN ) return 3.391*1.46009/Np0_baseline;
      if( var==qfacUP ) return 2.62*2.2773/Np0_baseline;
      if( var==qfacDOWN ) return 4.705*1.22604/Np0_baseline;
    }
    else if(sampleId==105891){
      if( var==ktfacUP ) return 2.964*1.93265/Np1_baseline;
      if( var==ktfacDOWN ) return 3.86*1.46009/Np1_baseline;
      if( var==qfacUP ) return 2.54*2.2773/Np1_baseline;
      if( var==qfacDOWN ) return 4.769*1.22604/Np1_baseline;
    }
    else if(sampleId==105892){
      if( var==ktfacUP ) return 1.75*1.93265/Np2_baseline;
      if( var==ktfacDOWN ) return 2.743*1.46009/Np2_baseline;
      if( var==qfacUP ) return 1.562*2.2773/Np2_baseline;
      if( var==qfacDOWN ) return 2.957*1.22604/Np2_baseline;
    }
    else if(sampleId==117897 ){
      if( var==ktfacUP ) return 0.649*1.93265/Np3_baseline;
      if( var==ktfacDOWN ) return 1.336*1.46009/Np3_baseline;
      if( var==qfacUP ) return 0.676*2.2773/Np3_baseline;
      if( var==qfacDOWN ) return 1.311*1.22604/Np3_baseline;
    }
    else if(sampleId==117898 ){
      if( var==ktfacUP ) return 0.195*1.93265/Np4_baseline;
      if( var==ktfacDOWN ) return 0.508*1.46009/Np4_baseline;
      if( var==qfacUP ) return 0.230*2.2773/Np4_baseline;
      if( var==qfacDOWN ) return 0.419*1.22604/Np4_baseline;
    }
    else if(sampleId==117899 ){
      if( var==ktfacUP ) return 0.081*1.93265/Np5_baseline;
      if( var==ktfacDOWN ) return 0.153*1.46009/Np5_baseline;
      if( var==qfacUP ) return 0.06*2.2773/Np5_baseline;
      if( var==qfacDOWN ) return 0.119*1.22604/Np5_baseline;
    }
	
    //top AlpGen lnqq
    /* --------------------------------------
       /  Tot cross section ttbar full leptonic:
       /  166.78 * (2*3*0.108*0.676)  = 73.0576 pb
       /--------------------------------------*/  
    Np0_baseline=13.844*1.75634;
    Np1_baseline=13.671*1.75634;
    Np2_baseline=8.4661*1.75634;
    Np3_baseline=3.7759*1.75634;
    Np4_baseline=1.3355*1.75634;
    Np5_baseline=0.50399*1.75634; 
	
    if(sampleId==105894){
      if( var==ktfacUP ) return 13.563*2.03537/Np0_baseline;
      if( var==ktfacDOWN ) return 13.578*1.52346/Np0_baseline;
      if( var==qfacUP ) return 10.60*2.37547/Np0_baseline;
      if( var==qfacDOWN ) return 18.685*1.2884/Np0_baseline;
    }
    else if(sampleId==105895){
      if( var==ktfacUP ) return 12.03*2.03537/Np1_baseline;
      if( var==ktfacDOWN ) return 15.406*1.52346/Np1_baseline;
      if( var==qfacUP ) return 10.223*2.37547/Np1_baseline;
      if( var==qfacDOWN ) return 18.897*1.2884/Np1_baseline;
    }
    else if(sampleId==105896){
      if( var==ktfacUP ) return 6.715*2.03537/Np2_baseline;
      if( var==ktfacDOWN ) return 10.894*1.52346/Np2_baseline;
      if( var==qfacUP ) return 6.125*2.37547/Np2_baseline;
      if( var==qfacDOWN ) return 11.817*1.2884/Np2_baseline;
    }
    else if(sampleId==117887 ){
      if( var==ktfacUP ) return 2.628*2.03537/Np3_baseline;
      if( var==ktfacDOWN ) return 5.446*1.52346/Np3_baseline;
      if( var==qfacUP ) return 2.685*2.37547/Np3_baseline;
      if( var==qfacDOWN ) return 5.104*1.2884/Np3_baseline;
    }
    else if(sampleId==117888 ){
      if( var==ktfacUP ) return 0.771*2.03537/Np4_baseline;
      if( var==ktfacDOWN ) return 2.024*1.52346/Np4_baseline;
      if( var==qfacUP ) return 0.884*2.37547/Np4_baseline;
      if( var==qfacDOWN ) return 1.776*1.2884/Np4_baseline;
    }
    else if(sampleId==117889 ){
      if( var==ktfacUP ) return 0.187*2.03537/Np5_baseline;
      if( var==ktfacDOWN ) return 0.607*1.52346/Np5_baseline;
      if( var==qfacUP ) return 0.238*2.37547/Np5_baseline;
      if( var==qfacDOWN ) return 0.425*1.2884/Np5_baseline;
    }	
  }
  // NUMBERS FOR MC12
  else if(mc == mc12) {
    if(var==None) return 1.;
    //W->lnu
    //Np0:    Wenu               Wmunu             Wtaunu  (inclusive)
    if(sampleId==107680 || sampleId==107690 || sampleId==107700 ||
       sampleId==117680 || sampleId==117690 || sampleId==117700 ||
       // Zee              Zmumu               Ztautau
       sampleId==107650 || sampleId==107660 || sampleId==107670 ||
       sampleId==117650 || sampleId==117660 || sampleId==117670 ||
       // Znunu
       sampleId==156803 ||
       // DY ee            DY mumu             DY tautau
       sampleId==146830 || sampleId==146840 || sampleId==146850 ||
       sampleId==146860 || sampleId==146870 || sampleId==146880 ){
      if( var==ktfacUP ) return 1.0341;
      if( var==ktfacDOWN ) return 0.9588;
      if( var==qfacUP ) return 1.0191;
      if( var==qfacDOWN ) return 0.9689;
    }
    //Np1:  Wenu                Wmunu               Wtaunu  (inclusive + susyfilt)
    else if(sampleId==107681 || sampleId==107691 || sampleId==107701 || 
	    sampleId==117681 || sampleId==117691 || sampleId==117701 || 
	    sampleId==172001 || sampleId==172011 || sampleId==172021 ||
	    // Zee              Zmumu               Ztautau
	    sampleId==107651 || sampleId==107661 || sampleId==107671 ||
	    sampleId==117651 || sampleId==117661 || sampleId==117671 ||
	    // Znunu
	    sampleId==156804 ||
	    // DY ee            DY mumu             DY tautau
	    sampleId==146831 || sampleId==146841 || sampleId==146851 ||
	    sampleId==146861 || sampleId==146871 || sampleId==146881){
      if( var==ktfacUP ) return 0.9152;
      if( var==ktfacDOWN ) return 1.0790;
      if( var==qfacUP ) return 0.9420;
      if( var==qfacDOWN ) return 1.0903;
    }
    //Np2:  Wenu                Wmunu               Wtaunu  (inclusive + susyfilt)
    else if(sampleId==107682 || sampleId==107692 || sampleId==107702 || 
	    sampleId==117682 || sampleId==117692 || sampleId==117702 || 
	    sampleId==172002 || sampleId==172012 || sampleId==172022 ||
	    // Zee              Zmumu               Ztautau
	    sampleId==107652 || sampleId==107662 || sampleId==107672 ||
	    sampleId==117652 || sampleId==117662 || sampleId==117672 ||
	    // Znunu
	    sampleId==156809 ||
	    // DY ee            DY mumu             DY tautau
	    sampleId==146832 || sampleId==146842 || sampleId==146852 ||
	    sampleId==146862 || sampleId==146872 || sampleId==146882 ){
      if( var==ktfacUP ) return 0.8170;
      if( var==ktfacDOWN ) return 1.2515;
      if( var==qfacUP ) return 0.8979;
      if( var==qfacDOWN ) return 1.1539;
    }
    //Np3:  Wenu                Wmunu               Wtaunu  (inclusive + susyfilt)
    else if(sampleId==107683 || sampleId==107693 || sampleId==107703 || 
	    sampleId==117683 || sampleId==117693 || sampleId==117703 ||
	    sampleId==172003 || sampleId==172013 || sampleId==172023 ||
	    // Zee              Zmumu               Ztautau
	    sampleId==107653 || sampleId==107663 || sampleId==107673 ||
	    sampleId==117653 || sampleId==117663 || sampleId==117673 ||
	    // Znunu
	    sampleId==156814 ||
	    // DY ee            DY mumu             DY tautau
	    sampleId==146833 || sampleId==146843 || sampleId==146853 ||
	    sampleId==146863 || sampleId==146873 || sampleId==146883 ){
      if( var==ktfacUP ) return 0.7337;
      if( var==ktfacDOWN ) return 1.4199;
      if( var==qfacUP ) return 0.9504;
      if( var==qfacDOWN ) return 1.1851;
    }
    //Np4:  Wenu                Wmunu               Wtaunu  (inclusive + susyfilt)
    else if(sampleId==107684 || sampleId==107694 || sampleId==107704 || 
	    sampleId==117684 || sampleId==117694 || sampleId==117704 || 
	    sampleId==172004 || sampleId==172014 || sampleId==172024 ||
	    // Zee              Zmumu               Ztautau
	    sampleId==107654 || sampleId==107664 || sampleId==107674 ||
	    sampleId==117654 || sampleId==117664 || sampleId==117674 ||
	    // Znunu
	    sampleId==156819 ||
	    // DY ee            DY mumu             DY tautau
	    sampleId==146834 || sampleId==146844 || sampleId==146854 ||
	    sampleId==146864 || sampleId==146874 || sampleId==146884){
      if( var==ktfacUP ) return 0.6542;
      if( var==ktfacDOWN ) return 1.6099;
      if( var==qfacUP ) return 0.8671;
      if( var==qfacDOWN ) return 1.2059;
    }
    //Np5:    Wenu               Wmunu                Wtaunu  (inclusive + susyfilt)
    else if(sampleId==107685 || sampleId==107695 || sampleId==107705 || 	
	    sampleId==117685 || sampleId==117695 || sampleId==117705 || 
	    sampleId==172005 || sampleId==172015 || sampleId==172025 ||
	    // Zee              Zmumu               Ztautau
	    sampleId==107655 || sampleId==107665 || sampleId==107675 ||
	    sampleId==117655 || sampleId==117665 || sampleId==117675 ||
	    // Znunu
	    sampleId==156824 ||
	    // DY ee            DY mumu             DY tautau
	    sampleId==146835 || sampleId==146845 || sampleId==146855 ||
	    sampleId==146865 || sampleId==146875 || sampleId==146885 ){
      if( var==ktfacUP ) return 0.5650;
      if( var==ktfacDOWN ) return 1.8046;
      if( var==qfacUP ) return 0.8324;
      if( var==qfacDOWN ) return 1.2157;
    }
    //Np6:    Wenu               Wmunu                Wtaunu  (susyfilt)
    else if(sampleId==172006 || sampleId==172016 || sampleId==172026 ){
      if( var==ktfacUP ) return 0.5650;
      if( var==ktfacDOWN ) return 1.8046;
      if( var==qfacUP ) return 0.8324;
      if( var==qfacDOWN ) return 1.2157;
    }
    //top AlpGen lnln 
    else if(sampleId==164440){  // NP0 excl
      if( var==ktfacUP ) return 1.1443;
      if( var==ktfacDOWN ) return 0.8365;
      if( var==qfacUP ) return 1.0229;
      if( var==qfacDOWN ) return 0.9830;
    }
    else if(sampleId==164441){  // NP1 excl
      if( var==ktfacUP ) return 1.0228;
      if( var==ktfacDOWN ) return 0.9610;
      if( var==qfacUP ) return 1.0044;
      if( var==qfacDOWN ) return 1.0010;
    }
    else if(sampleId==164442){ // NP2 excl
      if( var==ktfacUP ) return 0.9134;
      if( var==ktfacDOWN ) return 1.0989;
      if( var==qfacUP ) return 1.0072;
      if( var==qfacDOWN ) return 1.0141;
    }
    else if(sampleId==164443){ // NP3 incl
      if( var==ktfacUP ) return 0.7725;
      if( var==ktfacDOWN ) return 1.2853;
      if( var==qfacUP ) return 0.9331;
      if( var==qfacDOWN ) return 1.0133;
    }
    
    else if(sampleId==164444 ){ // NP3 excl
      if( var==ktfacUP ) return 0.8209;
      if( var==ktfacDOWN ) return 1.2212;
      if( var==qfacUP ) return 0.9759;
      if( var==qfacDOWN ) return 1.0224;
    }
    else if(sampleId==164445 ){ // NP4 incl
      if( var==ktfacUP ) return 0.6830;
      if( var==ktfacDOWN ) return 1.4038;
      if( var==qfacUP ) return 0.8539;
      if( var==qfacDOWN ) return 0.9965;
    }
    //top AlpGen lnqq
    else if(sampleId==164450){ // NP0 excl
      if( var==ktfacUP ) return 1.1465;
      if( var==ktfacDOWN ) return 0.8384;
      if( var==qfacUP ) return 1.0226;
      if( var==qfacDOWN ) return 0.9904;
    }
    else if(sampleId==164451){ // NP1 excl
      if( var==ktfacUP ) return 1.0172;
      if( var==ktfacDOWN ) return 0.9551;
      if( var==qfacUP ) return 0.9875;
      if( var==qfacDOWN ) return 0.9865;
    }
    else if(sampleId==164452){ // NP2 excl
      if( var==ktfacUP ) return 0.9204;
      if( var==ktfacDOWN ) return 1.1080;
      if( var==qfacUP ) return 1.0088;
      if( var==qfacDOWN ) return 1.0301;
    }
    else if(sampleId==164453){ // NP3 incl
      if( var==ktfacUP ) return 0.7712;
      if( var==ktfacDOWN ) return 1.2814;
      if( var==qfacUP ) return 0.9689;
      if( var==qfacDOWN ) return 1.0077;
    }
    else if(sampleId==164454 ){ // NP3 excl
      if( var==ktfacUP ) return 0.7951;
      if( var==ktfacDOWN ) return 1.2242;
      if( var==qfacUP ) return 0.9629;
      if( var==qfacDOWN ) return 1.0010;
    }
    else if(sampleId==164455 ){ // NP4 incl
      if( var==ktfacUP ) return 0.7249;
      if( var==ktfacDOWN ) return 1.3924;
      if( var==qfacUP ) return 0.9806;
      if( var==qfacDOWN ) return 1.0206;
    }	
  }
  return 1.;	
}
	
	
