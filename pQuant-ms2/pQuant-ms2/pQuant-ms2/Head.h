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
#include <stdio.h>
#include <unordered_map>
#include <map>
// Attention:
// If you use visual studio cl.exe compiler, use this header file
#include "dirent.h"
// else if you use gcc compiler, use this header file
//#include <dirent.h>
#include "stringProcess.h"

using namespace std;

//自定义变量区

typedef struct parainfo{

	string binPath = ".\\";

	int quantMethod = 0;						//1-3代表iTRAQ4、iTRAQ8、TMT
	double detaFragment = 0.0;					//搜索窗口大小
	vector<double> reporterMZ;

	string input_spectra_path = "";				//pFind.spectra结果文件，作为输入文件；
	string pf_path = "";						//pf2文件路径；
	string pfidx_path = "";						//pf2idx文件路径;
	string pf1_path = "";						//pf1文件路径；
	string pf1idx_path = "";					//pf1idx文件路径；
	string output_ratio_path = "";				//定量比值输出路径；


}paraInfo;

//谱峰
typedef struct peakinfo{
	double mz;
	double iten;
}peakInfo;

//包含一个psm需要的全部信息
typedef struct psminfo{

	string	title			= "";
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
	
	int		pf2Pos			= 0;

	int		peakNums		= 0;
	vector<peakInfo> peaks;

	vector<double> reporter;

	//TODO: 利用继承派生出各种定量方法类

}psmInfo;

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
	//in this case, class B also have int a menber;
};

//参数区
const string TIMESTRING = "2015-10-26 16:00";


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

#endif