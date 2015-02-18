#ifndef COMMONXML_H
#define COMMONXML_H

#include <sys/ioctl.h>
#include <libxml/parser.h>

#include "commonConstants.h"



int findSystemStr(char* buf, const int MAX, char** start) {
    if(DEBUG>1) printf("[ findSystemStr ]: finding system string from %p and %d chars len with start at %p\n",buf,MAX,*start);
    char* b;
    char* e;
    char* s;
    char* p_ending;
    char* status_tag_s;
    char* status_tag_e;

    b = buf;
    while(1!=0) {    
        s = strstr(b,"<system>");  
        p_ending = strchr(b,'\f');  

        if(s!=NULL) {
            if(p_ending!=NULL) {      
                //check that status exists
                if(DEBUG>1) printf("[ findSystemStr ]: found system at len %d and ending and len %d\n",s-b,p_ending-b);
                status_tag_s = strstr(b,"<status>");
                status_tag_e = strstr(b,"</status>");
                // look at this system string  if status tags are found inside the ending
                if(status_tag_s!=NULL && status_tag_e!=NULL) {
                    if(DEBUG>1) printf("[ findSystemStr ]: found status tags at len %d and %d\n",status_tag_s-b, status_tag_e-b);
                    if((status_tag_s-b)<(p_ending-b) && (status_tag_e-b)<(p_ending-b)) {
                        if(DEBUG>1) printf("[ findSystemStr ]: found status tags inside ending\n");
                        // return this
                        *start = s;
                        e = p_ending-1;
                        if(DEBUG>1) {
                            printf("[ findSystemStr ]: found s at %p and e at %p and *start at %p with len %d \n",s,e,*start,e-s);
                            printf("[ findSystemStr ]: last characters are:\n");
                            int ii;
                            for(ii=-50;ii<=0;++ii) {
                                char ee = *(e+ii);
                                printf("[ findSystemStr ]: %d: '%c'\n",ii,ee);
                            }
                        }
                        return (int)(e-s);
                    }
                } 
                else {
                    // go to next, if there is one
                    b = p_ending+1;
                    if((b-buf)>MAX) return -1;
                }
            } else {
                if(DEBUG>1) printf("[ findSystemStr ]: p_ending couldn't be found\n"); 
                // nothing in this string to work with
                break;
            }
        } else {
            if(DEBUG>1) printf("[ findSystemStr ]: <system> couldn't be found\n"); 
            // nothing in this string to work with
            break;      
        }
    }


    return -1;
}



void pollDpmXmlString(int socketfd, char** xml_string_out, int* len_out) {
   char* buf = NULL;
   char* buf_loop = NULL;
   int buf_len;
   int read_i;
   int read_n;
   int nempty;
   int counter;
   int n_endings;
   time_t timer;
   time_t cur_time;
   struct tm *lt;
   int dt;
   char *pch;
   
   
   if(DEBUG>0) printf("[ pollDpmXmlString ]:  from socket %d \n", socketfd);
      
   time(&timer);
   
   if(DEBUG>0) {
     lt = localtime(&timer);
     printf("[ pollDpmXmlString ]: start_time at %s\n",asctime(lt));
   }
   
   nempty=0;
   counter=0;
   read_i=0;
   buf_len=0;
   n_endings=0;
   dt=0;
   
   while(dt<3) { 
      
      time(&cur_time);
      dt = difftime(cur_time,timer);
      
      if(DEBUG>1) 
	printf("[ pollDpmXmlString ]: Try to read from socket (nempty %d read_i %d time %ds)\n",nempty,read_i,dt);
      
      read_n = 0;
      ioctl(socketfd, FIONREAD, &read_n);
      
      if(DEBUG>1) {
         printf("[ pollDpmXmlString ]: %d chars available on socket\n",read_n);
      }
      
      if(read_n>0) {      
         
         // allocate memory needed
         if(DEBUG>1) printf("[ pollDpmXmlString ]: Allocate %d array\n",read_n);      
         
         // check that the buffer used is not weird
         if(buf_loop!=NULL) {
            printf("[ pollDpmXmlString ]: [ ERROR ]: buf_loop is not null!\n");
            exit(1);
         }
         
         // allocate space to hold the input
         buf_loop = (char*) calloc(read_n+1,sizeof(char));
         
         if(DEBUG>1) printf("[ pollDpmXmlString ]: Allocated buf_loop array at %p strlen %d with %d length \n",buf_loop,strlen(buf_loop),(int)sizeof(char)*(read_n+1));      
         
         // Read from socket
         read_n = read(socketfd,buf_loop,read_n);
         
         if(DEBUG>1) printf("[ pollDpmXmlString ]: Read %d chars from socket\n",read_n);
         
         if(DEBUG>2) printf("[ pollDpmXmlString ]: buf_loop strlen is %d\n",strlen(buf_loop));
         
         if (read_n < 0) {
            printf("[ pollDpmXmlString ]: [ ERROR ]: read %d from socket\n",read_n);
            exit(1);
         }
         
         //fix c-string terminations, cases where there is a termination in the middle..
         int k;
         for(k=0;k<read_n;++k) {
            //printf("[ pollDpmXmlString ]: '%c'\n",buf_loop[k]);
            if(buf_loop[k]=='\0') {
               if(DEBUG>2) printf("[ pollDpmXmlString ]: fix termination at idx %d in this buf_loop\n",k);
               buf_loop[k]=' ';
            }
         }
         
         if(DEBUG>2) printf("[ pollDpmXmlString ]: Fixed buf_loop strlen %d:\n%s\n",strlen(buf_loop),buf_loop);
         
         
         // search for xml endings in this buffer
         pch = strchr(buf_loop,'\f'); 
         while(pch!=NULL) { 
            if(DEBUG>1) printf("[ pollDpmXmlString ]: found ending at %p (array index %d) in this buf!\n",pch,pch-buf_loop); 
            n_endings++; 
            pch = strchr(pch+1,'\f'); 
         } 
         
         
         // copy to other buffer while looping            
         if(DEBUG>2) printf("[ pollDpmXmlString ]: Copy %d to other buffer (at %p before realloc) \n",read_n,buf);      
         
         // reallocate more memory
         buf = (char*) realloc(buf,sizeof(char)*(buf_len+read_n));
         if(buf==NULL) {
            printf("[ pollDpmXmlString ]: [ ERROR ]: failed to allocated buf\n");
            if(buf_loop==NULL) {
               free(buf_loop);
            }
            exit(1);
         }
         
         if(DEBUG>2) printf("[ pollDpmXmlString ]: Allocated longer buf at %p and copy to pointer %p (offset= %d) \n",buf,buf+buf_len,buf_len);      
         
         
         // do the copy
         memcpy(buf+buf_len,buf_loop,sizeof(char)*read_n);
         
         if(DEBUG>1) printf("[ pollDpmXmlString ]: memcpy done\n");
         
         //update the buffer length counter
         buf_len += read_n;      
         
         if(DEBUG>1) printf("[ pollDpmXmlString ]: free buf_loop\n");
         
         // free loop buffer for next loop
         if(buf_loop!=NULL) {
            free(buf_loop);
            buf_loop=NULL;
         }
         
         if(DEBUG>2) printf("[ pollDpmXmlString ]: end of read_i %d with buf strlen %d\n",read_i,strlen(buf));
         
         read_i++;
         
      } // read_n>0
      else {
         if(DEBUG>1) printf("[ pollDpmXmlString ]: Nothing to read from socket. Sleep a little..\n");      
         usleep(0.05);
         nempty++;
      } 
      
      
      
      if(n_endings>1) {
         if(DEBUG>1) printf("[ pollDpmXmlString ]: \nfound %d endings at read_i %d with at len %d and strlen %d. Stop reading from buffer\n",n_endings,read_i,buf_len,strlen(buf));      
         break;
      }
      
      
      counter++;
      
      
   } //time out
   
   
   
   if(DEBUG>0) {
     printf("[ pollDpmXmlString ]: Done reading from socket. Found %d endings and a buf_len of %d (dt=%d)\n",n_endings, buf_len, dt);
     if(buf!=NULL) printf("[ pollDpmXmlString ]: strlen %d\n", strlen(buf));
   }
   
   
   if(n_endings>=1) {
     if(DEBUG>1) printf("[ pollDpmXmlString ]: \nPick out config and status string between <system> and %d endings in string with strlen %d and buf_len %d\n",n_endings,strlen(buf),buf_len);
     if(DEBUG>1) printf("[ pollDpmXmlString ]: \nbuf: \n%s\n",buf);
     
      //search for the <status> tag in each <system>->'\f' substring
      
      char* start = NULL;
      char* tmp_str = NULL;
      int len = findSystemStr(buf, buf_len,&start);    
      if(len>0) {      
         char* stop = start+len;
         if(DEBUG>1) printf("[ pollDpmXmlString ]: len %d start at %p stop at %p\n",len,start, stop);
         if(DEBUG>1) printf("[ pollDpmXmlString ]: calloc xml string len %d\n",len+1);
         tmp_str = (char*) calloc(len+1,sizeof(char));
         if(DEBUG>1) printf("[ pollDpmXmlString ]: copy to xml string at %p\n",tmp_str);	
         memcpy(tmp_str,start,len);
         // terminate
         tmp_str[len] = '\0'; 
         if(DEBUG>1) printf("[ pollDpmXmlString ]: \ncopied %d chars to %p with strlen %d\n%s\n",len+1,tmp_str,strlen(tmp_str),tmp_str);
         // fix output pars
         *xml_string_out = tmp_str;
         *len_out = len+1;
         if(DEBUG>1) printf("[ pollDpmXmlString ]: output pars are at %p and len %d\n",*xml_string_out,*len_out);
         
         
      } else {
         if(DEBUG>0) printf("[ pollDpmXmlString ]: Couldn't find system and/or status string in xml buffer\n");
      }
   }
   
   if(xml_string_out==NULL) {
      if(DEBUG>0) printf("[ pollDpmXmlString ]: No valid xml string extracted from this poll (%d endings)\n",n_endings);
   }
   
   return;
   
}




void getDpmXmlDoc(int sockfd, int dpm, xmlDoc** dpm_doc_ptrptr) {

  if(DEBUG>0) printf("[ getDpmXmlDoc ]: from socket %d for dpm %d at %p\n",sockfd,dpm,*dpm_doc_ptrptr);

  if(*dpm_doc_ptrptr!=NULL) {
    if(DEBUG>-1) printf("[ getDpmXmlDoc ]: [ ERROR ]: xml doc is not null!\n");
    exit(1);
  }
  
  // check that the socket is open
  if(sockfd<=0) {
    if(DEBUG>0) printf("[ getDpmXmlDoc ]: [ ERROR ]: socket is not open.\n");
    exit(1);
  }
  
  char* xml_str;
  xml_str = NULL;
  if(DEBUG>2) printf("[ getDpmXmlDoc ]: Before reading xml string (%p)\n",xml_str);
  int xml_str_len = -1;
   pollDpmXmlString(sockfd, &xml_str, &xml_str_len);
   
   if(DEBUG>2) printf("[ getDpmXmlDoc ]: After reading xml string with %d chars (%p)\n",xml_str_len, xml_str);
   
   if(xml_str!=NULL) {
     if(DEBUG>1) printf("[ getDpmXmlDoc ]:  xml string is not null\n");
     //int tt=0;
     //for(tt=0;tt<100;++tt) printf("%c\n",*(xml_str+tt));
     if(DEBUG>1) printf("[ getDpmXmlDoc ]:  strlen(xml string) = %d\n", strlen(xml_str));
     if(DEBUG>1) printf("[ getDpmXmlDoc ]:  2 xml string is not null\n");
     
     if(strlen(xml_str)>0) {
       if(DEBUG>1) {
	 printf("[ getDpmXmlDoc ]: create xml document from xml string(strlen %d)\n",strlen(xml_str));
	 printf("[ getDpmXmlDoc ]: xml string:\n\"%s\"\n",xml_str);
       }
       *dpm_doc_ptrptr = xmlReadMemory(xml_str,strlen(xml_str),"noname.xml",NULL,0);
       if(DEBUG>1) printf("[ getDpmXmlDoc ]: xml doc done %p\n",*dpm_doc_ptrptr);
       if(*dpm_doc_ptrptr!=NULL) {
	 xmlNode* dpm_root = xmlDocGetRootElement(*dpm_doc_ptrptr);
	 if(dpm_root!=NULL) {
	   if(DEBUG>2) {
	     printf("[ getDpmXmlDoc ]: found dpm_root name %s\n",(dpm_root)->name);
	     printf("[ getDpmXmlDoc ]: print xml to file\n");
	   }
	   char tmpxmldocname[40];
	   sprintf(tmpxmldocname,"dpm%d.xml",dpm);
	   int bytes_written = xmlSaveFormatFile(tmpxmldocname,(*dpm_doc_ptrptr),1);
	   if(DEBUG>2) {
	     printf("[ getDpmXmlDoc ]: printed %d bytes of xml to file\n",bytes_written);
	   }
	 } else {
	   printf("[ getDpmXmlDoc ]: [ ERROR ]: xml doc built but no root element found!?\n");
	   exit(1);	   
	 }
       } else {
	 printf("[ getDpmXmlDoc ]: [ ERROR ]: problem building xml doc at %p from \n%s\n",(*dpm_doc_ptrptr),xml_str);
	 exit(1);
       }
     } else {
       printf("[ getDpmXmlDoc ]: [ ERROR ]: xml_string is there but has zero string length!\n");	
       exit(1);
     }
   } else {
     printf("[ getDpmXmlDoc ]: [ WARNING ]:  xml_string is NULL after reading from socket!\n");	
   }
   
   if(DEBUG>2)
     printf("[ getDpmXmlDoc ]:  dpm_doc_ptrptr at %p \n", (*dpm_doc_ptrptr));

   if(xml_str!=NULL) {
     printf("[ getDpmXmlDoc ]:  free xml_str at %p \n",xml_str);
     free(xml_str);
   }
     
    

    if(DEBUG>0) printf("[ getDpmXmlDoc ]: done.\n");

}


#endif

