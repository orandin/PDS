#!/bin/bash
echo "====== DEBUT DES TESTS ========"
echo "Création du dossier exemples/"
mkdir exemples/

etat_test() {
	if [ $? -eq 1 ]
	then
		echo "Test : OK"
	else
		echo "Test : KO"
	fi
	echo ""
}

# === TEST : EACCESS {1} ===
# On vérifie qu'une erreur EACCESS est retournée lorsque l'on cherche à lire
# les permissions d'un fichier dont nous n'avons pas les droits (chmod 000).

touch exemples/test_eaccess
chmod 000 exemples/test_eaccess

echo "---- TEST : EACCESS {1} ----"
./maccess -r -v exemples/test_eaccess
etat_test

# === TEST: EACCESS {2} ===
# On vérifie qu'une erreur EACCESS est retournée lorsque l'on cherche à lire
# les permissions d'un fichier contenu dans un dossier dans lequel nous n'avons
# aucun droit.

mkdir exemples/eaccess
touch exemples/eaccess/file
chmod 000 exemples/eaccess

echo "---- TEST : EACCESS {2} ----"
./maccess -r -v exemples/eaccess/file
etat_test

# === TEST : ENAMETOOLONG ===
# On vérifie qu'une erreur ENAMETOOLONG est retournée lorsque l'on renseigne
# un pathname supérieur à 255

echo "---- TEST : ENAMETOOLONG ----"
./maccess -r -v tttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttttt
etat_test

# === TEST : ENOTDIR ===
# On vérifie qu'une erreur ENOTDIR est retournée lorsque l'on renseigne un
# pathname dans lequel un élement d'accès n'est pas un repertoire

touch exemples/enotdir

echo "---- TEST : ENOTDIR ----"
./maccess -r -v exemples/enotdir/file
etat_test


# === TEST : ENOENT {1} ===
# On vérifie qu'une erreur ENOENT est retournée lorsque l'on renseigne un
# pathname dans lequel un élément d'accès n'existe pas.

echo "---- TEST : ENOENT {1} ----"
./maccess -r -v exemples/notexist/file
etat_test


# === TEST : ENOENT {2} ===
# On vérifie qu'une erreur ENOENT est retournée lorsque l'on renseigne un
# pathname dans lequel un pointeur pointe vers nulle part.

cd exemples/
ln -s lien_nul lien_vide
cd ..

echo "---- TEST : ENOENT {2} ----"
./maccess -r -v exemples/lien_vide
etat_test

# === TEST : ELOOP ===
# On vérifie qu'une erreur ELOOP est retournée lorsque trop de lien 
# symbolique ont été rencontrés lors du parcours de pathname

cd exemples/
ln -s lien lien2
ln -s lien2 lien
cd ..

echo "---- TEST : ELOOP ----"
./maccess -r -v exemples/lien2
etat_test


# === TEST : EINVAL ===
# On vérifie qu'une erreur de type EINVAL est détectée lorsqu'aucune
# option de "mode" n'a été renseigné.
 
echo "---- TEST : EINVAL ----"
./maccess -v exemples/lien2
etat_test


# ===== NETTOYAGE =====
# On s'assure d'avoir les droits de suppression sur l'ensemble des fichiers
# contenu dans le dir "exemples"


chmod -R 777 exemples/
rm -R exemples/
echo "Nettoyage terminé (suppression du dossier exemples/ et son contenu)"


echo "====== FIN DES TESTS ========"