# TP : mTail
## PDS - L3 info - Groupe 4 

Réalisé en binome:

* Benjamin SZCZAPA
* Simon DELBERGHE


## Compilation
Taper `make` tout court pour compiler l'ensemble des programmes.

## Problème
Le mtail n'est pas opérationnel car nous avons rencontrés des difficultés à manipuler lseek. Lorsque l'on initialise le curseur à la fin du fichier, read ne lit rien. 



## Notes : 

### Comptage des lignes : Problème de la dernière ligne

Pour connaître le nombre de lignes d'un fichier, nous comptons le nombre de retour à la ligne trouvé (\n). Cependant, il y a toujours une ligne qui ne possède pas de retour à la ligne. 

Pour résoudre ce problème, nous avons établi un postulat :

	*« Tout fichier, y compris le fichier vide, possède une ligne qui ne se termine pas par le caractère "\n". »*


**Conséquence** :
	* N'importe quel fichier a au moins une ligne, qui peut être vide ou comporter au moins un caractère quelconque.


**Ainsi**, notre compteur est initialisé à 1 au lieu de 0, ce qui permet de compter la dernière ligne.