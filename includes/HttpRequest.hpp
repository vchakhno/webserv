#pragma once

#include <stdint.h>
#include <string>

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
	std::string path;
};

HttpRequest	parse_http_request(std::string raw_request);
