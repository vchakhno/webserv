#include "ClientHandler.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define RECV_SIZE 3000

ClientHandler::ClientHandler(int fd) : fd(fd) {}

ClientHandler::~ClientHandler()
{
	close(fd);
}

enum HttpMethod : uint8_t {
	METHOD_GET = 0,
	METHOD_POST = 1,
	METHOD_HEAD = 2,
	METHOD_UPDATE = 3,
	METHOD_PUT = 4,
	METHOD_DELETE = 5,
};

struct HttpRequest
{
	HttpMethod method;

	std::string	header;
	std::string	body;
};

bool	match_string(
	const std::string &string,
	const std::string &substring,
	std::size_t &pos
) {
	for (std::size_t i = 0; i < substring.size(); i++)
		if (string[i + pos] != substring[i])
			return false;
	pos += substring.size();
	return true;
}

// GET	/etc/test HTTP1.1

bool	parse_http_method(
	const std::string &raw_request,
	std::size_t	&pos,
	HttpMethod &method
) {
	std::string	methods[] = {
		"GET", "POST", "HEAD", "UPDATE", "PUT", "DELETE"
	};

	for (int i = 0; i < (int) sizeof(methods) / sizeof(*methods); i++)
	{
		if (match_string(raw_request, methods[i], pos))
		{
			method = (HttpMethod) i;
			return true;
		}
	}
	return false;
}

void	parse_http_request(std::string raw_request)
{
	HttpRequest	parsed_request;
	std::string	line;
	std::size_t	pos;

	// Parse request line hello
	// Method SP Request-URI SP HTTP-Version CRLF
	pos = 0;
	if (
		!parse_http_method(raw_request, pos, parsed_request.method)
		|| !match_string(raw_request, " ", pos)
		|| !parse_http_uri(raw_request, pos, parsed_request.method)
		|| !match_string(raw_request, " ", pos)
		|| !parse_http_version(raw_request, pos, parsed_request.method)
		|| !match_string(raw_request, "\r\n", pos)
	)
		
		
		
	// Parse header fields
	while ((end = raw_request.find("\n", start)) != std::string::npos)
	{
		line = raw_request.substr(start, end - start);
		std::cout << "Line: " << line << std::endl;
		start = end + 1;
	}
}

void	ClientHandler::execute(
	int event_flags,
	EventPool &pool,
	HandlerManager<ScriptHandler> &scripts,
	HandlerManager<FileHandler> &files,
	HandlerManager<ClientHandler> &clients_manager
) {
	(void) pool;
	(void) files;
	(void) scripts;

	if (event_flags & EPOLLIN)
	{
		char	buf[RECV_SIZE];
		int		ret;

		ret = recv(fd, buf, RECV_SIZE, 0);
		if (ret == -1)
			std::cerr << "Error while reading from client socket" << std::endl;
		else if (ret == 0)
		{
			// std::cout << http_request << std::endl;
			parse_http_request(http_request);
			clients_manager.remove_handler(fd);
			return;
		}
		else
			http_request.append(buf, ret);
	}
}
