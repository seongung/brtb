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

#include "console.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>

int get_key()
{
	DWORD mode, rd;
	HANDLE h;

	if ((h = GetStdHandle(STD_INPUT_HANDLE)) == NULL)
		return -1;

	GetConsoleMode( h, &mode );
	SetConsoleMode( h, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT) );

	int c = 0;
	ReadConsole( h, &c, 1, &rd, NULL );
	SetConsoleMode( h, mode );

	return c;
}

void set_colour(out_colours cl)
{
	WORD attr = 0;

	switch(cl)
	{
	case K_RED:
		attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case K_GREEN:
		attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case K_BLUE:
		attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
		break;
	case K_YELLOW:
		attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case K_CYAN:
		attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	case K_MAGENTA:
		attr = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
		break;
	case K_WHITE:
		attr = FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
		break;
	default:
		break;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
}

void reset_colour()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int get_key()
{
	struct termios oldattr, newattr;
	int ch;
	tcgetattr( STDIN_FILENO, &oldattr );
	newattr = oldattr;
	newattr.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
	return ch;
}

void set_colour(out_colours cl)
{
}

void reset_colour()
{
}
#endif // _WIN32

inline void comp_localtime(const time_t* ctime, tm* stime)
{
}

printer* printer::oInst = nullptr;

printer::printer()
{
}

bool printer::open_logfile(const char* file)
{
	return true;
}

void printer::print_msg(verbosity verbose, const char* fmt, ...)
{
}

void printer::print_str(const char* str)
{
}
