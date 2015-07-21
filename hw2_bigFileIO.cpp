// hw2_bigFileIO.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "FileIoHelper.h"
#include "FileIoHelperClass.h"
#include "stopwatch.h"

int _tmain(int argc, _TCHAR* argv[])
{
	_ASSERTE(create_very_big_file(L"C:\\aaa.txt", 1024*5));

	StopWatch sw;
	sw.Start();

	// file copy read write
	_ASSERTE(file_copy_using_read_write(L"C:\\aaa.txt", L"C:\\aaa2.txt"));
	sw.Stop();

	printf("file_copy_using_read_write: %f\n", sw.GetDurationSecond());

	uint32_t mfileSize = 4096;

	LARGE_INTEGER Size = { 0 };
	Size.QuadPart = (LONGLONG)(1024 * 1024)*(LONGLONG)mfileSize;

	// file copy using mmio
	DWORD bufSize = 2048;
	PUCHAR buf = (PUCHAR)malloc(bufSize);

	LARGE_INTEGER offset;
	offset.QuadPart = (LONGLONG)0;

	StopWatch sw2;
	sw2.Start();

	FileIoHelper FIOHelper;
	FIOHelper.FIOpenForRead(L"C:\\aaa.txt");
	FIOHelper.FIOCreateFile(L"c:\\aaa3.txt", Size);

	while (offset.QuadPart < Size.QuadPart) {
		if ((Size.QuadPart - offset.QuadPart) >(LONGLONG)bufSize) {
			bufSize = 2048;
		}
		else {
			bufSize = (DWORD)(Size.QuadPart - offset.QuadPart);
		}
		FIOHelper.FIOReadFromFile(offset, bufSize, buf);
		FIOHelper.FIOWriteToFile(offset, bufSize, buf);

		offset.QuadPart += (LONGLONG)bufSize;
	}
	sw2.Stop();
	printf("file_copy_using_memory_map: %f\n", sw2.GetDurationSecond());

	return 0;
}

