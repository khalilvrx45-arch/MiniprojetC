#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "pokemon.h"
#include "machine.h"
#include "client.h"
#include "commande.h"
#include "fichiers.h"  // Nouveau fichier pour la gestion des fichiers

// Prototypes des fonctions de menu
void Menu_Principal();
void Menu_Pokemon(liste_pokemon** liste_pokemons);
void Menu_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons,
                  liste_commande* liste_commandes);
void Menu_Client(liste_client** liste_clients);
void Menu_Commande(liste_commande** liste_commandes, liste_client* liste_clients,
                   liste_pokemon* liste_pokemons, liste_machine* liste_machines);
int Demander_Continuer();
void afficher_etat_usine(liste_pokemon* liste_pokemons, liste_machine* liste_machines,
                         liste_client* liste_clients, liste_commande* liste_commandes);

int main() {
    // Initialisation des listes chaînées
    liste_pokemon* liste_pokemons = NULL;
    liste_machine* liste_machines = NULL;
    liste_client* liste_clients = NULL;
    liste_commande* liste_commandes = NULL;

    printf("=== Chargement des donnees depuis les fichiers ===\n");

    // ÉTAPE 1: Charger les données depuis les fichiers
    charger_pokemons_depuis_fichier(&liste_pokemons);
    charger_clients_depuis_fichier(&liste_clients);
    charger_machines_depuis_fichier(&liste_machines, liste_pokemons);
    charger_commandes_depuis_fichier(&liste_commandes, liste_clients, liste_pokemons);

    printf("Donnees chargees avec succes!\n");
    printf("  - %d Pokemon(s)\n", compter_pokemons(liste_pokemons));
    printf("  - %d Machine(s)\n", compter_machines(liste_machines));
    printf("  - %d Client(s)\n", compter_clients(liste_clients));
    printf("  - %d Commande(s)\n", compter_commandes(liste_commandes));

    int continuer = 1;
    int choix;

    // ÉTAPE 2: Travailler avec les listes chaînées
    while (continuer) {
        Menu_Principal();
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                Menu_Pokemon(&liste_pokemons);
                break;
            case 2:
                Menu_Machine(&liste_machines, liste_pokemons, liste_commandes);
                break;
            case 3:
                Menu_Client(&liste_clients);
                break;
            case 4:
                Menu_Commande(&liste_commandes, liste_clients, liste_pokemons, liste_machines);
                break;
            case 5:
                afficher_etat_usine(liste_pokemons, liste_machines,
                                   liste_clients, liste_commandes);
                break;
            case 6:
                printf("\n=== Sauvegarde des donnees dans les fichiers ===\n");

                // ÉTAPE 3: Sauvegarder les données dans les fichiers
                sauvegarder_pokemons_dans_fichier(liste_pokemons);
                sauvegarder_clients_dans_fichier(liste_clients);
                sauvegarder_machines_dans_fichier(liste_machines);
                sauvegarder_commandes_dans_fichier(liste_commandes);

                printf("Donnees sauvegardees avec succes!\n");

                // ÉTAPE 4: Libérer la mémoire APRÈS la sauvegarde
                printf("Liberation de la memoire...\n");
                liberer_liste_pokemons(&liste_pokemons);
                liberer_liste_machines(&liste_machines);
                liberer_liste_clients(&liste_clients);
                liberer_liste_commandes(&liste_commandes);

                printf("Merci d'avoir utilise l'usine Pokemon. Au revoir!\n");
                continuer = 0;
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
                break;
        }

        if (continuer && choix >= 1 && choix <= 5) {
            continuer = Demander_Continuer();
        }
    }

    return 0;
}

void Menu_Principal() {
    printf("\n=============================\n");
    printf("====   Usine POKEMON     ====\n");
    printf("=============================\n");
    printf("1. Gerer les Pokemon\n");
    printf("2. Gerer les machines\n");
    printf("3. Gerer les clients\n");
    printf("4. Gerer les commandes\n");
    printf("5. Afficher l'etat de l'usine\n");
    printf("6. Quitter\n");
    printf("Votre choix: ");
}

void Menu_Pokemon(liste_pokemon** liste_pokemons) {
    int sous_choix;

    printf("\n==== GERER POKEMON ====\n");
    printf("1. Ajouter un Pokemon\n");
    printf("2. Afficher un Pokemon par ID\n");
    printf("3. Modifier un Pokemon\n");
    printf("4. Supprimer un Pokemon\n");
    printf("5. Afficher tous les Pokemons\n");
    printf("6. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            ajouter_Pokemon(liste_pokemons);
            break;
        case 2: {
            int id;
            printf("Donner l'ID du Pokemon a chercher: ");
            scanf("%d", &id);
            Pokemon* p = trouver_pokemon_par_id(*liste_pokemons, id);
            if (p != NULL) {
                afficher_un_Pokemon(p);
            } else {
                printf("Pokemon ID %d non trouve.\n", id);
            }
            break;
        }
        case 3:
            modifier_Pokemon(*liste_pokemons);
            break;
        case 4:
            supprimer_Pokemon(liste_pokemons);
            break;
        case 5:
            afficher_Pokemons(*liste_pokemons);
            break;
        case 6:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons,
                  liste_commande* liste_commandes) {
    int sous_choix;

    printf("\n==== GERER MACHINE ====\n");
    printf("1. Afficher les machines\n");
    printf("2. Ajouter une machine\n");
    printf("3. Modifier une machine\n");
    printf("4. Supprimer une machine\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Machines(*liste_machines);
            break;
        case 2:
            ajouter_Machine(liste_machines, liste_pokemons);
            break;
        case 3:
            modifier_Machine(*liste_machines, liste_pokemons);
            break;
        case 4:
            supprimer_Machine(liste_machines, liste_commandes);
            break;
        case 5:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Client(liste_client** liste_clients) {
    int sous_choix;

    printf("\n==== GERER CLIENT ====\n");
    printf("1. Afficher les clients\n");
    printf("2. Ajouter un client\n");
    printf("3. Modifier un client\n");
    printf("4. Supprimer un client\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Clients(*liste_clients);
            break;
        case 2:
            ajouter_Client(liste_clients);
            break;
        case 3:
            modifier_Client(*liste_clients);
            break;
        case 4:
            supprimer_Client(liste_clients);
            break;
        case 5:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Commande(liste_commande** liste_commandes, liste_client* liste_clients,
                   liste_pokemon* liste_pokemons, liste_machine* liste_machines) {
    int sous_choix;

    printf("\n==== GERER COMMANDE ====\n");
    printf("1. Afficher les commandes\n");
    printf("2. Afficher une commande par ID\n");
    printf("3. Ajouter une commande\n");
    printf("4. Modifier une commande\n");
    printf("5. Supprimer une commande\n");
    printf("6. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Commandes(*liste_commandes);
            break;
        case 2: {
            int id;
            printf("Donner l'ID de la commande: ");
            scanf("%d", &id);
            Commande* cmd = trouver_commande_par_id(*liste_commandes, id);
            if (cmd != NULL) {
                afficher_une_Commande(cmd);
            } else {
                printf("Commande ID %d non trouvee.\n", id);
            }
            break;
        }
        case 3: {
            if (liste_clients == NULL) {
                printf("Erreur: Aucun client disponible!\n");
                break;
            }
            if (liste_pokemons == NULL) {
                printf("Erreur: Aucun Pokemon disponible!\n");
                break;
            }

            printf("\n=== Clients disponibles ===\n");
            afficher_Clients(liste_clients);

            char matricule[20];
            printf("\nMatricule du client: ");
            scanf(" %19[^\n]", matricule);

            Client* client = trouver_client_par_matricule(liste_clients, matricule);
            if (client == NULL) {
                printf("Client non trouve!\n");
                break;
            }

            printf("\n=== Pokemons disponibles ===\n");
            afficher_Pokemons(liste_pokemons);

            int id_pokemon;
            printf("\nID du Pokemon: ");
            scanf("%d", &id_pokemon);

            Pokemon* pokemon = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
            if (pokemon == NULL) {
                printf("Pokemon non trouve!\n");
                break;
            }

            int quantite;
            do {
                printf("Quantite: ");
                scanf("%d", &quantite);
                if (quantite <= 0) {
                    printf("La quantite doit etre positive!\n");
                }
            } while (quantite <= 0);

            ajouter_Commande(liste_commandes, client, pokemon, quantite);
            break;
        }
        case 4:
            modifier_Commande(*liste_commandes);
            break;
        case 5:
            supprimer_Commande(liste_commandes);
            break;
        case 6:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

int Demander_Continuer() {
    char reponse;
    printf("\nVoulez-vous continuer? (o/n): ");
    scanf(" %c", &reponse);

    if (reponse == 'o' || reponse == 'O') {
        return 1;
    } else {
        printf("Au revoir!\n");
        return 0;
    }
}

void afficher_etat_usine(liste_pokemon* liste_pokemons, liste_machine* liste_machines,
                         liste_client* liste_clients, liste_commande* liste_commandes) {
    printf("\n======================================\n");
    printf("====    ETAT DE L'USINE POKEMON   ====\n");
    printf("======================================\n");

    int nb_pokemons = compter_pokemons(liste_pokemons);
    int nb_machines = compter_machines(liste_machines);
    int nb_clients = compter_clients(liste_clients);
    int nb_commandes = compter_commandes(liste_commandes);

    printf("Nombre de Pokemon     : %d\n", nb_pokemons);
    printf("Nombre de Machines    : %d\n", nb_machines);
    printf("Nombre de Clients     : %d\n", nb_clients);
    printf("Nombre de Commandes   : %d\n", nb_commandes);

    if (liste_machines != NULL) {
        printf("\n--- Etat des machines ---\n");
        int libres = compter_machines_par_etat(liste_machines, LIBRE);
        int occupees = compter_machines_par_etat(liste_machines, OCCUPEE);
        int maintenance = compter_machines_par_etat(liste_machines, EN_ATTENTE_MAINTENANCE);

        printf("  Libres               : %d\n", libres);
        printf("  Occupees             : %d\n", occupees);
        printf("  En maintenance       : %d\n", maintenance);
    }

    if (liste_commandes != NULL) {
        printf("\n--- Etat des commandes ---\n");
        int en_attente = compter_commandes_par_etat(liste_commandes, EN_ATTENTE);
        int en_cours = compter_commandes_par_etat(liste_commandes, EN_COURS);
        int realisees = compter_commandes_par_etat(liste_commandes, REALISEE);

        printf("  En attente           : %d\n", en_attente);
        printf("  En cours             : %d\n", en_cours);
        printf("  Realisees            : %d\n", realisees);
    }

    printf("======================================\n");
}
```

/*## **Ordre d'exécution correct :**
```
DÉMARRAGE DU PROGRAMME
    ↓
1. CHARGEMENT depuis fichiers
    - Pokemon.txt → liste_pokemons
    - clients.txt → liste_clients
    - machines.txt → liste_machines
    - commandes.txt → liste_commandes
    ↓
2. TRAVAIL avec les listes chaînées
    - Ajout, modification, suppression
    - Toutes les opérations en mémoire
    ↓
3. SAUVEGARDE dans les fichiers
    - liste_pokemons → Pokemon.txt
    - liste_clients → clients.txt
    - liste_machines → machines.txt
    - liste_commandes → commandes.txt
    ↓
4. LIBÉRATION de la mémoire
    - free() de tous les nœuds
    ↓
FIN DU PROGRAMME*/
