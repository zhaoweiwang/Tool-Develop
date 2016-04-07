#include "Head.h"

const string BitVersion = "x64";

//根据pFind Studio系列软件的传统，打印pQuant-MS2.exe的名称、版本号和pFind官网地址等信息
//该模块不需要经常改动，仅在软件升级、改名等时候需要重新设置打印的LOGO

void printLogo()
{
	cout << endl;
	cout << "\t  *********************************************" << endl;
	cout << "\t /       pIsobariQ(" << BitVersion << ") from pFind Studio      \\" << endl;
	cout << "\t/        Email  :  pfind@ict.ac.cn              \\" << endl;
	cout << "\t\\        Website:  http://pfind.ict.ac.cn       /" << endl;
	cout << "\t \\       Time   :  " << TIMESTRING << "            /" << endl;
	cout << "\t  *********************************************" << endl << endl;
}