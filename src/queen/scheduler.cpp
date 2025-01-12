#include "scheduler.hpp"
#include "../fourmis/main.h"
#include "../fourmis/utils/b_constants.h"
#include "../fourmis/scout.h"
#include "../fourmis/food.h"

#include <iostream>

void creer_scout(reine_action *action, int *arg, size_t n_to_create) {
	std::cout << "CRÉATION D’UNE FOURMI SCOUT\n";
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}
void creer_manger(reine_action *action, int *arg, size_t n_to_create) {
	std::cout << "CRÉATION D’UNE FOURMI FOOD\n";
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}

void creer_guarde(reine_action *action, int *arg, size_t n_to_create) {
	std::cout << "CRÉATION D’UNE FOURMI GUARDE\n";
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}

void gaslight_scout(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "scout", on les gaslight en scout
		if (fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
			std::cout << "GASLIGHT D’UNE FOURMI SCOUT\n";
			scout_loads(fourmis, input->state->team_id, NULL, 0, 1);
		}
		// Si elles sont de type "new", on les gaslight en scout
		if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			std::cout << "GASLIGHT D’UNE FOURMI SCOUT\n";
			scout_loads(fourmis, input->state->team_id, NULL, 0, 1);
		}
	}
}

void gaslight_manger(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "new", on les gaslight en scout
		if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			std::cout << "GASLIGHT D’UNE FOURMI FOOD\n";
			food_loads(fourmis, input->state->team_id, NULL, 0);
		}
		// Si elles sont de type "scout", on les gaslight en scout
		if (fourmi_kind(fourmis) == ANT_KIND_FOOD) {
			std::cout << "GASLIGHT D’UNE FOURMI FOOD\n";
			food_loads(fourmis, input->state->team_id, NULL, 0);
		}
	}
}

void send_forumis(reine_action *action, int *arg, size_t n_send) {
	std::cout << "ENVOI DES FOURMIS\n";
	*action = ENVOYER_FOURMI;
	*arg = (int) n_send;
}

void manger_forumis(reine_action *action, int *arg, size_t n_send) {
	std::cout << "MANGAGE DES FOURMIS\n";
	*action = RECUPERER_FOURMI;
	*arg = (int) n_send;
}

Task::Task(task_t type, arg_t arg) : _task_type{type}, _arg{arg} {}

void Task::execute(reine_action *action, int *arg, reine_input_t *input) {
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
			gaslight_scout(action, arg, _arg.amount, input);
			break;
		case (GASLIGHT_MANGER):
			gaslight_manger(action, arg, _arg.amount, input);
			break;
		case (SEND_FORUMIS):
			send_forumis(action, arg, _arg.amount);
			break;
		case (EAT_FORUMIS):
			manger_forumis(action, arg, _arg.amount);
			break;
		default:
			break;
	}
}

void Scheduler::execute_tasks(reine_action *action, int *arg, reine_input_t *input) {
	if (_tasks.empty()) {
		// Si on a rien dans le scheduler, on fait rien
		*action = REINE_PASSE;
		*arg = 0;
	} else {
		// Sinon, on execute la tache
		_tasks.front().execute(action, arg, input);
		_tasks.pop();
	}
}

void Scheduler::add_task(Task task) {
	_tasks.push(task);
}
