#include "HttpRequest.hpp"
#include <stdexcept>
#include <iostream>

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

// GET	/etc/test HTTP1.1

bool	parse_http_method(
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
			method = (HttpMethod) i;
			return true;
		}
	}
	return false;
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
	static const std::string set("!$&'()*+,;=");

	if (set.find(str[pos]) == std::string::npos)
		return false;
	pos++;
	return true;
}

bool	match_unreserved(const std::string &str, std::size_t &pos)
{
	static const std::string set("-._~");

	if (!isalnum(str[pos])
		&& set.find(str[pos]) == std::string::npos)
		return false;
	pos++;
	return true;
}

void	parse_segment(const std::string &str, std::size_t &pos)
{
	while (str.size() != pos && (
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

bool	parse_abs_path(const std::string &str, std::size_t &pos)
{
	if (!match_string(str, "/", pos))
		return false;
	

	if (parse_segment_nz(str, pos))
		while (match_string(str, "/", pos))
			parse_segment(str, pos);

	return true;
}

HttpRequest	parse_http_request(std::string raw_request)
{
	HttpRequest	parsed_request;
	std::string	line;
	std::size_t	pos;
	std::size_t	path_start;

	// Parse request line hello
	// Method SP Request-URI SP HTTP-Version CRLF
	pos = 0;
	if (
		!parse_http_method(raw_request, pos, parsed_request.method)
		|| !match_string(raw_request, " ", pos)
	)
		throw std::runtime_error("Bad method");
	path_start = pos;
	if (
		!parse_abs_path(raw_request, pos)
		|| !match_string(raw_request, " ", pos)
	)
		throw std::runtime_error("Bad path");
	parsed_request.path = raw_request.substr(path_start, pos - path_start - 1);
	return parsed_request;
}