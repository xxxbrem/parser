#include "lexicalAnalysis.h"
#include <fstream>

// 记录当前行号
// int lexicalLine = 0;

void Lexical::writeErr(string s) {
	ofstream fout(fileName + ".err", ios::app);
	fout << s << endl;
	fout.close();
}

// 补齐位数至16位并检查长度是否溢出
string Lexical::checkLen(string tmp) {
	if (tmp.size() > 16) {
		string s = "LINE: " + to_string(line) + "  identifier or number length overflow, expect length <= 16, but " + tmp + " length is " + to_string(tmp.size());
		writeErr(s);
		return "";
	}
	else {
		string s = "";
		for (int i = 0; i < 16 - tmp.size(); i++) {
			s += " ";
		}
		s += tmp;
		return s;
	}
}


// 分析关键字和用户自定义标识符的函数
Token Lexical::lexWord() {
	Token token;
	int i;
	// 在第一个字符为字母的情况下，若后续为数字或字母，则视为一个单词
	for (i = 0; isdigit(*ch) || isalpha(*ch); ch++) {
		temp[i++] = *ch;
	}
	temp[i] = '\0';
	string s = (string)temp;
	// s在keyword中
	if (keyword.find(s) != keyword.end()) {
		token.wordSymbol = checkLen(s);
		token.wordType = keyword[s];
	}
	// s不在keyword中，即为标志符
	else {
		token.wordSymbol = checkLen(s);
		token.wordType = "10";
	}
	return token;
}


// 分析整数的函数
Token Lexical::lexDigit() {
	int i;
	Token token;
	for (i = 0; isdigit(*ch); ch++) {
		temp[i++] = *ch;
	}
	temp[i] = '\0';
	token.wordSymbol = checkLen(temp);
	token.wordType = "11";
	return token;
}


// 分析运算符的函数，包含=, <>, <=, <, >=, >, -, *, :=
Token Lexical::lexOperators() {
	Token token;
	if (*ch == '<' || *ch == '>' || *ch == ':') {
		// >=, <=, :=
		if (*(ch + 1) == '=') {
			temp[0] = *ch;
			temp[1] = *(++ch);
			temp[2] = '\0';
		}
		// 冒号不匹配
		else if (*ch == ':') {
			string s = "LINE: " + to_string(line) + "  mismatch ':', expect '=' after ':'";
			writeErr(s);
			temp[0] = 'e';
			temp[1] = 'r';
			temp[2] = 'r';
			temp[3] = '\0';
		}
		// <>
		else if (*ch == '<' && *(ch + 1) == '>') {
			temp[0] = *ch;
			temp[1] = *(++ch);
			temp[2] = '\0';
		}
		// <, >
		else {
			temp[0] = *ch;
			temp[1] = '\0';
		}
		token.wordSymbol = checkLen(temp);
		token.wordType = operators[temp];
	}
	// =, -, *
	else if (*ch == '=' || *ch == '-' || *ch == '*') {
		temp[0] = *ch;
		temp[1] = '\0';
		token.wordSymbol = checkLen(temp);
		token.wordType = operators[temp];
	}
	ch++;
	return token;
}


// 分析界符（括号，分号）的函数
Token Lexical::lexDelimiter() {
	temp[0] = *(ch++);
	temp[1] = '\0';
	string s = (string)temp;
	Token token;
	if (delimiters.find(s) != delimiters.end()) {
		token.wordSymbol = checkLen(s);
		token.wordType = delimiters[s];
	}
	return token;
}

vector<Token> Lexical::lexSource() {
	vector<Token> result;
	while (*ch != '\0') {
		// 判断是否是空格
		while (isspace(*ch)) {
			ch++;
		}
		// 判断是否是字母
		if (isalpha(*ch)) {
			result.push_back(lexWord());
		}
		// 判断是否是数字
		else if (isdigit(*ch)) {
			result.push_back(lexDigit());
		}
		// 判断是否是运算符
		else if (operate.find(*ch) != string::npos) {
			result.push_back(lexOperators());
		}
		// 判断是否是界符
		else if (delimiter.find(*ch) != string::npos) {
			result.push_back(lexDelimiter());
		}
		// 非法字符
		else {
			string s = "LINE: " + to_string(line) + "  illegal character: " + *ch;
			writeErr(s);
			ch++;
		}
	}
	return result;
}

/*
int main() {
	string name;
	cout << "Input file path + name: ";
	cin >> name;
	// 读入代码
	ifstream in(name + ".pas");
	// 写词法分析
	ofstream off(name + ".dyd");
	// 初始化.err
	ofstream err(name + ".err");
	err.close();

	string buffer;
	vector<vector<Token>> all;
	while (getline(in, buffer)) {
		lexicalLine++;
		char* ch = (char*)buffer.c_str();
		Lexical lex(ch);
		lex.fileName = name;
		vector<Token> s = lex.lexSource();
		for (int i = 0; i < s.size(); i++) {
			off << s[i].wordSymbol << ' ' << s[i].wordType << endl;
		}
		// 每行加入ELON
		off << "            ELON" << ' ' << "24" << endl;
		all.push_back(s);
	}
	// 结尾加入EOF
	off << "             EOF" << ' ' << "25" << endl;
	in.close();
	off.close();
	return 0;
}
*/