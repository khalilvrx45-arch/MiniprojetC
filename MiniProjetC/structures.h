#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED

#include <time.h>

// Global ID counters
extern int prochain_id_pokemon;
extern int prochain_id_machine;
extern int prochain_id_commande;

// Enums
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

// Structures
typedef struct {
    float solde;
} Caisse;

typedef struct {
    int identifiant;
    char nom[50];
    float cout_unitaire;
    TypePokemon type;
} Pokemon;

// Forward declarations
struct Commande;
typedef struct Commande Commande;
struct Client;
typedef struct Client Client;

struct Client {
    char matricule[20];
    char nom[100];
    Commande** liste_commandes;
    int nb_commandes;
    int capacite_commandes;
};

struct Commande {
    int identifiant;
    Client* client;
    Pokemon* pokemon_commande;
    int quantite;
    time_t date_emission;
    EtatCommande etat;
};

typedef struct {
    int identifiant;
    char nom[50];
    Pokemon* pokemon_produit;
    int temps_production;
    int max_figurines_avant_maintenance;
    int compteur_figurines;
    float cout_maintenance;
    EtatMachine etat;
    Commande* commande_en_cours;
    time_t date_disponibilite;
} Machine;

// Utility function
const char* type_to_string(TypePokemon type);

#endif // STRUCTURES_H_INCLUDED
