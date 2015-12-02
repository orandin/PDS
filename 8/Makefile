CC      = gcc
CFLAGS  = -Wall -Werror
TP=8
NOM1=DELBERGHE
NOMARCHIVE = TP${TP}_${NOM1}

CFLAGS += -g
LDFLAGS = -g -pthread

BINS    = aleazard compteur-gc

all: ${BINS}

aleazard: aleazard.o
	${CC} ${LDFLAGS} -o $@ $^

compteur-gc: compteur-gc.o
	${CC} ${LDFLAGS} -o $@ $^

%.o: %.c %.h
	${CC} ${CFLAGS} -c $<
%.o: %.c
	${CC} ${CFLAGS} -c $<

clean:
	rm *.o || true

realclean: clean
	rm ${BINS} || true
	rm text || true
	rm res.* || true

test: 
	./test.sh

archive: realclean
	mkdir ${NOMARCHIVE} || true
	cp ./* ${NOMARCHIVE}/ || true
	tar -zcf ${NOMARCHIVE}.tar.gz ${NOMARCHIVE}/
	rm -R ${NOMARCHIVE}


.PHONY: all clean realclean test archive

