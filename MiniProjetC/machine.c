#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "machine.h"

void afficher_Machines(Machine machines[], int nbMachines) {
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

void ajouter_Machine(Machine machines[], int* nbMachines, Pokemon pokemons[], int nbPokemons) {
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

    // Vider le buffer
    while (getchar() != '\n');
    
    printf("Nom de la machine: ");
    scanf("%49[^\n]", m.nom);

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

void modifier_Machine(Machine machines[], int nbMachines, Pokemon pokemons[], int nbPokemons) {
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

    // Vider le buffer
    while (getchar() != '\n');
    
    printf("Nouveau nom (actuel: %s): ", machines[index].nom);
    scanf("%49[^\n]", machines[index].nom);

    // Modifier le Pokemon
    if (nbPokemons > 0) {
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
    }

    printf("Nouveau temps de production (actuel: %d): ", machines[index].temps_production);
    scanf("%d", &machines[index].temps_production);

    printf("Nouveau max avant maintenance (actuel: %d): ", machines[index].max_figurines_avant_maintenance);
    scanf("%d", &machines[index].max_figurines_avant_maintenance);

    printf("Nouveau cout de maintenance (actuel: %.2f): ", machines[index].cout_maintenance);
    scanf("%f", &machines[index].cout_maintenance);

    printf("\nMachine modifiee avec succes!\n");
}

void supprimer_Machine(Machine machines[], int* nbMachines) {
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
