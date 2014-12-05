#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <libxml/parser.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


static void print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
	printf("node type: Element, name: %s\n, content: \%s\n", cur_node->name, cur_node->content);
      }
      
      print_element_names(cur_node->children);
    }
}



static void print_hybrid_enabled(xmlDoc* doc, xmlNode* a_node) {
  
  xmlNode *cur_node = NULL;
  xmlChar* key;
  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if(cur_node->type == XML_ELEMENT_NODE) {
      //printf("node : %s\n, -> %d\n", cur_node->name,xmlStrcmp(cur_node->name,(const xmlChar * )"hybrid" ));
      if( xmlStrcmp(cur_node->name,(const xmlChar * )"Enabled" ) == 0) {
	printf("node type: Element, name: %s\n", cur_node->name);	
	key = xmlNodeListGetString(doc,cur_node->children,1);
	printf("Is it enabled? %s\n", key);	
	xmlFree(key);
	
      }
    }
  }
  
}

static void find_hybrid(xmlDoc* doc, xmlNode* a_node) {
  
  xmlNode *cur_node = NULL;
  
  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if(cur_node->type == XML_ELEMENT_NODE) {
      //printf("node : %s\n, -> %d\n", cur_node->name,xmlStrcmp(cur_node->name,(const xmlChar * )"hybrid" ));
      if( xmlStrcmp(cur_node->name,(const xmlChar * )"hybrid" ) == 0) {
	printf("node type: Element, name: %s\n", cur_node->name);	
	print_hybrid_enabled(doc,cur_node->children);
      } else {
	find_hybrid(doc,cur_node->children);
      }
    }
    
  }
}

int main(int argc, char *argv[])
{

    const unsigned int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    if (argc < 1) {
       fprintf(stderr,"usage %s xmlfile\n", argv[0]);
       exit(0);
    }
    
    xmlDoc         *document;
    xmlNode        *root, *first_child, *node;
    char           *filename;
    
    filename = "/u1/software/daq/config/devboard.xml";     
    document = xmlReadFile(filename, NULL, 0);
    root = xmlDocGetRootElement(document);
    fprintf(stdout, "Root is <%s> (%i)\n", root->name, root->type);
    //print_element_names(root);
    /*
    first_child = root->children;
    for (node = first_child; node; node = node->next) {
      fprintf(stdout, "\t Child is <%s> (%i)\n", node->name, node->type);
    }
    fprintf(stdout, "...\n");
    
    
    xmlFreeDoc(document);
    root = xmlDocGetRootElement(document);
    
    fprintf(stdout, "...\n");
    */
    //print_hybrid_enabled(root);
    find_hybrid(document,root);




    //Free and cleanup
    xmlFreeDoc(document);
    xmlCleanupParser();

    return 0;
}
