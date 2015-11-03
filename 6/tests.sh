#! /bin/sh

echo "====== DEBUT DU SCRIPT ========"

echo "Vérification du fonctionnement du do avec les options AND et OR."
echo "----"
./do -o  true  true  || echo "Test 1.1: Erreur (renvoie false)";
./do -a  true  true  || echo "Test 1.2: Erreur (renvoie false)";

./do -o  false false && echo "Test 1.3: Erreur (renvoie true)";
./do -a  false false && echo "Test 1.4: Erreur (renvoie true)";

./do -o  true  false || echo "Test 1.5: Erreur (renvoie false)";
./do -a  true  false && echo "Test 1.6: Erreur (renvoie true)";

./do -o  false true  || echo "Test 1.7: Erreur (renvoie false)";
./do -a  false true  && echo "Test 1.8: Erreur (renvoie true)";

echo "----------------------"
echo "Vérification du fonctionnement du do avec l'option -c."
echo "----"
# Dans les deux tests suivants, on lance xclock et on redonne la main.
./do -o -c xclock true  || echo "Test 2.1: Valeur retour incorrecte"
./do -a -c xclock false && echo "Test 2.2: Valeur retour incorrecte"

# Dans les deux tests suivants, on lance xclock et on ne redonne pas la main., 
./do -o -c xclock false || echo "Test 2.3: Valeur retour incorrecte"
./do -a -c xclock true  || echo "Test 2.4: Valeur retour incorrecte"

echo "----------------------"
echo "Vérification du fonctionnement du do avec l'option -k."
echo "----"

# Dans les deux tests suivants, on lance xclock et on redonne la main en tuant xclock
./do -o -c -k xclock true  || echo "Test 3.1: Valeur retour incorrecte"
./do -a -c -k xclock false && echo "Test 3.2: Valeur retour incorrecte"

# Dans les deux tests suivants, on lance xclock et on ne redonne pas la main.
./do -o -c -k xclock false || echo "Test 3.3: Valeur retour incorrecte"
./do -a -c -k xclock true  || echo "Test 3.4: Valeur retour incorrecte"

# On lance 2 xclock et dès qu'un processus a terminé, on tue le reste.
./do -o  -c -k xclock xclock || echo "Test 3.5: Valeur retour incorrecte"
# On lance 2 xclock et ils doivent être quittés manuellement avant de redonner la main
./do -a -c -k xclock xclock || echo "Test 3.6: Valeur retour incorrecte"

echo "====== FIN DU SCRIPT ========"