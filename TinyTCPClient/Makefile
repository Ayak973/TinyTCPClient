CC=g++
RM=rm -f
CPPFLAGS=-c -Wall -pthread
LDFLAGS=-g
LDLIBS=-lpthread
DEPS=stringhelper.h socket.h

SRCS=socket.cpp main.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: main

debug: CPPFLAGS += -DDEBUG -g
debug: main

main: $(OBJS)
	$(CC) $(LDFLAGS) -o tinytcpclient $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS) $(DEPS)
	$(RM) ./.depend
	$(CC) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) *~ .depend

include .depend
