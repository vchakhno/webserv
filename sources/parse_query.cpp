#include <string>
#include <ctype>

//  query       = *( pchar / "/" / "?" )
//    pchar       = unreserved / pct-encoded / sub-delims / ":" / "@"
//      unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~
//      pct-encoded = "%" HEXDIG HEXDIG
//      sub-delims    = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="

// ALPHA DIGIT "-._~!$&'()*+,;=" pct-encoded

bool	match_unreserved(
	const std::string &string,
	std::size_t &pos
) {
	if (string.size() < 1 || (
		!isalnum(string[pos])
		&& std::string("-._~").find(string[pos]) == std::string::npos
	))
		return false;
	pos++;
	return true;
}

bool	match_escaped(
	const std::string &string,
	std::size_t &pos
) {
	if (string.size() < 3 || string[pos] != '%')
		return false;
	for (std::size_t i = pos + 1; i < pos + 3; i++)
		if (!isdigit(string[i]) && (string[i] < 'A' || 'F' < string[i]))
			return false;
	pos += 3;
	return true;
}

bool	parse_uri_query(
	const std::string &string,
	std::size_t &pos
) {
	
	return true;
}
