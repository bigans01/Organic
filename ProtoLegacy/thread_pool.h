#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <future>
#include <queue>
//#include "thread_safe_queue.h"
//#include "function_wrapper.h"
#include "join_threads.h"

class EnclaveCollectionActivateList;

template<typename T>
class thread_safe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;
public:
	thread_safe_queue()
	{}

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(std::move(new_value));	// was "data"?
		data_cond.notify_one();
	}

	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		value = std::move(data_queue.front());
		data_queue.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(mut);
		data_cond.wait(lk, [this] {return !data_queue.empty(); });
		std::shared_ptr<T> res(
			std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return false;
		value = std::move(data_queue.front());
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if (data_queue.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> res(
			std::make_shared<T>(std::move(data_queue.front())));
		data_queue.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(mut);
		return data_queue.empty();
	}
};




class function_wrapper
{
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	std::unique_ptr<impl_base> impl;
	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
	};
public:
	template<typename F>
	function_wrapper(F&& f):
		impl(new impl_type<F>(std::move(f)))
	{}

	void operator()() { impl->call(); }

	function_wrapper() = default;

	function_wrapper(function_wrapper&& other):
		impl(std::move(other.impl))
	{}

	function_wrapper& operator=(function_wrapper&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper(const function_wrapper&) = delete;
	function_wrapper(function_wrapper&) = delete;
	function_wrapper& operator=(const function_wrapper&) = delete;
};

class function_wrapper2
{
	struct impl_base {
		virtual void call() = 0;
		virtual ~impl_base() {}
	};

	std::unique_ptr<impl_base> impl;
	template<typename F>
	struct impl_type : impl_base
	{
		F f;
		impl_type(F&& f_) : f(std::move(f_)) {}
		void call() { f(); }
		//int call() { f(); }
	};
public:
	template<typename F>
	function_wrapper2(F&& f) :
		impl(new impl_type<F>(std::move(f)))
	{}

	void operator()() { impl->call(); }

	function_wrapper2() = default;

	function_wrapper2(function_wrapper2&& other) :
		impl(std::move(other.impl))
	{}

	function_wrapper2& operator=(function_wrapper2&& other)
	{
		impl = std::move(other.impl);
		return *this;
	}

	function_wrapper2(const function_wrapper&) = delete;
	function_wrapper2(function_wrapper2&) = delete;
	function_wrapper2& operator=(const function_wrapper2&) = delete;
};

class thread_pool
{
	std::atomic_bool done;
	thread_safe_queue<function_wrapper> work_queue;								// DO SEARCH FOR (7/25/2017)
	thread_safe_queue<function_wrapper2> work_queue2;
	//thread_safe_queue<int> lolwhat;
	std::vector<std::thread> threads;
	join_threads joiner;

	void worker_thread()
	{
		while (!done)
		{
			function_wrapper task;
			function_wrapper2 task2;

			
			if (work_queue.try_pop(task))
			{
				task();
			}
			else
			{
				std::this_thread::yield();
			}
			


		}
	}
public:
	/*
	template<class Function, class ...Args>
	std::future<typename std::result_of<Function(Args...)>::type>
		submit2(Function &&f, Args &&...args) 
	{
		std::packaged_task<typename std::result_of<Function(Args...)>::type()> task(std::bind(f,args...));
		auto res=task.get_future();
		work_queue.push(std::packaged_task<void()>(std::move(task)));
		return res;
	}	
	*/

	template<typename FunctionType>
	std::future<typename std::result_of<FunctionType()>::type>
		submit(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type
			result_type;

		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}



	template<class Function, class ...Args>
	std::future<EnclaveKeyDef::EnclaveKey>
		submit2(Function &&f, Args &&...args)
	{
		std::packaged_task<EnclaveKeyDef::EnclaveKey()> task(std::bind(f, args...));
		std::future<EnclaveKeyDef::EnclaveKey> res = task.get_future();
		work_queue2.push(std::packaged_task<EnclaveKeyDef::EnclaveKey()>(std::move(task)));
		return res;
	}



	// currently being enhanced (submit3) THIS IS WORKING! YES -- returns an INT

	template<typename FunctionType>
	std::future<int>
		submit3(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type
			result_type;

		std::packaged_task<int()> task(std::move(f));
		std::future<int> res(task.get_future());
		work_queue2.push(std::move(task));
		return res;
	}

	// currently being enhanced (submit4)
	template<typename FunctionType>
	std::future<EnclaveKeyDef::EnclaveKey>
		submit4(FunctionType f)
	{
		typedef typename std::result_of<FunctionType()>::type
			result_type;

		std::packaged_task<EnclaveKeyDef::EnclaveKey()> task(std::move(f));
		std::future<EnclaveKeyDef::EnclaveKey> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
	}


	thread_pool() : done(false), joiner(threads)
	{
		{
			unsigned const thread_count = 1;			// change this possibly...

			try
			{
				for (unsigned i = 0; i < thread_count; ++i)
				{
					threads.push_back(std::thread(&thread_pool::worker_thread, this));
				}
			}
			catch (...)
			{
				done = true;
				throw;
			}
		}
	}
	//	done(false), joiner(threads) {};
	//thread_pool(done(bool), joiner(std::vector<std::thread>));

	~thread_pool()
	{
		done = true;
	}



	//template<typename FunctionType>
	//std::future<typename std::result_of<FunctionType()>::type>
	//	submit(FunctionType f);
	



	
};

#endif