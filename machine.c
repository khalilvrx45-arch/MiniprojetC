#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "machine.h"

// ============================================================================
// FONCTIONS OBLIGATOIRES
// ============================================================================

// Afficher toutes les machines
void afficher_Machines(liste_machine* liste_machines) {
    if (liste_machines == NULL) {
        printf("\n=== AUCUNE MACHINE ENREGISTREE ===\n");
        return;
    }

    int count = compter_machines(liste_machines);
    printf("\n=============== LISTE DES MACHINES (%d) ===============\n", count);

    liste_machine* courant = liste_machines;
    int numero = 1;

    while (courant != NULL) {
        printf("\n--- Machine #%d ---\n", numero);
        printf("ID: %d\n", courant->machine.identifiant);
        printf("Nom: %s\n", courant->machine.nom);

        if (courant->machine.pokemon_produit != NULL) {
            printf("Pokemon produit: %s (ID: %d)\n",
                   courant->machine.pokemon_produit->nom,
                   courant->machine.pokemon_produit->identifiant);
        } else {
            printf("Pokemon produit: Aucun\n");
        }

        printf("Temps de production: %d min\n", courant->machine.temps_production);
        printf("Max avant maintenance: %d\n", courant->machine.max_figurines_avant_maintenance);
        printf("Compteur production: %d\n", courant->machine.compteur_figurines);
        printf("Cout maintenance: %.2f pieces\n", courant->machine.cout_maintenance);

        printf("Etat: ");
        switch(courant->machine.etat) {
            case LIBRE: printf("LIBRE\n"); break;
            case OCCUPEE: printf("OCCUPEE\n"); break;
            case EN_ATTENTE_MAINTENANCE: printf("EN ATTENTE MAINTENANCE\n"); break;
        }

        courant = courant->suivant;
        numero++;
    }

    printf("\n====================================================\n");
}

// Ajouter une machine
void ajouter_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons) {
    if (liste_pokemons == NULL) {
        printf("\nErreur: Aucun Pokemon disponible! Creez d'abord des Pokemons.\n");
        return;
    }

    // Créer le nouveau nœud
    liste_machine* nouveau = (liste_machine*)malloc(sizeof(liste_machine));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire!\n");
        return;
    }

    Machine* m = &(nouveau->machine);
    m->identifiant = prochain_id_machine;

    printf("\n========== AJOUT D'UNE MACHINE ==========\n");

    // Vider le buffer
    while (getchar() != '\n');

    printf("Nom de la machine: ");
    scanf("%49[^\n]", m->nom);

    // Afficher les Pokemons disponibles
    printf("\nPokemons disponibles:\n");
    afficher_Pokemons(liste_pokemons);

    int id_pokemon;
    printf("\nID du Pokemon a produire: ");
    scanf("%d", &id_pokemon);

    m->pokemon_produit = trouver_pokemon_par_id(liste_pokemons, id_pokemon);

    if (m->pokemon_produit == NULL) {
        printf("Erreur: Pokemon ID %d non trouve!\n", id_pokemon);
        free(nouveau);
        return;
    }

    printf("Temps de production (minutes): ");
    scanf("%d", &m->temps_production);

    printf("Nombre max de figurines avant maintenance: ");
    scanf("%d", &m->max_figurines_avant_maintenance);

    printf("Cout de maintenance (pieces): ");
    scanf("%f", &m->cout_maintenance);

    // Initialisation
    m->compteur_figurines = 0;
    m->etat = LIBRE;
    m->commande_en_cours = NULL;
    m->date_disponibilite = time(NULL);

    // Ajout en tête de liste
    nouveau->suivant = *liste_machines;
    *liste_machines = nouveau;

    prochain_id_machine++;

    printf("\nMachine ajoutee avec succes! (ID: %d)\n", m->identifiant);
}

// Modifier une machine
void modifier_Machine(liste_machine* liste_machines, liste_pokemon* liste_pokemons) {
    if (liste_machines == NULL) {
        printf("\nAucune machine a modifier!\n");
        return;
    }

    afficher_Machines(liste_machines);

    int id;
    printf("\nID de la machine a modifier: ");
    scanf("%d", &id);

    // Rechercher la machine
    Machine* m = trouver_machine_par_id(liste_machines, id);

    if (m == NULL) {
        printf("\nMachine ID %d introuvable!\n", id);
        return;
    }

    printf("\n========== MODIFICATION MACHINE ID: %d ==========\n", id);

    // Vider le buffer
    while (getchar() != '\n');

    printf("Nouveau nom (actuel: %s): ", m->nom);
    scanf("%49[^\n]", m->nom);

    // Modifier le Pokemon
    if (liste_pokemons != NULL) {
        printf("\nPokemons disponibles:\n");
        afficher_Pokemons(liste_pokemons);

        int id_pokemon;
        printf("\nID du nouveau Pokemon: ");
        scanf("%d", &id_pokemon);

        Pokemon* nouveau_pokemon = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
        if (nouveau_pokemon != NULL) {
            m->pokemon_produit = nouveau_pokemon;
        }
    }

    printf("Nouveau temps de production (actuel: %d): ", m->temps_production);
    scanf("%d", &m->temps_production);

    printf("Nouveau seuil (actuel: %d): ", m->max_figurines_avant_maintenance);
    scanf("%d", &m->max_figurines_avant_maintenance);

    printf("Nouveau cout (actuel: %.2f): ", m->cout_maintenance);
    scanf("%f", &m->cout_maintenance);

    printf("\nMachine modifiee avec succes!\n");
}

// Supprimer une machine
void supprimer_Machine(liste_machine** liste_machines, liste_commande* liste_commandes) {
    if (*liste_machines == NULL) {
        printf("\nAucune machine a supprimer!\n");
        return;
    }

    afficher_Machines(*liste_machines);

    int id;
    printf("\nID de la machine a supprimer: ");
    scanf("%d", &id);

    // Recherche et suppression
    liste_machine* courant = *liste_machines;
    liste_machine* precedent = NULL;

    while (courant != NULL) {
        if (courant->machine.identifiant == id) {
            // Vérification simple : machine occupée ?
            if (courant->machine.etat == OCCUPEE) {
                printf("Erreur: Impossible de supprimer, machine occupee!\n");
                return;
            }

            printf("\nMachine '%s' (ID: %d) sera supprimee.\n",
                   courant->machine.nom, id);

            // Supprimer le nœud
            if (precedent == NULL) {
                *liste_machines = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }

            printf("\nMachine supprimee avec succes!\n");
            free(courant);
            return;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nMachine ID %d introuvable!\n", id);
}

// Trouver une machine par ID (INDISPENSABLE pour le chargement)
Machine* trouver_machine_par_id(liste_machine* liste_machines, int id) {
    liste_machine* courant = liste_machines;

    while (courant != NULL) {
        if (courant->machine.identifiant == id) {
            return &(courant->machine);
        }
        courant = courant->suivant;
    }

    return NULL;
}

// Compter les machines (INDISPENSABLE pour l'état de l'usine)
int compter_machines(liste_machine* liste_machines) {
    int count = 0;
    liste_machine* courant = liste_machines;

    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }

    return count;
}

// Compter par état (INDISPENSABLE pour l'état de l'usine)
int compter_machines_par_etat(liste_machine* liste_machines, EtatMachine etat) {
    int count = 0;
    liste_machine* courant = liste_machines;

    while (courant != NULL) {
        if (courant->machine.etat == etat) {
            count++;
        }
        courant = courant->suivant;
    }

    return count;
}

// Libérer la mémoire (INDISPENSABLE)
void liberer_liste_machines(liste_machine** liste_machines) {
    if (liste_machines == NULL || *liste_machines == NULL) {
        return;
    }

    liste_machine* courant = *liste_machines;
    liste_machine* suivant;

    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }

    *liste_machines = NULL;
}
