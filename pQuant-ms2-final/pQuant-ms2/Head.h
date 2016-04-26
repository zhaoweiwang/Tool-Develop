/**************************************************
文件名：		Head.h
创建人：		Mako Wan
加注日期：	2015-10-26
描述：		总头文件，放置所需东西
**************************************************/
#ifndef IHEAD_H
#define IHEAD_H


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>

#include <windows.h>

#include <iomanip>
#include <WinNT.h>
#include <tchar.h>
#include <float.h>
#include "sisocks.h"
#include "Rconnection.h"
// Attention:
// If you use visual studio cl.exe compiler, use this header file
#include "dirent.h"
// else if you use gcc compiler, use this header file
//#include <dirent.h>
#include "stringProcess.h"


using namespace std;

//自定义变量区

typedef struct modificationinfo{
	string name = "";
	double mass = 0.0;
	int index = 0;
}modificationInfo;

typedef struct pIDLplexinfo{
	string modN = "";
	double massN = 0.0;

	string modC = "";
	double massC = 0.0;
}pIDLplexInfo;

typedef struct parainfo{
	string binPath = ".\\";

	int quantMethod = 0;											//0-3代表iTRAQ4、iTRAQ8、TMT6和TMT10，4代表pIDL方法
	string  FTMSType = "";											//窗口类型
	double detaFragment = 0.0;								//搜索窗口大小
	vector<double> reporterMZ;							//iTRAQ、TMT报告离子MZ
	vector<pIDLplexInfo> pIDLplex;						//pIDL标记列表

	string input_spectra_path = "";							//pFind.spectra结果文件，作为输入文件
	string input_protein_path = "";
	string pf_path = "";												//pf2文件路径
	string pfidx_path = "";											//pf2idx文件路径
	string pf1_path = "";												//pf1文件路径
	string pf1idx_path = "";											//pf1idx文件路径
	string output_ratio_path = "";								//定量比值输出路径

	string fasta_path = "";											//fasta文件路径
	string modification_path = "";								//modification.ini文件路径

	double minRange = 0.0;										//pIDL定量的最大范围
	double maxRange = 0.0;										//pIDL定量的最小范围
	double PIF = 0.75;													//PIF值
	double PsmFDR = 0.01;										//PSM层次卡的FDR值
	double ProteinFDR = 0.01;									//Protein层次卡的FDR值

	bool correct = true;												//是否使用校正矩阵
	bool runVSN = true;												//是否使用VSN算法

}paraInfo;

//谱峰
typedef struct peakinfo{
	double mz;
	double iten;
}peakInfo;

//pIDL方法中BY离子对（强度和）
typedef struct BYIteninfo{
	double fz = 0.0;
	double fm = 0.0;
}BYItenInfo;

//包含一个psm需要的全部信息
typedef struct psminfo{
	string	title			= "";

	string pf1idx			= "";
	string pf1				= "";
	string pf2idx			= "";
	string pf2				= "";

	int scan					= 0;
	double mass1		= 0.0;
	int charge				= 0;
	double fdr				= 0.0;
	string pepSq			= "";
	double mass2		= 0.0;
	double massGapDa		= 0.0;
	double massGapPpm	= 0.0;
	double score					= 0.0;
	string modification		= "";
	string proAc					= "";
	string prosandCons		= "";

	vector<modificationInfo> mod;
	vector<peakInfo> peaks;

	//Reporter区
	int pf2Pos = 0;
	int peakNums = 0;
	vector<double> reporter;
	vector<double> reporterCorrect;
	vector<double> ratioReporter;
	vector<double> ratioReporterCorrect;

	//PIF区
	int scanPre = 0;
	int pf1Pos = 0;
	double PIF = 0.0;
	int precuNums = 0;
	vector<peakInfo> precus;

	//pIDL区
	vector<BYItenInfo> a1PairInten;				//a1+离子强度对
	vector<BYItenInfo> allIPairInten;			//all离子强度对
	vector<double> a1RatioVSN;					//a1+离子比值，VSN处理后的
	vector<double> allRatioVSN;					//all离子比值，VSN处理后的
	vector<double> a1Ratio;							//a1+离子比值，正常计算下得到的
	vector<double> allRatio;							//all离子比值，正常计算下得到的
	set<string> annotation;								//标记匹配到的离子类型，n重中有一重匹配到则算

	//TODO: 利用继承派生出各种定量方法类
}psmInfo;

typedef struct proteininfo{
	string ac = "";
	vector<int> index;
	string de = "";
	double PIF = 0.0;
	vector<double> ratio;
	string tag = "";
	string led = "";
	set<string> peptide;

	int psmNum = 0;
	int pepNum = 0;
	int uniqueNum = 0;
}proteinInfo;

typedef struct rela{
	string str = "";
	vector<string> pro;
}Rela;

class screenInfo{
public:
	void printInfoTo(ostream& output, string infoType, string inFo);
	string getTimeStr();
};

class A{
public:
	int a = 1;
	void showInfo(){
		cout << "This is " << this->a << endl;
	}
};

class B : public A{
public:
	int b = 2;
	void showInfo(){
		cout << "This is " << this->b << endl;
	}
};

//参数区
const string TIMESTRING = "2016-02-18 16:00";
const string runningInfo = "<RunningInformation>";

//函数声明区

/*
函数名：	printLogo
功能：	打印pQuant-ms2`s Logo
输入：	无
返回：	无
*/
/*
根据pFind Studio系列软件的传统，打印pQuant-MS2.exe的名称、版本号和pFind官网地址等信息
该模块不需要经常改动，仅在软件升级、改名等时候需要重新设置打印的LOGO
*/
void printLogo();


/*
函数名：	checkDate
功能：	检查软件是否过期；
输入：	无
返回：	无
*/
/*
pFind Studio系列软件并非免费、开源，pIsobariQ.exe同样也有使用期限
该模块一个月需要更新一次，即提供用户使用的期限为1个月，这是pFind系列软件的正常使用期限
*/
void checkDate(int year, int month, int day);

/*
函数名：	readCmdline
功能：	读取cmd命令行；
输入：	年月日
返回：	无
*/
/*
该函数主要用于获取用户传递的参数，主要有3中情况
第1种：参数只有1个，即对应用户双击了pIsobariQ.exe
第2种：参数2个，进行检查，运行参数文件调用模式
第3种：参数2个以上，进行检查并从命令行运行，目前该模式待定义（pIsobariQ参数较多，改模式的存在不合理，可以废除掉）
*/
void readCmdline(const int argc, char* argv[]);

/*
函数名：	readData
功能：	读取pFind.spectra获取PSM，读取pf2.idx，读取pf2获取谱峰信息；
输入：	无
返回：	无
*/
void readData();
/*
该模块主要支持pFind 3的搜索结果，通过读取pFind.spectra文件获取PSMs
通过读取pParse.exe导出的pf2idx、pf2文件提取谱峰
然后根据具体的定量方法提取谱峰，赋给对应的PSM
如果pIsobariQ.exe需要扩展支持其它引擎的定量结果，仅需扩展对应引擎分支即可，该分支属于pFind 3
*/

/*
函数名：	calcuReporter
功能：	计算reporter ions比值、中位数和标准差；
输入：	无
返回：	无
*/
void calcuReporter();

/*
函数名：	calcuPIF
功能：	计算PIF值；
输入：	无
返回：	无
*/
void calcuPIF();

/*
函数名：	proteinInfer
功能：	蛋白归并；
输入：	无
返回：	无
*/
void proteinInfer();

/*
函数名：	outputResult
功能：	输出带有定量信息的定性结果，fdr合格的PSMs；
输入：	无
返回：	无
*/
void outputResult();

/*
函数名：	readModification
功能：	读取modification.ini文件，获取修饰-质量的哈希表；
输入：	无
返回：	无
*/
void readModification();

/*
函数名：	calVSN
功能：	调用R包中的VSN
输入：	无
返回：	无
*/
void calVSN();


#endif