#pragma once

#include "../sinfourmis.h"
#include "graph.hpp"
#include "reine_input.hpp"
#include "../fourmis/utils/pile.h"
#include "../fourmis/utils/stats.h"

#include <queue>

typedef enum taks_e {
	CREER_SCOUT,
	CREER_MANGER,
	CREER_GUARDE,
	GASLIGHT_MANGER,
	GASLIGHT_SCOUT,
	SEND_FORUMIS,
	SEND_MANGER,
	EAT_FORUMIS,
	UPDGRADE_WATER,
	PASS,
} task_t;

typedef struct arg_u {
	size_t amount;
	node_id manger_target;
} arg_t;

class Task {
	private:
		task_t _task_type;
		arg_t _arg;
	public:
		void execute(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present, std::optional<std::vector<pile_t>> *path_to_node, stats_t stat);
		Task(task_t type, arg_t arg);
		
};

class Scheduler {
	private:
		std::queue<Task> _tasks;
	public:
		void execute_tasks(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present, std::optional<std::vector<pile_t>> *path_to_node, stats_t stat);
		void add_task(Task task);

};
