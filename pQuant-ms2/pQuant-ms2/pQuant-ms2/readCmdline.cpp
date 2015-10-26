#include "Head.h"

vector<string> m_cmdInfo;

void readCmdline(const int argc, char* argv[]){

	for (int arg_i = 0; arg_i < argc; arg_i++){
		m_cmdInfo.push_back(argv[arg_i]);
		m_cmdInfo.push_back(" ");
	}
	m_cmdInfo.push_back("\n");

	for (auto i = 0; i < m_cmdInfo.size(); i++)
		cout << m_cmdInfo[i];
}
