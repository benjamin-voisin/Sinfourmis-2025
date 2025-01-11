#ifndef ANT_PILE
#define ANT_PILE

#include "../../sinfourmis.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct pile_s {
    uint8_t id;
    uint8_t poid;
    uint8_t degree_entrant;
    uint8_t degree_sortant;
    enum salle_type type;
} pile_t;


/*
 CETTE STRUCTURE PERMET DE CALCULER LA TAILLE PRISE PAR LES METADATAS
 Elles est morallement ABSTRAITE, ne pas l'utiliser, merci <3 
*/
typedef struct pilemetadata_s { 
    uint8_t taillepilemax;
    uint8_t taillepile;
} pilemetadata_t;

void pilemetadata_pp_body(FILE* f, char* memoire);
void pile_pp(FILE* f, char* memoire);

pile_t* head(char* memoire);

void simplipile(char* memoire);
// Savoir si on est bien revenus sur la reine
bool pile_vide(char* memoire);
bool pile_complete(char* memoire);

// Pour empiler un bout de chemin
void empiler(char* memoire, pile_t e);

void false_empiler(char* memoire);

// Pour dépiler un bout de chemin
pile_t* depiler(char* memoire);

void pile_edit_id(char* memoire, uint8_t id);

// Pour que la reine puisse retrouver le trajet d'une fourmisse
pile_t* pile_dumps(char* memoire, size_t* size);
void pile_loads(char* memoire, pile_t* pile, size_t size);


uint32_t water2base(char* memoire);
uint32_t water2dest(char* memoire);

#endif
