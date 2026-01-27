#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fonction.h"
#define MAX_MACHINES 100
#define MAX_CLIENTS 100
/*int prochain_id_pokemon = 1;
int prochain_id_machine = 1;
int prochain_id_commande = 1;
/*uintptr_t generate_id(void *ptr){
    return (uintptr_t)ptr;
}*/

void Afficher_Pokemon(Pokemon tab[], int nb_pokemon, int id) {

    for (int i = 0; i < nb_pokemon; i++) {
        if (tab[i].identifiant == id) {
            printf("=== Pokémon ID: %d ===\n", tab[i].identifiant);
            printf("Nom: %s\n", tab[i].nom);
            printf("Coût unitaire: %.2f\n", tab[i].cout_unitaire);

            // Affichage du type sous forme textuelle
            printf("Type: ");
            switch (tab[i].type) {
                case ELECTRIQUE:
                    printf("Électrique\n");
                    break;
                case FEU:
                    printf("Feu\n");
                    break;
                case EAU:
                    printf("Eau\n");
                    break;
                case PLANTE:
                    printf("Plante\n");
                    break;
                default:
                    printf("Inconnu\n");
            }
            printf("======================\n");
            return;
        }
    }
    printf("Pokemon avec ID %d non trouve.\n", id);
     // Pokémon non trouvé
}
void Afficher_tous_pokemons(Pokemon tab[], int nb_pokemon) {
    printf("\n=== LISTE DE TOUS LES POKEMONS (%d) ===\n", nb_pokemon);
    for (int i = 0; i < nb_pokemon; i++) {
        printf("Index %d: ID=%d, Nom=%s\n", i, tab[i].identifiant, tab[i].nom);
    }
}
void Ajouter_Pokemon(Pokemon pokemons[], int *nb_pokemons, int max_pokemons) {
    // Vérifier si le tableau est plein
    if (*nb_pokemons >= max_pokemons) {
        printf("Erreur : Le tableau de Pokemon est plein (capacite max: %d)\n", max_pokemons);
        return;
    }



    // Créer un nouveau Pokémon
    Pokemon nouveau_pokemon;

    // Générer l'identifiant automatiquement
    nouveau_pokemon.identifiant = prochain_id_pokemon++;

    // Vider le buffer avant la saisie
    while (getchar() != '\n');
    // Saisie du nom
    printf("Entrez le nom du Pokemon (max 49 caracteres) : ");
    scanf(" %s", nouveau_pokemon.nom);

    // Saisie du coût unitaire
    do {
        printf("Entrez le cout unitaire du Pokemon (positif) : ");
        scanf("%f", &nouveau_pokemon.cout_unitaire);
        if (nouveau_pokemon.cout_unitaire <= 0) {
            printf("Erreur : Le coût doit etre positif.\n");
            while (getchar() != '\n');
            continue;
         }


    } while (nouveau_pokemon.cout_unitaire <= 0);

    // Saisie du type
    int choix_type;
    printf("Choisissez le type du Pokémon :\n");
    printf("1 - Électrique\n");
    printf("2 - Feu\n");
    printf("3 - Eau\n");
    printf("4 - Plante\n");

    do {
        printf("Votre choix (1-4) : ");
        scanf("%d", &choix_type);

        switch (choix_type) {
            case 1:
                nouveau_pokemon.type = ELECTRIQUE;
                break;
            case 2:
                nouveau_pokemon.type = FEU;
                break;
            case 3:
                nouveau_pokemon.type = EAU;
                break;
            case 4:
                nouveau_pokemon.type = PLANTE;
                break;
            default:
                printf("Choix invalide. Veuillez saisir un nombre entre 1 et 4.\n");
                choix_type = 0;
        }
    } while (choix_type < 1 || choix_type > 4);

    // Ajouter le Pokémon au tableau
    pokemons[*nb_pokemons] = nouveau_pokemon;
    (*nb_pokemons)++;
}

void Modifier_Pokemon(Pokemon pokemons[], int nb_pokemons) {
    const char* type_to_string(TypePokemon type) {
    switch (type) {
        case ELECTRIQUE: return "Électrique";
        case FEU: return "Feu";
        case EAU: return "Eau";
        case PLANTE: return "Plante";
        default: return "Inconnu";
    }
    }
    if (nb_pokemons == 0) {
        printf("Aucun Pokémon à modifier. La base de données est vide.\n");

    }

    int id_recherche;
    int index = -1;


    // Rechercher le Pokémon par ID
    printf("Entrez l'ID du Pokémon à modifier : ");
    scanf("%d", &id_recherche);

    // Chercher le Pokémon
    for (int i = 0; i < nb_pokemons; i++) {
        if (pokemons[i].identifiant == id_recherche) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Pokémon avec l'ID %d non trouvé.\n", id_recherche);

    }

    // Afficher les informations actuelles
    printf("\n═══════════════════════════════════════\n");
    printf("        MODIFICATION DU POKÉMON\n");
    printf("═══════════════════════════════════════\n");
    printf("ID : %d (non modifiable)\n", pokemons[index].identifiant);
    printf("═══════════════════════════════════════\n");

    int choix;
    char buffer[50];
    float nouveau_cout;
    int nouveau_type;

    do {
        printf("\nQue souhaitez-vous modifier ?\n");
        printf("1. Nom (actuel : %s)\n", pokemons[index].nom);
        printf("2. Coût unitaire (actuel : %.2f)\n", pokemons[index].cout_unitaire);
        printf("3. Type (actuel : %s)\n", type_to_string(pokemons[index].type));
        printf("4. Tout modifier\n");
        printf("5. Annuler\n");
        printf("Votre choix (1-5) : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: // Modifier le nom
                printf("Nouveau nom : ");
                scanf(" %[^\n]", buffer);

                // Vérifier si le nom n'est pas déjà utilisé (sauf pour le même Pokémon)
                int nom_existe = 0;
                for (int i = 0; i < nb_pokemons; i++) {
                    if (i != index && strcasecmp(pokemons[i].nom, buffer) == 0) {
                        nom_existe = 1;
                        break;
                    }
                }

                if (nom_existe) {
                    printf("Ce nom est déjà utilisé par un autre Pokémon.\n");
                } else if (strlen(buffer) >= 50) {
                    printf(" Nom trop long (max 49 caractères).\n");
                } else {
                    strcpy(pokemons[index].nom, buffer);
                    printf("Nom modifié avec succès.\n");
                }
                break;

            case 2: // Modifier le coût unitaire
                printf("Nouveau coût unitaire : ");
                scanf("%f", &nouveau_cout);

                if (nouveau_cout <= 0) {
                    printf("Le coût doit être positif.\n");
                } else if (nouveau_cout > 5000) {
                    printf("Coût très élevé. Confirmez-vous ? (o/n) : ");
                    char confirmation;
                    scanf(" %c", &confirmation);
                    if (confirmation == 'o' || confirmation == 'O') {
                        pokemons[index].cout_unitaire = nouveau_cout;
                        printf("Coût unitaire modifié avec succès.\n");
                    }
                } else {
                    pokemons[index].cout_unitaire = nouveau_cout;
                    printf("Coût unitaire modifié avec succès.\n");
                }
                break;

            case 3: // Modifier le type
                printf("Nouveau type :\n");
                printf("1. Électrique\n");
                printf("2. Feu\n");
                printf("3. Eau\n");
                printf("4. Plante\n");
                printf("Choix (1-4) : ");
                scanf("%d", &nouveau_type);

                if (nouveau_type < 1 || nouveau_type > 4) {
                    printf("Type invalide.\n");
                } else {
                    switch (nouveau_type) {
                        case 1:
                            pokemons[index].type = ELECTRIQUE;
                            break;
                        case 2:
                            pokemons[index].type = FEU;
                            break;
                        case 3:
                            pokemons[index].type = EAU;
                            break;
                        case 4:
                            pokemons[index].type = PLANTE;
                            break;
                    }
                    printf("Type modifié avec succès.\n");
                }
                break;

            case 4: // Modifier tout
                // Nom
                printf("Nouveau nom : ");
                scanf(" %[^\n]", buffer);

                int nom_existe_tout = 0;
                for (int i = 0; i < nb_pokemons; i++) {
                    if (i != index && strcasecmp(pokemons[i].nom, buffer) == 0) {
                        nom_existe_tout = 1;
                        break;
                    }
                }

                if (nom_existe_tout) {
                    printf("Ce nom est déjà utilisé par un autre Pokémon.\n");
                    break;
                }

                if (strlen(buffer) >= 50) {
                    printf("Nom trop long (max 49 caractères).\n");
                    break;
                }

                strcpy(pokemons[index].nom, buffer);

                // Coût unitaire
                printf("Nouveau coût unitaire : ");
                scanf("%f", &nouveau_cout);

                if (nouveau_cout <= 0) {
                    printf("Le coût doit être positif. Modification annulée.\n");
                    break;
                }

                pokemons[index].cout_unitaire = nouveau_cout;

                // Type
                printf("Nouveau type (1:Électrique, 2:Feu, 3:Eau, 4:Plante) : ");
                scanf("%d", &nouveau_type);

                if (nouveau_type < 1 || nouveau_type > 4) {
                    printf("Type invalide. Modification annulée.\n");
                    break;
                }

                switch (nouveau_type) {
                    case 1:
                        pokemons[index].type = ELECTRIQUE;
                        break;
                    case 2:
                        pokemons[index].type = FEU;
                        break;
                    case 3:
                        pokemons[index].type = EAU;
                        break;
                    case 4:
                        pokemons[index].type = PLANTE;
                        break;
                }

                printf("Toutes les modifications ont été appliquées.\n");
                break;

            case 5: // Annuler
                printf("Modification annulée.\n");


            default:
                printf("Choix invalide.\n");
        }

        // Demander si l'utilisateur veut continuer à modifier
        if (choix >= 1 && choix <= 4) {
            printf("\nSouhaitez-vous modifier autre chose pour ce Pokémon ? (o/n) : ");
            char continuer;
            scanf(" %c", &continuer);

            if (continuer == 'n' || continuer == 'N') {
                break;
            }
        }

    } while (choix != 5);
}

void Supprimer_Pokemon(int id, Pokemon tab[], int *nb_pokemons) {
    Pokemon inter;
    int pos,i;
    for (i=0;i< *nb_pokemons ;i++){
        if (tab[i].identifiant == id){
            pos = i;
            break;
        }
    }
    if(pos== *nb_pokemons){
        *nb_pokemons--;
    } else{
        for (i=pos;i<*nb_pokemons-1;i++){
            tab[i]=tab[i+1];
        }
        (*nb_pokemons)--;
    }
}
int Afficher_commande_par_id(Commande commandes[], int nb_commandes, int id) {
    for (int i = 0; i < nb_commandes; i++) {
        if (commandes[i].identifiant == id) {
            printf("\n═══════════════════════════════════════\n");
            printf("        COMMANDE ID: %d\n", id);
            printf("═══════════════════════════════════════\n");

            printf("ID: %d\n", commandes[i].identifiant);

            // Informations client
            if (commandes[i].client != NULL) {
                printf("Client:\n");
                printf("  Nom: %s\n", commandes[i].client->nom);
                printf("  Matricule: %s\n", commandes[i].client->matricule);
            } else {
                printf("Client: Non specifie\n");
            }

            // Informations Pokémon
            if (commandes[i].pokemon_commande != NULL) {
                printf("Pokemon commande:\n");
                printf("  Nom: %s\n", commandes[i].pokemon_commande->nom);
                printf("  ID: %d\n", commandes[i].pokemon_commande->identifiant);
                printf("  Cout unitaire: %.2f\n", commandes[i].pokemon_commande->cout_unitaire);

                printf("  Type: ");
                switch (commandes[i].pokemon_commande->type) {
                    case ELECTRIQUE: printf("Electrique\n"); break;
                    case FEU: printf("Feu\n"); break;
                    case EAU: printf("Eau\n"); break;
                    case PLANTE: printf("Plante\n"); break;
                    default: printf("Inconnu\n");
                }
            } else {
                printf("Pokemon: Non specifie\n");
            }

            printf("Quantité: %d\n", commandes[i].quantite);
            printf("Coût total: %.2f\n",
                   commandes[i].pokemon_commande != NULL ?
                   commandes[i].quantite * commandes[i].pokemon_commande->cout_unitaire : 0);

            // Conversion de la date
            char date_str[100];
            strftime(date_str, sizeof(date_str), "%A %d %B %Y à %H:%M:%S",
                     localtime(&commandes[i].date_emission));
            printf("Date d'emission: %s\n", date_str);

            printf("Etat: ");
            switch (commandes[i].etat) {
                case EN_ATTENTE:
                    printf("En attente\n");
                    break;
                case EN_COURS:
                    printf("En cours\n");
                    break;
                case REALISEE:
                    printf("Realisee\n");
                    break;
                default:
                    printf("Inconnu\n");
            }

            printf("═══════════════════════════════════════\n");
            return 1;
        }
    }

    printf("Commande avec l'ID %d non trouvee.\n", id);
    return 0;
}
void Ajouter_Commande(Commande commandes[], int *nb_commandes, int capacite, Client *client ,Pokemon *pokemon, int quantite){
    printf("====Ajouter une commande====\n");
    if (commandes == NULL || nb_commandes == NULL || client == NULL || pokemon == NULL) {
        printf("Erreur : paramètres invalides\n");

    }
    if (*nb_commandes >= capacite) {
        printf("Erreur : tableau de commandes plein (capacité : %d)\n", capacite);
    }
    if (quantite <= 0) {
        printf("Erreur : quantité invalide %d\n", quantite);

    }
    if (client->matricule[0] == '\0') {
        printf("Erreur : client invalide\n");

    }
    if (pokemon->identifiant <= 0) {
        printf("Erreur : Pokémon invalide\n");

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
    //Mettre à jour le client (si sa liste existe)
    int index = *nb_commandes;
    if (client->liste_commandes != NULL) {
        // Vérifier si le client a de la place
        if (client->nb_commandes < client->capacite_commandes) {
            client->liste_commandes[client->nb_commandes] = &commandes[index];
            client->nb_commandes++;
        } else {
            printf("Avertissement : client a atteint sa capacité de commandes\n");
        }
    }
    (*nb_commandes)++;
    printf("Commande #%d ajoutée (total: %d)\n",
           commandes[index].identifiant, *nb_commandes);
}
void modifier_Commande(Commande commandes[], int *nb_commandes, int id_commande, Client *nouveau_client, Pokemon *nouveau_pokemon, int nouvelle_quantite, EtatCommande nouvel_etat) {
    // Recherche de la commande dans le tableau
    int index = -1;
    for (int i = 0; i < *nb_commandes; i++) {
        if (commandes[i].identifiant == id_commande) {
            index = i;
            break;
        }
    }

    // Si la commande n'existe pas
    if (index == -1) {
        printf("Commande avec ID %d non trouvée.\n", id_commande);

    }
    if (nouveau_client != NULL) {
        commandes[index].client = nouveau_client;
    }

    if (nouveau_pokemon != NULL) {
        commandes[index].pokemon_commande = nouveau_pokemon;
    }

    if (nouvelle_quantite != -1) {
        if (nouvelle_quantite > 0) {
            commandes[index].quantite = nouvelle_quantite;
        } else {
            printf("Erreur: La quantité doit être positive.\n");

        }
    }

    if (nouvel_etat != -1) {
        commandes[index].etat = nouvel_etat;

        if (nouvel_etat == REALISEE) {
            printf("Commande %d marquée comme réalisée.\n", id_commande);
        }
    }

    printf("Commande %d modifiée avec succès.\n", id_commande);

}
int supprimer_Commande(Commande commandes[], int *nb_commandes, int id_commande) {

    // Vérifier si le tableau est vide
    if (*nb_commandes == 0) {
        printf("Aucune commande à supprimer.\n");
        return 0;
    }

    // Recherche de la commande dans le tableau
    int index = -1;
    for (int i = 0; i < *nb_commandes; i++) {
        if (commandes[i].identifiant == id_commande) {
            index = i;
            break;
        }
    }

    // Si la commande n'existe pas
    if (index == -1) {
        printf("Commande avec ID %d non trouvée.\n", id_commande);
        return 0;
    }

    // Suppression de la commande du tableau du client associé
    if (commandes[index].client != NULL) {
        Client *client = commandes[index].client;

        // Recherche de la commande dans la liste des commandes du client
        for (int i = 0; i < client->nb_commandes; i++) {
            if (client->liste_commandes[i]->identifiant == id_commande) {
                // Décalage des commandes suivantes dans le tableau du client
                for (int j = i; j < client->nb_commandes - 1; j++) {
                    client->liste_commandes[j] = client->liste_commandes[j + 1];
                }
                client->nb_commandes--;
                break;
            }
        }
    }

    // Décalage des commandes suivantes dans le tableau principal
    for (int i = index; i < *nb_commandes - 1; i++) {
        commandes[i] = commandes[i + 1];
    }

    // Mise à jour du compteur de commandes
    (*nb_commandes)--;

    printf("Commande %d supprimée avec succès.\n", id_commande);
    return 1;
}

extern int prochain_id_machine;

// ============ 1. AFFICHER LES MACHINES ============

void afficherMachines(Machine machines[], int nbMachines) {
    if (nbMachines == 0) {
        printf("\n=== AUCUNE MACHINE ENREGISTREE ===\n");
        return;
    }

    printf("\n=============== LISTE DES MACHINES (%d) ===============\n", nbMachines);

    for (int i = 0; i < nbMachines; i++) {
        printf("\n--- Machine #%d ---\n", i + 1);
        printf("ID: %d\n", machines[i].identifiant);
        printf("Nom: %s\n", machines[i].nom);

        if (machines[i].pokemon_produit != NULL) {
            printf("Pokemon produit: %s (ID: %d)\n",
                   machines[i].pokemon_produit->nom,
                   machines[i].pokemon_produit->identifiant);
        } else {
            printf("Pokemon produit: Aucun\n");
        }

        printf("Temps de production: %d min\n", machines[i].temps_production);
        printf("Max avant maintenance: %d\n", machines[i].max_figurines_avant_maintenance);
        printf("Compteur production: %d\n", machines[i].compteur_figurines);
        printf("Cout maintenance: %.2f pieces\n", machines[i].cout_maintenance);

        printf("Etat: ");
        switch(machines[i].etat) {
            case LIBRE: printf("LIBRE\n"); break;
            case OCCUPEE: printf("OCCUPEE\n"); break;
            case EN_ATTENTE_MAINTENANCE: printf("EN ATTENTE MAINTENANCE\n"); break;
        }
    }
    printf("\n====================================================\n");
}

// ============ 2. AJOUTER UNE MACHINE ============

void ajouterMachine(Machine machines[], int* nbMachines, Pokemon pokemons[], int nbPokemons) {
    if (*nbMachines >= MAX_MACHINES) {
        printf("\nErreur: Nombre maximum de machines atteint!\n");
        return;
    }

    if (nbPokemons == 0) {
        printf("\nErreur: Aucun Pokemon disponible! Creez d'abord des Pokemons.\n");
        return;
    }

    Machine m;
    m.identifiant = prochain_id_machine;

    printf("\n========== AJOUT D'UNE MACHINE ==========\n");

    printf("Nom de la machine: ");
    scanf(" %[^\n]", m.nom);

    // Afficher les Pokemons disponibles
    printf("\nPokemons disponibles:\n");
    for (int i = 0; i < nbPokemons; i++) {
        printf("  %d. %s (ID: %d)\n", i + 1, pokemons[i].nom, pokemons[i].identifiant);
    }

    int choix;
    do {
        printf("Choisissez un Pokemon (1-%d): ", nbPokemons);
        scanf("%d", &choix);
    } while (choix < 1 || choix > nbPokemons);

    m.pokemon_produit = &pokemons[choix - 1];

    printf("Temps de production (minutes): ");
    scanf("%d", &m.temps_production);

    printf("Nombre max de figurines avant maintenance: ");
    scanf("%d", &m.max_figurines_avant_maintenance);

    printf("Cout de maintenance (pieces): ");
    scanf("%f", &m.cout_maintenance);

    // Initialisation
    m.compteur_figurines = 0;
    m.etat = LIBRE;
    m.commande_en_cours = NULL;
    m.date_disponibilite = time(NULL);

    machines[*nbMachines] = m;
    (*nbMachines)++;
    prochain_id_machine++;

    printf("\nMachine ajoutee avec succes! (ID: %d)\n", m.identifiant);
}

// ============ 3. MODIFIER UNE MACHINE ============

void modifierMachine(Machine machines[], int nbMachines, Pokemon pokemons[], int nbPokemons) {
    if (nbMachines == 0) {
        printf("\nAucune machine a modifier!\n");
        return;
    }

    int id;
    printf("\nID de la machine a modifier: ");
    scanf("%d", &id);

    // Rechercher la machine
    int index = -1;
    for (int i = 0; i < nbMachines; i++) {
        if (machines[i].identifiant == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nMachine introuvable!\n");
        return;
    }

    printf("\n========== MODIFICATION MACHINE ID: %d ==========\n", id);

    printf("Nouveau nom (actuel: %s): ", machines[index].nom);
    scanf(" %[^\n]", machines[index].nom);

    // Modifier le Pokemon
    printf("\nPokemons disponibles:\n");
    for (int i = 0; i < nbPokemons; i++) {
        printf("  %d. %s (ID: %d)\n", i + 1, pokemons[i].nom, pokemons[i].identifiant);
    }

    int choix;
    do {
        printf("Nouveau Pokemon (1-%d): ", nbPokemons);
        scanf("%d", &choix);
    } while (choix < 1 || choix > nbPokemons);

    machines[index].pokemon_produit = &pokemons[choix - 1];

    printf("Nouveau temps de production (actuel: %d): ", machines[index].temps_production);
    scanf("%d", &machines[index].temps_production);

    printf("Nouveau max avant maintenance (actuel: %d): ", machines[index].max_figurines_avant_maintenance);
    scanf("%d", &machines[index].max_figurines_avant_maintenance);

    printf("Nouveau cout de maintenance (actuel: %.2f): ", machines[index].cout_maintenance);
    scanf("%f", &machines[index].cout_maintenance);

    printf("\nMachine modifiee avec succes!\n");
}

// ============ 4. SUPPRIMER UNE MACHINE ============
void supprimerMachine(Machine machines[], int* nbMachines) {
    if (*nbMachines == 0) {
        printf("\nAucune machine a supprimer!\n");
        return;
    }

    int id;
    printf("\nID de la machine a supprimer: ");
    scanf("%d", &id);

    // Rechercher la machine
    int index = -1;
    for (int i = 0; i < *nbMachines; i++) {
        if (machines[i].identifiant == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("\nMachine introuvable!\n");
        return;
    }

    printf("\nMachine '%s' (ID: %d) sera supprimee.\n", machines[index].nom, id);

    // Décalage des éléments
    for (int i = index; i < *nbMachines - 1; i++) {
        machines[i] = machines[i + 1];
    }

    (*nbMachines)--;
    printf("\nMachine supprimee avec succes!\n");
}
