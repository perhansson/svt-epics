#include "hybxml.h"
#include <cstring>



xmlXPathObjectPtr getHybVs(xmlDocPtr doc, const char* type, int hyb, const char* pos) {
   char tmp[256];
   sprintf(tmp,"/system/status/ControlDpm/FebFpga/FebCore/SoftPowerMonitor/Hybrid%d_%s_%s",hyb,type,pos);
   return getnodeset(doc, (xmlChar*) tmp);
}


xmlNodePtr getHybVNode(xmlDocPtr doc, const char* type, int index, int hyb, const char* pos) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getHybVs(doc, type, hyb, pos);
   node = getFebNode(doc, result, index, 3);
   return node;
}



double getHybVValue(xmlDocPtr doc, const char* type, int index, int hyb, const char* pos) {
   double t;
   xmlChar* value;
   xmlNodePtr node;
   node = getHybVNode(doc,type, index, hyb, pos);
   t = getFloatValue(doc, node);
   return t;
}



xmlXPathObjectPtr getHybIs(xmlDocPtr doc, const char* type, int hyb) {
   char tmp[256];
   sprintf(tmp,"/system/status/ControlDpm/FebFpga/FebCore/SoftPowerMonitor/Hybrid%d_%s_Current",hyb,type);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getHybINode(xmlDocPtr doc, const char* type, int index, int hyb) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getHybIs(doc, type, hyb);
   node = getFebNode(doc, result, index, 3);
   return node;
}

double getHybIValue(xmlDocPtr doc, const char* type, int index, int hyb) {
   double t;
   xmlChar* value;
   xmlNodePtr node;
   node = getHybINode(doc,type, index, hyb);
   t = getFloatValue(doc, node);
   return t;
}




xmlXPathObjectPtr getHybTs(xmlDocPtr doc, const char* type, int hyb) {
   char tmp[256];
   sprintf(tmp,"/system/status/ControlDpm/FebFpga/FebCore/PowerMonitor/Hybrid%d_%s",hyb,type);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getHybTNode(xmlDocPtr doc, const char* type, int index, int hyb) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getHybTs(doc, type, hyb);
   node = getFebNode(doc, result, index, 3);
   return node;
}

double getHybTValue(xmlDocPtr doc, const char* type, int index, int hyb) {
   double t;
   xmlChar* value;
   xmlNodePtr node;
   node = getHybTNode(doc,type, index, hyb);
   t = getFloatValue(doc, node);
   return t;
}





xmlXPathObjectPtr getHybSwitches(xmlDocPtr doc, int hyb) {
   char tmp[256];
   sprintf(tmp,"/system/config/ControlDpm/FebFpga/FebCore/Hybrid%dPwrEn",hyb);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getHybSwitchNode(xmlDocPtr doc, int index, int hyb) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getHybSwitches(doc, hyb);
   node = getFebNode(doc, result, index, 2);
   return node;
}

void getHybSwitchValue(xmlDocPtr doc,  xmlChar* value, int index, int hyb) {
   xmlNodePtr node;
   node = getHybSwitchNode(doc, index, hyb);
   getStrValue(doc, node, value);
}









xmlXPathObjectPtr getHybTrims(xmlDocPtr doc, const char* type, int hyb) {
   char tmp[256];
   //monkey around with case
   char s[10];
   if(strcmp(type,"AVDD")==0) strcpy(s,"Avdd");
   else if(strcmp(type,"DVDD")==0) strcpy(s,"Dvdd");
   else if(strcmp(type,"V125")==0) strcpy(s,"V1_25");
   else {
      printf("[ getHybTrims ] : [ WARNING ] : this type is not supported \"%s\"\n",type);
      return NULL;
   }   
   sprintf(tmp,"/system/config/ControlDpm/FebFpga/FebCore/Hybrid%d%sTrim",hyb, s);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getHybTrimNode(xmlDocPtr doc, const char* type, int index, int hyb) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getHybTrims(doc, type, hyb);
   node = getFebNode(doc, result, index, 2);
   return node;
}

int getHybTrimValue(xmlDocPtr doc, const char* type, int index, int hyb) {
   int t;
   xmlChar* value;
   xmlNodePtr node;
   node = getHybTrimNode(doc,type, index, hyb);
   t = getIntValue(doc, node);
   return t;
}
