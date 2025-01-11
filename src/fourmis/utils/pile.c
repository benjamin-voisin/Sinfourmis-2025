#include "pile.h"

#include "b_constants.h"

#include <assert.h>

pile_t* pile(char* memoire) {
    return (pile_t*) (memoire + 256);
}

pilemetadata_t* pilemetadata(char* memoire) {
    return (pilemetadata_t*) memoire;
}

pile_t* pile_get(char* memoire, size_t i) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(met->taillepilemax > 0);
    assert(i < met->taillepilemax);
    pile_t* p = pile(memoire);
    return p - i - 1;
}

void pilemetadata_pp_body(FILE* f, char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    fprintf(f, "PILEMETADATA:\n");
    fprintf(f, "    taille     = %u\n", met->taillepile);
    fprintf(f, "    taillemax  = %u\n", met->taillepilemax);
}

void pile_pp_saletype(FILE* f, enum salle_type t) {
    switch (t)
    {
    case VIDE:
        fprintf(f, "VIDE");
        break;
    case EAU:
        fprintf(f, "EAU");
        break;
    case NOURRITURE:
        fprintf(f, "NOURRITURE");
        break;
    case REINE:
        fprintf(f, "REINE");
        break;
    default:
        fprintf(f, "UNKNOWN");
        break;
    }

}

void pile_pp_part(FILE* f, pile_t* p) {
    fprintf(f, "NOEUDPILE [\n");
    fprintf(f, "    id         = %u\n", p->id);
    fprintf(f, "    d_entrant  = %u\n", p->degree_entrant);
    fprintf(f, "    d_sortant  = %u\n", p->degree_sortant);
    fprintf(f, "    poid       = %u\n", p->poid);
    fprintf(f, "    type       = ");
    pile_pp_saletype(f, p->type);
    fprintf(f, "\n");
    fprintf(f, "]\n");
}

void pile_pp(FILE* f, char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    pilemetadata_pp_body(f, memoire);
    fprintf(f, "PILE:\n");
    fprintf(f, "\033[31m");
    for (size_t i=0; i<met->taillepile; ++i) {
        pile_t* p = pile_get(memoire, i);
        pile_pp_part(f, p);
    }
    fprintf(f, "\033[32m");
    for (size_t i=met->taillepile+1; i<met->taillepilemax; ++i) {
        pile_t* p = pile_get(memoire, i);
        pile_pp_part(f, p);
    }
    fprintf(f, "\033[0m");
}

bool pile_vide(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    return met->taillepile == 0; 
}
bool pile_complete(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    return met->taillepile >= met->taillepilemax; 
}

pile_t* head(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(!pile_vide(memoire));
    return pile_get(memoire, met->taillepile - 1);
}

void false_empiler(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(!(pile_complete(memoire)));
    met->taillepile += 1;
    if (met->taillepilemax < met->taillepile) {
        met->taillepilemax = met->taillepile;
    }
}

void empiler(char* memoire, pile_t e) {
    pilemetadata_t* met = pilemetadata(memoire);
    met->taillepile += 1;
    if (met->taillepilemax < met->taillepile) {
        met->taillepilemax = met->taillepile;
    }
    pile_t* hd = head(memoire);
    hd->id = e.id;
    hd->degree_entrant = e.degree_entrant;
    hd->degree_sortant = e.degree_sortant;
    hd->poid = e.poid;
    hd->type = e.type;
}

pile_t* depiler(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(!pile_vide(memoire));
    pile_t* hd = head(memoire);
    met->taillepile -= 1;
    return hd;
}

void pile_edit_id(char* memoire, uint8_t id) {
    assert(!pile_vide(memoire));
    pile_t* hd = head(memoire);
    hd->id = id;
}

pile_t* pile_dumps_last(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    assert(met->taillepilemax > 0);
    return pile_get(memoire, met->taillepilemax - 1);
}
pile_t* pile_dumps(char* memoire, size_t* size) {
    pilemetadata_t* met = pilemetadata(memoire);
    *size = met->taillepilemax;
    if (met->taillepilemax == 0)
        return NULL;
    return pile_dumps_last(memoire);
} 
#include <string.h>
void pile_loads(char* memoire, pile_t* pile, size_t size) {
    pilemetadata_t* met = pilemetadata(memoire);
    met->taillepile = 0;
    met->taillepilemax = size;
    if (met->taillepilemax > 0) {
        pile_t* p = pile_dumps_last(memoire);
        memcpy(p, pile, size * sizeof(pile_t));
    }
}

uint32_t water2base(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    uint32_t water = 0;
    for (size_t i=0; i<met->taillepile; ++i) {
        pile_t* p = pile_get(memoire, i);
        water += p->poid;
        if (p->type == EAU)
            return water;
    }
    return water;
}

uint32_t water2dest(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    uint32_t water = 0;
    for (size_t i=met->taillepile+1; i<met->taillepilemax; ++i) {
        pile_t* p = pile_get(memoire, i);
        water += p->poid;
        if (p->type == EAU)
            return water;
    }
    return water;
}


