#include "febxml.h"

xmlXPathObjectPtr getFebTemps(xmlDocPtr doc, const char* type) {
   char tmp[256];
   sprintf(tmp,"/system/status/ControlDpm/FebFpga/FebCore/SoftPowerMonitor/%s",type);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getFebTempNode(xmlDocPtr doc, const char* type, int index) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getFebTemps(doc, type);
   node = getFebNode(doc, result, index, 3);
   return node;
}

double getFebTempValue(xmlDocPtr doc, const char* type, int index) {
   double t;
   xmlChar* value;
   xmlNodePtr node;
   node = getFebTempNode(doc,type, index);
   t = getFloatValue(doc, node);
   return t;
}

