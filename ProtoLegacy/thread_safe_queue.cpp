#include "stdafx.h"
#include <stdio.h>
//#include "thread_safe_queue.h"

/*
template<typename T> void thread_safe_queue<T>::push(T new_value)
{
	std::shared_ptr<T> new_data(
		std::make_shared<T>(std::move(new_value)));
	std::unique_ptr<node> p(new node);
	{
		std::lock_guard<std::mutex> tail_lock(tail_mutex);
		tail->data = new_data;
		node* const new_tail = p.get();
		tail->next = std::move(p);
		tail = new_tail;
	}
	data_cond.notify_one();
}



template<typename T> std::shared_ptr<T> thread_safe_queue<T>::try_pop()
{
	std::unique_ptr<node> old_head = try_pop_head();
	return old_head ? old_head->data : std::shared_ptr<T>();
}

template<typename T> bool thread_safe_queue<T>::try_pop(T& value)
{
	std::unique_ptr<node> const old_head = try_pop_head(value);
	return old_head;
}

template<typename T> std::shared_ptr<T> thread_safe_queue<T>::wait_and_pop()
{
	std::unique_ptr<node> const old_head = wait_pop_head();
	return old_head->data;
}

template<typename T> void thread_safe_queue<T>::wait_and_pop(T& value)
{
	std::unique_lock<std::mutex> head_lock(wait_fort_data());
	value = std::move(*head->data);
	return pop_head();
}

template<typename T> void thread_safe_queue<T>::empty()
{
	std::lock_guard<std::mutex> head_lock(head_mutex);
}
*/

//template<typename T> thread_safe_queue<T>::thread_safe_queue() : head(new node), tail(head.get()) {}