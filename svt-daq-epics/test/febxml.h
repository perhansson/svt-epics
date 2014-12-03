#ifndef FEBXML_H
#define FEBXML_H

#include "common.h"

xmlXPathObjectPtr getFebTemps(xmlDocPtr doc, const char* type);
xmlNodePtr getFebTempNode(xmlDocPtr doc, const char* type, int index);
double getFebTempValue(xmlDocPtr doc, const char* type, int index);


#endif
