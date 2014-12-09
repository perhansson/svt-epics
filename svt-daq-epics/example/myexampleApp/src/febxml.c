#include "febxml.h"
#include "constants.h"

xmlXPathObjectPtr getFebTemps(xmlDocPtr doc, const char* type) {
   char tmp[256];
   sprintf(tmp,"/system/status/ControlDpm/FebFpga/FebCore/SoftPowerMonitor/%s",type);
   if(DEBUG>1) printf("[ getFebTemps ] : xpath %s\n", tmp);
   return getnodeset(doc, (xmlChar*) tmp);
}

xmlNodePtr getFebTempNode(xmlDocPtr doc, const char* type, int index) {
   xmlXPathObjectPtr result;
   xmlNodePtr node;
   result = getFebTemps(doc, type);
   if(DEBUG>1) printf("[ getFebTempNode ] : found result at %p\n", result);
   node = getFebNode(doc, result, index, 3);
   if(DEBUG>1) printf("[ getFebTempNode ] : found node at %p\n", node);
   return node;
}

double getFebTempValue(xmlDocPtr doc, const char* type, int index) {
   double t;
   xmlNodePtr node;
   node = getFebTempNode(doc,type, index);
   if(DEBUG>1) printf("[ getFebTempValue ] : found node at %p\n", node);
   t = getFloatValue(doc, node);
   if(DEBUG>1) printf("[ getFebTempValue ] : got t %f\n",t);
   return t;
}

