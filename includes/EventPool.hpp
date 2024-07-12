#pragma once

#include <sys/epoll.h>
#include <stdexcept>

class Event;

#define MAX_EVENTS 50

class EventPool {
public:
	EventPool() throw (std::runtime_error);
	~EventPool();

	void	observe(int new_fd, int event_mask, Event &event) throw(std::runtime_error);
	Event&	get_event(int &event_flags);
private:
	int					fd;
	struct epoll_event	buffer[MAX_EVENTS];
	unsigned int		current;
	unsigned int		count;
};
