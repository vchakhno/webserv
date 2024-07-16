#include "EventPool.hpp"
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

static struct epoll_event	serialize_handler(EventPool::Event event)
{
	struct epoll_event serialized;

	serialized.events = event.flags;
	serialized.data.u64 = ((uintptr_t)event.handler & ~(0b11ull << 62)) | ((uintptr_t) event.handler_type << 62);
	return serialized;
}

void	EventPool::observe(int event_fd, Event event) throw(std::runtime_error)
{
	struct epoll_event	serialized;

	serialized = serialize_handler(event);
	if (epoll_ctl(fd, EPOLL_CTL_ADD, event_fd, &serialized) == -1)
		throw std::runtime_error(std::string(__FILE__) + ": " + strerror(errno));
}

static EventPool::Event		deserialize_event(struct epoll_event serialized)
{
	EventPool::Event event = {
		.flags = serialized.events,
		.handler_type = serialized.data.u64 >> 62,
		.handler = (void *)((serialized.data.u64 << 2) >> 2)
	};
	return event;
}

EventPool::Event	EventPool::get_event()
{
	if (current == count)
	{
		count = epoll_wait(fd, buffer, MAX_EVENTS, -1);
		current = 0;
	}
	return deserialize_event(buffer[current++]);
}
