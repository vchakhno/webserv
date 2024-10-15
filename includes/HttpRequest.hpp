#pragma once

#include <stdint.h>
#include <string>
#include <stdexcept>
#include <sys/types.h>
#include "HttpRequest.hpp"

#define REQUEST_URI_ILLEGAL_CHARACTER "Request syntax error: Illegal character in Request-URI"

enum ParsingStage : uint8_t {
	PARSING_STAGE_REQUEST_LINE = 0,
	PARSING_STAGE_HEADER = 1,
	PARSING_STAGE_BODY = 2,
	PARSING_STAGE_DONE = 3,
};

enum HttpMethod : uint8_t {
	METHOD_GET = 0,
	METHOD_POST = 1,
	METHOD_HEAD = 2,
	METHOD_UPDATE = 3,
	METHOD_PUT = 4,
	METHOD_DELETE = 5,
};

enum RequestUriType : uint8_t {
	REQUEST_URI_ABSOLUTE_PATH = 0
};

// struct HttpRequestLine {
// 	HttpMethod		method;
// 	RequestUriType	uri_type;
// 	std::string		uri;
// };

class HttpRequest
{
public:
	HttpRequest();
	void	parse_request_chunk(char *recv_buffer, ssize_t recv_size) throw(std::runtime_error);
	void	parse_line(std::string line) throw(std::runtime_error);

	friend std::ostream &operator<<(std::ostream &output, const HttpRequest &request);

	ParsingStage	parsing_stage;
private:
	std::string		request_buffer;
	HttpMethod		method;
	RequestUriType	uri_type;
	std::string		uri;	
	std::string		headers;
	std::string		body;

	void	parse_request_uri(const std::string &line, std::size_t &pos) throw(std::runtime_error);
	void	parse_request_line(std::string line) throw(std::runtime_error);
	void	parse_request_header(std::string line) throw(std::runtime_error);
	void	parse_request_body(std::string line) throw(std::runtime_error);

};