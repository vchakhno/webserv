[ ] Request-Line
	- [x] Method
	- [ ] Request-URI
		- [ ] "*"
		- [ ] absoluteURI
			- [ ] scheme	# Velimir
			- [ ] authority # H
			- [ ] path		# H
			- [ ] query		# Velimir
		- [ ] abs_path
		- [ ] authority
	- [ ] HTTP-Version


"122.222.111.22:3343:AJ@RFEE:111:1:1:115:5678"

authority	=	[ userinfo "@" ] host [ ":" port ]

userinfo	=	*( unreserved / pct-encoded / sub-delims / ":" )

host		=	IP-literal / IPv4address / reg-name

port		=	*DIGIT

reg-name	=	*( unreserved / pct-encoded / sub-delims )

unreserved	=	ALPHA / DIGIT / "-" / "." / "_" / "~"

pct-encoded	=	"%" HEXDIG HEXDIG

sub-delims	=	"!" / "$" / "&" / "'" / "(" / ")"
				/ "*" / "+" / "," / ";" / "="

IPv4address	=	dec-octet "." dec-octet "." dec-octet "." dec-octet

IP-literal	=	"[" ( IPv6address / IPvFuture  ) "]"

IPv6address =	6( h16 ":" ) ls32
				/                       "::" 5( h16 ":" ) ls32
				/ [               h16 ] "::" 4( h16 ":" ) ls32
				/ [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
				/ [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
				/ [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
				/ [ *4( h16 ":" ) h16 ] "::"              ls32
				/ [ *5( h16 ":" ) h16 ] "::"              h16
				/ [ *6( h16 ":" ) h16 ] "::"

IPvFuture	=	"v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )