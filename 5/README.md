# TP: 5
## PDS - L3 info - Groupe 4 

Réalisé par:
* Simon DELBERGHE


## Description

### Multif 

#### Commande 
La commande attend un seul et unique argument, un chiffre correspond au nombre de fils à créer.

#### Prototype
Les valeurs de retours du prototype multif correspondent à ceux de la logique booléenne. À savoir, `0` vaut `false` et `1` vaut `true`.



## Réponses aux questions du poly

### Question 8.2

Le processus père s'interrompt. Les enfants continuent de s'exécuter sans être perturbés. Ils sont devenus des processus orphelins dont le système leur a attribué un nouveau père, à savoir le processus init, dont le pid vaut 1.


### Question 8.3

Voici le résultat de la commande `ps -a` lorsqu'un fils est tué sans que le père n'ait commencé à attendre la terminaison de ses processus fils :

	> 10079 pts/0    00:00:00 exo8 
	> 10080 pts/0    00:00:00 exo8 
	> 10081 pts/0    00:00:00 exo8 <defunct>

Le processus 10081 est à l'état zombie *(defunct process)* puisque son père ne l'a pas attendu lorsqu'il a terminé.



Après saisie clavier, le père notifie que le fils 10081 s'est terminé et on obtient le résultat suivant avec la commande `ps -a` :

	> 10079 pts/0    00:00:00 exo8
	> 10080 pts/0    00:00:00 exo8

Le processus fils 10081 n'est plus dans l'état zombie car son père l'a attendu. Il a alors changé d'état et n'est désormais plus affiché par la commande `ps -a`.