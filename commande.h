#ifndef COMMANDE_H
#define COMMANDE_H

#include "structures.h"

// Recherche et statistiques
Commande* trouver_commande_par_id(liste_commande* liste_commandes, int id);
int compter_commandes(liste_commande* liste_commandes);
int compter_commandes_par_etat(liste_commande* liste_commandes, EtatCommande etat);
float calculer_montant_commande(Commande* commande);

// Gestion des etats
void changer_etat_commande(Commande* cmd, EtatCommande nouvel_etat);

// Nettoyage et cascade
void liberer_liste_commandes(liste_commande** liste_commandes);
void supprimer_commandes_par_client(liste_commande** liste_commandes, liste_machine* liste_machines, const char* matricule);

#endif
