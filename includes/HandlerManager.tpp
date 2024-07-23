#pragma once

#include <map>
#include <iostream>

template <class T>
class HandlerManager {
public:
	HandlerManager() {}
	~HandlerManager()
	{
		for (typename std::map<int, T*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
			delete it->second;
	}

	// todo: add throw
	void add_handler(int fd, T *handler)
	{
		handlers.insert(std::make_pair(fd, handler));
		std::cout << "Handler added (" << handlers.size() << " currently)" << std::endl;
	}

	void remove_handler(int fd)
	{
		delete handlers[fd];
		handlers.erase(fd);
		std::cout << "Handler removed (" << handlers.size() << " remaining)" << std::endl;
	}
private:
	std::map<int, T*>		handlers;
};


// std::map<int, *Client> // fd to handler
// std::list<T>
// Client client[MAX_CLIENTS] + int available[MAX_CLIENTS] + int nb_clients;