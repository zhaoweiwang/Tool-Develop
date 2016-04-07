//以下存在是因为使用Rserve和R进行线程通信而必须存在的组件
#pragma comment(lib,"Ws2_32.lib") 
#define MAIN							// we are the main program, we need to define this
#define SOCK_ERRORS			// we will use verbose socket errors

/**************************************************
文件名：		main.cpp
创建人：		Mako Wan
加注日期：	2015-10-26
描述：			二级谱定量的主函数
**************************************************/

#include "Head.h"

/*
二级定量软件pQuant-MS2的主程序，通过该入口，调用各个功能模块进行计算
主要支持iTRAQ、TMT和pIDL等二级定量方法，同时也支持输出PSM、Protein层次的定量结果
过程中支持PIF计算，VSN算法等功能
*/
int main(int argc, char* argv[]){

	int start = clock();							//主进程计时器

	printLogo();										//打印logo
	/*
	根据pFind Studio系列软件的传统，打印pQuant-MS2.exe的名称、版本号和pFind官网地址等信息
	该模块不需要经常改动，仅在软件升级、改名等时候需要重新设置打印的LOGO
	*/

	checkDate(2016,3,31);						//检查软件是否过期
	/*
	pFind Studio系列软件并非免费、开源，pIsobariQ.exe同样也有使用期限
	该模块一个月需要更新一次，即提供用户使用的期限为1个月，这是pFind系列软件的正常使用期限
	*/

	readCmdline(argc, argv);					//通过读取 [命令行] 或 [参数文件] 获取软件参数设置
	/*
	该函数主要用于获取用户传递的参数，主要有3中情况
	第1种：参数只有1个，即对应用户双击了pIsobariQ.exe
	第2种：参数2个，进行检查、解析，运行参数文件调用模式
	第3种：参数2个以上，进行检查并从命令行运行，目前该模式待定义（pIsobariQ参数较多，改模式的存在不合理，可以废除掉）
	*/


	readData();									//获取標記離子的信息；

	calVSN();										//调用R中的VSN进行强度处理

	calcuReporter();							//计算比值、中位数和标准差；

	calcuPIF();									//计算PIF；

	proteinInfer();								//蛋白归并；

	outputResult();							//输出PSM层级的结果；

	int seconds = (clock() - start) / CLOCKS_PER_SEC;
	cout << "\t    == == == Time elapsed: " << seconds << " seconds. == == ==" << endl;

	return 0;
}