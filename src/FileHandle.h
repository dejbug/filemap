#ifndef FILE_HANDLE_H
#define FILE_HANDLE_H

#include <windows.h>

#include <sstream>

#include "DefinesToString.h"

struct FileHandle : public DefinesToString
{
	DEFINE_ERROR(OpenError, Error)

	HANDLE handle = INVALID_HANDLE_VALUE;
	wchar_t const * path = nullptr;

	FileHandle(wchar_t const * path) : path(_wcsdup(path))
	{
		handle = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if (INVALID_HANDLE_VALUE == handle)
			throw OpenError("FileHandle", "couldn't open file for reading");
	}

	virtual ~FileHandle()
	{
		if (INVALID_HANDLE_VALUE != handle)
		{
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}

	bool get_size(ULARGE_INTEGER & uli) const
	{
		return get_file_size(handle, uli);
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << L"FileHandle{" <<
			"handle=" << handle <<
			", path='" << path << "'" <<
			"}";
		return ss.str();
	}
};

#endif // !FILE_HANDLE_H
