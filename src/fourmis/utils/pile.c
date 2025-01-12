#include "pile.h"

#include "../../sinfourmis.h"
#include "b_constants.h"


pile_t* pile(char* memoire) {
    return (pile_t*) (memoire + 256);
}

pilemetadata_t* pilemetadata(char* memoire) {
    return (pilemetadata_t*) memoire;
}

uint8_t pile_size(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    return met->taillepile;
}

pile_t* pile_get(char* memoire, size_t i) {
    pilemetadata_t* met = pilemetadata(memoire);
    Assert(CAT_PILE, met->taillepilemax > 0, "Get sur pile de taille 0\n");
    Assert(CAT_PILE, i < met->taillepilemax, "Indice de get trop grand !\n");
    pile_t* p = pile(memoire);
    return p - i - 1;
}

void pilemetadata_pp_body(logcat_t cat, loglevel_t level, char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    Log(cat, level, "PILEMETADATA:\n");
    Log(cat, level, "    taille     = %u\n", met->taillepile);
    Log(cat, level, "    taillemax  = %u\n", met->taillepilemax);
}

void pile_pp_saletype(logcat_t cat, loglevel_t level, enum salle_type t) {
    switch (t)
    {
    case VIDE:
        Log(cat, level, "VIDE");
        break;
    case EAU:
        Log(cat, level, "EAU");
        break;
    case NOURRITURE:
        Log(cat, level, "NOURRITURE");
        break;
    case REINE:
        Log(cat, level, "REINE");
        break;
    default:
        Log(cat, level, "UNKNOWN");
        break;
    }

}

void pile_pp_part(logcat_t cat, loglevel_t level, pile_t* p) {
    Log(cat, level, "NOEUDPILE [\n");
    Log(cat, level, "    id         = %u\n", p->id);
    Log(cat, level, "    d_entrant  = %u\n", p->degree_entrant);
    Log(cat, level, "    d_sortant  = %u\n", p->degree_sortant);
    Log(cat, level, "    poid       = %u\n", p->poid);
    Log(cat, level, "    type       = ");
    pile_pp_saletype(CAT_NOBLOAT, level, p->type);
    Log(CAT_NOBLOAT, level, "\n");
    Log(cat, level, "]\n");
}

void pile_pp(logcat_t cat, loglevel_t level, char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    pilemetadata_pp_body(cat, level, memoire);
    Log(cat, level, "PILE:\n");
    log_set_color(COLOR_GREEN);
    for (size_t i=0; i<met->taillepile; ++i) {
        pile_t* p = pile_get(memoire, i);
        pile_pp_part(cat, level, p);
    }
    log_set_color(COLOR_YELLOW);
    for (size_t i=met->taillepile; i<met->taillepilemax; ++i) {
        pile_t* p = pile_get(memoire, i);
        pile_pp_part(cat, level, p);
    }
    log_reset_color();
}

void pile_copy(pile_t* psrc, pile_t* pdst) {
    pdst->degree_entrant = psrc->degree_entrant;
    pdst->degree_sortant = psrc->degree_sortant;
    pdst->id = psrc->id;
    pdst->poid = psrc->poid;
    pdst->type = psrc->type;
}

void simplipile(char* memoire) {
    Log_info(CAT_PILE, "Simplipile !\n");
    pilemetadata_t* met = pilemetadata(memoire);
    if (met->taillepile > 0) {
    pile_t* p_base = pile_get(memoire, met->taillepile-1);
    for (size_t i=0; i<met->taillepile-1; ++i) {
        pile_t* p = pile_get(memoire, i);
        if (p_base->id == p->id) {
            pile_pp_part(CAT_PILE, LOG_DEBUG, p);
            uint8_t delta = met->taillepilemax-met->taillepile;
            for (size_t j=0; j<delta; ++j) {
                pile_t* psrc = pile_get(memoire, met->taillepile+j);
                pile_t* pdst = pile_get(memoire, i+j+1);
                pile_copy(psrc, pdst);
            }
            met->taillepile = i + 1;
            met->taillepilemax = i + delta + 1;
            return;
        }
    }
    }
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
    Assert(CAT_PILE, !pile_vide(memoire), "Head sur pile vide\n");
    return pile_get(memoire, met->taillepile - 1);
}

void false_empiler(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    Assert(CAT_PILE, !(pile_complete(memoire)), "Déjà au bout de la pile\n");
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
    pile_copy(&e, hd);
}

pile_t* depiler(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    Assert(CAT_PILE, !pile_vide(memoire), "Dépiler sur pile vide\n");
    pile_t* hd = head(memoire);
    met->taillepile -= 1;
    return hd;
}

void pile_edit_id(char* memoire, uint8_t id) {
    Assert(CAT_PILE, !pile_vide(memoire), "Edit sur pile vide\n");
    pile_t* hd = head(memoire);
    hd->id = id;
}

pile_t* pile_dumps_last(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    Assert(CAT_PILE, met->taillepilemax > 0, "Recherche dernier noeud sur pile de taille 0\n");
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

void pile_reduceloads(char* memoire) {
    pilemetadata_t* met = pilemetadata(memoire);
    Assert(CAT_PILE, met->taillepile == 0, "Réduire pile vide\n");
    met->taillepilemax = 0;
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


