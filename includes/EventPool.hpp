#pragma once

#include <sys/epoll.h>
#include <stdexcept>

#define MAX_EVENTS 50

class EventPool {
public:
	EventPool() throw (std::runtime_error);
	~EventPool();

	using t_event = struct epoll_event;
	void	observe(int new_fd, int event_mask) throw(std::runtime_error);
	t_event	*get_event();
private:
	int				fd;
	t_event			buffer[MAX_EVENTS];
	unsigned int	current;
	unsigned int	count;
};
