#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commande.h"
#include "client.h" // Ajout de client.h pour ajouter_commande_au_client
#include "machine.h"

float calculer_montant_commande(Commande* commande) {
    if (commande == NULL || commande->pokemon_commande == NULL) {
        return 0.0;
    }

    return commande->quantite * commande->pokemon_commande->cout_unitaire;
}


void afficher_une_Commande(Commande* commande) {
    if (commande == NULL) {
        printf("Commande invalide!\n");
        return;
    }

    printf("ID: %d\n", commande->identifiant);

    // Afficher le client
    if (commande->client != NULL) {
        printf("Client: %s (Matricule: %s)\n",
               commande->client->nom,
               commande->client->matricule);
    } else {
        printf("Client: Non defini\n");
    }

    // ✅ VÉRIFIER LE POKEMON AVANT AFFICHAGE
    if (commande->pokemon_commande != NULL) {
        // Vérifier que le prix est raisonnable (< 10000)
        if (commande->pokemon_commande->cout_unitaire > 0 &&
            commande->pokemon_commande->cout_unitaire < 10000) {

            printf("Pokemon: %s (ID: %d, Prix unitaire: %.2f pieces)\n",
                   commande->pokemon_commande->nom,
                   commande->pokemon_commande->identifiant,
                   commande->pokemon_commande->cout_unitaire);
        } else {
            printf("Pokemon: DONNEES CORROMPUES (prix invalide: %.2f)\n",
                   commande->pokemon_commande->cout_unitaire);
        }
    } else {
        printf("Pokemon: Non defini\n");
    }

    printf("Quantite: %d\n", commande->quantite);

    // Calculer le montant (avec vérification)
    if (commande->pokemon_commande != NULL &&
        commande->pokemon_commande->cout_unitaire > 0 &&
        commande->pokemon_commande->cout_unitaire < 10000) {

        float montant = calculer_montant_commande(commande);
        printf("Montant total: %.2f pieces\n", montant);
    } else {
        printf("Montant total: IMPOSSIBLE A CALCULER (donnees corrompues)\n");
    }

    // Afficher la date
    char date_str[80];
    struct tm* timeinfo = localtime(&(commande->date_emission));
    strftime(date_str, sizeof(date_str), "%d/%m/%Y %H:%M:%S", timeinfo);
    printf("Date emission: %s\n", date_str);

    // Afficher l'état
    printf("Etat: ");
    switch (commande->etat) {
        case EN_ATTENTE: printf("EN ATTENTE\n"); break;
        case EN_COURS: printf("EN COURS\n"); break;
        case REALISEE: printf("REALISEE\n"); break;
        default: printf("INCONNU\n"); break;
    }
}

void afficher_Commandes(liste_commande* liste_commandes) {
    if (liste_commandes == NULL) {
        printf("\n=== AUCUNE COMMANDE ENREGISTREE ===\n");
        return;
    }

    int count = compter_commandes(liste_commandes);
    printf("\n=============== LISTE DES COMMANDES (%d) ===============\n", count);

    liste_commande* courant = liste_commandes;
    int numero = 1;

    while (courant != NULL) {
        printf("\n--- Commande #%d ---\n", numero);
        afficher_une_Commande(&(courant->commande));
        courant = courant->suivant;
        numero++;
    }

    printf("\n====================================================\n");
}


void ajouter_Commande(liste_commande** liste_commandes, Client* client,Pokemon* pokemon, int quantite) {
    // Vérifications
    if (client == NULL) {
        printf("Erreur: Client invalide!\n");
        return;
    }

    if (pokemon == NULL) {
        printf("Erreur: Pokemon invalide!\n");
        return;
    }

    if (quantite <= 0) {
        printf("Erreur: La quantite doit etre positive!\n");
        return;
    }

    //Créer le nouveau nœud
    liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire!\n");
        return;
    }

    Commande* cmd = &(nouveau->commande);

    //Initialisation de la commande
    cmd->identifiant = prochain_id_commande;
    cmd->client = client;
    cmd->pokemon_commande = pokemon;
    cmd->quantite = quantite;
    cmd->date_emission = time(NULL);
    cmd->etat = EN_ATTENTE;

    //Ajout en tête de liste
    nouveau->suivant = *liste_commandes;
    *liste_commandes = nouveau;

    //Ajouter la commande à la liste du client
    ajouter_commande_au_client(client, cmd);

    prochain_id_commande++;

    printf("\nCommande creee avec succes!\n");
    printf("  ID: %d\n", cmd->identifiant);
    printf("  Client: %s\n", client->nom);
    printf("  Pokemon: %s x %d\n", pokemon->nom, quantite);
    printf("  Montant: %.2f pieces\n", calculer_montant_commande(cmd));
}

void changer_etat_commande(Commande* cmd, EtatCommande nouvel_etat) {
    if (cmd != NULL) {
        cmd->etat = nouvel_etat;
    }
}

void modifier_Commande(liste_commande* liste_commandes) {
    if (liste_commandes == NULL) {
        printf("\nAucune commande a modifier!\n");
        return;
    }

    afficher_Commandes(liste_commandes);

    int id;
    printf("\nID de la commande a modifier: ");
    scanf("%d", &id);

    // Rechercher la commande
    Commande* cmd = trouver_commande_par_id(liste_commandes, id);

    if (cmd == NULL) {
        printf("\nCommande ID %d introuvable!\n", id);
        return;
    }

    printf("\n========== MODIFICATION COMMANDE ID: %d ==========\n", id);
    afficher_une_Commande(cmd);

    // Menu de modification
    printf("\nQue voulez-vous modifier?\n");
    printf("1. Quantite\n");
    printf("2. Etat de la commande\n");
    printf("3. Modifier tout\n");
    printf("0. Annuler\n");
    printf("Choix: ");

    int choix;
    scanf("%d", &choix);

    switch (choix) {
        case 1: {
            int nouvelle_quantite;
            printf("Nouvelle quantite (actuelle: %d): ", cmd->quantite);
            scanf("%d", &nouvelle_quantite);

            if (nouvelle_quantite <= 0) {
                printf("Erreur: La quantite doit etre positive!\n");
                break;
            }

            cmd->quantite = nouvelle_quantite;
            printf(" Quantite modifiee!\n");
            printf("  Nouveau montant: %.2f pieces\n", calculer_montant_commande(cmd));
            break;
        }

        case 2: {
            printf("\nEtats disponibles:\n");
            printf("0. EN_ATTENTE\n");
            printf("1. EN_COURS\n");
            printf("2. REALISEE\n");
            printf("Nouvel etat: ");

            int nouvel_etat;
            scanf("%d", &nouvel_etat);

            if (nouvel_etat < 0 || nouvel_etat > 2) {
                printf("Erreur: Etat invalide!\n");
                break;
            }

            changer_etat_commande(cmd, (EtatCommande)nouvel_etat);
            printf("✓ Etat modifie!\n");
            break;
        }

        case 3: {
            // Modifier quantité
            int nouvelle_quantite;
            printf("Nouvelle quantite (actuelle: %d): ", cmd->quantite);
            scanf("%d", &nouvelle_quantite);

            if (nouvelle_quantite > 0) {
                cmd->quantite = nouvelle_quantite;
            }

            // Modifier état
            printf("\nEtats disponibles:\n");
            printf("0. EN_ATTENTE\n");
            printf("1. EN_COURS\n");
            printf("2. REALISEE\n");
            printf("Nouvel etat: ");

            int nouvel_etat;
            scanf("%d", &nouvel_etat);

            if (nouvel_etat >= 0 && nouvel_etat <= 2) {
                changer_etat_commande(cmd, (EtatCommande)nouvel_etat);
            }

            printf("\nCommande entierement modifiee!\n");
            break;
        }

        case 0:
            printf("Modification annulee.\n");
            return;

        default:
            printf("Choix invalide!\n");
            return;
    }

    printf("\n✓ Modifications enregistrees avec succes!\n");
}

void supprimer_Commande(liste_commande** liste_commandes) {
    if (*liste_commandes == NULL) {
        printf("\nAucune commande a supprimer!\n");
        return;
    }

    afficher_Commandes(*liste_commandes);

    int id;
    printf("\nID de la commande a supprimer: ");
    scanf("%d", &id);

    // Recherche et suppression
    liste_commande* courant = *liste_commandes;
    liste_commande* precedent = NULL;

    while (courant != NULL) {
        if (courant->commande.identifiant == id) {
            // Vérification de l'état
            if (courant->commande.etat == EN_COURS) {
                printf("\nAttention: Cette commande est en cours de traitement.\n");
                printf("Voulez-vous vraiment la supprimer? (o/n): ");
                char reponse;
                scanf(" %c", &reponse);

                if (reponse != 'o' && reponse != 'O') {
                    printf("Suppression annulee.\n");
                    return;
                }
            }

            // Confirmation
            printf("\nCommande #%d sera supprimee.\n", id);
            printf("  Client: %s\n", courant->commande.client->nom);
            printf("  Pokemon: %s x %d\n",
                   courant->commande.pokemon_commande->nom,
                   courant->commande.quantite);
            printf("Confirmer? (o/n): ");
            char confirmation;
            scanf(" %c", &confirmation);

            if (confirmation != 'o' && confirmation != 'O') {
                printf("Suppression annulee.\n");
                return;
            }

            // Supprimer le nœud
            if (precedent == NULL) {
                // Suppression en tête
                *liste_commandes = courant->suivant;
            } else {
                // Suppression au milieu ou fin
                precedent->suivant = courant->suivant;
            }

            printf("\n✓ Commande #%d supprimee avec succes!\n", id);
            free(courant);
            return;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nCommande ID %d introuvable!\n", id);
}


Commande* trouver_commande_par_id(liste_commande* liste_commandes, int id) {
    liste_commande* courant = liste_commandes;

    while (courant != NULL) {
        if (courant->commande.identifiant == id) {
            return &(courant->commande);
        }
        courant = courant->suivant;
    }

    return NULL;
}

int compter_commandes(liste_commande* liste_commandes) {
    int count = 0;
    liste_commande* courant = liste_commandes;

    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }

    return count;
}

int compter_commandes_par_etat(liste_commande* liste_commandes, EtatCommande etat) {
    int count = 0;
    liste_commande* courant = liste_commandes;

    while (courant != NULL) {
        if (courant->commande.etat == etat) {
            count++;
        }
        courant = courant->suivant;
    }

    return count;
}

void liberer_liste_commandes(liste_commande** liste_commandes) {
    if (liste_commandes == NULL || *liste_commandes == NULL) {
        return;
    }

    liste_commande* courant = *liste_commandes;
    liste_commande* suivant;
    int count = 0;

    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
        count++;
    }

    *liste_commandes = NULL;
    printf("  -> %d commande(s) liberee(s) de la memoire\n", count);
}

// ✅ NOUVEAU : Supprimer toutes les commandes d'un client spécifique
void supprimer_commandes_par_client(liste_commande** liste_commandes, liste_machine* liste_machines, const char* matricule) {
    if (liste_commandes == NULL || *liste_commandes == NULL || matricule == NULL) return;

    liste_commande* courant = *liste_commandes;
    liste_commande* precedent = NULL;

    while (courant != NULL) {
        if (courant->commande.client != NULL && 
            strcmp(courant->commande.client->matricule, matricule) == 0) {
            
            // On retire d'abord la commande de toutes les files d'attente des machines
            supprimer_commande_de_toutes_files(liste_machines, courant->commande.identifiant);
            
            liste_commande* a_supprimer = courant;
            if (precedent == NULL) {
                *liste_commandes = courant->suivant;
                courant = *liste_commandes;
            } else {
                precedent->suivant = courant->suivant;
                courant = courant->suivant;
            }
            free(a_supprimer);
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }
}
