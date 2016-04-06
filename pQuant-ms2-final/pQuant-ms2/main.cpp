/**************************************************
文件名：		main.cpp
创建人：		Mako Wan
加注日期：	2015-10-26
描述：		二级谱定量的主函数
**************************************************/

#include "Head.h"

int main(int argc, char* argv[]){

	printLogo();				//打印logo；

	checkDate(2016,3,31);	    //检查软件是否过期；

	readCmdline(argc, argv);	//通过读取 [命令行] 或 [参数文件] 获取软件参数设置；

	readData();					//获取標記離子的信息；

	calcuReporter();			//计算比值、中位数和标准差；

	calcuPIF();				//计算PIF；

	proteinInfer();			//蛋白归并；

	outputResult();				//输出PSM层级的结果；

	return 0;
}