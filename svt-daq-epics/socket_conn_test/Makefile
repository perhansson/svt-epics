
all:
	gcc -Wall -g -c -I/usr/include/libxml2 client_util.c
	gcc -g -o client_poll -I./ -I/usr/include/libxml2 -lxml2 -lz -lm client_util.o client_poll.c
	gcc -g -o xml_client -I/usr/include/libxml2 -lxml2 -lz -lm xml_client.c
	gcc -g -o client -I./ -I/usr/include/libxml2 -lxml2 -lz -lm client_util.o client.c
