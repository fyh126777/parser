#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include "parser.h"

double stripString(char* stringIn);
void printComponents(Component* compPtr);
void printNodes(Node* nodePtr, int compFlag);
char* strComponentType(Component* compPtr);

int main() {
	ifstream inFile;
	ofstream outFile;
	NodeHead nodeList;
	CompHead compList;
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
	EquaType eqType = Modified;
	strcpy(inName, "网表.txt");
	strcpy(outName, "输出4.txt");
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
				if ((charPtr1[0] == 'T') && (charPtr1[1] == 'E') && (charPtr1[4] == '=')) {
					douBuf4 = stripString(charPtr1);
				}
				charPtr1 = strtok(NULL, " ");

			}
			modelPtr = new Model(buf2, TtypeBuf, douBuf1, douBuf2, douBuf3, 1);
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
			nodePtr->printNodal(outFile, datum, lastnode);
		}
		nodePtr = nodePtr->getNext();
	}

	//go down the component list and give equations for all sources
	compPtr = compList.getComp(0);
	while (compPtr != NULL) {
		compPtr->specialPrint(outFile, datum);
		compPtr = compPtr->getNext();
	}
	

	return 0;
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
