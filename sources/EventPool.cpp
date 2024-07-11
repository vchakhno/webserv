#include "EventPool.hpp"
#include <string.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>

EventPool::EventPool() throw (std::runtime_error) : current(), count()
{
	fd = epoll_create(1);
	if (fd == -1)
		throw std::runtime_error(strerror(errno));
}

EventPool::~EventPool()
{
	close(fd);
}

void	EventPool::observe(int new_fd, int event_mask) throw(std::runtime_error)
{
	t_event	event;

	event.data.fd = new_fd;
	event.events = event_mask;
	if (epoll_ctl(fd, EPOLL_CTL_ADD, new_fd, &event) == -1)
		throw std::runtime_error(strerror(errno));
}

EventPool::t_event *EventPool::get_event()
{
	if (current < count)
		return &buffer[current++];
	count = epoll_wait(fd, buffer, MAX_EVENTS, -1);
	current = 0;
	return &buffer[current];
}
