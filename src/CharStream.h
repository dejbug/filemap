#ifndef CHAR_STREAM_H
#define CHAR_STREAM_H

#include <windows.h>

#include <sstream>

#include "DefinesToString.h"
#include "FileHandle.h"
#include "FileMapping.h"
#include "FileView.h"
#include "FileSize.h"

struct CharStream : public DefinesToString
{
	DEFINE_ERROR(OpenError, Error)
	DEFINE_ERROR(EndOfInputError, Error)
	DEFINE_ERROR(EncodingError, Error)
	DEFINE_ERROR(IncompleteError, Error)

	FileHandle const & fh;
	FileMapping const fm;
	FileView const fv;
	FileSize const fs;

	size_t next_offset = 0;

	std::mbstate_t state = std::mbstate_t();

	explicit CharStream(FileHandle const & fh) : fh(fh), fm(fh), fv(fm), fs(fh.handle)
	{}

	char getc(size_t offset) const
	{
		return fv[offset];
	}

	wchar_t getw(size_t offset) const
	{
		return (wchar_t) fv[offset];
	}

	wchar_t next()
	{
		wchar_t wc;

		size_t const bytes_left = fs.lo - next_offset;
		size_t const len = mbrtowc(&wc, (char *) fv + next_offset, bytes_left, &state);

		switch (len)
		{
			case (size_t) -2: throw IncompleteError(); break;
			case (size_t) -1: throw EncodingError(); break;
			case 0: throw EndOfInputError(); break;
			default: case 1: /*everything ok*/
				next_offset += len;
				break;
		}

		return wc;
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << L"CharStream{" <<
			"fh=" << fh <<
			", fm=" << fm <<
			", fv=" << fv <<
			", fs=" << fs <<
			", next_offset=" << next_offset <<
			"}";
		return ss.str();
	}
};

#endif // !CHAR_STREAM_H
