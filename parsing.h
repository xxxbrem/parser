#pragma once

#include<iostream>
#include"lexicalAnalysis.h"
#include<stack>

using namespace std;

// �﷨������
class SynaticParser{
	public:
		// index��ָ��phrases��λ��
		int index;
		// ������
		vector<Token> phrases;
		// numOfPhrases�����ʵ�����
		int numOfPhrases;
		// ��ǰ����
		Token sym;

		// ��ʼ��
		SynaticParser(vector<Token> phrase) {
			index = 0;
			this->phrases = phrase;
			this->numOfPhrases = this->phrases.size();
			this->sym = phrase[index];
			vproc.push("main");
		}

		~SynaticParser(){}

		// ��ʼ��
		void init();
		// ��ת��һ��
		void toNext();
		// ���λ��
		string checkLen(string tmp);
		// �ļ���
		string fileName;
		// д�ļ�
		void writePro(vector<vector<string>> v);
		void writeVar(vector<vector<string>> v);
		void writeErr(string s);
		void writeDys();
		void write();
		// �����
		vector<string> errTable;
		// ��������
		vector<vector<string>> varTable;
		
		// ����procTable
		void UpdatePro();

		//�����ڱ������е�λ��
		int vadr = 0;
		// ��ǰ����
		int lineCount = 1;

		// һ�б�һ����
		bool lineFlag[1000] = {0};
		// �����һ������
		string nextWord();
		// �洢������
		stack<string> vproc;
		// �������
		int vlev = 0;
		// ��������
		vector<vector<string>> procTable;
		// ������
		string pname = "main";
		// ���̲��
		int plev = 0;
		// ��һ������
		string firstVar = "";
		// ���һ������
		string lastVar = "";
		// �����жϱ����Ƿ��ظ�
		bool procFlag = false;
		// ��һ�������ڱ������е�λ��
		string fadr = "";
		// ���һ�������ڱ������е�λ��
		string ladr = "";

		// �﷨��������ǰ�ƶ�һ������
		void Advance();

		// <����>��<�ֳ���>
		void MainProcedure(); // ����

		// <�ֳ���>��begin <˵������>;<ִ������> end
		void SubProcedure(); // �ֳ���

		// <˵������>��<˵�����><˵������A>
		// <˵������A>��; <˵�����><˵������A>����
		void DeclareStatementTable(); // ˵������
		void DeclareStatementTableA(); // ˵������A
		void DeclareStatement(); // ˵�����
		
		// <˵�����>��<����˵��>��<����˵��>
		void FunctionDeclare(); // ����˵��
		void VarDeclare(); // ����˵�� // <����˵��>��integer <����>
		
		// <����˵��>��integer function <��ʶ��>(<����>);<������>
		string Identifier(); // ��ʶ��
		void Parameter(); // ���� 
		void FunctionBody(); // ������ // <������>��begin <˵������>;<ִ������> end

		// <����>��<����>
		void Variable(int vkind); // ����

		// <ִ������>��<ִ�����><ִ������A>
		// <ִ������A>��;<ִ�����><ִ������A>����
		void ExecuteStatementTable(); // ִ������
		void ExecuteStatement(); // ִ�����
		void ExecuteStatementTableA(); // ִ������A

		// <ִ�����>��<�����>��<д���>��<��ֵ���>��<�������>
		void ReadStatement(); // ����� // <�����>��read(<����>)
		void WriteStatement(); // д��� // <д���>��write(<����>)
		void AssignStatement(); // ��ֵ��� // <��ֵ���>��<����>:=<�������ʽ>
		void ConditionStatement(); // ������� 

		// <�������ʽ>��<��><�������ʽA>
		// <�������ʽA>��-<��><�������ʽA>|��
		void MathExpression(); // �������ʽ
		void Term(); // ��
		void MathExpressionA(); // �������ʽA

		// <��>��<����><��A>
		// <��A>��*<����><��A>����
		void Factor(); // ���� 
		void TermA(); // ��A // <��A>��*<����><��A>����

		// <����>��<����>��<����>��<��������>
		void Constant(); // ����
		void FunctionCall(); // �������� // <��������>��<��ʶ��>(<�������ʽ>)

		// <����>��<�޷�������>
		void UnsignedInteger();

		// <�������>��if<�������ʽ>then<ִ�����>else<ִ�����>
		void ConditionExpression();

		// <�������ʽ>��<�������ʽ><��ϵ�����><�������ʽ>
		void RelationOperator(); // ��ϵ����� // <��ϵ�����>��<��<=��>��>=��=��<>
};