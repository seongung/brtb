// ConsoleApplication1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>

typedef UINT(CALLBACK* MAIN)(int argc, char *argv[]);

int main(int argc, char *argv[])
{
	HMODULE hModule = ::LoadLibrary(_T("brt.dll"));
	if (hModule)	
	{
		MAIN p = (MAIN)::GetProcAddress(hModule, "main");
		if (p)
		{
			p(argc, argv);
		}
	}

    return 0;
}

