#include "lexicalAnalysis.h"
#include "parsing.h"
#include <fstream>

using namespace std;

// 记录当前行号
int lexicalLine = 0;

int main() {
	// Lexical
	string name;
	cout << "Input file path + name: ";
	cin >> name;
	// 读入代码
	ifstream pasIn(name + ".pas");
	// 写词法分析
	ofstream dydOut(name + ".dyd");
	// 初始化.err
	ofstream err1(name + ".err");
	err1.close();

	string buffer1;
	vector<vector<Token>> all;
	while (getline(pasIn, buffer1)) {
		lexicalLine++;
		char* ch = (char*)buffer1.c_str();
		Lexical lex(ch);
		lex.fileName = name;
		lex.line = lexicalLine;
		vector<Token> s = lex.lexSource();
		for (int i = 0; i < s.size(); i++) {
			dydOut << s[i].wordSymbol << ' ' << s[i].wordType << endl;
		}
		// 每行加入ELON
		dydOut << "            ELON" << ' ' << "24" << endl;
		all.push_back(s);
	}
	// 结尾加入EOF
	dydOut << "             EOF" << ' ' << "25" << endl;
	pasIn.close();
	dydOut.close();

	ifstream err(name + ".err");
	char ch;
	err >> ch;
	if (!err.eof()) {
		cout << "Lexical analysis error, procedure exited." << endl;
		return 0;
	}

	// Parser
	ifstream dydIn(name + ".dyd");
	// 初始化
	ofstream errOut(name + ".err");
	errOut.close();
	ofstream varOut(name + ".var");
	varOut.close();
	ofstream proOut(name + ".pro");
	proOut.close();

	string buffer2;
	vector<Token> tokens;
	// 读取dyd文件
	while (getline(dydIn, buffer2)) {
		Token tmp;
		buffer2.erase(0, buffer2.find_first_not_of(" "));
		int space = buffer2.find_first_of(" ");
		tmp.wordSymbol = buffer2.substr(0, space);
		tmp.wordType = buffer2.substr(space + 1, buffer2.length());
		tokens.push_back(tmp);
	}
	dydIn.close();
	SynaticParser parser(tokens);
	parser.fileName = name;
	parser.MainProcedure();
	parser.write();

	ifstream err2(name + ".err");
	err2 >> ch;
	if (err2.eof()) {
		parser.writeDys();
	}
	return 0;
}