#include "scheduler.hpp"
#include "../fourmis/main.h"
#include "../fourmis/utils/b_constants.h"
#include "../fourmis/utils/log.h"
#include "../fourmis/scout.h"
#include "../fourmis/food.h"

#include <iostream>

void creer_scout(reine_action *action, int *arg, size_t n_to_create) {
	/* std::cout << "CRÉATION D’UNE FOURMI SCOUT\n"; */
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
	*arg = 1;
}
void creer_manger(reine_action *action, int *arg, size_t n_to_create) {
	/* std::cout << "CRÉATION D’UNE FOURMI FOOD\n"; */
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
	*arg = 1;
}

void creer_guarde(reine_action *action, int *arg, size_t n_to_create) {
	/* std::cout << "CRÉATION D’UNE FOURMI GUARDE\n"; */
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
	*arg = 1;
}

void gaslight_scout(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input, stats_t stat) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "scout", on les gaslight en scout
		if (fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
			/* std::cout << "GASLIGHT D’UNE FOURMI SCOUT\n"; */
			memoire_scout_t* mem = (memoire_scout_t*) fourmis->memoire;
			if (mem->tile_counter < COUNTER_MASK) {
				scout_reloads(fourmis, stat);
			} else {
				scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
			}
		}
		// Si elles sont de type "new", on les gaslight en scout
		else  { //if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			/* std::cout << "GASLIGHT D’UNE FOURMI SCOUT\n"; */
			scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
		}
	}
	*action = ENVOYER_FOURMI;
	*arg = 1;
}

void gaslight_manger(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input, std::vector<pile_t> path, stats_t stat) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "new", on les gaslight en scout
		if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			Log_warning(CAT_OTHER, "GASLIGHT D’UNE FOURMI FOOD\n");
			food_loads(fourmis, stat, input->state->team_id, &(path)[0], path.size());
		}
		// Si elles sont de type "scout", on les gaslight en scout
		else if (fourmi_kind(fourmis) == ANT_KIND_FOOD) {
			Log_warning(CAT_OTHER, "GASLIGHT D’UNE FOURMI FOOD\n");
			food_loads(fourmis, stat, input->state->team_id, &(path)[0], path.size());
		}
		else if (fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
			Log_warning(CAT_OTHER, "GASLIGHT D’UNE FOURMI SCOUT\n");
			memoire_scout_t* mem = (memoire_scout_t*) fourmis->memoire;
			if (mem->tile_counter < COUNTER_MASK) {
				scout_reloads(fourmis, stat);
			} else {
				scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
			}
		} else { 
			Log_warning(CAT_OTHER, "GERR TYPE FOURMIS INCONNU\n");
			common_kind_pp(CAT_OTHER, LOG_FATAL, fourmi_kind(fourmis));
		}
		fourmi_pp(CAT_OTHER, LOG_INFO, fourmis);
	}
	*action = ENVOYER_FOURMI;
	*arg = 1;
}


void send_manger(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input, std::vector<pile_t> path, stats_t stat) {
	gaslight_manger(action, arg, n_to_create, input, path, stat);
	*action = ENVOYER_FOURMI;
	*arg = 10;
}

void send_forumis(reine_action *action, int *arg, size_t n_send, reine_input_t *input, stats_t stat) {
	/* std::cout << "ENVOI DES FOURMIS\n"; */
	// On gaslight totute les fourmis en elle même mais reset
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		size_t pile_size;
		pile_t *pile;
		memoire_scout_t* mem;
		switch (fourmi_kind(fourmis)) {
			case (ANT_KIND_SCOUT):
				mem = (memoire_scout_t*) fourmis->memoire;
				if (mem->tile_counter < COUNTER_MASK) {
					scout_reloads(fourmis, stat);
				} else {
					scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
				}
				break;
			case (ANT_KIND_FOOD) :
				pile = pile_dumps(fourmis->memoire, &pile_size);
				food_loads(fourmis, stat, input->state->team_id, pile, pile_size);
				break;
			default :
				scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
				break;
		}
	}
	*action = ENVOYER_FOURMI;
	*arg = (int) n_send;
}

void manger_forumis(reine_action *action, int *arg, size_t n_send) {
	/* std::cout << "MANGAGE DES FOURMIS\n"; */
	*action = RECUPERER_FOURMI;
	*arg = (int) n_send;
}

Task::Task(task_t type, arg_t arg) : _task_type{type}, _arg{arg} {}

void default_cmp(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present, stats_t stat) {
	if (input->forumis_miam_miam.size() > 0) {
		/* std::cerr << "ENVOYER FORUMI\n"; */
		*action = ENVOYER_FOURMI;
		*arg = input->forumis_miam_miam.size();
		// On re-gaslight totu le monde en scout
		for (fourmi_etat *fourmis : input->forumis_miam_miam) {
			if (fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
				memoire_scout_t* mem = (memoire_scout_t*) fourmis->memoire;
				if (mem->tile_counter < COUNTER_MASK) {
					scout_reloads(fourmis, stat);
				} else {
					scout_loads(fourmis, stat, input->state->team_id, NULL, 0, 1);
				}
			}
			else if (fourmi_kind(fourmis) == ANT_KIND_FOOD) {
				size_t pile_size;
				pile_t *pile = pile_dumps(fourmis->memoire,&pile_size);
				food_loads(fourmis, stat, input->state->team_id, pile, pile_size);
			}
		}
		//
	} else if (friendly_ants_present > 0) {
		/* std::cerr << "RÉCUPER FOURMI\n"; */
		*action = RECUPERER_FOURMI;
		*arg = friendly_ants_present;
	} else {
		*action = REINE_PASSE;
		*arg = 0;
	}
}

void Task::execute(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present,
		std::optional<std::vector<pile_t>> *path_to_node, stats_t stat) {
	/* std::cerr << "EXECUTE" << _task_type << "\n"; */
	switch (_task_type) {
		case (CREER_SCOUT):
			creer_scout(action, arg, _arg.amount);
			break;
		case (CREER_MANGER):
			creer_manger(action, arg, _arg.amount);
			break;
		case (CREER_GUARDE):
			creer_guarde(action, arg, _arg.amount);
			break;
		case (GASLIGHT_SCOUT):
			gaslight_scout(action, arg, _arg.amount, input, stat);
			break;
		case (GASLIGHT_MANGER):
			gaslight_manger(action, arg, _arg.amount, input, path_to_node[_arg.manger_target].value(), stat);
			break;
		case (SEND_FORUMIS):
			send_forumis(action, arg, _arg.amount, input, stat);
			break;
		case (EAT_FORUMIS):
			manger_forumis(action, arg, _arg.amount);
			break;
		case (SEND_MANGER):
			send_manger(action, arg, _arg.amount, input, path_to_node[_arg.manger_target].value(), stat);
			break;
		case (UPDGRADE_WATER):
			/* std::cout << "UPGRADE WATER\n"; */
			*action = AMELIORE_EAU;
			*arg = 1;
			break;
		case (PASS):
			/* std::cout << "FAIS RIEN\n"; */
			*action = REINE_PASSE;
			*arg = 0;
			break;
		default:
		default_cmp(action, arg, input, friendly_ants_present, stat);
			break;
	}
}

void Scheduler::execute_tasks(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present, std::optional<std::vector<pile_t>> *path_to_node, stats_t stat) {
	if (_tasks.empty()) {
		// Si on a rien dans le scheduler, on mange une fourmis
		default_cmp(action, arg, input, friendly_ants_present, stat);
	} else {
		// Sinon, on execute la tache
		_tasks.front().execute(action, arg, input, friendly_ants_present, path_to_node, stat);
		_tasks.pop();
	}
}

void Scheduler::add_task(Task task) {
	_tasks.push(task);
}
