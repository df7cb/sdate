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
