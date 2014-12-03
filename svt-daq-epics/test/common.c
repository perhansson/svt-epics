
#include "common.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <cstring>

xmlNodePtr getFebNode(xmlDocPtr doc, xmlXPathObjectPtr result, int index, int depth) {
   xmlNodeSetPtr nodeset;
   int i,d;
   xmlNodePtr node;
   xmlNode* n;
   xmlNode* p;
   xmlChar* index_test;
   int index_test_int;
   node = NULL;
   if(result) {
      nodeset = result->nodesetval;
      for(i=0;i<nodeset->nodeNr;++i) {
         n = nodeset->nodeTab[i];
         p=n;
         d = depth;
         while(d>0) {
            p = p->parent;
            d--;
         }
         //printf("%s\n",p->name);
         if(xmlStrcmp(p->name,(xmlChar*)"FebFpga")==0) {
            index_test = xmlGetProp(p,(xmlChar*)"index");   
            if(index_test!=NULL) {
               // printf("index %s\n",index_test);
               index_test_int = atoi((const char*)index_test);
               xmlFree(index_test);
               if(index==index_test_int) {
                  node = n;
                  //printf("found it! %p\n",node);
                  break;
               }           
            } 
         }
      }
   }
   return node;
}


xmlDocPtr
getdoc (char *docname) {
	xmlDocPtr doc;
	doc = xmlParseFile(docname);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	return doc;
}

xmlXPathObjectPtr
getnodeset (xmlDocPtr doc, xmlChar *xpath) {
	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;

	context = xmlXPathNewContext(doc);
	if (context == NULL) {
		printf("Error in xmlXPathNewContext\n");
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL) {
		printf("Error in xmlXPathEvalExpression\n");
		return NULL;
	}
	if(xmlXPathNodeSetIsEmpty(result->nodesetval)){
		xmlXPathFreeObject(result);
                printf("No result\n");
		return NULL;
	}
	return result;
}


double getFloatValue(xmlDocPtr doc, xmlNodePtr node) {
   double t=0.0;
   xmlChar* value;
   if(node!=NULL) {
      //printf("%s found\n",nodeFebFpgaTemp->name);      
      value  = xmlNodeListGetString(doc,node->children,0);
      if(value!=NULL) {
         //printf("FEB FPGA %s \n", value);      
         t = atof((const char*)value);
         //printf("FEB FPGA %f \n", t);      
         xmlFree(value);
      }
   }
   return t;
}

void getStrValue(xmlDocPtr doc, xmlNodePtr node, xmlChar* str) {
   xmlChar* value;
   char* s;
   s = (char*) str;
   if(node!=NULL) {
      value = xmlNodeListGetString(doc,node->children,0);
      if(value!=NULL) {
         strcpy(s,(char*) value);
         xmlFree(value);
      }
   } else {
      strcpy(s,"");
   }
}

int getIntValue(xmlDocPtr doc, xmlNodePtr node) {
   int t=0;
   xmlChar* value;
   if(node!=NULL) {
      //printf("%s found\n",nodeFebFpgaTemp->name);      
      value  = xmlNodeListGetString(doc,node->children,0);
      if(value!=NULL) {
         //printf("FEB FPGA %s \n", value);      
         t = atoi((const char*)value);
         //printf("FEB FPGA %f \n", t);      
         xmlFree(value);
      }
   }
   return t;
}
