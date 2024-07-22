#include <string>

#define UNRESERVED_SET "abcdefghijklmnopqrstuvwxywABCDEFGHIJKLMNOPQRSTUVWXYW0123456789"
#define SUB_DELIMS_SET "!$&'()*+,;="

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

bool	match_set(
	const std::string	&str,
	std::size_t			pos,
	char				*set
) {
	if (std::string(set).find(str[pos]) == std::string::npos)
		return false;
	return true;
}

bool	parse_userinfo(std::string str, std::size_t &pos)
{
	if (!isalnum(str[pos])
		&& str.find_first_of(SUB_DELIMS_SET)
		&& !)
		;
}

bool	parse_host(std::string str, std::size_t &pos)
{
	;
}

bool	parse_port(std::string str, std::size_t &pos)
{
	;
}

bool	parse_authority(std::string str)
{
	std::size_t pos;

	pos = 0;

	if (!parse_userinfo(str, pos)
		|| !match_string(str, "@", pos))
		pos = 0;
	if (!parse_host(str, pos))
		return false;
	if (match_string(str, ":", pos)
		&& !parse_port(str, pos))
		return false;
}