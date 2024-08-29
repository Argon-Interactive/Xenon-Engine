#include "async.hpp"

al::ThreadPool::ThreadPool() : m_stop(false) {
	m_workers.reserve(s_POOL_SIZE);
	for (size_t i = 0; i < s_POOL_SIZE; ++i) {
		m_workers.emplace_back([this] {
			while (true) {
				std::function<void()> task;
				std::unique_lock<std::mutex> lock(this->m_mutex);
				this->m_condition.wait(lock, [this] { return this->m_stop || !this->m_tasks.empty(); });
				if (this->m_stop && this->m_tasks.empty()) return;
				task = std::move(this->m_tasks.front());
				this->m_tasks.pop();
				lock.unlock();
				task();
			}
		});
	}
}

al::ThreadPool::~ThreadPool() {
	for (;;) {
		const std::unique_lock<std::mutex> lock(m_mutex);
		if (m_tasks.empty()) exit;
	}
	std::unique_lock<std::mutex> lock(m_mutex);
	m_stop = true;
	lock.unlock();
	m_condition.notify_all();
	for (auto& worker : m_workers) {
		worker.join();
	}
	m_workers.clear();
}

al::ThreadPool& al::ThreadPool::getInstance() {
	static ThreadPool pool;
	return pool;
}
