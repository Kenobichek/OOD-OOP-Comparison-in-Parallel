#include "thread_pool.h"
#include <iostream>

ThreadPool::ThreadPool(const int thread_count) {
	write_lock _(rw_lock);

	if (initialized || terminated) {
		return;
	}

	threads.reserve(thread_count);

	for (int i = 0; i < thread_count; i++) {
		threads.emplace_back(&ThreadPool::Routine, this);
	}

	initialized = !threads.empty();
}

ThreadPool::~ThreadPool() {
	Terminate();
}

void ThreadPool::Routine()
{
	while (true)
	{
		std::function<void ()>  task;

		if (waiting && tasks_running == 0 && tasks_queue.empty()) {
			tasks_done_waiter.notify_all();
		}

		{
			write_lock _(rw_lock);
			task_waiter.wait(rw_lock, [this] {
				return terminated || !tasks_queue.empty(); 
			});

			if (terminated && tasks_queue.empty()) {
				return;
			}

			task = std::move(tasks_queue.front());
			tasks_queue.pop();
			++tasks_running;
		}
		task();

		{
			write_lock _(rw_lock);
			--tasks_running;
		}
	}
}

void ThreadPool::AddTask(std::function<void()> fun) {
	write_lock _(rw_lock);
	if (!WorkingUnsafe()) {
		return;
	}

	if (tasks_queue.emplace(fun)) {
		task_waiter.notify_one();
	}
}

void ThreadPool::Terminate(const bool should_finish_tasks) {
	{
		write_lock _(rw_lock);

		if (!should_finish_tasks) {
			threads.clear();
		}
		if (WorkingUnsafe()) {
			terminated = true;
		}
		else {
			threads.clear();
			terminated = false;
			initialized = false;
			return;
		}
	}

	task_waiter.notify_all();

	for (std::thread& thread : threads) {
		thread.join();
	}

	threads.clear();
	terminated = false;
	initialized = false;
}

bool ThreadPool::WorkingUnsafe() const
{
	return initialized && !terminated;
}

int ThreadPool::GetThreadCount() const
{
	return threads.size();
}

void ThreadPool::WaitForAllThreads() {
	write_lock _(rw_lock);
	waiting = true;

	tasks_done_waiter.wait(rw_lock, [this] {
		return tasks_running == 0 && tasks_queue.empty();
	});

	waiting = false;
}
