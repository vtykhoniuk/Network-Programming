CC              = cc
CFLAGS          = -std=c99 -I. -I/usr/local/include -O1 -Wall -D_XOPEN_SOURCE=700
LDFLAGS         = -L/usr/local/lib
LDLIBS          = -lunet -lcerror

.PHONY : all clean
.DEFAULT_GOAL = all

BINS            = tcptimeserver-iterative-raw tcptimeserver-iterative-libunet tcptimeserver-iterative-libunet-protocol-independent

all: $(BINS)

clean:
	rm -f $(OBJS) $(BINS)
