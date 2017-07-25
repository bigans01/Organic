#pragma once

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <future>
#include "thread_safe_queue.h"
#include "function_wrapper.h"
#include "join_threads.h"

class thread_pool
{
	std::atomic_bool done;
	thread_safe_queue<function_wrapper> work_queue;
	std::vector<std::thread> threads;
	join_threads joiner;

	void worker_thread()
	{
		while (!done)
		{
			function_wrapper task;
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
	thread_pool() :
		done(false), joiner(threads)
	{
		unsigned const thread_count = 2;			// change this possibly...

		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{

			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}

	~thread_pool()
	{
		done = true;
	}



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
	};



	
};

#endif