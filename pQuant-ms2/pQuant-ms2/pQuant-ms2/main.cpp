/**************************************************
文件名：		main.cpp
创建人：		Mako Wan
加注日期：	2015-10-26
描述：		二级谱定量的主函数
**************************************************/

#include "Head.h"

using namespace std;

int main(int argc, char* argv[]){

	printLogo();				//打印logo；

	checkDate();				//检查软件是否过期；

	readCmdline(argc, argv);	//通过读取命令行or文件获取用户设置参数；

	readData();					//获取谱图信息；

	return 0;
}