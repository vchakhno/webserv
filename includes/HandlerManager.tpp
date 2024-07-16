#pragma once

#include <vector>

template <class T>
class HandlerManager {
public:
	HandlerManager() {}
	~HandlerManager()
	{
		for (size_t i = 0; i < handlers.size(); i++)
			delete handlers[i];
	}

	// todo: add throw
	void add_handler(T *handler)
	{
		handlers.push_back(handler);
	}
private:
	std::vector<T*>	handlers;
};
