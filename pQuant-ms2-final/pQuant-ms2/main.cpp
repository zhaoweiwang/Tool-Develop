﻿/**************************************************
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

	//calcuPIF();				//计算PIF；

	//proteinInfer();			//蛋白归并；

	outputResult();				//输出PSM层级的结果；

	//TODO: 1.针对pf的批处理而不是单文件，因为1.人家load了很多wiff，不止一个；2.单个wiff本身可以很多，pParse会切割；
	//TODO: 2.加入蛋白归并模块；

	return 0;
}