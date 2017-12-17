#ifndef DEFINES_TO_STRING_H
#define DEFINES_TO_STRING_H

#include <string>
#include <ostream>

struct DefinesToString
{
	virtual std::wstring to_string() const = 0;
};

std::wostream & operator<<(std::wostream & os, DefinesToString const & that)
{
	return os << that.to_string();
}

#endif // !DEFINES_TO_STRING_H
