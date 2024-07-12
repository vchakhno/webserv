#include "EventPool.hpp"
#include "Event.hpp"
#include <string.h>
#include <iostream>
#include <cerrno>
#include <unistd.h>

EventPool::EventPool() throw (std::runtime_error) : current(), count()
{
	fd = epoll_create(1);
	if (fd == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
}

EventPool::~EventPool()
{
	close(fd);
}

void	EventPool::observe(int new_fd, int event_mask, Event &event) throw(std::runtime_error)
{
	struct epoll_event	new_event;

	new_event.data.fd = new_fd;
	new_event.data.ptr = (void *) &event;
	new_event.events = event_mask;
	if (epoll_ctl(fd, EPOLL_CTL_ADD, new_fd, &new_event) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
}

Event&	EventPool::get_event(int &event_flags)
{
	if (current == count)
	{
		count = epoll_wait(fd, buffer, MAX_EVENTS, -1);
		current = 0;
	}
	event_flags = buffer[current].events;
	return *(Event *)buffer[current++].data.ptr;
}
