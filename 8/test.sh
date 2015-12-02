#! /bin/sh

TEXT_FILE=text

TIME_FILE=res.dat

# initialisation du fichier de résultats
rm -f $TEXT_FILE
rm -f $TIME_FILE

# On compile les programmes
make

# On lance l'expérimentation
for size in `awk 'BEGIN { for( i=100; i<=1000000000; i*=10 ) print i }'`;
do
	echo "Génération d'un fichier GC de taille: $size"
	./aleazard $size > $TEXT_FILE

	for nbthreads in `awk 'BEGIN { for( i=1; i<=32; i*=2 ) print i }'`;
	do
		echo "Test avec $nbthreads thread(s)."
		./compteur-gc $TEXT_FILE $nbthreads >> $TIME_FILE
	done
done

echo "Génération de la courbe avec gnuplot"
gnuplot run.gnu
echo "Fin du test. Bonne journée !"