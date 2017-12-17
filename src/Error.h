#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <sstream>

struct Error : public std::runtime_error
{
	char const * error_type, * thrower_id, * error_msg;

	Error(char const * error_type, char const * thrower_id, char const * error_msg) : std::runtime_error(make_what(error_type, thrower_id, error_msg)), error_type(error_type), thrower_id(thrower_id), error_msg(error_msg)
	{}

	static std::string make_what(char const * error_type, char const * thrower_id, char const * error_msg)
	{
		std::stringstream ss;
		ss << "[" << error_type << "] {" << thrower_id << "} : |" << error_msg << "|";
		return ss.str();
	}
};

#define DEFINE_ERROR(name, base) struct name : public base {\
		name(char const * thrower_id, char const * msg) : Error(#name, thrower_id, msg) {}\
		name(char const * msg) : Error(#name, "", msg) {}\
		name() : Error(#name, "", "") {}\
	};

#endif // !EXCEPTIONS_H
