#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// Token�ࣺ�������ʷ��ţ�wordSymbol�����ֱ�wordType��
class Token {
	public:
		string wordSymbol;
		string wordType;
		Token() {}
		Token(string wordSymbol, string wordType) {
			this->wordSymbol = wordSymbol;
			this->wordType = wordType;
		}
};

// �ʷ���������
class Lexical {
	public:
		// ������
		const string operate = "-*=<>:";

		// ���
		const string delimiter = "();";
		
		// ��ϣ�����洢���ʷ������ֱ���ձ�
		unordered_map<string, string> keyword{
			{"begin","01"},{"end","02"},{"integer","03"},
			{"if","04"},{"then","05"},{"else","06"},
			{"function","07"},{"read", "08"},{"write","09"}
		};
		unordered_map<string, string> operators{
			{"=","12"},{"<>","13"},{"<=","14"},
			{"<","15"},{">=","16"},{">","17"},
			{"-","18"},{"*","19"},{":=","20"}
		};
		unordered_map<string, string> delimiters{
			{"(","21"},{")","22"},{";","23"}
		};

		// ��¼��ǰ�к�
		int line = 0;

		string fileName;

		void writeErr(string s);

		// token����
		char temp[1000] = {};

		// �����ַ�����ָ��
		char* ch = NULL;

		// ����������ĺ���������=, <>, <=, <, >=, >, -, *, :=
		Token lexOperators();

		// �����ؼ��ֺ��û��Զ����ʶ���ĺ���
		Token lexWord();

		// �������ֵĺ���
		Token lexDigit();

		// ������������ţ��ֺţ��ĺ���
		Token lexDelimiter();

		// ����λ����16λ����鳤���Ƿ����
		string checkLen(string s);

		// ���캯��
		Lexical(char* ch) { this->ch = ch; } 

		vector<Token> lexSource();
};