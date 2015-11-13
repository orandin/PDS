#! /bin/sh -uf
#
# mcat -- campagne d'appels à mcat-scd
#
# squelette de shell script

# La commande à tester
MCAT=./mcat
# le fichier à lui mettre en entrée
MCAT_INPUT=bigfile
# Le fichier de temps à générer
TIME_FILE=mcat-tm.dat

# la commande gnu time
TIME_CMD="/usr/bin/time"
# les options de cette commande
TIME_OPT="%e %U %S"

# initialisation du fichier de résultats
rm -f $TIME_FILE && echo "# taille real user sys" > $TIME_FILE
rm -f $MCAT_INPUT

# Création d'un fichier lourd (de 35Mo)
dd if=/dev/zero of=$MCAT_INPUT bs=1M count=35

# un autre exemple de boucle
for size in `awk 'BEGIN { for( i=1; i<=8388608; i*=2 ) print i }'`; do
    export MCAT_BUFSIZ=$size
    echo $MCAT_BUFSIZ

	$TIME_CMD "-f $MCAT_BUFSIZ $TIME_OPT" $MCAT $MCAT_INPUT > /dev/null 2>> $TIME_FILE
done

echo "Génération de la courbe avec gnuplot"
gnuplot run.gnu

echo "Fin du test. Bonne journée !"
# eof
