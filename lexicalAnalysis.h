#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <string>

using namespace std;

// Token类：包含单词符号（wordSymbol）和种别（wordType）
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

// 词法分析器类
class Lexical {
	public:
		// 操作符
		const string operate = "-*=<>:";

		// 界符
		const string delimiter = "();";
		
		// 哈希表来存储单词符号与种别对照表
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

		// 记录当前行号
		int line = 0;

		string fileName;

		void writeErr(string s);

		// token缓存
		char temp[1000] = {};

		// 分析字符串的指针
		char* ch = NULL;

		// 分析运算符的函数，包含=, <>, <=, <, >=, >, -, *, :=
		Token lexOperators();

		// 分析关键字和用户自定义标识符的函数
		Token lexWord();

		// 分析数字的函数
		Token lexDigit();

		// 分析界符（括号，分号）的函数
		Token lexDelimiter();

		// 补齐位数至16位并检查长度是否溢出
		string checkLen(string s);

		// 构造函数
		Lexical(char* ch) { this->ch = ch; } 

		vector<Token> lexSource();
};