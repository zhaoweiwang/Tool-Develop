#include "Head.h"

void checkDate(){

	tm exptm;
	exptm.tm_year = 2015 - 1900;
	exptm.tm_mon = 12;
	exptm.tm_mday = 31;
	exptm.tm_hour = exptm.tm_min = exptm.tm_sec = 0;

	if (mktime(&exptm) < time(0))
	{
		printf("Sorry, the license has expired.\nPlease visit http://pfind.ict.ac.cn/software"
			" and download the newest version.\n\n");
		exit(0);
	}

}