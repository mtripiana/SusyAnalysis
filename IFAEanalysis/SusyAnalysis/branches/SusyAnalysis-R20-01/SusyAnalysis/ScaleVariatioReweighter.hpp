#ifndef SCALEVARIATIONREWEIGHTER_H
#define SCALEVARIATIONREWEIGHTER_H

#include <iostream>

namespace ScaleVariatioReweighter{
  enum variation{None,ktfacUP,ktfacDOWN,qfacUP,qfacDOWN,ptminUP,ptminDOWN,IqoptUP,IqoptDOWN,ScalesUP,ScalesDOWN,qfacUPktfacDOWN,qfacDOWNktfacUP};
  enum mcperiod{mc11,mc12};

    double GetWeight(int sampleId, variation var, mcperiod mc = mc12);
	
}
#endif
