#pragma once

#include <Windows.h>

#define log_err	printf(
#define log_end	);

typedef struct FILE_CTX {
	HANDLE	fileHandle;
	DWORD	fileSize;
	//UINT64	fileSize;
	HANDLE	fileMap;
	PCHAR	fileView;
}FILE_CTX, *PFILE_CTX;


bool create_very_big_file(_In_ const wchar_t* filePath, _In_ uint64_t size_in_mb);
bool file_copy_using_read_write(_In_ const wchar_t* src, _In_ const wchar_t* dst);