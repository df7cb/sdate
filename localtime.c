/*
 * (C) 2004 Christoph Berg <cb@df7cb.de>
 * GNU GPL.
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEN 100

int main (int argc, char **argv)
{
	char s[LEN];
	struct tm t;
	time_t ti;
	char *format = "%F %T";

	setlocale(LC_ALL, "");

	ti = time(NULL);
	localtime_r(&ti, &t);

	/*
	if((t.tm_year == 93 && t.tm_mon == 8) || t.tm_year > 93) {
		struct tm sep93 = { 0, 0, 0,
			31, 7, 93,
			0, 0, 0
		};
		time_t epoch = mktime(&sep93);
		int d = (int)((ti - epoch) / 86400);
		t.tm_year = 93;
		t.tm_mon = 8;
		t.tm_mday = d;
	}
	*/

	if(argc > 2)
		exit(2);
	if(argc == 2)
		format = argv[1];

	if(strftime(s, LEN, format, &t) == 0) {
		exit(1);
	}
	
	puts(s);

	return 0;
}
