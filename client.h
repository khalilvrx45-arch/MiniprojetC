#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED
#include "structures.h"


void afficher_Clients(liste_client* liste_clients);
void ajouter_Client(liste_client** liste_clients);
void modifier_Client(liste_client* liste_clients);
void supprimer_Client(liste_client** liste_clients, liste_commande** liste_commandes, liste_machine* liste_machines);
Client* trouver_client_par_matricule(liste_client* liste_clients, const char* matricule);
int compter_clients(liste_client* liste_clients);
void ajouter_commande_au_client(Client* client, Commande* commande);
int compter_commandes_client(Client* client);
void liberer_commandes_client(Client* client);
void liberer_liste_clients(liste_client** liste_clients);



#endif // CLIENT_H_INCLUDED
