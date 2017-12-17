#ifndef FILE_SIZE_H
#define FILE_SIZE_H

#include <windows.h>

#include <sstream>

#include "common.h"
#include "Error.h"
#include "DefinesToString.h"

struct FileSize : public DefinesToString
{
	DEFINE_ERROR(OpenError, Error)

	__int64 size;
	DWORD hi;
	DWORD lo;

	FileSize(HANDLE handle)
	{
		ULARGE_INTEGER uli;
		if (!get_file_size(handle, uli))
			throw OpenError("FileSize", "couldn't query file size");
		size = uli.QuadPart;
		hi = uli.HighPart;
		lo = uli.LowPart;
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << L"FileSize{" <<
			"size=" << size <<
			", hi=" << hi <<
			", lo=" << lo <<
			"}";
		return ss.str();
	}
};

#endif // !FILE_SIZE_H
