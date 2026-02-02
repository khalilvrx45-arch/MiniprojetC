#include "fichiers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fonction de chargement depuis kes fichiers
// Charger les Pokemons depuis Pokemon.txt
void charger_pokemons_depuis_fichier(liste_pokemon** liste) {
    FILE* fichier = fopen(FICHIER_POKEMONS, "r");

    if (fichier == NULL) {
        printf("Fichier '%s' introuvable. Demarrage avec une liste vide.\n",
               FICHIER_POKEMONS);
        return;
    }

    printf("Chargement des Pokemons depuis '%s'...\n", FICHIER_POKEMONS);

    int count = 0;
    Pokemon pokemon_temp;
    int type_temp;

    while (fscanf(fichier, "%d|%49[^|]|%f|%d\n",
                  &pokemon_temp.identifiant,
                  pokemon_temp.nom,
                  &pokemon_temp.cout_unitaire,
                  &type_temp) == 4) {

        pokemon_temp.type = (TypePokemon)type_temp;

        // Créer un nouveau nœud
        liste_pokemon* nouveau = (liste_pokemon*)malloc(sizeof(liste_pokemon));
        if (nouveau == NULL) {
            printf("Erreur d'allocation memoire!\n");
            fclose(fichier);
            return;
        }

        nouveau->pokemon = pokemon_temp;
        nouveau->suivant = *liste;
        *liste = nouveau;

        count++;

        // Mettre à jour le prochain ID
        if (pokemon_temp.identifiant >= prochain_id_pokemon) {
            prochain_id_pokemon = pokemon_temp.identifiant + 1;
        }
    }

    fclose(fichier);
    printf("  -> %d Pokemon(s) charge(s)\n", count);
}

// Charger les Clients depuis clients.txt
void charger_clients_depuis_fichier(liste_client** liste) {
    FILE* fichier = fopen(FICHIER_CLIENTS, "r");

    if (fichier == NULL) {
        printf("Fichier '%s' introuvable. Demarrage avec une liste vide.\n",
               FICHIER_CLIENTS);
        return;
    }

    printf("Chargement des Clients depuis '%s'...\n", FICHIER_CLIENTS);

    int count = 0;
    Client client_temp;

    while (fscanf(fichier, "%19[^|]|%99[^\n]\n",
                  client_temp.matricule,
                  client_temp.nom) == 2) {

        // Créer un nouveau nœud
        liste_client* nouveau = (liste_client*)malloc(sizeof(liste_client));
        if (nouveau == NULL) {
            printf("Erreur d'allocation memoire!\n");
            fclose(fichier);
            return;
        }

        nouveau->client = client_temp;
        nouveau->client.liste_commandes = NULL;  // Les commandes seront chargées après
        nouveau->suivant = *liste;
        *liste = nouveau;

        count++;
    }

    fclose(fichier);
    printf("  -> %d Client(s) charge(s)\n", count);
}

// Charger les Machines depuis machines.txt
void charger_machines_depuis_fichier(liste_machine** liste, liste_pokemon* liste_pokemons) {
    FILE* fichier = fopen(FICHIER_MACHINES, "r");

    if (fichier == NULL) {
        printf("Fichier '%s' introuvable. Demarrage avec une liste vide.\n",
               FICHIER_MACHINES);
        return;
    }

    printf("Chargement des Machines depuis '%s'...\n", FICHIER_MACHINES);

    int count = 0;
    Machine machine_temp;
    int id_pokemon;
    int etat_temp;

    while (fscanf(fichier, "%d|%49[^|]|%d|%d|%d|%d|%f|%d\n",
                  &machine_temp.identifiant,
                  machine_temp.nom,
                  &id_pokemon,
                  &machine_temp.temps_production,
                  &machine_temp.max_figurines_avant_maintenance,
                  &machine_temp.compteur_figurines,
                  &machine_temp.cout_maintenance,
                  &etat_temp) == 8) {

        machine_temp.etat = (EtatMachine)etat_temp;

        // Trouver le Pokemon correspondant
        machine_temp.pokemon_produit = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
        if (machine_temp.pokemon_produit == NULL && id_pokemon != -1) {
            printf("Attention: Pokemon ID %d non trouve pour la machine '%s'\n",
                   id_pokemon, machine_temp.nom);
        }

        // Initialiser les champs qui ne sont pas dans le fichier
        machine_temp.commande_en_cours = NULL;
        machine_temp.date_disponibilite = 0;

        // Créer un nouveau noeud
        liste_machine* nouveau = (liste_machine*)malloc(sizeof(liste_machine));
        if (nouveau == NULL) {
            printf("Erreur d'allocation memoire!\n");
            fclose(fichier);
            return;
        }

        nouveau->machine = machine_temp;
        nouveau->suivant = *liste;
        *liste = nouveau;

        count++;

        // Mettre à jour le prochain ID
        if (machine_temp.identifiant >= prochain_id_machine) {
            prochain_id_machine = machine_temp.identifiant + 1;
        }
    }

    fclose(fichier);
    printf("  -> %d Machine(s) chargee(s)\n", count);
}

// Charger les Commandes depuis
void charger_commandes_depuis_fichier(liste_commande** liste, liste_client* liste_clients,
                                      liste_pokemon* liste_pokemons) {
    FILE* fichier = fopen(FICHIER_COMMANDES, "r");

    if (fichier == NULL) {
        printf("Fichier '%s' introuvable. Demarrage avec une liste vide.\n",
               FICHIER_COMMANDES);
        return;
    }

    printf("Chargement des Commandes depuis '%s'...\n", FICHIER_COMMANDES);

    int count = 0;
    Commande commande_temp;
    char matricule_client[20];
    int id_pokemon;
    int etat_temp;
    long date_temp;


    while (fscanf(fichier, "%d|%19[^|]|%d|%d|%ld|%d\n",
                  &commande_temp.identifiant,
                  matricule_client,
                  &id_pokemon,
                  &commande_temp.quantite,
                  &date_temp,
                  &etat_temp) == 6) {

        commande_temp.date_emission = (time_t)date_temp;
        commande_temp.etat = (EtatCommande)etat_temp;

        // Trouver le client correspondant
        commande_temp.client = trouver_client_par_matricule(liste_clients, matricule_client);
        if (commande_temp.client == NULL) {
            printf("Attention: Client '%s' non trouve pour la commande ID %d\n",
                   matricule_client, commande_temp.identifiant);
            continue;  // Ignorer cette commande
        }

        // Trouver le Pokemon correspondant
        commande_temp.pokemon_commande = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
        if (commande_temp.pokemon_commande == NULL) {
            printf("Attention: Pokemon ID %d non trouve pour la commande ID %d\n",
                   id_pokemon, commande_temp.identifiant);
            continue;  // Ignorer cette commande
        }

        // Creation d'un nouveau noued
        liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
        if (nouveau == NULL) {
            printf("Erreur d'allocation memoire!\n");
            fclose(fichier);
            return;
        }

        nouveau->commande = commande_temp;
        nouveau->suivant = *liste;
        *liste = nouveau;

        // Ajouter aussi dans la liste de commandes du client
        liste_commande* nouveau_client = (liste_commande*)malloc(sizeof(liste_commande));
        if (nouveau_client != NULL) {
            nouveau_client->commande = commande_temp;
            nouveau_client->suivant = commande_temp.client->liste_commandes;
            commande_temp.client->liste_commandes = nouveau_client;
        }

        count++;

        // Mettre à jour le prochain identifiant
        if (commande_temp.identifiant >= prochain_id_commande) {
            prochain_id_commande = commande_temp.identifiant + 1;
        }
    }

    fclose(fichier);
    printf("  -> %d Commande(s) chargee(s)\n", count);
}


//fonctions de sauvgarde dans les fichiers


void sauvegarder_pokemons_dans_fichier(liste_pokemon* liste) {
    FILE* fichier = fopen(FICHIER_POKEMONS, "w");

    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier '%s' en ecriture!\n",
               FICHIER_POKEMONS);
        return;
    }

    printf("Sauvegarde des Pokemons dans '%s'...\n", FICHIER_POKEMONS);

    int count = 0;
    liste_pokemon* courant = liste;

    while (courant != NULL) {
        fprintf(fichier, "%d|%s|%.2f|%d\n",
                courant->pokemon.identifiant,
                courant->pokemon.nom,
                courant->pokemon.cout_unitaire,
                (int)courant->pokemon.type);

        count++;
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("  -> %d Pokemon(s) sauvegarde(s)\n", count);
}

void sauvegarder_clients_dans_fichier(liste_client* liste) {
    FILE* fichier = fopen(FICHIER_CLIENTS, "w");

    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier '%s' en ecriture!\n",
               FICHIER_CLIENTS);
        return;
    }

    printf("Sauvegarde des Clients dans '%s'...\n", FICHIER_CLIENTS);

    int count = 0;
    liste_client* courant = liste;

    while (courant != NULL) {
        fprintf(fichier, "%s|%s\n",
                courant->client.matricule,
                courant->client.nom);

        count++;
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("  -> %d Client(s) sauvegarde(s)\n", count);
}

void sauvegarder_machines_dans_fichier(liste_machine* liste) {
    FILE* fichier = fopen(FICHIER_MACHINES, "w");

    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier '%s' en ecriture!\n",
               FICHIER_MACHINES);
        return;
    }

    printf("Sauvegarde des Machines dans '%s'...\n", FICHIER_MACHINES);

    int count = 0;
    liste_machine* courant = liste;

    while (courant != NULL) {
        // Si pas de pokemon produit, utiliser -1 comme ID
        int id_pokemon = (courant->machine.pokemon_produit != NULL)
                         ? courant->machine.pokemon_produit->identifiant
                         : -1;

        fprintf(fichier, "%d|%s|%d|%d|%d|%d|%.2f|%d\n",
                courant->machine.identifiant,
                courant->machine.nom,
                id_pokemon,
                courant->machine.temps_production,
                courant->machine.max_figurines_avant_maintenance,
                courant->machine.compteur_figurines,
                courant->machine.cout_maintenance,
                (int)courant->machine.etat);

        count++;
        courant = courant->suivant;
    }

    fclose(fichier);
    printf("  -> %d Machine(s) sauvegardee(s)\n", count);
}

void sauvegarder_commandes_dans_fichier(liste_commande* liste) {
    FILE* fichier = fopen(FICHIER_COMMANDES, "w");

    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier '%s' en ecriture!\n",
               FICHIER_COMMANDES);
        return;
    }

    printf("Sauvegarde des Commandes dans '%s'...\n", FICHIER_COMMANDES);

    int count = 0;
    liste_commande* courant = liste;

    while (courant != NULL) {

        if (courant->commande.client != NULL &&
            courant->commande.pokemon_commande != NULL) {

            fprintf(fichier, "%d|%s|%d|%d|%ld|%d\n",
                    courant->commande.identifiant,
                    courant->commande.client->matricule,
                    courant->commande.pokemon_commande->identifiant,
                    courant->commande.quantite,
                    (long)courant->commande.date_emission,
                    (int)courant->commande.etat);

            count++;
        }

        courant = courant->suivant;
    }

    fclose(fichier);
    printf("  -> %d Commande(s) sauvegardee(s)\n", count);
}


