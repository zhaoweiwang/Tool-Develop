#include "Head.h"

vector<string> m_cmdInfo;
const string Version = "-Release-1.0.0";

void WhatsNew()
{
	cout << "What's New:" << endl;
	cout << "\t[2013.12.28] Upgrade: Up to iTRAQ4/8 && TMT are available. " << endl;

}

void printVersion()
{
	cout << "pQuant-ms2 Version" << Version << endl << endl;;
	WhatsNew();
}

void Usage()
{
	cout << endl;
	cout << "Usage via config files:" << endl;
	cout << "\tpQuant-ms2.exe pQuant-ms2.para" << endl << endl;;
}

void GeneratedParamTemplate()
{
	FILE *pfile;
	pfile = fopen("pQuant-ms2.para", "r");
	if (pfile != NULL)
	{
		cerr << "pQuant-ms.para already exists." << endl;
		fclose(pfile);
		return;
	}

	pfile = fopen("pParse.para", "w");
	fprintf(pfile, "# pQuant-ms2.para template\n");
	fprintf(pfile, "# For help: mail to wangzhaowei@ict.ac.cn \n");
	fprintf(pfile, "# Time: 2015.10.27\n\n");

	fprintf(pfile, "[Basic Options]\n");
	fprintf(pfile, "datapath=E:\\Data\\\n\n");

	fprintf(pfile, "[Advanced Options]\n");
	fprintf(pfile, "co-elute=1\n");
	fprintf(pfile, "# 0, output single precursor for single scan;\n");
	fprintf(pfile, "# 1, output all co-eluted precursors.\n");
	fprintf(pfile, "input_format=raw\n");
	fprintf(pfile, "# raw / ms1\n");
	fprintf(pfile, "isolation_width=2\n");
	fprintf(pfile, "# 2 / 2.5 / 3 / 4\n");

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
	cout << "\n========== Welcome to try pQuant-ms2 Switches! ===========" << endl;
	cout << "Usage via command options:" << endl << endl;
	cout << "\tpQuant-ms2.exe -D D:\\Dataset\\1.raw" << endl << endl;
	cout << "Or" << endl << endl;
	cout << "\tpParse.exe -D D:\\Dataset\\" << endl << endl;
	cout << endl;
	cout << "More Options:" << endl;
	cout << "\t-D datapath               default (D:\\data\\)" << endl;
	cout << "\t-L logfilepath            default (the same with datapath)" << endl;
	cout << "\t-O outputpath             default (the same with datapath)" << endl;
	cout << "\t-W isolation_width        default (2) " << endl;
	cout << "\t-F input_format           default (raw) " << endl;
	cout << "\t-C co-elute               default (1)" << endl;

}

void readCmdline(const int argc, char* argv[]){

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
	
		RunFromFile(argv, *plog);
	
	}



}
