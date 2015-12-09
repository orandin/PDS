# TP: 9
## PDS - L3 info - Groupe 4 

Réalisé par:
* Simon DELBERGHE

## Comment l'utiliser ?

Syntaxe pour utiliser la commande `tri-rapide` : 
	`./tri-rapide [options] {fichier à trier}`

Avec comme options :

* -h : affiche ce message
* -t : choisit le format « texte » (défaut: binaire) (il est nécessaire d’indiquer -l dans ce cas)
* -c : contrôle que le résultat final est trié (débogage)
* -a : affiche le tableau trié (sur la sortie standard)
* -v : affiche des informations supplémentaires
* -n {nombre} : utilise NBTH threads (défaut = 1)
* -l {taille} : indique le nombre d’éléments à lire dans le fichier (défaut = taille du fichier binaire)
-s {taille} : choisit la taille seuil des blocs « longs » (défaut = 512)


*Exemple d'utilisation* :

On crée un fichier binaire (/tmp/alea) aléatoire (et non trié).
`dd if=/dev/urandom of=/tmp/alea bs=1048576 count=100`

On exécute ensuite le tri rapide sur ce fichier avec 3 threads
`./tri-rapide -n 3 -c /tmp/alea`