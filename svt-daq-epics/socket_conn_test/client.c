#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <libxml/parser.h>
#include <client_util.h>

//extern char* temp_str;

const unsigned int BUFFER_SIZE = 256;
const unsigned int MAX_BUFFERS = 1000;
const unsigned int XML_STRING_MAX = 65536*10;





void error(const char *msg)
{
  perror(msg);
    exit(0);
}


int open_socket(char* hostname, int portno) {
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serv_addr;
  struct hostent *server;
  int socketfd;
  //char overlap_buffer[BUFFER_SIZE*2];  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) 
    error("ERROR opening socket");
  server = gethostbyname(hostname);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, 
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(socketfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
    error("ERROR connecting");
  return socketfd;

}

int close_socket(int socketfd) {
  return close(socketfd);
}






int main(int argc, char *argv[])
{
  int n,i,ii,s,cur_buff;
  int sockfd;
  char* hostname;
  int portno;
  int opt;
  char buffer[BUFFER_SIZE];
  char overlap_buffer[BUFFER_SIZE*2];
  char* buffers[MAX_BUFFERS];
  char* xml_string_pruned;
  char xml_string_pruned2[XML_STRING_MAX];
  char tag[BUFFER_SIZE];
  xml_string_pruned = NULL;

  for(i=0;i<MAX_BUFFERS;++i)
    buffers[i] = NULL;


  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  hostname = argv[1];
  portno = atoi(argv[2]);
  opt = -1;
  
  i=0;
  while(i!=-1) {

    printf("\nPlease enter the message: ");
    bzero(buffer,BUFFER_SIZE);
    fgets(buffer,(BUFFER_SIZE-1),stdin);

    if(strstr(buffer,"exit")!=0) {
      printf("Exiting\n");
      exit(0);
    }
    else if(strstr(buffer,"dom")!=0) {
      bzero(buffer,BUFFER_SIZE);
      int npr = sprintf(buffer,"%c",'\0');
      if(npr<=0) {
	printf("Error creating cmd (%d)\n",npr);
	exit(0);
      }      
      opt=1;
    }
    else if(strstr(buffer,"config dom")!=0) {
      opt=1;
      bzero(buffer,BUFFER_SIZE);
      int npr = sprintf(buffer,"%s","<system><command>ReadConfig</command></system>\f");
      if(npr<=0) {
	printf("Error creating cmd (%d)\n",npr);
	exit(0);
      }
    }
    else if(strstr(buffer,"Hz")!=0) {
      opt=1;
      char * rate_str = (char*)malloc(sizeof(char)*strlen(buffer)-1);
      memcpy(rate_str,buffer,strlen(buffer)-1);
      bzero(buffer,BUFFER_SIZE);
      int npr = sprintf(buffer,"<system><config><RunRate>%s</RunRate></config></system>\f",rate_str);
      if(npr<=0) {
	printf("Error creating cmd (%d)\n",npr);
	exit(0);
      }
      free(rate_str);
    }
    else if(strstr(buffer,"CalDelay:")!=0) {
      opt=1;
      int nstr = strlen("CalDelay:");
      char buf[256];
      char* start = buffer + nstr;
      strcpy(buf,start);
      buf[strlen(buf)-1] = NULL;
      int npr = sprintf(buffer,"<system><config><cntrlFpga><CalDelay>%s</CalDelay></cntrlFpga></config></system>\f",buf);
      if(npr<=0) {
	printf("Error creating cmd (%d)\n",npr);
	exit(0);
      }
    }
    else if(strstr(buffer,"DebugEnable:")!=0) {
      opt=1;
      int nstr = strlen("DebugEnable:");
      char buf[256];//  = (char*)malloc(sizeof(char)*strlen(buffer));
      bzero(buf,256);
      char* start = buffer + nstr;
      strcpy(buf,start);//,strlen(buffer)-nstr-1);
      buf[strlen(buf)-1] = NULL;
      int k=0;
      for(k=0;k<256;++k) printf("buf len %d\nk=%ld \'%c\'\n",strlen(buf),k,buf[k]);
      int npr = sprintf(buffer,"<system><config><DebugEnable>%s</DebugEnable></config></system>\f",buf);
      if(npr<=0) {
	printf("Error creating cmd (%d)\n",npr);
	exit(0);
      }
    }
    else if(strstr(buffer,"temps")!=0) {
      opt=2;
    }
    else if(strstr(buffer,"tag:")!=0) {
      opt=3;
      strcpy(tag,buffer+4);
      int length = strlen(tag);
      if(tag[length-1]=='\n') {
	tag[length-1] = '\0';
      }
      //length = strlen(tag);
      //printf("tag to find: %s (strlen %d)\n",tag,length);
      bzero(buffer,BUFFER_SIZE);
    }
    else {
      opt = 1;
      // do nothing
    }
    sockfd = open_socket(hostname,portno);
    printf("Executing:%s\n",buffer);
    printf("len %d\n",strlen(buffer));
    int k;
    for(k=0;k<BUFFER_SIZE;++k) printf("k=%d \'%c\'\n",k,buffer[k]);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
      error("ERROR writing to socket");
    
    // dangerous to allocate on heap in function?
    //xml_string_pruned = (char*) read_system_xml_string(&sockfd);
    //bzero(xml_string_pruned2,BUFFER_SIZE*300);
    //read_system_xml_string2(&sockfd,xml_string_pruned2, BUFFER_SIZE*300);
    bzero(xml_string_pruned2,XML_STRING_MAX);
    read_system_xml_string3(&sockfd,xml_string_pruned2, XML_STRING_MAX);
    
    //if(xml_string_pruned == NULL) {
    if(strlen(xml_string_pruned2)==0) {
      printf("Error reading system xml string\n");
      close_socket(sockfd);
      exit(0);
    }
    
    
    //printf("xml_string_pruned:\n%s\n",xml_string_pruned2);


    // build a xml dom
    xmlDoc         *document;
    xmlNode        *root;//, *first_child, *node;
    xmlChar        *temp_val; 
    document = xmlReadMemory(xml_string_pruned2, strlen(xml_string_pruned2), "noname.xml", NULL, 0);
    root = xmlDocGetRootElement(document);


    if(opt==1) {
      //printf("Dump DOM root %p document %p\n",root, document);
      printf("xml string:\n%s\n",xml_string_pruned2);
      
      // printf("Dump DOM tree root %s of type %s:\n", root->name, root->type);      
      //print_element_names(root);
      //printf("dsdd2 after Executing:%s\n",buffer);
    }
    else if(opt==2) {
      printf("Print hybrid temperatures:\n");
      i=0;
      ii=0;
      for(i=0;i<3;++i) {
	for(ii=0;ii<4;++ii) {
	  temp_val = (char*)retrieveTemp(document,root,i,ii);
	  if(temp_val != NULL) {
	    printf("RCE %ld hybrid %d: %s (%p)\n",i,ii,temp_val,temp_val);
	    free(temp_val);
	  } 
	}
      }
    }
    else if(opt==3) {
      char * tag_bak = (char*)malloc(sizeof(char)*strlen(tag));
      strcpy(tag_bak,tag);
      temp_val = retrieveValue(document,root,tag);
      //temp_val = retrieveTagValue(document,root,tag);
	if(temp_val != NULL) {
	  printf("<%s>: %s (%p)\n",tag_bak,temp_val);
	} 
	xmlFree(temp_val);
	free(tag_bak);
    } else {
      printf("Do nothing option\n");
    }
    
    // clean up xml
    xmlFreeDoc(document);
    xmlCleanupParser();
    
    
    // clean up strings
    free(xml_string_pruned);
    
    // close socket
    int close_socket_int = close_socket(sockfd);
  }
  int close_socket_final = close_socket(sockfd);
  return 0;
}











    /*
    ii=0;
    s=0;
    cur_buff=0;
    for(i=0;i<MAX_BUFFERS;++i)
      buffers[i] = NULL;
    
    while(ii>-1) { // some large number to avoid problems
      ++ii;
      //strcpy(overlap_buffer,buffer);//,BUFFER_SIZE); 
      bzero(buffer,BUFFER_SIZE);
      n = read(sockfd,buffer,(BUFFER_SIZE-1));
      if (n < 0) 
	error("ERROR reading from socket");
      //printf("overlap_buffer after clearing the old buffer:\n%s\nbuffer below:\n",overlap_buffer);
      //printf("%s",buffer);
      cur_buff = ii-1; 
      buffers[cur_buff] = (char*) malloc(sizeof(char)*strlen(buffer)); //BUFFER_SIZE-1);
      //printf("malloc %d of length %d at %p\n",cur_buff,sizeof(char)*strlen(buffer),buffers[cur_buff]);
      strcpy(buffers[cur_buff],buffer);
      
      if(c==1) {
	// reading config, check that I get the config tag
	if(strstr(buffer,"<config>")!=0) {
	  printf("\n\nFound config tag!\n");
	  //break;
	  //exit(0);
	}
	if(strstr(buffer,"</config>")!=0) {
	  printf("\n\nFound /config tag!\n");
	  //break;
	  //exit(0);
	}
      }
      
      
      // check if I want to continue polling forever
      //if(c==1) {
      //  continue; 
      //}	
      // first find start and then end to make sure to see the whole xml string
      if(s==0) {
	if(strstr(buffer,"<system>")!=0) {
	  s=1;
	  printf("\n found start of xml string in buffer (ii=%d)\n",ii);
	  bzero(overlap_buffer,2*BUFFER_SIZE);
	  continue;
	}
	// check if we cut the element by adding prev buffer
	strcpy(overlap_buffer+BUFFER_SIZE-1,buffer);//avoid last char that ends string
	if(strstr(overlap_buffer,"<system>")!=0) {
	  printf("\n found start of xml string in overlap_buffer (ii=%d)\n",ii);
	  continue;
	}
      }
      else {
	if(strstr(buffer,"</system>")!=0) {
	  printf("\n found end of xml string in buffer (ii=%d)\n",ii);
	  break;
	} 
	// check if we cut the element by adding prev buffer
	strcpy(overlap_buffer+BUFFER_SIZE-1,buffer);//avoid last char that ends string
	//printf("\noverlap_buffer (strlen=%d) after adding prev and new buffer to dest:\n%s\n",strlen(overlap_buffer),overlap_buffer);
	if(strstr(overlap_buffer,"</system>")!=0) {
	  printf("\n found end of xml string in overlap_buffer (ii=%d)\n",ii);
	  break;
	}
	//printf("\n found NO end of xml string in buffer (ii=%d)\n",ii);	  
	strcpy(overlap_buffer,buffer);//,BUFFER_SIZE); 
      }
    }
    
    char* xml_string = (char*)malloc(sizeof(char)*cur_buff*BUFFER_SIZE);
    int offset = 0;
    //printf("\nallocating xml_string size %d\n",sizeof(char)*cur_buff*BUFFER_SIZE);
    //printf("cur_buf = %d\n",cur_buff);
    for(i=0;i<MAX_BUFFERS;++i) {	
      if(buffers[i]!=NULL) {	  
	strcpy(xml_string+offset,buffers[i]);
	offset += strlen(buffers[i]);
	//printf("copying i%d to %p with offset %d and strlen %d:\n",i,(xml_string+offset),offset,strlen(buffers[i]));
	//printf("%s",buffers[i]);
      }
      //printf("\nfreeing %d at %p:\n%s\n",i,buffers[i],buffers[i]);
      free(buffers[i]);
    }
    //printf("after copying to xml_string offset is %d\n",offset);
    printf("\n\nxml_string is %d chars\n",strlen(xml_string));
    //printf("%s\n",xml_string);
    
    // remove stuff outside the xml tags
    // monkey around with null character to terminate strings correctly
    char* p_start_system = strstr(xml_string,"<system>");
    char* p_end_system = strstr(xml_string,"</system>");
    int len = p_end_system+sizeof(char)*strlen("</system>")-p_start_system;
    //printf("found start %p and end %p which is of length %d (strlen(</system>)=%d)\n",p_start_system,p_end_system,len,strlen("</system>"));
    char* xml_string_pruned =(char*) malloc(sizeof(char)*len+1);
    char* cpy_dest = memcpy(xml_string_pruned, p_start_system,sizeof(char)*len);
    // add termination to string
    xml_string_pruned[sizeof(char)*len] = '\0';
    
    */
    /*
      printf("look at last piece of the xml_string\n");
      i=0;
      int index;
      while(i>-1) {
      index = (p_end_system-p_start_system)+i;
      //xml_string[index];
      printf("xml_string (index %d): \"%c\"\n",index,xml_string[index]);
      if(index>=sizeof(char)*cur_buff*BUFFER_SIZE) break;
      i++;
      }
      printf("after pruning xml_string (strlen %d) \n%s\n",strlen(xml_string_pruned),xml_string_pruned);
      printf("look at last piece of the xml_string_pruned\n");
      p_start_system = strstr(xml_string,"<system>");
      p_end_system = strstr(xml_string,"</system>");
      i=0;
      while(i>-1) {
      index = (p_end_system-p_start_system)+i;
      if(index>=sizeof(char)*len) break;
      //xml_string[index];
      printf("xml_string_pruned (index %d): \"%c\"\n",index,xml_string_pruned[index]);
      i++;
      }
      printf("look at first piece of the xml_string_pruned\n");
      i=0;
      while(i<50) {
      printf("xml_string_pruned (index %d): \"%c\"\n",i,xml_string_pruned[i]);
      ++i;
      }
    */
    
