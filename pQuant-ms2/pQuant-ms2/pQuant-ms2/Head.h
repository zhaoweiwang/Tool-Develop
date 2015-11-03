/**************************************************
�ļ�����		Head.h
�����ˣ�		Mako Wan
��ע���ڣ�	2015-10-26
������		��ͷ�ļ����������趫��
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

//�Զ��������

typedef struct parainfo{

	int quantMethod;				//1-3����iTRAQ4��iTRAQ8��TMT
	string input_spectra_path = "";	//pFind.spectra����ļ�����Ϊ�����ļ���
	string pf_path = "";			//pf2��pfidx�ļ�·����
	string output_ratio_path = "";	//������ֵ���·����

}paraInfo;

//����һ��psm��Ҫ��ȫ����Ϣ
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
	string	prosandCons = "";//����or����
	vector<string> modify;
	int cleaves = 0;

}psmInfo;


//������
const string TIMESTRING = "2015-10-26 16:00";


//����������

/*
��������	printLogo
���ܣ�	��ӡpQuant-ms2`s Logo
���룺	��
���أ�	��
*/
void printLogo();

/*
��������	checkDate
���ܣ�	�������Ƿ���ڣ�
���룺	��
���أ�	��
*/
void checkDate();

/*
��������	readCmdline
���ܣ�	��ȡcmd�����У�
���룺	��
���أ�	��
*/
void readCmdline(const int argc, char* argv[]);

/*
��������	readData
���ܣ�	��ȡpFind.spectra��ȡPSM����ȡpf2.idx����ȡpf2��ȡ�׷���Ϣ��
���룺	��
���أ�	��
*/
void readData();

#endif