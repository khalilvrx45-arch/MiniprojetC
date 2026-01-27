#include <stdio.h>
#include <stdlib.h>
#include "fonction.h"
#define MAX_POKEMON 100
#define MAX_MACHINES 50
#define MAX_CLIENTS 100
#define MAX_COMMANDES 200

int main()
{
    Pokemon tabPokemon[MAX_POKEMON];
    int nb_Pokemon = 0;

    Machine tabMachine[MAX_MACHINES];
    int nb_Machine = 0;

    Client tabClient[MAX_CLIENTS];
    int nb_Client = 0;

    Commande tabCommande[MAX_COMMANDES];
    int nb_Commande = 0;

    int choix,sous_choix;

    printf("==== Usine POKMON ====\n");
    printf("1. Gerer les Pokemon\n");
    printf("2. Gerer les machines\n");
    printf("3. Gerer les clinets\n");
    printf("4. Gerer les commandes\n");
    printf("5. Afficher l'etat de l'unsine\n");
    printf("6. Quitter\n");
    scanf("%d",&choix);
    switch(choix){
    case 1:
        printf("==== GERER POKEMON =====\n");
        printf("1. Ajouter un Pokemon\n");
        printf("2. Afficher les Pokemons\n");
        printf("3. Modifier un Pokemon\n");
        printf("4. Supprimer un Pokemon\n");
        printf("5. Revenir au menu precedent\n");
        scanf("%d",&sous_choix);
        switch (sous_choix) {
            case 1 :
                 Ajouter_Pokemon(tabPokemon, &nb_Pokemon, MAX_POKEMON);
                 break;
            case 2 : {
                int id;
                printf("Donner l'id de pokemon a cherche : ");
                scanf("%d",&id);
                Afficher_Pokemon(tabPokemon,nb_Pokemon,id);
                break;
            }
            case 3 :
                Modifier_Pokemon(tabPokemon,nb_Pokemon);
                break;
            case 4 : {
                int id;
                printf("Donner l'id de pokemon a supprimer\n");
                scanf("%d",&id);
                Supprimer_Pokemon(id,tabPokemon,&nb_Pokemon);
            }
            case 5:
                printf("Retour au menu principal...\n");
                break;
        }
        break;

    case 2:
        printf("==== GERER MACHINE =====\n");
        printf("1. Affincher le(s) machine(s)\n");
        printf("2. Ajouter une machine\n");
        printf("3. Modifier une machine\n");
        printf("4. Supprimer une machine\n");
        printf("5. Revenir au menu precedent\n");
        break;
    case 3:
        printf("==== GERER CLIENT =====\n");
        printf("1. Affincher le(s) client(s)\n");
        printf("2. Ajouter un client\n");
        printf("3. Modifier un client\n");
        printf("4. Supprimer un client\n");
        printf("5. Revenir au menu precedent\n");
        break;
    case 4:
        printf("==== GERER COMMANDE =====\n");
        printf("1. Affincher le(s) commande(s)\n");
        printf("2. Ajouter une commande\n");
        printf("3. Modifier une commande\n");
        printf("4. Supprimer une commande\n");
        printf("5. Revenir au menu precedent\n");
        break;



    }
}

