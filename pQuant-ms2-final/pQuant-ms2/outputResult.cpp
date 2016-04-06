#include "Head.h"

extern parainfo para;
extern vector<psmInfo> psmVec;					//PSM»º³åÇø

extern vector<proteinInfo> proteinVec;

void outputPsms(){
	ofstream out(para.output_ratio_path.c_str());
	if (!out.is_open()){
		cout << "Error opening file" << endl; 
		exit(1);
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
	for (int i = 0; i < psmVec[0].a1Ratio.size(); ++i) out << "a1_ratio" + to_string(i) + "\t";
	for (int i = 0; i < psmVec[0].allRatio.size(); ++i) out << "all_ratip" + to_string(i) + "\t";
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
		for (int j = 0; j < psmVec[i].a1Ratio.size(); ++j)
			out << psmVec[i].a1Ratio[j] << "\t";
		for (int j = 0; j < psmVec[i].allRatio.size(); ++j)
			out << psmVec[i].allRatio[j] << "\t";
		out << endl;
	}

	out.close();
}

void outputPsmspIDL(){
	ofstream out(para.output_ratio_path.c_str());
	if (!out.is_open()){
		cout << "Error opening file" << endl;
		exit(1);
	}
	else{
		cout << "Successfully open the file of " << para.output_ratio_path << "." << endl;
	}


	out << "Title\tScan\tScore\tMass1\tMass2\tCharge\tFDR\tPeptideSquence\tModification\tProteinAc\t";

	for (int i = 0; i < psmVec[0].a1Ratio.size(); ++i) 
		out << "a1_ratio" + to_string(i) + "\t";
	out << "annotation" << "\t";
	for (int i = 0; i < psmVec[0].allRatio.size(); ++i) 
		out << "all_ratip" + to_string(i) + "\t";
	out << endl;

	for (int i = 0; i < psmVec.size(); i++){
		out << psmVec[i].title << "\t" << psmVec[i].scan << "\t" << psmVec[i].score << "\t" << psmVec[i].mass1 << "\t"
			<< psmVec[i].mass2 << "\t" << psmVec[i].charge << "\t" << psmVec[i].fdr << "\t" << psmVec[i].pepSq << "\t"
			<< psmVec[i].modification << "\t" << psmVec[i].proAc << "\t";

		for (int j = 0; j < psmVec[i].a1Ratio.size(); ++j)
			out << psmVec[i].a1Ratio[j] << "\t";

		if (psmVec[i].annotation.size() != 0){
			out << "[" << psmVec[i].annotation[0];
			for (int j = 1; j < psmVec[i].annotation.size(); ++j)
				out << ", " << psmVec[i].annotation[j];
			out << "]" << "\t";
		}else
			out << "NAN" << "\t";


		for (int j = 0; j < psmVec[i].allRatio.size(); ++j)
			out << psmVec[i].allRatio[j] << "\t";
		out << endl;
	}

	out.close();
}

void outputProtein(){
	string output_protein_path = "";
	output_protein_path = para.output_ratio_path.substr(0, para.output_ratio_path.find_last_of('.')) + ".protein";
	//cout << output_protein_path << endl;
	//getchar();

	//for (int i = 0; i < proteinVec.size(); i++){
	//	cout << proteinVec[i].ac << "\t" << proteinVec[i].de << "\t" << proteinVec[i].uniqueNum;
	//	for (int j = 0; j < proteinVec[i].ratio.size(); j++) cout << "\t" << proteinVec[i].ratio[j];
	//	getchar();
	//}


	ofstream out(output_protein_path.c_str());
	if (!out.is_open()){
		cout << "\nError opening file" << endl; exit(1);
	}else{
		cout << "Successfully open the file of " << output_protein_path << "." << endl;
	}

	//out << "Ac\tDe\tPsmNum\tPepNum\tTag\tLeadinPro\t";
	out << "Ac\tDe\tUniquePeptide";
	for (int i = 0; i < psmVec[0].reporterCorrect.size(); i++) out << "\t" << "Inten" + to_string(i);
	out << endl;


	for (int i = 0; i < proteinVec.size(); i++){
		out << proteinVec[i].ac << "\t" << proteinVec[i].de << "\t" << proteinVec[i].uniqueNum;
		for (int j = 0; j < proteinVec[i].ratio.size(); j++) out << "\t" << proteinVec[i].ratio[j];
		out << endl;
	}

	out.close();
}

void outputResult(){

	cout << "\nStep9: Output .spectra and .protein file." << endl;
	
	if (para.quantMethod == 4){
		outputPsmspIDL();
		cout << "\nThat`s all, you can see What you want in the pQuant-ms2_Result.spectra." << endl;
	}
	else{
		outputPsms();
		outputProtein();
		cout << "\nThat`s all, you can see What you want in the pQuant-ms2_Result.spectra." << endl;
		cout << "And you can see more information in the pQuant-ms2_Result.protein." << endl;
	}

}