#ifndef COMMANDE_H_INCLUDED
#define COMMANDE_H_INCLUDED
#include "structures.h"

void afficher_Commandes(liste_commande* liste_commandes);
void ajouter_Commande(liste_commande** liste_commandes, Client* client,Pokemon* pokemon, int quantite);
void modifier_Commande(liste_commande* liste_commandes);
void supprimer_Commande(liste_commande** liste_commandes);
Commande* trouver_commande_par_id(liste_commande* liste_commandes, int id);
int compter_commandes(liste_commande* liste_commandes);
int compter_commandes_par_etat(liste_commande* liste_commandes, EtatCommande etat);
void liberer_liste_commandes(liste_commande** liste_commandes);

// Nouvelles fonctions rajoutees
void afficher_une_Commande(Commande* commande);
float calculer_montant_commande(Commande* commande);
void changer_etat_commande(Commande* cmd, EtatCommande nouvel_etat);
void supprimer_commandes_par_client(liste_commande** liste_commandes, liste_machine* liste_machines, const char* matricule);

#endif // COMMANDE_H_INCLUDED
