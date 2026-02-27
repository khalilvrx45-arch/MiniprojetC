#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pokemon.h"
#include "machine.h"
#include "commande.h"

// ============================================================================
// FONCTIONS DE RECHERCHE
// ============================================================================

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

Machine* trouver_machine_libre_pour_pokemon(liste_machine* liste_machines, Pokemon* pokemon) {
    liste_machine* courant = liste_machines;
    Machine* meilleure_machine = NULL;
    int min_file = 999;

    while (courant != NULL) {
        if (courant->machine.pokemon_produit != NULL && pokemon != NULL && 
            courant->machine.pokemon_produit->identifiant == pokemon->identifiant) {
            
            if (courant->machine.etat == LIBRE) {
                return &(courant->machine);
            }

            int taille_file = 0;
            liste_commande* c = courant->machine.file_attente;
            while (c != NULL) {
                taille_file++;
                c = c->suivant;
            }

            if (taille_file < min_file) {
                min_file = taille_file;
                meilleure_machine = &(courant->machine);
            }
        }
        courant = courant->suivant;
    }
    return meilleure_machine;
}

// ============================================================================
// FONCTIONS DE COMPTAGE
// ============================================================================

int compter_machines(liste_machine* liste_machines) {
    int count = 0;
    liste_machine* courant = liste_machines;
    while (courant != NULL) {
        count++;
        courant = courant->suivant;
    }
    return count;
}

// ============================================================================
// GESTION DE PRODUCTION AVEC TEMPS
// ============================================================================

void lancer_production(Machine* machine, Commande* commande, int quantite_a_produire) {
    if (machine == NULL || commande == NULL) return;
    if (machine->etat != LIBRE) return;

    int final_compteur;
    int minutes_totales = simuler_temps_et_compteur(machine, quantite_a_produire, machine->compteur_figurines, &final_compteur);
    
    machine->etat = OCCUPEE;
    machine->commande_en_cours = commande;
    commande->etat = EN_COURS;
    machine->date_disponibilite = time(NULL) + (minutes_totales * 60);
}

void terminer_production(Machine* machine, Caisse* caisse) {
    if (machine == NULL || machine->etat != OCCUPEE || machine->commande_en_cours == NULL) return;
    if (!production_terminee(machine)) return;

    Commande* commande = machine->commande_en_cours;
    int final_compteur;
    simuler_temps_et_compteur(machine, commande->quantite, machine->compteur_figurines, &final_compteur);

    if (caisse != NULL) {
        caisse->solde += commande->quantite * commande->pokemon_commande->cout_unitaire;
    }

    machine->compteur_figurines = final_compteur;
    machine->total_produit += commande->quantite;
    commande->etat = REALISEE;
    machine->commande_en_cours = NULL;

    if (machine->compteur_figurines >= machine->max_figurines_avant_maintenance) {
        machine->etat = EN_ATTENTE_MAINTENANCE;
        machine->date_disponibilite = time(NULL) + (machine->temps_maintenance * 60);
    } else {
        machine->etat = LIBRE;
        machine->date_disponibilite = time(NULL);
    }
}

void effectuer_maintenance(Machine* machine, Caisse* caisse) {
    if (machine == NULL || machine->etat != EN_ATTENTE_MAINTENANCE || !maintenance_terminee(machine)) return;
    if (caisse != NULL && caisse->solde < machine->cout_maintenance) return;

    if (caisse != NULL) caisse->solde -= machine->cout_maintenance;
    machine->compteur_figurines = 0;
    machine->etat = LIBRE;
    machine->date_disponibilite = time(NULL);
}

// ============================================================================
// VÉRIFICATION DU TEMPS
// ============================================================================

int production_terminee(Machine* machine) {
    if (machine == NULL || machine->etat != OCCUPEE) return 0;
    return (time(NULL) >= machine->date_disponibilite);
}

int maintenance_terminee(Machine* machine) {
    if (machine == NULL || machine->etat != EN_ATTENTE_MAINTENANCE) return 0;
    return (time(NULL) >= machine->date_disponibilite);
}

// ============================================================================
// SIMULATION ET FILE
// ============================================================================

int simuler_temps_et_compteur(Machine* machine, int quantite, int compteur_depart, int* compteur_final) {
    if (machine == NULL || quantite <= 0) return 0;

    int temps_total = 0;
    int compteur = compteur_depart;
    int qte_restante = quantite;

    while (qte_restante > 0) {
        int capacite_avant_maint = machine->max_figurines_avant_maintenance - compteur;
        if (capacite_avant_maint > 0) {
            int a_produire = (qte_restante < capacite_avant_maint) ? qte_restante : capacite_avant_maint;
            temps_total += a_produire * machine->temps_production;
            compteur += a_produire;
            qte_restante -= a_produire;
        }
        if (compteur >= machine->max_figurines_avant_maintenance && qte_restante > 0) {
            temps_total += machine->temps_maintenance;
            compteur = 0;
        }
    }
    if (compteur_final != NULL) *compteur_final = compteur;
    return temps_total;
}

void ajouter_a_file(Machine* machine, Commande* commande) {
    if (machine == NULL || commande == NULL) return;

    liste_commande* nouveau = (liste_commande*)malloc(sizeof(liste_commande));
    if (nouveau == NULL) return;

    nouveau->commande = *commande;
    nouveau->commande.etat = PLANIFIEE;
    commande->etat = PLANIFIEE;
    nouveau->suivant = NULL;

    if (machine->file_attente == NULL) {
        machine->file_attente = nouveau;
    } else {
        liste_commande* courant = machine->file_attente;
        while (courant->suivant != NULL) courant = courant->suivant;
        courant->suivant = nouveau;
    }
}

Commande* retirer_de_file(Machine* machine) {
    if (machine == NULL || machine->file_attente == NULL) return NULL;
    liste_commande* a_retirer = machine->file_attente;
    Commande* cmd = (Commande*)malloc(sizeof(Commande));
    if (cmd != NULL) *cmd = a_retirer->commande;
    machine->file_attente = a_retirer->suivant;
    free(a_retirer);
    return cmd;
}

// ============================================================================
// LIBÉRATION MÉMOIRE
// ============================================================================

void liberer_liste_machines(liste_machine** liste_machines) {
    if (liste_machines == NULL || *liste_machines == NULL) return;
    liste_machine* courant = *liste_machines;
    while (courant != NULL) {
        liste_machine* suivant = courant->suivant;
        liste_commande* cmd_courant = courant->machine.file_attente;
        while (cmd_courant != NULL) {
            liste_commande* cmd_suiv = cmd_courant->suivant;
            free(cmd_courant);
            cmd_courant = cmd_suiv;
        }
        free(courant);
        courant = suivant;
    }
    *liste_machines = NULL;
}

// ============================================================================
// MISE À JOUR AUTOMATIQUE
// ============================================================================

void mettre_a_jour_machines(liste_machine* liste_machines, Caisse* caisse, liste_commande* liste_commandes) {
    if (liste_machines == NULL) return;
    liste_machine* courant = liste_machines;

    while (courant != NULL) {
        Machine* m = &(courant->machine);

        if (m->etat == LIBRE && m->file_attente != NULL) {
            Commande* copie = retirer_de_file(m);
            if (copie != NULL) {
                Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                if (vraie_commande != NULL) lancer_production(m, vraie_commande, vraie_commande->quantite);
                free(copie);
            }
        }

        if (m->etat == OCCUPEE && production_terminee(m)) {
            terminer_production(m, caisse);
            if (m->etat == LIBRE && m->file_attente != NULL) {
                Commande* copie = retirer_de_file(m);
                if (copie != NULL) {
                    Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                    if (vraie_commande != NULL) lancer_production(m, vraie_commande, vraie_commande->quantite);
                    free(copie);
                }
            }
        }

        if (m->etat == EN_ATTENTE_MAINTENANCE && maintenance_terminee(m)) {
            effectuer_maintenance(m, caisse);
            if (m->etat == LIBRE && m->file_attente != NULL) {
                Commande* copie = retirer_de_file(m);
                if (copie != NULL) {
                    Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                    if (vraie_commande != NULL) lancer_production(m, vraie_commande, vraie_commande->quantite);
                    free(copie);
                }
            }
        }
        courant = courant->suivant;
    }
}

// ============================================================================
// GESTION DES COMMANDES ET MACHINES (CASCADE)
// ============================================================================

void supprimer_commande_de_toutes_files(liste_machine* liste, int id_commande) {
    liste_machine* curr_m = liste;
    while (curr_m) {
        Machine* m = &curr_m->machine;
        if (m->commande_en_cours && m->commande_en_cours->identifiant == id_commande) {
            m->commande_en_cours = NULL;
            m->etat = LIBRE;
            m->date_disponibilite = time(NULL);
        }
        liste_commande* prev_c = NULL;
        liste_commande* curr_c = m->file_attente;
        while (curr_c) {
            if (curr_c->commande.identifiant == id_commande) {
                liste_commande* a_suppr = curr_c;
                if (prev_c) prev_c->suivant = curr_c->suivant;
                else m->file_attente = curr_c->suivant;
                curr_c = curr_c->suivant;
                free(a_suppr);
            } else {
                prev_c = curr_c;
                curr_c = curr_c->suivant;
            }
        }
        curr_m = curr_m->suivant;
    }
}

void liberer_commandes_machine_supprimee(Machine* m) {
    if (!m) return;
    if (m->commande_en_cours) {
        m->commande_en_cours->etat = EN_ATTENTE;
        m->commande_en_cours = NULL;
    }
    liste_commande* curr = m->file_attente;
    while (curr) {
        curr->commande.etat = EN_ATTENTE;
        liste_commande* a_suppr = curr;
        curr = curr->suivant;
        free(a_suppr);
    }
    m->file_attente = NULL;
}

void supprimer_machines_par_pokemon(liste_machine** liste_machines, int id_pokemon) {
    if (liste_machines == NULL || *liste_machines == NULL) return;
    liste_machine* courant = *liste_machines;
    liste_machine* precedent = NULL;
    while (courant != NULL) {
        if (courant->machine.pokemon_produit != NULL && courant->machine.pokemon_produit->identifiant == id_pokemon) {
            liberer_commandes_machine_supprimee(&(courant->machine));
            liste_machine* a_supprimer = courant;
            if (precedent == NULL) {
                *liste_machines = courant->suivant;
                courant = *liste_machines;
            } else {
                precedent->suivant = courant->suivant;
                courant = courant->suivant;
            }
            free(a_supprimer);
        } else {
            precedent = courant;
            courant = courant->suivant;
        }
    }
}
