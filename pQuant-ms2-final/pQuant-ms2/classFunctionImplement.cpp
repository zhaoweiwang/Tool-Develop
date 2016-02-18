#include "Head.h"

void screenInfo::printInfoTo(ostream& output, string infoType, string inFo){
	string strtime = getTimeStr();
	output << "[" << infoType << "]" << inFo << " " << endl;
}

string screenInfo::getTimeStr(){
	time_t filetime;
	struct tm* timeInfo;
	time(&filetime);
	timeInfo = localtime(&filetime);
	string strtime = asctime(timeInfo);
	strtime = strtime.substr(0, strtime.length() - 1);
	return strtime;
}