all: keyword
clean:
	rm -f *.o 
	rm -f keyword


LIBS = -L/usr/lib -lxml2
INCS = -I/usr/include/libxml2

objs: common.h common.c febxml.h febxml.c hybxml.h hybxml.c
	g++ ${INCS} -c common.c
	g++ ${INCS} -c febxml.c
	g++ ${INCS} -c hybxml.c


keyword: keyword.c objs
	g++ ${LIBS} ${INCS} common.o febxml.o hybxml.o keyword.c  -o keyword
