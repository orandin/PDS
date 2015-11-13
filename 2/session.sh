#!/bin/bash
echo "====== DEBUT DES TESTS ========"
echo "Création des dossiers contenant les résultats."

mkdir resultats/
mkdir resultats/mdu/
mkdir resultats/du/

etat_test() {
	if [ $? -eq 0 ]
	then
		echo "OK"
	else
		echo "KO"
	fi
	echo ""
}

echo "Création de l'arborescence :"
mkdir exemples/
mkdir exemples/tutu
mkdir exemples/tata
mkdir exemples/tata/toto
mkdir exemples/titi
mkdir exemples/titi/tete
mkdir exemples/titi/tete/to

echo "******************"
ls -A -R exemples/
echo "******************"

echo""
echo "Création d'un fichier contenant un texte et un autre vide."
echo "toto" > exemples/tata/toto/toto.txt
touch exemples/tutu/tutu.txt

echo""
echo "******************"
ls -A -R exemples/
echo "******************"
echo""

echo "# Test 1 (sans aucune option)"
./mdu exemples > resultats/mdu/test1
du --block-size=512 exemples > resultats/du/test1

diff resultats/mdu/test1 resultats/du/test1
etat_test

echo "# Test 2 (avec l'option -b)"
./mdu -b exemples > resultats/mdu/test2
du --block-size=512 -b exemples > resultats/du/test2

diff resultats/mdu/test2 resultats/du/test2
etat_test

echo "Création d'un fichier symbolique pointant vers le fichier exemples/tata/toto/toto"
cd exemples/
ln -s tata/toto/toto.txt lien
cd ..

echo""
echo "******************"
ls -A -R exemples/
echo "******************"
echo""

echo "# Test 3 (avec l'option -L et sans -b)"
./mdu -L exemples > resultats/mdu/test3
du --block-size=512 -L exemples > resultats/du/test3

diff resultats/mdu/test3 resultats/du/test3
etat_test


echo "# Test 4 (avec l'option -L et avec -b)"
./mdu -b -L exemples > resultats/mdu/test4
du --block-size=512 -b -L exemples > resultats/du/test4

diff resultats/mdu/test4 resultats/du/test4
etat_test

echo "# Test 5 (sans l'option -L et sans -b)"
./mdu exemples > resultats/mdu/test5
du --block-size=512 exemples > resultats/du/test5

diff resultats/mdu/test5 resultats/du/test5
etat_test


echo "Création de deux liens symboliques pointant l'un sur un même fichier"
cd exemples/
ln -s lien lien2
ln -s lien lien3
cd ..

echo""
echo "******************"
ls -A -R exemples/
echo "******************"
echo""

echo "# Test 6 (avec l'option -L et avec -b)"
./mdu -b -L exemples > resultats/mdu/test6
du --block-size=512 -b -L exemples > resultats/du/test6

diff resultats/mdu/test6 resultats/du/test6
etat_test


echo "# Test 7 (avec l'option -L et sans -b)"
./mdu -L exemples > resultats/mdu/test7
du --block-size=512 -L exemples > resultats/du/test7

diff resultats/mdu/test7 resultats/du/test7
etat_test



echo "# Test 8 (sans l'option -L et sans -b)"
./mdu exemples > resultats/mdu/test8
du --block-size=512 exemples > resultats/du/test8

diff resultats/mdu/test8 resultats/du/test8
etat_test




# ===== NETTOYAGE ====

chmod -R 777 resultats/
chmod -R 777 exemples/
rm -R exemples/
rm -R resultats/

echo "Nettoyage terminé (suppression des dossiers exemples/ et resultats/ ainsi que de leur contenu)"

echo "====== FIN DES TESTS ========"