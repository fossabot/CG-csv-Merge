//������-�����������"/FORCE:MULTIPLE "�Ժ���LNK2005����
#include <iostream>
#include <windows.h>
using namespace std;
void merge3() {
	//�������˵��
	string cont;
	int ch, line = 0;
	FILE* help3 = fopen("config\\help-merge-3.txt", "r");
	do {
		ch = fgetc(help3);
		if (ch == '\n')		line++;
	} while (ch != EOF);
	rewind(help3);		//ָ�뷵�ؿ�ͷ
	char temp1[100];
	string temp2 = "";
	for (int i = 1; i <= line * 2; i++) {
		fgets(temp1, 101, help3);	//�������ݲ�����100��
		if (temp2 != temp1) {	//ȥ��
			cout << temp1;
			temp2 = temp1;
		}
	}
	system("pause");
}