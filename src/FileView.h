#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <windows.h>

#include <sstream>

#include "DefinesToString.h"
#include "FileMapping.h"

struct FileView : public DefinesToString
{
	struct ChunkInfo : public DefinesToString
	{
		DWORD const size = get_allocation_granularity();
		ULARGE_INTEGER offset;

		ChunkInfo(DWORD index=0)
		{
			offset.QuadPart = UInt32x32To64(index, size);
		}

		std::wstring to_string() const
		{
			std::wstringstream ss;
			ss << L"ChunkInfo{" << std::dec <<
				", offset=" << offset.QuadPart <<
				", offset.HighPart=" << offset.HighPart <<
				", offset.LowPart=" << offset.LowPart <<
				", size=" << size <<
				"}";
			return ss.str();
		}
	};

	VOID * base_ptr = nullptr;
	ChunkInfo const chunk;

	explicit FileView(FileMapping const & fm)
	{
		base_ptr = MapViewOfFile(fm.handle, FILE_MAP_READ, 0, 0, 0);
	}

	FileView(FileMapping const & fm, DWORD chunk_index) : chunk(chunk_index)
	{
		base_ptr = MapViewOfFile(fm.handle, FILE_MAP_READ, chunk.offset.HighPart, chunk.offset.LowPart, 0);
	}

	virtual ~FileView()
	{
		if (base_ptr)
		{
			UnmapViewOfFile(base_ptr);
			base_ptr = nullptr;
		}
	}

	operator char * () const
	{
		return (char *) base_ptr;
	}

	char operator[](size_t offset) const
	{
		return *(((char *) base_ptr) + offset);
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << L"FileView{" <<
			"base_ptr=" << (HANDLE) base_ptr << std::dec <<
			", chunk.offset=" << chunk.offset.QuadPart <<
			", chunk.size=" << chunk.size <<
			"}";
		return ss.str();
	}
};

#endif // !FILE_VIEW_H
