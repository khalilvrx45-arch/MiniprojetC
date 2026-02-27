#ifndef CLIENT_H
#define CLIENT_H

#include "structures.h"

// Fonctions de recherche et statistiques
Client* trouver_client_par_matricule(liste_client* liste_clients, const char* matricule);
int compter_clients(liste_client* liste_clients);
int compter_commandes_client(Client* client);

// Gestion des relations
void ajouter_commande_au_client(Client* client, Commande* commande);

// Nettoyage memoire
void liberer_commandes_client(Client* client);
void liberer_liste_clients(liste_client** liste_clients);

#endif
