#include "merge-1.cpp"
#include "merge-2.cpp"
#include "merge-3.cpp"
//������-�����������"/FORCE:MULTIPLE "�Ժ���LNK2005����
#include <iostream>
#include <windows.h>
using namespace std;
int main() {
	//���"option.txt"����
	string cont;
	int ch, line = 0;
	FILE* option = fopen("config\\options.txt", "r");
	do {
		ch = fgetc(option);
		if (ch == '\n')		line++;
	} while (ch != EOF);
	rewind(option);		//ָ�뷵�ؿ�ͷ
	char temp1[100];
	string temp2="";
	for (int i = 1; i <= line*2; i++) {
		fgets(temp1, 101, option);	//�������ݲ�����100��
		if (temp2 != temp1) {	//ȥ��
			cout << temp1;
			temp2 = temp1;
		}
	}
	cout << "�����룺";
	char input=getchar();
	cout << endl;
	if (input == '1')	merge1();
	if (input == '2')	merge2();
	if (input == '3') {
		merge3();
		merge1();
		merge2();
	}
	system("pause");
	return 0;
}