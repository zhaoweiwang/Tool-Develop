/**************************************************
文件名：		Head.h
创建人：		Mako Wan
加注日期：	2015-10-26
描述：		总头文件，放置所需东西
**************************************************/
#ifndef IHEAD_H
#define IHEAD_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdio.h>

using namespace std;

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

#endif