#ifndef _ARGON_DEVTOOLS_ASYNC_
#define _ARGON_DEVTOOLS_ASYNC_

#include <type_traits>
#include <future>
#include <vector>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <future>
#include <functional>

namespace al {

	enum struct launch {
		async, deferred, pooled
	};

	class ThreadPool {
		static constexpr size_t s_POOL_SIZE = 64;
	public:
		~ThreadPool();

		ThreadPool(ThreadPool&&) = delete;
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
	private:
		ThreadPool();

		static ThreadPool& getInstance();
		template<typename T>
		void pushTask(std::shared_ptr<std::packaged_task<T>> task) {
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				if (m_stop) throw std::runtime_error("Pushing a task to a stopped thread pool");
				m_tasks.emplace([task]() { (*task)(); });
			}
			m_condition.notify_one();
		}

		std::vector<std::thread> m_workers{};
		std::queue<std::function<void()>> m_tasks{};
		std::mutex m_mutex{};
		std::condition_variable m_condition{};
		bool m_stop;

		template<typename F, typename ...Args>
		friend auto async(const launch& launchPolicy, F&& func, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type>;
	};

	template<typename F, typename ...Args>
	auto async(const launch& launchPolicy, F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
		using return_type = typename std::invoke_result<F, Args...>::type;
			switch (launchPolicy) {
			case al::launch::async:
				return std::async(std::launch::async, std::forward<F>(f), std::forward<Args>(args)...);
			case al::launch::deferred:
				return std::async(std::launch::deferred, std::forward<F>(f), std::forward<Args>(args)...);
			case al::launch::pooled:
				auto task = std::make_shared<std::packaged_task<return_type()>>(
					std::bind(std::forward<F>(f), std::forward<Args>(args)...)
				);

				std::future<return_type> res = task->get_future();
				al::ThreadPool::getInstance().pushTask(task);
				return res;
		}
	}

}

#endif