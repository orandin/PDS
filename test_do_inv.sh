#! /bin/sh

./do --or  true  true  && echo "X [1.1] -or true true: renvoie false";
./do --or  true  false && echo "X [1.2] -or true false: renvoie false";
./do --or  false true  && echo "X [1.3] -or false true: renvoie false";
./do --or  false false || echo "X [1.4] -or false false: renvoie true";
./do --and true  true  && echo "X [2.1] -and true true: renvoie false";
./do --and true  false || echo "X [2.2] -and true false: renvoie true";
./do --and false true  || echo "X [2.3] -and false true: renvoie true";
./do --and false false || echo "X [2.4] -and false false: renvoie true";

# doit lancer xterm et s'arreter
./do --or  --cc xterm true  && echo "X [3.1] erreur valeur retour"
# doit lancer xterm et ne pas s'arreter
./do --or  --cc xterm false && echo "X [3.2] erreur valeur retour"
# doit lancer xterm et ne pas s'arreter
./do --and --cc xterm true  && echo "X [3.3] erreur valeur retour"
# doit lancer xterm et s'arreter
./do --and --cc xterm false || echo "X [3.4] erreur valeur retour"

# doit lancer xterm et s'arreter en tuant xterm
./do --or  --cc --kill xterm true  && echo "X [4.1] erreur valeur retour"
# doit lancer xterm et ne pas s'arreter
./do --or  --cc --kill xterm false && echo "X [4.2] erreur valeur retour"
# doit lancer xterm et ne pas s'arreter
./do --and --cc --kill xterm true  && echo "X [4.3] erreur valeur retour"
# doit lancer xterm et s'arreter en tuant xterm
./do --and --cc --kill xterm false || echo "X [4.4] erreur valeur retour"

# doit lancer 2 xterm : des que le 1er quitte tout le monde meurt
./do --or  --cc --kill xterm xterm && echo "X [5.1] erreur valeur retour"
# doit lancer 2 xterm : les deux doivent etre quittÃ©s independament pour que le prog termine
./do --and --cc --kill xterm xterm && echo "X [5.2] erreur valeur retour"
