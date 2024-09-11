#include<iostream>
#include<stdio.h>
#include<stdlib.h> 
#include<cstring>
#include<string>
#include<iomanip>
#include<vector>
#include<malloc.h>
#include<sstream>
#include <algorithm>
#include<conio.h>
using namespace std;

//Ԥ���峣�����������������鳤�ȶ�������ʱ������ 
#define SIZE_OF_SYM 1000  //���ű�
#define SIZE_OF_MRY 10000 //�ڴ��
#define SIZE_OF_SYM_FUNC 100//��������ʱ�ķ��ű�
#define SIZE_OF_MRY_FUNC 1000 //��������ʱ���ڴ��


bool lock = false;  //��������ִ�к���ǰ����������ִ���������� 

typedef struct ST {
	string varName;	//������
	int addr;	//����ʵ��ֵ�洢����ʼλ��
	string varType;	//�������ͣ�ע�⣬����Ҳ�����ͣ�
	int varLength;	//��������
}ST;

ST symList[SIZE_OF_SYM]; //���ű����ͬʱ֧��1000������
int mryList[SIZE_OF_MRY]; //�ڴ�� 

typedef struct funcActivation {//�����¼�Ľṹ��
	string funcName;
	string returnType;
	string returnVar;
	string funcVar1;
	string funcVar2;
	ST func_ST[SIZE_OF_SYM_FUNC]; //���������еľֲ����������֧��100���ֲ����� 
	int func_mry[SIZE_OF_MRY_FUNC]; //�ֲ��ڴ��
}funcActivation;
funcActivation funcAct;

typedef struct temp {
	string varType;
	string varValue;
}temp;
temp tmp;//��ʱ�����洢��


int getSTLen(ST s[]) {
	if (s == symList) {
		return SIZE_OF_SYM; //���ط��ű���
	}
	else if (s == funcAct.func_ST) {
		return SIZE_OF_SYM_FUNC; //���ؾֲ����ű���
	}
}

int getMRYLen(int m[]) {
	if (m == mryList) {
		return SIZE_OF_MRY;	//�����ڴ����
	}
	else if (m == funcAct.func_mry) {
		return SIZE_OF_MRY_FUNC; //���ؾֲ��ڴ����
	}
}

string intToStr(int ans) { //������ת��Ϊ�ַ������ͣ��Է����ڴ������ű����
	stringstream s;
	s << ans;
	string p = s.str();
	const char* res = p.c_str();
	return res;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	char* strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char* p = strtok(strs, d);
	while (p) {
		string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
		res.push_back(s); //����������  
		p = strtok(NULL, d);
	}
	return res;
}

void symctl(ST* symList) { //���ű�鿴
	cout << "************ Symbol List ************" << endl;
	int len = getSTLen(symList);
	cout << "varType" << '\t' << "varName" << '\t' << "    varAddr" << '\t' << "varLength" << endl;
	for (int i = 0; i < len; i++) {
		if (symList[i].varName == "") break;
		cout << symList[i].varType << '\t' << symList[i].varName << "\t    \t" << symList[i].addr << "\t\t" << symList[i].varLength << endl;
	}
	cout << endl;
}

void memory(int* mry) { //��ȫ���ڴ��Ĳ鿴
	cout << "************ Memory List ************" << endl;
	cout << "address" << '\t' << "value" << endl;
	int len = getMRYLen(mry);
	for (int i = 1; i < len; i++) {
		if (mry[i] != 0)
			cout << i << '\t' << mry[i] << endl;
	}
	cout << endl;
}

void funcAct_memory(int* mry) { //�Ժ�������ʱ�ڴ��Ĳ鿴
	cout << "************ Memory List of Activation Record ************" << endl;
	cout << "address" << '\t' << "value" << endl;
	for (int i = 1; i < 1000; i++) {
		if (funcAct.func_mry[i] != 0) //�ֲ��ڴ��
			cout << i << '\t' << funcAct.func_mry[i] << endl;
	}
	cout << endl;
}

void showTmp() { //�Ĵ����鿴
	cout << "********** �Ĵ��� **********" << endl;
	cout << "������������:" << tmp.varType << endl;
	cout << "��������ֵ�� " << tmp.varValue << endl;
}


void declear(string type, ST* symList) { //�û����б�������ʱ���øú���
	if (type == "string" || type == "int") {
		string varName;
		cin >> varName;	//�����������
		int len = getSTLen(symList);
		for (int i = 0; i < len; i++) {  //���ű�˳��洢���� 
			if (symList[i].varName == "") {
				symList[i].varName = varName; //������������ű� 
				symList[i].varType = type;
				break;
			}
			if (symList[i].varName == varName) {
				cout << "Variable name conflict! This variable has been defined already!" << endl << endl;
				break;
			}
		}
	}
	else {
		cout << "Data type is not supported !" << endl;	//ֻ֧��string��int����
	}
}

void func_declear(string type, string varName, ST* symList) {//�û����к�������ʱ���øú���
	if (type == "string" || type == "int") {
		int len = getSTLen(symList);
		for (int i = 0; i < len; i++) { //���ű�˳��洢���� 
			if (symList[i].varName == "") {
				symList[i].varName = varName; //������ű� 
				symList[i].varType = type;
				break;
			}

			if (symList[i].varName == varName) {
				cout << "Variable name conflict! This varluable has been defined already!" << endl << endl;
				break;
			}
		}
	}
	else {
		cout << "Data type is not supported !" << endl;
	}
}

void op_assignment(string varValue, int keyInST, ST* symList, int* mry) { //�û����г��θ�ֵ
	//���ڴ���������ֵ�������ռ�ڴ泤��ת�������ű�
	if (symList[keyInST].varType == "int") {

		int value = atoi(varValue.c_str());
		if (value == 0 && varValue != "0") {
			cout << "�������Ͳ�ƥ�䣡��ֵʧ�ܣ�" << endl;
		}
		else {
			int len = getMRYLen(mry);
			for (int i = 1; i < len; i++) {
				if (mry[i] == NULL) {
					mry[i] = value;
					symList[keyInST].addr = i;
					symList[keyInST].varLength = 1; //����ռһ��intλ�� ��4Bytes�� 
					break;
				}
			}
		}
	}
	else if (symList[keyInST].varType == "string") {
		int len = getMRYLen(mry);
		for (int i = 1; i < len; i++) {
			if (mry[i] == NULL) {
				int j = i;
				for (; j < i + varValue.length(); j++) { //�״���Ӧ������������Ƭ���� 
					if (mry[j] != NULL) {
						i = j;
						break;
					}
				}
				if (j == i + varValue.length()) { //ѭ���������ж�j��λ�þ����Ƿ��ڴ˴洢 
					for (int j = i, seq = 0; j < i + varValue.length(); j++, seq++) {
						mry[j] = varValue[seq]; //��string�е�ÿһ��char����ASSIC��洢���������� 
					}
					symList[keyInST].varLength = varValue.length();
					symList[keyInST].addr = i;	//string����
					break;
				}
			}
		}
	}
}

void op_againAssignment(string varValue, int keyInST, ST* symList, int* mry) { //�û������ٴθ�ֵ
	if (symList[keyInST].varType == "int") {
		mry[symList[keyInST].addr] = atoi(varValue.c_str());
	}
	else if (symList[keyInST].varType == "string") {
		for (int i = symList[keyInST].addr; i < symList[keyInST].addr + symList[keyInST].varLength; i++) {
			mry[i] = 0;
		}
		int len = getMRYLen(mry);
		for (int i = 1; i < len; i++) {
			if (mry[i] == NULL) {
				int j = i;
				for (; j < i + varValue.length(); j++) { //�״���Ӧ������������Ƭ���� 
					if (mry[j] != NULL) {
						i = j;
						break;
					}
				}
				if (j == i + varValue.length()) { //ѭ���������ж�j��λ�þ����Ƿ��ڴ˴洢 
					for (int j = i, seq = 0; j < i + varValue.length(); j++, seq++) {
						mry[j] = varValue[seq]; //��string�е�ÿһ��char����ASSIC��洢���������� 
					}
					symList[keyInST].varLength = varValue.length();
					symList[keyInST].addr = i;
					break;
				}
			}
		}

	}
}

string test(string elm, ST sym[], int m[]);

int fetchInt(int key, ST* symList, int* mry);

int op_culInt(string exp) {//���͵ļ򵥼���
	string elm1;
	string elm2;
	char op;
	for (int i = 0; i < exp.length(); i++) {
		if (exp[i] == '*' || exp[i] == '/' || exp[i] == '+' || exp[i] == '-') {
			elm1 = exp.substr(0, i);
			elm2 = exp.substr(i + 1, exp.length() - 1);
			op = exp[i];
		}
	}
	string Elm1 = test(elm1, symList, mryList);
	string Elm2 = test(elm2, symList, mryList);
	int intElm1 = atoi(Elm1.c_str());
	int intElm2 = atoi(Elm2.c_str());
	if ((intElm1 == 0) && (Elm1 != "0") || (intElm2 == 0) && (Elm2 == "0")) {
		cout << "�����Ͳ�ƥ�䣡�������" << endl;
		return -1;
	}
	switch (op)
	{
	case '*': return intElm1 * intElm2; break;
	case '/': return intElm1 / intElm2; break;
	case '+': return intElm1 + intElm2; break;
	case '-': return intElm1 - intElm2; break;
	defalut: cout << "����ģ����ϣ�" << endl << endl;
	}
}

string test(string elm, ST* symList, int* mry);

string op_culString(string exp, ST* symList, int* mry) {//�ַ����ļ򵥼���
	string elm1;
	string elm2;
	char op;
	for (int i = 0; i < exp.length(); i++) {
		if (exp[i] == '|' || exp[i] == '^' || exp[i] == '+') {
			elm1 = exp.substr(0, i);
			elm2 = exp.substr(i + 1, exp.length() - 1);

			elm1 = test(elm1, symList, mry);
			elm2 = test(elm2, symList, mry);
			op = exp[i];
		}
	}
	int intElm1 = atoi(elm1.c_str());
	int intElm2 = atoi(elm2.c_str());
	switch (op)
	{
	case '+': {
		string ans = elm1 + elm2;
		return ans;
		break;
	}
	case '|': { //�Բ������������ߵ��ַ�����λ�Ƚϣ�ȡС
		int minLen = min(elm1.length(), elm2.length());
		char res[minLen];
		int i = 0;
		for (; i < minLen; i++) {
			res[i] = (elm1[i] > elm2[i]) ? elm1[i] : elm2[i];
		}
		string ans = res;
		if (elm1.length() != elm2.length()) {
			string back = (elm1.length() > elm2.length()) ? elm1.substr(i, elm1.length() - 1) : elm2.substr(i, elm2.length() - 1);
			ans = ans + back;
		}
		return ans;
		break;
	}
	case '^': { //�Բ������������ߵ��ַ�����λ�Ƚϣ�ȡ��
		int minLen = min(elm1.length(), elm2.length());
		char res[minLen];
		int i = 0;
		for (; i < minLen; i++) {
			res[i] = (elm1[i] < elm2[i]) ? elm1[i] : elm2[i];
		}
		string ans = res;
		if (elm1.length() != elm2.length()) {
			string back = (elm1.length() > elm2.length()) ? elm1.substr(i, elm1.length() - 1) : elm2.substr(i, elm2.length() - 1);
			ans = ans + back;
		}
		return ans;
		break;
	}
		defalut: cout << "����ģ����ϣ�" << endl << endl;
	}
}

void call_func(vector<string> line_call);

void assignment(int key, ST* symList, int* mry) {	//�����=���ұ߱��ʽ�Ľ����׼����ֵ����=�����

	bool flag_deal_func = false; //��������Ƿ�����˺������ã� 
	string cmd3;
	cin >> cmd3;
	string res;
	if (split(cmd3, "(").size() == 2) {
		flag_deal_func = true;
		vector<string> testify;
		testify.push_back(cmd3);
		call_func(testify);
	}
	if (symList[key].varType == "int") {
		if (flag_deal_func == true) {
			if (funcAct.returnType != "int") {
				cout << "����ֵ������������Ͳ�ƥ��" << endl;
			}
			else {
				res = funcAct.returnVar;
			}
		}
		else {
			cmd3 = test(cmd3, symList, mry);
			int flag = 0;
			for (int i = 0; i < cmd3.length(); i++) {
				if (cmd3[i] == '*' || cmd3[i] == '/' || cmd3[i] == '+' || cmd3[i] == '-') {
					flag = 1;
					if (i == 0) {
						flag = 0; //��ʱ�Ȳ����Ǹ�������ǰ��Ķ�Ԫ���� 
					}
					break;
				}
			}
			if (flag == 1) {
				int ans = op_culInt(cmd3);
				res = intToStr(ans);
			}
			else {
				res = cmd3;
			}
		}
		if (symList[key].addr != 0) {// �ٴθ�ֵ����� 
			op_againAssignment(res, key, symList, mry);
		}
		else {                 // ���θ�ֵ����� 
			op_assignment(res, key, symList, mry);
		}
	}
	else if (symList[key].varType == "string") {
		if (flag_deal_func == true) {
			if (funcAct.returnType != "string") {
			}
			else {
				res = funcAct.returnVar;
			}
		}

		else {
			cmd3 = test(cmd3, symList, mry);
			int flag = 0;
			for (int i = 0; i < cmd3.length(); i++) {
				if (cmd3[i] == '|' || cmd3[i] == '^' || cmd3[i] == '+') {
					flag = 1;
				}
			}
			if (flag == 1) {
				res = op_culString(cmd3, symList, mry);
			}
			else {
				res = cmd3;
			}
		}
		if (symList[key].addr != 0) {// �ٴθ�ֵ����� 
			op_againAssignment(res, key, symList, mry);
		}
		else {
			op_assignment(res, key, symList, mry);
		}
	}

}

void all_assignment(int key, string varValue, ST* symList, int* mry) {//�����˳��θ�ֵ���ٴθ�ֵ�ĸ�ֵ 
	if (symList[key].addr != 0) {// �ٴθ�ֵ����� 
		op_againAssignment(varValue, key, symList, mry);
	}
	else {                 // ���θ�ֵ����� 
		op_assignment(varValue, key, symList, mry);
	}
}

int findVar(string cmd1, ST* symList) {//�ڷ��ű��в��ұ���Var
	int len = getSTLen(symList);
	for (int i = 0; i < len; i++) {
		if (symList[i].varName == cmd1) return i;
		else if (symList[i].varName == "") return -1;//�Ҳ���������-1
	}
}

void  func_assignment(string varName, string cmd3, ST* sym, int* m) { //������ֵ�ı�������������ֵ��ʽ�ӣ����ű��ڴ�� 
	int key = -1;
	bool flag_exist = true;
	ST* symList;
	int* mry;

	if (lock == false) {
		if (findVar(varName, sym) < 0) flag_exist = false;
		else {
			symList = sym;
			mry = m;
			key = (findVar(varName, symList));
		}
	}
	else if (lock == true) {
		if (findVar(varName, sym) < 0) {
			if (findVar(varName, ::symList) < 0) {
				flag_exist = false;
			}
			else {
				symList = ::symList;
				mry = ::mryList;
				key = (findVar(varName, symList));
			}
		}
		else {
			symList = sym;
			mry = m;
			key = (findVar(varName, symList));
		}

	}
	if (flag_exist == false) {
		cout << "����" << varName << "δ������" << endl;
	}
	else {
		//int����� 
		if (symList[key].varType == "int") {
			int flag = 0;
			for (int i = 0; i < cmd3.length(); i++) {
				if (cmd3[i] == '*' || cmd3[i] == '/' || cmd3[i] == '+' || cmd3[i] == '-') {
					flag = 1;
					if (i == 0) {
						flag = 0; //��ʱ�Ȳ����Ǹ�������ǰ��Ķ�Ԫ���� 
					}
					break;
				}
			}
			if (flag == 1) {
				int ans = op_culInt(cmd3);
				string res = intToStr(ans);
				all_assignment(key, res, symList, mry); //��ֵ 
			}
			else { //����Ҫ���� 
				if (lock == false) {

					if (findVar(cmd3, symList) > -1) {  //������cmd3��ֵ��ֵ�� ����varName 
						if (symList[findVar(cmd3, symList)].varType == "string") {
						}

						else if (symList[findVar(cmd3, symList)].varType == "int") {
							int k = findVar(cmd3, symList);
							int ans = fetchInt(k, symList, mry);
							string res = intToStr(ans);
							all_assignment(key, res, symList, mry); //��ֵ 
						}

					}
					else {
						if (atoi(cmd3.c_str()) > 0 || cmd3 == "0") {
							all_assignment(key, cmd3, symList, mry); //��ֵ 
						}
					}
				}
				else if (lock == true) {
					string res = test(cmd3, symList, mry);
					all_assignment(key, res, symList, mry); //��ֵ
				}
			}
		}
		else if (symList[key].varType == "string") {
			int flag = 0;
			for (int i = 0; i < cmd3.length(); i++) {
				if (cmd3[i] == '|' || cmd3[i] == '^' || cmd3[i] == '+') {
					flag = 1;
				}
			}
			if (flag == 1) {
				cmd3 = op_culString(cmd3, symList, mry);
			}
			else {
				cmd3 = test(cmd3, symList, mry);
			}
			if (symList[key].addr != 0) {// �ٴθ�ֵ����� 
				op_againAssignment(cmd3, key, symList, mry);
			}
			else {
				op_assignment(cmd3, key, symList, mry);
			}
		}
	}
}

int fetchAddr(string varName, ST* symList) {//���ر�����ַ
	int len = getSTLen(symList);
	for (int i = 0; i < len; i++) {
		if (symList[i].varName == varName) {
			return symList[i].addr; //ֱ�ӷ��ص�ַ
		}
	}
	cout << "����δ������" << endl;
	return 0;
}

string fetchString(int key, ST* symList, int* mry) {//�����ַ���
	int len = symList[key].varLength;
	int addrr = symList[key].addr;
	char ans[len];
	for (int i = addrr, seq = 0; i < addrr + len; i++, seq++) {
		ans[seq] = mry[i];
	}
	string res;
	res += ans;
	return res;
}
int fetchInt(int key, ST* symList, int* mry) {
	return mry[symList[key].addr];
}
int fetchLen(string varName, ST* symList) {
	if (findVar(varName, symList) == -1) {
		cout << "����δ������" << endl;
		return -1;
	}
	else {
		int pos = findVar(varName, symList); //cout<<"ST��λ�ã�"<<pos<<endl;
		int len = symList[pos].varLength; //cout<<"ST�г��ȣ�"<<len<<endl;
		return len;
	}
}

string test(string elm, ST* sym, int* m) { //��ȫ�ֻ�ֲ����ű��в��ұ�����Ϣ
	if (lock == false) {
		int intElm = atoi(elm.c_str());
		if (intElm == 0 && elm != "0") { //���Ǵ����������Ҫȡֵ 
			if (findVar(elm, sym) < 0) {
				return elm; //��ST��Ҳ�Ҳ����ñ�������˵��elm��һ���µ��ַ��� 
			}
			else {    //˵�����Ѿ�������ST�ı������� 
				if (sym[findVar(elm, sym)].varType == "int") {
					return intToStr(fetchInt(findVar(elm, sym), sym, m));
				}
				else if (sym[findVar(elm, sym)].varType == "string") {
					return fetchString(findVar(elm, sym), sym, m);
				}
			}
		}
		else {  //�Ǵ�����������ֱ�ӷ��� 
			return elm;  //�������ַ�����ʽ�洢�Ĵ����� 
		}
	}
	else if (lock == true) { //������˵������ִ�к���������Ҫ��ȫ�ֻ�Ҫ���Ǿֲ� 
		int intElm = atoi(elm.c_str());
		if (intElm == 0 && elm != "0") { // ���Ǵ����������Ҫȡֵ 
			if (findVar(elm, funcAct.func_ST) < 0 && findVar(elm, symList) < 0) { //����ֲ���ȫ�� 
				return elm; //��ST��Ҳ�Ҳ����ñ�������˵��elm��һ���µ��ַ��� 
			}
			else {    //˵�����Ѿ�������ST�ı������� 
				if (findVar(elm, funcAct.func_ST) >= 0) {
					if (sym[findVar(elm, funcAct.func_ST)].varType == "int") {
						return intToStr(fetchInt(findVar(elm, funcAct.func_ST), funcAct.func_ST, funcAct.func_mry));
					}
					else if (sym[findVar(elm, funcAct.func_ST)].varType == "string") {
						return fetchString(findVar(elm, funcAct.func_ST), funcAct.func_ST, funcAct.func_mry);
					}
				}
				else if (findVar(elm, symList) >= 0) {
					if (symList[findVar(elm, symList)].varType == "int") {
						return intToStr(fetchInt(findVar(elm, symList), symList, mryList));
					}
					else if (symList[findVar(elm, symList)].varType == "string") {
						return fetchString(findVar(elm, symList), symList, mryList);
					}
				}
			}
		}
		else {
			return elm;
		}
	}
}
//if������ж�expression �ĶԴ��������������������������������������������������������������������������������������������������������� 
bool judgeExp(string exp, ST* symList, int* mry) {
	if (exp.find("==") != 18446744073709551615) {
		string elm1 = exp.substr(0, exp.find("=="));
		string elm2 = exp.substr(exp.find("==") + 2);
		elm1 = test(elm1, symList, mry);
		elm2 = test(elm2, symList, mry);
		if (elm1 == elm2) return true;
		else return false;
	}
	else if (exp.find(">") != 18446744073709551615) {
		string elm1 = exp.substr(0, exp.find(">"));
		string elm2 = exp.substr(exp.find(">") + 1);
		elm1 = test(elm1, symList, mry);
		elm2 = test(elm2, symList, mry);
		if ((atoi(elm1.c_str()) == 0 && elm1 != "0") && (atoi(elm2.c_str()) == 0 && elm2 != "0")) { //�ַ�����ֱ�Ӱ���ASSIC��Ƚ� 
			if (elm1 < elm2) return true;
			else return false;
		}
		else if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) { //�ɴ���ת�������ַ�����ת����int���Ƚ� 
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			if (e1 > e2) return true;
			else return false;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return false;
		}
	}
	else if (exp.find("<") != 18446744073709551615) {
		string elm1 = exp.substr(0, exp.find("<"));
		string elm2 = exp.substr(exp.find("<") + 1);
		elm1 = test(elm1, symList, mry);
		elm2 = test(elm2, symList, mry);
		if ((atoi(elm1.c_str()) == 0 && elm1 != "0") && (atoi(elm2.c_str()) == 0 && elm2 != "0")) { //�ַ�����ֱ�Ӱ���ASSIC��Ƚ� 
			if (elm1 < elm2) return true;
			else return false;
		}
		else if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) { //�ɴ���ת�������ַ�����ת����int���Ƚ� 
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			if (e1 < e2) return true;
			else return false;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return false;
		}
	}
}

string cul_Statement(string stm, ST* symList, int* mry) { //�������������ı��ʽ
	char op;
	int pos;
	for (int i = 0; i < stm.length(); i++) {
		if (stm[i] == '+') {
			op = '+';
			pos = i;
			break;
		}
		else if (stm[i] == '-') {
			op = '-';
			pos = i;
			break;
		}
		else if (stm[i] == '*') {
			op = '*';
			pos = i;
			break;
		}
		else if (stm[i] == '/') {
			op = '/';
			pos = i;
			break;
		}
		else if (stm[i] == '|') {
			op = '|';
			pos = i;
			break;
		}
		else if (stm[i] == '^') {
			op = '^';
			pos = i;
			break;
		}
	}

	if (!op) {
		//cout<<"�޼��㣡"<<endl; 
		return test(stm, symList, mry);
	}
	string elm1 = stm.substr(0, pos);
	string elm2 = stm.substr(pos + 1);
	elm1 = test(elm1, symList, mry);
	elm2 = test(elm2, symList, mry);
	switch (op)
	{
	case '+': {
		if ((atoi(elm1.c_str()) == 0 && elm1 != "0") && (atoi(elm2.c_str()) == 0 && elm2 != "0")) { //�����ַ�����ʹ���ַ�����+�� 
			string ans = elm1 + elm2;
			return ans;
		}
		else if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) { // �������Σ�ʹ�����ε�+�� 
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			string ans = intToStr(e1 + e2);
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	case '-': {
		if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) {
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			string ans = intToStr(e1 - e2);
			//cout<<"-�ķ���ֵ��"<<ans<<endl;
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	case '*': {
		if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) {
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			string ans = intToStr(e1 * e2);
			//				cout<<"*�ķ���ֵ��"<<ans<<endl;
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	case '/': {
		if (atoi(elm1.c_str()) != 0 && atoi(elm2.c_str()) != 0) {
			int e1 = atoi(elm1.c_str());
			int e2 = atoi(elm2.c_str());
			string ans = intToStr(e1 / e2);
			//				cout<<"/�ķ���ֵ��"<<ans<<endl;
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	case '|': {
		if ((atoi(elm1.c_str()) == 0 && elm1 != "0") && (atoi(elm2.c_str()) == 0 && elm2 != "0")) { //�����ַ�����ʹ���ַ�����+�� 
			int minLen = min(elm1.length(), elm2.length());
			char res[minLen];
			int i = 0;
			for (; i < minLen; i++) {
				res[i] = (elm1[i] > elm2[i]) ? elm1[i] : elm2[i];
			}
			string ans = res;
			if (elm1.length() != elm2.length()) {
				string back = (elm1.length() > elm2.length()) ? elm1.substr(i, elm1.length() - 1) : elm2.substr(i, elm2.length() - 1);
				ans = ans + back;
			}
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	case '^': {
		if ((atoi(elm1.c_str()) == 0 && elm1 != "0") && (atoi(elm2.c_str()) == 0 && elm2 != "0")) { //�����ַ�����ʹ���ַ�����+�� 
			int minLen = min(elm1.length(), elm2.length());
			char res[minLen];
			int i = 0;
			for (; i < minLen; i++) {
				res[i] = (elm1[i] < elm2[i]) ? elm1[i] : elm2[i];
			}
			string ans = res;
			if (elm1.length() != elm2.length()) {
				string back = (elm1.length() > elm2.length()) ? elm1.substr(i, elm1.length() - 1) : elm2.substr(i, elm2.length() - 1);
				ans = ans + back;
			}
			return ans;
		}
		else {
			cout << "�������Ͳ�ƥ�䣡" << endl;
			return "ERROR!";//���Ը���key�����Ż� 
		}
		break;
	}
	default: {
		cout << "statement����ģ����ϣ�" << endl;
		return "ERROR!";
		break;
	}
	}
}

void deal_Statement(string stm, ST* symList, int* mry) { //�����û���������
	int pos = -1;
	for (int i = 0; i < stm.length(); i++) {
		if (stm[i] == '=') {
			pos = i;
			break;
		}
	}
	if (pos == -1) {
		string ans = cul_Statement(stm, symList, mry);
		if (atoi(ans.c_str()) != 0 || ans == "0") {
			tmp.varType = "int";	//�洢�����м���� //ֻΪû�и�ֵ�����ļ��㣨�����������ã��Ľ�����üĴ���
		}
		else {
			tmp.varType = "string";
		}
		tmp.varValue = ans;
	}
	else { //������ʽ�к��С�=���������и�ֵ����
		string varName = stm.substr(0, pos);
		string operation = stm.substr(pos + 1);
		string ans = cul_Statement(operation, symList, mry);
		func_assignment(varName, ans, symList, mry); //������ֵ�ı�������������ֵ��ʽ�ӣ����ű��ڴ��
	}
}

void fetch(string varName, ST* symList, int* mry) {
	if (findVar(varName, symList) > -1) {
		//		cout<<"����ȡֵ��"<<endl;
		int key = findVar(varName, symList);
		if (symList[key].varType == "int") {
			int res = fetchInt(key, symList, mry);
			cout << res << endl << endl;
		}
		else if (symList[key].varType == "string") {
			string str = fetchString(key, symList, mry);
			cout << str << endl << endl;
		}
	}
}

vector<string> writeVector(vector<string> text) {
	text.clear();
	string content;

	while (1) {
		getline(cin, content);
		if (content != "endDefine") {
			text.push_back(content);
		}
		else {
			break;
		}
	}
	return text;
}

void deal_if(vector<string> text, ST* symList, int* mry) {  //Ԥ�������text�Ѿ����ָ���� 
	string cmd1 = text[0];
	string exp = cmd1.substr(3, cmd1.length() - 4);
	string statement1 = text[1];
	string cmd3 = text[2];
	bool flag;
	if (cmd3 == "else") {
		string cmd4 = text[3];
		string cmd5 = text[4];
		if (cmd5 != "endif") {
			cout << "if����﷨������endif��β��" << endl << endl;
		}
		else {
			string statement2 = cmd4;
			flag = judgeExp(exp, symList, mry);
			if (flag == true) {
				deal_Statement(statement1, symList, mry);
			}
			else {
				deal_Statement(statement2, symList, mry);
			}
		}
	}
	else if (cmd3 == "endif") {
		flag = judgeExp(exp, symList, mry);
		if (flag == true) {
			deal_Statement(statement1, symList, mry);
		}
		else {
		}
	}
	else {
		cout << "if�﷨����" << endl;
	}
}
void deal_while(vector<string> text, ST* symList, int* mry) {
	string cmd1 = text[0];
	string exp = cmd1.substr(6, cmd1.length() - 7);
	string statement = text[1];
	string cmd3 = text[2];
	bool flag;
	if (cmd3 != "endWhile") {
	}
	else {
		while (1) {
			if (judgeExp(exp, symList, mry) == false) break;
			else if (judgeExp(exp, symList, mry) == true) {
				deal_Statement(statement, symList, mry);
			}
		}
	}
}
//��������ǰ������
int find_func(string str, char c) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == c) {
			return i;
		}
	}
	return -1;
}

void call_func(vector<string> line_call);
//********************************** �������õ��������� ********************************** 
void deal_Line(string line, ST* symList, int* mry) {

	vector<string> text = split(line, " ");
	if (text.size() == 1 && text[0] == "symtab") {
		symctl(symList);
	}
	else if (text.size() == 1 && text[0] == "memory") {
		memory(mry);
	}
	else if (text.size() == 1 && text[0] == "tmptab") {
		showTmp();
	}
	else if (text.size() == 2 && text[0] == "&") {
		cout << fetchAddr(text[1], symList) << endl << endl;
	}
	else if (text[0].substr(0, 5) == "size(" && text[0][text[0].length() - 1] == ')') {
		string varName = text[0].substr(5, 1);
		cout << fetchLen(varName, symList) << endl << endl;
	}
	else if (text.size() == 2 && text[1] == "?") {
		fetch(text[0], symList, mry);
	}
	else if (text.size() == 2 && (text[0] == "string" || text[0] == "int")) {
		func_declear(text[0], text[1], symList);
	}

	else if (text.size() == 1) {
		deal_Statement(text[0], symList, mry);
	}

	else if (text.size() == 3 && text[1] == "=") {
		if (find(text[2].begin(), text[2].end(), '(') != text[2].end()) {
		}
		else func_assignment(text[0], text[2], symList, mry);
	}

	else if (text[0].substr(0, 2) == "if") {
		deal_if(text, symList, mry);
	}

	else if (text[0].substr(0, 5) == "while") {
		deal_while(text, symList, mry);
	}

	else if (text.size() == 1 && split(text[0], "(").size() == 2) {
		call_func(text);

	}

	else if (text.size() == 2 && text[0] == "return") {
		string ans = cul_Statement(text[1], symList, mry);
		if (ans != "ERROR!") {
			string ansType;
			if (atoi(ans.c_str()) == 0 && ans != "0") ansType = "string";
			else ansType = "int";
			if (ansType != funcAct.returnType) cout << "����ֵ�����뺯��������ƥ�䣡" << endl;
			else {
				funcAct.returnVar = ans;
			}
		}
	}
}

void deal_func(vector<string> text, ST* symList, int* mry) {
	for (int j = 0; j < text.size(); j++) {
		string s = text[j];
		deal_Line(s, symList, mry);
		cout << endl;
	}
}

vector<string> text;
void show_func() {
	for (int k = 0; k < text.size(); k++) {
		cout << text[k] << endl;
	}
}
void showReturn() {
}
void showReturn();
void eraseFuncVar() {
	for (int i = 0; i < SIZE_OF_SYM_FUNC; i++) {
		if (funcAct.func_ST[i].varName != "") {
			funcAct.func_ST[i].varName = "";
			funcAct.func_ST[i].varLength = 0;
			funcAct.func_ST[i].addr = 0;
			funcAct.func_ST[i].varType = "";
		}
	}

	for (int j = 0; j < SIZE_OF_MRY_FUNC; j++) {
		if (funcAct.func_mry[j] != 0) {
			funcAct.func_mry[j] = 0;
		}
	}
}

void call_func(vector<string> line_call) { //�û����к�������
	int pos = find_func(line_call[0], '(');
	string funcName = line_call[0].substr(0, pos);
	if (findVar(funcName, symList) > -1) {
		eraseFuncVar();
		string elmAll = line_call[0].substr(pos + 1);
		elmAll = elmAll.substr(0, elmAll.length() - 1);
		vector<string> elm = split(elmAll, ",");
		string elm1 = test(elm[0], symList, mryList);
		string elm2 = test(elm[1], symList, mryList);
		string elm1Type = (atoi(elm1.c_str()) != 0) ? "int" : "string";
		string elm2Type = (atoi(elm2.c_str()) != 0) ? "int" : "string";
		string funcType_ST = symList[findVar(funcName, symList)].varType;
		vector<string> typeArr = split(funcType_ST, ",");
		string returnType = typeArr[2].substr(0, typeArr[2].length() - 1);
		string funcType_real = "func(" + elm1Type + "," + elm2Type + "," + returnType + ")";
		if (funcType_ST != funcType_real) cout << "�������Ͳ�ƥ�䣡" << endl;
		else {
			func_declear(elm1Type, funcAct.funcVar1, funcAct.func_ST);
			func_assignment(funcAct.funcVar1, elm1, funcAct.func_ST, funcAct.func_mry);

			func_declear(elm2Type, funcAct.funcVar2, funcAct.func_ST);
			func_assignment(funcAct.funcVar2, elm2, funcAct.func_ST, funcAct.func_mry);
			lock = true;
			deal_func(text, funcAct.func_ST, funcAct.func_mry);
			lock = false;
			showReturn();
		}
	}
	else {
		cout << "û�������ú���!" << endl;
	}
}

void writeFunc() { //�û��Զ��庯��
	string funcType[3]; //�����洢�������� 
	string cmd2;
	cin >> cmd2;
	funcAct.funcName = cmd2; //����Ĵ���
	int keyInST;
	for (int i = 0; i < SIZE_OF_SYM_FUNC; i++) {
		if (symList[i].varName == "") {
			symList[i].varName = cmd2;
			symList[i].addr = 10001; //����洢���Ķε��ļ� 
			keyInST = i;
			break;
		}
	}
	string cmd3;
	cin >> cmd3;
	if (cmd3 == "returnType") { //returnType��ʾ������������
		string cmd4;
		cin >> cmd4;
		if (cmd4 == "int" || cmd4 == "string") {
			funcAct.returnType = cmd4;
			funcType[0] = cmd4;
			//�����βα�
			string var1Type; cin >> var1Type; funcType[1] = var1Type;
			string var1Name; cin >> var1Name; funcAct.funcVar1 = var1Name;
			string var2Type; cin >> var2Type; funcType[2] = var2Type;
			string var2Name; cin >> var2Name; funcAct.funcVar2 = var2Name;
			symList[keyInST].varType = "func(" + funcType[1] + "," + funcType[2] + "," + funcType[0] + ")";
			cout << "#System notice --- The type of this function:" << symList[keyInST].varType << endl;
			text = writeVector(text);
			text.erase(text.begin());
		}
		else {
			cout << "����ֵ���ͱ���Ϊint��string��" << endl;
		}
	}
	else {
		cout << "returnType�﷨����" << endl;
	}
}

void eraseFuncInST() {
	for (int i = 0; i < SIZE_OF_SYM; i++) {
		if (split(symList[i].varType, ",").size() == 3 && symList[i].varLength == 0) {
			symList[i].varName = "";
			break;
		}
	}
}

int whereFunc(string str) {
	int pos = find_func(str, '(');
	string funcName = str.substr(0, pos);
	int posInST = findVar(funcName, symList);
	if (posInST > -1) {
		return posInST;
	}
	else {
		return -1;
	}
}


void controller() {
	while (1) {
		string cmd1;
		cin >> cmd1;
		if (cmd1 == "define") { //�û����к�������
			eraseFuncInST();
			writeFunc();
		}
		else if (cmd1 == "exit") break;

		else if (cmd1 == "int" || cmd1 == "string") { //�û����б�������
			declear(cmd1, symList);
		}

		else if (cmd1 == "symtab") { //�û��鿴���ű�
			symctl(symList);
		}
		else if (cmd1 == "funcacts") { //�û��鿴�ֲ�����������ʱ�����ű�
			symctl(funcAct.func_ST);
		}
		else if (cmd1 == "memory") { //�û��鿴�ڴ��
			memory(mryList);
		}
		else if (cmd1 == "funcactm") { //�û��鿴�ֲ�����������ʱ���ڴ��
			funcAct_memory(funcAct.func_mry);
		}
		else if (cmd1 == "tmptab") { //�û��鿴�Ĵ�������
			showTmp();
		}
		else if ((findVar(cmd1, symList)) > -1) {
			int key = findVar(cmd1, symList);
			string cmd2;
			cin >> cmd2;
			if (cmd2 == "=") {
				assignment(key, symList, mryList);

			}
			else if (cmd2 == "?") { //�û��鿴���
				if (symList[key].varType == "int") {
					int res = fetchInt(key, symList, mryList);
					cout << res << endl << endl;
				}
				else if (symList[key].varType == "string") {
					string str = fetchString(key, symList, mryList);
					cout << str << endl << endl;
				}
			}
			else if (cmd2 == "+" || cmd2 == "-" || cmd2 == "*" || cmd2 == "/" || cmd2 == "|" || cmd2 == "^") {
				string cmd3;
				cin >> cmd3;
				string statement = cmd1 + cmd2 + cmd3;
				deal_Statement(statement, symList, mryList);
			}
		}
		else if (cmd1[0] == '&') {
			string varName = cmd1.substr(1);
			cout << fetchAddr(varName, symList) << endl << endl;
		}
		else if (cmd1.substr(0, 5) == "size(" && cmd1[cmd1.length() - 1] == ')') {
			cout << "ȡ����" << endl;
			int end = cmd1.length() - 6;
			string varName = cmd1.substr(5, end);
			cout << fetchLen(varName, symList) << endl << endl;
		}
		else if (cmd1.substr(0, 3) == "if(" && cmd1[cmd1.length() - 1] == ')') {
			string exp = cmd1.substr(3, cmd1.length() - 4);
			string cmd2;
			cin >> cmd2;
			string statement1 = cmd2;
			string cmd3;
			cin >> cmd3;
			bool flag;
			if (cmd3 == "endif") {
				flag = judgeExp(exp, symList, mryList);
				if (flag == true) {
					deal_Statement(statement1, symList, mryList);
				}
				else {
				}
			}
			else if (cmd3 == "else") {
				string cmd4; cin >> cmd4;
				string cmd5; cin >> cmd5;
				if (cmd5 != "endif") {
					cout << "if����﷨������endif��β��" << endl << endl;
					continue;
				}
				else {
					string statement2 = cmd4;
					flag = judgeExp(exp, symList, mryList);
					if (flag == true) {
						deal_Statement(statement1, symList, mryList);
					}
					else {
						deal_Statement(statement2, symList, mryList);
					}
				}
			}
			else {
				cout << "if���ľ䷨����" << endl;
			}
		}
		else if (cmd1.substr(0, 5) == "while") {
			string cmd2; cin >> cmd2;
			string cmd3; cin >> cmd3;
			vector<string> testWhile;
			testWhile.push_back(cmd1);
			testWhile.push_back(cmd2);
			testWhile.push_back(cmd3);
			deal_while(testWhile, symList, mryList);
		}

		else if (split(cmd1, "(").size() == 2 && split(cmd1, ",").size() == 2 && whereFunc(cmd1) != -1) {//����ʹ�üĴ��� 
			lock = true;
			vector<string> testify;
			testify.push_back(cmd1);
			call_func(testify);
			tmp.varValue = funcAct.returnVar;
			tmp.varType = funcAct.returnType;
			lock = false;
		}

		else if (cmd1 != "") {
			string cmd2;
			cin >> cmd2;
			if (cmd2 == "+" || cmd2 == "-" || cmd2 == "*" || cmd2 == "/" || cmd2 == "|" || cmd2 == "^") {
				string cmd3;
				cin >> cmd3;
				string statement = cmd1 + cmd2 + cmd3;
				deal_Statement(statement, symList, mryList);
			}
			else if (cmd2 == "=") {
				string cmd3;
				cin >> cmd3;
				cout << "����!" << cmd1 << "δ������" << endl;
				continue;
			}
		}
		else {
			cout << "synax error" << endl;
		}
	}
}

int main() {

	controller();
	cout << "The End!" << endl;
	return 0;
}



