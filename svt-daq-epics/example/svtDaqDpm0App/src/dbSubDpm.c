#include <stdio.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <epicsExport.h>
#include "commonSocket.h"
#include "commonXml.h"
#include "commonDoc.h"
#include <libxml/parser.h>


int mySubDebug = 0;
int process_order = 0;
const int thisDpmId = 0;  


static long subPollInit(subRecord *precord) {
  process_order++;
  if (mySubDebug>-1) {
    printf("[ subPollInit ] %d Record %s called subPollInit(%p)\n", process_order, precord->name, (void*) precord);
  }
  
  return 0;
}


static long subPollProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug>-1)
    printf("[ subPollProcess ]: %d Record %s called subPollProcess(%p)\n",process_order, precord->name, (void*) precord);
  

  socketFDs[thisDpmId] = open_socket("dpm0",8090);
  

  if(socketFDs[thisDpmId]>0) {
    printf("[ subPollProcess ]: successfully opened socket at %d\n", socketFDs[thisDpmId]);

    if(dpm_doc[thisDpmId]!=NULL) {
      printf("[ subPollProcess ]: dpm doc is not null(%p). Clean up.\n", dpm_doc[thisDpmId]);
      xmlFreeDoc(dpm_doc[thisDpmId]);
      xmlCleanupParser();      
      dpm_doc[thisDpmId] = NULL;
    }

    if (mySubDebug>-1)
      printf("[ subPollProcess ]: get the xml doc\n");
    
    getDpmXmlDoc(socketFDs[thisDpmId], 0, &(dpm_doc[thisDpmId]));
    
    
    if (mySubDebug>-1)
      printf("[ subPollProcess ]: found xml doc at %p\n", dpm_doc[thisDpmId]);
    
    
    
/*     if(dpm_doc[thisDpmId]!=NULL) {       */
/*       if (mySubDebug>-1) */
/* 	printf("[ subPollProcess ]: clean up dpm doc (%p)\n", dpm_doc[thisDpmId]); */
/*       xmlFreeDoc(dpm_doc[thisDpmId]); */
/*       xmlCleanupParser();       */
/*       dpm_doc[thisDpmId] = NULL; */
/*    } */
    
  } else {
    printf("[ subPollProcess ]: [ WARNING ]: failed to open socket\n");
  }


  


  if(socketFDs[thisDpmId]>0) {
    printf("[ subPollProcess ]: close socket\n");
    socketFDs[0] = close_socket(socketFDs[0]);
  }

  return 0;
}



/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(subPollInit);
epicsRegisterFunction(subPollProcess);
