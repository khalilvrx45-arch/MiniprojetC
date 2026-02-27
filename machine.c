#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "pokemon.h"
#include "machine.h"
#include "commande.h"

// ============================================================================
// FONCTIONS D'AFFICHAGE
// ============================================================================

void afficher_Machines(liste_machine* liste_machines) {
    if (liste_machines == NULL) {
        printf("\n=== AUCUNE MACHINE ENREGISTREE ===\n");
        return;
    }

    int count = compter_machines(liste_machines);
    printf("\n=============== LISTE DES MACHINES (%d) ===============\n", count);

    liste_machine* courant = liste_machines;
    int numero = 1;

    while (courant != NULL) {
        printf("\n--- Machine #%d ---\n", numero);
        printf("ID: %d\n", courant->machine.identifiant);
        printf("Nom: %s\n", courant->machine.nom);

        if (courant->machine.pokemon_produit != NULL) {
            printf("Pokemon produit: %s (ID: %d)\n",
                   courant->machine.pokemon_produit->nom,
                   courant->machine.pokemon_produit->identifiant);
        } else {
            printf("Pokemon produit: Aucun\n");
        }

        printf("Temps de production: %d min\n", courant->machine.temps_production);
        printf("Max avant maintenance: %d\n", courant->machine.max_figurines_avant_maintenance);
        printf("Compteur production: %d / %d\n",
               courant->machine.compteur_figurines,
               courant->machine.max_figurines_avant_maintenance);
        printf("Cout maintenance: %.2f pieces\n", courant->machine.cout_maintenance);

        printf("Etat: ");
        switch(courant->machine.etat) {
            case LIBRE:
                printf("LIBRE\n");
                break;
            case OCCUPEE:
                printf("OCCUPEE");
                if (courant->machine.commande_en_cours != NULL) {
                    printf(" (Commande #%d)", courant->machine.commande_en_cours->identifiant);
                }
                printf("\n");
                afficher_temps_restant(&(courant->machine));
                break;
            case EN_ATTENTE_MAINTENANCE:
                printf("EN MAINTENANCE\n");
                afficher_temps_restant(&(courant->machine));
                break;
        }

        courant = courant->suivant;
        numero++;
    }

    printf("\n====================================================\n");
}

// ============================================================================
// FONCTIONS CRUD (inchangées)
// ============================================================================

void ajouter_Machine(liste_machine** liste_machines, liste_pokemon* liste_pokemons) {
    if (liste_pokemons == NULL) {
        printf("\nErreur: Aucun Pokemon disponible! Creez d'abord des Pokemons.\n");
        return;
    }

    liste_machine* nouveau = (liste_machine*)malloc(sizeof(liste_machine));
    if (nouveau == NULL) {
        printf("Erreur d'allocation memoire!\n");
        return;
    }

    Machine* m = &(nouveau->machine);
    m->identifiant = prochain_id_machine;

    printf("\n========== AJOUT D'UNE MACHINE ==========\n");
    while (getchar() != '\n');

    printf("Nom de la machine: ");
    scanf("%49[^\n]", m->nom);

    printf("\nPokemons disponibles:\n");
    afficher_Pokemons(liste_pokemons);

    int id_pokemon;
    printf("\nID du Pokemon a produire: ");
    scanf("%d", &id_pokemon);

    m->pokemon_produit = trouver_pokemon_par_id(liste_pokemons, id_pokemon);

    if (m->pokemon_produit == NULL) {
        printf("Erreur: Pokemon ID %d non trouve!\n", id_pokemon);
        free(nouveau);
        return;
    }

    printf("Temps de production (minutes): ");
    scanf("%d", &m->temps_production);

    printf("Nombre max de figurines avant maintenance: ");
    scanf("%d", &m->max_figurines_avant_maintenance);

    printf("Cout de maintenance (pieces): ");
    scanf("%f", &m->cout_maintenance);

    printf("Duree de maintenance (minutes): ");
    scanf("%d", &m->temps_maintenance);

    m->compteur_figurines = 0;
    m->total_produit = 0;
    m->etat = LIBRE;
    m->commande_en_cours = NULL;
    m->date_disponibilite = time(NULL);

    nouveau->suivant = *liste_machines;
    *liste_machines = nouveau;

    prochain_id_machine++;

    printf("\nMachine ajoutee avec succes! (ID: %d)\n", m->identifiant);
}

void modifier_Machine(liste_machine* liste_machines, liste_pokemon* liste_pokemons) {
    if (liste_machines == NULL) {
        printf("\nAucune machine a modifier!\n");
        return;
    }

    afficher_Machines(liste_machines);

    int id;
    printf("\nID de la machine a modifier: ");
    scanf("%d", &id);

    Machine* m = trouver_machine_par_id(liste_machines, id);

    if (m == NULL) {
        printf("\nMachine ID %d introuvable!\n", id);
        return;
    }

    printf("\n========== MODIFICATION MACHINE ID: %d ==========\n", id);
    while (getchar() != '\n');

    printf("Nouveau nom (actuel: %s): ", m->nom);
    scanf("%49[^\n]", m->nom);

    if (liste_pokemons != NULL) {
        printf("\nPokemons disponibles:\n");
        afficher_Pokemons(liste_pokemons);

        int id_pokemon;
        printf("\nID du nouveau Pokemon: ");
        scanf("%d", &id_pokemon);

        Pokemon* nouveau_pokemon = trouver_pokemon_par_id(liste_pokemons, id_pokemon);
        if (nouveau_pokemon != NULL) {
            m->pokemon_produit = nouveau_pokemon;
        }
    }

    printf("Nouveau temps de production (actuel: %d): ", m->temps_production);
    scanf("%d", &m->temps_production);

    printf("Nouveau seuil (actuel: %d): ", m->max_figurines_avant_maintenance);
    scanf("%d", &m->max_figurines_avant_maintenance);

    printf("Nouveau cout (actuel: %.2f): ", m->cout_maintenance);
    scanf("%f", &m->cout_maintenance);

    printf("Nouvelle duree maintenance (actuelle: %d): ", m->temps_maintenance);
    scanf("%d", &m->temps_maintenance);

    printf("\nMachine modifiee avec succes!\n");
}

void supprimer_Machine(liste_machine** liste_machines) {
    if (*liste_machines == NULL) {
        printf("\nAucune machine a supprimer!\n");
        return;
    }

    afficher_Machines(*liste_machines);

    int id;
    printf("\nID de la machine a supprimer: ");
    scanf("%d", &id);

    liste_machine* courant = *liste_machines;
    liste_machine* precedent = NULL;

    while (courant != NULL) {
        if (courant->machine.identifiant == id) {
            if (courant->machine.etat == OCCUPEE) {
                printf("Erreur: Impossible de supprimer, machine occupee!\n");
                return;
            }

            printf("\nMachine '%s' (ID: %d) sera supprimee.\n",
                   courant->machine.nom, id);

            if (precedent == NULL) {
                *liste_machines = courant->suivant;
            } else {
                precedent->suivant = courant->suivant;
            }

            // Liberer proprement les commandes associees a cette machine
            liberer_commandes_machine_supprimee(&(courant->machine));

            printf("\nMachine supprimee avec succes!\n");
            free(courant);
            return;
        }

        precedent = courant;
        courant = courant->suivant;
    }

    printf("\nMachine ID %d introuvable!\n", id);
}

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
        // Sécurité : Vérifier le pointeur ET comparer les identifiants pour plus de robustesse
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

int compter_machines_par_etat(liste_machine* liste_machines, EtatMachine etat) {
    int count = 0;
    liste_machine* courant = liste_machines;

    while (courant != NULL) {
        if (courant->machine.etat == etat) {
            count++;
        }
        courant = courant->suivant;
    }

    return count;
}

// ============================================================================
// NOUVELLES FONCTIONS DE GESTION DE PRODUCTION AVEC TEMPS
// ============================================================================

void lancer_production(Machine* machine, Commande* commande, int quantite_a_produire) {
    if (machine == NULL || commande == NULL) {
        printf("Erreur: Machine ou commande invalide!\n");
        return;
    }

    if (machine->etat != LIBRE) {
        printf("Erreur: La machine n'est pas libre!\n");
        return;
    }

    if (machine->pokemon_produit == NULL || commande->pokemon_commande == NULL || 
        machine->pokemon_produit->identifiant != commande->pokemon_commande->identifiant) {
        printf("Erreur: Cette machine ne produit pas le bon Pokemon!\n");
        return;
    }

    // Simulation pour calculer le temps total incluant les maintenances
    int final_compteur;
    int minutes_totales = simuler_temps_et_compteur(machine, quantite_a_produire, machine->compteur_figurines, &final_compteur);
    
    machine->etat = OCCUPEE;
    machine->commande_en_cours = commande;
    commande->etat = EN_COURS; // Marquer la commande comme en cours

    // Calculer le temps de fin de production (en secondes)
    machine->date_disponibilite = time(NULL) + (minutes_totales * 60);

    printf("\n✓ Production demarree sur la machine '%s'\n", machine->nom);
    printf("  Commande: #%d\n", commande->identifiant);
    printf("  Quantite: %d figurines\n", quantite_a_produire);
    
    // Alerte si une maintenance est prévue dans le lot
    if (machine->compteur_figurines + quantite_a_produire > machine->max_figurines_avant_maintenance) {
        printf("  ⚠ Note: Une maintenance auto est incluse dans ce delai.\n");
    }

    printf("  Duree totale estimee: %d minutes\n", minutes_totales);

    // Afficher l'heure de fin estimée
    char temps_fin[80];
    struct tm* timeinfo = localtime(&(machine->date_disponibilite));
    strftime(temps_fin, sizeof(temps_fin), "%H:%M:%S", timeinfo);
    printf("  Fin estimee: %s\n", temps_fin);
}

void terminer_production(Machine* machine, Caisse* caisse) {
    if (machine == NULL) {
        printf("Erreur: Machine invalide!\n");
        return;
    }

    if (machine->etat != OCCUPEE) {
        printf("Erreur: Aucune production en cours!\n");
        return;
    }

    if (machine->commande_en_cours == NULL) {
        printf("Erreur: Aucune commande associee!\n");
        machine->etat = LIBRE;
        machine->date_disponibilite = time(NULL);
        return;
    }

    if (!production_terminee(machine)) {
        printf("Erreur: Production pas encore terminee!\n");
        afficher_temps_restant(machine);
        return;
    }

    Commande* commande = machine->commande_en_cours;

    // Simulation pour connaitre l'etat FINAL reel
    int final_compteur;
    simuler_temps_et_compteur(machine, commande->quantite, machine->compteur_figurines, &final_compteur);

    // Revenus calcules sur toute la quantite
    float prix_unitaire = commande->pokemon_commande->cout_unitaire;
    float revenu = commande->quantite * prix_unitaire;

    if (caisse != NULL) {
        caisse->solde += revenu;
    }

    printf("\n✓ Production INDUSTRIELLE terminee sur '%s'\n", machine->nom);
    printf("  Commande: #%d | Pokemon: %s\n", commande->identifiant, commande->pokemon_commande->nom);
    printf("  Total produit: %d figurines (Revenu: %.2f)\n", commande->quantite, revenu);

    // Mise a jour du compteur et de la commande
    machine->compteur_figurines = final_compteur;
    machine->total_produit += commande->quantite;
    // On garde la quantité pour l'historique/stats
    commande->etat = REALISEE;

    // Liberer la reference
    machine->commande_en_cours = NULL;

    // Etat de la machine apres ce bloc de production
    // Si le compteur final est très haut, ou si on a fini sur un cycle de maintenance
    if (machine->compteur_figurines >= machine->max_figurines_avant_maintenance) {
        machine->etat = EN_ATTENTE_MAINTENANCE;
        machine->date_disponibilite = time(NULL) + (machine->temps_maintenance * 60);
        printf("  [!] Machine en maintenance apres livraison.\n");
    } else {
        machine->etat = LIBRE;
        machine->date_disponibilite = time(NULL);
        printf("  Machine prete pour la file d'attente.\n");
    }
}
void effectuer_maintenance(Machine* machine, Caisse* caisse) {
    if (machine == NULL) {
        printf("Erreur: Machine invalide!\n");
        return;
    }

    if (machine->etat != EN_ATTENTE_MAINTENANCE) {
        printf("Erreur: Machine n'est pas en maintenance!\n");
        return;
    }

    if (!maintenance_terminee(machine)) {
        printf("Erreur: Maintenance pas encore terminee!\n");
        afficher_temps_restant(machine);
        return;
    }

    // Vérifier le solde de la caisse
    if (caisse != NULL && caisse->solde < machine->cout_maintenance) {
        printf("Erreur: Solde insuffisant pour la maintenance!\n");
        printf("  Requis: %.2f pieces\n", machine->cout_maintenance);
        printf("  Disponible: %.2f pieces\n", caisse->solde);
        return;
    }

    // Déduire le coût
    if (caisse != NULL) {
        caisse->solde -= machine->cout_maintenance;
    }

    // Réinitialiser le compteur
    machine->compteur_figurines = 0;
    machine->etat = LIBRE;
    machine->date_disponibilite = time(NULL);

    printf("\n✓ Maintenance effectuee sur la machine '%s'\n", machine->nom);
    printf("  Cout: %.2f pieces\n", machine->cout_maintenance);
    if (caisse != NULL) {
        printf("  Nouveau solde: %.2f pieces\n", caisse->solde);
    }
    printf("  Compteur reinitialise: 0 / %d\n", machine->max_figurines_avant_maintenance);
    printf("  Machine disponible pour production\n");
}

// ============================================================================
// FONCTIONS DE VÉRIFICATION DU TEMPS
// ============================================================================

int production_terminee(Machine* machine) {
    if (machine == NULL || machine->etat != OCCUPEE) {
        return 0;
    }

    time_t maintenant = time(NULL);
    return (maintenant >= machine->date_disponibilite);
}

int maintenance_terminee(Machine* machine) {
    if (machine == NULL || machine->etat != EN_ATTENTE_MAINTENANCE) {
        return 0;
    }

    time_t maintenant = time(NULL);
    return (maintenant >= machine->date_disponibilite);
}

void afficher_temps_restant(Machine* machine) {
    if (machine == NULL) {
        return;
    }

    time_t maintenant = time(NULL);

    if (machine->etat == LIBRE) {
        printf("  Machine disponible immediatement\n");
        return;
    }

    if (maintenant >= machine->date_disponibilite) {
        if (machine->etat == OCCUPEE) {
            printf("  ✓ Production terminee! Appelez terminer_production()\n");
        } else if (machine->etat == EN_ATTENTE_MAINTENANCE) {
            printf("  ✓ Maintenance terminee! Appelez effectuer_maintenance()\n");
        }
        return;
    }

    // Calculer le temps restant
    int secondes_restantes = (int)difftime(machine->date_disponibilite, maintenant);
    int minutes = secondes_restantes / 60;
    int secondes = secondes_restantes % 60;

    if (machine->etat == OCCUPEE) {
        printf("  Temps restant de production: %d min %d sec\n", minutes, secondes);
    } else if (machine->etat == EN_ATTENTE_MAINTENANCE) {
        printf("  Temps restant de maintenance: %d min %d sec\n", minutes, secondes);
    }

    // Afficher l'heure de fin
    char temps_fin[80];
    struct tm* timeinfo = localtime(&(machine->date_disponibilite));
    strftime(temps_fin, sizeof(temps_fin), "%H:%M:%S", timeinfo);
    printf("  Disponible a: %s\n", temps_fin);
}

int calculer_capacite_restante(Machine* machine) {
    if (machine == NULL) {
        return 0;
    }

    int capacite = machine->max_figurines_avant_maintenance - machine->compteur_figurines;
    return (capacite > 0) ? capacite : 0;
}

// ============================================================================
// LIBÉRATION MÉMOIRE
// ============================================================================

// ============================================================================
// NOUVELLES FONCTIONS DE GESTION DE FILE ET SIMULATION
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

        if (compteur >= machine->max_figurines_avant_maintenance) {
            if (qte_restante > 0) {
                temps_total += machine->temps_maintenance;
                compteur = 0;
            }
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
    nouveau->commande.etat = PLANIFIEE; // Securite
    commande->etat = PLANIFIEE;
    nouveau->suivant = NULL;

    if (machine->file_attente == NULL) {
        machine->file_attente = nouveau;
    } else {
        liste_commande* courant = machine->file_attente;
        while (courant->suivant != NULL) {
            courant = courant->suivant;
        }
        courant->suivant = nouveau;
    }
}

Commande* retirer_de_file(Machine* machine) {
    if (machine == NULL || machine->file_attente == NULL) return NULL;

    liste_commande* a_retirer = machine->file_attente;
    // On doit faire attention ici car la commande est une copie dans la liste
    // Mais on veut retourner un pointeur vers la commande originale ou une allocation
    // Pour simplifier selon le prompt, on va retourner un pointeur vers une allocation
    Commande* cmd = (Commande*)malloc(sizeof(Commande));
    if (cmd != NULL) *cmd = a_retirer->commande;

    machine->file_attente = a_retirer->suivant;
    free(a_retirer);
    return cmd;
}

void afficher_temps_attente_file(Machine* machine) {
    if (machine == NULL) return;

    printf("\n--- File d'attente de la machine: %s ---\n", machine->nom);

    int temps_cumule = 0;
    int compteur_virtuel = machine->compteur_figurines;

    // 1. Commande en cours
    if (machine->etat == OCCUPEE && machine->commande_en_cours != NULL) {
        time_t maintenant = time(NULL);
        int reste_sec = (int)difftime(machine->date_disponibilite, maintenant);
        int reste_min = (reste_sec > 0) ? (reste_sec / 60) + 1 : 0;
        
        printf("[En cours]  Commande #%d | Client: %s | Qte: %d -> fin dans %d min\n",
               machine->commande_en_cours->identifiant,
               machine->commande_en_cours->client->nom,
               machine->commande_en_cours->quantite,
               reste_min);
        
        temps_cumule = reste_min;
        // Le compteur virtuel sera celui après cette commande
        // Note: lancer_production a déjà mis à jour le compteur dans la simulation réelle ?
        // Non, terminer_production le fait. Donc ici on simule.
        int final;
        simuler_temps_et_compteur(machine, machine->commande_en_cours->quantite, machine->compteur_figurines, &final);
        compteur_virtuel = final;
    } else if (machine->etat == EN_ATTENTE_MAINTENANCE) {
        time_t maintenant = time(NULL);
        int reste_sec = (int)difftime(machine->date_disponibilite, maintenant);
        int reste_min = (reste_sec > 0) ? (reste_sec / 60) + 1 : 0;
        printf("[Maintenance] Fin dans %d min\n", reste_min);
        temps_cumule = reste_min;
        compteur_virtuel = 0;
    }

    // 2. Commandes en attente
    liste_commande* courant = machine->file_attente;
    int index = 1;
    while (courant != NULL) {
        int final;
        int duree = simuler_temps_et_compteur(machine, courant->commande.quantite, compteur_virtuel, &final);
        temps_cumule += duree;
        compteur_virtuel = final;

        printf("[Attente %d] Commande #%d | Client: %s | Qte: %d -> fin dans %d min\n",
               index++,
               courant->commande.identifiant,
               courant->commande.client->nom,
               courant->commande.quantite,
               temps_cumule);
        
        courant = courant->suivant;
    }

    if (index == 1 && machine->etat == LIBRE) {
        printf("(File vide)\n");
    }
}

void liberer_liste_machines(liste_machine** liste_machines) {
    if (liste_machines == NULL || *liste_machines == NULL) {
        return;
    }

    liste_machine* courant = *liste_machines;
    liste_machine* suivant;

    while (courant != NULL) {
        suivant = courant->suivant;
        
        // Libérer la file d'attente de la machine
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
// FONCTION DE MISE À JOUR AUTOMATIQUE DES MACHINES
// ============================================================================

void mettre_a_jour_machines(liste_machine* liste_machines, Caisse* caisse, liste_commande* liste_commandes) {
    if (liste_machines == NULL) {
        return;
    }

    liste_machine* courant = liste_machines;
    int notifications = 0;

    while (courant != NULL) {
        Machine* m = &(courant->machine);

        // 1. Declenchement automatique de la file si LIBRE
        if (m->etat == LIBRE && m->file_attente != NULL) {
            if (notifications == 0) printf("\n========== AUTOMATISATION ==========\n");
            
            Commande* copie = retirer_de_file(m);
            if (copie != NULL) {
                // Chercher l'originale pour mettre a jour l'etat Global
                Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                if (vraie_commande != NULL) {
                    printf("[INFO] Machine '%s' : Reprise de la file (Commande #%d)\n", m->nom, vraie_commande->identifiant);
                    lancer_production(m, vraie_commande, vraie_commande->quantite);
                    notifications++;
                }
                free(copie);
            }
        }

        // 2. Vérifier si production terminée
        if (m->etat == OCCUPEE && production_terminee(m)) {
            if (notifications == 0) printf("\n========== AUTOMATISATION ==========\n");
            printf("[OK] Machine '%s' : Production terminee! Finalisation auto...\n", m->nom);
            
            terminer_production(m, caisse);
            notifications++;

            if (m->etat == LIBRE && m->file_attente != NULL) {
                Commande* copie = retirer_de_file(m);
                if (copie != NULL) {
                    Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                    if (vraie_commande != NULL) {
                        printf(">> Lancement automatique de la prochaine commande (#%d) dans la file.\n", vraie_commande->identifiant);
                        lancer_production(m, vraie_commande, vraie_commande->quantite);
                    }
                    free(copie);
                }
            }
        }

        // 3. Vérifier si maintenance terminée
        if (m->etat == EN_ATTENTE_MAINTENANCE && maintenance_terminee(m)) {
            if (notifications == 0) printf("\n========== AUTOMATISATION ==========\n");
            printf("[OK] Machine '%s' : Maintenance terminee! Remise en service auto...\n", m->nom);
            
            effectuer_maintenance(m, caisse);
            notifications++;

            if (m->etat == LIBRE && m->file_attente != NULL) {
                Commande* copie = retirer_de_file(m);
                if (copie != NULL) {
                    Commande* vraie_commande = trouver_commande_par_id(liste_commandes, copie->identifiant);
                    if (vraie_commande != NULL) {
                        printf(">> Lancement automatique de la commande (#%d) apres maintenance.\n", vraie_commande->identifiant);
                        lancer_production(m, vraie_commande, vraie_commande->quantite);
                    }
                    free(copie);
                }
            }
        }

        courant = courant->suivant;
    }

    if (notifications > 0) {
        printf("====================================\n\n");
    }
}

// Nettoyer les liens quand une commande est supprimee
void supprimer_commande_de_toutes_files(liste_machine* liste, int id_commande) {
    liste_machine* curr_m = liste;
    while (curr_m) {
        Machine* m = &curr_m->machine;
        
        // 1. Si la commande était en cours, on libère la machine sans valider la production
        if (m->commande_en_cours && m->commande_en_cours->identifiant == id_commande) {
            m->commande_en_cours = NULL;
            m->etat = LIBRE;
            m->date_disponibilite = time(NULL);
        }
        
        // 2. Retirer de la file d'attente
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

// Quand une machine est supprimee, on libere ses commandes
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
// Supprimer toutes les machines qui produisent un Pokemon specifique
void supprimer_machines_par_pokemon(liste_machine** liste_machines, int id_pokemon) {
    if (liste_machines == NULL || *liste_machines == NULL) return;

    liste_machine* courant = *liste_machines;
    liste_machine* precedent = NULL;

    while (courant != NULL) {
        if (courant->machine.pokemon_produit != NULL && 
            courant->machine.pokemon_produit->identifiant == id_pokemon) {
            
            // On libère la machine
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
