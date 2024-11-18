### Makefile ---

## Author: noahm@LAPTOP-RTLRU19Q.
## Keywords:
## X-URL:

CFLAGS=-g -Wall
OBJECTS=ifaddrs.o

all: main

main: $(OBJECTS)

clean:
	-rm -f *~ *.o main

.PHONY: all clean

### Makefile ends here
