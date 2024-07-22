#include <string>
#include <ctype>

bool	parse_uri_scheme(
	const std::string &string,
	std::size_t &pos
) {
	if (!isalpha(string[pos]))
		return false;
	pos++;
	while (isalnum(string[pos])
		|| string[pos] == '+'
		|| string[pos] == '-'
		|| string[pos] == '.')
		pos++
	return true;
}