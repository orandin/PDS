# TP: 7
## PDS - L3 info - Groupe 4 

Réalisé par:
* Simon DELBERGHE


## Description

### Options
Le mshell accepte l'option :
* -v (verbose)

### Utilisation
	`./mshell (-v)`


### Ce qui fonctionne
* kill pid ou job
* CTRL + Z
* CTRL + C
* bg
* fg
* pipe

### Ce qui ne fonctionne pas

Lorsque l'on se sert du pipe, le kill sur un des processus du job ne fonctionne pas depuis le mshell courant.
On doit se servir d'un autre shell pour envoyer un signal. 


## Notes

### Note 1:

À partir de la ligne 72 à 121 du fichier SESSION.txt, l'envoi de message (kill) a été fait dans un autre shell *( voir ce qui ne fonctionne pas pour comprendre )*.
On ne voit dans la trace que le résultat de la réception de ces messages. 
