#pragma once
#include <vector>
#include <thread>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <queue>

using read_write_lock = std::shared_mutex;
using read_lock = std::shared_lock<read_write_lock>;
using write_lock = std::unique_lock<read_write_lock>;

class ThreadPool
{
	public:
		ThreadPool(const int thread_count = 4);
		~ThreadPool();
		void AddTask(std::function<void()>);
		void Terminate(const bool should_finish_tasks = true);
		void Routine();
		bool WorkingUnsafe() const;
		int GetThreadCount() const;
		void WaitForAllThreads();

	private:
		mutable read_write_lock rw_lock;
		mutable std::condition_variable_any task_waiter;
		mutable std::condition_variable_any tasks_done_waiter;

		std::vector<std::thread> threads;
		std::queue<std::function<void()>> tasks_queue;

		int tasks_running = 0;

		bool initialized = false;
		bool terminated = false;
		bool waiting = false;
};