#include "Head.h"

extern vector<psmInfo> psmVec;					//PSM»º³åÇø

void calVSN(){
	initsocks();							// this is needed for Win32 - it does nothing on unix
	Rconnection *rc = new Rconnection();

	int i = rc->connect();
	if (i) {
		//char msg[128];
		//sockerrorchecks(msg, 128, -1);
		//printf("unable to connect (result=%d, socket:%s).\n", i, msg);
		cout << "Unable to connect the R..." << endl;
	}
	else{
		cout << "\tconnecting to R...(connected)" << endl;
	}

	int status = 0;
	//load the VSN package in R
	status = rc->voidEval("require('vsn')");

	//for (int i = 0; i < psmVec.size(); ++i){
	//	for (int j = 0; j < psmVec[i].a1Iten.size(); ++j){
	//		cout << psmVec[i].a1Iten[j] << "\t";
	//	}
	//	cout << endl;
	//	getchar();
	//}

	vector<double> rep1, rep2;
	for (int i = 0; i < psmVec.size(); ++i){
		if (psmVec[i].a1Iten[0] != 0.0 && psmVec[i].a1Iten[1] != 0.0){// if a reporter is missing, dont use it for normalization
			rep1.push_back(psmVec[i].a1Iten[0]);
			rep2.push_back(psmVec[i].a1Iten[1]);
		}
	}
	

	//transfer all the intensities to a double and assign that to variable 'a' in R
	int sizeL = rep1.size();
	double *d;
	d = new double[sizeL * 2];
	for (int i = 0; i < sizeL; ++i){
		/*cout << rep1[i] << " " << rep2[i] << endl;*/
		d[i * 2] = rep1[i];
		d[i * 2 + 1] = rep2[i];
	}

	Rdouble *rd = new Rdouble(d, sizeL * 2);
	status = rc->assign("a", rd);

	delete rd;
	delete[] d;

	string msg = "b<-matrix(a,ncol=2,byrow=TRUE)";
	status = rc->voidEval(msg.c_str());

	msg = "m = new('ExpressionSet', exprs = b)";
	status = rc->voidEval(msg.c_str());

	msg = "x = justvsn(m)";
	status = rc->voidEval(msg.c_str());

	msg = "a=as.double(t(exprs(x)))";
	status = rc->voidEval(msg.c_str());

	//get the data in 'a' back to C++
	ofstream testOut("testVSN.txt");
	Rdouble *x = (Rdouble*)rc->eval("a");
	if (x){
		//now place all this data where it belongs, and calculate normalized ratio
		//remember that the data from VSN is log2'ed.
		double *d_norm = x->doubleArray();
		int counter = 0;
		double int1 = 0.0, int2 = 0.0;
		for (int i = 0; i < sizeL; i++){
			int1 = d_norm[counter++];
			int2 = d_norm[counter++];
			int1 = pow(2, int1);
			int2 = pow(2, int2);
			//cout << rep1[i] << " " << rep2[i] << " " << rep1[i] / rep2[i] <<  " " << log(rep1[i] / rep2[i]) << endl;
			//cout << int1 << " " << int2 << " "  << int1/int2 << " " << log(int1/int2) <<  endl;
			//getchar();

			testOut << rep1[i] << "\t" << rep2[i] << "\t" << rep1[i] / rep2[i] << "\t" << log(rep1[i] / rep2[i]) << "\t";
			testOut << int1 << "\t" << int2 << "\t" << int1 / int2 << "\t" << log(int1 / int2) << endl;
		}
	}
	testOut.close();
	cout << "\tperforming VSN normalization...(success)" << endl;
}