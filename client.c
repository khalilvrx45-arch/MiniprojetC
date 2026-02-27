#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "commande.h"

Client* trouver_client_par_matricule(liste_client* liste_clients, const char* matricule) {
    liste_client* courant = liste_clients;

    while (courant != NULL) {
        if (strcmp(courant->client.matricule, matricule) == 0) {
            return &(courant->client);
        }
        courant = courant->suivant;
    }

    return NULL;
}

int compter_clients(liste_client* liste_clients) {
    int count = 0;
    liste_client* courant = liste_clients;

    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }

    return count;
}

void ajouter_commande_au_client(Client* client, Commande* commande) {
    if (client == NULL || commande == NULL) {
        printf("Erreur: Client ou commande invalide!\n");
        return;
    }

    // Créer un nouveau nœud pour la liste de commandes du client
    liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire pour la commande du client!\n");
        return;
    }

    // Copier la commande (pas de pointeur, on stocke la structure complète)
    nouveau->commande = *commande;

    // Ajouter en tête de la liste de commandes du client
    nouveau->suivant = client->liste_commandes;
    client->liste_commandes = nouveau;
}

int compter_commandes_client(Client* client) {
    if (client == NULL) {
        return 0;
    }

    int count = 0;
    liste_commande* courant = client->liste_commandes;

    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }

    return count;
}

void liberer_commandes_client(Client* client) {
    if (client == NULL || client->liste_commandes == NULL) {
        return;
    }

    liste_commande* courant = client->liste_commandes;
    liste_commande* suivant;

    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }

    client->liste_commandes = NULL;
}

void liberer_liste_clients(liste_client** liste_clients) {
    if (liste_clients == NULL || *liste_clients == NULL) {
        return;
    }

    liste_client* courant = *liste_clients;
    liste_client* suivant;
    int count = 0;

    while (courant != NULL) {
        suivant = courant->suivant;

        // Libérer les commandes du client
        liberer_commandes_client(&(courant->client));

        // Libérer le client
        free(courant);
        courant = suivant;
        count++;
    }

    *liste_clients = NULL;
    printf("  -> %d client(s) libere(s) de la memoire\n", count);
}
