#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

void afficher_Clients(Client clients[], int nbClients) {
    if (nbClients == 0) {
        printf("\n=== AUCUN CLIENT ENREGISTRE ===\n");
        return;
    }

    printf("\n=============== LISTE DES CLIENTS (%d) ===============\n", nbClients);

    for (int i = 0; i < nbClients; i++) {
        printf("\n--- Client #%d ---\n", i + 1);
        printf("Matricule: %s\n", clients[i].matricule);
        printf("Nom: %s\n", clients[i].nom);
        printf("Nombre de commandes: %d\n", clients[i].nb_commandes);
        printf("Capacite commandes: %d\n", clients[i].capacite_commandes);
    }
    printf("\n====================================================\n");
}

void ajouter_Client(Client clients[], int* nbClients) {
    if (*nbClients >= MAX_CLIENTS) {
        printf("\nErreur: Nombre maximum de clients atteint!\n");
        return;
    }

    Client nouveau_client;

    printf("\n========== AJOUT D'UN CLIENT ==========\n");

    // Vider le buffer
    while (getchar() != '\n');

    printf("Matricule du client: ");
    scanf("%19[^\n]", nouveau_client.matricule);

    // Vérifier si le matricule existe déjà
    for (int i = 0; i < *nbClients; i++) {
        if (strcmp(clients[i].matricule, nouveau_client.matricule) == 0) {
            printf("\nErreur: Ce matricule existe deja!\n");
            return;
        }
    }

    // Vider le buffer
    while (getchar() != '\n');

    printf("Nom du client: ");
    scanf("%99[^\n]", nouveau_client.nom);

    // Initialiser la capacité des commandes
    printf("Capacite initiale de commandes (recommande: 10): ");
    scanf("%d", &nouveau_client.capacite_commandes);

    if (nouveau_client.capacite_commandes <= 0) {
        nouveau_client.capacite_commandes = 10;
    }

    // Allouer de la mémoire pour la liste de commandes
    nouveau_client.liste_commandes = (Commande**)malloc(nouveau_client.capacite_commandes * sizeof(Commande*));

    if (nouveau_client.liste_commandes == NULL) {
        printf("\nErreur: Allocation memoire echouee!\n");
        return;
    }

    nouveau_client.nb_commandes = 0;

    clients[*nbClients] = nouveau_client;
    (*nbClients)++;

    printf("\nClient ajoute avec succes!\n");
}

void modifier_Client(Client clients[], int nbClients) {
    if (nbClients == 0) {
        printf("\nAucun client a modifier!\n");
        return;
    }

    char matricule[20];
    printf("\nMatricule du client a modifier: ");
    scanf("%19s", matricule);

    // Rechercher le client
    int index = -1;
    for (int i = 0; i < nbClients; i++) {
        if (strcmp(clients[i].matricule, matricule) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nClient introuvable!\n");
        return;
    }

    printf("\n========== MODIFICATION CLIENT: %s ==========\n", matricule);

    int choix;
    do {
        printf("\nQue souhaitez-vous modifier?\n");
        printf("1. Nom (actuel: %s)\n", clients[index].nom);
        printf("2. Matricule (actuel: %s)\n", clients[index].matricule);
        printf("3. Retour\n");
        printf("Votre choix (1-3): ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                // Vider le buffer
                while (getchar() != '\n');
                printf("Nouveau nom: ");
                scanf("%99[^\n]", clients[index].nom);
                printf("Nom modifie avec succes!\n");
                break;

            case 2:
                printf("Nouveau matricule: ");
                char nouveau_matricule[20];
                scanf("%19s", nouveau_matricule);

                // Vérifier si le nouveau matricule existe déjà
                int existe = 0;
                for (int i = 0; i < nbClients; i++) {
                    if (i != index && strcmp(clients[i].matricule, nouveau_matricule) == 0) {
                        existe = 1;
                        break;
                    }
                }

                if (existe) {
                    printf("Ce matricule existe deja!\n");
                } else {
                    strcpy(clients[index].matricule, nouveau_matricule);
                    printf("Matricule modifie avec succes!\n");
                }
                break;

            case 3:
                printf("Retour au menu...\n");
                break;

            default:
                printf("Choix invalide!\n");
        }
    } while (choix != 3);
}

void supprimer_Client(Client clients[], int* nbClients) {
    if (*nbClients == 0) {
        printf("\nAucun client a supprimer!\n");
        return;
    }

    char matricule[20];
    printf("\nMatricule du client a supprimer: ");
    scanf("%19s", matricule);

    // Rechercher le client
    int index = -1;
    for (int i = 0; i < *nbClients; i++) {
        if (strcmp(clients[i].matricule, matricule) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nClient introuvable!\n");
        return;
    }

    printf("\nClient '%s' (Matricule: %s) sera supprime.\n",
           clients[index].nom, matricule);

    // Libérer la mémoire allouée pour la liste de commandes
    if (clients[index].liste_commandes != NULL) {
        free(clients[index].liste_commandes);
    }

    // Décalage des éléments
    for (int i = index; i < *nbClients - 1; i++) {
        clients[i] = clients[i + 1];
    }

    (*nbClients)--;
    printf("\nClient supprime avec succes!\n");
}
