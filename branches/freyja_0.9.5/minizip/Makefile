CC=cc
CFLAGS=-O -I../..

UNZ_OBJS = miniunz.o unzip.o ioapi.o
ZIP_OBJS = minizip.o zip.o   ioapi.o 

LIB_OBJS = zip.c unzip.c ioapi.c minizip.c miniunz.c

.c.o:
	$(CC) -c $(CFLAGS) $*.c

libminizip.so:
	$(CC) $(CFLAGS) -shared -lz -fPIC $(LIB_OBJS) -o $@

all: miniunz minizip

miniunz:  $(UNZ_OBJS)
	$(CC) $(CFLAGS) -o $@ $(UNZ_OBJS)  

minizip:  $(ZIP_OBJS)
	$(CC) $(CFLAGS) -o $@ $(ZIP_OBJS) 

test:	miniunz minizip
	./minizip test readme.txt
	./miniunz -l test.zip
	mv readme.txt readme.old
	./miniunz test.zip

clean:
	/bin/rm -f *.o *~ minizip miniunz
