#include <windows.h>
// #include <wchar.h>
// #include <strsafe.h>

#include <iostream>
#include <clocale>

#include "common.h"

#include "Error.h"
#include "DefinesToString.h"
#include "DiskInfo.h"
#include "FileHandle.h"
#include "FileMapping.h"
#include "FileView.h"
#include "CharStream.h"

wchar_t const * const path = L"LICENSE";

// int __cdecl _wmain(int argc, wchar_t ** argv, wchar_t ** envp)
int __cdecl main(int argc, char ** argv, char ** /*envp*/)
{
	std::setlocale(LC_ALL, "en_US.utf8");

	std::wcout << std::endl;

	print_args(std::wcout, argc, argv);
	// print_env(std::wcout, envp);

	std::wcout << std::endl;

	std::wcout << "* allocation granularity: " << get_allocation_granularity() << std::endl;

	std::wcout << std::endl;

	DiskInfo di;
	std::wcout << di << std::endl;

	std::wcout << std::endl;

	FileHandle fh(path);
	// std::wcout << fh << std::endl;

	// FileMapping fm(fh);
	// std::wcout << fm << std::endl;

	// FileView fv(fm);
	// std::wcout << fv << std::endl;

	CharStream cs(fh);
	std::wcout << cs << std::endl;

	// std::wcout << std::endl;

	// std::wcout << cs.getc(0) << cs.getc(1) << std::endl;
	// std::wcout << cs.getw(0) << cs.getw(1) << std::endl;

	std::wcout << std::endl;

	print_hr(std::wcout);

	for (size_t i=0; i<64; ++i)
		std::wcout << cs.next();
	std::wcout << std::endl;

	print_hr(std::wcout);

	return 0;
}
