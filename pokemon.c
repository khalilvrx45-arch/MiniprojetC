#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"

void Afficher_Pokemon(Pokemon tab[], int nb_pokemon, int id) {
    for (int i = 0; i < nb_pokemon; i++) {
        if (tab[i].identifiant == id) {
            printf("=== Pokémon ID: %d ===\n", tab[i].identifiant);
            printf("Nom: %s\n", tab[i].nom);
            printf("Coût unitaire: %.2f\n", tab[i].cout_unitaire);
            printf("Type: %s\n", type_to_string(tab[i].type));
            printf("======================\n");
            return;
        }
    }
    printf("Pokemon avec ID %d non trouve.\n", id);
}

void Afficher_tous_pokemons(Pokemon tab[], int nb_pokemon) {
    printf("\n=== LISTE DE TOUS LES POKEMONS (%d) ===\n", nb_pokemon);
    for (int i = 0; i < nb_pokemon; i++) {
        printf("Index %d: ID=%d, Nom=%s, Type=%s, Coût=%.2f\n",
               i, tab[i].identifiant, tab[i].nom,
               type_to_string(tab[i].type), tab[i].cout_unitaire);
    }
}

void Ajouter_Pokemon(Pokemon pokemons[], int *nb_pokemons, int max_pokemons) {
    if (*nb_pokemons >= max_pokemons) {
        printf("Erreur : Le tableau de Pokemon est plein (capacite max: %d)\n", max_pokemons);
        return;
    }

    Pokemon nouveau_pokemon;
    nouveau_pokemon.identifiant = prochain_id_pokemon++;

    // Vider le buffer
    while (getchar() != '\n');

    printf("Entrez le nom du Pokemon (max 49 caracteres) : ");
    scanf("%49[^\n]", nouveau_pokemon.nom);

    do {
        printf("Entrez le cout unitaire du Pokemon (positif) : ");
        scanf("%f", &nouveau_pokemon.cout_unitaire);
        if (nouveau_pokemon.cout_unitaire <= 0) {
            printf("Erreur : Le coût doit etre positif.\n");
            while (getchar() != '\n');
        }
    } while (nouveau_pokemon.cout_unitaire <= 0);

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
            case 1: nouveau_pokemon.type = ELECTRIQUE; break;
            case 2: nouveau_pokemon.type = FEU; break;
            case 3: nouveau_pokemon.type = EAU; break;
            case 4: nouveau_pokemon.type = PLANTE; break;
            default:
                printf("Choix invalide. Veuillez saisir un nombre entre 1 et 4.\n");
                choix_type = 0;
        }
    } while (choix_type < 1 || choix_type > 4);

    pokemons[*nb_pokemons] = nouveau_pokemon;
    (*nb_pokemons)++;

    printf("Pokemon '%s' ajoute avec succes! (ID: %d)\n",
           nouveau_pokemon.nom, nouveau_pokemon.identifiant);
}

void Modifier_Pokemon(Pokemon pokemons[], int nb_pokemons) {
    if (nb_pokemons == 0) {
        printf("Aucun Pokémon à modifier. La base de données est vide.\n");
        return;
    }

    int id_recherche;
    int index = -1;

    printf("Entrez l'ID du Pokémon à modifier : ");
    scanf("%d", &id_recherche);

    for (int i = 0; i < nb_pokemons; i++) {
        if (pokemons[i].identifiant == id_recherche) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Pokémon avec l'ID %d non trouvé.\n", id_recherche);
        return;
    }

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
                    printf("Nom trop long (max 49 caractères).\n");
                } else {
                    strcpy(pokemons[index].nom, buffer);
                    printf("Nom modifié avec succès.\n");
                }
                break;

            case 2: // Modifier le coût
                printf("Nouveau coût unitaire : ");
                scanf("%f", &nouveau_cout);

                if (nouveau_cout <= 0) {
                    printf("Le coût doit être positif.\n");
                } else {
                    pokemons[index].cout_unitaire = nouveau_cout;
                    printf("Coût unitaire modifié avec succès.\n");
                }
                break;

            case 3: // Modifier le type
                printf("Nouveau type :\n");
                printf("1. Électrique\n2. Feu\n3. Eau\n4. Plante\n");
                printf("Choix (1-4) : ");
                scanf("%d", &nouveau_type);

                if (nouveau_type >= 1 && nouveau_type <= 4) {
                    pokemons[index].type = (TypePokemon)(nouveau_type - 1);
                    printf("Type modifié avec succès.\n");
                } else {
                    printf("Type invalide.\n");
                }
                break;

            case 4: // Modifier tout
                printf("Nouveau nom : ");
                scanf(" %[^\n]", buffer);
                strcpy(pokemons[index].nom, buffer);

                printf("Nouveau coût unitaire : ");
                scanf("%f", &nouveau_cout);
                if (nouveau_cout > 0) {
                    pokemons[index].cout_unitaire = nouveau_cout;
                }

                printf("Nouveau type (1:Électrique, 2:Feu, 3:Eau, 4:Plante) : ");
                scanf("%d", &nouveau_type);
                if (nouveau_type >= 1 && nouveau_type <= 4) {
                    pokemons[index].type = (TypePokemon)(nouveau_type - 1);
                }

                printf("Toutes les modifications ont été appliquées.\n");
                break;

            case 5: // Annuler
                printf("Modification annulée.\n");
                return;

            default:
                printf("Choix invalide.\n");
        }

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
    int pos = -1;

    for (int i = 0; i < *nb_pokemons; i++) {
        if (tab[i].identifiant == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Pokemon avec ID %d non trouve.\n", id);
        return;
    }

    for (int i = pos; i < *nb_pokemons - 1; i++) {
        tab[i] = tab[i + 1];
    }

    (*nb_pokemons)--;
    printf("Pokemon avec ID %d supprime avec succes.\n", id);
}
