CXX 		= gcc
CXXFLAGS	= -g
LD		= gcc
LDFLAGS		= -g -L.

ROOTCXXFLAGS	:= #$(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS	:= #$(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS	:= #$(shell $(ROOTCONFIG) --libs)

XML2LIB		:= -L/usr/lib -l xml2
XML2INC		:= -I/usr/include/libxml2

CXXFLAGS	:= $(ROOTCXXFLAGS) $(CXXFLAGS)
LDFLAGS		:= $(ROOTLDFLAGS) $(LDFLAGS) $(XML2INC)
LIBS		:= $(ROOTLIBS) $(LIBS) $(XML2LIB)

HDRS		= commonXml.h commonConstants.h commonDoc.h
SRCS		= commonXml.c commonConstants.c commonDoc.c
OBJS		= commonXml.o commonConstants.o commonDoc.o


all: libsvtDaq.so

libsvtDaq.so:	$(OBJS) $(SRCS) $(HDRS)
	@echo "Linking $(@)..."
	@rm -f $@
	$(LD) $(LDFLAGS) -shared -o $@ $(OBJS) $(LIBS)
	@echo "done linking"

%.o: %.c %.h
	@echo "Compiling $(@)..."
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $*.c

clean:
	@rm *.o 
	@rm *.so



