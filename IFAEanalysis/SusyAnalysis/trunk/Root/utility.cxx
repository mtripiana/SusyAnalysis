#include "SusyAnalysis/utility.h"

float m_calibConstants[45][4] = {{-0.676568,-6.73995,21.8839,-19.9172},{-0.645147,-6.25005,18.6959,-15.3563},{-0.48034,-7.83875,24.2973,-22.0308},{-0.518052,-7.44206,22.7878,-20.0835},{-0.47104,-7.51981,21.7959,-17.9213},{-0.469625,-7.85256,23.6274,-20.6898},{-0.297181,-9.5623,28.5822,-25.3111},{-0.375276,-9.98404,32.0187,-30.423},{-0.821604,-5.94113,18.547,-15.1776},{-1.10474,-3.72317,13.0692,-11.2257},{-0.057028,-12.5764,38.5174,-35.7054},{0.456398,-16.619,49.7,-46.4188},{0.191065,-14.4411,43.3863,-40.1411},{-0.670523,-7.80262,24.8267,-21.8666},{-1.59944,-0.754052,6.83622,-6.6323},{-0.781707,-4.59825,11.5363,-6.32582},{-0.580922,-5.37769,13.6154,-9.0677},{-0.274334,-6.85484,16.0482,-9.96231},{-0.210738,-7.2436,18.2536,-13.5124},{-0.142391,-7.59879,19.066,-13.9389},{-0.371412,-5.36762,12.3751,-7.20744},{-0.403531,-4.99653,11.7236,-6.91864},{-0.23515,-6.75943,18.5876,-15.2072},{-0.283737,-6.17728,17.0107,-13.6622},{-0.299086,-6.73421,20.4318,-18.2762},{-0.943061,-1.21085,4.5216,-2.48307},{-0.48727,-5.07157,16.0091,-13.8729},{-0.911438,-1.28576,4.47514,-2.35127},{-0.626118,-4.21669,13.4081,-11.3929},{-0.963458,-1.94427,7.33021,-5.96605},{-1.13151,-1.56778,6.78688,-5.23567},{-1.57391,-1.7708,13.467,-14.828},{-3.39539,9.70605,-11.8343,4.5743},{-3.95261,17.5622,-39.5824,34.3734},{0.0318764,-10.1119,29.3263,-25.2184},{-0.344382,-3.12503,4.80667,0.00960191},{-0.265061,-4.17192,10.5905,-8.12998},{-0.992244,1.66835,-5.14251,6.57914},{-1.4656,5.68716,-17.1531,19.0366},{-1.13527,1.82556,-3.67579,4.48026},{-2.05429,9.43394,-23.6326,21.6411},{-3.18889,19.2942,-50.9616,46.7402},{-0.315742,-4.09849,12.3794,-9.94372},{0.569235,-13.3529,41.7681,-39.8164},{-0.568405,-3.98445,15.0592,-16.0003}};

TString bold(TString t){
  return "\x1b[1m"+t+"\x1b[0m";
}

TString red(TString t){
  return "\x1b[31m"+t+"\x1b[0m";
}

float MinimumOf(float a, float b){
  float min=0.;
  (a < b) ? min=a : min=b;
  return min;
}

bool checkNonFiltered(int run){
  if(run>=107711 && run<=107714){return 1;} //Znunu Np1 --> Np4
  if(run>=107681 && run<=107684){return 1;} //Wenu Np1 --> Np4
  if(run>=107691 && run<=107694){return 1;} //Wmunu Np1 --> Np4
  if(run>=107701 && run<=107704){return 1;} //Wtaunu Np1 --> Np4
  return 0;
}

bool DoIHaveToKeepIt(int run, float var){
  //Sherpa W+jets samples
  if((run==147774 || run==147775 || run==147776 ) && var<200.){return 1;} //samples with no cut
  if( run==157534 || run==157535 || run==157536 ){return 1;} //samples with var>200

  //AlpGen and Sherpa Znn + jets
  if((run==156803 || run==156804 || run==156809 || run==156814 || run==156819 || run==156824 || run==147773) && var<70.){return 1;} //samples with no cut
//  if((               run==156808 || run==156813 || run==156818 || run==156823 || run==156828 || run==157537) && var<140.){return 1;} //samples with var>70
  if((               run==156808 || run==156813 || run==156818 || run==156823 || run==156828 || run==157537) && var<280.){return 1;} //samples with var>70
//  if((               run==156805 || run==156810 || run==156815 || run==156820 || run==156825 || run==157538) && var<280.){return 1;}  //samples with var>140
  if((               run==156806 || run==156811 || run==156816 || run==156821 || run==156826 || run==157539) && var<500.){return 1;}  //samples with var>280
  if(                run==156807 || run==156812 || run==156817 || run==156822 || run==156827 || run==157540){return 1;}  //samples with var>500

/*
  //AlpGen and Sherpa Znn + jets
  if((run==156803 || run==156804 || run==156809 || run==156814 || run==156819 || run==156824 || run==147773) && var<70.){return 1;} //samples with no cut
  if((               run==156808 || run==156813 || run==156818 || run==156823 || run==156828 || run==157537) && var<140.){return 1;} //samples with var>70
  if((               run==156805 || run==156810 || run==156815 || run==156820 || run==156825 || run==157538) && var<280.){return 1;}  //samples with var>140
  if((               run==156806 || run==156811 || run==156816 || run==156821 || run==156826 || run==157539) && var<500.){return 1;}  //samples with var>280
  if(                run==156807 || run==156812 || run==156817 || run==156822 || run==156827 || run==157540){return 1;}  //samples with var>500
*/
  //Sherpa p1328
  if((run>=167150 && run<=167155) && var<140.){return 1;} // WxnuMassiveBPt0_BFilter, WxnuMassiveBPt0_VetoB
  if((run>=167156 && run<=167161) && var>=140.){return 1;} // WxnuMassiveBPt140_BFilter, WxnuMassiveBPt140_VetoB

  if((run>=167740 && run<=167760) && var<70.) {return 1;} // W, Z --> MassiveCB --> Pt0 --> BFilter, CJetFilterBVeto, CJetVetoBVeto. Only one that needs to be fixed.
  if((run>=167761 && run<=167844))            {return 1;} // W, Z --> MassiveCB --> PtXPtY --> BFilter, CJetFilterBVeto, CJetVetoBVeto. Just to have the bos_pt distribution.

  // low invariantmass DY samples

  if (run>=173041 && run<=173046) {return 1;}


  //--- Filtered signal samples:
  if((run==157851 && var<=220.) || (run==176266 && var>220. && var<=400.) || (run==176267 && var>400.)) {return 1;} 
  if((run==157852 && var<=220.) || (run==176268 && var>220. && var<=400.) || (run==176269 && var>400.)) {return 1;} 
  if((run==157853 && var<=220.) || (run==176270 && var>220. && var<=400.) || (run==176271 && var>400.)) {return 1;} // t100 n85
  if((run==157854 && var<=220.) || (run==176272 && var>220. && var<=400.) || (run==176273 && var>400.)) {return 1;} 
  if((run==157855 && var<=220.) || (run==176274 && var>220. && var<=400.) || (run==176275 && var>400.)) {return 1;}

  if((run==157856 && var<=220.) || (run==176276 && var>220. && var<=400.) || (run==176277 && var>400.)) {return 1;}
  if((run==157857 && var<=220.) || (run==176278 && var>220. && var<=400.) || (run==176279 && var>400.)) {return 1;} 
  if((run==157858 && var<=220.) || (run==176280 && var>220. && var<=400.) || (run==176281 && var>400.)) {return 1;} 
  if((run==157859 && var<=220.) || (run==176282 && var>220. && var<=400.) || (run==176283 && var>400.)) {return 1;}
  if((run==157860 && var<=220.) || (run==176284 && var>220. && var<=400.) || (run==176285 && var>400.)) {return 1;}

  if((run==157861 && var<=220.) || (run==176286 && var>220. && var<=400.) || (run==176287 && var>400.)) {return 1;}
  if((run==157862 && var<=220.) || (run==176288 && var>220. && var<=400.) || (run==176289 && var>400.)) {return 1;}
  if((run==157863 && var<=220.) || (run==176290 && var>220. && var<=400.) || (run==176291 && var>400.)) {return 1;}
  if((run==157864 && var<=220.) || (run==176292 && var>220. && var<=400.) || (run==176293 && var>400.)) {return 1;}
  if((run==157865 && var<=220.) || (run==176294 && var>220. && var<=400.) || (run==176295 && var>400.)) {return 1;}
  if((run==157866 && var<=220.) || (run==176296 && var>220. && var<=400.) || (run==176297 && var>400.)) {return 1;}

  if((run==157867 && var<=220.) || (run==176298 && var>220. && var<=400.) || (run==176299 && var>400.)) {return 1;}
  if((run==157868 && var<=220.) || (run==176300 && var>220. && var<=400.) || (run==176301 && var>400.)) {return 1;}
  if((run==157869 && var<=220.) || (run==176302 && var>220. && var<=400.) || (run==176303 && var>400.)) {return 1;}
  if((run==157870 && var<=220.) || (run==176304 && var>220. && var<=400.) || (run==176305 && var>400.)) {return 1;}
  if((run==157871 && var<=220.) || (run==176306 && var>220. && var<=400.) || (run==176307 && var>400.)) {return 1;}
  if((run==157872 && var<=220.) || (run==176308 && var>220. && var<=400.) || (run==176309 && var>400.)) {return 1;}

  if((run==157873 && var<=220.) || (run==176310 && var>220. && var<=400.) || (run==176311 && var>400.)) {return 1;}
  if((run==157874 && var<=220.) || (run==176312 && var>220. && var<=400.) || (run==176313 && var>400.)) {return 1;}
  if((run==157875 && var<=220.) || (run==176314 && var>220. && var<=400.) || (run==176315 && var>400.)) {return 1;}
  if((run==157876 && var<=220.) || (run==176316 && var>220. && var<=400.) || (run==176317 && var>400.)) {return 1;}
  if((run==157877 && var<=220.) || (run==176318 && var>220. && var<=400.) || (run==176319 && var>400.)) {return 1;}
  if((run==157878 && var<=220.) || (run==176320 && var>220. && var<=400.) || (run==176321 && var>400.)) {return 1;}

  return 0;
}


float calib(float pt, float eta){
  int eta_i = (int)(10*fabs(eta));
 if(eta_i>44){eta_i = 44;}
 float log_jet_pt = log(pt/1000.);
 if(pt<10000.){log_jet_pt = log(10.);}

 float R = 1.;
 float a1 = m_calibConstants[eta_i][0];
 float a2 = m_calibConstants[eta_i][1];
 float a3 = m_calibConstants[eta_i][2];
 float a4 = m_calibConstants[eta_i][3];

 if(log_jet_pt != 0)
   {
     R += a1/log_jet_pt;
     R += a2/pow(log_jet_pt,2);
     R += a3/pow(log_jet_pt,3);
     R += a4/pow(log_jet_pt,4);
   }

 R = (1./R);
 return R;
}

float rapidity(float e, float m, float eta){
  float p=sqrt((e*e)-(m*m));
  float pz=p*tanh(eta);
  float rap=0.5*log((e+pz)/(e-pz));
  return rap;
}


float deltaPhi(Float_t phi1,Float_t phi2){
  Float_t deltaPhi=phi1-phi2;
  Float_t sign=deltaPhi/fabs(deltaPhi);
  if(fabs(deltaPhi)>TMath::Pi()){deltaPhi=2*TMath::Pi()*sign-deltaPhi;}
  return fabs(deltaPhi);
}

float deltaR(Float_t phi1,Float_t eta1,Float_t phi2,Float_t eta2){
  Float_t dPhi=deltaPhi(phi1,phi2);
  Float_t dEta=fabs(eta1-eta2);
  return sqrt(dPhi*dPhi + dEta*dEta);
}

bool isBeamHalo(int run=0,int event=0){ //CHECK_ME
  bool isBH=0;
  //  std::cout<<run<<"\t"<<event<<std::endl;
  if(run==179804 && event==25569828){isBH=1;return isBH;}
  if(run==179804 && event==58049650){isBH=1;return isBH;}
  if(run==180124 && event==19339070){isBH=1;return isBH;}
  if(run==180124 && event==43983153){isBH=1;return isBH;}
  if(run==180139 && event==71949585){isBH=1;return isBH;}
  if(run==180144 && event==7513531){isBH=1;return isBH;}
  if(run==180149 && event==11599878){isBH=1;return isBH;}
  if(run==180164 && event==155811122){isBH=1;return isBH;}
  if(run==180225 && event==107576587){isBH=1;return isBH;}
  if(run==180400 && event==65602861){isBH=1;return isBH;}
  if(run==180448 && event==4000428){isBH=1;return isBH;}
  if(run==180481 && event==26550624){isBH=1;return isBH;}
  if(run==180636 && event==23004421){isBH=1;return isBH;}
  if(run==180636 && event==34834356){isBH=1;return isBH;}
  if(run==180636 && event==50254258){isBH=1;return isBH;}
  if(run==180710 && event==35431006){isBH=1;return isBH;}
  if(run==180710 && event==37833641){isBH=1;return isBH;}
  if(run==182284 && event==91229245){isBH=1;return isBH;}
  if(run==182424 && event==49106874){isBH=1;return isBH;}
  if(run==182454 && event==3852905){isBH=1;return isBH;}
  if(run==182454 && event==47919953){isBH=1;return isBH;}
  if(run==182456 && event==116320663){isBH=1;return isBH;}
  if(run==182518 && event==5129826){isBH=1;return isBH;}
  if(run==182787 && event==38730893){isBH=1;return isBH;}
  if(run==182796 && event==13977392){isBH=1;return isBH;}
  if(run==182796 && event==24734882){isBH=1;return isBH;}
  if(run==182879 && event==18185422){isBH=1;return isBH;}
  if(run==183286 && event==77892314){isBH=1;return isBH;}
  if(run==183462 && event==1675840){isBH=1;return isBH;}
  if(run==183581 && event==54371679){isBH=1;return isBH;}
  if(run==184130 && event==209030514){isBH=1;return isBH;}
  if(run==186399 && event==3076863){isBH=1;return isBH;}
  if(run==186493 && event==5614942){isBH=1;return isBH;}
  if(run==186669 && event==3147076){isBH=1;return isBH;}
  if(run==186673 && event==54079860){isBH=1;return isBH;}
  if(run==186877 && event==74542417){isBH=1;return isBH;}
  if(run==187014 && event==70112686){isBH=1;return isBH;}
  if(run==187014 && event==83281842){isBH=1;return isBH;}
  if(run==187219 && event==92420097){isBH=1;return isBH;}
  if(run==189049 && event==66810203){isBH=1;return isBH;}
  if(run==189280 && event==19675947){isBH=1;return isBH;}
  if(run==189421 && event==40697471){isBH=1;return isBH;}
  if(run==189421 && event==44163286){isBH=1;return isBH;}
  if(run==189561 && event==68816684){isBH=1;return isBH;}
  if(run==189598 && event==9572526){isBH=1;return isBH;}
  if(run==189598 && event==76832225){isBH=1;return isBH;}
  if(run==189875 && event==4440151){isBH=1;return isBH;}
  if(run==189965 && event==8877734){isBH=1;return isBH;}
  if(run==190236 && event==61048051){isBH=1;return isBH;}
  if(run==190256 && event==30793053){isBH=1;return isBH;}
  if(run==190300 && event==60832471){isBH=1;return isBH;}
  if(run==190611 && event==18615342){isBH=1;return isBH;}
  if(run==190611 && event==25855344){isBH=1;return isBH;}
  if(run==190618 && event==29113731){isBH=1;return isBH;}
  if(run==190618 && event==64111225){isBH=1;return isBH;}
  if(run==190643 && event==2271173){isBH=1;return isBH;}
  if(run==190643 && event==13407982){isBH=1;return isBH;}
  if(run==190644 && event==51663925){isBH=1;return isBH;}
  if(run==190872 && event==64245675){isBH=1;return isBH;}
  if(run==190933 && event==5310947){isBH=1;return isBH;}
  if(run==190933 && event==135797847){isBH=1;return isBH;}
  if(run==191150 && event==23841046){isBH=1;return isBH;}
  if(run==191190 && event==2127279){isBH=1;return isBH;}
  if(run==191190 && event==5773247){isBH=1;return isBH;}
  if(run==191190 && event==117421360){isBH=1;return isBH;}
  if(run==191190 && event==128973146){isBH=1;return isBH;}
  if(run==191190 && event==149415555){isBH=1;return isBH;}
  
  return isBH;
}
  
bool goodLumiBlock(int rn, int lbn){

  bool good = 0;

  //MC
  if(rn>105000 && rn<150000){ good = 1; return good;    }

  //DATA 2011
  if(rn==191933){
    if ((lbn >= 87) && (lbn <=110)){      good = 1; return good;    }
    if ((lbn >= 112) && (lbn <=188)){      good = 1; return good;    }
    if ((lbn >= 191) && (lbn <=315)){      good = 1; return good;    }
    if ((lbn >= 317) && (lbn <=320)){      good = 1; return good;    }
  }
      
  if(rn==191920){
    if ((lbn >= 254) && (lbn <=297)){      good = 1; return good;    }
    if ((lbn >= 299) && (lbn <=331)){      good = 1; return good;    }
    if ((lbn >= 333) && (lbn <=352)){      good = 1; return good;    }
    if ((lbn >= 354) && (lbn <=397)){      good = 1; return good;    }
  }
      
  if(rn==191715){
    if ((lbn >= 78) && (lbn <=102)){      good = 1; return good;    }
    if ((lbn >= 104) && (lbn <=135)){      good = 1; return good;    }
    if ((lbn >= 137) && (lbn <=154)){      good = 1; return good;    }
    if ((lbn >= 156) && (lbn <=212)){      good = 1; return good;    }
    if ((lbn >= 214) && (lbn <=253)){      good = 1; return good;    }
    if ((lbn >= 255) && (lbn <=333)){      good = 1; return good;    }
    if ((lbn >= 335) && (lbn <=391)){      good = 1; return good;    }
  }
      
  if(rn==191676){
    if ((lbn >= 377) && (lbn <=381)){      good = 1; return good;    }
    if ((lbn >= 679) && (lbn <=681)){      good = 1; return good;    }
    if ((lbn >= 683) && (lbn <=693)){      good = 1; return good;    }
    if ((lbn >= 695) && (lbn <=697)){      good = 1; return good;    }
    if ((lbn >= 699) && (lbn <=715)){      good = 1; return good;    }
  }
      
  if(rn==191635){
    if ((lbn >= 568) && (lbn <=579)){      good = 1; return good;    }
    if ((lbn >= 581) && (lbn <=581)){      good = 1; return good;    }
    if ((lbn >= 583) && (lbn <=585)){      good = 1; return good;    }
    if ((lbn >= 587) && (lbn <=590)){      good = 1; return good;    }
    if ((lbn >= 592) && (lbn <=630)){      good = 1; return good;    }
    if ((lbn >= 632) && (lbn <=686)){      good = 1; return good;    }
    if ((lbn >= 688) && (lbn <=722)){      good = 1; return good;    }
    if ((lbn >= 724) && (lbn <=751)){      good = 1; return good;    }
  }
      
  if(rn==191517){
    if ((lbn >= 4) && (lbn <=55)){      good = 1; return good;    }
    if ((lbn >= 57) && (lbn <=77)){      good = 1; return good;    }
    if ((lbn >= 79) && (lbn <=230)){      good = 1; return good;    }
  }
      
  if(rn==191513){
    if ((lbn >= 220) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 243) && (lbn <=243)){      good = 1; return good;    }
    if ((lbn >= 245) && (lbn <=273)){      good = 1; return good;    }
    if ((lbn >= 328) && (lbn <=359)){      good = 1; return good;    }
    if ((lbn >= 381) && (lbn <=388)){      good = 1; return good;    }
  }
      
  if(rn==191428){
    if ((lbn >= 129) && (lbn <=129)){      good = 1; return good;    }
    if ((lbn >= 131) && (lbn <=149)){      good = 1; return good;    }
    if ((lbn >= 151) && (lbn <=254)){      good = 1; return good;    }
  }
      
  if(rn==191426){
    if ((lbn >= 191) && (lbn <=307)){      good = 1; return good;    }
    if ((lbn >= 309) && (lbn <=356)){      good = 1; return good;    }
    if ((lbn >= 364) && (lbn <=400)){      good = 1; return good;    }
    if ((lbn >= 402) && (lbn <=448)){      good = 1; return good;    }
    if ((lbn >= 450) && (lbn <=501)){      good = 1; return good;    }
    if ((lbn >= 503) && (lbn <=526)){      good = 1; return good;    }
    if ((lbn >= 528) && (lbn <=528)){      good = 1; return good;    }
    if ((lbn >= 530) && (lbn <=824)){      good = 1; return good;    }
    if ((lbn >= 826) && (lbn <=864)){      good = 1; return good;    }
    if ((lbn >= 866) && (lbn <=1052)){      good = 1; return good;    }
  }
      
  if(rn==191425){
    if ((lbn >= 222) && (lbn <=227)){      good = 1; return good;    }
    if ((lbn >= 229) && (lbn <=238)){      good = 1; return good;    }
    if ((lbn >= 245) && (lbn <=278)){      good = 1; return good;    }
  }
      
  if(rn==191239){
    if ((lbn >= 43) && (lbn <=61)){      good = 1; return good;    }
    if ((lbn >= 63) && (lbn <=83)){      good = 1; return good;    }
    if ((lbn >= 86) && (lbn <=97)){      good = 1; return good;    }
    if ((lbn >= 99) && (lbn <=174)){      good = 1; return good;    }
  }
      
  if(rn==191235){
    if ((lbn >= 234) && (lbn <=258)){      good = 1; return good;    }
  }
      
  if(rn==191218){
    if ((lbn >= 3) && (lbn <=12)){      good = 1; return good;    }
  }
      
  if(rn==191217){
    if ((lbn >= 147) && (lbn <=151)){      good = 1; return good;    }
  }
      
  if(rn==191190){
    if ((lbn >= 221) && (lbn <=244)){      good = 1; return good;    }
    if ((lbn >= 246) && (lbn <=282)){      good = 1; return good;    }
    if ((lbn >= 285) && (lbn <=372)){      good = 1; return good;    }
    if ((lbn >= 374) && (lbn <=439)){      good = 1; return good;    }
    if ((lbn >= 442) && (lbn <=460)){      good = 1; return good;    }
    if ((lbn >= 462) && (lbn <=576)){      good = 1; return good;    }
    if ((lbn >= 578) && (lbn <=744)){      good = 1; return good;    }
    if ((lbn >= 747) && (lbn <=800)){      good = 1; return good;    }
    if ((lbn >= 802) && (lbn <=1200)){      good = 1; return good;    }
  }
      
  if(rn==191150){
    if ((lbn >= 157) && (lbn <=310)){      good = 1; return good;    }
  }
      
  if(rn==191149){
    if ((lbn >= 229) && (lbn <=232)){      good = 1; return good;    }
    if ((lbn >= 234) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 243) && (lbn <=266)){      good = 1; return good;    }
    if ((lbn >= 268) && (lbn <=273)){      good = 1; return good;    }
    if ((lbn >= 275) && (lbn <=296)){      good = 1; return good;    }
  }
      
  if(rn==191139){
    if ((lbn >= 289) && (lbn <=349)){      good = 1; return good;    }
    if ((lbn >= 351) && (lbn <=353)){      good = 1; return good;    }
    if ((lbn >= 356) && (lbn <=360)){      good = 1; return good;    }
    if ((lbn >= 362) && (lbn <=380)){      good = 1; return good;    }
    if ((lbn >= 382) && (lbn <=410)){      good = 1; return good;    }
    if ((lbn >= 465) && (lbn <=643)){      good = 1; return good;    }
    if ((lbn >= 645) && (lbn <=700)){      good = 1; return good;    }
    if ((lbn >= 702) && (lbn <=747)){      good = 1; return good;    }
    if ((lbn >= 749) && (lbn <=757)){      good = 1; return good;    }
    if ((lbn >= 759) && (lbn <=879)){      good = 1; return good;    }
    if ((lbn >= 881) && (lbn <=942)){      good = 1; return good;    }
    if ((lbn >= 944) && (lbn <=989)){      good = 1; return good;    }
    if ((lbn >= 991) && (lbn <=1010)){      good = 1; return good;    }
  }
      
  if(rn==191138){
    if ((lbn >= 6) && (lbn <=36)){      good = 1; return good;    }
    if ((lbn >= 38) && (lbn <=74)){      good = 1; return good;    }
  }
      
  if(rn==190975){
    if ((lbn >= 176) && (lbn <=176)){      good = 1; return good;    }
    if ((lbn >= 178) && (lbn <=227)){      good = 1; return good;    }
    if ((lbn >= 229) && (lbn <=231)){      good = 1; return good;    }
    if ((lbn >= 233) && (lbn <=234)){      good = 1; return good;    }
    if ((lbn >= 236) && (lbn <=237)){      good = 1; return good;    }
    if ((lbn >= 239) && (lbn <=242)){      good = 1; return good;    }
    if ((lbn >= 244) && (lbn <=252)){      good = 1; return good;    }
    if ((lbn >= 255) && (lbn <=255)){      good = 1; return good;    }
    if ((lbn >= 257) && (lbn <=263)){      good = 1; return good;    }
    if ((lbn >= 265) && (lbn <=289)){      good = 1; return good;    }
    if ((lbn >= 291) && (lbn <=376)){      good = 1; return good;    }
    if ((lbn >= 379) && (lbn <=380)){      good = 1; return good;    }
    if ((lbn >= 382) && (lbn <=382)){      good = 1; return good;    }
    if ((lbn >= 384) && (lbn <=387)){      good = 1; return good;    }
    if ((lbn >= 391) && (lbn <=391)){      good = 1; return good;    }
    if ((lbn >= 393) && (lbn <=394)){      good = 1; return good;    }
    if ((lbn >= 396) && (lbn <=435)){      good = 1; return good;    }
    if ((lbn >= 437) && (lbn <=469)){      good = 1; return good;    }
  }
      
  if(rn==190934){
    if ((lbn >= 4) && (lbn <=10)){      good = 1; return good;    }
  }
      
  if(rn==190933){
    if ((lbn >= 379) && (lbn <=390)){      good = 1; return good;    }
    if ((lbn >= 392) && (lbn <=451)){      good = 1; return good;    }
    if ((lbn >= 453) && (lbn <=453)){      good = 1; return good;    }
    if ((lbn >= 456) && (lbn <=456)){      good = 1; return good;    }
    if ((lbn >= 460) && (lbn <=469)){      good = 1; return good;    }
    if ((lbn >= 471) && (lbn <=480)){      good = 1; return good;    }
    if ((lbn >= 482) && (lbn <=607)){      good = 1; return good;    }
    if ((lbn >= 609) && (lbn <=707)){      good = 1; return good;    }
    if ((lbn >= 709) && (lbn <=1139)){      good = 1; return good;    }
  }
      
  if(rn==190878){
    if ((lbn >= 4) && (lbn <=248)){      good = 1; return good;    }
    if ((lbn >= 250) && (lbn <=252)){      good = 1; return good;    }
    if ((lbn >= 254) && (lbn <=255)){      good = 1; return good;    }
    if ((lbn >= 257) && (lbn <=354)){      good = 1; return good;    }
  }
      
  if(rn==190872){
    if ((lbn >= 5) && (lbn <=32)){      good = 1; return good;    }
    if ((lbn >= 34) && (lbn <=45)){      good = 1; return good;    }
    if ((lbn >= 47) && (lbn <=47)){      good = 1; return good;    }
    if ((lbn >= 49) && (lbn <=54)){      good = 1; return good;    }
    if ((lbn >= 56) && (lbn <=124)){      good = 1; return good;    }
    if ((lbn >= 126) && (lbn <=167)){      good = 1; return good;    }
    if ((lbn >= 169) && (lbn <=258)){      good = 1; return good;    }
    if ((lbn >= 260) && (lbn <=348)){      good = 1; return good;    }
    if ((lbn >= 350) && (lbn <=381)){      good = 1; return good;    }
  }
      
  if(rn==190689){
    if ((lbn >= 95) && (lbn <=101)){      good = 1; return good;    }
    if ((lbn >= 104) && (lbn <=108)){      good = 1; return good;    }
    if ((lbn >= 110) && (lbn <=110)){      good = 1; return good;    }
    if ((lbn >= 112) && (lbn <=129)){      good = 1; return good;    }
    if ((lbn >= 131) && (lbn <=168)){      good = 1; return good;    }
    if ((lbn >= 170) && (lbn <=178)){      good = 1; return good;    }
    if ((lbn >= 183) && (lbn <=198)){      good = 1; return good;    }
    if ((lbn >= 201) && (lbn <=212)){      good = 1; return good;    }
    if ((lbn >= 214) && (lbn <=352)){      good = 1; return good;    }
  }
      
  if(rn==190661){
    if ((lbn >= 301) && (lbn <=322)){      good = 1; return good;    }
  }
      
  if(rn==190644){
    if ((lbn >= 4) && (lbn <=4)){      good = 1; return good;    }
    if ((lbn >= 6) && (lbn <=84)){      good = 1; return good;    }
    if ((lbn >= 86) && (lbn <=113)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=137)){      good = 1; return good;    }
    if ((lbn >= 139) && (lbn <=186)){      good = 1; return good;    }
    if ((lbn >= 188) && (lbn <=282)){      good = 1; return good;    }
    if ((lbn >= 284) && (lbn <=319)){      good = 1; return good;    }
  }
      
  if(rn==190643){
    if ((lbn >= 148) && (lbn <=185)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=189)){      good = 1; return good;    }
    if ((lbn >= 191) && (lbn <=229)){      good = 1; return good;    }
  }
      
  if(rn==190618){
    if ((lbn >= 4) && (lbn <=5)){      good = 1; return good;    }
    if ((lbn >= 8) && (lbn <=178)){      good = 1; return good;    }
    if ((lbn >= 180) && (lbn <=229)){      good = 1; return good;    }
    if ((lbn >= 231) && (lbn <=436)){      good = 1; return good;    }
    if ((lbn >= 438) && (lbn <=740)){      good = 1; return good;    }
    if ((lbn >= 742) && (lbn <=751)){      good = 1; return good;    }
    if ((lbn >= 753) && (lbn <=796)){      good = 1; return good;    }
    if ((lbn >= 799) && (lbn <=817)){      good = 1; return good;    }
  }
      
  if(rn==190611){
    if ((lbn >= 294) && (lbn <=306)){      good = 1; return good;    }
    if ((lbn >= 308) && (lbn <=314)){      good = 1; return good;    }
    if ((lbn >= 317) && (lbn <=330)){      good = 1; return good;    }
    if ((lbn >= 335) && (lbn <=379)){      good = 1; return good;    }
    if ((lbn >= 381) && (lbn <=474)){      good = 1; return good;    }
  }
      
  if(rn==190608){
    if ((lbn >= 541) && (lbn <=558)){      good = 1; return good;    }
    if ((lbn >= 560) && (lbn <=565)){      good = 1; return good;    }
    if ((lbn >= 568) && (lbn <=569)){      good = 1; return good;    }
  }
      
  if(rn==190343){
    if ((lbn >= 18) && (lbn <=144)){      good = 1; return good;    }
    if ((lbn >= 146) && (lbn <=227)){      good = 1; return good;    }
    if ((lbn >= 230) && (lbn <=263)){      good = 1; return good;    }
    if ((lbn >= 265) && (lbn <=368)){      good = 1; return good;    }
    if ((lbn >= 370) && (lbn <=411)){      good = 1; return good;    }
    if ((lbn >= 413) && (lbn <=433)){      good = 1; return good;    }
    if ((lbn >= 435) && (lbn <=497)){      good = 1; return good;    }
    if ((lbn >= 499) && (lbn <=543)){      good = 1; return good;    }
  }
      
  if(rn==190300){
    if ((lbn >= 38) && (lbn <=240)){      good = 1; return good;    }
    if ((lbn >= 242) && (lbn <=450)){      good = 1; return good;    }
    if ((lbn >= 452) && (lbn <=664)){      good = 1; return good;    }
  }
      
  if(rn==190297){
    if ((lbn >= 152) && (lbn <=155)){      good = 1; return good;    }
  }
      
  if(rn==190256){
    if ((lbn >= 272) && (lbn <=280)){      good = 1; return good;    }
    if ((lbn >= 282) && (lbn <=285)){      good = 1; return good;    }
    if ((lbn >= 287) && (lbn <=324)){      good = 1; return good;    }
    if ((lbn >= 326) && (lbn <=327)){      good = 1; return good;    }
    if ((lbn >= 329) && (lbn <=439)){      good = 1; return good;    }
    if ((lbn >= 441) && (lbn <=468)){      good = 1; return good;    }
    if ((lbn >= 470) && (lbn <=596)){      good = 1; return good;    }
    if ((lbn >= 612) && (lbn <=831)){      good = 1; return good;    }
    if ((lbn >= 834) && (lbn <=1132)){      good = 1; return good;    }
  }
      
  if(rn==190236){
    if ((lbn >= 398) && (lbn <=409)){      good = 1; return good;    }
    if ((lbn >= 411) && (lbn <=411)){      good = 1; return good;    }
    if ((lbn >= 413) && (lbn <=455)){      good = 1; return good;    }
    if ((lbn >= 457) && (lbn <=458)){      good = 1; return good;    }
    if ((lbn >= 460) && (lbn <=584)){      good = 1; return good;    }
    if ((lbn >= 586) && (lbn <=630)){      good = 1; return good;    }
    if ((lbn >= 632) && (lbn <=691)){      good = 1; return good;    }
    if ((lbn >= 693) && (lbn <=940)){      good = 1; return good;    }
    if ((lbn >= 942) && (lbn <=1003)){      good = 1; return good;    }
    if ((lbn >= 1005) && (lbn <=1285)){      good = 1; return good;    }
  }
      
  if(rn==190120){
    if ((lbn >= 3) && (lbn <=23)){      good = 1; return good;    }
    if ((lbn >= 25) && (lbn <=88)){      good = 1; return good;    }
    if ((lbn >= 90) && (lbn <=173)){      good = 1; return good;    }
  }
      
  if(rn==190119){
    if ((lbn >= 48) && (lbn <=60)){      good = 1; return good;    }
  }
      
  if(rn==190116){
    if ((lbn >= 87) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 248) && (lbn <=311)){      good = 1; return good;    }
    if ((lbn >= 313) && (lbn <=403)){      good = 1; return good;    }
  }
      
  if(rn==190046){
    if ((lbn >= 93) && (lbn <=127)){      good = 1; return good;    }
    if ((lbn >= 129) && (lbn <=322)){      good = 1; return good;    }
  }
      
  if(rn==189965){
    if ((lbn >= 35) && (lbn <=120)){      good = 1; return good;    }
    if ((lbn >= 122) && (lbn <=279)){      good = 1; return good;    }
  }
      
  if(rn==189963){
    if ((lbn >= 557) && (lbn <=579)){      good = 1; return good;    }
    if ((lbn >= 581) && (lbn <=603)){      good = 1; return good;    }
    if ((lbn >= 605) && (lbn <=608)){      good = 1; return good;    }
  }
      
  if(rn==189875){
    if ((lbn >= 182) && (lbn <=194)){      good = 1; return good;    }
    if ((lbn >= 196) && (lbn <=229)){      good = 1; return good;    }
    if ((lbn >= 231) && (lbn <=290)){      good = 1; return good;    }
  }
      
  if(rn==189845){
    if ((lbn >= 4) && (lbn <=36)){      good = 1; return good;    }
    if ((lbn >= 38) && (lbn <=82)){      good = 1; return good;    }
    if ((lbn >= 84) && (lbn <=132)){      good = 1; return good;    }
    if ((lbn >= 134) && (lbn <=135)){      good = 1; return good;    }
    if ((lbn >= 137) && (lbn <=281)){      good = 1; return good;    }
  }
      
  if(rn==189836){
    if ((lbn >= 116) && (lbn <=217)){      good = 1; return good;    }
  }
      
  if(rn==189822){
    if ((lbn >= 121) && (lbn <=279)){      good = 1; return good;    }
    if ((lbn >= 281) && (lbn <=380)){      good = 1; return good;    }
    if ((lbn >= 386) && (lbn <=408)){      good = 1; return good;    }
    if ((lbn >= 410) && (lbn <=655)){      good = 1; return good;    }
    if ((lbn >= 657) && (lbn <=856)){      good = 1; return good;    }
    if ((lbn >= 858) && (lbn <=861)){      good = 1; return good;    }
  }
      
  if(rn==189813){
    if ((lbn >= 140) && (lbn <=170)){      good = 1; return good;    }
  }
      
  if(rn==189781){
    if ((lbn >= 20) && (lbn <=113)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=121)){      good = 1; return good;    }
    if ((lbn >= 123) && (lbn <=130)){      good = 1; return good;    }
    if ((lbn >= 132) && (lbn <=153)){      good = 1; return good;    }
  }
      
  if(rn==189751){
    if ((lbn >= 116) && (lbn <=191)){      good = 1; return good;    }
    if ((lbn >= 193) && (lbn <=269)){      good = 1; return good;    }
    if ((lbn >= 271) && (lbn <=276)){      good = 1; return good;    }
    if ((lbn >= 278) && (lbn <=356)){      good = 1; return good;    }
    if ((lbn >= 358) && (lbn <=362)){      good = 1; return good;    }
  }
      
  if(rn==189719){
    if ((lbn >= 207) && (lbn <=219)){      good = 1; return good;    }
    if ((lbn >= 221) && (lbn <=224)){      good = 1; return good;    }
    if ((lbn >= 226) && (lbn <=234)){      good = 1; return good;    }
    if ((lbn >= 236) && (lbn <=243)){      good = 1; return good;    }
    if ((lbn >= 251) && (lbn <=292)){      good = 1; return good;    }
    if ((lbn >= 294) && (lbn <=330)){      good = 1; return good;    }
    if ((lbn >= 332) && (lbn <=382)){      good = 1; return good;    }
    if ((lbn >= 384) && (lbn <=470)){      good = 1; return good;    }
  }
      
  if(rn==189693){
    if ((lbn >= 220) && (lbn <=346)){      good = 1; return good;    }
  }
      
  if(rn==189660){
    if ((lbn >= 4) && (lbn <=24)){      good = 1; return good;    }
    if ((lbn >= 26) && (lbn <=103)){      good = 1; return good;    }
  }
      
  if(rn==189639){
    if ((lbn >= 60) && (lbn <=209)){      good = 1; return good;    }
  }
      
  if(rn==189610){
    if ((lbn >= 307) && (lbn <=328)){      good = 1; return good;    }
    if ((lbn >= 330) && (lbn <=364)){      good = 1; return good;    }
    if ((lbn >= 373) && (lbn <=440)){      good = 1; return good;    }
    if ((lbn >= 442) && (lbn <=462)){      good = 1; return good;    }
    if ((lbn >= 464) && (lbn <=538)){      good = 1; return good;    }
    if ((lbn >= 541) && (lbn <=581)){      good = 1; return good;    }
    if ((lbn >= 583) && (lbn <=589)){      good = 1; return good;    }
    if ((lbn >= 591) && (lbn <=648)){      good = 1; return good;    }
  }
      
  if(rn==189602){
    if ((lbn >= 29) && (lbn <=70)){      good = 1; return good;    }
  }
      
  if(rn==189598){
    if ((lbn >= 76) && (lbn <=107)){      good = 1; return good;    }
    if ((lbn >= 109) && (lbn <=169)){      good = 1; return good;    }
    if ((lbn >= 171) && (lbn <=286)){      good = 1; return good;    }
    if ((lbn >= 288) && (lbn <=358)){      good = 1; return good;    }
    if ((lbn >= 360) && (lbn <=513)){      good = 1; return good;    }
  }
      
  if(rn==189561){
    if ((lbn >= 41) && (lbn <=73)){      good = 1; return good;    }
    if ((lbn >= 75) && (lbn <=81)){      good = 1; return good;    }
    if ((lbn >= 83) && (lbn <=144)){      good = 1; return good;    }
    if ((lbn >= 146) && (lbn <=161)){      good = 1; return good;    }
    if ((lbn >= 164) && (lbn <=217)){      good = 1; return good;    }
    if ((lbn >= 219) && (lbn <=473)){      good = 1; return good;    }
    if ((lbn >= 475) && (lbn <=508)){      good = 1; return good;    }
    if ((lbn >= 510) && (lbn <=567)){      good = 1; return good;    }
    if ((lbn >= 569) && (lbn <=609)){      good = 1; return good;    }
    if ((lbn >= 611) && (lbn <=666)){      good = 1; return good;    }
  }
      
  if(rn==189536){
    if ((lbn >= 230) && (lbn <=260)){      good = 1; return good;    }
    if ((lbn >= 262) && (lbn <=265)){      good = 1; return good;    }
  }
      
  if(rn==189530){
    if ((lbn >= 132) && (lbn <=148)){      good = 1; return good;    }
  }
      
  if(rn==189483){
    if ((lbn >= 4) && (lbn <=19)){      good = 1; return good;    }
    if ((lbn >= 21) && (lbn <=54)){      good = 1; return good;    }
    if ((lbn >= 56) && (lbn <=199)){      good = 1; return good;    }
    if ((lbn >= 201) && (lbn <=479)){      good = 1; return good;    }
    if ((lbn >= 481) && (lbn <=498)){      good = 1; return good;    }
  }
      
  if(rn==189481){
    if ((lbn >= 787) && (lbn <=830)){      good = 1; return good;    }
  }
      
  if(rn==189425){
    if ((lbn >= 3) && (lbn <=10)){      good = 1; return good;    }
    if ((lbn >= 22) && (lbn <=78)){      good = 1; return good;    }
    if ((lbn >= 80) && (lbn <=115)){      good = 1; return good;    }
    if ((lbn >= 117) && (lbn <=168)){      good = 1; return good;    }
    if ((lbn >= 170) && (lbn <=268)){      good = 1; return good;    }
  }
      
  if(rn==189421){
    if ((lbn >= 235) && (lbn <=300)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=325)){      good = 1; return good;    }
    if ((lbn >= 327) && (lbn <=406)){      good = 1; return good;    }
  }
      
  if(rn==189372){
    if ((lbn >= 5) && (lbn <=29)){      good = 1; return good;    }
    if ((lbn >= 46) && (lbn <=62)){      good = 1; return good;    }
    if ((lbn >= 79) && (lbn <=95)){      good = 1; return good;    }
    if ((lbn >= 102) && (lbn <=175)){      good = 1; return good;    }
    if ((lbn >= 177) && (lbn <=215)){      good = 1; return good;    }
  }
      
  if(rn==189288){
    if ((lbn >= 11) && (lbn <=137)){      good = 1; return good;    }
    if ((lbn >= 139) && (lbn <=199)){      good = 1; return good;    }
  }
      
  if(rn==189280){
    if ((lbn >= 100) && (lbn <=113)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=129)){      good = 1; return good;    }
    if ((lbn >= 131) && (lbn <=157)){      good = 1; return good;    }
    if ((lbn >= 159) && (lbn <=185)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=204)){      good = 1; return good;    }
    if ((lbn >= 206) && (lbn <=217)){      good = 1; return good;    }
    if ((lbn >= 247) && (lbn <=254)){      good = 1; return good;    }
    if ((lbn >= 256) && (lbn <=370)){      good = 1; return good;    }
    if ((lbn >= 372) && (lbn <=855)){      good = 1; return good;    }
  }
      
  if(rn==189242){
    if ((lbn >= 35) && (lbn <=43)){      good = 1; return good;    }
    if ((lbn >= 45) && (lbn <=50)){      good = 1; return good;    }
    if ((lbn >= 52) && (lbn <=64)){      good = 1; return good;    }
    if ((lbn >= 67) && (lbn <=99)){      good = 1; return good;    }
    if ((lbn >= 101) && (lbn <=143)){      good = 1; return good;    }
    if ((lbn >= 146) && (lbn <=174)){      good = 1; return good;    }
    if ((lbn >= 176) && (lbn <=176)){      good = 1; return good;    }
    if ((lbn >= 178) && (lbn <=234)){      good = 1; return good;    }
  }
      
  if(rn==189207){
    if ((lbn >= 4) && (lbn <=41)){      good = 1; return good;    }
    if ((lbn >= 44) && (lbn <=65)){      good = 1; return good;    }
    if ((lbn >= 67) && (lbn <=242)){      good = 1; return good;    }
    if ((lbn >= 244) && (lbn <=327)){      good = 1; return good;    }
    if ((lbn >= 330) && (lbn <=366)){      good = 1; return good;    }
    if ((lbn >= 369) && (lbn <=441)){      good = 1; return good;    }
  }
      
  if(rn==189184){
    if ((lbn >= 101) && (lbn <=130)){      good = 1; return good;    }
    if ((lbn >= 133) && (lbn <=170)){      good = 1; return good;    }
    if ((lbn >= 172) && (lbn <=195)){      good = 1; return good;    }
    if ((lbn >= 197) && (lbn <=303)){      good = 1; return good;    }
  }
      
  if(rn==189090){
    if ((lbn >= 63) && (lbn <=64)){      good = 1; return good;    }
    if ((lbn >= 67) && (lbn <=96)){      good = 1; return good;    }
  }
      
  if(rn==189079){
    if ((lbn >= 187) && (lbn <=225)){      good = 1; return good;    }
  }
      
  if(rn==189049){
    if ((lbn >= 236) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 371) && (lbn <=500)){      good = 1; return good;    }
    if ((lbn >= 502) && (lbn <=506)){      good = 1; return good;    }
    if ((lbn >= 508) && (lbn <=613)){      good = 1; return good;    }
  }
      
  if(rn==189028){
    if ((lbn >= 5) && (lbn <=31)){      good = 1; return good;    }
  }
      
  if(rn==189027){
    if ((lbn >= 12) && (lbn <=22)){      good = 1; return good;    }
  }
      
  if(rn==188951){
    if ((lbn >= 190) && (lbn <=307)){      good = 1; return good;    }
    if ((lbn >= 309) && (lbn <=480)){      good = 1; return good;    }
    if ((lbn >= 482) && (lbn <=491)){      good = 1; return good;    }
  }
      
  if(rn==188921){
    if ((lbn >= 134) && (lbn <=142)){      good = 1; return good;    }
    if ((lbn >= 193) && (lbn <=214)){      good = 1; return good;    }
    if ((lbn >= 246) && (lbn <=327)){      good = 1; return good;    }
    if ((lbn >= 330) && (lbn <=481)){      good = 1; return good;    }
  }
      
  if(rn==187815){
    if ((lbn >= 5) && (lbn <=36)){      good = 1; return good;    }
    if ((lbn >= 38) && (lbn <=138)){      good = 1; return good;    }
    if ((lbn >= 140) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 248) && (lbn <=314)){      good = 1; return good;    }
    if ((lbn >= 327) && (lbn <=331)){      good = 1; return good;    }
    if ((lbn >= 333) && (lbn <=456)){      good = 1; return good;    }
  }
      
  if(rn==187812){
    if ((lbn >= 4) && (lbn <=21)){      good = 1; return good;    }
    if ((lbn >= 23) && (lbn <=32)){      good = 1; return good;    }
    if ((lbn >= 34) && (lbn <=214)){      good = 1; return good;    }
  }
      
  if(rn==187811){
    if ((lbn >= 205) && (lbn <=209)){      good = 1; return good;    }
    if ((lbn >= 211) && (lbn <=358)){      good = 1; return good;    }
    if ((lbn >= 361) && (lbn <=407)){      good = 1; return good;    }
    if ((lbn >= 409) && (lbn <=478)){      good = 1; return good;    }
    if ((lbn >= 480) && (lbn <=523)){      good = 1; return good;    }
  }
      
  if(rn==187763){
    if ((lbn >= 131) && (lbn <=152)){      good = 1; return good;    }
    if ((lbn >= 154) && (lbn <=156)){      good = 1; return good;    }
    if ((lbn >= 158) && (lbn <=172)){      good = 1; return good;    }
    if ((lbn >= 174) && (lbn <=188)){      good = 1; return good;    }
    if ((lbn >= 190) && (lbn <=202)){      good = 1; return good;    }
    if ((lbn >= 204) && (lbn <=229)){      good = 1; return good;    }
    if ((lbn >= 231) && (lbn <=433)){      good = 1; return good;    }
    if ((lbn >= 457) && (lbn <=471)){      good = 1; return good;    }
    if ((lbn >= 474) && (lbn <=595)){      good = 1; return good;    }
  }
      
  if(rn==187552){
    if ((lbn >= 3) && (lbn <=17)){      good = 1; return good;    }
    if ((lbn >= 19) && (lbn <=30)){      good = 1; return good;    }
    if ((lbn >= 32) && (lbn <=106)){      good = 1; return good;    }
    if ((lbn >= 108) && (lbn <=119)){      good = 1; return good;    }
    if ((lbn >= 121) && (lbn <=305)){      good = 1; return good;    }
  }
      
  if(rn==187543){
    if ((lbn >= 7) && (lbn <=28)){      good = 1; return good;    }
  }
      
  if(rn==187501){
    if ((lbn >= 5) && (lbn <=24)){      good = 1; return good;    }
    if ((lbn >= 27) && (lbn <=79)){      good = 1; return good;    }
  }
      
  if(rn==187457){
    if ((lbn >= 142) && (lbn <=153)){      good = 1; return good;    }
    if ((lbn >= 155) && (lbn <=203)){      good = 1; return good;    }
    if ((lbn >= 206) && (lbn <=225)){      good = 1; return good;    }
    if ((lbn >= 228) && (lbn <=281)){      good = 1; return good;    }
    if ((lbn >= 283) && (lbn <=298)){      good = 1; return good;    }
    if ((lbn >= 300) && (lbn <=322)){      good = 1; return good;    }
    if ((lbn >= 324) && (lbn <=338)){      good = 1; return good;    }
    if ((lbn >= 340) && (lbn <=385)){      good = 1; return good;    }
  }
      
  if(rn==187453){
    if ((lbn >= 481) && (lbn <=620)){      good = 1; return good;    }
    if ((lbn >= 622) && (lbn <=660)){      good = 1; return good;    }
  }
      
  if(rn==187219){
    if ((lbn >= 111) && (lbn <=199)){      good = 1; return good;    }
    if ((lbn >= 201) && (lbn <=431)){      good = 1; return good;    }
    if ((lbn >= 433) && (lbn <=458)){      good = 1; return good;    }
    if ((lbn >= 460) && (lbn <=595)){      good = 1; return good;    }
    if ((lbn >= 598) && (lbn <=612)){      good = 1; return good;    }
  }
      
  if(rn==187196){
    if ((lbn >= 3) && (lbn <=68)){      good = 1; return good;    }
    if ((lbn >= 91) && (lbn <=204)){      good = 1; return good;    }
    if ((lbn >= 208) && (lbn <=257)){      good = 1; return good;    }
  }
      
  if(rn==187014){
    if ((lbn >= 491) && (lbn <=512)){      good = 1; return good;    }
    if ((lbn >= 514) && (lbn <=532)){      good = 1; return good;    }
    if ((lbn >= 534) && (lbn <=630)){      good = 1; return good;    }
    if ((lbn >= 633) && (lbn <=730)){      good = 1; return good;    }
    if ((lbn >= 732) && (lbn <=831)){      good = 1; return good;    }
    if ((lbn >= 833) && (lbn <=1001)){      good = 1; return good;    }
  }
      
  if(rn==186965){
    if ((lbn >= 161) && (lbn <=331)){      good = 1; return good;    }
    if ((lbn >= 333) && (lbn <=388)){      good = 1; return good;    }
    if ((lbn >= 390) && (lbn <=518)){      good = 1; return good;    }
    if ((lbn >= 520) && (lbn <=920)){      good = 1; return good;    }
    if ((lbn >= 924) && (lbn <=932)){      good = 1; return good;    }
    if ((lbn >= 934) && (lbn <=1273)){      good = 1; return good;    }
  }
      
  if(rn==186934){
    if ((lbn >= 368) && (lbn <=605)){      good = 1; return good;    }
    if ((lbn >= 607) && (lbn <=645)){      good = 1; return good;    }
    if ((lbn >= 647) && (lbn <=717)){      good = 1; return good;    }
  }
      
  if(rn==186933){
    if ((lbn >= 323) && (lbn <=346)){      good = 1; return good;    }
  }
      
  if(rn==186923){
    if ((lbn >= 74) && (lbn <=170)){      good = 1; return good;    }
    if ((lbn >= 172) && (lbn <=211)){      good = 1; return good;    }
    if ((lbn >= 213) && (lbn <=221)){      good = 1; return good;    }
    if ((lbn >= 223) && (lbn <=308)){      good = 1; return good;    }
    if ((lbn >= 310) && (lbn <=333)){      good = 1; return good;    }
    if ((lbn >= 335) && (lbn <=376)){      good = 1; return good;    }
    if ((lbn >= 378) && (lbn <=485)){      good = 1; return good;    }
    if ((lbn >= 487) && (lbn <=559)){      good = 1; return good;    }
    if ((lbn >= 561) && (lbn <=599)){      good = 1; return good;    }
    if ((lbn >= 601) && (lbn <=651)){      good = 1; return good;    }
    if ((lbn >= 653) && (lbn <=688)){      good = 1; return good;    }
    if ((lbn >= 690) && (lbn <=787)){      good = 1; return good;    }
    if ((lbn >= 789) && (lbn <=814)){      good = 1; return good;    }
    if ((lbn >= 816) && (lbn <=867)){      good = 1; return good;    }
    if ((lbn >= 869) && (lbn <=923)){      good = 1; return good;    }
    if ((lbn >= 925) && (lbn <=1061)){      good = 1; return good;    }
    if ((lbn >= 1063) && (lbn <=1075)){      good = 1; return good;    }
    if ((lbn >= 1077) && (lbn <=1088)){      good = 1; return good;    }
  }
      
  if(rn==186878){
    if ((lbn >= 9) && (lbn <=119)){      good = 1; return good;    }
  }
      
  if(rn==186877){
    if ((lbn >= 139) && (lbn <=141)){      good = 1; return good;    }
    if ((lbn >= 276) && (lbn <=279)){      good = 1; return good;    }
    if ((lbn >= 281) && (lbn <=391)){      good = 1; return good;    }
    if ((lbn >= 393) && (lbn <=657)){      good = 1; return good;    }
    if ((lbn >= 659) && (lbn <=729)){      good = 1; return good;    }
    if ((lbn >= 731) && (lbn <=909)){      good = 1; return good;    }
    if ((lbn >= 911) && (lbn <=1310)){      good = 1; return good;    }
    if ((lbn >= 1312) && (lbn <=1313)){      good = 1; return good;    }
    if ((lbn >= 1315) && (lbn <=1338)){      good = 1; return good;    }
  }
      
  if(rn==186873){
    if ((lbn >= 467) && (lbn <=482)){      good = 1; return good;    }
  }
      
  if(rn==186755){
    if ((lbn >= 11) && (lbn <=33)){      good = 1; return good;    }
    if ((lbn >= 35) && (lbn <=35)){      good = 1; return good;    }
    if ((lbn >= 37) && (lbn <=42)){      good = 1; return good;    }
    if ((lbn >= 44) && (lbn <=49)){      good = 1; return good;    }
  }
      
  if(rn==186753){
    if ((lbn >= 90) && (lbn <=104)){      good = 1; return good;    }
    if ((lbn >= 106) && (lbn <=196)){      good = 1; return good;    }
  }
      
  if(rn==186729){
    if ((lbn >= 52) && (lbn <=109)){      good = 1; return good;    }
    if ((lbn >= 111) && (lbn <=400)){      good = 1; return good;    }
    if ((lbn >= 402) && (lbn <=449)){      good = 1; return good;    }
    if ((lbn >= 451) && (lbn <=490)){      good = 1; return good;    }
    if ((lbn >= 492) && (lbn <=531)){      good = 1; return good;    }
    if ((lbn >= 533) && (lbn <=538)){      good = 1; return good;    }
    if ((lbn >= 540) && (lbn <=549)){      good = 1; return good;    }
    if ((lbn >= 551) && (lbn <=551)){      good = 1; return good;    }
    if ((lbn >= 553) && (lbn <=554)){      good = 1; return good;    }
    if ((lbn >= 556) && (lbn <=557)){      good = 1; return good;    }
    if ((lbn >= 565) && (lbn <=568)){      good = 1; return good;    }
    if ((lbn >= 571) && (lbn <=622)){      good = 1; return good;    }
    if ((lbn >= 624) && (lbn <=749)){      good = 1; return good;    }
    if ((lbn >= 751) && (lbn <=1054)){      good = 1; return good;    }
    if ((lbn >= 1056) && (lbn <=1146)){      good = 1; return good;    }
    if ((lbn >= 1153) && (lbn <=1160)){      good = 1; return good;    }
    if ((lbn >= 1162) && (lbn <=1184)){      good = 1; return good;    }
    if ((lbn >= 1186) && (lbn <=1214)){      good = 1; return good;    }
    if ((lbn >= 1216) && (lbn <=1333)){      good = 1; return good;    }
  }
      
  if(rn==186721){
    if ((lbn >= 97) && (lbn <=374)){      good = 1; return good;    }
    if ((lbn >= 376) && (lbn <=405)){      good = 1; return good;    }
    if ((lbn >= 407) && (lbn <=576)){      good = 1; return good;    }
    if ((lbn >= 578) && (lbn <=627)){      good = 1; return good;    }
    if ((lbn >= 629) && (lbn <=787)){      good = 1; return good;    }
  }
      
  if(rn==186673){
    if ((lbn >= 198) && (lbn <=207)){      good = 1; return good;    }
    if ((lbn >= 209) && (lbn <=219)){      good = 1; return good;    }
    if ((lbn >= 221) && (lbn <=416)){      good = 1; return good;    }
    if ((lbn >= 421) && (lbn <=472)){      good = 1; return good;    }
    if ((lbn >= 475) && (lbn <=477)){      good = 1; return good;    }
    if ((lbn >= 479) && (lbn <=606)){      good = 1; return good;    }
  }
      
  if(rn==186669){
    if ((lbn >= 370) && (lbn <=416)){      good = 1; return good;    }
    if ((lbn >= 418) && (lbn <=491)){      good = 1; return good;    }
    if ((lbn >= 493) && (lbn <=539)){      good = 1; return good;    }
  }
      
  if(rn==186533){
    if ((lbn >= 185) && (lbn <=226)){      good = 1; return good;    }
    if ((lbn >= 576) && (lbn <=577)){      good = 1; return good;    }
    if ((lbn >= 579) && (lbn <=602)){      good = 1; return good;    }
    if ((lbn >= 605) && (lbn <=610)){      good = 1; return good;    }
    if ((lbn >= 612) && (lbn <=633)){      good = 1; return good;    }
  }
      
  if(rn==186532){
    if ((lbn >= 200) && (lbn <=217)){      good = 1; return good;    }
    if ((lbn >= 219) && (lbn <=226)){      good = 1; return good;    }
    if ((lbn >= 228) && (lbn <=250)){      good = 1; return good;    }
  }
      
  if(rn==186516){
    if ((lbn >= 51) && (lbn <=66)){      good = 1; return good;    }
    if ((lbn >= 68) && (lbn <=68)){      good = 1; return good;    }
    if ((lbn >= 70) && (lbn <=79)){      good = 1; return good;    }
    if ((lbn >= 81) && (lbn <=97)){      good = 1; return good;    }
    if ((lbn >= 99) && (lbn <=104)){      good = 1; return good;    }
  }
      
  if(rn==186493){
    if ((lbn >= 220) && (lbn <=220)){      good = 1; return good;    }
    if ((lbn >= 223) && (lbn <=304)){      good = 1; return good;    }
    if ((lbn >= 306) && (lbn <=407)){      good = 1; return good;    }
    if ((lbn >= 409) && (lbn <=416)){      good = 1; return good;    }
    if ((lbn >= 418) && (lbn <=432)){      good = 1; return good;    }
    if ((lbn >= 434) && (lbn <=488)){      good = 1; return good;    }
    if ((lbn >= 490) && (lbn <=516)){      good = 1; return good;    }
  }
      
  if(rn==186456){
    if ((lbn >= 234) && (lbn <=251)){      good = 1; return good;    }
    if ((lbn >= 253) && (lbn <=378)){      good = 1; return good;    }
    if ((lbn >= 380) && (lbn <=446)){      good = 1; return good;    }
    if ((lbn >= 448) && (lbn <=492)){      good = 1; return good;    }
    if ((lbn >= 494) && (lbn <=531)){      good = 1; return good;    }
    if ((lbn >= 533) && (lbn <=604)){      good = 1; return good;    }
    if ((lbn >= 606) && (lbn <=689)){      good = 1; return good;    }
  }
      
  if(rn==186399){
    if ((lbn >= 281) && (lbn <=322)){      good = 1; return good;    }
    if ((lbn >= 324) && (lbn <=373)){      good = 1; return good;    }
    if ((lbn >= 383) && (lbn <=425)){      good = 1; return good;    }
  }
      
  if(rn==186361){
    if ((lbn >= 72) && (lbn <=117)){      good = 1; return good;    }
    if ((lbn >= 119) && (lbn <=224)){      good = 1; return good;    }
  }
      
  if(rn==186275){
    if ((lbn >= 102) && (lbn <=319)){      good = 1; return good;    }
    if ((lbn >= 321) && (lbn <=398)){      good = 1; return good;    }
    if ((lbn >= 400) && (lbn <=438)){      good = 1; return good;    }
    if ((lbn >= 440) && (lbn <=450)){      good = 1; return good;    }
    if ((lbn >= 452) && (lbn <=452)){      good = 1; return good;    }
    if ((lbn >= 454) && (lbn <=463)){      good = 1; return good;    }
  }
      
  if(rn==186217){
    if ((lbn >= 51) && (lbn <=94)){      good = 1; return good;    }
    if ((lbn >= 96) && (lbn <=126)){      good = 1; return good;    }
    if ((lbn >= 128) && (lbn <=151)){      good = 1; return good;    }
    if ((lbn >= 153) && (lbn <=159)){      good = 1; return good;    }
    if ((lbn >= 161) && (lbn <=164)){      good = 1; return good;    }
  }
      
  if(rn==186216){
    if ((lbn >= 4) && (lbn <=124)){      good = 1; return good;    }
    if ((lbn >= 126) && (lbn <=208)){      good = 1; return good;    }
    if ((lbn >= 210) && (lbn <=259)){      good = 1; return good;    }
  }
      
  if(rn==186182){
    if ((lbn >= 36) && (lbn <=76)){      good = 1; return good;    }
  }
      
  if(rn==186180){
    if ((lbn >= 114) && (lbn <=176)){      good = 1; return good;    }
  }
      
  if(rn==186179){
    if ((lbn >= 382) && (lbn <=391)){      good = 1; return good;    }
    if ((lbn >= 393) && (lbn <=461)){      good = 1; return good;    }
    if ((lbn >= 463) && (lbn <=479)){      good = 1; return good;    }
  }
      
  if(rn==186178){
    if ((lbn >= 125) && (lbn <=147)){      good = 1; return good;    }
    if ((lbn >= 149) && (lbn <=161)){      good = 1; return good;    }
  }
      
  if(rn==186169){
    if ((lbn >= 139) && (lbn <=174)){      good = 1; return good;    }
    if ((lbn >= 176) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 243) && (lbn <=380)){      good = 1; return good;    }
    if ((lbn >= 382) && (lbn <=558)){      good = 1; return good;    }
    if ((lbn >= 560) && (lbn <=573)){      good = 1; return good;    }
    if ((lbn >= 575) && (lbn <=583)){      good = 1; return good;    }
    if ((lbn >= 588) && (lbn <=608)){      good = 1; return good;    }
    if ((lbn >= 610) && (lbn <=761)){      good = 1; return good;    }
    if ((lbn >= 764) && (lbn <=814)){      good = 1; return good;    }
    if ((lbn >= 816) && (lbn <=819)){      good = 1; return good;    }
  }
      
  if(rn==186156){
    if ((lbn >= 43) && (lbn <=68)){      good = 1; return good;    }
    if ((lbn >= 70) && (lbn <=95)){      good = 1; return good;    }
    if ((lbn >= 97) && (lbn <=99)){      good = 1; return good;    }
    if ((lbn >= 101) && (lbn <=291)){      good = 1; return good;    }
    if ((lbn >= 293) && (lbn <=328)){      good = 1; return good;    }
    if ((lbn >= 330) && (lbn <=465)){      good = 1; return good;    }
    if ((lbn >= 467) && (lbn <=471)){      good = 1; return good;    }
    if ((lbn >= 473) && (lbn <=489)){      good = 1; return good;    }
    if ((lbn >= 491) && (lbn <=545)){      good = 1; return good;    }
  }
      
  if(rn==186049){
    if ((lbn >= 139) && (lbn <=159)){      good = 1; return good;    }
    if ((lbn >= 161) && (lbn <=303)){      good = 1; return good;    }
    if ((lbn >= 305) && (lbn <=382)){      good = 1; return good;    }
    if ((lbn >= 384) && (lbn <=428)){      good = 1; return good;    }
    if ((lbn >= 430) && (lbn <=602)){      good = 1; return good;    }
    if ((lbn >= 604) && (lbn <=610)){      good = 1; return good;    }
    if ((lbn >= 612) && (lbn <=722)){      good = 1; return good;    }
    if ((lbn >= 724) && (lbn <=748)){      good = 1; return good;    }
    if ((lbn >= 750) && (lbn <=788)){      good = 1; return good;    }
    if ((lbn >= 793) && (lbn <=872)){      good = 1; return good;    }
    if ((lbn >= 874) && (lbn <=953)){      good = 1; return good;    }
    if ((lbn >= 955) && (lbn <=958)){      good = 1; return good;    }
    if ((lbn >= 960) && (lbn <=1035)){      good = 1; return good;    }
    if ((lbn >= 1037) && (lbn <=1078)){      good = 1; return good;    }
  }
      
  if(rn==185998){
    if ((lbn >= 37) && (lbn <=57)){      good = 1; return good;    }
    if ((lbn >= 59) && (lbn <=126)){      good = 1; return good;    }
  }
      
  if(rn==185976){
    if ((lbn >= 381) && (lbn <=416)){      good = 1; return good;    }
    if ((lbn >= 418) && (lbn <=447)){      good = 1; return good;    }
  }
      
  if(rn==185856){
    if ((lbn >= 141) && (lbn <=156)){      good = 1; return good;    }
  }
      
  if(rn==185823){
    if ((lbn >= 51) && (lbn <=61)){      good = 1; return good;    }
  }
      
  if(rn==185761){
    if ((lbn >= 182) && (lbn <=207)){      good = 1; return good;    }
    if ((lbn >= 209) && (lbn <=228)){      good = 1; return good;    }
    if ((lbn >= 230) && (lbn <=398)){      good = 1; return good;    }
    if ((lbn >= 400) && (lbn <=548)){      good = 1; return good;    }
  }
      
  if(rn==185747){
    if ((lbn >= 24) && (lbn <=109)){      good = 1; return good;    }
  }
      
  if(rn==185731){
    if ((lbn >= 216) && (lbn <=217)){      good = 1; return good;    }
    if ((lbn >= 219) && (lbn <=243)){      good = 1; return good;    }
    if ((lbn >= 245) && (lbn <=290)){      good = 1; return good;    }
  }
      
  if(rn==185649){
    if ((lbn >= 52) && (lbn <=59)){      good = 1; return good;    }
    if ((lbn >= 61) && (lbn <=201)){      good = 1; return good;    }
    if ((lbn >= 203) && (lbn <=238)){      good = 1; return good;    }
    if ((lbn >= 240) && (lbn <=253)){      good = 1; return good;    }
    if ((lbn >= 255) && (lbn <=261)){      good = 1; return good;    }
    if ((lbn >= 263) && (lbn <=390)){      good = 1; return good;    }
  }
      
  if(rn==185644){
    if ((lbn >= 21) && (lbn <=24)){      good = 1; return good;    }
    if ((lbn >= 26) && (lbn <=37)){      good = 1; return good;    }
    if ((lbn >= 39) && (lbn <=72)){      good = 1; return good;    }
    if ((lbn >= 75) && (lbn <=93)){      good = 1; return good;    }
    if ((lbn >= 96) && (lbn <=113)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=123)){      good = 1; return good;    }
    if ((lbn >= 125) && (lbn <=128)){      good = 1; return good;    }
    if ((lbn >= 130) && (lbn <=196)){      good = 1; return good;    }
    if ((lbn >= 198) && (lbn <=212)){      good = 1; return good;    }
    if ((lbn >= 215) && (lbn <=250)){      good = 1; return good;    }
    if ((lbn >= 252) && (lbn <=284)){      good = 1; return good;    }
    if ((lbn >= 286) && (lbn <=302)){      good = 1; return good;    }
    if ((lbn >= 305) && (lbn <=316)){      good = 1; return good;    }
  }
      
  if(rn==185518){
    if ((lbn >= 151) && (lbn <=164)){      good = 1; return good;    }
    if ((lbn >= 166) && (lbn <=284)){      good = 1; return good;    }
    if ((lbn >= 286) && (lbn <=300)){      good = 1; return good;    }
  }
      
  if(rn==185353){
    if ((lbn >= 26) && (lbn <=117)){      good = 1; return good;    }
  }
      
  if(rn==184169){
    if ((lbn >= 86) && (lbn <=105)){      good = 1; return good;    }
    if ((lbn >= 107) && (lbn <=123)){      good = 1; return good;    }
    if ((lbn >= 125) && (lbn <=178)){      good = 1; return good;    }
    if ((lbn >= 180) && (lbn <=180)){      good = 1; return good;    }
    if ((lbn >= 211) && (lbn <=296)){      good = 1; return good;    }
    if ((lbn >= 307) && (lbn <=528)){      good = 1; return good;    }
    if ((lbn >= 531) && (lbn <=600)){      good = 1; return good;    }
    if ((lbn >= 602) && (lbn <=858)){      good = 1; return good;    }
    if ((lbn >= 860) && (lbn <=902)){      good = 1; return good;    }
  }
      
  if(rn==184130){
    if ((lbn >= 216) && (lbn <=253)){      good = 1; return good;    }
    if ((lbn >= 298) && (lbn <=354)){      good = 1; return good;    }
    if ((lbn >= 357) && (lbn <=400)){      good = 1; return good;    }
    if ((lbn >= 406) && (lbn <=451)){      good = 1; return good;    }
    if ((lbn >= 453) && (lbn <=539)){      good = 1; return good;    }
    if ((lbn >= 581) && (lbn <=823)){      good = 1; return good;    }
    if ((lbn >= 827) && (lbn <=901)){      good = 1; return good;    }
    if ((lbn >= 908) && (lbn <=930)){      good = 1; return good;    }
    if ((lbn >= 932) && (lbn <=1105)){      good = 1; return good;    }
    if ((lbn >= 1146) && (lbn <=1200)){      good = 1; return good;    }
    if ((lbn >= 1202) && (lbn <=1203)){      good = 1; return good;    }
    if ((lbn >= 1205) && (lbn <=1228)){      good = 1; return good;    }
    if ((lbn >= 1234) && (lbn <=1237)){      good = 1; return good;    }
    if ((lbn >= 1288) && (lbn <=1330)){      good = 1; return good;    }
    if ((lbn >= 1332) && (lbn <=1361)){      good = 1; return good;    }
    if ((lbn >= 1363) && (lbn <=1363)){      good = 1; return good;    }
    if ((lbn >= 1375) && (lbn <=1407)){      good = 1; return good;    }
  }
      
  if(rn==184088){
    if ((lbn >= 166) && (lbn <=167)){      good = 1; return good;    }
    if ((lbn >= 171) && (lbn <=189)){      good = 1; return good;    }
    if ((lbn >= 191) && (lbn <=228)){      good = 1; return good;    }
    if ((lbn >= 230) && (lbn <=274)){      good = 1; return good;    }
  }
      
  if(rn==184074){
    if ((lbn >= 95) && (lbn <=108)){      good = 1; return good;    }
  }
      
  if(rn==184072){
    if ((lbn >= 37) && (lbn <=80)){      good = 1; return good;    }
    if ((lbn >= 82) && (lbn <=110)){      good = 1; return good;    }
  }
      
  if(rn==184066){
    if ((lbn >= 91) && (lbn <=122)){      good = 1; return good;    }
  }
      
  if(rn==184022){
    if ((lbn >= 226) && (lbn <=228)){      good = 1; return good;    }
    if ((lbn >= 231) && (lbn <=256)){      good = 1; return good;    }
    if ((lbn >= 258) && (lbn <=345)){      good = 1; return good;    }
    if ((lbn >= 348) && (lbn <=350)){      good = 1; return good;    }
    if ((lbn >= 352) && (lbn <=401)){      good = 1; return good;    }
    if ((lbn >= 404) && (lbn <=442)){      good = 1; return good;    }
    if ((lbn >= 444) && (lbn <=464)){      good = 1; return good;    }
    if ((lbn >= 484) && (lbn <=515)){      good = 1; return good;    }
    if ((lbn >= 517) && (lbn <=600)){      good = 1; return good;    }
    if ((lbn >= 603) && (lbn <=687)){      good = 1; return good;    }
    if ((lbn >= 689) && (lbn <=776)){      good = 1; return good;    }
    if ((lbn >= 778) && (lbn <=820)){      good = 1; return good;    }
    if ((lbn >= 822) && (lbn <=1086)){      good = 1; return good;    }
  }
      
  if(rn==183963){
    if ((lbn >= 209) && (lbn <=211)){      good = 1; return good;    }
    if ((lbn >= 214) && (lbn <=216)){      good = 1; return good;    }
    if ((lbn >= 218) && (lbn <=228)){      good = 1; return good;    }
    if ((lbn >= 230) && (lbn <=254)){      good = 1; return good;    }
    if ((lbn >= 256) && (lbn <=299)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=340)){      good = 1; return good;    }
    if ((lbn >= 350) && (lbn <=366)){      good = 1; return good;    }
  }
      
  if(rn==183780){
    if ((lbn >= 399) && (lbn <=403)){      good = 1; return good;    }
    if ((lbn >= 405) && (lbn <=405)){      good = 1; return good;    }
    if ((lbn >= 408) && (lbn <=486)){      good = 1; return good;    }
    if ((lbn >= 488) && (lbn <=504)){      good = 1; return good;    }
    if ((lbn >= 506) && (lbn <=692)){      good = 1; return good;    }
    if ((lbn >= 694) && (lbn <=992)){      good = 1; return good;    }
    if ((lbn >= 994) && (lbn <=1379)){      good = 1; return good;    }
    if ((lbn >= 1381) && (lbn <=1424)){      good = 1; return good;    }
    if ((lbn >= 1426) && (lbn <=1482)){      good = 1; return good;    }
  }
      
  if(rn==183602){
    if ((lbn >= 19) && (lbn <=32)){      good = 1; return good;    }
  }
      
  if(rn==183581){
    if ((lbn >= 210) && (lbn <=259)){      good = 1; return good;    }
    if ((lbn >= 261) && (lbn <=672)){      good = 1; return good;    }
  }
      
  if(rn==183580){
    if ((lbn >= 302) && (lbn <=329)){      good = 1; return good;    }
    if ((lbn >= 332) && (lbn <=352)){      good = 1; return good;    }
  }
      
  if(rn==183544){
    if ((lbn >= 278) && (lbn <=501)){      good = 1; return good;    }
    if ((lbn >= 503) && (lbn <=725)){      good = 1; return good;    }
  }
      
  if(rn==183462){
    if ((lbn >= 402) && (lbn <=435)){      good = 1; return good;    }
    if ((lbn >= 437) && (lbn <=450)){      good = 1; return good;    }
    if ((lbn >= 452) && (lbn <=454)){      good = 1; return good;    }
    if ((lbn >= 456) && (lbn <=465)){      good = 1; return good;    }
    if ((lbn >= 467) && (lbn <=468)){      good = 1; return good;    }
    if ((lbn >= 470) && (lbn <=475)){      good = 1; return good;    }
    if ((lbn >= 477) && (lbn <=513)){      good = 1; return good;    }
    if ((lbn >= 515) && (lbn <=522)){      good = 1; return good;    }
    if ((lbn >= 525) && (lbn <=525)){      good = 1; return good;    }
    if ((lbn >= 527) && (lbn <=530)){      good = 1; return good;    }
    if ((lbn >= 532) && (lbn <=556)){      good = 1; return good;    }
    if ((lbn >= 558) && (lbn <=559)){      good = 1; return good;    }
    if ((lbn >= 561) && (lbn <=570)){      good = 1; return good;    }
    if ((lbn >= 573) && (lbn <=574)){      good = 1; return good;    }
    if ((lbn >= 576) && (lbn <=576)){      good = 1; return good;    }
    if ((lbn >= 578) && (lbn <=585)){      good = 1; return good;    }
    if ((lbn >= 587) && (lbn <=587)){      good = 1; return good;    }
    if ((lbn >= 589) && (lbn <=589)){      good = 1; return good;    }
    if ((lbn >= 591) && (lbn <=592)){      good = 1; return good;    }
    if ((lbn >= 594) && (lbn <=597)){      good = 1; return good;    }
    if ((lbn >= 599) && (lbn <=601)){      good = 1; return good;    }
    if ((lbn >= 603) && (lbn <=635)){      good = 1; return good;    }
    if ((lbn >= 637) && (lbn <=639)){      good = 1; return good;    }
    if ((lbn >= 641) && (lbn <=659)){      good = 1; return good;    }
    if ((lbn >= 661) && (lbn <=712)){      good = 1; return good;    }
    if ((lbn >= 714) && (lbn <=731)){      good = 1; return good;    }
    if ((lbn >= 733) && (lbn <=738)){      good = 1; return good;    }
    if ((lbn >= 740) && (lbn <=768)){      good = 1; return good;    }
    if ((lbn >= 770) && (lbn <=777)){      good = 1; return good;    }
    if ((lbn >= 780) && (lbn <=799)){      good = 1; return good;    }
    if ((lbn >= 801) && (lbn <=802)){      good = 1; return good;    }
    if ((lbn >= 804) && (lbn <=805)){      good = 1; return good;    }
    if ((lbn >= 807) && (lbn <=829)){      good = 1; return good;    }
    if ((lbn >= 831) && (lbn <=840)){      good = 1; return good;    }
    if ((lbn >= 842) && (lbn <=859)){      good = 1; return good;    }
    if ((lbn >= 861) && (lbn <=871)){      good = 1; return good;    }
    if ((lbn >= 873) && (lbn <=875)){      good = 1; return good;    }
    if ((lbn >= 877) && (lbn <=896)){      good = 1; return good;    }
    if ((lbn >= 898) && (lbn <=908)){      good = 1; return good;    }
    if ((lbn >= 910) && (lbn <=917)){      good = 1; return good;    }
    if ((lbn >= 919) && (lbn <=923)){      good = 1; return good;    }
    if ((lbn >= 925) && (lbn <=939)){      good = 1; return good;    }
    if ((lbn >= 941) && (lbn <=958)){      good = 1; return good;    }
    if ((lbn >= 960) && (lbn <=967)){      good = 1; return good;    }
    if ((lbn >= 969) && (lbn <=1000)){      good = 1; return good;    }
    if ((lbn >= 1002) && (lbn <=1015)){      good = 1; return good;    }
    if ((lbn >= 1017) && (lbn <=1024)){      good = 1; return good;    }
    if ((lbn >= 1026) && (lbn <=1044)){      good = 1; return good;    }
    if ((lbn >= 1046) && (lbn <=1046)){      good = 1; return good;    }
    if ((lbn >= 1048) && (lbn <=1053)){      good = 1; return good;    }
    if ((lbn >= 1055) && (lbn <=1060)){      good = 1; return good;    }
    if ((lbn >= 1063) && (lbn <=1071)){      good = 1; return good;    }
    if ((lbn >= 1073) && (lbn <=1076)){      good = 1; return good;    }
    if ((lbn >= 1078) && (lbn <=1085)){      good = 1; return good;    }
    if ((lbn >= 1087) && (lbn <=1088)){      good = 1; return good;    }
    if ((lbn >= 1090) && (lbn <=1096)){      good = 1; return good;    }
    if ((lbn >= 1098) && (lbn <=1103)){      good = 1; return good;    }
    if ((lbn >= 1105) && (lbn <=1108)){      good = 1; return good;    }
    if ((lbn >= 1110) && (lbn <=1110)){      good = 1; return good;    }
    if ((lbn >= 1112) && (lbn <=1114)){      good = 1; return good;    }
    if ((lbn >= 1116) && (lbn <=1116)){      good = 1; return good;    }
    if ((lbn >= 1118) && (lbn <=1118)){      good = 1; return good;    }
    if ((lbn >= 1120) && (lbn <=1126)){      good = 1; return good;    }
    if ((lbn >= 1128) && (lbn <=1131)){      good = 1; return good;    }
    if ((lbn >= 1133) && (lbn <=1134)){      good = 1; return good;    }
    if ((lbn >= 1136) && (lbn <=1142)){      good = 1; return good;    }
    if ((lbn >= 1144) && (lbn <=1156)){      good = 1; return good;    }
    if ((lbn >= 1158) && (lbn <=1176)){      good = 1; return good;    }
    if ((lbn >= 1178) && (lbn <=1196)){      good = 1; return good;    }
    if ((lbn >= 1198) && (lbn <=1206)){      good = 1; return good;    }
    if ((lbn >= 1208) && (lbn <=1216)){      good = 1; return good;    }
    if ((lbn >= 1218) && (lbn <=1218)){      good = 1; return good;    }
    if ((lbn >= 1220) && (lbn <=1250)){      good = 1; return good;    }
    if ((lbn >= 1252) && (lbn <=1253)){      good = 1; return good;    }
    if ((lbn >= 1255) && (lbn <=1256)){      good = 1; return good;    }
    if ((lbn >= 1258) && (lbn <=1278)){      good = 1; return good;    }
    if ((lbn >= 1280) && (lbn <=1281)){      good = 1; return good;    }
    if ((lbn >= 1283) && (lbn <=1288)){      good = 1; return good;    }
    if ((lbn >= 1290) && (lbn <=1309)){      good = 1; return good;    }
    if ((lbn >= 1311) && (lbn <=1316)){      good = 1; return good;    }
    if ((lbn >= 1318) && (lbn <=1330)){      good = 1; return good;    }
  }
      
  if(rn==183426){
    if ((lbn >= 67) && (lbn <=123)){      good = 1; return good;    }
    if ((lbn >= 128) && (lbn <=231)){      good = 1; return good;    }
    if ((lbn >= 234) && (lbn <=358)){      good = 1; return good;    }
    if ((lbn >= 360) && (lbn <=564)){      good = 1; return good;    }
    if ((lbn >= 566) && (lbn <=580)){      good = 1; return good;    }
  }
      
  if(rn==183412){
    if ((lbn >= 405) && (lbn <=414)){      good = 1; return good;    }
    if ((lbn >= 416) && (lbn <=421)){      good = 1; return good;    }
  }
      
  if(rn==183407){
    if ((lbn >= 147) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 264) && (lbn <=349)){      good = 1; return good;    }
    if ((lbn >= 354) && (lbn <=356)){      good = 1; return good;    }
  }
      
  if(rn==183391){
    if ((lbn >= 201) && (lbn <=203)){      good = 1; return good;    }
    if ((lbn >= 205) && (lbn <=211)){      good = 1; return good;    }
    if ((lbn >= 213) && (lbn <=225)){      good = 1; return good;    }
    if ((lbn >= 227) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 243) && (lbn <=251)){      good = 1; return good;    }
    if ((lbn >= 253) && (lbn <=256)){      good = 1; return good;    }
    if ((lbn >= 259) && (lbn <=259)){      good = 1; return good;    }
    if ((lbn >= 261) && (lbn <=262)){      good = 1; return good;    }
    if ((lbn >= 264) && (lbn <=267)){      good = 1; return good;    }
    if ((lbn >= 269) && (lbn <=270)){      good = 1; return good;    }
    if ((lbn >= 272) && (lbn <=319)){      good = 1; return good;    }
    if ((lbn >= 321) && (lbn <=323)){      good = 1; return good;    }
    if ((lbn >= 325) && (lbn <=335)){      good = 1; return good;    }
    if ((lbn >= 337) && (lbn <=344)){      good = 1; return good;    }
    if ((lbn >= 346) && (lbn <=348)){      good = 1; return good;    }
    if ((lbn >= 351) && (lbn <=356)){      good = 1; return good;    }
    if ((lbn >= 358) && (lbn <=377)){      good = 1; return good;    }
    if ((lbn >= 379) && (lbn <=390)){      good = 1; return good;    }
    if ((lbn >= 392) && (lbn <=394)){      good = 1; return good;    }
    if ((lbn >= 396) && (lbn <=402)){      good = 1; return good;    }
    if ((lbn >= 404) && (lbn <=405)){      good = 1; return good;    }
    if ((lbn >= 407) && (lbn <=413)){      good = 1; return good;    }
    if ((lbn >= 415) && (lbn <=464)){      good = 1; return good;    }
    if ((lbn >= 466) && (lbn <=471)){      good = 1; return good;    }
    if ((lbn >= 474) && (lbn <=483)){      good = 1; return good;    }
    if ((lbn >= 485) && (lbn <=491)){      good = 1; return good;    }
    if ((lbn >= 493) && (lbn <=512)){      good = 1; return good;    }
    if ((lbn >= 514) && (lbn <=526)){      good = 1; return good;    }
    if ((lbn >= 528) && (lbn <=548)){      good = 1; return good;    }
    if ((lbn >= 550) && (lbn <=551)){      good = 1; return good;    }
    if ((lbn >= 567) && (lbn <=569)){      good = 1; return good;    }
    if ((lbn >= 571) && (lbn <=611)){      good = 1; return good;    }
    if ((lbn >= 613) && (lbn <=615)){      good = 1; return good;    }
  }
      
  if(rn==183347){
    if ((lbn >= 757) && (lbn <=790)){      good = 1; return good;    }
    if ((lbn >= 792) && (lbn <=818)){      good = 1; return good;    }
    if ((lbn >= 820) && (lbn <=828)){      good = 1; return good;    }
    if ((lbn >= 830) && (lbn <=832)){      good = 1; return good;    }
    if ((lbn >= 846) && (lbn <=1030)){      good = 1; return good;    }
    if ((lbn >= 1032) && (lbn <=1097)){      good = 1; return good;    }
    if ((lbn >= 1099) && (lbn <=1230)){      good = 1; return good;    }
    if ((lbn >= 1232) && (lbn <=1330)){      good = 1; return good;    }
  }
      
  if(rn==183286){
    if ((lbn >= 15) && (lbn <=43)){      good = 1; return good;    }
    if ((lbn >= 57) && (lbn <=139)){      good = 1; return good;    }
    if ((lbn >= 181) && (lbn <=377)){      good = 1; return good;    }
    if ((lbn >= 379) && (lbn <=625)){      good = 1; return good;    }
  }
      
  if(rn==183272){
    if ((lbn >= 421) && (lbn <=479)){      good = 1; return good;    }
    if ((lbn >= 481) && (lbn <=485)){      good = 1; return good;    }
    if ((lbn >= 487) && (lbn <=650)){      good = 1; return good;    }
  }
      
  if(rn==183216){
    if ((lbn >= 59) && (lbn <=237)){      good = 1; return good;    }
    if ((lbn >= 239) && (lbn <=319)){      good = 1; return good;    }
    if ((lbn >= 321) && (lbn <=407)){      good = 1; return good;    }
    if ((lbn >= 409) && (lbn <=488)){      good = 1; return good;    }
    if ((lbn >= 490) && (lbn <=571)){      good = 1; return good;    }
    if ((lbn >= 573) && (lbn <=608)){      good = 1; return good;    }
    if ((lbn >= 610) && (lbn <=700)){      good = 1; return good;    }
  }
      
  if(rn==183130){
    if ((lbn >= 17) && (lbn <=45)){      good = 1; return good;    }
    if ((lbn >= 47) && (lbn <=213)){      good = 1; return good;    }
    if ((lbn >= 215) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 248) && (lbn <=266)){      good = 1; return good;    }
    if ((lbn >= 268) && (lbn <=499)){      good = 1; return good;    }
  }
      
  if(rn==183129){
    if ((lbn >= 3) && (lbn <=123)){      good = 1; return good;    }
    if ((lbn >= 125) && (lbn <=146)){      good = 1; return good;    }
  }
      
  if(rn==183127){
    if ((lbn >= 93) && (lbn <=196)){      good = 1; return good;    }
    if ((lbn >= 198) && (lbn <=312)){      good = 1; return good;    }
  }
      
  if(rn==183081){
    if ((lbn >= 73) && (lbn <=114)){      good = 1; return good;    }
    if ((lbn >= 116) && (lbn <=536)){      good = 1; return good;    }
    if ((lbn >= 538) && (lbn <=592)){      good = 1; return good;    }
    if ((lbn >= 594) && (lbn <=831)){      good = 1; return good;    }
    if ((lbn >= 833) && (lbn <=860)){      good = 1; return good;    }
    if ((lbn >= 862) && (lbn <=984)){      good = 1; return good;    }
  }
      
  if(rn==183079){
    if ((lbn >= 353) && (lbn <=355)){      good = 1; return good;    }
    if ((lbn >= 357) && (lbn <=363)){      good = 1; return good;    }
    if ((lbn >= 365) && (lbn <=375)){      good = 1; return good;    }
  }
      
  if(rn==183078){
    if ((lbn >= 11) && (lbn <=52)){      good = 1; return good;    }
  }
      
  if(rn==183054){
    if ((lbn >= 100) && (lbn <=172)){      good = 1; return good;    }
    if ((lbn >= 174) && (lbn <=200)){      good = 1; return good;    }
    if ((lbn >= 202) && (lbn <=274)){      good = 1; return good;    }
    if ((lbn >= 276) && (lbn <=276)){      good = 1; return good;    }
  }
      
  if(rn==183045){
    if ((lbn >= 180) && (lbn <=184)){      good = 1; return good;    }
    if ((lbn >= 186) && (lbn <=209)){      good = 1; return good;    }
    if ((lbn >= 211) && (lbn <=233)){      good = 1; return good;    }
    if ((lbn >= 235) && (lbn <=260)){      good = 1; return good;    }
    if ((lbn >= 262) && (lbn <=518)){      good = 1; return good;    }
  }
      
  if(rn==183038){
    if ((lbn >= 61) && (lbn <=80)){      good = 1; return good;    }
  }
      
  if(rn==183021){
    if ((lbn >= 550) && (lbn <=605)){      good = 1; return good;    }
    if ((lbn >= 644) && (lbn <=755)){      good = 1; return good;    }
    if ((lbn >= 757) && (lbn <=789)){      good = 1; return good;    }
    if ((lbn >= 791) && (lbn <=800)){      good = 1; return good;    }
    if ((lbn >= 802) && (lbn <=947)){      good = 1; return good;    }
    if ((lbn >= 949) && (lbn <=1014)){      good = 1; return good;    }
  }
      
  if(rn==183003){
    if ((lbn >= 177) && (lbn <=220)){      good = 1; return good;    }
    if ((lbn >= 228) && (lbn <=261)){      good = 1; return good;    }
    if ((lbn >= 263) && (lbn <=305)){      good = 1; return good;    }
    if ((lbn >= 307) && (lbn <=349)){      good = 1; return good;    }
    if ((lbn >= 351) && (lbn <=488)){      good = 1; return good;    }
    if ((lbn >= 490) && (lbn <=540)){      good = 1; return good;    }
    if ((lbn >= 542) && (lbn <=593)){      good = 1; return good;    }
    if ((lbn >= 595) && (lbn <=597)){      good = 1; return good;    }
    if ((lbn >= 603) && (lbn <=702)){      good = 1; return good;    }
    if ((lbn >= 704) && (lbn <=726)){      good = 1; return good;    }
    if ((lbn >= 728) && (lbn <=728)){      good = 1; return good;    }
    if ((lbn >= 730) && (lbn <=769)){      good = 1; return good;    }
    if ((lbn >= 771) && (lbn <=808)){      good = 1; return good;    }
    if ((lbn >= 810) && (lbn <=946)){      good = 1; return good;    }
    if ((lbn >= 951) && (lbn <=956)){      good = 1; return good;    }
    if ((lbn >= 958) && (lbn <=1074)){      good = 1; return good;    }
  }
      
  if(rn==182997){
    if ((lbn >= 61) && (lbn <=117)){      good = 1; return good;    }
  }
      
  if(rn==182886){
    if ((lbn >= 48) && (lbn <=80)){      good = 1; return good;    }
    if ((lbn >= 82) && (lbn <=99)){      good = 1; return good;    }
    if ((lbn >= 101) && (lbn <=115)){      good = 1; return good;    }
    if ((lbn >= 117) && (lbn <=117)){      good = 1; return good;    }
  }
      
  if(rn==182879){
    if ((lbn >= 331) && (lbn <=370)){      good = 1; return good;    }
    if ((lbn >= 372) && (lbn <=413)){      good = 1; return good;    }
  }
      
  if(rn==182796){
    if ((lbn >= 77) && (lbn <=282)){      good = 1; return good;    }
    if ((lbn >= 284) && (lbn <=319)){      good = 1; return good;    }
    if ((lbn >= 321) && (lbn <=445)){      good = 1; return good;    }
    if ((lbn >= 447) && (lbn <=501)){      good = 1; return good;    }
    if ((lbn >= 503) && (lbn <=573)){      good = 1; return good;    }
    if ((lbn >= 575) && (lbn <=602)){      good = 1; return good;    }
  }
      
  if(rn==182787){
    if ((lbn >= 125) && (lbn <=126)){      good = 1; return good;    }
    if ((lbn >= 140) && (lbn <=185)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=361)){      good = 1; return good;    }
    if ((lbn >= 363) && (lbn <=495)){      good = 1; return good;    }
    if ((lbn >= 497) && (lbn <=510)){      good = 1; return good;    }
    if ((lbn >= 519) && (lbn <=616)){      good = 1; return good;    }
    if ((lbn >= 618) && (lbn <=626)){      good = 1; return good;    }
    if ((lbn >= 630) && (lbn <=728)){      good = 1; return good;    }
    if ((lbn >= 730) && (lbn <=747)){      good = 1; return good;    }
  }
      
  if(rn==182766){
    if ((lbn >= 189) && (lbn <=216)){      good = 1; return good;    }
    if ((lbn >= 218) && (lbn <=229)){      good = 1; return good;    }
    if ((lbn >= 231) && (lbn <=239)){      good = 1; return good;    }
    if ((lbn >= 241) && (lbn <=276)){      good = 1; return good;    }
  }
      
  if(rn==182747){
    if ((lbn >= 14) && (lbn <=40)){      good = 1; return good;    }
    if ((lbn >= 54) && (lbn <=115)){      good = 1; return good;    }
    if ((lbn >= 117) && (lbn <=127)){      good = 1; return good;    }
    if ((lbn >= 129) && (lbn <=160)){      good = 1; return good;    }
    if ((lbn >= 165) && (lbn <=176)){      good = 1; return good;    }
    if ((lbn >= 178) && (lbn <=234)){      good = 1; return good;    }
    if ((lbn >= 236) && (lbn <=259)){      good = 1; return good;    }
    if ((lbn >= 261) && (lbn <=294)){      good = 1; return good;    }
    if ((lbn >= 296) && (lbn <=340)){      good = 1; return good;    }
    if ((lbn >= 342) && (lbn <=620)){      good = 1; return good;    }
  }
      
  if(rn==182726){
    if ((lbn >= 213) && (lbn <=213)){      good = 1; return good;    }
    if ((lbn >= 215) && (lbn <=276)){      good = 1; return good;    }
    if ((lbn >= 281) && (lbn <=315)){      good = 1; return good;    }
  }
      
  if(rn==182519){
    if ((lbn >= 3) && (lbn <=42)){      good = 1; return good;    }
  }
      
  if(rn==182518){
    if ((lbn >= 163) && (lbn <=193)){      good = 1; return good;    }
  }
      
  if(rn==182516){
    if ((lbn >= 284) && (lbn <=347)){      good = 1; return good;    }
    if ((lbn >= 357) && (lbn <=403)){      good = 1; return good;    }
    if ((lbn >= 405) && (lbn <=424)){      good = 1; return good;    }
    if ((lbn >= 426) && (lbn <=484)){      good = 1; return good;    }
  }
      
  if(rn==182486){
    if ((lbn >= 155) && (lbn <=158)){      good = 1; return good;    }
    if ((lbn >= 160) && (lbn <=178)){      good = 1; return good;    }
    if ((lbn >= 180) && (lbn <=283)){      good = 1; return good;    }
    if ((lbn >= 285) && (lbn <=309)){      good = 1; return good;    }
    if ((lbn >= 311) && (lbn <=359)){      good = 1; return good;    }
    if ((lbn >= 361) && (lbn <=437)){      good = 1; return good;    }
    if ((lbn >= 439) && (lbn <=520)){      good = 1; return good;    }
    if ((lbn >= 522) && (lbn <=741)){      good = 1; return good;    }
    if ((lbn >= 743) && (lbn <=748)){      good = 1; return good;    }
    if ((lbn >= 750) && (lbn <=756)){      good = 1; return good;    }
  }
      
  if(rn==182456){
    if ((lbn >= 6) && (lbn <=7)){      good = 1; return good;    }
    if ((lbn >= 9) && (lbn <=15)){      good = 1; return good;    }
    if ((lbn >= 17) && (lbn <=22)){      good = 1; return good;    }
    if ((lbn >= 24) && (lbn <=30)){      good = 1; return good;    }
    if ((lbn >= 32) && (lbn <=35)){      good = 1; return good;    }
    if ((lbn >= 37) && (lbn <=57)){      good = 1; return good;    }
    if ((lbn >= 60) && (lbn <=76)){      good = 1; return good;    }
    if ((lbn >= 78) && (lbn <=91)){      good = 1; return good;    }
    if ((lbn >= 93) && (lbn <=104)){      good = 1; return good;    }
    if ((lbn >= 106) && (lbn <=134)){      good = 1; return good;    }
    if ((lbn >= 136) && (lbn <=159)){      good = 1; return good;    }
    if ((lbn >= 161) && (lbn <=175)){      good = 1; return good;    }
    if ((lbn >= 177) && (lbn <=181)){      good = 1; return good;    }
    if ((lbn >= 183) && (lbn <=183)){      good = 1; return good;    }
    if ((lbn >= 185) && (lbn <=185)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=234)){      good = 1; return good;    }
    if ((lbn >= 236) && (lbn <=247)){      good = 1; return good;    }
    if ((lbn >= 249) && (lbn <=258)){      good = 1; return good;    }
    if ((lbn >= 260) && (lbn <=291)){      good = 1; return good;    }
    if ((lbn >= 293) && (lbn <=293)){      good = 1; return good;    }
    if ((lbn >= 295) && (lbn <=322)){      good = 1; return good;    }
    if ((lbn >= 324) && (lbn <=340)){      good = 1; return good;    }
    if ((lbn >= 342) && (lbn <=364)){      good = 1; return good;    }
    if ((lbn >= 366) && (lbn <=373)){      good = 1; return good;    }
    if ((lbn >= 375) && (lbn <=407)){      good = 1; return good;    }
    if ((lbn >= 409) && (lbn <=469)){      good = 1; return good;    }
    if ((lbn >= 473) && (lbn <=480)){      good = 1; return good;    }
    if ((lbn >= 483) && (lbn <=484)){      good = 1; return good;    }
    if ((lbn >= 486) && (lbn <=562)){      good = 1; return good;    }
  }
      
  if(rn==182455){
    if ((lbn >= 181) && (lbn <=181)){      good = 1; return good;    }
    if ((lbn >= 183) && (lbn <=187)){      good = 1; return good;    }
    if ((lbn >= 189) && (lbn <=190)){      good = 1; return good;    }
    if ((lbn >= 192) && (lbn <=197)){      good = 1; return good;    }
    if ((lbn >= 199) && (lbn <=201)){      good = 1; return good;    }
    if ((lbn >= 204) && (lbn <=208)){      good = 1; return good;    }
    if ((lbn >= 210) && (lbn <=215)){      good = 1; return good;    }
  }
      
  if(rn==182454){
    if ((lbn >= 191) && (lbn <=194)){      good = 1; return good;    }
    if ((lbn >= 196) && (lbn <=198)){      good = 1; return good;    }
    if ((lbn >= 200) && (lbn <=207)){      good = 1; return good;    }
    if ((lbn >= 211) && (lbn <=216)){      good = 1; return good;    }
    if ((lbn >= 218) && (lbn <=220)){      good = 1; return good;    }
    if ((lbn >= 222) && (lbn <=224)){      good = 1; return good;    }
    if ((lbn >= 226) && (lbn <=227)){      good = 1; return good;    }
    if ((lbn >= 229) && (lbn <=230)){      good = 1; return good;    }
    if ((lbn >= 232) && (lbn <=239)){      good = 1; return good;    }
    if ((lbn >= 241) && (lbn <=241)){      good = 1; return good;    }
    if ((lbn >= 243) && (lbn <=244)){      good = 1; return good;    }
    if ((lbn >= 246) && (lbn <=250)){      good = 1; return good;    }
    if ((lbn >= 252) && (lbn <=254)){      good = 1; return good;    }
    if ((lbn >= 256) && (lbn <=260)){      good = 1; return good;    }
    if ((lbn >= 262) && (lbn <=262)){      good = 1; return good;    }
    if ((lbn >= 264) && (lbn <=264)){      good = 1; return good;    }
    if ((lbn >= 268) && (lbn <=272)){      good = 1; return good;    }
    if ((lbn >= 275) && (lbn <=277)){      good = 1; return good;    }
    if ((lbn >= 279) && (lbn <=279)){      good = 1; return good;    }
    if ((lbn >= 283) && (lbn <=285)){      good = 1; return good;    }
    if ((lbn >= 288) && (lbn <=291)){      good = 1; return good;    }
    if ((lbn >= 293) && (lbn <=293)){      good = 1; return good;    }
    if ((lbn >= 295) && (lbn <=306)){      good = 1; return good;    }
    if ((lbn >= 309) && (lbn <=314)){      good = 1; return good;    }
    if ((lbn >= 316) && (lbn <=321)){      good = 1; return good;    }
    if ((lbn >= 323) && (lbn <=343)){      good = 1; return good;    }
    if ((lbn >= 345) && (lbn <=351)){      good = 1; return good;    }
    if ((lbn >= 353) && (lbn <=358)){      good = 1; return good;    }
    if ((lbn >= 361) && (lbn <=366)){      good = 1; return good;    }
    if ((lbn >= 368) && (lbn <=376)){      good = 1; return good;    }
    if ((lbn >= 379) && (lbn <=387)){      good = 1; return good;    }
    if ((lbn >= 389) && (lbn <=391)){      good = 1; return good;    }
    if ((lbn >= 393) && (lbn <=397)){      good = 1; return good;    }
    if ((lbn >= 399) && (lbn <=405)){      good = 1; return good;    }
    if ((lbn >= 408) && (lbn <=412)){      good = 1; return good;    }
  }
      
  if(rn==182450){
    if ((lbn >= 146) && (lbn <=146)){      good = 1; return good;    }
    if ((lbn >= 148) && (lbn <=154)){      good = 1; return good;    }
    if ((lbn >= 156) && (lbn <=162)){      good = 1; return good;    }
    if ((lbn >= 164) && (lbn <=164)){      good = 1; return good;    }
    if ((lbn >= 166) && (lbn <=176)){      good = 1; return good;    }
    if ((lbn >= 178) && (lbn <=182)){      good = 1; return good;    }
    if ((lbn >= 184) && (lbn <=186)){      good = 1; return good;    }
  }
      
  if(rn==182449){
    if ((lbn >= 7) && (lbn <=10)){      good = 1; return good;    }
    if ((lbn >= 12) && (lbn <=32)){      good = 1; return good;    }
    if ((lbn >= 34) && (lbn <=34)){      good = 1; return good;    }
    if ((lbn >= 36) && (lbn <=37)){      good = 1; return good;    }
    if ((lbn >= 39) && (lbn <=40)){      good = 1; return good;    }
    if ((lbn >= 42) && (lbn <=48)){      good = 1; return good;    }
    if ((lbn >= 50) && (lbn <=52)){      good = 1; return good;    }
    if ((lbn >= 54) && (lbn <=55)){      good = 1; return good;    }
    if ((lbn >= 57) && (lbn <=62)){      good = 1; return good;    }
    if ((lbn >= 64) && (lbn <=68)){      good = 1; return good;    }
    if ((lbn >= 70) && (lbn <=72)){      good = 1; return good;    }
    if ((lbn >= 74) && (lbn <=77)){      good = 1; return good;    }
    if ((lbn >= 79) && (lbn <=85)){      good = 1; return good;    }
  }
      
  if(rn==182424){
    if ((lbn >= 112) && (lbn <=122)){      good = 1; return good;    }
    if ((lbn >= 124) && (lbn <=125)){      good = 1; return good;    }
    if ((lbn >= 127) && (lbn <=127)){      good = 1; return good;    }
    if ((lbn >= 129) && (lbn <=136)){      good = 1; return good;    }
    if ((lbn >= 138) && (lbn <=170)){      good = 1; return good;    }
    if ((lbn >= 172) && (lbn <=185)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=214)){      good = 1; return good;    }
    if ((lbn >= 216) && (lbn <=216)){      good = 1; return good;    }
    if ((lbn >= 218) && (lbn <=223)){      good = 1; return good;    }
    if ((lbn >= 226) && (lbn <=235)){      good = 1; return good;    }
    if ((lbn >= 237) && (lbn <=240)){      good = 1; return good;    }
    if ((lbn >= 242) && (lbn <=253)){      good = 1; return good;    }
    if ((lbn >= 261) && (lbn <=274)){      good = 1; return good;    }
    if ((lbn >= 276) && (lbn <=285)){      good = 1; return good;    }
    if ((lbn >= 287) && (lbn <=295)){      good = 1; return good;    }
    if ((lbn >= 297) && (lbn <=300)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=304)){      good = 1; return good;    }
    if ((lbn >= 306) && (lbn <=326)){      good = 1; return good;    }
    if ((lbn >= 329) && (lbn <=352)){      good = 1; return good;    }
    if ((lbn >= 354) && (lbn <=384)){      good = 1; return good;    }
    if ((lbn >= 386) && (lbn <=391)){      good = 1; return good;    }
    if ((lbn >= 393) && (lbn <=402)){      good = 1; return good;    }
    if ((lbn >= 404) && (lbn <=406)){      good = 1; return good;    }
    if ((lbn >= 408) && (lbn <=417)){      good = 1; return good;    }
    if ((lbn >= 419) && (lbn <=425)){      good = 1; return good;    }
    if ((lbn >= 427) && (lbn <=437)){      good = 1; return good;    }
    if ((lbn >= 439) && (lbn <=452)){      good = 1; return good;    }
    if ((lbn >= 455) && (lbn <=459)){      good = 1; return good;    }
    if ((lbn >= 461) && (lbn <=465)){      good = 1; return good;    }
    if ((lbn >= 467) && (lbn <=471)){      good = 1; return good;    }
    if ((lbn >= 473) && (lbn <=473)){      good = 1; return good;    }
    if ((lbn >= 475) && (lbn <=480)){      good = 1; return good;    }
    if ((lbn >= 482) && (lbn <=484)){      good = 1; return good;    }
    if ((lbn >= 487) && (lbn <=491)){      good = 1; return good;    }
    if ((lbn >= 493) && (lbn <=592)){      good = 1; return good;    }
    if ((lbn >= 594) && (lbn <=721)){      good = 1; return good;    }
  }
      
  if(rn==182372){
    if ((lbn >= 5) && (lbn <=10)){      good = 1; return good;    }
    if ((lbn >= 12) && (lbn <=14)){      good = 1; return good;    }
    if ((lbn >= 67) && (lbn <=67)){      good = 1; return good;    }
    if ((lbn >= 70) && (lbn <=90)){      good = 1; return good;    }
    if ((lbn >= 92) && (lbn <=95)){      good = 1; return good;    }
    if ((lbn >= 97) && (lbn <=98)){      good = 1; return good;    }
    if ((lbn >= 100) && (lbn <=123)){      good = 1; return good;    }
    if ((lbn >= 125) && (lbn <=153)){      good = 1; return good;    }
    if ((lbn >= 155) && (lbn <=166)){      good = 1; return good;    }
    if ((lbn >= 168) && (lbn <=171)){      good = 1; return good;    }
    if ((lbn >= 173) && (lbn <=212)){      good = 1; return good;    }
    if ((lbn >= 214) && (lbn <=214)){      good = 1; return good;    }
    if ((lbn >= 216) && (lbn <=224)){      good = 1; return good;    }
    if ((lbn >= 228) && (lbn <=245)){      good = 1; return good;    }
    if ((lbn >= 247) && (lbn <=267)){      good = 1; return good;    }
    if ((lbn >= 269) && (lbn <=270)){      good = 1; return good;    }
    if ((lbn >= 273) && (lbn <=288)){      good = 1; return good;    }
    if ((lbn >= 291) && (lbn <=344)){      good = 1; return good;    }
    if ((lbn >= 346) && (lbn <=354)){      good = 1; return good;    }
    if ((lbn >= 356) && (lbn <=357)){      good = 1; return good;    }
    if ((lbn >= 359) && (lbn <=368)){      good = 1; return good;    }
    if ((lbn >= 370) && (lbn <=402)){      good = 1; return good;    }
    if ((lbn >= 404) && (lbn <=413)){      good = 1; return good;    }
    if ((lbn >= 415) && (lbn <=444)){      good = 1; return good;    }
    if ((lbn >= 446) && (lbn <=465)){      good = 1; return good;    }
    if ((lbn >= 467) && (lbn <=482)){      good = 1; return good;    }
    if ((lbn >= 485) && (lbn <=489)){      good = 1; return good;    }
    if ((lbn >= 491) && (lbn <=505)){      good = 1; return good;    }
  }
      
  if(rn==182346){
    if ((lbn >= 108) && (lbn <=109)){      good = 1; return good;    }
    if ((lbn >= 111) && (lbn <=124)){      good = 1; return good;    }
    if ((lbn >= 126) && (lbn <=126)){      good = 1; return good;    }
    if ((lbn >= 128) && (lbn <=129)){      good = 1; return good;    }
    if ((lbn >= 131) && (lbn <=131)){      good = 1; return good;    }
    if ((lbn >= 133) && (lbn <=133)){      good = 1; return good;    }
    if ((lbn >= 136) && (lbn <=150)){      good = 1; return good;    }
    if ((lbn >= 153) && (lbn <=162)){      good = 1; return good;    }
  }
      
  if(rn==182284){
    if ((lbn >= 318) && (lbn <=340)){      good = 1; return good;    }
    if ((lbn >= 342) && (lbn <=366)){      good = 1; return good;    }
    if ((lbn >= 368) && (lbn <=374)){      good = 1; return good;    }
    if ((lbn >= 376) && (lbn <=385)){      good = 1; return good;    }
    if ((lbn >= 387) && (lbn <=398)){      good = 1; return good;    }
    if ((lbn >= 400) && (lbn <=442)){      good = 1; return good;    }
    if ((lbn >= 444) && (lbn <=460)){      good = 1; return good;    }
    if ((lbn >= 462) && (lbn <=474)){      good = 1; return good;    }
    if ((lbn >= 516) && (lbn <=622)){      good = 1; return good;    }
    if ((lbn >= 626) && (lbn <=750)){      good = 1; return good;    }
  }
      
  if(rn==182161){
    if ((lbn >= 286) && (lbn <=287)){      good = 1; return good;    }
    if ((lbn >= 289) && (lbn <=292)){      good = 1; return good;    }
    if ((lbn >= 295) && (lbn <=315)){      good = 1; return good;    }
    if ((lbn >= 317) && (lbn <=324)){      good = 1; return good;    }
    if ((lbn >= 326) && (lbn <=344)){      good = 1; return good;    }
    if ((lbn >= 346) && (lbn <=368)){      good = 1; return good;    }
    if ((lbn >= 370) && (lbn <=399)){      good = 1; return good;    }
    if ((lbn >= 401) && (lbn <=416)){      good = 1; return good;    }
  }
      
  if(rn==182013){
    if ((lbn >= 637) && (lbn <=668)){      good = 1; return good;    }
    if ((lbn >= 670) && (lbn <=732)){      good = 1; return good;    }
  }
      
  if(rn==180776){
    if ((lbn >= 100) && (lbn <=101)){      good = 1; return good;    }
    if ((lbn >= 103) && (lbn <=113)){      good = 1; return good;    }
  }
      
  if(rn==180710){
    if ((lbn >= 394) && (lbn <=397)){      good = 1; return good;    }
    if ((lbn >= 399) && (lbn <=475)){      good = 1; return good;    }
    if ((lbn >= 477) && (lbn <=534)){      good = 1; return good;    }
    if ((lbn >= 536) && (lbn <=542)){      good = 1; return good;    }
    if ((lbn >= 544) && (lbn <=620)){      good = 1; return good;    }
    if ((lbn >= 622) && (lbn <=745)){      good = 1; return good;    }
  }
      
  if(rn==180664){
    if ((lbn >= 497) && (lbn <=520)){      good = 1; return good;    }
    if ((lbn >= 522) && (lbn <=531)){      good = 1; return good;    }
    if ((lbn >= 533) && (lbn <=581)){      good = 1; return good;    }
  }
      
  if(rn==180636){
    if ((lbn >= 61) && (lbn <=144)){      good = 1; return good;    }
    if ((lbn >= 146) && (lbn <=149)){      good = 1; return good;    }
    if ((lbn >= 151) && (lbn <=236)){      good = 1; return good;    }
    if ((lbn >= 238) && (lbn <=243)){      good = 1; return good;    }
    if ((lbn >= 245) && (lbn <=284)){      good = 1; return good;    }
    if ((lbn >= 286) && (lbn <=298)){      good = 1; return good;    }
    if ((lbn >= 300) && (lbn <=350)){      good = 1; return good;    }
    if ((lbn >= 352) && (lbn <=421)){      good = 1; return good;    }
    if ((lbn >= 423) && (lbn <=484)){      good = 1; return good;    }
    if ((lbn >= 486) && (lbn <=494)){      good = 1; return good;    }
    if ((lbn >= 500) && (lbn <=674)){      good = 1; return good;    }
    if ((lbn >= 676) && (lbn <=732)){      good = 1; return good;    }
    if ((lbn >= 734) && (lbn <=746)){      good = 1; return good;    }
    if ((lbn >= 748) && (lbn <=925)){      good = 1; return good;    }
    if ((lbn >= 932) && (lbn <=1002)){      good = 1; return good;    }
    if ((lbn >= 1004) && (lbn <=1006)){      good = 1; return good;    }
    if ((lbn >= 1010) && (lbn <=1015)){      good = 1; return good;    }
  }
      
  if(rn==180614){
    if ((lbn >= 33) && (lbn <=59)){      good = 1; return good;    }
    if ((lbn >= 61) && (lbn <=124)){      good = 1; return good;    }
    if ((lbn >= 126) && (lbn <=138)){      good = 1; return good;    }
  }
      
  if(rn==180481){
    if ((lbn >= 165) && (lbn <=332)){      good = 1; return good;    }
    if ((lbn >= 334) && (lbn <=392)){      good = 1; return good;    }
    if ((lbn >= 394) && (lbn <=498)){      good = 1; return good;    }
    if ((lbn >= 500) && (lbn <=619)){      good = 1; return good;    }
    if ((lbn >= 621) && (lbn <=649)){      good = 1; return good;    }
    if ((lbn >= 651) && (lbn <=651)){      good = 1; return good;    }
    if ((lbn >= 653) && (lbn <=715)){      good = 1; return good;    }
    if ((lbn >= 717) && (lbn <=766)){      good = 1; return good;    }
    if ((lbn >= 768) && (lbn <=958)){      good = 1; return good;    }
  }
      
  if(rn==180448){
    if ((lbn >= 122) && (lbn <=148)){      good = 1; return good;    }
    if ((lbn >= 150) && (lbn <=161)){      good = 1; return good;    }
  }
      
  if(rn==180400){
    if ((lbn >= 212) && (lbn <=229)){      good = 1; return good;    }
    if ((lbn >= 235) && (lbn <=272)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=442)){      good = 1; return good;    }
    if ((lbn >= 444) && (lbn <=505)){      good = 1; return good;    }
    if ((lbn >= 510) && (lbn <=606)){      good = 1; return good;    }
    if ((lbn >= 615) && (lbn <=717)){      good = 1; return good;    }
    if ((lbn >= 738) && (lbn <=768)){      good = 1; return good;    }
    if ((lbn >= 772) && (lbn <=802)){      good = 1; return good;    }
  }
      
  if(rn==180309){
    if ((lbn >= 75) && (lbn <=121)){      good = 1; return good;    }
    if ((lbn >= 123) && (lbn <=148)){      good = 1; return good;    }
    if ((lbn >= 152) && (lbn <=234)){      good = 1; return good;    }
    if ((lbn >= 236) && (lbn <=266)){      good = 1; return good;    }
    if ((lbn >= 268) && (lbn <=271)){      good = 1; return good;    }
    if ((lbn >= 273) && (lbn <=310)){      good = 1; return good;    }
    if ((lbn >= 322) && (lbn <=419)){      good = 1; return good;    }
    if ((lbn >= 421) && (lbn <=476)){      good = 1; return good;    }
    if ((lbn >= 480) && (lbn <=522)){      good = 1; return good;    }
  }
      
  if(rn==180242){
    if ((lbn >= 90) && (lbn <=96)){      good = 1; return good;    }
    if ((lbn >= 108) && (lbn <=183)){      good = 1; return good;    }
    if ((lbn >= 191) && (lbn <=272)){      good = 1; return good;    }
    if ((lbn >= 290) && (lbn <=367)){      good = 1; return good;    }
  }
      
  if(rn==180241){
    if ((lbn >= 90) && (lbn <=92)){      good = 1; return good;    }
    if ((lbn >= 98) && (lbn <=114)){      good = 1; return good;    }
    if ((lbn >= 117) && (lbn <=126)){      good = 1; return good;    }
    if ((lbn >= 128) && (lbn <=131)){      good = 1; return good;    }
    if ((lbn >= 136) && (lbn <=351)){      good = 1; return good;    }
  }
      
  if(rn==180225){
    if ((lbn >= 4) && (lbn <=299)){      good = 1; return good;    }
    if ((lbn >= 301) && (lbn <=755)){      good = 1; return good;    }
    if ((lbn >= 763) && (lbn <=767)){      good = 1; return good;    }
    if ((lbn >= 781) && (lbn <=977)){      good = 1; return good;    }
  }
      
  if(rn==180212){
    if ((lbn >= 286) && (lbn <=291)){      good = 1; return good;    }
  }
      
  if(rn==180164){
    if ((lbn >= 67) && (lbn <=86)){      good = 1; return good;    }
    if ((lbn >= 88) && (lbn <=102)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=142)){      good = 1; return good;    }
    if ((lbn >= 144) && (lbn <=145)){      good = 1; return good;    }
    if ((lbn >= 147) && (lbn <=149)){      good = 1; return good;    }
    if ((lbn >= 151) && (lbn <=179)){      good = 1; return good;    }
    if ((lbn >= 181) && (lbn <=213)){      good = 1; return good;    }
    if ((lbn >= 215) && (lbn <=230)){      good = 1; return good;    }
    if ((lbn >= 232) && (lbn <=358)){      good = 1; return good;    }
    if ((lbn >= 360) && (lbn <=506)){      good = 1; return good;    }
    if ((lbn >= 509) && (lbn <=561)){      good = 1; return good;    }
    if ((lbn >= 563) && (lbn <=646)){      good = 1; return good;    }
    if ((lbn >= 648) && (lbn <=668)){      good = 1; return good;    }
    if ((lbn >= 670) && (lbn <=707)){      good = 1; return good;    }
    if ((lbn >= 709) && (lbn <=1129)){      good = 1; return good;    }
  }
      
  if(rn==180153){
    if ((lbn >= 5) && (lbn <=46)){      good = 1; return good;    }
    if ((lbn >= 54) && (lbn <=131)){      good = 1; return good;    }
    if ((lbn >= 134) && (lbn <=175)){      good = 1; return good;    }
    if ((lbn >= 177) && (lbn <=240)){      good = 1; return good;    }
    if ((lbn >= 250) && (lbn <=307)){      good = 1; return good;    }
    if ((lbn >= 309) && (lbn <=330)){      good = 1; return good;    }
    if ((lbn >= 332) && (lbn <=363)){      good = 1; return good;    }
    if ((lbn >= 365) && (lbn <=428)){      good = 1; return good;    }
    if ((lbn >= 430) && (lbn <=482)){      good = 1; return good;    }
    if ((lbn >= 493) && (lbn <=531)){      good = 1; return good;    }
  }
      
  if(rn==180149){
    if ((lbn >= 80) && (lbn <=101)){      good = 1; return good;    }
    if ((lbn >= 103) && (lbn <=114)){      good = 1; return good;    }
    if ((lbn >= 119) && (lbn <=145)){      good = 1; return good;    }
    if ((lbn >= 153) && (lbn <=215)){      good = 1; return good;    }
    if ((lbn >= 221) && (lbn <=221)){      good = 1; return good;    }
    if ((lbn >= 224) && (lbn <=225)){      good = 1; return good;    }
    if ((lbn >= 227) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 248) && (lbn <=254)){      good = 1; return good;    }
    if ((lbn >= 256) && (lbn <=262)){      good = 1; return good;    }
  }
      
  if(rn==180144){
    if ((lbn >= 97) && (lbn <=208)){      good = 1; return good;    }
    if ((lbn >= 210) && (lbn <=359)){      good = 1; return good;    }
  }
      
  if(rn==180139){
    if ((lbn >= 126) && (lbn <=146)){      good = 1; return good;    }
    if ((lbn >= 149) && (lbn <=156)){      good = 1; return good;    }
    if ((lbn >= 158) && (lbn <=194)){      good = 1; return good;    }
    if ((lbn >= 196) && (lbn <=239)){      good = 1; return good;    }
    if ((lbn >= 241) && (lbn <=412)){      good = 1; return good;    }
    if ((lbn >= 418) && (lbn <=430)){      good = 1; return good;    }
    if ((lbn >= 432) && (lbn <=466)){      good = 1; return good;    }
    if ((lbn >= 468) && (lbn <=538)){      good = 1; return good;    }
  }
      
  if(rn==180124){
    if ((lbn >= 89) && (lbn <=93)){      good = 1; return good;    }
    if ((lbn >= 95) && (lbn <=120)){      good = 1; return good;    }
    if ((lbn >= 126) && (lbn <=158)){      good = 1; return good;    }
    if ((lbn >= 160) && (lbn <=183)){      good = 1; return good;    }
    if ((lbn >= 188) && (lbn <=228)){      good = 1; return good;    }
    if ((lbn >= 230) && (lbn <=314)){      good = 1; return good;    }
    if ((lbn >= 316) && (lbn <=319)){      good = 1; return good;    }
    if ((lbn >= 325) && (lbn <=334)){      good = 1; return good;    }
    if ((lbn >= 336) && (lbn <=356)){      good = 1; return good;    }
    if ((lbn >= 358) && (lbn <=405)){      good = 1; return good;    }
    if ((lbn >= 407) && (lbn <=479)){      good = 1; return good;    }
    if ((lbn >= 481) && (lbn <=534)){      good = 1; return good;    }
    if ((lbn >= 536) && (lbn <=592)){      good = 1; return good;    }
  }
      
  if(rn==180122){
    if ((lbn >= 210) && (lbn <=246)){      good = 1; return good;    }
    if ((lbn >= 248) && (lbn <=324)){      good = 1; return good;    }
    if ((lbn >= 327) && (lbn <=437)){      good = 1; return good;    }
    if ((lbn >= 439) && (lbn <=450)){      good = 1; return good;    }
    if ((lbn >= 454) && (lbn <=517)){      good = 1; return good;    }
    if ((lbn >= 519) && (lbn <=523)){      good = 1; return good;    }
    if ((lbn >= 525) && (lbn <=601)){      good = 1; return good;    }
    if ((lbn >= 604) && (lbn <=608)){      good = 1; return good;    }
    if ((lbn >= 610) && (lbn <=614)){      good = 1; return good;    }
    if ((lbn >= 616) && (lbn <=625)){      good = 1; return good;    }
    if ((lbn >= 627) && (lbn <=651)){      good = 1; return good;    }
    if ((lbn >= 653) && (lbn <=667)){      good = 1; return good;    }
  }
      
  if(rn==179940){
    if ((lbn >= 6) && (lbn <=16)){      good = 1; return good;    }
  }
      
  if(rn==179939){
    if ((lbn >= 247) && (lbn <=295)){      good = 1; return good;    }
    if ((lbn >= 297) && (lbn <=324)){      good = 1; return good;    }
    if ((lbn >= 342) && (lbn <=439)){      good = 1; return good;    }
    if ((lbn >= 454) && (lbn <=515)){      good = 1; return good;    }
  }
      
  if(rn==179938){
    if ((lbn >= 53) && (lbn <=58)){      good = 1; return good;    }
    if ((lbn >= 61) && (lbn <=89)){      good = 1; return good;    }
    if ((lbn >= 106) && (lbn <=146)){      good = 1; return good;    }
  }
      
  if(rn==179804){
    if ((lbn >= 287) && (lbn <=297)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=318)){      good = 1; return good;    }
    if ((lbn >= 328) && (lbn <=329)){      good = 1; return good;    }
    if ((lbn >= 343) && (lbn <=359)){      good = 1; return good;    }
    if ((lbn >= 361) && (lbn <=368)){      good = 1; return good;    }
    if ((lbn >= 370) && (lbn <=464)){      good = 1; return good;    }
    if ((lbn >= 466) && (lbn <=493)){      good = 1; return good;    }
    if ((lbn >= 499) && (lbn <=605)){      good = 1; return good;    }
    if ((lbn >= 622) && (lbn <=624)){      good = 1; return good;    }
    if ((lbn >= 626) && (lbn <=654)){      good = 1; return good;    }
    if ((lbn >= 656) && (lbn <=708)){      good = 1; return good;    }
    if ((lbn >= 725) && (lbn <=725)){      good = 1; return good;    }
    if ((lbn >= 727) && (lbn <=835)){      good = 1; return good;    }
  }
      
  if(rn==179771){
    if ((lbn >= 86) && (lbn <=106)){      good = 1; return good;    }
    if ((lbn >= 108) && (lbn <=113)){      good = 1; return good;    }
    if ((lbn >= 115) && (lbn <=172)){      good = 1; return good;    }
  }
      
  if(rn==179739){
    if ((lbn >= 158) && (lbn <=183)){      good = 1; return good;    }
    if ((lbn >= 187) && (lbn <=232)){      good = 1; return good;    }
    if ((lbn >= 245) && (lbn <=300)){      good = 1; return good;    }
    if ((lbn >= 302) && (lbn <=329)){      good = 1; return good;    }
    if ((lbn >= 331) && (lbn <=457)){      good = 1; return good;    }
  }
      
  if(rn==179725){
    if ((lbn >= 18) && (lbn <=44)){      good = 1; return good;    }
    if ((lbn >= 46) && (lbn <=50)){      good = 1; return good;    }
    if ((lbn >= 52) && (lbn <=73)){      good = 1; return good;    }
    if ((lbn >= 75) && (lbn <=98)){      good = 1; return good;    }
    if ((lbn >= 100) && (lbn <=166)){      good = 1; return good;    }
    if ((lbn >= 168) && (lbn <=175)){      good = 1; return good;    }
    if ((lbn >= 177) && (lbn <=230)){      good = 1; return good;    }
  }
      
  if(rn==179710){
    if ((lbn >= 284) && (lbn <=402)){      good = 1; return good;    }
    if ((lbn >= 409) && (lbn <=427)){      good = 1; return good;    }
    if ((lbn >= 429) && (lbn <=459)){      good = 1; return good;    }
    if ((lbn >= 467) && (lbn <=475)){      good = 1; return good;    }
    if ((lbn >= 477) && (lbn <=555)){      good = 1; return good;    }
    if ((lbn >= 574) && (lbn <=576)){      good = 1; return good;    }
    if ((lbn >= 586) && (lbn <=611)){      good = 1; return good;    }
    if ((lbn >= 621) && (lbn <=627)){      good = 1; return good;    }
    if ((lbn >= 629) && (lbn <=634)){      good = 1; return good;    }
    if ((lbn >= 637) && (lbn <=646)){      good = 1; return good;    }
    if ((lbn >= 725) && (lbn <=864)){      good = 1; return good;    }
    if ((lbn >= 868) && (lbn <=889)){      good = 1; return good;    }
  }
      
  if(rn==178109){
    if ((lbn >= 154) && (lbn <=182)){      good = 1; return good;    }
    if ((lbn >= 184) && (lbn <=184)){      good = 1; return good;    }
    if ((lbn >= 188) && (lbn <=237)){      good = 1; return good;    }
    if ((lbn >= 247) && (lbn <=320)){      good = 1; return good;    }
    if ((lbn >= 324) && (lbn <=848)){      good = 1; return good;    }
  }
      
  if(rn==178047){
    if ((lbn >= 97) && (lbn <=102)){      good = 1; return good;    }
  }
      
  if(rn==178044){
    if ((lbn >= 109) && (lbn <=207)){      good = 1; return good;    }
    if ((lbn >= 211) && (lbn <=268)){      good = 1; return good;    }
    if ((lbn >= 270) && (lbn <=277)){      good = 1; return good;    }
    if ((lbn >= 287) && (lbn <=306)){      good = 1; return good;    }
    if ((lbn >= 314) && (lbn <=314)){      good = 1; return good;    }
    if ((lbn >= 319) && (lbn <=412)){      good = 1; return good;    }
    if ((lbn >= 416) && (lbn <=424)){      good = 1; return good;    }
    if ((lbn >= 432) && (lbn <=487)){      good = 1; return good;    }
    if ((lbn >= 493) && (lbn <=494)){      good = 1; return good;    }
    if ((lbn >= 499) && (lbn <=536)){      good = 1; return good;    }
    if ((lbn >= 540) && (lbn <=591)){      good = 1; return good;    }
    if ((lbn >= 594) && (lbn <=613)){      good = 1; return good;    }
  }

  return good;
}
