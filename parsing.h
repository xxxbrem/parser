#pragma once

#include<iostream>
#include"lexicalAnalysis.h"
#include<stack>

using namespace std;

// 语法分析器
class SynaticParser{
	public:
		// index：指向phrases的位置
		int index;
		// 单词流
		vector<Token> phrases;
		// numOfPhrases：单词的数量
		int numOfPhrases;
		// 当前单词
		Token sym;

		// 初始化
		SynaticParser(vector<Token> phrase) {
			index = 0;
			this->phrases = phrase;
			this->numOfPhrases = this->phrases.size();
			this->sym = phrase[index];
			vproc.push("main");
		}

		~SynaticParser(){}

		// 初始化
		void init();
		// 跳转下一行
		void toNext();
		// 填充位数
		string checkLen(string tmp);
		// 文件名
		string fileName;
		// 写文件
		void writePro(vector<vector<string>> v);
		void writeVar(vector<vector<string>> v);
		void writeErr(string s);
		void writeDys();
		void write();
		// 报错表
		vector<string> errTable;
		// 变量名表
		vector<vector<string>> varTable;
		
		// 更新procTable
		void UpdatePro();

		//变量在变量表中的位置
		int vadr = 0;
		// 当前行数
		int lineCount = 1;

		// 一行报一个错
		bool lineFlag[1000] = {0};
		// 获得下一个单词
		string nextWord();
		// 存储过程名
		stack<string> vproc;
		// 变量层次
		int vlev = 0;
		// 过程名表
		vector<vector<string>> procTable;
		// 过程名
		string pname = "main";
		// 过程层次
		int plev = 0;
		// 第一个变量
		string firstVar = "";
		// 最后一个变量
		string lastVar = "";
		// 用于判断变量是否重复
		bool procFlag = false;
		// 第一个变量在变量表中的位置
		string fadr = "";
		// 最后一个变量在变量表中的位置
		string ladr = "";

		// 语法分析器向前移动一个单词
		void Advance();

		// <程序>→<分程序>
		void MainProcedure(); // 程序

		// <分程序>→begin <说明语句表>;<执行语句表> end
		void SubProcedure(); // 分程序

		// <说明语句表>→<说明语句><说明语句表A>
		// <说明语句表A>→; <说明语句><说明语句表A>│空
		void DeclareStatementTable(); // 说明语句表
		void DeclareStatementTableA(); // 说明语句表A
		void DeclareStatement(); // 说明语句
		
		// <说明语句>→<变量说明>│<函数说明>
		void FunctionDeclare(); // 函数说明
		void VarDeclare(); // 变量说明 // <变量说明>→integer <变量>
		
		// <函数说明>→integer function <标识符>(<参数>);<函数体>
		string Identifier(); // 标识符
		void Parameter(); // 参数 
		void FunctionBody(); // 函数体 // <函数体>→begin <说明语句表>;<执行语句表> end

		// <参数>→<变量>
		void Variable(int vkind); // 变量

		// <执行语句表>→<执行语句><执行语句表A>
		// <执行语句表A>→;<执行语句><执行语句表A>│空
		void ExecuteStatementTable(); // 执行语句表
		void ExecuteStatement(); // 执行语句
		void ExecuteStatementTableA(); // 执行语句表A

		// <执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
		void ReadStatement(); // 读语句 // <读语句>→read(<变量>)
		void WriteStatement(); // 写语句 // <写语句>→write(<变量>)
		void AssignStatement(); // 赋值语句 // <赋值语句>→<变量>:=<算术表达式>
		void ConditionStatement(); // 条件语句 

		// <算术表达式>→<项><算术表达式A>
		// <算术表达式A>→-<项><算术表达式A>|空
		void MathExpression(); // 算术表达式
		void Term(); // 项
		void MathExpressionA(); // 算术表达式A

		// <项>→<因子><项A>
		// <项A>→*<因子><项A>│空
		void Factor(); // 因子 
		void TermA(); // 项A // <项A>→*<因子><项A>│空

		// <因子>→<变量>│<常数>│<函数调用>
		void Constant(); // 常数
		void FunctionCall(); // 函数调用 // <函数调用>→<标识符>(<算数表达式>)

		// <常数>→<无符号整数>
		void UnsignedInteger();

		// <条件语句>→if<条件表达式>then<执行语句>else<执行语句>
		void ConditionExpression();

		// <条件表达式>→<算术表达式><关系运算符><算术表达式>
		void RelationOperator(); // 关系运算符 // <关系运算符>→<│<=│>│>=│=│<>
};