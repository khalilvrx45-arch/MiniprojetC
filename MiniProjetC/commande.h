#ifndef COMMANDE_H_INCLUDED
#define COMMANDE_H_INCLUDED

#include "structures.h"

#define MAX_COMMANDES 200

// Commande function prototypes
void afficher_Commandes(Commande commandes[], int nb_commandes);
int Afficher_commande_par_id(Commande commandes[], int nb_commandes, int id);
void Ajouter_Commande(Commande commandes[], int *nb_commandes, int capacite, 
                      Client *client, Pokemon *pokemon, int quantite);
void modifier_Commande(Commande commandes[], int *nb_commandes, int id_commande, 
                       Client *nouveau_client, Pokemon *nouveau_pokemon, 
                       int nouvelle_quantite, EtatCommande nouvel_etat);
int supprimer_Commande(Commande commandes[], int *nb_commandes, int id_commande);

#endif // COMMANDE_H_INCLUDED
