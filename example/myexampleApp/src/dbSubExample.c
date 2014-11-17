#include <stdio.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <tcpipExample.h>
#include <stringinRecord.h>
#include <longinRecord.h>
#include <dbAccess.h>
#include <client_util.h>


int mySubDebug = 0;
int process_order = 0;
const int BUF_SIZE = 256;
char* hostNameDef = "134.79.229.141";
char hostName[256];
int portDef = 8091;
int port;
int sockfd = 0;
int counter = 0;
int status_poll_flag=0;
int status_flag=0;
const double def_hyb_v = -999.9;
const double def_hyb_i = -999.9;
const double def_hyb_t = -999.9;
const double def_AxiXadcTemp = -999.9;

static long subLVInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("%d Record %s called subLVInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}

static long subTempInit(subRecord *precord) {
   process_order++;
  if (mySubDebug) {
    printf("%d: Record %s called subTempInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}

static void getIpFromRecord(subRecord* precord, char value[], const int MAX) {
  char inpa_val[40]; // 40 is maximum size of stringin/out records
  memset(inpa_val,0,40);
  snprintf( inpa_val, 40, "%s", (char*) ((precord->inpa).value.constantStr) ) ;
  if(strlen(inpa_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpa_val,&paddr)!=0) {
      printf("ERROR: getIpFromRecord: dbNameToAddr for %s failed (paddr=%p)\n",inpa_val,&paddr);
    }
    struct stringinRecord* recA = (stringinRecord*)paddr.precord;
    if (mySubDebug>1) printf("getIpFromRecord: recA at %p\n",recA);
    if(recA!=NULL) {
      if (mySubDebug>1) printf("getIpFromRecord: recA name %s val %p\n",recA->name,recA->val);
      char inpa_val2[40];
      memset(inpa_val2,0,40);
      snprintf( inpa_val2, 40, "%s", recA->val ) ;
      if (mySubDebug>1) printf("getIpFromRecord: got IP %s\n",inpa_val2);
      if(strlen(inpa_val2)<MAX) {
	strcpy(value,inpa_val2);
      } else {
	printf("WARNING: getIpFromRecord: IP from DB is too long? %s \n",inpa_val2);
      }
    } else {
      printf("WARNING: getIpFromRecord: cannot get IP record from inpa_val %s \n",inpa_val);
    }
  } else {
      printf("WARNING: getIpFromRecord: INPA string has zero length \n");    
  }
}
static int getPortFromRecord(subRecord* precord) {
  char inpb_val[40]; // 40 is maximum size of stringin/out records
  memset(inpb_val,0,40);
  snprintf( inpb_val, 40, "%s", (char*) ((precord->inpb).value.constantStr) ) ;
  if(strlen(inpb_val)>0) {
    dbAddr paddr;
    if(dbNameToAddr(inpb_val,&paddr)!=0) {
      printf("getPortFromRecord: dbNameToAddr for %s failed (paddr=%p)\n",inpb_val,&paddr);
    }
    struct longinRecord* recA = (longinRecord*)paddr.precord;
    if (mySubDebug>1) printf("getPortFromRecord: recA at %p\n",recA);
    if(recA!=NULL) {
      if (mySubDebug>1) printf("getPortFromRecord: recA name %s val %d\n",recA->name,recA->val);
      return recA->val;
    } else {
      printf("WARNING: getPortFromRecord: cannot get port record from inpb_val %s \n",inpb_val);
    }
  } else {
      printf("WARNING: getIpFromRecord: INPB string has zero length \n");    
  }
  return -1;
}
  
static void setupSocket(subRecord *precord) {
  process_order++;
  if (mySubDebug>1) {
    printf("%d: Record %s called setupSocket(%p)\n", process_order, precord->name, (void*) precord);
  }

  // only setup socket from DB if it's the poll record?
  if(strcmp(precord->name,"SVT:poll_xml")==0) {    
    // look for host name and port in DB
    if (mySubDebug>1) printf("setupSocket: look for host name and port in DB\n");
    char host[40];
    int p;
    getIpFromRecord(precord,host,40);
    p = getPortFromRecord(precord);
    if (mySubDebug>1) printf("Got host \"%s\" port %d\n",host,p);
    if(strlen(host)==0 || p<=0) {
      printf("ERROR: setupSocket: No hostname or port found in DB.\n");
      exit(1);
    } else {
      strcpy(hostName,host);
      port = p;
    }      
  }

  if(strcmp(hostName,"")==0) {
    printf("ERROR: setupSocket: no valid hostname found. Fix this.\n");
    exit(1);
  } 
  
}


static long subPollInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("%d: Record %s called subPollInit(%p)\n", process_order, precord->name, (void*) precord);
  }

  return 0;
}


static long subPollStatInit(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("%d: Record %s called subPollStatInit(%p)\n", process_order, precord->name, (void*) precord);
  }
  
  return 0;
}


  static void writeHybrid(subRecord* precord,char action[], int id, int feb_id, char ch_name[])
{
  if(mySubDebug) {
    printf("Record %s called writeHybrid %s with val %f for feb_id= %d  id=%d ch_name=%s\n", precord->name,action,precord->val,feb_id,id,ch_name);
  }
  time_t cur_time;
  time_t timer;
  time(&timer);
  int dt;
  // get a valid socket
  dt = 0;
  if(sockfd>0) {
    if (mySubDebug) printf("writeHybrid: socket %d is already open, wait for it to close\n", sockfd);    
    while(sockfd>0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug) printf("writeHybrid: socket %d is still open after %ds, sleep 1s\n", sockfd, dt);    
      sleep(1);
    }    
  }
  
  if(sockfd>0) {    
    printf("WARNING: writeHybrid: socket %d was still open after %ds, don't write anything\n", sockfd, dt); 
    return;
  } 
  else {    
    if (mySubDebug) printf("writeHybrid: Opening socket: host: %s:%d\n",hostName, port);    
    setupSocket(precord);
    sockfd = open_socket(hostName,port);
    if (mySubDebug) printf("writeHybrid: Opened socket : %d\n",sockfd);            
  }
  
  if(sockfd<=0) {
    printf("ERROR: writeHybrid: Failed to open socket in writeHybrid (host %s:%d) \n",hostName,sockfd);        
    return;
  }
  
  if(strcmp(action,"v_set_sub")==0) {    
    if(precord->val<255 && precord->val>0) {
      
      writeHybridVTrim(sockfd,(int)precord->val, id, ch_name);    
      
      if (mySubDebug) printf("writeHybrid: Poll xml string after write\n");
      
      getXmlDoc(sockfd,0,0);
      
      if (mySubDebug) printf("writeHybrid:  Poll XML done after write.\n");
      
    } else {
      printf("ERROR: writeHybrid: voltage trim %f is not allowed!\n",precord->val);
      exit(1);
    }
    
  } 
  else if(strcmp(action,"switch_sub")==0) {    

    if(strcmp(ch_name,"all")==0) {
      
      int val = (int)precord->val;
      if(val==0 || val==1) {
	
	writeHybridVSwitch(sockfd, val, id);    
	
	if (mySubDebug) printf("writeHybrid: Poll xml string after write\n");

	getXmlDoc(sockfd,0,0);

	if (mySubDebug) printf("writeHybrid:  Poll XML done after write.\n");
	
      } else {
	printf("ERROR: writeHybrid: voltage switch %d is not allowed!\n",val);
	exit(1);
      }

    } else {
      printf("WARNING: writeHybrid: this ch_name %s for action %s is not defined yet\n",ch_name,action);
    }    
  }
  else {
    printf("ERROR: writeHybrid: this action \"%s\" for writeHybrid is not defined!\n",action);
    exit(1);
  }
  
  if (mySubDebug) {
    printf("writeHybrid: Closing socket\n");
  }
  
  sockfd = close_socket(sockfd);
  
  if (mySubDebug) {
    printf("writeHybrid:  after closing socket is %d\n",sockfd);
  }

}




  static void readHybrid(subRecord* precord,char action[], int id, int feb_id, char ch_name[])
{
  double constant;
  if(mySubDebug) {
    printf("Record %s called readHybrid %s for feb_id= %d  id=%d ch_name=%s\n", precord->name,action,feb_id,id,ch_name);
  }

  //set to default
  //precord->val = def_hyb_t;
  
  char value[BUF_SIZE];
  memset(value,0,BUF_SIZE);
  constant = 1.;
  if(strcmp(action,"i_rd_sub")==0) {
    readHybridI(feb_id, id, ch_name, value, BUF_SIZE);
  } 
  else if(strcmp(action,"t_rd_sub")==0) {
    int tId;
    if(strcmp(ch_name,"temp0")==0) {
      tId = 0;
    }
    else if(strcmp(ch_name,"temp1")==0) {
      tId = 1;
    }
    else {
      printf("ERROR: in readHybrid: the ch_name %s for action %s is not defined!\n",ch_name,action);
      exit(1);
    }
    readHybridT(feb_id, id, tId, value, BUF_SIZE);
  } 
  else if(strcmp(action,"vn_sub")==0) {
    readHybridV(feb_id, id, ch_name, "Near", value, BUF_SIZE);
  } 
  else if(strcmp(action,"vf_sub")==0) {
    readHybridV(feb_id, id, ch_name, "Far", value, BUF_SIZE);
  } 
  else if(strcmp(action,"v_set_rd_sub")==0) {
    readHybridVTrim(feb_id, id, ch_name, value, BUF_SIZE);
  } 
  else {
    printf("wrong action for readHybrid \"%s\"\n",action);
    return;
  }
  if (mySubDebug) {
    printf("Got value=\"%s\"\n",value);
  }
  if(strlen(value)>0) {
    precord->val = atof(value)*constant;
    if (mySubDebug) {
      printf("precord-val is now %f \n",precord->val);
    }
  }
}


  static void readFeb(subRecord* precord,char action[], int feb_id, char ch_name[])
{
  if(mySubDebug) {
    printf("Record %s called readFeb %s feb_id=%d ch_name=%s\n",precord->name,action,feb_id,ch_name);
  }

  
  char value[BUF_SIZE];
  memset(value,0,BUF_SIZE);
  if(strcmp(action,"t_rd_sub")==0) {
    //set to default
    //precord->val=def_AxiXadcTemp;
    readFebT(feb_id, value, ch_name, BUF_SIZE);
    
  } 
  else {
    printf("No such action %s implemented for readFeb!\n",action);
    return;
  } 
  if (mySubDebug) {
    printf("Got value=\"%s\"\n",value);
  }
  if(strlen(value)>0) {
    precord->val = atof(value);
    if (mySubDebug) {
      printf("precord-val is now %f \n",precord->val);
    }
  }
}






static long subLVProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("%d: Record %s called subLVProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  //SVT:lv:hyb:bot:0:dvdd:vn_sub
  //SVT:lv:hyb:bot:0:dvdd:v_set_sub
  int feb_id;
  char type[BUF_SIZE];
  char board_type[BUF_SIZE];
  char half[BUF_SIZE];
  char id[BUF_SIZE];
  char ch_name[BUF_SIZE];
  char action[BUF_SIZE];
  getType(precord->name,type,BUF_SIZE);
  getBoardType(precord->name,board_type,BUF_SIZE);
  getHalf(precord->name,half,BUF_SIZE);
  getId(precord->name,id,BUF_SIZE);
  getChName(precord->name,ch_name,BUF_SIZE);
  getAction(precord->name,action,BUF_SIZE);

  if (mySubDebug) {
    printf("Record %s has type %s board type \"%s\"\n", precord->name, type,board_type);
  }

  if(strcmp(type,"lv")!=0) {
    printf("ERROR this type is not valid \"%s\"\n",type);
    return 0;
  }
  
  char* p_end = id;
  long int li_id = strtol(id,&p_end,0);
  if(p_end!=id) {
    if(li_id<0 && li_id>17) {     
      printf("ERROR this hybrid id %ld is not valid\n",li_id);
      return 0;
    }
  } else {
    printf("ERROR converting this hybrid id %s is not valid\n",id);
    return 0;      
  }
  feb_id = getFebIdFromDaqMap((int)li_id,half);
  if(feb_id<0) {
    printf("ERROR getting feb id\n");
    return 0;
  } 
  
  if(strcmp(board_type,"hyb")==0) {
   
    if(strcmp(action,"vn_sub")==0 || strcmp(action,"vf_sub")==0 || strcmp(action,"i_rd_sub")==0 || strcmp(action,"v_set_rd_sub")==0) {

      if(strcmp(ch_name,"dvdd")!=0 && strcmp(ch_name,"avdd")!=0 && strcmp(ch_name,"v125")!=0) {
	printf("ERROR wrong option for hybrid ch: %s\n",ch_name);
	return 0;
      }
      readHybrid(precord,action,(int)li_id,feb_id,ch_name);  
      
    } else if(strcmp(action,"v_set_sub")==0 || strcmp(action,"switch_sub")==0) { 

      if(strcmp(ch_name,"dvdd")!=0 && strcmp(ch_name,"avdd")!=0 && strcmp(ch_name,"v125")!=0 && strcmp(ch_name,"all")!=0) {
	printf("ERROR wrong option for hybrid ch: %s\n",ch_name);
	return 0;
      }

      writeHybrid(precord,action,(int)li_id,feb_id,ch_name);  

    } else {
      printf("ERROR this hybrid action type is not valid \"%s\"\n",action);
      return 0;
    }    
  }
  else {
    printf("ERROR this board type is not valid \"%s\" for LV\n",board_type);
    return 0;
  }
  
  
  
  
  return 0;
}



static long subTempProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug) {
    printf("%d: Record %s called subTempProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  //SVT:temp:hyb:bot:0:temp1:t_rd_sub
  //SVT:temp:hyb:bot:0:temp2:t_rd_sub
  //SVT:temp:fe:0:axixadc:t_rd_sub)
  int feb_id;
  char type[BUF_SIZE];
  memset(type,0,BUF_SIZE);
  char board_type[BUF_SIZE];
  char half[BUF_SIZE];
  char id[BUF_SIZE];
  char ch_name[BUF_SIZE];
  char action[BUF_SIZE];

  getType(precord->name,type,BUF_SIZE);
  getBoardType(precord->name,board_type,BUF_SIZE);


  if (mySubDebug) {
    printf("Record %s has type %s board type \"%s\"\n", precord->name, type,board_type);
  }

  //  char tmp[BUF_SIZE];
  int type_cmp;
  if((type_cmp=strcmp(type,"temp"))!=0) {
    printf("ERROR this type is not valid \"%s\" cmp %d\n",type,type_cmp);
    return 0;
  }

  if(strcmp(board_type,"hyb")==0) {  
    getHalf(precord->name,half,BUF_SIZE);
    getId(precord->name,id,BUF_SIZE);
    getChName(precord->name,ch_name,BUF_SIZE);
    getAction(precord->name,action,BUF_SIZE);

    if(strcmp(ch_name,"temp0")!=0 && strcmp(ch_name,"temp1")!=0) {
      printf("ERROR wrong option for hybrid ch: %s\n",ch_name);
      return 0;
    }

    char* p_end = id;
    long int li_id = strtol(id,&p_end,0);
    if(p_end!=id) {
      if(li_id<0 && li_id>17) {     
	printf("ERROR this hybrid id %ld is not valid\n",li_id);
	return 0;
      }
    } else {
      printf("ERROR converting this hybrid id %s is not valid\n",id);
      return 0;      
    }
    
    feb_id = getFebIdFromDaqMap((int)li_id,half);
    if(feb_id<0) {
      printf("ERROR getting feb id\n");
      return 0;
    } 

   
    readHybrid(precord,action,(int)li_id,feb_id,ch_name);  


  } 
  else if(strcmp(board_type,"fe")==0) {  
    sprintf(half,"%s","bot");
    getFebId(precord->name,id,BUF_SIZE);
    getFebChName(precord->name,ch_name,BUF_SIZE);
    getFebAction(precord->name,action,BUF_SIZE);  
    feb_id = atoi(id);
    if(strcmp(action,"t_rd_sub")!=0) {
      printf("ERROR this feb action type is not valid \"%s\"\n",action);
      return 0;
    }  
    if(strcmp(ch_name,"axixadc") !=0 && strcmp(ch_name,"FebTemp0") !=0 && strcmp(ch_name,"FebTemp1") !=0 ) {
      printf("This ch name is not implemented for for readFeb: \"%s\" !\n",ch_name);
      return 0;      
    }
    
    readFeb(precord,action,feb_id,ch_name);  
    

  } 
  else {
    printf("ERROR this board type is not valid \"%s\"\n",board_type);
    return 0;
  }
  

  
  
  return 0;
}



static void updatePollStatusFlag() {
  // get the status from the client utils
  int status = getXMLPollStatus();
  // flip the status flag if xml poll was ok.
  if (mySubDebug) {
    printf("updatePollStatusFlag: start status_poll_flag = %d\n",status_poll_flag);
    printf("updatePollStatusFlag: getXMLPollStatus=%d\n",status);
  }
  if(status==1) {
    if(status_poll_flag==0) {
      status_poll_flag = 1;
    } else {
      status_poll_flag = 0;
    }
  }
  if (mySubDebug) printf("updatePollStatusFlag: end status_poll_flag = %d\n",status_poll_flag);
}



static long subPollProcess(subRecord *precord) {
  time_t cur_time;
  int dt;
  time_t timer;

  process_order++;
  if (mySubDebug>0) {
    printf("%d: Record %s called subPollProcess(%p)\n",process_order, precord->name, (void*) precord);
  }
  
  // check that the socket is available
  dt=0;
  time(&timer);
  if(sockfd>0) {
    if (mySubDebug>0) printf("subPollProcess: socket %d is already open, wait for it to close\n", sockfd);    
    while(sockfd>0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug>0) printf("subPollProcess: socket %d is still open (%ds)\n", sockfd, dt);    
      sleep(1);
    }    
  }
  
  if(sockfd>0) {    
    printf("WARNING: subPollProcess: socket %d was still open after %ds, don't write anything\n", sockfd, dt); 
    return 0;
  } 
  else {

    if (mySubDebug>0) printf("subPollProcess: socket is available now (socket %d after %ds)\n", sockfd, dt); 
    // get a valid socket
    while(sockfd<=0 && dt<6) {
      time(&cur_time);
      dt = difftime(cur_time, timer);
      if (mySubDebug>0) printf("subPollProcess: try to setup socket (%ds)\n",dt);

      setupSocket(precord);

      sockfd = open_socket(hostName,port);
      
      if(sockfd<=0) {
	if (mySubDebug>0) printf("subPollProcess: couln't get socket, sleep before retrying\n");	
	sleep(1);
      }
    }

  }
  
  // poll the xml string
  
  if(sockfd<=0) {
    printf("subPollProcess: couldn't open a socket (tried over %ds period). Check host and port?\n",dt);
  }
  
  if (mySubDebug) printf("subPollProcess: Poll xml string\n");
  
  getXmlDoc(sockfd,0,0);
  
  if (mySubDebug) printf("subPollProcess: Poll XML done. Close socket if needed\n");
  if(sockfd>0) {
    sockfd = close_socket(sockfd);
  } 

  if(mySubDebug>1) {
    char * s = NULL;
    int len;
    getXmlDocStrFormat(&s, &len);
    printf("subPollProcess: got XML with len %d\n", len);
    if(len>0) printf("\n%s\n",s);
    if(s!=NULL) {
      printf("subPollProcess: free string at %p\n",s);      
      free(s);
      printf("subPollProcess: done free string at %p\n",s);      
    }
  }

  if (mySubDebug) printf("subPollProcess: before update status_poll_flag = %d\n", status_poll_flag);
  
  updatePollStatusFlag();
  
  if (mySubDebug) printf("subPollProcess: after update status_poll_flag = %d\n", status_poll_flag);


  return 0;
}




static long subPollStatProcess(subRecord *precord) {
  process_order++;
  if (mySubDebug>0) {
    printf("%d: Record %s called subPollStatProcess(%p)\n",process_order, precord->name, (void*) precord);
    printf("status_flag: %d\n",status_flag);
    printf("status_poll_flag: %d\n",status_poll_flag);
  }    
  
  if(status_flag==status_poll_flag) {
    if (mySubDebug>0) {
      printf("same flag: no update was done\n");
    }    
    precord->val = 0;
  } else {
    if (mySubDebug>0) {
      printf("diff flag: update was done, flip the status_flag\n");
    }    
    precord->val = 1;
    // update status_flag
    if(status_flag==1) {
      status_flag = 0;
    } else {
      status_flag = 1;
    }
  }
  
  
  return 0;
}






/* Register these symbols for use by IOC code: */

epicsExportAddress(int, mySubDebug);
epicsRegisterFunction(subLVInit);
epicsRegisterFunction(subLVProcess);
epicsRegisterFunction(subTempInit);
epicsRegisterFunction(subTempProcess);
epicsRegisterFunction(subPollInit);
epicsRegisterFunction(subPollProcess);
epicsRegisterFunction(subPollStatInit);
epicsRegisterFunction(subPollStatProcess);
