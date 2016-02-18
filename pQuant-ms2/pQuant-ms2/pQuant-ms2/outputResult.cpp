#include "Head.h"

extern parainfo para;
extern vector<psmInfo> psmVec;					//PSM������

extern vector<proteinInfo> proteinVec;

void outputPsms(){
	ofstream out(para.output_ratio_path.c_str());
	if (!out.is_open()){
		cout << "Error opening file" << endl; exit(1);
	}
	else{
		cout << "Successfully open the file of " << para.output_ratio_path << "." << endl;
	}

	out << "Title\tScan\tScore\tMass1\tMass2\tCharge\tFDR\tPeptideSquence\tModification\tProteinAc\t";
	for (int i = 0; i < psmVec[0].reporter.size(); i++) out << "RepoInt" + to_string(i) + "\t";
	for (int i = 0; i < psmVec[0].reporterCorrect.size(); i++) out << "RepoIntCorrect" + to_string(i) + "\t";
	out << "PIF\t";
	for (int i = 0; i < psmVec[0].ratioReporter.size(); i++) out << "Ratio" + to_string(i) + "\t";
	for (int i = 0; i < psmVec[0].ratioReporterCorrect.size(); i++) out << "RatioCorrect" + to_string(i) + "\t";
	out << endl;

	for (int i = 0; i < psmVec.size(); i++){
		out << psmVec[i].title << "\t" << psmVec[i].scan << "\t" << psmVec[i].score << "\t" << psmVec[i].mass1 << "\t"
			<< psmVec[i].mass2 << "\t" << psmVec[i].charge << "\t" << psmVec[i].fdr << "\t" << psmVec[i].pepSq << "\t"
			<< psmVec[i].modification << "\t" << psmVec[i].proAc << "\t";
		for (int j = 0; j < psmVec[i].reporter.size(); j++) out << psmVec[i].reporter[j] << "\t";
		for (int j = 0; j < psmVec[i].reporterCorrect.size(); j++) out << psmVec[i].reporterCorrect[j] << "\t";
		out << psmVec[i].PIF << "\t";
		for (int j = 0; j < psmVec[i].ratioReporter.size(); j++) out << psmVec[i].ratioReporter[j] << "\t";
		for (int j = 0; j < psmVec[i].ratioReporterCorrect.size(); j++) out << psmVec[i].ratioReporterCorrect[j] << "\t";
		out << endl;
	}

	out.close();
}

void outputProtein(){
	string output_protein_path = "";
	output_protein_path = para.output_ratio_path.substr(0, para.output_ratio_path.find_last_of('.')) + ".protein";
	//cout << output_protein_path << endl;
	//getchar();

	ofstream out(output_protein_path.c_str());
	if (!out.is_open()){
		cout << "\nError opening file" << endl; exit(1);
	}
	else{
		cout << "Successfully open the file of " << output_protein_path << "." << endl;
	}

	out << "Ac\tDe\tPsmNum\tPepNum\tTag\tLeadinPro\t";
	for (int i = 0; i < psmVec[0].ratioReporter.size(); i++) out << "Ratio" + to_string(i) + "\t";
	out << "PIF" << endl;

	for (int i = 0; i < proteinVec.size(); i++){
		out << proteinVec[i].ac << "\t" << proteinVec[i].de << "\t" << proteinVec[i].psmNum << "\t" << proteinVec[i].pepNum
			<< "\t" << proteinVec[i].tag << "\t" << proteinVec[i].led << "\t";
		for (int j = 0; j < proteinVec[i].ratio.size(); j++) out << proteinVec[i].ratio[j] << "\t";
		out << proteinVec[i].PIF << endl;
	}

	out.close();
}

void outputResult(){
	cout << "\nStep9: Output .spectra and .protein file." << endl;

	outputPsms();
	outputProtein();

	cout << "\nThat`s all, you can see What you want in the pQuant-ms2_Result.spectra." << endl;
	cout << "And you can see more information in the pQuant-ms2_Result.protein." << endl;
	
}