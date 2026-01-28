#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "structures.h"

#define MAX_CLIENTS 100

// Client function prototypes
void afficher_Clients(Client clients[], int nbClients);
void ajouter_Client(Client clients[], int* nbClients);
void modifier_Client(Client clients[], int nbClients);
void supprimer_Client(Client clients[], int* nbClients);

#endif // CLIENT_H_INCLUDED
