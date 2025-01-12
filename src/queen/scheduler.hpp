#pragma once

#include "../sinfourmis.h"
#include "reine_input.hpp"

#include <queue>

typedef enum taks_e {
	CREER_SCOUT,
	CREER_MANGER,
	CREER_GUARDE,
	GASLIGHT_MANGER,
	GASLIGHT_SCOUT,
	SEND_FORUMIS,
	EAT_FORUMIS,
	PASS,
} task_t;

typedef union arg_u {
	size_t amount;
} arg_t;

class Task {
	private:
		task_t _task_type;
		arg_t _arg;
	public:
		void execute(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present);
		Task(task_t type, arg_t arg);
};

class Scheduler {
	private:
		std::queue<Task> _tasks;
	public:
		void execute_tasks(reine_action *action, int *arg, reine_input_t *input, size_t friendly_ants_present);
		void add_task(Task task);

};
