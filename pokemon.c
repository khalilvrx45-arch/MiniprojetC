#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pokemon.h"
#include "machine.h"


// 1. Afficher un seul Pokemon
void afficher_un_Pokemon(Pokemon* p) {
    if (p == NULL) return;
    printf("=== Pokemon ID: %d ===\n", p->identifiant);
    printf("Nom: %s\n", p->nom);
    printf("Cout unitaire: %.2f\n", p->cout_unitaire);
    printf("Type: %s\n", type_to_string(p->type));
    printf("======================\n");
}

// 2. Afficher tous les Pokemons
void afficher_Pokemons(liste_pokemon* liste) {
    if (liste == NULL) {
        printf("\n=== AUCUN POKEMON ENREGISTRE ===\n");
        return;
    }

    int count = compter_pokemons(liste);
    printf("\n=== LISTE DES POKEMONS (%d) ===\n", count);

    liste_pokemon* courant = liste;
    while (courant != NULL) {
        afficher_un_Pokemon(&(courant->pokemon));
        courant = courant->suivant;
    }
}

// 3. Ajouter un Pokemon
void ajouter_Pokemon(liste_pokemon** liste) {
    liste_pokemon* nouveau = (liste_pokemon*)malloc(sizeof(liste_pokemon));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire.\n");
        return;
    }

    Pokemon* p = &(nouveau->pokemon);
    p->identifiant = prochain_id_pokemon;

    while(getchar() != '\n'); // Vider buffer

    printf("Nom du Pokemon: ");
    scanf("%49[^\n]", p->nom);

    do {
        printf("Cout unitaire: ");
        scanf("%f", &p->cout_unitaire);
        if(p->cout_unitaire <= 0) printf("Le cout doit etre positif.\n");
    } while(p->cout_unitaire <= 0);

    printf("Type (1:Elec, 2:Feu, 3:Eau, 4:Plante): ");
    int type;
    scanf("%d", &type);
    if(type >= 1 && type <= 4) p->type = (TypePokemon)(type-1);
    else {
        printf("Type invalide, par defaut: ELECTRIQUE\n");
        p->type = ELECTRIQUE;
    }

    // Ajout en tete
    nouveau->suivant = *liste;
    *liste = nouveau;

    prochain_id_pokemon++;

    printf("Pokemon ajoute avec succes (ID %d)!\n", p->identifiant);
}

// 4. Trouver un Pokemon par ID
Pokemon* trouver_pokemon_par_id(liste_pokemon* liste, int id) {
    liste_pokemon* courant = liste;
    while (courant != NULL) {
        if (courant->pokemon.identifiant == id) {
            return &(courant->pokemon);
        }
        courant = courant->suivant;
    }
    return NULL;
}

// 5. Modifier un Pokemon
void modifier_Pokemon(liste_pokemon* liste) {
    if (liste == NULL) {
        printf("Aucun Pokemon a modifier.\n");
        return;
    }

    afficher_Pokemons(liste);

    int id;
    printf("ID du Pokemon a modifier: ");
    scanf("%d", &id);

    Pokemon* p = trouver_pokemon_par_id(liste, id);
    if (p == NULL) {
        printf("Pokemon ID %d introuvable.\n", id);
        return;
    }

    printf("\nModification de : %s\n", p->nom);
    printf("1. Nom\n2. Cout\n3. Type\n4. Tout\nChoice: ");
    int choix;
    scanf("%d", &choix);

    char buffer[50];

    switch(choix) {
        case 1:
            printf("Nouveau nom: ");
            scanf(" %49[^\n]", buffer);
            strcpy(p->nom, buffer);
            break;
        case 2:
            printf("Nouveau cout: ");
            scanf("%f", &p->cout_unitaire);
            break;
        case 3:
            printf("Nouveau type (1-4): ");
            int type;
            scanf("%d", &type);
            if(type >= 1 && type <= 4) p->type = (TypePokemon)(type-1);
            break;
        case 4:
            printf("Nouveau nom: ");
            scanf(" %49[^\n]", buffer);
            strcpy(p->nom, buffer);
            printf("Nouveau cout: ");
            scanf("%f", &p->cout_unitaire);
            printf("Nouveau type (1-4): ");
            scanf("%d", &type);
            if(type >= 1 && type <= 4) p->type = (TypePokemon)(type-1);
            break;
        default:
             printf("Annule.\n");
    }
    printf("Modifications enregistrees.\n");
}

// 6. Supprimer un Pokemon
void supprimer_Pokemon(liste_pokemon** liste, liste_machine** liste_machines) {
    if (*liste == NULL) {
        printf("Liste vide.\n");
        return;
    }

    afficher_Pokemons(*liste);

    int id;
    printf("ID a supprimer: ");
    scanf("%d", &id);

    liste_pokemon* courant = *liste;
    liste_pokemon* precedent = NULL;

    while (courant != NULL) {
        if (courant->pokemon.identifiant == id) {
             // Cascade: Supprimer les machines associées
             supprimer_machines_par_pokemon(liste_machines, id);
             
             // Suppression
             if (precedent == NULL) {
                 *liste = courant->suivant; // Tete
             } else {
                 precedent->suivant = courant->suivant;
             }
             free(courant);
             printf("Pokemon ID %d supprime.\n", id);
             return;
        }
        precedent = courant;
        courant = courant->suivant;
    }
    printf("Pokemon ID %d non trouve.\n", id);
}

// 7. Compter
int compter_pokemons(liste_pokemon* liste) {
    int c = 0;
    //liste_pokemon* courant = liste;
    while(liste) {
        c++;
        liste = liste->suivant;
        //courant = courant->suivant;
    }
    return c;
}

// 8. Liberer memoire
void liberer_liste_pokemons(liste_pokemon** liste) {
    liste_pokemon* courant = *liste;
    while(courant) {
        liste_pokemon* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
    *liste = NULL;
}
