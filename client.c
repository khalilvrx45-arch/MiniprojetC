#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

void afficher_Clients(liste_client* liste_clients) {
    if (liste_clients == NULL) {
        printf("\n=== AUCUN CLIENT ENREGISTRE ===\n");
        return;
    }

    int count = compter_clients(liste_clients);
    printf("\n=============== LISTE DES CLIENTS (%d) ===============\n", count);

    liste_client* courant = liste_clients;
    int numero = 1;

    while (courant != NULL) {
        printf("\n--- Client #%d ---\n", numero);
        printf("Matricule: %s\n", courant->client.matricule);
        printf("Nom: %s\n", courant->client.nom);

        // Afficher le nombre de commandes du client
        int nb_commandes = compter_commandes_client(&(courant->client));
        printf("Nombre de commandes: %d\n", nb_commandes);

        courant = courant->suivant;
        numero++;
    }

    printf("\n====================================================\n");
}

void ajouter_Client(liste_client** liste_clients) {

    liste_client* nouveau = (liste_client*)malloc(sizeof(liste_client));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire!\n");
        return;
    }

    Client* c = &(nouveau->client);

    printf("\n========== AJOUT D'UN CLIENT ==========\n");

    // Vider le buffer
    while (getchar() != '\n');

    // Saisie du matricule
    printf("Matricule du client: ");
    scanf("%19[^\n]", c->matricule);
    while (getchar() != '\n');

    // Vérifier si le matricule existe déjà
    if (trouver_client_par_matricule(*liste_clients, c->matricule) != NULL) {
        printf("Erreur: Un client avec le matricule '%s' existe deja!\n", c->matricule);
        free(nouveau);
        return;
    }

    // Saisie du nom
    printf("Nom du client: ");
    scanf("%99[^\n]", c->nom);

    // Initialisation de la liste de commandes
    c->liste_commandes = NULL;

    // Ajout en tête de liste
    nouveau->suivant = *liste_clients;
    *liste_clients = nouveau;

    printf("\nClient '%s' ajoute avec succes! (Matricule: %s)\n", c->nom, c->matricule);
}

void modifier_Client(liste_client* liste_clients) {
    if (liste_clients == NULL) {
        printf("\nAucun client a modifier!\n");
        return;
    }

    afficher_Clients(liste_clients);

    char matricule[20];
    printf("\nMatricule du client a modifier: ");
    scanf("%19s", matricule);

    // Rechercher le client
    Client* c = trouver_client_par_matricule(liste_clients, matricule);

    if (c == NULL) {
        printf("\nClient avec le matricule '%s' introuvable!\n", matricule);
        return;
    }

    printf("\n========== MODIFICATION CLIENT '%s' ==========\n", c->matricule);
    printf("Client actuel: %s\n", c->nom);

    // Menu de modification
    printf("\nQue voulez-vous modifier?\n");
    printf("1. Nom\n");
    printf("2. Matricule\n");
    printf("3. Modifier tout\n");
    printf("0. Annuler\n");
    printf("Choix: ");

    int choix;
    scanf("%d", &choix);

    // Vider le buffer
    while (getchar() != '\n');

    switch (choix) {
        case 1:
            printf("Nouveau nom (actuel: %s): ", c->nom);
            scanf("%99[^\n]", c->nom);
            printf("✓ Nom modifie!\n");
            break;

        case 2: {
            char nouveau_matricule[20];
            printf("Nouveau matricule (actuel: %s): ", c->matricule);
            scanf("%19s", nouveau_matricule);

            // Vérifier si le nouveau matricule existe déjà
            if (strcmp(nouveau_matricule, c->matricule) != 0) {
                if (trouver_client_par_matricule(liste_clients, nouveau_matricule) != NULL) {
                    printf("Erreur: Ce matricule existe deja!\n");
                    break;
                }
                strcpy(c->matricule, nouveau_matricule);
                printf("Matricule modifie!\n");
            } else {
                printf("Le matricule n'a pas change.\n");
            }
            break;
        }

        case 3: {
            printf("Nouveau nom (actuel: %s): ", c->nom);
            scanf("%99[^\n]", c->nom);
            while (getchar() != '\n');

            char nouveau_matricule[20];
            printf("Nouveau matricule (actuel: %s): ", c->matricule);
            scanf("%19s", nouveau_matricule);

            // Vérifier si le nouveau matricule existe déjà
            if (strcmp(nouveau_matricule, c->matricule) != 0) {
                if (trouver_client_par_matricule(liste_clients, nouveau_matricule) != NULL) {
                    printf("Erreur: Ce matricule existe deja! Matricule non modifie.\n");
                } else {
                    strcpy(c->matricule, nouveau_matricule);
                }
            }

            printf("\nClient entierement modifie!\n");
            break;
        }

        case 0:
            printf("Modification annulee.\n");
            return;

        default:
            printf("Choix invalide!\n");
            return;
    }

    printf("\nModifications enregistrees avec succes!\n");
}

void supprimer_Client(liste_client** liste_clients) {
    if (*liste_clients == NULL) {
        printf("\nAucun client a supprimer!\n");
        return;
    }

    afficher_Clients(*liste_clients);

    char matricule[20];
    printf("\nMatricule du client a supprimer: ");
    scanf("%19s", matricule);

    // Recherche et suppression
    liste_client* courant = *liste_clients;
    liste_client* precedent = NULL;

    while (courant != NULL) {
        if (strcmp(courant->client.matricule, matricule) == 0) {
            // Vérifier s'il a des commandes
            int nb_commandes = compter_commandes_client(&(courant->client));

            if (nb_commandes > 0) {
                printf("\nAttention: Ce client a %d commande(s).\n", nb_commandes);
                printf("Voulez-vous vraiment supprimer ce client? (o/n): ");
                char reponse;
                scanf(" %c", &reponse);

                if (reponse != 'o' && reponse != 'O') {
                    printf("Suppression annulee.\n");
                    return;
                }
            }

            // Confirmation
            printf("\nClient '%s' (Matricule: %s) sera supprime.\n",
                   courant->client.nom, matricule);
            printf("Confirmer? (o/n): ");
            char confirmation;
            scanf(" %c", &confirmation);

            if (confirmation != 'o' && confirmation != 'O') {
                printf("Suppression annulee.\n");
                return;
            }

            // Libérer les commandes du client
            liberer_commandes_client(&(courant->client));

            // Supprimer le nœud
            if (precedent == NULL) {
                // Suppression en tête
                *liste_clients = courant->suivant;
            } else {
                // Suppression au milieu ou fin
                precedent->suivant = courant->suivant;
            }

            printf("\n✓ Client '%s' supprime avec succes!\n", courant->client.nom);
            free(courant);
            return;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nClient avec le matricule '%s' introuvable!\n", matricule);
}


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
