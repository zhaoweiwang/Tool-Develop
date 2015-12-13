#include "Head.h"

vector<string> m_cmdInfo;
const string Version = "-Release-1.0.0";

parainfo para;


void WhatsNew()
{
	cout << "What's New:" << endl;
	cout << "\t[2015.12.01] Upgrade: Up to iTRAQ4/8 && TMT are available. " << endl;
	cout << "\t[2015.12.08] Upgrade: Up to correct isotope impurities is available. " << endl;

}

void printVersion()
{
	cout << "\t         pQuant-ms2 Version" << Version << endl << endl;;
	WhatsNew();
}

void Usage()
{
	cout << endl;
	cout << "Usage via config files:" << endl;
	cout << "\tpQuant-ms2.exe pQuant-ms2.para" << endl;
}

void GeneratedParamTemplate()
{
	string paraFileName = "";
	time_t tTemp = time(nullptr);
	tm* timeLocal = localtime(&tTemp);

	int year = timeLocal->tm_year + 1900;
	int month = timeLocal->tm_mon + 1;
	int day = timeLocal->tm_mday;
	int hour = timeLocal->tm_hour;
	int min = timeLocal->tm_min;
	int second = timeLocal->tm_sec;

	paraFileName = "pQuant-ms2_" + to_string(year) + to_string(month) + to_string(day) + to_string(hour) + to_string(min) + to_string(second) + ".para";

	FILE* pfile;
	pfile = fopen(paraFileName.c_str(), "r");
	if (pfile != NULL)
	{
		fclose(pfile);
		return;
	}

	pfile = fopen(paraFileName.c_str(), "w");
	fprintf(pfile, "# pQuant-ms2.para params template\n");
	fprintf(pfile, "# For help: mail to wangzhaowei@ict.ac.cn \n");
	fprintf(pfile, "# Time: 2015.10.27\n\n");

	fprintf(pfile, "[Basic Options]\n");
	fprintf(pfile, "spectraDatapath = D:\\pFindWorkspace\\\n");
	fprintf(pfile, "pfDatapath = D:\\DataSet\\\n");
	fprintf(pfile, "quantResultDatapath = D:\\DataSet\\\n\n");

	fprintf(pfile, "[Advanced Options]\n");
	fprintf(pfile, "quantitativeMethod = 0\n");
	fprintf(pfile, "# 0, quantitative method: iTRAQ-4plex;\n");
	fprintf(pfile, "# 1, quantitative method: iTRAQ-8plex;\n");
	fprintf(pfile, "# 1, quantitative method: TMT-6plex.\n");
	fprintf(pfile, "reporterIonMZ = 114.110, 115.110, 116.110, 117.110\n");
	fprintf(pfile, "# you could customize the reporter ion M/Z.\n");
	fprintf(pfile, "# e.g. iTRAQ-4plex: 114.110, 115.110, 116.110, 117.110.\n");
	fprintf(pfile, "FTMS = 20ppm\n");
	fprintf(pfile, "# a fragment mass tolerance was set by yourself, which default is 20 ppm.\n");

	fclose(pfile);
}

void Attention()
{
	cout << endl << "Attention:" << endl;
	cout << "\tCarefully configured parameter file is required!" << endl;
	cout << "\tA standard parameter file named 'pQuant-ms2.para' is generated." << endl << endl;;
	GeneratedParamTemplate();
}

void printHelpInfo(){

	printVersion();
	Usage();
	Attention();

}

void DisplayCMDUsage()
{
	cout << "\t=================================================" << endl;
	cout << "\t===    Welcome to try pQuant-ms2 Switches!    ===" << endl;
	cout << "\t=================================================" << endl;
	cout << "\nUsage via command options:" << endl;
	cout << "\tpQuant-ms2.exe -D D:\\pFindWorkspace\\pFind.spectra -P D:\\DataSet\\data.pf2" << endl;
	cout << "Or" << endl;
	cout << "\tpQuant-ms2.exe -D D:\\pFindWorkspace\\ -P D:\\DataSet\\" << endl;
	cout << endl;
	cout << "More Options:" << endl;
	cout << "\t-D pFind.Spectra datapath   default (D:\\pFindWorkspace\\)" << endl;
	cout << "\t-P .pf file datapath        default (D:\\DataSet\\)" << endl;
	cout << "\t-O result file outputpath   default (the same with datapath)" << endl;
	cout << "\t-W fragment mass tolerance  default (20 ppm) " << endl;
	cout << "\t-M quantitative method      default (0) " << endl;
	cout << "\t-R reporter ion M/Z         default (114.110, 115.110, 116.110, 117.110)" << endl;
}

void InitializePara(){
	para.binPath = ".\\";
	para.input_spectra_path = "D:\\pFindWorkspace\\";		//pFind工作目录下的pFind.spectra路径
	para.pfidx_path = "D:\\DataSet\\";						//数据存放目录下pParse导出的pf2idx路径
	para.pf_path = "D:\\DataSet\\";							//数据存放目录下pParse导出的pf2路径
	para.pf1idx_path = "D:\\DataSet\\";						//数据存放目录下pParse导出的pf1idx路径
	para.pf1_path = "D:\\DataSet\\";						//数据存放目录下pParse导出的pf1路径
	para.output_ratio_path = "D:\\DataSet\\";				//计算结果的导出路径
	para.quantMethod = 0;									//定量方法的选择，default = 0, iTRAQ-4plex
	para.detaFragment = 20.0;								//Reporter Ion的窗口大小
}

void setBinPath(char* argv[]){								// The path of pParse.exe. Extract the path from argv[0]
	string binPath = argv[0];
	int found = binPath.find_last_of("/\\");
	if (found != string::npos)	para.binPath = binPath.substr(0, found + 1);
}

void Trim(string str){
	int found = str.find_last_not_of(' ');
	if (found != string::npos)
		str = str.substr(found);
	else
		throw "pQuant-ms2 found invalid key in config file!";
	found = str.find_first_not_of(' ');
	if (found != string::npos)
		str = str.substr(0, found + 1);
	else
		throw "pQuant-ms2 found invalid key in config file!";
}

void checkPara(){


}

void readPara(char* argv[]){								//打开m_cmdInfo[1] .para文件读参数值；
	
	InitializePara();										//初始化配置文件参数；

	ifstream fin;
	fin.open(argv[1], ios::in);
	if (!fin.good()){
		string inFo = "The File \"";
		inFo += argv[1];
		inFo += "\" dose not exist!";
		screenInfo pS;
		pS.printInfoTo(cout, "Error", inFo);
		throw inFo;
	}

	char strTemp[256];
	string str = "";
	map<string, string> param;									//先存到map里，再格式化存在para中；
	while (!fin.eof()){
		fin.getline(strTemp, 256);
		str = strTemp;
		if (str == "" || str[0] == '#' || str[0] == ';' || str[0] == '[')	continue;
		int pos = str.find('=');
		string forward;
		string backward;
		forward = str.substr(0, pos);
		backward = str.substr(pos + 2, str.size());
		Trim(forward);
		Trim(backward);
		param[forward] = backward;
	}

	para.input_spectra_path = param["spectraDatapath"];
	para.pfidx_path = param["pfDatapath"];
	para.pf_path = param[""];

	setBinPath(argv);
	checkPara();
	
	char* paraFileName = argv[1];
	

	para.quantMethod = 1;
	para.detaFragment = 200.0*0.000001;
	para.input_spectra_path = "H:\\3-database\\WIFF\\IPRG_2012\\mgf\\Task1\\result\\pFind.spectra";
	para.pf_path = "H:\\3-database\\WIFF\\IPRG_2012\\mgf\\iPRG_2012_HCDFT.pf2";
	para.pfidx_path = "H:\\3-database\\WIFF\\IPRG_2012\\mgf\\iPRG_2012_HCDFT.pf2idx";
	para.output_ratio_path = "H:\\3-database\\WIFF\\IPRG_2012\\mgf\\QuantRatio-ms2.pq2";

}

//void readCmd(){
//
//}

void readCmdline(const int argc, char* argv[]){

	int start = clock();		//计时器


	for (int arg_i = 0; arg_i < argc; arg_i++){
		m_cmdInfo.push_back(argv[arg_i]);
		m_cmdInfo.push_back(" ");
	}
	m_cmdInfo.push_back("\n");

	if (1 == argc){
		printHelpInfo();
		DisplayCMDUsage();
	}
	else if (argc == 2 && (stringProcess::bMatchingFix(argv[1], ".cfg", true, true) || stringProcess::bMatchingFix(argv[1], ".para", true, true))){
		readPara(argv);
	}
	else{
		//readCmd();
	}

	int seconds = (clock() - start) / CLOCKS_PER_SEC;
	cout << "== == == Time elapsed: " << seconds << " seconds. == == ==" << endl;
}
