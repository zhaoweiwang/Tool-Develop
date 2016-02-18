#include "Head.h"

void checkDate(int year, int month, int day){

	tm exptm;
	exptm.tm_year = year - 1900;
	exptm.tm_mon = month;
	exptm.tm_mday = day;
	exptm.tm_hour = exptm.tm_min = exptm.tm_sec = 0;

	if (mktime(&exptm) < time(0))
	{
		printf("\t         Sorry, the license has expired.\n\nPlease visit http://pfind.ict.ac.cn/software"
			" and download the newest version.\n\n");
		exit(0);
	}
	printf("\t       The license will expire in %d/%d/%d\n\n", day, month, year);
}