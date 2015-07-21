#include "stdafx.h"
#include "FileIoHelper.h"

bool create_very_big_file(_In_ const wchar_t* file_path, _In_ uint64_t size_in_mb) {
	_ASSERTE(NULL != file_path);
	if (NULL == file_path) return false;

	if (is_file_existsW(file_path))
	{
		::DeleteFileW(file_path);
	}

	// create very big file
	HANDLE file_handle = CreateFile(
		file_path,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == file_handle)
	{
		printf("err ] CreateFile( %ws ) failed. gle = %u\n", file_path, GetLastError());
		return false;
	}

	LARGE_INTEGER file_size = { 0 };
	//file_size.LowPart = 0;
	//file_size.HighPart = 1;
	file_size.QuadPart = (1024 * 1024) * size_in_mb;	// size in mega byte

	if (!SetFilePointerEx(file_handle, file_size, NULL, FILE_BEGIN))	// file의 position을 file의 size만큼 이동시킴
	{
		printf("err ] SetFilePointerEx() failed. gle = %u\n", GetLastError());

		CloseHandle(file_handle);
		return false;
	}

	SetEndOfFile(file_handle);	// file size만큼 handle을 옮기면 순식간에 file size 크기를 가진 파일이 만들어짐.
	CloseHandle(file_handle);
	return true;
}

bool file_copy_using_read_write(_In_ const wchar_t* src_file, _In_ const wchar_t* dst_file) {
	_ASSERTE(NULL != src_file);
	_ASSERTE(NULL != dst_file);
	if (NULL == src_file || NULL == dst_file)	return false;

	if (!is_file_existsW(src_file)) {
		log_err
			"err] no src file = %ws\n",
			src_file
		log_end
		return false;
	}
	if (is_file_existsW(dst_file)) {
		DeleteFileW(dst_file);
	}

	// open src file with READ mode
	HANDLE src_handle = CreateFileW(
		src_file,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == src_handle) {
		log_err
			"err] CreateFile(%ws) failed. gle = %u\n",
			src_file,
			GetLastError()
		log_end
		CloseHandle(src_handle);
		return false;
	}

	// open dst file with WRITE mode
	HANDLE dst_handle = CreateFileW(
		dst_file,
		GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (INVALID_HANDLE_VALUE == dst_handle) {
		log_err
			"err] CreateFile(%ws) failed. gle = %u\n",
			src_file,
			GetLastError()
			log_end
			CloseHandle(dst_handle);
		return false;
	}

	// file copy
	bool ret = false;
	char buf[4096] = { 0 };
	DWORD bytes_written = 0;
	DWORD bytes_read = 0;

	do {
		// read from src
		if (!ReadFile(src_handle, buf, sizeof(buf), &bytes_read, NULL)) {
			log_err
				"err] ReadFile(src_handle) failed. gle = %u\n",
				GetLastError()
			log_end
			break;
		}
		else {
			if (0 == bytes_read) {
				ret = true;
				break;
			}
		}

		// write to dst
		if (!WriteFile(dst_handle, buf, sizeof(buf), &bytes_written, NULL)) {
			log_err
				"err] WriteFile(dst_handle) failed. gle = %u\n",
				GetLastError()
			log_end
			break;
		}
	} while (true);
	CloseHandle(src_handle);
	CloseHandle(dst_handle);
	return ret;
}