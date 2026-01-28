#include "structures.h"

// Initialize global ID counters
int prochain_id_pokemon = 1;
int prochain_id_machine = 1;
int prochain_id_commande = 1;

// Utility function to convert type enum to string
const char* type_to_string(TypePokemon type) {
    switch (type) {
        case ELECTRIQUE: return "Électrique";
        case FEU: return "Feu";
        case EAU: return "Eau";
        case PLANTE: return "Plante";
        default: return "Inconnu";
    }
}
