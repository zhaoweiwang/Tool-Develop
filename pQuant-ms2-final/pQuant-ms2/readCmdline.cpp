#include "Head.h"

vector<string> m_cmdInfo;
const string Version = "-Release-1.0.1";

parainfo para;

const bool DO_NOT_DISPLAY = false;


void WhatsNew(){
	cout << "What's New:" << endl;
	cout << "\t[2015.12.01] Upgrade: Up to iTRAQ4/8/10 && TMT2/6 are available." << endl;
	cout << "\t[2015.12.08] Upgrade: Up to correct isotope impurities is available." << endl;
	cout << "\t[2016.02.01] Upgrade: Up to batch processing function." << endl;
	cout << "\t[2016.03.28] Upgrade: Up to pIDL are available." << endl;
}

void printVersion(){
	cout << "\t         pQuant-ms2 Version" << Version << endl << endl;;
	WhatsNew();			//版本更新修改、添加了哪些功能
}

void Usage(){
	cout << endl;
	cout << "Usage via config files:" << endl;
	cout << "\tpQuant-ms2.exe pQuant-ms2.para" << endl;
}

void GeneratedParamTemplate(){
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
	if (pfile != NULL){
		fclose(pfile);
		return;
	}

	pfile = fopen(paraFileName.c_str(), "w");
	fprintf(pfile, "# pQuant-ms2.para params template\n");
	fprintf(pfile, "# For help: mail to pFind@ict.ac.cn \n");
	fprintf(pfile, "# Time: 2015.10.27\n\n");

	fprintf(pfile, "[Basic Options]\n");
	fprintf(pfile, "resultDatapath = D:\\pFindWorkspace\\\n");
	fprintf(pfile, "pfDatapath = D:\\DataSet\\\n");
	fprintf(pfile, "# you need to add the path of pf files which output by pParse.\n");
	fprintf(pfile, "quantResultDatapath = D:\\DataSet\\\n\n");

	fprintf(pfile, "[Database Options]\n");
	fprintf(pfile, "fastaDatapath = D:\\DataSet\\\n");
	fprintf(pfile, "modificationDatapath = D:\Software\pFind3\pFindStudio\bin\modification.ini\n\n");

	fprintf(pfile, "[Method Options]\n");
	fprintf(pfile, "quantitativeMethod = 0\n");
	fprintf(pfile, "# 0, quantitative method: iTRAQ-4plex;\n");
	fprintf(pfile, "# 1, quantitative method: iTRAQ-8plex;\n");
	fprintf(pfile, "# 2, quantitative method: TMT-6plex.\n");
	fprintf(pfile, "# 3, quantitative method: TMT-10plex.\n");
	fprintf(pfile, "# 4, quantitative method: pIDL.\n");
	fprintf(pfile, "# According to your choice to set the following parameter.\n");
	fprintf(pfile, "reporterIonMZ = 114.110, 115.110, 116.110, 117.110\n");
	fprintf(pfile, "# you could customize the reporter ion M/Z.\n");
	fprintf(pfile, "# e.g. iTRAQ-4plex: 114.110, 115.110, 116.110, 117.110.\n");
	fprintf(pfile, "pIDLplex = Dim34NL 34.063119, Dim34KL 34.063119; Dim34NH 34.068963, Dim34KH 34.068963\n");
	fprintf(pfile, "# If you use pIDL method, customize the Modification-Mass of N-item and C-item.\n");
	fprintf(pfile, "# e.g. pIDL-2: Dim34NL 34.063119, Dim34KL 34.063119; Dim34NH 34.068963, Dim34KH 34.068963;\n\n");

	fprintf(pfile, "[Advanced Options]\n");
	fprintf(pfile, "FTMS = 200ppm\n");
	fprintf(pfile, "# a fragment mass tolerance was set by yourself, which default is 20 ppm.\n");
	fprintf(pfile, "PIF = 0.75\n");
	fprintf(pfile, "# a option to filter the PSMS and Protein for the interference of co-elution precursor ions.\n");
	fprintf(pfile, "PsmFDR = 0.01\n");
	fprintf(pfile, "# FDR of PSMs result you need to set, which default is 0.01.\n");
	fprintf(pfile, "ProteinFDR = 0.01\n");
	fprintf(pfile, "# FDR of protein you need to set, which default is 0.01.\n");
	fprintf(pfile, "Correct = 1\n");
	fprintf(pfile, "# Did you want to use CorrectMatrix to correct the itensity of plex ion, which default is true.\n");
	fclose(pfile);
}

void Attention(){
	cout << endl << "Attention:" << endl;
	cout << "\tCarefully configured parameter file is required!" << endl;
	cout << "\tA standard parameter file named 'pQuant-ms2.para' is generated." << endl << endl;;
	GeneratedParamTemplate();
}

void printHelpInfo(){
	printVersion();			//打印版本号，以及版本更新区别
	Usage();				//配置文件使用打印
	Attention();			//提示配置文件配置，以及生成默认配置文件
}

void DisplayCMDUsage(){
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
	cout << "\t-FDR FDR                    default (0.01)" << endl;
	cout << "\t-PIF PIF                    default (0.75)" << endl;
}

void InitializePara(){
	para.binPath = ".\\";

	para.input_spectra_path = "D:\\pFindWorkspace\\pFind.spectra";			//pFind工作目录下的pFind.spectra路径
	para.input_protein_path = "D:\\pFindWorkspace\\pFind.protein";			//pFind工作目录下的pFind.protein路径

	para.pfidx_path = "D:\\DataSet\\";										//数据存放目录下pParse导出的pf2idx路径
	para.pf_path = "D:\\DataSet\\";											//数据存放目录下pParse导出的pf2路径
	para.pf1idx_path = "D:\\DataSet\\";										//数据存放目录下pParse导出的pf1idx路径
	para.pf1_path = "D:\\DataSet\\";										//数据存放目录下pParse导出的pf1路径

	para.output_ratio_path = "D:\\DataSet\\";								//计算结果的导出路径

	para.fasta_path = "D:\\DataSet\\";										//fasta数据库路径
	para.modification_path = "D:\\pFind\\bin\\modification.ini";			//fasta数据库路径

	para.quantMethod = 0;													//定量方法的选择，default = 0, iTRAQ-4plex
	para.detaFragment = 200.0;												//Reporter Ion的窗口大小
	para.reporterMZ = { 114.111, 115.111, 116.111, 117.111 };				//存放reporter Ion对应的的MZ，默认是iTRAQ4

	para.PIF = 0.75;														//PIF，默认卡0.75
	para.PsmFDR = 0.01;														//PSM层次的FDR，卡0.01
	para.ProteinFDR = 0.01;													//Protein层次的FDR，卡0.01

	para.correct = true;													//强度校正矩阵，默认为true
}

void setBinPath(char* argv[]){								// The path of pParse.exe. Extract the path from argv[0]
	string binPath = argv[0];
	int found = binPath.find_last_of("/\\");
	if (found != string::npos)	
		para.binPath = binPath.substr(0, found + 1);
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

bool isPath(string strpath){
	DIR* p = opendir(strpath.c_str());
	if (p == NULL)	return false;
	else{
		closedir(p);
		return true;
	}
}

void displayPara(){
	if (DO_NOT_DISPLAY)	return;

	int paraCount = 1;

	cout << "\n\t       --------- BEGIN PARAMETERS ---------" << endl << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "resultDatapath" << " = " << para.input_spectra_path << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "pfDatapath" << " = " << para.pf_path << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "fastaDatapath" << " = " << para.fasta_path << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "modificationDatapath" << " = " << para.modification_path << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "quantResultDatapath" << " = " << para.output_ratio_path << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "quantitativeMethod" << " = " << para.quantMethod << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "FTMS" << " = " << para.detaFragment << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "PIF" << " = " << para.PIF << endl;
	cout << "[" << paraCount++ << "]" << ":  " << "PsmFDR" << " = " << para.PsmFDR << endl;
	cout << "[" << paraCount++ << "]" << ": " << "ProteinFDR" << " = " << para.ProteinFDR << endl;
	cout << "[" << paraCount++ << "]" << ": " << "Correct" << " = " << para.correct << endl;

	cout << "[" << paraCount++ << "]" << ": " << "reporterIonMZ" << " =";
	for (int i = 0; i < para.reporterMZ.size(); ++i) 
		cout << " " << para.reporterMZ[i];
	cout << endl;

	cout << "[" << paraCount++ << "]" << ": " << "pIDLplex" << " =";
	for (int i = 0; i < para.pIDLplex.size(); ++i){
		cout << " " << para.pIDLplex[i].modN << " " << para.pIDLplex[i].massN << ", "
			<< para.pIDLplex[i].modC << " " << para.pIDLplex[i].massC << ";";
	}
	cout << endl << endl;

	cout << "\t       --------- END PARAMETERS -----------" << endl << endl;
}

//void checkPara(){											//读取参数后的判断步骤；
//	if (isPath(para.input_spectra_path)){
//		if (para.input_spectra_path[para.input_spectra_path.size() - 1] != '\\')
//			para.input_spectra_path += "\\";
//	}
//	else{
//		
//	}
//}

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
	}else{
		cout << "The File " << argv[1] << " opened successfully." << endl;
	}

	char strTemp[256];
	string str = "";
	map<string, string> param;									//先存到map里，再格式化存在para中；
	while (!fin.eof()){
		fin.getline(strTemp, 256);
		str = strTemp;

		if (str == "" || str[0] == '#' || str[0] == ';' || str[0] == '[')
			continue;

		int pos = str.find('=');
		string forward;
		string backward;
		forward = str.substr(0, pos-1);
		backward = str.substr(pos + 2, str.size());
		Trim(forward);
		Trim(backward);

		param[forward] = backward;
	}

	para.input_spectra_path = param["resultDatapath"];
	para.input_protein_path = param["resultDatapath"].substr(0, param["resultDatapath"].find_last_of('.')) + ".protein";

	para.pf_path = param["pfDatapath"];
	para.fasta_path = param["fastaDatapath"];
	para.modification_path = param["modificationDatapath"];
	para.output_ratio_path = param["quantResultDatapath"];

	para.quantMethod = atoi(param["quantitativeMethod"].c_str());
	para.detaFragment = atof(param["FTMS"].c_str());
	para.PIF = atof(param["PIF"].c_str());
	para.PsmFDR = atof(param["PsmFDR"].c_str());
	para.ProteinFDR = atof(param["ProteinFDR"].c_str());
	para.correct = atoi(param["Correct"].c_str());

	vector<double> mZ;
	while (param["reporterIonMZ"].find(',') != string::npos){		//动态提取reporterIonMZ内容
		string temp = "";
		temp = param["reporterIonMZ"].substr(0, param["reporterIonMZ"].find(','));
		mZ.push_back(atof(temp.c_str()));
		param["reporterIonMZ"] = param["reporterIonMZ"].substr(param["reporterIonMZ"].find(',') + 2, param["reporterIonMZ"].size());
	}
	mZ.push_back(atof(param["reporterIonMZ"].c_str()));
	para.reporterMZ.swap(mZ);

	pIDLplexInfo tempIDL;
	while (param["pIDLplex"].find(';') != string::npos){
		string temp = "";
		temp = param["pIDLplex"].substr(0, param["pIDLplex"].find(';'));
		tempIDL.modN = temp.substr(0, temp.find(' '));
		tempIDL.massN = atof(temp.substr(temp.find(' '), temp.find(',') - temp.find(' ')).c_str());
		tempIDL.modC = temp.substr(temp.find(',')+2, temp.find_last_of(' ') - temp.find(',')-2);
		tempIDL.massC = atof(temp.substr(temp.find_last_of(' '), temp.size()- temp.find_last_of(' ')).c_str());

		para.pIDLplex.push_back(tempIDL);
		param["pIDLplex"] = param["pIDLplex"].substr(param["pIDLplex"].find(';') + 2, param["pIDLplex"].size());
	}
	string temp = "";
	temp = param["pIDLplex"].substr(0, param["pIDLplex"].find(';'));
	tempIDL.modN = temp.substr(0, temp.find(' '));
	tempIDL.massN = atof(temp.substr(temp.find(' '), temp.find(',') - temp.find(' ')).c_str());
	tempIDL.modC = temp.substr(temp.find(',') + 2, temp.find_last_of(' ') - temp.find(',') - 2);
	tempIDL.massC = atof(temp.substr(temp.find_last_of(' '), temp.size() - temp.find_last_of(' ')).c_str());
	para.pIDLplex.push_back(tempIDL);
	

	setBinPath(argv);
	//checkPara();
	displayPara();
}


void readCmdline(const int argc, char* argv[]){

	int start = clock();		//计时器

	for (int arg_i = 0; arg_i < argc; arg_i++){
		m_cmdInfo.push_back(argv[arg_i]);
		m_cmdInfo.push_back(" ");
	}
	m_cmdInfo.push_back("\n");

	if (1 == argc){
		printHelpInfo();
		//DisplayCMDUsage();
	}else if (argc == 2 && (stringProcess::bMatchingFix(argv[1], ".cfg", true, true) || stringProcess::bMatchingFix(argv[1], ".para", true, true))){
		readPara(argv);
	}else{
		//readCmd();
		cout << "Todo: 准备加入从命令行读取参数的运行方式" << endl;
		getchar();
	}

	int seconds = (clock() - start) / CLOCKS_PER_SEC;
	cout << "\t    == == == Time elapsed: " << seconds << " seconds. == == ==" << endl;
}
