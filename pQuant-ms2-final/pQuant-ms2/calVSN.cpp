#include "Head.h"

extern int countStep;
extern parainfo para;
extern vector<psmInfo> psmVec;							// PSM������

void calVSN(){

	if (TRUE == para.runVSN){							// Ĭ���������VSN
		cout << "\n[Step" << countStep++ << "]" << " Performing VSN normalization on all of the PSMs` peaks intensity" << endl;
		if (4 == para.quantMethod){						// ����������pIDL
			initsocks();

			Rconnection *rc = new Rconnection();
			int notConnect = rc->connect();
			if (notConnect) {
				cout << runningInfo <<  " Unable to connect the R" << endl;
			}else{
				cout << runningInfo << " Connecting to R (connected)" << endl;
			}
			int status = 0;								//�� R �м��� VSN ��
			status = rc->voidEval("require('vsn')");

			for (int i = 0; i < psmVec[0].a1PairInten.size(); ++i){

				vector<double> repFz, repFm;

				for (int j = 0; j < psmVec.size(); ++j){
					if (psmVec[j].a1PairInten[i].fm != 0.0 && psmVec[j].a1PairInten[i].fz != 0.0){
						repFm.push_back(psmVec[j].a1PairInten[i].fm);
						repFz.push_back(psmVec[j].a1PairInten[i].fz);
					}
				}
	
				// �� C++ �е�ǿ�ȱ���ת���� R �е�ǿ�ȱ���
				int sizeL = repFm.size();
				double *tempD;
				tempD = new double[sizeL * 2];

				for (int j = 0; j < sizeL; ++j){
					tempD[j * 2] = repFm[j];
					tempD[j * 2 + 1] = repFz[j];
				}

				Rdouble *rd = new Rdouble(tempD, sizeL * 2);
				status = rc->assign("a", rd);

				delete rd;
				delete []tempD;

				// R ��̵� VSN
				string msg = "b<-matrix(a,ncol=2,byrow=TRUE)";
				status = rc->voidEval(msg.c_str());

				msg = "m = new('ExpressionSet', exprs = b)";
				status = rc->voidEval(msg.c_str());

				msg = "x = justvsn(m)";
				status = rc->voidEval(msg.c_str());

				msg = "a = as.double(t(exprs(x)))";
				status = rc->voidEval(msg.c_str());


				repFz.resize(0); repFm.resize(0);
				// �� R �е�ǿ�� a ת�ص� C++
				Rdouble *x = (Rdouble*)rc->eval("a");
				if (x){
					// �����ݷŻ�ԭ�����ǵ� VSN ����������ȡ��log2
					double *d_norm = x->doubleArray();
					int counter = 0;
					double intFm = 0.0, intFz = 0.0;
					for (int j = 0; j < sizeL; j++){
						intFm = d_norm[counter++];
						intFz = d_norm[counter++];
						intFm = pow(2, intFm);
						intFz = pow(2, intFz);

						repFm.push_back(intFm);
						repFz.push_back(intFz);
					}
				}

				int counter = 0;
				for (int j = 0; j < psmVec.size(); ++j){
					if (psmVec[j].a1PairInten[i].fm != 0.0 && psmVec[j].a1PairInten[i].fz != 0.0){
						// ����֮ǰת����ԭ�����ݹ�λ
						psmVec[j].a1RatioVSN.push_back(log(repFz[counter] / repFm[counter++]));
					}else{
						// ͬʱ���жϷ���
						psmVec[j].a1RatioVSN.push_back(NAN);
					}

				}

				cout << runningInfo << " performing VSN normalization (success)" << endl;
			}

		}else{											//��������ΪiTRAQ��TMT
		// TODO: ���iTRAQ��TMT��������VSN
		}
	
	}else{												//VSN����û��
		return;
	}

}