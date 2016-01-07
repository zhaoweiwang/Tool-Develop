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
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unordered_map>
#include <map>
#include <set>
#include <algorithm>
#include <windows.h>
// Attention:
// If you use visual studio cl.exe compiler, use this header file
#include "dirent.h"
// else if you use gcc compiler, use this header file
//#include <dirent.h>
#include "stringProcess.h"

using namespace std;

//�Զ��������

typedef struct parainfo{

	string binPath = ".\\";

	int quantMethod = 0;						//1-3����iTRAQ4��iTRAQ8��TMT
	double detaFragment = 0.0;					//�������ڴ�С
	vector<double> reporterMZ;

	string input_spectra_path = "";				//pFind.spectra����ļ�����Ϊ�����ļ���
	string input_protein_path = "";
	string pf_path = "";						//pf2�ļ�·����
	string pfidx_path = "";						//pf2idx�ļ�·��;
	string pf1_path = "";						//pf1�ļ�·����
	string pf1idx_path = "";					//pf1idx�ļ�·����
	string output_ratio_path = "";				//������ֵ���·����

	string fasta_path = "";				//fasta�ļ�·����

}paraInfo;

//�׷�
typedef struct peakinfo{
	double mz;
	double iten;
}peakInfo;

//����һ��psm��Ҫ��ȫ����Ϣ
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

	vector<peakInfo> peaks;

	//Reporter��
	int pf2Pos = 0;
	int peakNums = 0;
	vector<double> reporter;
	vector<double> reporterCorrect;
	vector<double> ratioReporter;
	vector<double> ratioReporterCorrect;

	//PIF��
	int scanPre = 0;
	int pf1Pos = 0;
	double PIF = 0.0;
	int precuNums = 0;
	vector<peakInfo> precus;
	//vector<peakInfo> peaksPrecusor;

	//TODO: ���ü̳����������ֶ���������

}psmInfo;

typedef struct proteininfo{

	string ac = "";
	vector<int> index;
	string de = "";
	double PIF = 0.0;
	vector<double> ratio;
	//vector<double> inten;
	string tag = "";
	string led = "";
	set<string> peptide;

	int psmNum = 0;
	int pepNum = 0;

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
	//in this case, class B also have int a menber;
};

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
void checkDate(int year, int month, int day);

/*
��������	readCmdline
���ܣ�	��ȡcmd�����У�
���룺	������
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


/*
��������	calcuReporter
���ܣ�	����reporter ions��ֵ����λ���ͱ�׼�
���룺	��
���أ�	��
*/
void calcuReporter();

/*
��������	calcuPIF
���ܣ�	����PIFֵ��
���룺	��
���أ�	��
*/
void calcuPIF();

/*
��������	proteinInfer
���ܣ�	���׹鲢��
���룺	��
���أ�	��
*/
void proteinInfer();

/*
��������	outputResult
���ܣ�	������ж�����Ϣ�Ķ��Խ����fdr�ϸ��PSMs��
���룺	��
���أ�	��
*/
void outputResult();

#endif