#ifndef FILE_MAPPING_H
#define FILE_MAPPING_H

#include <windows.h>

#include <sstream>

#include "DefinesToString.h"
#include "FileHandle.h"

struct FileMapping : public DefinesToString
{
	DEFINE_ERROR(OpenError, Error)

	HANDLE handle = NULL;
	bool originator = false;
	// ULARGE_INTEGER file_size;

	explicit FileMapping(FileHandle const & fh)
	{
		handle = CreateFileMapping(fh.handle, nullptr, PAGE_READONLY, 0, 0, nullptr);
		if (NULL == handle)
			throw OpenError("FileHandle", "couldn't open file for reading");
		originator = ERROR_ALREADY_EXISTS != GetLastError();

		// if (!fh.get_size(file_size))
		// 	throw OpenError("FileHandle", "couldn't query file size");
	}

	virtual ~FileMapping()
	{
		if (NULL != handle)
		{
			CloseHandle(handle);
			handle = NULL;
		}
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << L"FileMapping{" <<
			"handle=" << handle <<
			", originator=" << originator <<
			"}";
		return ss.str();
	}
};

#endif // !FILE_MAPPING_H
