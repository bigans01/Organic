#include "stdafx.h"
#include "thread_pool.h"
#include <stdio.h>


template<typename FunctionType> std::future<typename std::result_of<FunctionType()>::type> thread_pool::submit(FunctionType f)
{
	typedef typename std::result_of<FunctionType()>::type
			result_type;

		std::packaged_task<result_type()> task(std::move(f));
		std::future<result_type> res(task.get_future());
		work_queue.push(std::move(task));
		return res;
}