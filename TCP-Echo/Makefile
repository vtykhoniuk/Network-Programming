CC              = cc
CFLAGS          = -std=c99 -I. -I/usr/local/include -O1 -Wall
LDFLAGS         = -L/usr/local/lib
LDLIBS          = -lunet -lcerror

.PHONY : all clean
.DEFAULT_GOAL = all

BINS            = tcpechoserver-fork

all: $(BINS)

clean:
	rm -f $(OBJS) $(BINS)
