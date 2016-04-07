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

	double PIF = 0.75;													//PIF值
	double PsmFDR = 0.01;										//PSM层次卡的FDR值
	double ProteinFDR = 0.01;									//Protein层次卡的FDR值

	bool correct = true;												//是否使用校正矩阵
}paraInfo;

//谱峰
typedef struct peakinfo{
	double mz;
	double iten;
}peakInfo;

//包含一个psm需要的全部信息
typedef struct psminfo{
	string	title			= "";

	string pf1idx			= "";
	string pf1				= "";
	string pf2idx			= "";
	string pf2				= "";

	int		scan			= 0;
	double	mass1			= 0.0;
	int		charge			= 0;
	double	fdr				= 0.0;
	string	pepSq			= "";
	double	mass2			= 0.0;
	double  massGapDa		= 0.0;
	double  massGapPpm		= 0.0;
	double	score			= 0.0;
	string  modification	= "";
	string	proAc			= "";
	string	prosandCons		= "";

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

	vector<double> a1Iten;			//a1+离子强度
	vector<double> allIten;			//b、y离子强度和
	vector<double> a1Ratio;			//a1+离子比值
	vector<double> allRatio;			//b、y离子比值
	vector<string> annotation;		//标记匹配到的离子类型

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


//函数声明区

/*
函数名：	printLogo
功能：	打印pQuant-ms2`s Logo
输入：	无
返回：	无
*/
void printLogo();

/*
函数名：	checkDate
功能：	检查软件是否过期；
输入：	无
返回：	无
*/
void checkDate(int year, int month, int day);

/*
函数名：	readCmdline
功能：	读取cmd命令行；
输入：	年月日
返回：	无
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