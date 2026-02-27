#ifndef STRUCTURES_H_INCLUDED
#define STRUCTURES_H_INCLUDED
#include <time.h>

// ============================================================================
// COMPTEURS GLOBAUX D'IDENTIFIANTS
// ============================================================================
extern int prochain_id_pokemon;
extern int prochain_id_machine;
extern int prochain_id_commande;

// ============================================================================
// CONSTANTES DE CONFIGURATION
// ============================================================================
#define DUREE_MAINTENANCE_MINUTES 2  // ← PLACER ICI (après les compteurs, avant les enums)

// ============================================================================
// ÉNUMÉRATIONS
// ============================================================================

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
    REALISEE,
    PLANIFIEE
} EtatCommande;

// ============================================================================
// DÉCLARATIONS ANTICIPÉES (Forward declarations)
// ============================================================================
struct Commande;
typedef struct Commande Commande;
struct Client;
typedef struct Client Client;
struct Pokemon;
typedef struct Pokemon Pokemon;
struct Machine;
typedef struct Machine Machine;

typedef struct liste_pokemon liste_pokemon;
typedef struct liste_commande liste_commande;
typedef struct liste_client liste_client;
typedef struct liste_machine liste_machine;

// ============================================================================
// STRUCTURES PRINCIPALES
// ============================================================================

typedef struct {
    float solde;
} Caisse;

// ← PLACER ProductionPartielle ICI (après Caisse, avant Pokemon)
typedef struct {
    int quantite_restante;
    Commande* commande_originale;
} ProductionPartielle;

struct Pokemon {
    int identifiant;
    char nom[50];
    float cout_unitaire;
    TypePokemon type;
};

struct Client {
    char matricule[20];
    char nom[100];
    liste_commande* liste_commandes;
};

struct Commande {
    int identifiant;
    Client* client;
    Pokemon* pokemon_commande;
    int quantite;
    time_t date_emission;
    EtatCommande etat;
};

struct Machine {
    int identifiant;
    char nom[50];
    Pokemon* pokemon_produit;
    int temps_production;
    int max_figurines_avant_maintenance;
    int compteur_figurines;
    int total_produit; // Lifetime production for stats
    float cout_maintenance;
    int temps_maintenance;
    EtatMachine etat;
    Commande* commande_en_cours;
    liste_commande* file_attente;
    time_t date_disponibilite;
};

// ============================================================================
// STRUCTURES DE LISTES CHAÎNÉES
// ============================================================================

struct liste_pokemon {
    Pokemon pokemon;
    liste_pokemon* suivant;
};

struct liste_commande {
    Commande commande;
    liste_commande* suivant;
};

struct liste_client {
    Client client;
    liste_client* suivant;
};

struct liste_machine {
    Machine machine;
    liste_machine* suivant;
};

// ============================================================================
// FONCTIONS UTILITAIRES
// ============================================================================
const char* type_to_string(TypePokemon type);

#endif // STRUCTURES_H_INCLUDED


