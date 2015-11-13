#!/bin/bash

etat_test() {
	if [ $? -eq 0 ]
	then
		echo "OK"
	else
		echo "KO"
	fi
	echo ""
}

# Variables "globales"
prog=tail
prog2=mtail
prog3=mtail_stupide
dirResultat=resultats
dirRP=$dirResultat/$prog
dirRP2=$dirResultat/$prog2
dirRP3=$dirResultat/$prog3
dirExemples=exemples
nbTest=1

echo "====== INITIALISATION ========"
echo "Création des dossiers contenant les résultats."


mkdir $dirResultat/
mkdir $dirRP/
mkdir $dirRP2/
mkdir $dirRP3/

echo "Création du dossier ${dirExemples}"
mkdir $dirExemples/

echo "====== DEBUT DES TESTS ========"
# ---- TEST 1  ----

echo "Création d'un fichier vide."
touch $dirExemples/vide.txt

tail $dirExemples/vide.txt > $dirRP/test_$nbTest
#./mtail $dirExemples/vide.txt > $dirRP2/test_$nbTest
./mtail_stupide $dirExemples/vide.txt > $dirRP3/test_$nbTest

echo ">> Test diff entre ${prog} et ${prog2}"
#diff $dirRP/test_$nbTest $dirRP2/test_$nbTest
#etat_test

echo ">> Test diff entre ${prog} et ${prog3}"
diff $dirRP/test_$nbTest $dirRP3/test_$nbTest
etat_test

# ---- TEST 2  ----
nbTest+=1

txtFile="1\n2\n3\n4"
echo "Création d'un fichier contenant : \"${txtFile}\""
echo -e $txtFile > $dirExemples/file_$nbTest.txt

tail $dirExemples/file_$nbTest.txt > $dirRP/test_$nbTest
#./mtail $dirExemples/file_$nbTest.txt > $dirRP2/test_$nbTest
./mtail_stupide $dirExemples/file_$nbTest.txt > $dirRP3/test_$nbTest

echo ">> Test diff entre ${prog} et ${prog2}"
#diff $dirRP/test_$nbTest $dirRP2/test_$nbTest
#etat_test

echo ">> Test diff entre ${prog} et ${prog3}"
diff $dirRP/test_$nbTest $dirRP3/test_$nbTest
etat_test


# ---- TEST 3  ----

nbTest+=1
txtFile="1\n2\n3\n4\n"
echo "Création d'un fichier contenant : \"${txtFile}\" (Ajout d'un \n à la fin du fichier)"
echo -e $txtFile > $dirExemples/file_$nbTest.txt

tail $dirExemples/file_$nbTest.txt > $dirRP/test_$nbTest
#./mtail $dirExemples/file_$nbTest.txt > $dirRP2/test_$nbTest
./mtail_stupide $dirExemples/file_$nbTest.txt > $dirRP3/test_$nbTest

echo ">> Test diff entre ${prog} et ${prog2}"
#diff $dirRP/test_$nbTest $dirRP2/test_$nbTest
#etat_test

echo ">> Test diff entre ${prog} et ${prog3}"
diff $dirRP/test_$nbTest $dirRP3/test_$nbTest
etat_test


# ---- TEST 4  ----

nbTest+=1
txtFile="1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11"
echo "Création d'un fichier contenant : \"${txtFile}\""
echo -e $txtFile > $dirExemples/file_$nbTest.txt

tail $dirExemples/file_$nbTest.txt > $dirRP/test_$nbTest
#./mtail $dirExemples/file_$nbTest.txt > $dirRP2/test_$nbTest
./mtail_stupide $dirExemples/file_$nbTest.txt > $dirRP3/test_$nbTest

echo ">> Test diff entre ${prog} et ${prog2}"
#diff $dirRP/test_$nbTest $dirRP2/test_$nbTest
#etat_test

echo ">> Test diff entre ${prog} et ${prog3}"
diff $dirRP/test_$nbTest $dirRP3/test_$nbTest
etat_test


# ---- TEST 5  ----

nbTest+=1
txtFile="1\n2\n3\n4\n5\n6"
echo "Création d'un fichier contenant : \"${txtFile}\""
echo -e $txtFile > $dirExemples/file_$nbTest.txt
echo "Initialisation de la variable -n à 3"

tail -3 $dirExemples/file_$nbTest.txt > $dirRP/test_$nbTest
#./mtail $dirExemples/file_$nbTest.txt > $dirRP2/test_$nbTest
./mtail_stupide -n 3 $dirExemples/file_$nbTest.txt > $dirRP3/test_$nbTest

echo ">> Test diff entre ${prog} et ${prog2}"
#diff $dirRP/test_$nbTest $dirRP2/test_$nbTest
#etat_test

echo ">> Test diff entre ${prog} et ${prog3}"
diff $dirRP/test_$nbTest $dirRP3/test_$nbTest
etat_test


echo "====== FIN DES TESTS ========"

# ===== NETTOYAGE =====
# On s'assure d'avoir les droits de suppression sur l'ensemble des fichiers
# contenu dans le dir "exemples"


chmod -R 777 $dirExemples/
rm -R $dirExemples/

chmod -R 777 $dirResultat/
rm -R $dirResultat/
echo "Nettoyage terminé (suppression du dossier exemples/ et son contenu)"

echo "====== FIN DU SCRIPT ========"
