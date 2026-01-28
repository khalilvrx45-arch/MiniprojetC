#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"
#define MAX_POKEMON 100
#define MAX_MACHINES 50
#define MAX_CLIENTS 100
#define MAX_COMMANDES 200

// Prototypes des fonctions de menu
void Menu_Principal();
void Menu_Pokemon(Pokemon tabPokemon[], int *nb_Pokemon, int *prochain_id);
void Menu_Machine(Machine tabMachine[], int *nb_Machine, int *prochain_id);
void Menu_Client(Client tabClient[], int *nb_Client);
void Menu_Commande(Commande tabCommande[], int *nb_Commande, int *prochain_id);
int Demander_Continuer();

int main()
{
    int prochain_id_pokemon = 1;
    int prochain_id_machine = 1;
    int prochain_id_commande = 1;

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
                Menu_Machine(tabMachine, &nb_Machine, &prochain_id_machine);
                break;
            case 3:
                Menu_Client(tabClient, &nb_Client);
                break;
            case 4:
                Menu_Commande(tabCommande, &nb_Commande, &prochain_id_commande);
                break;
            case 5:
                // Afficher l'état de l'usine
                printf("\n==== ETAT DE L'USINE ====\n");
                printf("Nombre de Pokemon: %d\n", nb_Pokemon);
                printf("Nombre de Machines: %d\n", nb_Machine);
                printf("Nombre de Clients: %d\n", nb_Client);
                printf("Nombre de Commandes: %d\n", nb_Commande);
                break;
            case 6:
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
        case 1: {
            // Ajouter un Pokemon (à implémenter)
            printf("Fonction Ajouter Pokemon a implementer\n");
            break;
        }
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

void Menu_Machine(Machine tabMachine[], int *nb_Machine, int *prochain_id) {
    int sous_choix;

    printf("\n==== GERER MACHINE ====\n");
    printf("1. Afficher le(s) machine(s)\n");
    printf("2. Ajouter une machine\n");
    printf("3. Modifier une machine\n");
    printf("4. Supprimer une machine\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    // À implémenter selon vos fonctions
    printf("Fonction a implementer\n");
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

    // À implémenter selon vos fonctions
    printf("Fonction a implementer\n");
}

void Menu_Commande(Commande tabCommande[], int *nb_Commande, int *prochain_id) {
    int sous_choix;

    printf("\n==== GERER COMMANDE ====\n");
    printf("1. Afficher le(s) commande(s)\n");
    printf("2. Ajouter une commande\n");
    printf("3. Modifier une commande\n");
    printf("4. Supprimer une commande\n");
    printf("5. Retour au menu principal\n");
    printf("Votre choix: ");
    scanf("%d", &sous_choix);

    // À implémenter selon vos fonctions
    printf("Fonction a implementer\n");
}

int Demander_Continuer() {
    char reponse;
    printf("\nVoulez-vous continuer? (o/n): ");
    scanf(" %c", &reponse);

    if (reponse == 'o' || reponse == 'O') {
        return 1;  // Continuer
    } else {
        printf("Au revoir!\n");
        return 0;  // Quitter
    }
}
