#ifndef COMMON_H
#define COMMON_H

#include <windows.h>

#include <string>
#include <ostream>
#include <iomanip>

#define UNUSED __attribute__((__unused__))

std::wostream & operator<<(std::wostream & os, HANDLE const & that)
{
	return os << "<" << std::hex << std::setfill(L'0') << std::setw(8) << (size_t) that << ">";
}

void print_env(std::wostream & os, char * const * envp)
{
	for (size_t i=0; envp[i]; ++i)
		os << "envp[" << i << "]: '" << envp[i] << "'\n";
}

void print_args(std::wostream & os, int argc, char * const * argv)
{
	for (int i=0; i < argc; ++i)
		os << "argv[" << i << "]: '" << argv[i] << "'\n";
}

void print_hr(std::wostream & os, size_t length=78, wchar_t ch='-', bool endline=true)
{
	if (78 == length && '-' == ch) os << "------------------------------------------------------------------------------"; else for (size_t i=0; i<length; ++i) os << ch;
	if (endline) os << std::endl;
}

DWORD get_allocation_granularity()
{
	SYSTEM_INFO si;
	memset(&si, 0, sizeof si);
	GetSystemInfo(&si);
	return si.dwAllocationGranularity;
}

bool get_file_size(HANDLE handle, ULARGE_INTEGER & uli)
{
	BY_HANDLE_FILE_INFORMATION fi;
	memset(&fi, 0, sizeof fi);

	if (!GetFileInformationByHandle(handle, &fi))
		return false;

	uli.LowPart = fi.nFileSizeLow;
	uli.HighPart = fi.nFileSizeHigh;

	return true;
}

#endif // !COMMON_H
