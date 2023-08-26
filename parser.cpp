#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "parser.h"

double stripString(char* stringIn);
void printComponents(Component* compPtr);
void printNodes(Node* nodePtr, int compFlag);
char* strComponentType(Component* compPtr);
char* ComponentTypeName(Component* compPtr);  //obtain component type name
int portNum(Component* comPtr, Node* nodePtr); //obtain port number
bool isAccurate(double result[], int num, double accurateValue);
void LU(double A[][30], double x[], double b[], int n);
void convertArray(double jacMat[][30], double A[][30], double result[], double y[], int number);
void solving_method(double jacMat[][30], double result[], double minDert[], int number, int& count, double accurateValue, int datum, int lastnode, int flag, double t = 0);
NodeHead nodeList;
CompHead compList;

int main() {
	ifstream inFile;
	ofstream outFile;
	ModelHead modelList;
	// Buffers used in parsing:
	char  inName[NameLength], outName[NameLength], buf[BufLength], buf1[BufLength], buf2[BufLength], buf3[BufLength], nameBuf[NameLength], * bufPtr, * charPtr1, * charPtr2;
	Component* compPtr, * compPtr1, * compPtr2;
	int intBuf1, intBuf2, intBuf3, intBuf4,datum = 0, eqNum = NA, specPrintJacMNA = 0;
	Node* nodePtr, * nodePtr1, * nodePtr2;
	double douBuf1 = 0.0, douBuf2 = 0.0, douBuf3 = 0.0, douBuf4 = 0.0;
	CompType typeBuf;
	TranType TtypeBuf;
	Model* modelPtr;
	
	strcpy(inName, "case1.txt");
	strcpy(outName, "输出1.txt");
	inFile.open(inName, ios::in);
	outFile.open(outName, ios::out);
	cout << "Output saved to file: " << outName << endl;

	while (inFile.good()) {
		if ((buf == NULL) || (*buf == '\0')) {
			inFile.getline(buf, BufLength);
			continue;
		}
		strcpy(buf1, buf);
		if (!strcmp(strtok(buf1, " "), ".model")) {
			strcpy(buf2, strtok(NULL, " "));//buf2 nbjt
			charPtr1 = strtok(NULL, " ");//charPtr1 NPN
			if (!strcmp(charPtr1, "PNP"))
				TtypeBuf = PNP;
			else if (!strcmp(charPtr1, "NPN"))
				TtypeBuf = NPN;
			else if (!strcmp(charPtr1, "NMOS"))
				TtypeBuf = NMOS;
			else if (!strcmp(charPtr1, "PMOS"))
				TtypeBuf = PMOS;

			charPtr1 = strtok(NULL, " ");
			while (charPtr1 != NULL) {
				strcpy(buf3, "");
				if ((charPtr1[0] == 'I') && (charPtr1[1] == 'S') && (charPtr1[2] == '=')) {
					douBuf1 = stripString(charPtr1);
				}
				if ((charPtr1[0] == 'B') && (charPtr1[1] == 'F') && (charPtr1[2] == '=')) {
					douBuf2 = stripString(charPtr1);
				}
				if ((charPtr1[0] == 'B') && (charPtr1[1] == 'R') && (charPtr1[2] == '=')) {
					douBuf3 = stripString(charPtr1);
				}
				if ((charPtr1[0] == 'T') && (charPtr1[1] == 'E') && (charPtr1[2] == '=')) {
					douBuf4 = stripString(charPtr1);
				}
				charPtr1 = strtok(NULL, " ");

			}
			modelPtr = new Model(buf2, TtypeBuf, douBuf1, douBuf2, douBuf3, douBuf4);
			modelList.addModel(modelPtr);


		}
		inFile.getline(buf, BufLength);
	}
	inFile.close();
	inFile.open(inName, ios::in);
	char model_str[9];
	//  starting of parsing by creating linked list of components
	inFile.getline(buf, BufLength);       // first line of netlist is discarded
	inFile.getline(buf, BufLength);
	while (inFile.good()) {
		if ((buf == NULL) || (*buf == '\0')) {
			inFile.getline(buf, BufLength);
			continue;
		}

		if (isalpha(*buf)) {

			//  EDIT THIS SECTION IF NEW COMPONENTS ARE ADDED!!!
			//  we could do some rearranging in this section to catch each type in order.
			switch (*buf) {
			case 'v':
			case 'V':
				typeBuf = VSource;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				douBuf1 = atof(strtok(NULL, " "));
				compPtr = new Component(typeBuf, douBuf1, NA, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			case 'i':
			case 'I':
				cout << "I" << endl;
				typeBuf = ISource;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				douBuf1 = atof(strtok(NULL, " "));
				compPtr = new Component(typeBuf, douBuf1, NA, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			case 'q':
			case 'Q':
				typeBuf = BJT;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				intBuf3 = atoi(strtok(NULL, " "));
				compPtr = new Component(typeBuf, NA, NA, intBuf1, intBuf2, intBuf3, NA,
					modelList.getModel(strtok(NULL, " ")), nameBuf);//(nbjt)
				compList.addComp(compPtr);
				break;
			case 'm':
			case 'M':
				typeBuf = MOSFET;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				intBuf3 = atoi(strtok(NULL, " "));
				intBuf4 = atoi(strtok(NULL, " "));
				compPtr = new Component(typeBuf, NA, NA, intBuf1, intBuf2, intBuf3, intBuf4,
					modelList.getModel(strtok(NULL, " ")), nameBuf);
				compList.addComp(compPtr);
				break;
			case 'r':
			case 'R':
				typeBuf = Resistor;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				douBuf1 = atof(strtok(NULL, " "));
				compPtr = new Component(typeBuf, douBuf1, NA, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			case 'd':
			case 'D':
				typeBuf = Diode;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				charPtr1 = strtok(NULL, " ");
				while (charPtr1 != NULL) {
					if ((charPtr1[0] == 'I') && (charPtr1[1] == 'S') && (charPtr1[2] == '=')) {
						douBuf1 = stripString(charPtr1);
					}
					if ((charPtr1[0] == 'T') && (charPtr1[1] == 'E') && (charPtr1[4] == '=')) {
						douBuf2 = stripString(charPtr1);
					}
					charPtr1 = strtok(NULL, " ");
				}
				compPtr = new Component(typeBuf, douBuf1, douBuf2, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			case 'c':
			case 'C':
				typeBuf = Capacitor;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				douBuf1 = atof(strtok(NULL, " "));
				compPtr = new Component(typeBuf, douBuf1, NA, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			case 'l':
			case 'L':
				typeBuf = Inductor;
				strcpy(nameBuf, strtok(buf, " "));
				intBuf1 = atoi(strtok(NULL, " "));
				intBuf2 = atoi(strtok(NULL, " "));
				douBuf1 = atof(strtok(NULL, " "));
				compPtr = new Component(typeBuf, douBuf1, NA, intBuf1, intBuf2, NA, NA, NULL, nameBuf);
				compList.addComp(compPtr);
				break;
			};
		}
		inFile.getline(buf, BufLength);
	}

	//  Now the components are created and it is time to set up the list of nodes.
	//  we should actually use second connector of first Source as the first Node (Datum)
	compPtr1 = compList.getComp(0);
	while (compPtr1 != NULL) {
		for (int b = 0; b < 3; b++) { /* ~> J. Erik Melo note: A component can have until 4 connectors. But here just 3 are been considered. It should change the condition to 'b <= 3' or 'b < 4'?*/
			if ((!compPtr1->isCon(b)) && (compPtr1->getConVal(b) != NA)) { //~> verify if the connector 'b' is not set && if the name of the node to which this same connector 'b' is connected is a valid name as found in the circuit file. That is, if the name is not NA, that is, if this connector was named in the instantiation of the component.
				intBuf1 = compPtr1->getConVal(b); // 获取网表文件中的连接器编号。    如果连接同一连接器“b”的节点的名称是电路文件中找到的有效名称。（不等于NA）即，如果此连接器在组件的实例化中被命名。
				nodePtr1 = nodeList.addNode();
				nodePtr1->setNameNum(intBuf1);  //  将节点命名为网表文件中的名称
				compPtr1->connect(b, nodePtr1); // 将组件的“连接器”连接到节点
				nodePtr1->connect(b, compPtr1); // 将节点的“连接器”连接到组件

				// now search and connect all other appropriate connectors to this node.现在寻找并将所有其他合适的连接器连接到该节点。
				// error checking should be added to prevent duplicated, or skipped connectors.应该添加错误检查以防止重复或跳过连接器。
				compPtr2 = compPtr1->getNext();
				while (compPtr2 != NULL) {
					for (int c = 0; c < 3; c++) { //~> verifying which one of the others connectors (of components) are connected to the node above验证哪个连接器被连接到上述节点上
						if (compPtr2->getConVal(c) == intBuf1) { //如果组件列表中的下一个组件具有与上述连接器同名的连接器(conNum)，则将其连接到相同的节点。	      compPtr2->connect( c, nodePtr1 );
							compPtr2->connect(c, nodePtr1);//66666
							nodePtr1->connect(c, compPtr2);
							break;                                    //~> As a component can only have one connector with the same name (connected in the same node), don't search the others and go out of the 'for' loop
						}
					}
					compPtr2 = compPtr2->getNext();
				}
			}
		}
		compPtr1 = compPtr1->getNext();
	}
	nodePtr = nodeList.getNode(0); //找lastnode
	int lastnode = nodePtr->getNameNum();
	while (nodePtr != NULL) {
		lastnode = (nodePtr->getNameNum() > lastnode) ? nodePtr->getNameNum() : lastnode;
		nodePtr = nodePtr->getNext();
	}

	compPtr = compList.getComp(0);
	printComponents(compPtr);

	nodePtr = nodeList.getNode(0);
	printNodes(nodePtr, 1);

	outFile << endl
		<< "%*****************************************************************************" << endl;
	outFile << "%                      Component Values:" << endl;
	compPtr = compList.getComp(0);
	while (compPtr != NULL) {
		compPtr->printVal(outFile);
		compPtr = compPtr->getNext();
	}
	outFile << endl
		<< "%*****************************************************************************" << endl;


	// go down the nodal list and have components announce themselves
	outFile << endl << "%                      Circuit Equations: " << endl;
	nodePtr = nodeList.getNode(0);
	while (nodePtr != NULL) {
		if (nodePtr->getNameNum() != datum) {
			nodePtr->printNodal(outFile, datum, lastnode);//输出正常节点（不连接激励源）的方程
		}
		nodePtr = nodePtr->getNext();
	}
	
	//go down the component list and give equations for all sources
	compPtr = compList.getComp(0);
	while (compPtr != NULL) {
		compPtr->specialPrint(outFile, datum);
		compPtr = compPtr->getNext();
	}


	EquaType eqType = Modified;
	//~> go down the component list and give supernode equations for all float sources (Nodal Analysis)
	if (eqType != Modified) {
		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			compPtr->printSuperNode(outFile, datum, lastnode);
			compPtr = compPtr->getNext();
		}
	}
	
	// go down the node list and give additional MNA equations
	if (eqType == Modified) {
		nodePtr = nodeList.getNode(0);
		while (nodePtr != NULL) {
			if (nodePtr->getNameNum() != datum)
				nodePtr->printMNA(outFile, datum, lastnode);
			nodePtr = nodePtr->getNext();
		}
	}
	
	// print jacobians
	outFile << endl
		<< "%*****************************************************************************" << endl;
	outFile << endl << "%                      Jacobians: " << endl;
	nodePtr1 = nodeList.getNode(0);
	while (nodePtr1 != NULL) {   //这个循环处理未连接到Vsource的节点和那些不是零点的节点
		if (nodePtr1->getNameNum() != datum) {
			nodePtr2 = nodeList.getNode(0);
			while (nodePtr2 != NULL) {
				if (nodePtr2->getNameNum() != datum) {
					nodePtr1->printJac(outFile, datum, nodePtr2, lastnode, eqType);
				}
				nodePtr2 = nodePtr2->getNext();
			}
		}
		nodePtr1 = nodePtr1->getNext();
	}

	// go down the component list and give equations for all sources
	compPtr = compList.getComp(0);
	while (compPtr != NULL) {
		nodePtr2 = nodeList.getNode(0);
		compPtr2 = compList.getComp(0);
		while (nodePtr2 != NULL) {
			if (nodePtr2->getNameNum() != datum) {
				compPtr->specialPrintJac(outFile, datum, nodePtr2, lastnode, eqType, compPtr2, &specPrintJacMNA ); // ~> specPrintJacMNA is used to verify if the jacobians w.r.t. the Modified equations was already printed to print only once.
			}
			nodePtr2 = nodePtr2->getNext();
		}
		specPrintJacMNA = 0;
		compPtr = compPtr->getNext();
	}




	// print the Jacobians for the additional MNA equations
	if (eqType == Modified) {
		nodePtr1 = nodeList.getNode(0);
		while (nodePtr1 != NULL) {
			if (nodePtr1->getNameNum() != datum) {
				nodePtr2 = nodeList.getNode(0);
				while (nodePtr2 != NULL) {
					if (nodePtr2->getNameNum() != datum)
						nodePtr1->printJacMNA(outFile, datum, nodePtr2, lastnode);
					nodePtr2 = nodePtr2->getNext();
				}
			}
			nodePtr1 = nodePtr1->getNext();
		}
	}
	int number;//未知量的个数
	cout << "请输入初始数据的个数：" << endl;
	cin >> number;
	cout << "请输入初始数据值:" << endl;
	for (int i = 0; i < number; i++) {
		cin >> nodeValue[i + 1];
	}

	int flag;
	printf("请输入要选择的求解电路的方法(1:牛顿迭代法;2:同伦法;3:瞬态分析法):\n");
	scanf("%d", &flag);

	int count = 1;
	double accurateValue;
    cout << "请输入精度:" << endl;
	cin >> accurateValue;

	if (flag == 1) {
		
		bool NR = true;

		nodePtr = nodeList.getNode(0);
		while (nodePtr != NULL) {
			if (nodePtr->getNameNum() != datum) {
				nodePtr->printNodalMat(datum, lastnode, result);
			}
			nodePtr = nodePtr->getNext();
		}

		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			compPtr->specialPrintMat(datum, result);
			compPtr = compPtr->getNext();
		}


		//~> go down the component list and give supernode equations for all float sources (Nodal Analysis)
		if (eqType != Modified) {
			compPtr = compList.getComp(0);
			while (compPtr != NULL) {
				compPtr->printSuperNodeMat(datum, lastnode, result);
				compPtr = compPtr->getNext();
			}
		}


		// go down the node list and give additional MNA equations
		if (eqType == Modified) {
			nodePtr = nodeList.getNode(0);
			while (nodePtr != NULL) {
				if (nodePtr->getNameNum() != datum)
					nodePtr->printMNAMat(datum, lastnode, result);
				nodePtr = nodePtr->getNext();
			}
		}





		nodePtr1 = nodeList.getNode(0);
		while (nodePtr1 != NULL) {
			if (nodePtr1->getNameNum() != datum) {
				nodePtr2 = nodeList.getNode(0);
				while (nodePtr2 != NULL) {
					if (nodePtr2->getNameNum() != datum) {
						nodePtr1->printJacMat(datum, nodePtr2, lastnode, eqType, jacMat);
					}
					nodePtr2 = nodePtr2->getNext();
				}
			}
			nodePtr1 = nodePtr1->getNext();
		}

		// go down the component list and give equations for all sources
		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			nodePtr2 = nodeList.getNode(0);
			compPtr2 = compList.getComp(0);
			while (nodePtr2 != NULL) {
				if (nodePtr2->getNameNum() != datum) {
					compPtr->specialPrintJacMat(datum, nodePtr2, lastnode, eqType, compPtr2, &specPrintJacMNA, jacMat); // ~> specPrintJacMNA is used to verify if the jacobians w.r.t. the Modified equations was already printed to print only once.
				}
				nodePtr2 = nodePtr2->getNext();
			}
			specPrintJacMNA = 0;
			compPtr = compPtr->getNext();
		}




		// print the Jacobians for the additional MNA equations
		if (eqType == Modified) {
			nodePtr1 = nodeList.getNode(0);
			while (nodePtr1 != NULL) {
				if (nodePtr1->getNameNum() != datum) {
					nodePtr2 = nodeList.getNode(0);
					while (nodePtr2 != NULL) {
						if (nodePtr2->getNameNum() != datum)
							nodePtr1->printJacMNAMat(datum, nodePtr2, lastnode, jacMat);
						nodePtr2 = nodePtr2->getNext();
					}
				}
				nodePtr1 = nodePtr1->getNext();
			}
		}
		


		cout << "------------------output------------------------------------" << endl;


		solving_method(jacMat, result, minDert, number, count, accurateValue, datum, lastnode ,flag);

		cout << "iteration number:" << "  " << count << endl;
		cout << endl;
		for (int i = 0; i < number; i++) {
			cout << "▲x(" << i + 1 << ") =    " << minDert[i] << endl;
		}
		cout << endl;
		cout << "the result:" << endl;
		for (int i = 0; i < number; i++) {
			cout << "x(" << i + 1 << ") =    " << nodeValue[i + 1] << endl;
		}
	}
	if (flag == 2) {//************************同伦法*********************************
		double stepsize;
		cout << "请输入步长：" << endl;
		cin >> stepsize;
		double t = 0;
		t = t + stepsize;

		nodePtr = nodeList.getNode(0);
		while (nodePtr != NULL) {
			if (nodePtr->getNameNum() != datum) {
				nodePtr->printNodalMat(datum, lastnode, result);
			}
			nodePtr = nodePtr->getNext();
		}

		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			compPtr->specialPrintMat(datum, result);
			compPtr = compPtr->getNext();
		}


		//~> go down the component list and give supernode equations for all float sources (Nodal Analysis)
		if (eqType != Modified) {
			compPtr = compList.getComp(0);
			while (compPtr != NULL) {
				compPtr->printSuperNodeMat(datum, lastnode, result);
				compPtr = compPtr->getNext();
			}
		}


		// go down the node list and give additional MNA equations
		if (eqType == Modified) {
			nodePtr = nodeList.getNode(0);
			while (nodePtr != NULL) {
				if (nodePtr->getNameNum() != datum)
					nodePtr->printMNAMat(datum, lastnode, result);
				nodePtr = nodePtr->getNext();
			}
		}





		nodePtr1 = nodeList.getNode(0);
		while (nodePtr1 != NULL) {
			if (nodePtr1->getNameNum() != datum) {
				nodePtr2 = nodeList.getNode(0);
				while (nodePtr2 != NULL) {
					if (nodePtr2->getNameNum() != datum) {
						nodePtr1->printJacMat(datum, nodePtr2, lastnode, eqType, jacMat);
					}
					nodePtr2 = nodePtr2->getNext();
				}
			}
			nodePtr1 = nodePtr1->getNext();
		}

		// go down the component list and give equations for all sources
		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			nodePtr2 = nodeList.getNode(0);
			compPtr2 = compList.getComp(0);
			while (nodePtr2 != NULL) {
				if (nodePtr2->getNameNum() != datum) {
					compPtr->specialPrintJacMat(datum, nodePtr2, lastnode, eqType, compPtr2, &specPrintJacMNA, jacMat); // ~> specPrintJacMNA is used to verify if the jacobians w.r.t. the Modified equations was already printed to print only once.
				}
				nodePtr2 = nodePtr2->getNext();
			}
			specPrintJacMNA = 0;
			compPtr = compPtr->getNext();
		}




		// print the Jacobians for the additional MNA equations
		if (eqType == Modified) {
			nodePtr1 = nodeList.getNode(0);
			while (nodePtr1 != NULL) {
				if (nodePtr1->getNameNum() != datum) {
					nodePtr2 = nodeList.getNode(0);
					while (nodePtr2 != NULL) {
						if (nodePtr2->getNameNum() != datum)
							nodePtr1->printJacMNAMat(datum, nodePtr2, lastnode, jacMat);
						nodePtr2 = nodePtr2->getNext();
					}
				}
				nodePtr1 = nodePtr1->getNext();
			}
		}
		for (int i = 1; i <= number; i++) {
			initF[i] = result[i];
			result[i] = t * initF[i];
		}
		while (t < 1.0) {
			solving_method(jacMat, result, minDert, number, count, accurateValue, datum, lastnode,flag);
			t += stepsize;

			nodePtr = nodeList.getNode(0);
			while (nodePtr != NULL) {
				if (nodePtr->getNameNum() != datum) {
					nodePtr->printNodalMat(datum, lastnode, result);
				}
				nodePtr = nodePtr->getNext();
			}

			compPtr = compList.getComp(0);
			while (compPtr != NULL) {
				compPtr->specialPrintMat(datum, result);
				compPtr = compPtr->getNext();
			}


			//~> go down the component list and give supernode equations for all float sources (Nodal Analysis)
			if (eqType != Modified) {
				compPtr = compList.getComp(0);
				while (compPtr != NULL) {
					compPtr->printSuperNodeMat(datum, lastnode, result);
					compPtr = compPtr->getNext();
				}
			}


			// go down the node list and give additional MNA equations
			if (eqType == Modified) {
				nodePtr = nodeList.getNode(0);
				while (nodePtr != NULL) {
					if (nodePtr->getNameNum() != datum)
						nodePtr->printMNAMat(datum, lastnode, result);
					nodePtr = nodePtr->getNext();
				}
			}

			//求jac矩阵

			nodePtr1 = nodeList.getNode(0);
			while (nodePtr1 != NULL) {
				if (nodePtr1->getNameNum() != datum) {
					nodePtr2 = nodeList.getNode(0);
					while (nodePtr2 != NULL) {
						if (nodePtr2->getNameNum() != datum) {
							nodePtr1->printJacMat(datum, nodePtr2, lastnode, eqType, jacMat);
						}
						nodePtr2 = nodePtr2->getNext();
					}
				}
				nodePtr1 = nodePtr1->getNext();
			}

			// go down the component list and give equations for all sources
			compPtr = compList.getComp(0);
			while (compPtr != NULL) {
				nodePtr2 = nodeList.getNode(0);
				compPtr2 = compList.getComp(0);
				while (nodePtr2 != NULL) {
					if (nodePtr2->getNameNum() != datum) {
						compPtr->specialPrintJacMat(datum, nodePtr2, lastnode, eqType, compPtr2, &specPrintJacMNA, jacMat); // ~> specPrintJacMNA is used to verify if the jacobians w.r.t. the Modified equations was already printed to print only once.
					}
					nodePtr2 = nodePtr2->getNext();
				}
				specPrintJacMNA = 0;
				compPtr = compPtr->getNext();
			}




			// print the Jacobians for the additional MNA equations
			if (eqType == Modified) {
				nodePtr1 = nodeList.getNode(0);
				while (nodePtr1 != NULL) {
					if (nodePtr1->getNameNum() != datum) {
						nodePtr2 = nodeList.getNode(0);
						while (nodePtr2 != NULL) {
							if (nodePtr2->getNameNum() != datum)
								nodePtr1->printJacMNAMat(datum, nodePtr2, lastnode, jacMat);
							nodePtr2 = nodePtr2->getNext();
						}
					}
					nodePtr1 = nodePtr1->getNext();
				}
			}
			if (t < 1.0) {
				for (int i = 1; i <= number; i++) {
					result[i] = result[i] - (1 - t) * initF[i];
				}
			}


		}
		solving_method(jacMat, result, minDert, number, count, accurateValue, datum, lastnode,1);

		cout << endl;
		for (int i = 0; i < number; i++) {
			cout << "▲x(" << i + 1 << ") =    " << minDert[i] << endl;
		}
		cout << endl;
		cout << "the result:" << endl;
		for (int i = 0; i < number; i++) {
			cout << "x(" << i + 1 << ") =    " << nodeValue[i + 1] << endl;
		}

	}


	cout << endl << "That's all folks!" << endl;

	return 0;
}


void solving_method(double jacMat[][30], double result[], double minDert[], int number, int& count, double accurateValue, int datum, int lastnode, int flag, double t) {

	Component* compPtr, * compPtr2;
	Node* nodePtr, * nodePtr1, * nodePtr2;
	int specPrintJacMNA = 0;
	EquaType eqType = Modified;

	double A[30][30], b[30];
	convertArray(jacMat, A, result, b, number);
	LU(A, minDert, b, number);

	for (int i = 0; i < number; i++) {
		nodeValue[i + 1] = nodeValue[i + 1] + minDert[i];
	}


	while (!isAccurate(minDert, number, accurateValue)) {//如果没达到精度，继续

		for (int i = 0; i < number; i++) {
			for (int j = 0; j < number; j++) {
				jacMat[i + 1][j + 1] = 0.0;
			}
			result[i + 1] = 0.0;
		}
		count++;
		nodePtr = nodeList.getNode(0);
		while (nodePtr != NULL) {
			if (nodePtr->getNameNum() != datum) {
				nodePtr->printNodalMat(datum, lastnode, result);
			}
			nodePtr = nodePtr->getNext();
		}

		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			compPtr->specialPrintMat(datum, result);
			compPtr = compPtr->getNext();
		}


		//~> go down the component list and give supernode equations for all float sources (Nodal Analysis)
		if (eqType != Modified) {
			compPtr = compList.getComp(0);
			while (compPtr != NULL) {
				compPtr->printSuperNodeMat(datum, lastnode, result);
				compPtr = compPtr->getNext();
			}
		}


		// go down the node list and give additional MNA equations
		if (eqType == Modified) {
			nodePtr = nodeList.getNode(0);
			while (nodePtr != NULL) {
				if (nodePtr->getNameNum() != datum)
					nodePtr->printMNAMat(datum, lastnode, result);
				nodePtr = nodePtr->getNext();
			}
		}





		nodePtr1 = nodeList.getNode(0);
		while (nodePtr1 != NULL) {
			if (nodePtr1->getNameNum() != datum) {
				nodePtr2 = nodeList.getNode(0);
				while (nodePtr2 != NULL) {
					if (nodePtr2->getNameNum() != datum) {
						nodePtr1->printJacMat(datum, nodePtr2, lastnode, eqType, jacMat);
					}
					nodePtr2 = nodePtr2->getNext();
				}
			}
			nodePtr1 = nodePtr1->getNext();
		}

		// go down the component list and give equations for all sources
		compPtr = compList.getComp(0);
		while (compPtr != NULL) {
			nodePtr2 = nodeList.getNode(0);
			compPtr2 = compList.getComp(0);
			while (nodePtr2 != NULL) {
				if (nodePtr2->getNameNum() != datum) {
					compPtr->specialPrintJacMat(datum, nodePtr2, lastnode, eqType, compPtr2, &specPrintJacMNA, jacMat); // ~> specPrintJacMNA is used to verify if the jacobians w.r.t. the Modified equations was already printed to print only once.
				}
				nodePtr2 = nodePtr2->getNext();
			}
			specPrintJacMNA = 0;
			compPtr = compPtr->getNext();
		}




		// print the Jacobians for the additional MNA equations
		if (eqType == Modified) {
			nodePtr1 = nodeList.getNode(0);
			while (nodePtr1 != NULL) {
				if (nodePtr1->getNameNum() != datum) {
					nodePtr2 = nodeList.getNode(0);
					while (nodePtr2 != NULL) {
						if (nodePtr2->getNameNum() != datum)
							nodePtr1->printJacMNAMat(datum, nodePtr2, lastnode, jacMat);
						nodePtr2 = nodePtr2->getNext();
					}
				}
				nodePtr1 = nodePtr1->getNext();
			}
		}

		if (flag==1) {

			convertArray(jacMat, A, result, b, number);
			LU(A, minDert, b, number);

			for (int i = 0; i < number; i++) {
				nodeValue[i + 1] = nodeValue[i + 1] + minDert[i];

			}
		}
		if (flag==2) {
			for (int i = 1; i <= number; i++) {
				result[i] = result[i] - (1 - t) * initF[i];
			}
			convertArray(jacMat, A, result, b, number);
			LU(A, minDert, b, number);

			for (int i = 0; i < number; i++) {
				nodeValue[i + 1] = nodeValue[i + 1] + minDert[i];
			}
		}
	}

}











void convertArray(double jacMat[][30], double A[][30], double result[], double y[], int number) {
	for (int i = 0; i < number; i++) {
		for (int j = 0; j < number; j++) {
			A[i][j] = jacMat[i + 1][j + 1];
		}

		y[i] = -result[i + 1];
	}
}


void LU(double A[][30], double x[], double b[], int n) {
	//初始化L和U矩阵
	double L[30][30] = { 0 }, U[30][30] = { 0 }, y[30] = { 0 };
	for (int j = 0; j < n; j++) {
		U[0][j] = A[0][j];
		L[j][j] = 1.0;

	}
	for (int i = 1; i < n; i++) {
		if (U[0][0] != 0.0) {
			L[i][0] = A[i][0] / U[0][0];
		}


	}
	//计算 L、U 矩阵
	for (int k = 1; k < n; k++) {
		double temp = 0;
		for (int j = k; j < n; j++) {
			temp = 0;
			for (int r = 0; r < k; r++) {
				temp += L[k][r] * U[r][j];
			}
			U[k][j] = A[k][j] - temp;
		}
		for (int i = k + 1; i < n; i++) {
			temp = 0;
			for (int l = 0; l < k; l++) {
				temp += L[i][l] * U[l][k];
			}
			if (U[k][k] != 0.0) {
				L[i][k] = (A[i][k] - temp) / U[k][k];
			}

		}
	}
	//求矩阵y
	y[0] = b[0];
	for (int i = 1; i < n; i++) {
		double temp = 0;
		for (int l = 0; l < i; l++) {
			temp += L[i][l] * y[l];
		}
		y[i] = b[i] - temp;
	}
	//求矩阵x
	if (U[n - 1][n - 1] != 0.0) {
		x[n - 1] = y[n - 1] / U[n - 1][n - 1];
	}

	for (int i = n - 2; i >= 0; i--) {
		double temp = 0;
		for (int l = n - 1; l > i; l--) {
			temp += U[i][l] * x[l];
		}
		if (U[i][i] != 0.0) {
			x[i] = (y[i] - temp) / U[i][i];
		}

	}

}



bool isAccurate(double minDert[], int num, double acc) {
	bool re = true;
	for (int i = 0; i < num; i++) {
		if (minDert[i] > acc || -minDert[i] > acc) {
			re = false;
		}
	}
	return re;

}
double stripString(char* stringIn) {
	char buf[BufLength], buf2[BufLength];
	int a, b;
	strcpy(buf, stringIn);
	for (a = 0; buf[a] != '='; a++) {};
	a++;
	for (b = 0; buf[a] != '\0'; b++, a++)
		buf2[b] = buf[a];
	buf2[b] = '\0';
	return atof(buf2);
};


//Print the linked list of components to check
void printComponents(Component* compPtr) {
	char compTypeName[6];
	cout << endl << "Components: " << endl << endl;
	while (compPtr != NULL) {
		strcpy(compTypeName, strComponentType(compPtr));
		cout << "->" << compTypeName << compPtr->getcompNum();
		compPtr = compPtr->getNext();
	}
	cout << endl;
	return;
}

void printNodes(Node* nodePtr, int compFlag) {

	Connections* conPtr;
	cout << endl << "Nodes: " << endl << endl;
	while (nodePtr != NULL) {
		if (compFlag == 0) { //It is printed just the names of the nodes
			cout << "节点：" << nodePtr->getNameNum();
		}
		else if (compFlag == 1) { //It is printed the nodes and the connections
			cout << "节点：" << nodePtr->getNameNum() << "       " << "所连器件数为: ";
			conPtr = nodePtr->getConList();
			cout << nodePtr->getCount() << endl;
			while (conPtr->next != NULL) {
				cout << "编号:" << conPtr->comp->getcompNum() << "   value:" << conPtr->comp->getVal() << "     名称 : " << conPtr->comp->getName() << endl;
				conPtr = conPtr->next;
			}
			cout << "编号:" << conPtr->comp->getcompNum() << "    value:" << conPtr->comp->getVal() << "     名称:" << conPtr->comp->getName() << endl;
		}
		else {
			cout << "Invalid value for compFlag. (0) to print just nodes, (1) to print nodes and connections!";
			exit(1);

		}

		nodePtr = nodePtr->getNext();
	}


	return;
}



char* strComponentType(Component* compPtr) {

	char* compTypeName = new char[6];
	switch (compPtr->getType()) {

	case VSource: strcpy(compTypeName, "V"); break;
	case Resistor: strcpy(compTypeName, "R"); break;
	case BJT: strcpy(compTypeName, "T"); break;
	case MOSFET: strcpy(compTypeName, "M"); break;
	case ISource: strcpy(compTypeName, "I"); break;
	case Inductor: strcpy(compTypeName, "ind"); break;
	case Diode: strcpy(compTypeName, "Diode"); break;
	case Capacitor: strcpy(compTypeName, "Cap"); break;
	}

	return compTypeName;
}
