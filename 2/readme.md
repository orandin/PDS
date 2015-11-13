# TP : mdu
## PDS - L3 info - Groupe 4 

Réalisé en binome:

* Benjamin SZCZAPA
* Simon DELBERGHE


## Description du mdu

./mdu [-L] [-b] {pathname}


**Par défaut**, mdu ne suit pas les liens symboliques et rapporte les tailles réelles. Il est possible de modifier son comportement à l'aide des options.

Options: 
* **-L** : Suivre les liens symboliques
* **-b** : Rapporte les tailles apparantes


## Contraintes mdu

### Syntaxe 

L'exécutable suppose que le dernier argument passé dans la commande correspond au nom du pathname.
Toutes les options doivent donc être passées avant !

Exemple : 
./mdu [-L] [-b] {pathname}

L'ordre des options entre elles n'a aucune importance.

### Taille des blocs

On suppose que l'on compare la commande "du" avec l'option définissant la taille des blocs à 512 octets.