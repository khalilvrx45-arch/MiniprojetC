#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "commande.h"

void afficher_Commandes(Commande commandes[], int nb_commandes) {
    if (nb_commandes == 0) {
        printf("\n=== AUCUNE COMMANDE ENREGISTREE ===\n");
        return;
    }

    printf("\n=============== LISTE DES COMMANDES (%d) ===============\n", nb_commandes);

    for (int i = 0; i < nb_commandes; i++) {
        printf("\n--- Commande #%d ---\n", commandes[i].identifiant);
        
        if (commandes[i].client != NULL) {
            printf("Client: %s (Matricule: %s)\n", 
                   commandes[i].client->nom, 
                   commandes[i].client->matricule);
        } else {
            printf("Client: Non specifie\n");
        }

        if (commandes[i].pokemon_commande != NULL) {
            printf("Pokemon: %s (ID: %d)\n", 
                   commandes[i].pokemon_commande->nom,
                   commandes[i].pokemon_commande->identifiant);
        } else {
            printf("Pokemon: Non specifie\n");
        }

        printf("Quantite: %d\n", commandes[i].quantite);
        
        if (commandes[i].pokemon_commande != NULL) {
            printf("Cout total: %.2f\n", 
                   commandes[i].quantite * commandes[i].pokemon_commande->cout_unitaire);
        }

        char date_str[100];
        strftime(date_str, sizeof(date_str), "%d/%m/%Y %H:%M:%S",
                 localtime(&commandes[i].date_emission));
        printf("Date: %s\n", date_str);

        printf("Etat: ");
        switch (commandes[i].etat) {
            case EN_ATTENTE: printf("En attente\n"); break;
            case EN_COURS: printf("En cours\n"); break;
            case REALISEE: printf("Realisee\n"); break;
            default: printf("Inconnu\n");
        }
    }
    printf("\n====================================================\n");
}

int Afficher_commande_par_id(Commande commandes[], int nb_commandes, int id) {
    for (int i = 0; i < nb_commandes; i++) {
        if (commandes[i].identifiant == id) {
            printf("\n═══════════════════════════════════════\n");
            printf("        COMMANDE ID: %d\n", id);
            printf("═══════════════════════════════════════\n");

            printf("ID: %d\n", commandes[i].identifiant);

            if (commandes[i].client != NULL) {
                printf("Client:\n");
                printf("  Nom: %s\n", commandes[i].client->nom);
                printf("  Matricule: %s\n", commandes[i].client->matricule);
            } else {
                printf("Client: Non specifie\n");
            }

            if (commandes[i].pokemon_commande != NULL) {
                printf("Pokemon commande:\n");
                printf("  Nom: %s\n", commandes[i].pokemon_commande->nom);
                printf("  ID: %d\n", commandes[i].pokemon_commande->identifiant);
                printf("  Cout unitaire: %.2f\n", commandes[i].pokemon_commande->cout_unitaire);
                printf("  Type: %s\n", type_to_string(commandes[i].pokemon_commande->type));
            } else {
                printf("Pokemon: Non specifie\n");
            }

            printf("Quantité: %d\n", commandes[i].quantite);
            printf("Coût total: %.2f\n",
                   commandes[i].pokemon_commande != NULL ?
                   commandes[i].quantite * commandes[i].pokemon_commande->cout_unitaire : 0);

            char date_str[100];
            strftime(date_str, sizeof(date_str), "%A %d %B %Y à %H:%M:%S",
                     localtime(&commandes[i].date_emission));
            printf("Date d'emission: %s\n", date_str);

            printf("Etat: ");
            switch (commandes[i].etat) {
                case EN_ATTENTE: printf("En attente\n"); break;
                case EN_COURS: printf("En cours\n"); break;
                case REALISEE: printf("Realisee\n"); break;
                default: printf("Inconnu\n");
            }

            printf("═══════════════════════════════════════\n");
            return 1;
        }
    }

    printf("Commande avec l'ID %d non trouvee.\n", id);
    return 0;
}

void Ajouter_Commande(Commande commandes[], int *nb_commandes, int capacite, 
                      Client *client, Pokemon *pokemon, int quantite) {
    printf("\n====Ajouter une commande====\n");
    
    if (commandes == NULL || nb_commandes == NULL || client == NULL || pokemon == NULL) {
        printf("Erreur : paramètres invalides\n");
        return;
    }
    
    if (*nb_commandes >= capacite) {
        printf("Erreur : tableau de commandes plein (capacité : %d)\n", capacite);
        return;
    }
    
    if (quantite <= 0) {
        printf("Erreur : quantité invalide %d\n", quantite);
        return;
    }
    
    if (client->matricule[0] == '\0') {
        printf("Erreur : client invalide\n");
        return;
    }
    
    if (pokemon->identifiant <= 0) {
        printf("Erreur : Pokémon invalide\n");
        return;
    }

    Commande nouvelle_commande;
    nouvelle_commande.identifiant = prochain_id_commande;
    prochain_id_commande++;
    nouvelle_commande.client = client;
    nouvelle_commande.pokemon_commande = pokemon;
    nouvelle_commande.quantite = quantite;
    nouvelle_commande.date_emission = time(NULL);
    nouvelle_commande.etat = EN_ATTENTE;
    
    commandes[*nb_commandes] = nouvelle_commande;
    
    int index = *nb_commandes;
    
    if (client->liste_commandes != NULL) {
        if (client->nb_commandes < client->capacite_commandes) {
            client->liste_commandes[client->nb_commandes] = &commandes[index];
            client->nb_commandes++;
        } else {
            printf("Avertissement : client a atteint sa capacité de commandes\n");
        }
    }
    
    (*nb_commandes)++;
    printf("Commande #%d ajoutée avec succès (total: %d)\n",
           commandes[index].identifiant, *nb_commandes);
}

void modifier_Commande(Commande commandes[], int *nb_commandes, int id_commande, 
                       Client *nouveau_client, Pokemon *nouveau_pokemon, 
                       int nouvelle_quantite, EtatCommande nouvel_etat) {
    int index = -1;
    for (int i = 0; i < *nb_commandes; i++) {
        if (commandes[i].identifiant == id_commande) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Commande avec ID %d non trouvée.\n", id_commande);
        return;
    }

    if (nouveau_client != NULL) {
        commandes[index].client = nouveau_client;
    }

    if (nouveau_pokemon != NULL) {
        commandes[index].pokemon_commande = nouveau_pokemon;
    }

    if (nouvelle_quantite > 0) {
        commandes[index].quantite = nouvelle_quantite;
    }

    if (nouvel_etat >= EN_ATTENTE && nouvel_etat <= REALISEE) {
        commandes[index].etat = nouvel_etat;
        
        if (nouvel_etat == REALISEE) {
            printf("Commande %d marquée comme réalisée.\n", id_commande);
        }
    }

    printf("Commande %d modifiée avec succès.\n", id_commande);
}

int supprimer_Commande(Commande commandes[], int *nb_commandes, int id_commande) {
    if (*nb_commandes == 0) {
        printf("Aucune commande à supprimer.\n");
        return 0;
    }

    int index = -1;
    for (int i = 0; i < *nb_commandes; i++) {
        if (commandes[i].identifiant == id_commande) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Commande avec ID %d non trouvée.\n", id_commande);
        return 0;
    }

    if (commandes[index].client != NULL) {
        Client *client = commandes[index].client;

        for (int i = 0; i < client->nb_commandes; i++) {
            if (client->liste_commandes[i]->identifiant == id_commande) {
                for (int j = i; j < client->nb_commandes - 1; j++) {
                    client->liste_commandes[j] = client->liste_commandes[j + 1];
                }
                client->nb_commandes--;
                break;
            }
        }
    }

    for (int i = index; i < *nb_commandes - 1; i++) {
        commandes[i] = commandes[i + 1];
    }

    (*nb_commandes)--;
    printf("Commande %d supprimée avec succès.\n", id_commande);
    return 1;
}
