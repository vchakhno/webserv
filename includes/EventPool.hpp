#pragma once

#include <sys/epoll.h>
#include <stdexcept>

#define MAX_EVENTS 50

class EventPool {
public:
	EventPool() throw (std::runtime_error);
	~EventPool();

	struct Event {
		int 	flags;
		int 	handler_type;
		void	*handler;
	};

	void	observe(int event_fd, Event event, std::string event_name) throw(std::runtime_error);
	Event	get_event();
private:
	int						fd;
	struct epoll_event		buffer[MAX_EVENTS];
	unsigned int			current;
	unsigned int			count;
};
