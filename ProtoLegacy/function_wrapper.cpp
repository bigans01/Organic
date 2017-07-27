#include "stdafx.h"
#include <stdio.h>
//#include "function_wrapper.h"

/*
template<typename F> function_wrapper::function_wrapper(F&& f) : impl(new impl_type<F>(std::move(f))){}

function_wrapper::function_wrapper(function_wrapper&& other) : impl(std::move(other.impl)) {}

//function_wrapper::function_wrapper() {}

function_wrapper& function_wrapper::operator=(function_wrapper&& other)
{
	impl = std::move(other.impl);
	return *this;
}
*/

/*
function_wrapper& function_wrapper::operator=(const function_wrapper&)
{

}

*/