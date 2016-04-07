CC              = cc
CFLAGS          = -std=c99 -I. -I/usr/local/include -O1 -Wall -D_XOPEN_SOURCE=700
LDFLAGS         = -L/usr/local/lib
LDLIBS          = -lunet -lcerror

.PHONY : all clean
.DEFAULT_GOAL = all

BINS            = tcpechoserver-fork tcpechoclient01

all: $(BINS)

clean:
	rm -f $(OBJS) $(BINS)
