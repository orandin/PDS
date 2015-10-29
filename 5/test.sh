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


echo "====== DEBUT DU SCRIPT ========"

echo "Execution de multif avec 10 fils."
echo "----------------------"
./multif 10

echo ""
echo "Execution de race"
echo "----------------------"
./race
etat_test

echo "====== FIN DU SCRIPT ========"
