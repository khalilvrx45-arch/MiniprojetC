#include "fichiers.h"
#include "pokemon.h"
#include "client.h"
#include "machine.h"
#include "commande.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ✅ NOUVELLE FONCTION : Restaurer les pointeurs commande_en_cours ET la file d'attente
void restaurer_commandes_en_cours(liste_machine* liste_machines, liste_commande* liste_commandes) {
    // Relire le fichier machines.txt pour récupérer les IDs des commandes
    FILE* fichier = fopen(FICHIER_MACHINES, "r");

    if (fichier == NULL) {
        return;
    }

    printf("Restauration des commandes et files d'attente...\n");

    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int id_machine, id_commande_en_cours;
        char IDs_file[512] = "";

        // On parse la ligne manuellement pour plus de flexibilité avec la file
        // Format attendu: ID|Nom|IDPokemon|Temps|Max|Compteur|Cout|Etat|Date|IDEnCours|[IDS_FILE]
        char* token = strtok(ligne, "|");
        if (!token) continue;
        id_machine = atoi(token);

        // Sauter jusqu'au 10ème champ (IDEnCours)
        // Fields: ID|Nom|IDPoke|Time|Max|Compteur|Total|Cout|Etat|Date|IDEnCours
        for (int i = 0; i < 9; i++) strtok(NULL, "|");
        
        token = strtok(NULL, "|");
        if (!token) continue;
        id_commande_en_cours = atoi(token);

        token = strtok(NULL, "\n");
        if (token) strcpy(IDs_file, token);

        Machine* machine = trouver_machine_par_id(liste_machines, id_machine);
        if (machine == NULL) continue;

        // 1. Restaurer commande en cours
        if (id_commande_en_cours != -1) {
            machine->commande_en_cours = trouver_commande_par_id(liste_commandes, id_commande_en_cours);
        }

        // 2. Restaurer file d'attente
        if (strlen(IDs_file) > 0) {
            char* id_token = strtok(IDs_file, ",");
            while (id_token) {
                int id_ptr = atoi(id_token);
                Commande* c = trouver_commande_par_id(liste_commandes, id_ptr);
                if (c) {
                    ajouter_a_file(machine, c);
                }
                id_token = strtok(NULL, ",");
            }
        }
    }

    fclose(fichier);
}
// ============================================================================
// FONCTIONS DE CHARGEMENT DEPUIS LES FICHIERS
// ============================================================================

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

    // Format: identifiant|nom|cout_unitaire|type
    // Exemple: 1|Pikachu|25.50|0
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

    // Format: matricule|nom
    // Exemple: C001|John Doe
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
        printf("Fichier '%s' introuvable. Demarrage avec une liste vide.\n", FICHIER_MACHINES);
        return;
    }

    printf("Chargement des Machines...\n");

    char ligne[1024];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        Machine machine_temp;
        int id_pokemon, etat_temp;
        long date_temp;
        
        // On lit les paramètres fixes
        if (sscanf(ligne, "%d|%49[^|]|%d|%d|%d|%d|%d|%f|%d|%ld|%d",
                   &machine_temp.identifiant, machine_temp.nom, &id_pokemon,
                   &machine_temp.temps_production, &machine_temp.max_figurines_avant_maintenance,
                   &machine_temp.compteur_figurines, &machine_temp.total_produit, &machine_temp.cout_maintenance,
                   &etat_temp, &date_temp, &machine_temp.temps_maintenance) >= 11) {

            machine_temp.etat = (EtatMachine)etat_temp;
            machine_temp.date_disponibilite = (time_t)date_temp;
            machine_temp.pokemon_produit = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
            machine_temp.commande_en_cours = NULL;
            machine_temp.file_attente = NULL;

            liste_machine* nouveau = (liste_machine*)malloc(sizeof(liste_machine));
            if (nouveau) {
                nouveau->machine = machine_temp;
                nouveau->suivant = *liste;
                *liste = nouveau;
                if (machine_temp.identifiant >= prochain_id_machine) prochain_id_machine = machine_temp.identifiant + 1;
            }
        }
    }
    fclose(fichier);
}
// Charger les Commandes depuis commandes.txt
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

        // ✅ DEBUG : Afficher ce qui est chargé
        printf("  Chargement commande ID %d : client=%s, pokemon_id=%d, qte=%d\n",
               commande_temp.identifiant, matricule_client, id_pokemon, commande_temp.quantite);

        // Trouver le client
        commande_temp.client = trouver_client_par_matricule(liste_clients, matricule_client);
        if (commande_temp.client == NULL) {
            printf("  ERREUR: Client '%s' non trouve!\n", matricule_client);
            continue;
        }

        // ✅ Trouver le Pokemon
        commande_temp.pokemon_commande = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
        if (commande_temp.pokemon_commande == NULL) {
            printf("  ERREUR: Pokemon ID %d non trouve!\n", id_pokemon);
            continue;
        }

        // ✅ DEBUG : Vérifier le Pokemon trouvé
        printf("  Pokemon trouve: %s (prix: %.2f)\n",
               commande_temp.pokemon_commande->nom,
               commande_temp.pokemon_commande->cout_unitaire);

        // Créer le nœud
        liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
        if (nouveau == NULL) {
            printf("Erreur d'allocation memoire!\n");
            fclose(fichier);
            return;
        }

        nouveau->commande = commande_temp;
        nouveau->suivant = *liste;
        *liste = nouveau;

        // ✅ Ajouter la commande à la liste du client
        ajouter_commande_au_client(commande_temp.client, &(nouveau->commande));

        count++;

        if (commande_temp.identifiant >= prochain_id_commande) {
            prochain_id_commande = commande_temp.identifiant + 1;
        }
    }

    fclose(fichier);
    printf("  -> %d Commande(s) chargee(s)\n", count);
}
// Charger la caisse depuis caisse.txt
void charger_caisse_depuis_fichier(Caisse* caisse) {
    FILE* fichier = fopen(FICHIER_CAISSE, "r");

    if (fichier == NULL) {
        printf("Fichier '%s' introuvable. Initialisation avec 5000 pieces.\n",
               FICHIER_CAISSE);
        caisse->solde = 5000.0;  // Valeur par défaut
        return;
    }

    printf("Chargement de la caisse depuis '%s'...\n", FICHIER_CAISSE);

    // Format simple: une seule ligne avec le solde
    if (fscanf(fichier, "%f", &caisse->solde) == 1) {
        printf("  -> Caisse chargee: %.2f pieces\n", caisse->solde);
    } else {
        printf("Erreur de lecture. Initialisation avec 5000 pieces.\n");
        caisse->solde = 5000.0;
    }

    fclose(fichier);
}

// ============================================================================
// FONCTIONS DE SAUVEGARDE DANS LES FICHIERS
// ============================================================================

// Sauvegarder les Pokemons dans Pokemon.txt
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

// Sauvegarder les Clients dans clients.txt
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

// Sauvegarder les Machines dans machines.txt
void sauvegarder_machines_dans_fichier(liste_machine* liste) {
    FILE* fichier = fopen(FICHIER_MACHINES, "w");
    if (fichier == NULL) return;

    printf("Sauvegarde des Machines (et files d'attente) dans '%s'...\n", FICHIER_MACHINES);

    liste_machine* courant = liste;
    while (courant != NULL) {
        Machine* m = &(courant->machine);
        int id_pokemon = (m->pokemon_produit) ? m->pokemon_produit->identifiant : -1;
        int id_cmd = (m->commande_en_cours) ? m->commande_en_cours->identifiant : -1;

        // Écrire les infos de base
        fprintf(fichier, "%d|%s|%d|%d|%d|%d|%d|%.2f|%d|%ld|%d|%d|",
                m->identifiant, m->nom, id_pokemon, m->temps_production,
                m->max_figurines_avant_maintenance, m->compteur_figurines,
                m->total_produit, m->cout_maintenance, (int)m->etat, 
                (long)m->date_disponibilite, id_cmd, m->temps_maintenance);

        // Écrire les IDs de la file d'attente séparés par des virgules
        liste_commande* c_file = m->file_attente;
        while (c_file != NULL) {
            fprintf(fichier, "%d", c_file->commande.identifiant);
            if (c_file->suivant != NULL) fprintf(fichier, ",");
            c_file = c_file->suivant;
        }
        fprintf(fichier, "\n");

        courant = courant->suivant;
    }
    fclose(fichier);
}

// Sauvegarder les Commandes dans commandes.txt
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
        // Vérifier que le client et le pokemon existent
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

// Sauvegarder la caisse dans caisse.txt
void sauvegarder_caisse_dans_fichier(Caisse* caisse) {
    FILE* fichier = fopen(FICHIER_CAISSE, "w");

    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier '%s' en ecriture!\n",
               FICHIER_CAISSE);
        return;
    }

    printf("Sauvegarde de la caisse dans '%s'...\n", FICHIER_CAISSE);

    // Format simple: une seule ligne avec le solde
    fprintf(fichier, "%.2f\n", caisse->solde);

    fclose(fichier);
    printf("  -> Caisse sauvegardee: %.2f pieces\n", caisse->solde);
}
