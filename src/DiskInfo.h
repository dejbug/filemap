#ifndef DISK_INFO_H
#define DISK_INFO_H

#include <windows.h>

#include <sstream>

#include "common.h"

#include "DefinesToString.h"
#include "Error.h"

struct DiskInfo : public DefinesToString
{
	DEFINE_ERROR(QueryError, Error)

	DWORD sectors_per_cluster = 0;
	DWORD bytes_per_sector = 0;
	DWORD free_cluster_count = 0;
	DWORD total_cluster_count = 0;

	__int64 total_byte_count = 0;
	__int64 free_byte_count = 0;
	__int64 free_byte_count_for_caller = 0;

	DiskInfo(wchar_t const * root=L"C:")
	{
		BOOL ok = GetDiskFreeSpace(root, &sectors_per_cluster, &bytes_per_sector, &free_cluster_count, &total_cluster_count);
		if (!ok) throw QueryError("DiskInfo", "GetDiskFreeSpace");

		ULARGE_INTEGER FreeBytesAvailableToCaller;
		ULARGE_INTEGER TotalNumberOfBytes;
		ULARGE_INTEGER TotalNumberOfFreeBytes ;

		ok = GetDiskFreeSpaceEx(root, &FreeBytesAvailableToCaller, &TotalNumberOfBytes, &TotalNumberOfFreeBytes);
		if (!ok) throw QueryError("DiskInfo", "GetDiskFreeSpaceEx");

		free_byte_count_for_caller = FreeBytesAvailableToCaller.QuadPart;
		total_byte_count = TotalNumberOfBytes.QuadPart;
		free_byte_count = TotalNumberOfFreeBytes.QuadPart;
	}

	__int64 calc_free_size() const
	{
		return (__int64) free_cluster_count * sectors_per_cluster * bytes_per_sector;
	}

	std::wstring to_string() const
	{
		std::wstringstream ss;
		ss << "DiskInfo{" <<
			"sectors_per_cluster=" << sectors_per_cluster <<
			", bytes_per_sector=" << bytes_per_sector <<
			", free_cluster_count=" << free_cluster_count <<
			", total_cluster_count=" << total_cluster_count <<
			", total_byte_count=" << total_byte_count <<
			", free_byte_count=" << free_byte_count <<
			", free_byte_count_for_caller=" << free_byte_count_for_caller <<
			"}";
		return ss.str();
	}
};

#endif // !DISK_INFO_H
