#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "pokemon.h"
#include "machine.h"
#include "client.h"
#include "commande.h"

#define MAX_POKEMON 100
#define MAX_MACHINES 50
#define MAX_CLIENTS 100
#define MAX_COMMANDES 200

// Prototypes des fonctions de menu
void Menu_Principal();
void Menu_Pokemon(Pokemon tabPokemon[], int *nb_Pokemon, int *prochain_id);
void Menu_Machine(Machine tabMachine[], int *nb_Machine, int *prochain_id, 
                  Pokemon tabPokemon[], int nb_Pokemon);
void Menu_Client(Client tabClient[], int *nb_Client);
void Menu_Commande(Commande tabCommande[], int *nb_Commande, int *prochain_id,
                   Client tabClient[], int nb_Client, Pokemon tabPokemon[], int nb_Pokemon);
int Demander_Continuer();

int main() {
    Pokemon tabPokemon[MAX_POKEMON];
    int nb_Pokemon = 0;

    Machine tabMachine[MAX_MACHINES];
    int nb_Machine = 0;

    Client tabClient[MAX_CLIENTS];
    int nb_Client = 0;

    Commande tabCommande[MAX_COMMANDES];
    int nb_Commande = 0;

    int continuer = 1;
    int choix;

    while (continuer) {
        Menu_Principal();
        scanf("%d", &choix);

        switch(choix) {
            case 1:
                Menu_Pokemon(tabPokemon, &nb_Pokemon, &prochain_id_pokemon);
                break;
            case 2:
                Menu_Machine(tabMachine, &nb_Machine, &prochain_id_machine, 
                            tabPokemon, nb_Pokemon);
                break;
            case 3:
                Menu_Client(tabClient, &nb_Client);
                break;
            case 4:
                Menu_Commande(tabCommande, &nb_Commande, &prochain_id_commande,
                             tabClient, nb_Client, tabPokemon, nb_Pokemon);
                break;
            case 5:
                printf("\n==== ETAT DE L'USINE ====\n");
                printf("Nombre de Pokemon: %d\n", nb_Pokemon);
                printf("Nombre de Machines: %d\n", nb_Machine);
                printf("Nombre de Clients: %d\n", nb_Client);
                printf("Nombre de Commandes: %d\n", nb_Commande);
                break;
            case 6:
                printf("Merci d'avoir utilise l'usine Pokemon. Au revoir!\n");
                
                // Libérer la mémoire des clients
                for (int i = 0; i < nb_Client; i++) {
                    if (tabClient[i].liste_commandes != NULL) {
                        free(tabClient[i].liste_commandes);
                    }
                }
                
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
    printf("==== Usine POKEMON ====\n");
    printf("=============================\n");
    printf("1. Gerer les Pokemon\n");
    printf("2. Gerer les machines\n");
    printf("3. Gerer les clients\n");
    printf("4. Gerer les commandes\n");
    printf("5. Afficher l'etat de l'usine\n");
    printf("6. Quitter\n");
    printf("Votre choix: ");
}

void Menu_Pokemon(Pokemon tabPokemon[], int *nb_Pokemon, int *prochain_id) {
    int sous_choix;

    printf("\n==== GERER POKEMON ====\n");
    printf("1. Ajouter un Pokemon\n");
    printf("2. Afficher un Pokemon\n");
    printf("3. Modifier un Pokemon\n");
    printf("4. Supprimer un Pokemon\n");
    printf("5. Afficher tous les Pokemons\n");
    printf("6. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            Ajouter_Pokemon(tabPokemon, nb_Pokemon, MAX_POKEMON);
            break;
        case 2: {
            int id;
            printf("Donner l'id du Pokemon a chercher: ");
            scanf("%d", &id);
            Afficher_Pokemon(tabPokemon, *nb_Pokemon, id);
            break;
        }
        case 3:
            Modifier_Pokemon(tabPokemon, *nb_Pokemon);
            break;
        case 4: {
            int id;
            printf("Donner l'id du Pokemon a supprimer: ");
            scanf("%d", &id);
            Supprimer_Pokemon(id, tabPokemon, nb_Pokemon);
            break;
        }
        case 5:
            Afficher_tous_pokemons(tabPokemon, *nb_Pokemon);
            break;
        case 6:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Machine(Machine tabMachine[], int *nb_Machine, int *prochain_id,
                  Pokemon tabPokemon[], int nb_Pokemon) {
    int sous_choix;

    printf("\n==== GERER MACHINE ====\n");
    printf("1. Afficher le(s) machine(s)\n");
    printf("2. Ajouter une machine\n");
    printf("3. Modifier une machine\n");
    printf("4. Supprimer une machine\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Machines(tabMachine, *nb_Machine);
            break;
        case 2:
            ajouter_Machine(tabMachine, nb_Machine, tabPokemon, nb_Pokemon);
            break;
        case 3:
            modifier_Machine(tabMachine, *nb_Machine, tabPokemon, nb_Pokemon);
            break;
        case 4:
            supprimer_Machine(tabMachine, nb_Machine);
            break;
        case 5:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Client(Client tabClient[], int *nb_Client) {
    int sous_choix;

    printf("\n==== GERER CLIENT ====\n");
    printf("1. Afficher le(s) client(s)\n");
    printf("2. Ajouter un client\n");
    printf("3. Modifier un client\n");
    printf("4. Supprimer un client\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Clients(tabClient, *nb_Client);
            break;
        case 2:
            ajouter_Client(tabClient, nb_Client);
            break;
        case 3:
            modifier_Client(tabClient, *nb_Client);
            break;
        case 4:
            supprimer_Client(tabClient, nb_Client);
            break;
        case 5:
            printf("Retour au menu principal...\n");
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }
}

void Menu_Commande(Commande tabCommande[], int *nb_Commande, int *prochain_id,
                   Client tabClient[], int nb_Client, Pokemon tabPokemon[], int nb_Pokemon) {
    int sous_choix;

    printf("\n==== GERER COMMANDE ====\n");
    printf("1. Afficher le(s) commande(s)\n");
    printf("2. Afficher une commande par ID\n");
    printf("3. Ajouter une commande\n");
    printf("4. Modifier une commande\n");
    printf("5. Supprimer une commande\n");
    printf("6. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    switch (sous_choix) {
        case 1:
            afficher_Commandes(tabCommande, *nb_Commande);
            break;
        case 2: {
            int id;
            printf("Donner l'ID de la commande: ");
            scanf("%d", &id);
            Afficher_commande_par_id(tabCommande, *nb_Commande, id);
            break;
        }
        case 3: {
            if (nb_Client == 0) {
                printf("Erreur: Aucun client disponible!\n");
                break;
            }
            if (nb_Pokemon == 0) {
                printf("Erreur: Aucun Pokemon disponible!\n");
                break;
            }

            // Afficher les clients
            printf("\nClients disponibles:\n");
            for (int i = 0; i < nb_Client; i++) {
                printf("%d. %s (Matricule: %s)\n", i + 1, 
                       tabClient[i].nom, tabClient[i].matricule);
            }

            int choix_client;
            do {
                printf("Choisissez un client (1-%d): ", nb_Client);
                scanf("%d", &choix_client);
            } while (choix_client < 1 || choix_client > nb_Client);

            // Afficher les Pokemons
            printf("\nPokemons disponibles:\n");
            for (int i = 0; i < nb_Pokemon; i++) {
                printf("%d. %s (ID: %d, Prix: %.2f)\n", i + 1, 
                       tabPokemon[i].nom, tabPokemon[i].identifiant,
                       tabPokemon[i].cout_unitaire);
            }

            int choix_pokemon;
            do {
                printf("Choisissez un Pokemon (1-%d): ", nb_Pokemon);
                scanf("%d", &choix_pokemon);
            } while (choix_pokemon < 1 || choix_pokemon > nb_Pokemon);

            int quantite;
            do {
                printf("Quantite: ");
                scanf("%d", &quantite);
            } while (quantite <= 0);

            Ajouter_Commande(tabCommande, nb_Commande, MAX_COMMANDES,
                           &tabClient[choix_client - 1],
                           &tabPokemon[choix_pokemon - 1],
                           quantite);
            break;
        }
        case 4: {
            int id;
            printf("ID de la commande a modifier: ");
            scanf("%d", &id);

            printf("Nouvelle quantite (0 pour ne pas changer): ");
            int nouvelle_quantite;
            scanf("%d", &nouvelle_quantite);

            printf("Nouvel etat (0:EN_ATTENTE, 1:EN_COURS, 2:REALISEE, -1 pour ne pas changer): ");
            int nouvel_etat;
            scanf("%d", &nouvel_etat);

            modifier_Commande(tabCommande, nb_Commande, id,
                            NULL, NULL, nouvelle_quantite,
                            (nouvel_etat >= 0 && nouvel_etat <= 2) ? (EtatCommande)nouvel_etat : -1);
            break;
        }
        case 5: {
            int id;
            printf("ID de la commande a supprimer: ");
            scanf("%d", &id);
            supprimer_Commande(tabCommande, nb_Commande, id);
            break;
        }
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
