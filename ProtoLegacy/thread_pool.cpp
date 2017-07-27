#include "stdafx.h"
//#include "thread_pool.h"
//#include "function_wrapper.h"
//#include "thread_safe_queue.h"
#include <stdio.h>




/*
template<typename FunctionType> std::future<typename std::result_of<FunctionType()>::type> thread_pool::submit(FunctionType f)
{
	typedef typename std::result_of<FunctionType()>::type
			result_type;

		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
}

thread_pool::thread_pool() : done(false), joiner(threads) 
{
	{
		unsigned const thread_count = 2;			// change this possibly...

		try
		{
			for (unsigned i = 0; i < thread_count; ++i)
			{
				//threads.push_back(std::thread(&thread_pool::worker_thread, this));
			}
		}
		catch (...)
		{
			done = true;
			throw;
		}
	}
}

thread_pool::~thread_pool()
{
	done = true;
}

void thread_pool::worker_thread()
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
*/