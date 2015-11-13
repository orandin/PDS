# TP : Maccess
## PDS - L3 info - Groupe 4 

Réalisé en binome:

* Benjamin SZCZAPA
* Simon DELBERGHE

## Contrainte Maccess et Maccess+

L'exécutable suppose que le dernier argument passé dans la commande correspond au nom du pathname.
Toutes les options doivent donc être passées avant.

Exemple : 
./maccess [-r] [-w] [-x] [-v] {pathname}

L'ordre des options entre elles n'a aucune importance.
De plus, il est possible dans la commande maccess d'ajouter plusieurs options.

## Au sujet des tests d'erreur non produits

Dans la question 3, nous n'avons pas pu tester les erreurs suivantes :

* **ENOMEN** : Il est très difficile de simuler la saturation mémoire du noyau et étrangement, on a pas très envie d'essayer ... ! :-)
* **EROFS** : Pour simuler cette erreur, il faut écrire un fichier sur un périphérique que l'on rendrait par la suite en lecture seule (tel qu'un CD). 
* **EFAULT** : Il faudrait créer un fichier en dehors de l'espace d'adressage accessible et on a pas la moindre idée de comment le faire. 
