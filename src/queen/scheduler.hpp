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
} task_t;

class Task {
	private:
		task_t _task_type;
		size_t _arg;
	public:
		void execute(reine_action *action, int *arg, reine_input_t *input);
		Task(task_t type, size_t arg);
};

class Scheduler {
	private:
		std::queue<Task> tasks;
	public:
		Task next_task();
		void add_task(Task task);

};
