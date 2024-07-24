#pragma once

#include <set>
#include <iostream>

template <class T>
class HandlerManager {
public:
	HandlerManager() {}
	~HandlerManager()
	{
		for (typename std::set<T*>::iterator it = handlers.begin(); it != handlers.end(); it++)
			delete *it;
	}

	void add_handler(T *handler) throw (std::bad_alloc)
	{
		handlers.insert(handler);
		std::cout << "Handler added (" << handlers.size() << " currently)" << std::endl;
	}

	void remove_handler(T* handler)
	{
		typename std::set<T*>::iterator it;
		
		it = handlers.find(handler);
		delete *it;
		handlers.erase(it);
		std::cout << "Handler removed (" << handlers.size() << " remaining)" << std::endl;
	}
private:
	std::set<T*>	handlers;
};
