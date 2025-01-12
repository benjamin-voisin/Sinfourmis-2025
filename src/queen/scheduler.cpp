#include "scheduler.hpp"
#include "../fourmis/main.h"
#include "../fourmis/utils/b_constants.h"
#include "../fourmis/scout.h"
#include "../fourmis/food.h"

void creer_scout(reine_action *action, int *arg, size_t n_to_create) {
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}
void creer_manger(reine_action *action, int *arg, size_t n_to_create) {
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}

void creer_guarde(reine_action *action, int *arg, size_t n_to_create) {
	*action = CREER_FOURMI;
	*arg = (int) n_to_create;
}

void gaslight_scout(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "new", on les gaslight en scout
		if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			scout_loads(fourmis, input->state->team_id, NULL, 0, 0);
		}
		// Si elles sont de type "scout", on les gaslight en scout
		if (fourmi_kind(fourmis) == ANT_KIND_SCOUT) {
			scout_loads(fourmis, input->state->team_id, NULL, 0, 0);
		}
	}
}

void gaslight_manger(reine_action *action, int *arg, size_t n_to_create, reine_input_t *input) {
	// On parcours la liste des fourmis dans notre bidou
	for (fourmi_etat *fourmis : input->forumis_miam_miam) {
		// Si elles sont de type "new", on les gaslight en scout
		if ( fourmi_kind(fourmis) == ANT_KIND_NEW) {
			food_loads(fourmis, input->state->team_id, NULL, 0);
		}
		// Si elles sont de type "scout", on les gaslight en scout
		if (fourmi_kind(fourmis) == ANT_KIND_FOOD) {
			food_loads(fourmis, input->state->team_id, NULL, 0);
		}
	}
}

Task::Task(task_t type, size_t arg) : _task_type{type}, _arg{arg} {}

void Task::execute(reine_action *action, int *arg, reine_input_t *input) {
	switch (_task_type) {
		case (CREER_SCOUT):
			creer_scout(action, arg, _arg);
			break;
		case (CREER_MANGER):
			creer_manger(action, arg, _arg);
			break;
		case (CREER_GUARDE):
			creer_guarde(action, arg, _arg);
			break;
		case (GASLIGHT_SCOUT):
			gaslight_scout(action, arg, _arg, input);
			break;
		case (GASLIGHT_MANGER):
			gaslight_manger(action, arg, _arg, input);
			break;
		default:
			break;
	}
}
