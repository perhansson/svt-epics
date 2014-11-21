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


double extractTempValFromString(char value[]);
void getSubStrFromName(char name[],const int i, char board_type[], const int MAX);
void getType(char name[], char board_type[], const int MAX);
void getBoardType(char name[], char board_type[], const int MAX);
void getHalf(char name[], char board_type[], const int MAX);
void getId(char name[], char board_type[], const int MAX);
void getChName(char name[], char board_type[], const int MAX);
void getAction(char name[], char board_type[], const int MAX);
void getFebId(char name[], char board_type[], const int MAX);
void getFebChName(char name[], char board_type[], const int MAX);
void getFebAction(char name[], char board_type[], const int MAX);
void getHalfFromDaqMapRecordName(char name[], char board_type[], const int MAX);
void getHybridFromDaqMapRecordName(char name[], char board_type[], const int MAX);
void error(const char *msg);
int open_socket(char* hostname, int portno);
int close_socket(int socketfd);
void pollXmlString(int socketfd);
xmlNode* retrieveElement(xmlDoc* doc, xmlNode* node, char* tag);
void retrieveValue(xmlDoc* doc, xmlNode* node, char* tags, char value[], const unsigned int MAX);
void writeReadConfig(int sockfd);
void writeReadStatus(int sockfd);
void writeHybridVTrim(int sockid, int value, int hyb_id, char ch_name[]);
void writeHybridVSwitch(int sockid, int value, int hyb_id);
void readHybridVTrim(int feb_id, int hyb_id, char ch_name[], char value[],const unsigned int MAX);
void readXadcT(int feb_id, char value[],const unsigned int MAX);
void readHybridI(int feb_id, int hyb_id,char ch_name[], char value[],const unsigned int MAX);
void readHybridT(int feb_id, int hyb_id, int tId, char value[],const unsigned int MAX);
void readFebT(int feb_id, char value[], char ch_name[], const unsigned int MAX);
void readHybridV(int feb_id, int hyb_id,char ch_name[], char ch_pos[], char value[],const unsigned int MAX);
void readHybridVSwitch(int feb_id, int hyb_id, char value[],const unsigned int MAX);
void getXmlDoc(int sockfd, int read_status, int read_config);
void getXmlDocStrFormat(char** xml_str, int * xml_str_len);
int getFebIdFromDaqMap(int hyb_id, char half[]);
int getXMLPollStatus();

#endif
