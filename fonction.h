#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED
#include <string.h>
#include <time.h>
#define MAX_MACHINES 100
#define MAX_CLIENTS 100
extern int prochain_id_pokemon;
extern int prochain_id_machine;
extern int prochain_id_commande;
typedef enum {
    ELECTRIQUE,
    FEU,
    EAU,
    PLANTE,
} TypePokemon;

typedef enum {
    LIBRE,
    OCCUPEE,
    EN_ATTENTE_MAINTENANCE
} EtatMachine;

typedef enum {
    EN_ATTENTE,
    EN_COURS,
    REALISEE
} EtatCommande;



typedef struct {
    float solde;  // Solde actuel de la caisse (initialisé à 1000 pièces)
} Caisse;


typedef struct  {
    int identifiant;           // Généré automatiquement (auto-incrémenté)
    char nom[50];              // Nom du Pokémon (ex: "Pikachu", "Salamèche")
    float cout_unitaire;       // Prix de vente ou coût de fabrication
    TypePokemon type;          // Type du Pokémon (optionnel)
}Pokemon;
struct Commande;
typedef struct Commande Commande;
struct Client;
typedef struct Client Client;
// 2.2.5 Les Commandes
struct Client {
    char matricule[20];         // Identifiant unique du client
    char nom[100];              // Nom du client
    Commande** liste_commandes; // Tableau dynamique de pointeurs vers commandes
    int nb_commandes;           // Nombre de commandes passées
    int capacite_commandes;     // Capacité du tableau (pour gestion dynamique)
};
struct Commande  {
    int identifiant;              // Généré automatiquement
    Client* client;               // Pointeur vers le client
    Pokemon* pokemon_commande;    // Pokémon commandé
    int quantite;                 // Nombre de figurines demandées
    time_t date_emission;         // Date et heure de la commande (time.h)
    EtatCommande etat;            // État actuel de la commande
};
// 2.2.4 Les Clients

// 2.2.3 Les Machines
typedef struct {
    int identifiant;                    // Identifiant unique de la machine
    char nom[50];                       // Nom de la machine
    Pokemon* pokemon_produit;           // Pointeur vers le Pokémon produit
    int temps_production;               // Temps pour produire 1 figurine (en minutes)
    int max_figurines_avant_maintenance; // Capacité avant maintenance obligatoire
    int compteur_figurines;             // Nombre de figurines produites depuis dernière maintenance
    float cout_maintenance;             // Coût d'une opération de maintenance
    EtatMachine etat;                   // État actuel de la machine
    Commande* commande_en_cours;        // Pointeur vers la commande en cours (NULL si aucune)
    time_t date_disponibilite;          // Date/heure de disponibilité (optionnel)
} Machine;

const char* type_to_string(TypePokemon type);
/*les fonctions des pokemons*/
void Afficher_tous_pokemons(Pokemon tab[], int nb_pokemon);
void Afficher_Pokemon(Pokemon tab[], int nb_pokemon, int id);
void Ajouter_Pokemon(Pokemon pokemons[], int *nb_pokemons, int max_pokemons);
void Modifier_Pokemon(Pokemon pokemons[], int nb_pokemons);
void Supprimer_Pokemon(int id, Pokemon tab[], int *nb_pokemons);
void Afficher_les_pokemons(Pokemon tab[],int nb_Pokemon);
/*les fonctions des commandes*/
int Afficher_commande_par_id(Commande commandes[], int nb_commandes, int id);
void Ajouter_Commande(Commande commandes[], int *nb_commandes, int capacite, Client *client ,Pokemon *pokemon, int quantite);
int Modifier_Commande(Commande commandes[], int *nb_commandes, int id_commande, Client *nouveau_client, Pokemon *nouveau_pokemon, int nouvelle_quantite, EtatCommande nouvel_etat);
int Supprimer_Commande(Commande commandes[], int *nb_commandes, int id_commande);
/*les fonctions des machines*/
void afficher_Machines(Machine machines[], int nbMachines);
void ajouter_Machine(Machine machines[], int* nbMachines, Pokemon pokemons[], int nbPokemons);
void modifier_Machine(Machine machines[], int nbMachines, Pokemon pokemons[], int nbPokemons);
void supprimer_Machine(Machine machines[], int* nbMachines);
/*les fonctions des clients*/
void afficher_Clients(Client clients[], int nbClients);
void ajouter_Client(Client clients[], int* nbClients);
void modifier_Client(Client clients[], int nbClients);
void supprimer_Client(Client clients[], int* nbClients);

#endif // STRUCTURES_H_INCLUDED
