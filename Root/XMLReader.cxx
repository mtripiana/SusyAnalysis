#define XMLReader_cxx

#include "SusyAnalysis/XMLReader.h"

XMLReader::XMLReader(){
  std::cout<<"XMLReader instance is created."<<std::endl;
}

XMLReader::~XMLReader(){
  std::cout<<"XMLReader instance is deleted."<<std::endl;
}

bool XMLReader::isValidNodeName(TXMLNode *node){
  if (TString(node->GetNodeName())!=TString("text")){
    return true;
  }
  else{
    return false;
  }
}

const char* XMLReader::getName(TXMLNode *node){
  return node->GetNodeName();
}

const char* XMLReader::readText(TXMLNode *node){
  return node->GetText();
}

std::string XMLReader::readAttribute(TXMLNode *node, const char* name){
  TString Name(name);
  TString Attribute;
  TListIter attribIt( node->GetAttributes() );
  TXMLAttr* curAttr( 0 );
  while ((curAttr = (TXMLAttr*)attribIt()) != 0) {
    if(curAttr->GetName()==Name){
      Attribute = curAttr->GetValue();
    }
  }

  std::string attributeToReturn = Attribute.Data();
//  cout<<"Inside attribute: "<<Attribute.Data()<<endl;
  return attributeToReturn;
}

void XMLReader::readXML(TString xmlFileName){

  m_xmlparser = new TDOMParser();
  m_xmlparser->SetValidate(kFALSE);

  //--- Parse the xml file to read
  m_xmlparser->ParseFile( xmlFileName.Data() );
  m_xmldoc = m_xmlparser->GetXMLDocument();

}

TXMLNode* XMLReader::loopOnNode(TXMLNode *parent, TString fullstring, unsigned int step, std::string &retrieveValue){

  TString mystring;
  //--- Cut the string
  //
  //--- Separate $
  std::vector<std::string> DollarStrings;
  std::istringstream f(fullstring.Data());
  std::string s;    
  std::string myDollarString;
  std::string cutstring;
  while (std::getline(f, s, '$')) {
    DollarStrings.push_back(s);
//    cout<<"loopOnNode "<<step<<"  "<<s<<endl;
    cutstring += s+'$';
  }
  cutstring.erase(cutstring.size()-1, cutstring.size());

  if (step < DollarStrings.size()){
    myDollarString = DollarStrings.at(step);
  }

  //--- Separate ->
  std::vector<std::string> ArrowStrings;
  std::istringstream fa(myDollarString.c_str());
  std::string sa;    
  std::string myArrowString;
  std::string myArrowAttrName;
  while (std::getline(fa, sa, '/')) {
    ArrowStrings.push_back(sa);
//    cout<<"loopOnNode, after separate, "<<step<<"  "<<sa<<endl;
  }
  if (ArrowStrings.size()==0) {
    myArrowString = myDollarString;
    myArrowAttrName = "";
  }
  else if (ArrowStrings.size()==1) {
    myArrowString = ArrowStrings.at(0);
  }
  else{
    myArrowString = ArrowStrings.at(0);
    myArrowAttrName = ArrowStrings.at(1);
  }
//  cout<<"I separate it --> "<<myArrowString<<endl;

  while (parent != 0){
//    cout<<"Point 1"<<endl;
    if (XMLReader::isValidNodeName(parent)){
//      cout<<"Point 2"<<endl;
//      cout<<"string parent at point 2: "<<XMLReader::getName(parent)<<endl;
//      cout<<"myarrowstring at point 2: "<<TString(myArrowString.c_str())<<endl;
      if (TString(myArrowString.c_str())==TString(XMLReader::getName(parent))){
//        cout<<"Point 3 arrowstrings size"<<ArrowStrings.size()<<endl;
        if (ArrowStrings.size()>1){
//          cout<<"attribute of "<<myArrowAttrName<<": "<<XMLReader::readAttribute(parent,myArrowAttrName.c_str())<<"  arrowstring: "<<ArrowStrings.at(2)<<endl;
          if(TString(XMLReader::readAttribute(parent,myArrowAttrName.c_str()).c_str())==TString(ArrowStrings.at(2).c_str())){
//            cout<<"Point 4"<<endl;
            TXMLNode * children = parent->GetChildren();
//            cout<<"Step = "<<step<<"  DollarStrings.size() = "<< DollarStrings.size()-1<<endl;
            if (step==DollarStrings.size()-1){
              retrieveValue = XMLReader::readText(parent);
              return 0;
            }
//            cout<<"Point 5 "<<cutstring<<endl;
//            cout<<"string parent: "<<XMLReader::getName(parent)<<endl;
            return loopOnNode(children, cutstring, step+1, retrieveValue);
          }
        }
        else {
//          cout<<"Point 4b"<<endl;
          TXMLNode * children = parent->GetChildren();
//          cout<<"Step = "<<step<<"  DollarStrings.size() = "<< DollarStrings.size()-1<<endl;
          if (step==DollarStrings.size()-1){
            retrieveValue = XMLReader::readText(parent);
            return 0;
          }
//          cout<<"Point 5b "<<cutstring<<endl;
//          cout<<"string parent: "<<XMLReader::getName(parent)<<endl;
          return loopOnNode(children, cutstring, step+1, retrieveValue);
        }
      }
    }
    parent = parent->GetNextNode();
  }
  return 0;
}

float XMLReader::retrieveFloat(TString address){

  TXMLNode* parent;
  TXMLNode* final;
  parent = m_xmldoc->GetRootNode();

  std::string retrieveValue = "";

  final = XMLReader::loopOnNode(parent, address, 0, retrieveValue);

  if (retrieveValue == ""){ 
    throw 1;
  }

  Info("XMLReader::retrieveFloat", Form("%s --> %s", address.Data(), retrieveValue.c_str()) );
  return atof(retrieveValue.c_str());
}

int XMLReader::retrieveInt(TString address){

  TXMLNode* parent;
  TXMLNode* final;
  parent = m_xmldoc->GetRootNode();

  std::string retrieveValue = "";

  final = XMLReader::loopOnNode(parent, address, 0, retrieveValue);

  if (retrieveValue == "") {
    throw 2;
  }

  Info("XMLReader::retrieveInt", Form("%s --> %s", address.Data(), retrieveValue.c_str()) );
  return atoi(retrieveValue.c_str());
}

std::string XMLReader::retrieveChar(TString address){

  TXMLNode* parent;
  TXMLNode* final;
  parent = m_xmldoc->GetRootNode();

  std::string retrieveValue = "";

  final = XMLReader::loopOnNode(parent, address, 0, retrieveValue);

  if (retrieveValue == "") {
    throw 3;
  }

  Info("XMLReader::retrieveChar", Form("%s --> %s", address.Data(), retrieveValue.c_str()) );
  return retrieveValue;
}

bool XMLReader::retrieveBool(TString address){

  TXMLNode* parent;
  TXMLNode* final;
  parent = m_xmldoc->GetRootNode();

  std::string retrieveValue = "";

  final = XMLReader::loopOnNode(parent, address, 0, retrieveValue);

  if (retrieveValue == "") {
    throw 4;
  }

  if (TString(retrieveValue.c_str())==TString("true")){
    Info("XMLReader::retrieveBool", Form("%s --> %s", address.Data(), retrieveValue.c_str()));
    return true;
  }
  else {
    Info("XMLReader::retrieveBool", Form("%s --> %s", address.Data(), retrieveValue.c_str()));
    return false;
  }
}
