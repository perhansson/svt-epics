//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//#include <string.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h> 
//#include <libxml/parser.h>
#include <time.h>
#include "client_util.h"
#include "sys/ioctl.h"

//char* temp_str = NULL;



char* getTemp(xmlDoc* doc, xmlNode* cur, int rce, int hyb) {
  xmlChar* key;
  char* key_copy = NULL;
  xmlChar str[20];
  sprintf(str,"Temp_%d_%d",rce,hyb);
  //printf("looking for %s in children to %s \n",str,cur->name);
  cur = cur->xmlChildrenNode;
  while( cur != NULL) {
    if( !xmlStrcmp(cur->name,(const xmlChar*)str) ) {
      key  = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
      //printf("key at %p\n",key);
      if( key != NULL) {
	//printf("Hybrid %s: %s\n",str,key);
	key_copy = (char*) malloc(strlen(key)*sizeof(char));
	strcpy(key_copy,key);
	xmlFree(key);
	//printf("return key_copy %s at %p\n",key_copy,key_copy);
	return key_copy;
      } else {
	//printf("no valid key\n");
      }
      xmlFree(key);
    }
    cur = cur->next;
  }
  return NULL;
}


char* retrieveTagValue(xmlDoc* doc, xmlNode* node, char* tag) {
  xmlNode* cur_node = NULL;
  char* temp_str = NULL;
  xmlChar* key;
  
  for(cur_node = node; cur_node; cur_node = cur_node->next) {
    if(temp_str!=NULL) {
      //printf("stop at cur_node %s prev %s  \n",cur_node->name,cur_node->prev->name);      
      break;
    } 
    //printf("comparing %s with tag %s\n",cur_node->name,tag);
    if( (!xmlStrcmp(cur_node->name,(const xmlChar*)tag)) ) {
      //printf("getting temp_str\n");
      key  = xmlNodeListGetString(doc,cur_node->xmlChildrenNode,1);
      if(key!=NULL) {	
	//printf("key (%d) \"%s\"\n",strlen(key),key);
	temp_str = (char*) malloc(strlen(key)*sizeof(char));
	strcpy(temp_str,key);
	xmlFree(key);
	//printf("temp_str %s at %p\n",temp_str,temp_str);
      }
    }
    
    if(temp_str!=NULL) {
      //printf("found it at name %s \n",cur_node->name);      
      return temp_str;
    }
    
    temp_str = retrieveTagValue(doc,cur_node->children,tag);
    
  }
  
  return temp_str;
}





char* retrieveTagTagValue(xmlDoc* doc, xmlNode* node, char* tag, char* tag_mother) {
  xmlNode* cur_node = NULL;
  char* temp_str = NULL;
  xmlChar* key;

  for(cur_node = node; cur_node; cur_node = cur_node->next) {
    if(temp_str!=NULL) {
      //printf("stop at cur_node %s prev %s parent %s  \n",cur_node->name,cur_node->prev->name,cur_node->parent->name);      
      break;
    } 
    //printf("comparing %s with tag %s\n",cur_node->name,tag);
    if( (!xmlStrcmp(cur_node->name,(const xmlChar*)tag)) ) {
      //if( strstr(cur_node->name,tag)!=0 ) {
      //printf("getting temp_str\n");
      xmlNode* mother_node = cur_node->parent;
      if(mother_node!=NULL) {
	//printf("comparing mother %s with tag %s\n",mother_node->name,tag_mother);
	if( (!xmlStrcmp(mother_node->name,(const xmlChar*)tag_mother)) ) {	  
	  key  = xmlNodeListGetString(doc,cur_node->xmlChildrenNode,1);
	  if(key!=NULL) {
	    //printf("key %s\n",key);
	    temp_str = (char*) malloc(strlen(key)*sizeof(char));
	    strcpy(temp_str,key);
	    xmlFree(key);
	    //printf("temp_str %s at %p\n",temp_str,temp_str);
	  }
	}
      }
      
      if(temp_str!=NULL) {
	//printf("found it at name %s \n",cur_node->name);      
	return temp_str;
      }
      
    }    
    temp_str = retrieveTagTagValue(doc,cur_node->children,tag,tag_mother);
  }    
  return temp_str;
 }



xmlNode* retrieveElement(xmlDoc* doc, xmlNode* node, char* tag) {
  xmlNode* cur_node = NULL;
  xmlNode* found_it = NULL;
  int debug = 0;
  for(cur_node = node; cur_node; cur_node = cur_node->next) {
    if(found_it!=NULL) {
      if(debug!=0) printf("stop at cur_node %s prev %s  \n",cur_node->name,cur_node->prev->name);      
      break;
    } 
    if(debug!=0) printf("Looking for %s and comparing to %s\n",tag,cur_node->name);
    if( (!xmlStrcmp(cur_node->name,(const xmlChar*)tag)) ) {
      if(debug!=0) printf("found an element of type %d\n",cur_node->type);
      if (cur_node->type == XML_ELEMENT_NODE) {	
	if(debug!=0) printf("found it\n");
	found_it = cur_node;
	break;
      }
    }
    
    if(found_it!=NULL) {
      if(debug!=0) printf("found it at name %s \n",cur_node->name);      
      return found_it;
    }
    
    found_it = retrieveElement(doc,cur_node->children,tag);
    
  }
  
  return found_it;
}



char* retrieveValue(xmlDoc* doc, xmlNode* node, char* tags) {
  int debug = 0;
  xmlChar* value_str = NULL;
  char* pch;
  char* pch_prev=NULL;
  xmlNode* cur_node;
  xmlNode* prev_node = node;
  //use a copy since it modifies the original string
  //char* tags = (char*)malloc(strlen(tags)*sizeof(char));
  //strcpy(tags,tags);
  if(debug!=0) printf("Splitting strings \"%s\" into tokens\n",tags);
  if(strlen(tags)>0) {
    pch = strtok(tags," :,.-");
    while(pch!=NULL) {
      if(debug!=0) printf("Find element %s \n",pch);
      if(pch_prev!=NULL) {
	if(debug!=0) printf("Find element %s from children of prev element at %p\n",pch,prev_node->name);
	cur_node = retrieveElement(doc,prev_node->children,pch);
      } 
      else {
	if(debug!=0) printf("Find element %s from element %s\n",pch,prev_node->name);
	cur_node = retrieveElement(doc,prev_node,pch);
      }

      // check that we found it
      if(cur_node != NULL) {
	if(debug!=0) printf("found cur_node name %s\n",cur_node->name);
      } else {
	if(debug!=0) printf("couldn't find cur_node\n");
	break;
      }
    
      pch_prev = pch;
      prev_node = cur_node;
      pch = strtok(NULL," :,.");
    }
  
    //if there is a node at the end it means we should get it's list of strings
    // if it's not the lowest level it might returns some garbage -> FIX THIS!
    if(cur_node !=NULL) {
      value_str  = xmlNodeListGetString(doc,cur_node->children,0);
    } else {
      value_str = NULL;
    }
    if(debug!=0) {
      if(value_str!=NULL) {
	printf("found value \"%s\"\n",value_str);
      } else {
	printf("couldn't find value for tags \"%s\"\n",tags);    
      }
    }
  }
  //xmlFree(value_str);  
  //if(debug!=0) printf("free tags_copy\n");
  //free(tags_copy);
  //if(debug!=0) printf("after free tags_copy\n");
  return value_str;
 
}


char* retrieveTemp(xmlDoc* doc, xmlNode* node, int rce, int hyb) {
  xmlNode* cur_node = NULL;
  char* temp_str = NULL;
  
  for(cur_node = node; cur_node; cur_node = cur_node->next) {
    if(temp_str!=NULL) {
      //printf("stop before processing name %s \n",node->name);      
      break;
    } 
    //printf("looking at %s\n",cur_node->name);
    if( !xmlStrcmp(cur_node->name,(const xmlChar*)"cntrlFpga") ) {
      //printf("getting temp_str\n");
      temp_str = getTemp(doc,cur_node,rce,hyb);
      //printf("temp_str %s at %p\n",temp_str,temp_str);
    }
    
    if(temp_str!=NULL) {
      //printf("found it at name %s \n",cur_node->name);      
      return temp_str;
    }
    
    temp_str = retrieveTemp(doc,cur_node->children,rce,hyb);
    
  }
  
  //printf("returning %s at %p from retrieveTemp\n",temp_str,temp_str);
  
  return temp_str;
 }




void read_system_xml_string3(int* socketfd, char buffer[], const int MAX) {
  const unsigned int READ_BUFFER_SIZE = 65536;
  const float wait_interval_usec = 100000;  
  char buffer_read[MAX];
  char buf[READ_BUFFER_SIZE];
  int read_i = 0;
  int read_n;
  int debug = 1;
  int n_endings = 0;
  int offset = 0;
  clock_t start_time;
  clock_t diff;
  //reset
  bzero(buffer,MAX);
  if(debug>0) printf("read xml3\n");
  start_time = clock();
  diff = (start_time - start_time);

  if(debug>0) printf("start_time at %f clocks\n",(float)start_time);
  
  while(((float)(diff/CLOCKS_PER_SEC))<5.0) { 
    if(debug>0) printf("at read_i %d clocks %f time %f\n",read_i,(float)diff,(float)((float)diff)/CLOCKS_PER_SEC);
    
    bzero(buf,READ_BUFFER_SIZE);
    
    read_n = 0;
    ioctl(*socketfd, FIONREAD, &read_n);
    
    if(debug>0) {
      printf("%d available on socket\n",read_n);
    }
    
    if(read_n>0) {      
      if(read_n>READ_BUFFER_SIZE) {
	read_n = READ_BUFFER_SIZE-1;
      }
      
      read_n = read((*socketfd),buf,read_n);
      
      if(debug>0) printf("read_n %d\n",read_n);
      
      if (read_n < 0) {
	printf("ERROR reading from socket");
	exit(1);
      }
      
      //if(debug!=0) printf("buf:\n%s\n",buf);
      
      //       if(strchr(buf,'\f')!=0) { 
      // 	if(debug>0) printf("found ending in this buf!\n"); 
      // 	n_endings++; 
      //       } 
      char *pch;
      pch = strchr(buf,'\f');
      while(pch!=NULL) {
	if(debug>0) printf("found ending at %p (array index %ld) in this buf!\n",pch,pch-buf);	
	n_endings++;
	pch = strchr(pch+1,'\f');
      }
      
      //check for overflow before copying
      if((offset+read_n) >= MAX) {
	printf("\nread offset %d exceeds available buffer length %d at  read_i %d\n",offset+read_n, MAX,read_i);
	return;
      }
      
      if(debug!=0) printf("\ncopy buf of size %d to %d offset (MAX=%d) after %p\n",read_n,offset,MAX,buffer_read);
      
      memcpy(buffer_read+offset,buf,read_n);
      //strcpy(buffer_read+offset,buf);
      
      // update offset
      //offset = offset + strlen(buf);
      offset = offset + read_n;
      //buffer_read[read_i*READ_BUFFER_SIZE+READ_BUFFER_SIZE] = NULL;
      
      if(debug!=0) {
	printf("\nlen of string is %u after read_i %d:\n%s\n",(unsigned)strlen(buffer_read),read_i,buffer_read);
	int ktmp;
	for(ktmp=read_n-100;ktmp<(read_n+100);ktmp++) printf("ktmp: '%c'\n",buffer_read[ktmp]);
      }
      
    } // read_n>0
    else {
      printf("Nothing to read from socket. Sleep a little\n");      
      usleep(wait_interval_usec);
      //sleep(1);
    } 
    
    
    
    if(n_endings>=2) {
      if(debug!=0) printf("\nfound two endings at read_i %u with total len %u\n",read_i,(unsigned)strlen(buffer_read));      
      break;
    }
    
    
    // check timing
    diff = clock()-start_time;
    //if(diff/CLOCKS_PER_SEC > 5.0) {
    //if(debug!=0) printf("\n after read_i %u nr clocks %f diff %f to read from socket\n",(unsigned)read_i,(float)diff,(float)((float)diff)/CLOCKS_PER_SEC);
    //break;    
    //}
    
    //if(read_i>10000) {
    //  if(debug!=0) printf("\ntoo many reads, error getting xml string at read_i %d\n",read_i);
    //  break;
    //}

    ++read_i;
  }
  
  if(n_endings==2) {
    
    if(debug!=0) printf("\nPick out string between endings \n%s\n",buffer_read);
    // start at first system tag
    if(debug!=0) printf("\nlen %u\n",(unsigned)strlen(buffer_read));
    char* start = strstr(buffer_read,"<system>");
    //char* start = strchr(buffer_read,'\f');
    //char* end2 = strchr(buffer_read,'\f');
    if(start!=NULL) {
      if(debug!=0) printf("\nstart at %p offset %ld\n",start,start-buffer_read);
      if(end2!=NULL) {
	if(debug!=0) printf("\n end2 at %p offset %ld\n",end2,end2-buffer_read);
      }
      char* end = strchr(start,'\f');
      if(end!=NULL) {
	if(debug!=0) printf("\nend at %p\n",end);
	memcpy(buffer,start,end-start);
	buffer[end-(start+1)+1] = NULL; // terminate it
	if(debug!=0) printf("\ncopied %ld chars to %p\n%s\n",end-(start+1),buffer,buffer);	
      } else {
	if(debug!=0) printf("\n no end found\n");
      }
    } else {
      if(debug!=0) printf("\n no start found found\n");
    }
    
  }

  return;

}


void read_system_xml_string2(int* socketfd, char buffer[], const int MAX) {
  const unsigned int READ_BUFFER_SIZE = 256;
  char buffer_read[MAX];
  char buf[READ_BUFFER_SIZE];
  int read_i = 0;
  int read_n;
  int debug = 1;
  int n_endings = 0;
  int offset = 0;
  //reset
  bzero(buffer,MAX);
  if(debug>0) printf("read xml2\n");

  while(read_i<10000) { // some large number to avoid problems
    if(debug>0) printf("at read_i %d\n",read_i);

    //strcpy(buffer_read_overlap,buffer_read);//,READ_BUFFER_SIZE); 
    bzero(buf,READ_BUFFER_SIZE);
    
    if(debug>0) {
      printf("at read_i %d\n",read_i);
      int len = 0;
      ioctl(*socketfd, FIONREAD, &len);
      printf("len %d available\n",len);
      // if (len > 0) {
      //len = read(sock, buffer, len);
      //}
    }
    
    read_n = read((*socketfd),buf,(READ_BUFFER_SIZE-1));
    if(debug>0) printf("read_n %d\n",read_n);

    if (read_n < 0) 
      error("ERROR reading from socket");
    //printf("buffer_read_overlap after clearing the old buffer:\n%s\nbuffer below:\n",buffer_read_overlap);
    if(debug!=0) printf("%s\n",buf);


    //if(strstr(buf,"\f")!=0) {
    if(strchr(buf,'\f')!=0) {
      if(debug>0) printf("found ending in this buf!\n");
      n_endings++;
    }

    //check for overflow
    if((offset+READ_BUFFER_SIZE) >= MAX) {
      printf("\nread offset %d exceeds available buffer length %d at  read_i %d\n",offset+READ_BUFFER_SIZE, MAX,read_i);
      return;
    }

    if(debug!=0) printf("\ncopy buf to %d offset (MAX=%d) after %p\n",offset,MAX,buffer_read);
    strcpy(buffer_read+offset,buf);
    offset = offset + strlen(buf);
    //memcpy(buffer_read+read_i*(READ_BUFFER_SIZE),buf,READ_BUFFER_SIZE-1);
    //buffer_read[read_i*READ_BUFFER_SIZE+READ_BUFFER_SIZE] = NULL;

    if(debug!=0) {
      printf("\nlen is %ld after read_i %d\n%s\n",strlen(buffer_read),read_i,buffer_read);
      //int k;
      //for(k=253;k<260;++k) printf("%ld \'%c\'\n",k,buffer_read[k]);
    }
    
    if(n_endings>=2) {
      if(debug!=0) printf("\nfound two endings at read_i %d  with total len %ld\n",read_i,strlen(buffer_read));      
      break;
    }

    if(read_i>10000) {
      if(debug!=0) printf("\ntoo many reads, error getting xml string at read_i %d\n",read_i);
      break;
    }

    ++read_i;
  }

  if(n_endings==2) {
    
    if(debug!=0) printf("\nPick out string between endings \n%s\n",buffer_read);
    //int k=0;
    //for(k=read_i*READ_BUFFER_SIZE;k>(read_i-2)*READ_BUFFER_SIZE;--k) printf("%d \'%c\'\n",k,buffer_read[k]);
    if(debug!=0) printf("\ndone\n");
    if(debug!=0) printf("\nlen %ld\n",strlen(buffer_read));
    //char* start = strstr(buffer_read,"\f");
    char* start = strchr(buffer_read,'\f');
    char* end2 = strrchr(buffer_read,'\f');
    if(start!=NULL) {
      if(debug!=0) printf("\nstart at %p offset %ld\n",start,start-buffer_read);
      if(end2!=NULL) {
	if(debug!=0) printf("\n end2 at %p offset %ld\n",end2,end2-buffer_read);
      }
      //char* end = strstr(start+2,"\f");
      char* end = strchr(start+1,'\f');
      if(end!=NULL) {
	if(debug!=0) printf("\nend at %p\n",end);
	memcpy(buffer,start+1,end-(start+1));
	buffer[end-(start+1)+1] = NULL; // terminate it
	if(debug!=0) printf("\ncopied %ld chars to %p\n%s\n",end-(start+1),buffer,buffer);	
      } else {
	if(debug!=0) printf("\n no end found\n");
      }
    } else {
      if(debug!=0) printf("\n no start found found\n");
    }
    
  }

  return;

}



char* read_system_xml_string(int* socketfd) {
  int debug = 1;
  int read_ii;
  int read_i;
  int read_c;
  int read_s;
  int read_n;
  int read_cur_buff;
  int read_offset;
  const unsigned int READ_BUFFER_SIZE = 256;
  const unsigned int READ_MAX_BUFFERS = 1000;
  char buffer_read[READ_BUFFER_SIZE];
  char buffer_read_overlap[READ_BUFFER_SIZE*2];
  char* buffers_read[READ_MAX_BUFFERS];
  
  read_i = 0;
  read_c = 0;
  read_ii = 0;
  read_s = 0;
  read_n = 0;
  read_cur_buff = 0;
  read_offset = 0;
  
  if(debug!=0) printf("reset buffers\n");
  for(read_i=0;read_i<READ_MAX_BUFFERS;++read_i) {
    //    if(buffers_read[read_i] != NULL) {
    //if(debug!=0) printf("reset buffers\n");
    //free(buffers_read[read_i]);
    //}
    buffers_read[read_i] = NULL;
  }
  
  while(read_ii<10000) { // some large number to avoid problems
    ++read_ii;
    //strcpy(buffer_read_overlap,buffer_read);//,READ_BUFFER_SIZE); 
    bzero(buffer_read,READ_BUFFER_SIZE);
    read_n = read((*socketfd),buffer_read,(READ_BUFFER_SIZE-1));
    if (read_n < 0) 
      error("ERROR reading from socket");
    //printf("buffer_read_overlap after clearing the old buffer:\n%s\nbuffer below:\n",buffer_read_overlap);
    if(debug!=0) printf("\nRead %d\n%s\n",read_ii,buffer_read);
    read_cur_buff = read_ii-1; 
    buffers_read[read_cur_buff] = (char*) malloc(sizeof(char)*strlen(buffer_read)); //READ_BUFFER_SIZE-1);
    if(debug!=0) printf("malloc %d of length %ld at %p\n",read_cur_buff,sizeof(char)*strlen(buffer_read),buffers_read[read_cur_buff]);
    strcpy(buffers_read[read_cur_buff],buffer_read);
    
/*     if(strstr(buffer_read,"<config>")!=0) { */
/*       printf("\n\nFound config tag!\n"); */
/*       //break; */
/*       //exit(0); */
/*     } */
/*     if(strstr(buffer_read,"</config>")!=0) { */
/*       printf("\n\nFound /config tag!\n"); */
/*       //break; */
/*       //exit(0); */
/*     } */
    
    
    
    // first find start and then end to make sure to see the whole xml string
    if(read_s==0) {
      if(strstr(buffer_read,"<system>")!=0) {
	read_s=1;
	if(debug!=0) printf("\n found start of xml string in buffer (read_ii=%d)\n",read_ii);
	bzero(buffer_read_overlap,2*READ_BUFFER_SIZE);
	continue;
      }
      // check if we cut the element by adding prev buffer
      strcpy(buffer_read_overlap+READ_BUFFER_SIZE-1,buffer_read);//avoid last char that ends string
      if(strstr(buffer_read_overlap,"<system>")!=0) {
	if(debug!=0) printf("\n found start of xml string in buffer_read_overlap (read_ii=%d)\n",read_ii);
	continue;
      }
    }
    else {
      if(strstr(buffer_read,"</system>")!=0) {
	if(debug!=0) printf("\n found end of xml string in buffer (read_ii=%d)\n",read_ii);
	break;
      } 
      // check if we cut the element by adding prev buffer
      strcpy(buffer_read_overlap+READ_BUFFER_SIZE-1,buffer_read);//avoid last char that ends string
      //printf("\nbuffer_read_overlap (strlen=%d) after adding prev and new buffer to dest:\n%s\n",strlen(buffer_read_overlap),buffer_read_overlap);
      if(strstr(buffer_read_overlap,"</system>")!=0) {
	if(debug!=0) printf("\n found end of xml string in buffer_read_overlap (read_ii=%d)\n",read_ii);
	break;
      }
      if(debug!=0) printf("\n found NO end of xml string in buffer (read_ii=%d)\n",read_ii);	  
      strcpy(buffer_read_overlap,buffer_read);//,READ_BUFFER_SIZE); 
    }
  }

  if(read_ii==(10000-1)) {
    printf("\n ERROR couldn't find the complete xml string\n");
    return NULL;
  }
  
  char* xml_string = (char*)malloc(sizeof(char)*(read_cur_buff+1)*READ_BUFFER_SIZE);
  read_offset = 0;
  if(debug!=0) {
    printf("\nallocating xml_string size %ld\n",sizeof(char)*(read_cur_buff+1)*READ_BUFFER_SIZE);
    printf("read_cur_buff = %d\n",read_cur_buff);
  }
  read_i=0;
  for(read_i=0;read_i<READ_MAX_BUFFERS;++read_i) {	
    if(buffers_read[read_i]!=NULL) {	  
      strcpy(xml_string+read_offset,buffers_read[read_i]);
      read_offset += strlen(buffers_read[read_i]);
      if(debug!=0) printf("copying i%d to %p with offset %d and strlen %ld:\n",read_i,(xml_string+read_offset),read_offset,strlen(buffers_read[read_i]));
      //printf("%s",buffers_read[i]);
      //printf("\nfreeing %d at %p:\n%s\n",i,buffers_read[i],buffers_read[i]);
      free(buffers_read[read_i]);
      
    }
  }
  if(debug!=0) {
    printf("after copying to xml_string offset is %d\n",read_offset);
    printf("\n\nxml_string is %ld chars\n",strlen(xml_string));
    //printf("%s\n",xml_string);
  }
  // remove stuff outside the xml tags
  // monkey around with null character to terminate strings correctly
  char* p_start_system = strstr(xml_string,"<system>");
  char* p_end_system = strstr(xml_string,"</system>");
  int len = p_end_system+sizeof(char)*strlen("</system>")-p_start_system;
  if(debug!=0) printf("found start %p and end %p which is of length %d (strlen(</system>)=%ld)\n",p_start_system,p_end_system,len,strlen("</system>"));
  char* xml_string_pruned =(char*) malloc(sizeof(char)*len+1);
  //char* cpy_dest = memcpy(xml_string_pruned, p_start_system,sizeof(char)*len);
  // add termination to string
  xml_string_pruned[sizeof(char)*len] = '\0';
  
  //printf("free xml_string\n");
  free(xml_string);
  //printf("after free xml_string\n");

  if(debug!=0) printf("xml_string_pruned at %p below\n%s\n",xml_string_pruned,xml_string_pruned);

  return xml_string_pruned;

 }








char* copy_substr(char* buf_src, int* nread, const char* str_start, const char* str_end) {

  char* p_start = strstr(buf_src,str_start);
  
  if(p_start == NULL) {
    printf("start string \"%s\" not in string\n",str_start);
    return 0;
  }
  
  char* p_end = strstr(buf_src,str_end);
  
  if(p_end == NULL) {
    printf("end string \"%s\" not in string\n",str_end);
    char str_end2[sizeof(str_end)+2];
    sprintf(str_end2,"%s\n",str_end);
    p_end = strstr(buf_src,str_end2);
    if(p_end == NULL) {
      printf("end string \"%s\" not in string\n",str_end2);
      return 0;
    }
  }

  *nread = p_end-p_start;

  char* buf_dest = (char*) malloc(*nread);

  strncpy(buf_dest, p_start, *nread);
  return buf_dest;
}



char* copy_to_array(int n, int len, char* xml_buffer[]) {
  const unsigned int BUFFER_SIZE = n*len;
  char* buffer = (char*) malloc(BUFFER_SIZE);
  int i;
  for(i=0;i<n;i++) {
    int offset = i*len;
    memcpy(buffer+offset, xml_buffer[i], len);
  }
  return buffer;
}


char* read_xml(int *sockfd, int* len) {

  // loop over input until I see a full cycle of the system
  // then put into a character array

  const unsigned int BUFFER_SIZE = 1024;
  const unsigned int XML_BUFFERS = 20;
  xmlChar buffer[BUFFER_SIZE];
  xmlChar* xml_buffer[XML_BUFFERS];
  int j;
  int n;
  unsigned int i = 0;
  unsigned int open_system = 0;
  int buffer_offset= 0;
  

  for(j=0;j<XML_BUFFERS;++j) {
    xml_buffer[j] = NULL;
  }
  
  
  while(i<100000) { // stupid
    bzero(buffer,BUFFER_SIZE);
    n = read(*sockfd,buffer,BUFFER_SIZE-1);
    if (n < 0) {
      error("ERROR reading from socket");
    }
    
    if(open_system == 0) {
      if(strstr(buffer,"<system>") != NULL) {
	open_system = 1;
	buffer_offset= 0;
	xml_buffer[buffer_offset] = (char*) malloc(BUFFER_SIZE);
	memcpy(xml_buffer[buffer_offset],buffer,BUFFER_SIZE);	    
      }
      continue;
    }
    
    if(open_system == 1) {	  
      buffer_offset++;
      xml_buffer[buffer_offset] = (char*) malloc(BUFFER_SIZE);
      memcpy(xml_buffer[buffer_offset],buffer,BUFFER_SIZE);
      if(strstr(buffer,"</system>") != NULL) {
	open_system = 0;
	break;
      }
    }
    ++i;
  }

  // Now take this 2D array and put into a long 1D array
  // this feels stupid.
  char* buffer_long = copy_to_array(buffer_offset+1,BUFFER_SIZE,xml_buffer);
  
  // I think I should free up some of the buffers used in the 2D array?  
  for(j=0;j<XML_BUFFERS;++j) {
    if(xml_buffer[j] != NULL) {
      free(xml_buffer[j]);
    }
  }

  // set the length as a returned parameter
  *len = (buffer_offset+1)*BUFFER_SIZE;

  // return the long array
  return buffer_long;
  
  
  
 }




 void print_element_names(xmlNode * a_node)
{
  //printf("print_element_names starting with name %s\n", a_node->name);
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
	printf("node type: Element, name: %s\n, content: \%s\n", cur_node->name, cur_node->content);
      }
      
      print_element_names(cur_node->children);
    }
}




char* get_xml_substr(char* xml_buf, int n, int* n_read, const char* p_start_str, const char* p_end_str) {
  
  // ok, here I'm not sure why strstr doesn't work?
  // so this is a stupid way of removing evereyting except within some 
  // xml tags
  
  //char* p_start_str = "<system>";
  char* p_start = NULL;
  //char* p_end_str = "</system>";
  char* p_end = NULL;
  int i,j;
  for(i=0;i<n;++i) {
    j = memcmp(xml_buf+i,p_start_str,strlen(p_start_str));
    if(j==0 && p_start==NULL) {
      //printf("found %s at %d\n",p_start_str,i);
      p_start = xml_buf+i;
    }
    j = memcmp(xml_buf+i,p_end_str,strlen(p_end_str));
    if(j==0 && p_end==NULL) {
      //printf("found %s at %d\n",p_end_str,i);
      p_end = xml_buf+i + strlen(p_end_str);
    }
  }
  
  if(p_start==NULL || p_end==NULL) {
    *n_read = 0;
    return NULL;
  }

  // then copy out that range
  
  char* xml_buf_clean = (char*) malloc(p_end-p_start);
  xml_buf_clean = memcpy(xml_buf_clean,p_start,p_end-p_start);
  *n_read = (p_end-p_start);
  return xml_buf_clean;
 }



void findTemp(char* xml_buf, int n) {

  // xml stuff
  xmlDoc         *document;
  xmlNode        *root, *first_child, *node;
  char           *filename;
  
  // ok, here I'm not sure why strstr doesn't work?
  // so this is a stupid way of removing evereyting except within some 
  // xml tags
  
  char* xml_buf_clean;
  int n_clean;
  xml_buf_clean = get_xml_substr(xml_buf,n,&n_clean,"<system>","</system>");
  printf("read %d of %d\n",n_clean,n);
  // I should turn this into a DOM
  // but I get a bunch of annoying characters that the parser compains about!?
  // be stupid here
  
  int rce;
  int hyb;
  //char* t_p_start = NULL;
  //char* t_p_end = NULL;
  char* temp;
  char t_str_start[256];
  char t_str_end[256];
  int n_temp;
  for(rce=0;rce<3;++rce) {
    for(hyb=0;hyb<4;++hyb) {
      sprintf(t_str_start,"<Temp_%d_%d>",rce,hyb);
      sprintf(t_str_end,"</Temp_%d_%d>",rce,hyb);
      //printf("%s -> %s\n",t_str_start,t_str_end);
      temp = get_xml_substr(xml_buf_clean,n_clean,&n_temp,t_str_start,t_str_end);
      if(temp == NULL) {
	printf(" couldn't find %s\n",temp);
      } else {
	printf("found %s\n",temp);
      }
      free(temp);
      
    }
    
  }
  
  //free(xml_buf_clean);

  /*

  FILE * pFile;
  pFile = fopen ("myfile.xml", "wb");
  const char weird = 0x0;//"\@";
  i=0;
  while(i<(p_end-p_start)) {
    if(strncmp(xml_buf_clean+i,"FilterFile",10)==0) {
      printf("found FilterFile at %d\n",i);
    }

    if(*(xml_buf_clean+i) == 0x0) {
      //if(strncmp(xml_buf_clean+i,weird,sizeof(weird))==0) {
      printf("\nfound weird char %s at %d\n",weird,i);
      i++;
    } else {
      fprintf(pFile,"%c",xml_buf_clean[i]);
      printf("%c",xml_buf_clean[i]);
    }
    i++;
  }
  fprintf(pFile,"\n","");
  //printf("%d\n",sizeof(xml_buf_clean));
  //fwrite (xml_buf_clean , sizeof(char), p_end-p_start, pFile);
  fclose (pFile);
  //return 0;
  //FILE* pFile;
  //pFile = tmpfile();
  
  //for(i=0;i<
  
  printf("\n\n");
  
  //for(i=0;i<(p_end-p_start);i++) {
  //  printf("%c",xml_buf_clean[i]);    
  ///}
  //printf("\n\n");
  
  
  //filename = "/u1/software/daq/config/devboard.xml";     
  filename = "myfile.xml";
  document = xmlReadFile(filename, NULL, 0);
  
  //document = xmlReadFile(xml_buf, NULL, 0);
  
  //char* arr = "<system>hej</system>";
  //document = xmlReadMemory(xml_buf_clean, p_end-p_start, "test.xml", NULL, 0);
  //document = xmlReadMemory(arr, 20, "test.xml", NULL, 0);
  root = xmlDocGetRootElement(document);
  fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
  
  print_element_names(root);
  */
  

}

 
