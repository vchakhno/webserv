#pragma once

#include <map>

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
	}

	void remove_handler(int fd)
	{
		delete handlers[fd];
		handlers.erase(fd);
	}
private:
	std::map<int, T*>		handlers;
};


// std::map<int, *Client> // fd to handler
// std::list<T>
// Client client[MAX_CLIENTS] + int available[MAX_CLIENTS] + int nb_clients;