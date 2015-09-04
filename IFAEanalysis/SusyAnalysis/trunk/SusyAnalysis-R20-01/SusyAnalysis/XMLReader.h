#ifndef XMLReader_h
#define XMLReader_h

using namespace std;

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <map>
#include <exception>
#include <iostream>
#include <TH1F.h>
#include <TProfile.h>
#include <TH2.h>
#include <TTree.h>
#include <TStyle.h>
#include <TString.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <math.h>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include "TDOMParser.h"
#include "TXMLDocument.h"
#include "TXMLNode.h"
#include "TXMLAttr.h"
#include "TError.h"

class XMLReader {

  public:
    XMLReader();
    ~XMLReader();
    void readXML(TString xmlFileName);
    float retrieveFloat(TString address);
    int   retrieveInt(TString address);
    std::string retrieveChar(TString address);
    bool retrieveBool(TString address);

  private:
    bool isValidNodeName(TXMLNode *node);
    const char* getName(TXMLNode *node);
    const char* readText(TXMLNode *node);
    std::string readAttribute(TXMLNode *node, const char* name);
    TXMLNode* loopOnNode(TXMLNode *parent, TString fullstring, unsigned int step, std::string &retrieveValue);

    TString m_xmlFileName;
    TString m_address;
    TDOMParser* m_xmlparser;
    TXMLDocument* m_xmldoc;
    

};


#endif

