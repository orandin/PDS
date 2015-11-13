## Nom(s) de/des executable(s)
EXEC = do

## Nom de l'archive
TP=6
NOM1=DELBERGHE

NOMARCHIVE = TP${TP}_${NOM1}


## Compilateur C et éditeur de liens
CC      = gcc

## Options pour la compilation du C
CFLAGS  = -Wall -Werror -ansi -pedantic

#  Spécifie la version de la norme POSIX à respecter
CFLAGS += -D_XOPEN_SOURCE=500
#  Active les informations de débogage
CFLAGS += -g

## Options de l’édition de liens
LDFLAGS  = -g

## Première cible
#  « make » sans argument construit sa première cible
#  Indiquez en dépendance le ou les binaires à construire
all: ${EXEC}

## Compilation séparée
#  Le .o doit être recompilé dès que le .c ou le .h (s'il existe) change
%.o: %.c %.h
	${CC} ${CFLAGS} -c $<

## Édition de liens
#  Définissez une règle par binaire que vous voulez créer
#  La commande devrait en général être identique pour chaque binaire
# prlimit: prlimit.o
# 	${CC} ${LDFlags} -O $@ $^

test: 
	./test.sh

archive: realclean
	mkdir ${NOMARCHIVE} || true
	cp ./* ${NOMARCHIVE}/ || true
	tar -zcf ${NOMARCHIVE}.tar.gz ${NOMARCHIVE}/
	rm -R ${NOMARCHIVE}

clean:
	rm *.o || true

realclean: clean
	rm ${EXEC} || true

## Liste des pseudo-cibles
#  Ces cibles ne correspondent pas à des fichiers que l'on veut créer,
#  juste à des séquences que l'on veut pouvoir déclencher
.PHONY: all clean realclean test archive
