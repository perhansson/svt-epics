#ifndef HYBXML_H
#define HYBXML_H

#include "common.h"

xmlXPathObjectPtr getHybVs(xmlDocPtr doc, const char* type, int hyb, const char* pos);
xmlNodePtr getHybVNode(xmlDocPtr doc, const char* type, int index, int hyb, const char* pos);
double getHybVValue(xmlDocPtr doc, const char* type, int index, int hyb, const char* pos);


xmlXPathObjectPtr getHybIs(xmlDocPtr doc, const char* type, int hyb);
xmlNodePtr getHybINode(xmlDocPtr doc, const char* type, int index, int hyb);
double getHybIValue(xmlDocPtr doc, const char* type, int index, int hyb);


xmlXPathObjectPtr getHybTs(xmlDocPtr doc, const char* type, int hyb);
xmlNodePtr getHybTNode(xmlDocPtr doc, const char* type, int index, int hyb);
double getHybTValue(xmlDocPtr doc, const char* type, int index, int hyb);


xmlXPathObjectPtr getHybSwitches(xmlDocPtr doc, int hyb);
xmlNodePtr getHybSwitchNode(xmlDocPtr doc,int index, int hyb);
void getHybSwitchValue(xmlDocPtr doc, xmlChar* value, int index, int hyb);

xmlXPathObjectPtr getHybTrims(xmlDocPtr doc, const char* type, int hyb);
xmlNodePtr getHybTrimNode(xmlDocPtr doc, const char* type, int index, int hyb);
int getHybTrimValue(xmlDocPtr doc, const char* type, int index, int hyb);


#endif
