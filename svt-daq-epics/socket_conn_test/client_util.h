#ifndef __client_util__
#define __client_util__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <libxml/parser.h>

void read_system_xml_string3(int* socketfd, char buffer[], const int MAX);
void read_system_xml_string2(int* socketfd, char buffer[], const int MAX);
char* read_system_xml_string(int* socketfd);
char* retrieveTemp(xmlDoc* doc, xmlNode* node, int rce, int hyb);
char* retrieveTagValue(xmlDoc* doc, xmlNode* node, char tag[]);
char* retrieveTagTagValue(xmlDoc* doc, xmlNode* node, char tag[],char tag_mother[]);
void print_element_names(xmlNode * a_node);
//xmlChar* retrieveValue(xmlDoc* doc, xmlNode* node, char* tags);
char* retrieveValue(xmlDoc* doc, xmlNode* node, char* tags);
char* getTemp(xmlDoc* doc, xmlNode* cur, int rce, int hyb);
xmlNode* retrieveElement(xmlDoc* doc, xmlNode* node, char* tag);
char* copy_substr(char* buf_src, int* nread, const char* str_start, const char* str_end);
char* copy_to_array(int n, int len, char* xml_buffer[]);
char* read_xml(int *sockfd, int* len);
void print_element_names(xmlNode * a_node);
char* get_xml_substr(char* xml_buf, int n, int* n_read, const char* p_start_str, const char* p_end_str);
void findTemp(char* xml_buf, int n);



#endif
