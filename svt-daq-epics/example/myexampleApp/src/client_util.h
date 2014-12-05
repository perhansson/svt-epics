#ifndef __client_util__
#define __client_util__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <libxml/parser.h>



char* strToUpper( char* s );
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
void writeReadConfig(int sockfd);
void writeReadStatus(int sockfd);


void pollXmlString(int socketfd);
xmlNode* retrieveElement(xmlDoc* doc, xmlNode* node, char* tag);
void retrieveValue(xmlDoc* doc, xmlNode* node, char* tags, char value[], const unsigned int MAX);
void getXmlDoc(int sockfd, int read_status, int read_config);
void getXmlDocStrFormat(char** xml_str, int * xml_str_len);
int getXMLPollStatus();



double getFebT(int feb_id, char* ch_name);
double getHybridT(int index, int hyb, const char* type);
double getHybridI(int index, int hyb, const char* type);
double getHybridV(int feb_id, int hyb_id,char ch_name[], char ch_pos[]);
double getHybridSwitch(int index, int hyb);
void writeHybridVSwitch(int sockid, int value, int feb_id, int hyb_id);
double getHybridTrim(int index, int hyb, char* type);
void writeHybridVTrim(int sockid, int value, int feb_id, int hyb_id, char *ch_name);

#endif
