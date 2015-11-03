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
#include "stringProcess.h"

using namespace std;

//自定义变量区

typedef struct parainfo{

	int quantMethod;				//1-3代表iTRAQ4、iTRAQ8、TMT
	string input_spectra_path = "";	//pFind.spectra结果文件，作为输入文件；
	string pf_path = "";			//pf2、pfidx文件路径；
	string output_ratio_path = "";	//定量比值输出路径；

}paraInfo;

//包含一个psm需要的全部信息
typedef struct psminfo{
	int		scan = 0;
	double	score = 0.0;
	double	spectra_mh = 0.0;
	double	pep_mh = 0.0;
	string	sq = "";
	int		charge = 0;
	double	mz = 0;
	double	fdr = 0;
	string	title = "";
	string	prosandCons = "";//正库or反库
	vector<string> modify;
	int cleaves = 0;

}psmInfo;


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
void checkDate();

/*
函数名：	readCmdline
功能：	读取cmd命令行；
输入：	无
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