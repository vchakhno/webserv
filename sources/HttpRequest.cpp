#include "HttpRequest.hpp"
#include <stdexcept>
#include <iostream>

HttpRequest::HttpRequest() : parsing_stage(PARSING_STAGE_REQUEST_LINE) {}

bool	match_string(
	const std::string &string,
	const std::string &substring,
	std::size_t &pos
) {
	if (pos + substring.size() > string.size())
		return false;
	for (std::size_t i = 0; i < substring.size() && string.size(); i++)
		if (string[i + pos] != substring[i])
			return false;
	pos += substring.size();
	return true;
}

bool	match_charset(
	const std::string &string,
	const char *charset,
	std::size_t &pos
) {
	static const std::string set(charset);

	if (set.find(string[pos]) == std::string::npos)
		return false;
	pos++;
	return true;
}

// GET	/etc/test HTTP1.1

void	parse_http_method(
	const std::string &raw_request,
	std::size_t	&pos,
	HttpMethod &method
) {
	std::string	methods[] = {
		"GET", "POST", "HEAD", "UPDATE", "PUT", "DELETE"
	};

	for (int i = 0; i < (int) (sizeof(methods) / sizeof(*methods)); i++)
	{
		if (match_string(raw_request, methods[i], pos))
		{
			if (!match_string(raw_request, " ", pos))
				break;
			method = (HttpMethod) i;
			return;
		}
	}
	throw std::runtime_error("Request syntax error: Unknown HTTP method");
}

bool	char_is_hex(char c)
{
	return (
		isdigit(c)
		|| ('A' <= c || c <= 'F')
		|| ('a' <= c || c <= 'f')
	);
}

bool	match_pct_encoded(const std::string &str, std::size_t &pos)
{
	if (
		str.size() < 3
		|| str[pos] != '%'
		|| !char_is_hex(str[pos + 1])
		|| !char_is_hex(str[pos + 2])
	)
		return false;
	pos += 3;
	return true;
}

bool	match_sub_delims(const std::string &str, std::size_t &pos)
{
	return match_charset(str, "!$&'()*+,;=", pos);
}

bool	match_unreserved(const std::string &str, std::size_t &pos)
{
	if (isalnum(str[pos]))
	{
		pos++;
		return true;
	}
	return match_charset(str, "-._~", pos);
}

bool	parse_reg_name(const std::string &str, std::size_t &pos)
{
	while (pos != str.size() && (
		match_unreserved(str, pos)
		|| match_pct_encoded(str, pos)
		|| match_sub_delims(str, pos)
	))
		;
	return true;
}

bool	parse_userinfo(const std::string &str, std::size_t &pos)
{
	while (pos != str.size() && (
		match_unreserved(str, pos)
		|| match_pct_encoded(str, pos)
		|| match_sub_delims(str, pos)
		|| match_string(str, ":", pos)
	))
		;
	return true;
}

void	parse_segment(const std::string &str, std::size_t &pos)
{
	while (pos != str.size() && (
		match_unreserved(str, pos)
		|| match_pct_encoded(str, pos)
		|| match_sub_delims(str, pos)
	))
		;
}

bool	parse_segment_nz(const std::string &str, std::size_t &pos)
{
	std::size_t	tmp = pos;
	parse_segment(str, pos);
	return tmp != pos;
}

bool	parse_ip_literal(const std::string &str, std::size_t &pos)
{
	(void)pos;
	(void)str;
	return true;
}

bool	parse_ipv4_address(const std::string &str, std::size_t &pos)
{
	(void)pos;
	(void)str;
	return true;
}

bool	parse_host(const std::string &str, std::size_t &pos)
{
	if (parse_ip_literal(str, pos)
		|| parse_ipv4_address(str, pos)
		|| parse_reg_name(str, pos))
		return true;
	return false;
}

bool	parse_port(const std::string &str, std::size_t &pos)
{
	std::size_t	start_pos = pos;
	while (match_charset(str, "0123456789", pos))
		;
	return start_pos != pos;
}

bool	parse_authority(const std::string &str, std::size_t &pos)
{
	// [ userinfo "@" ] host [ ":" port ]

	if (parse_userinfo(str, pos))
	{
		if (match_string(str, "@", pos))
			;
	}
	if (!parse_host(str, pos))
		return false;
	if (match_string(str, ":", pos))
	{
		if (parse_port(str, pos))
			;
	}
	return true;
}

bool	parse_abs_path(const std::string &str, std::size_t &pos)
{
	if (!match_string(str, "/", pos))
		return false;


	if (parse_segment_nz(str, pos))
		while (match_string(str, "/", pos))
			parse_segment(str, pos);

	return true;
}

void	parse_http_version(const std::string &str, std::size_t &pos)
{
	if (match_string(str, "HTTP/1.1\r\n", pos))
		throw std::runtime_error("Bad path");
}



void	HttpRequest::parse_request_uri(const std::string &line, std::size_t &pos) throw(std::runtime_error)
{
	std::size_t	uri_start;

	uri_start = pos;
	if (match_string(line, "*", pos))
		this->uri_type = REQUEST_URI_WILDCARD;
	else if (parse_abs_path(line, pos)) {
		this->uri_type = REQUEST_URI_ABSOLUTE_PATH;
		this->uri = line.substr(uri_start, pos - uri_start - 1);
	}
	else
		throw std::runtime_error("Request syntax error: Invalid Request-URI");
	if (!match_string(line, " ", pos))
		throw std::runtime_error(REQUEST_URI_ILLEGAL_CHARACTER);
}

void	HttpRequest::parse_request_line(std::string line) throw(std::runtime_error)
{
	// Method SP Request-URI SP HTTP-Version CRLF
	std::size_t	pos = 0;

	parse_http_method(line, pos, this->method);
	parse_request_uri(line, pos);
	parse_http_version(line, pos);
	
}

void	HttpRequest::parse_request_header(std::string line) throw(std::runtime_error)
{
	(void) line;
}

void	HttpRequest::parse_request_body(std::string line) throw(std::runtime_error)
{
	(void) line;
}

void	HttpRequest::parse_request_chunk(char *recv_buffer, ssize_t recv_size) throw(std::runtime_error)
{
	size_t					previous_size;
	std::string::size_type	newline_pos;
	std::string				line;

	switch (this->parsing_stage)
	{
		case PARSING_STAGE_REQUEST_LINE:
			previous_size = this->request_buffer.size();
			this->request_buffer.append(recv_buffer, recv_size);
			newline_pos = this->request_buffer.find("\r\n", previous_size);
			if (newline_pos == std::string::npos)
				return;
			line = this->request_buffer.substr(previous_size, newline_pos);
			parse_request_line(line);
			this->parsing_stage = PARSING_STAGE_DONE;
			break;
		case PARSING_STAGE_HEADER:
			break;
		case PARSING_STAGE_BODY:
			break;
		case PARSING_STAGE_DONE:
			break;
	}
}

std::ostream &operator<<(std::ostream &output, const HttpRequest &request) {
	std::string	methods[] = {
		"GET", "POST", "HEAD", "UPDATE", "PUT", "DELETE"
	};
	std::string	uri_types[] = {
		"*", "absoluteURI", "abs_path", "authority"
	};

	if (request.parsing_stage > PARSING_STAGE_REQUEST_LINE) {
		output	<< "Method: " <<  methods[request.method] << "\n"
				<< "Uri type: " <<  uri_types[request.uri_type] << "\n";
	} else {
		output	<< "Method: not parsed\n"
				<< "Uri type: not parsed\n";
	}
	output << "Header: " << request.headers << "\n";
	output << "Body: " << request.body;
	return output;
}
