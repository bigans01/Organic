#pragma once

#ifndef FUNCTION_SAFE_QUEUE_H
#define FUNCTION_SAFE_QUEUE_H

#include <thread>
#include <mutex>

template<typename T>
class thread_safe_queue
{
private:
	struct node
	{
		std::shared_ptr<T> data;
		std::unique_ptr<node> next;
	};

	std::mutex head_mutex;
	std::unique_ptr<node> head;
	std::mutex tail_mutex;
	node* tail;
	std::condition_variable data_cond;

public:
	thread_safe_queue() :
		head(new node), tail(head.get())
	{}
	thread_safe_queue(const thread_safe_queue& other) = delete;
	thread_safe_queue& operator=(const thread_safe_queue& other) = delete;

	std::shared_ptr<T> try_pop();
	bool try_pop(T& value);
	std::shared_ptr<T> wait_and_pop();
	void wait_and_pop(T& value);
	void push(T new_value);
	void empty();


};

#endif


/*
struct node
{
std::shared_ptr<T> data;
std::unique_ptr<node> next;
};
*/