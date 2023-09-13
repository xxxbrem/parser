#include "parsing.h"
#include <fstream>
#include <string>

#define IdentifierType "10"
#define DigitType "11"
#define FormalParameter 1
#define ActualParameter 0
#define UseParameter -1
#define ptype "integer"
#include <stack>

void SynaticParser::init() {
	stack<string> vproc;
	stack<string>().swap(vproc);
	vproc.push("main");
	errTable.clear();
	varTable.clear();
	vadr = 0;
	lineCount = 1;
	vlev = 0;
	procTable.clear();
	pname = "";
	plev = 0;
	firstVar = "";
	lastVar = "";
	procFlag = false;
	fadr = "";
	ladr = "";
	numOfPhrases = phrases.size();
	index = 0;
	sym = phrases[index];
	ofstream varOut(fileName + ".var");
	varOut.close();
	ofstream proOut(fileName + ".pro");
	proOut.close();
}

void SynaticParser::toNext() {
	while (sym.wordSymbol.compare(";") and index < numOfPhrases) {
		index++;
	}
}

// 补齐位数至16位
string SynaticParser::checkLen(string tmp) {
	string s = "";
	for (int i = 0; i < 16 - tmp.size(); i++) {
		s += " ";
	}
	s += tmp;
	return s;
}

void SynaticParser::writePro(vector<vector<string>> v) {
	ofstream fout(fileName + ".pro", ios::out);
	for (int i = 0; i < v.size(); i++) {
		string a = "";
		for (int j = 0; j < v[i].size(); j++) {
			a += v[i][j];
			a += " ";
		}
		fout << a << endl;
	}
	fout.close();
}

void SynaticParser::writeVar(vector<vector<string>> v) {
	ofstream fout(fileName + ".var", ios::out);
	for (int i = 0; i < v.size(); i++) {
		string a = "";
		for (int j = 0; j < v[i].size(); j++) {
			a += v[i][j];
			a += " ";
		}
		fout << a << endl;
	}
	fout.close();
}

void SynaticParser::writeErr(string s) {
	if (!lineFlag[lineCount]) {
		ofstream fout(fileName + ".err", ios::app);
		fout << "LINE: " << lineCount << s << endl;
		fout.close();
		lineFlag[lineCount] = 1;
	}
}

void SynaticParser::writeDys() {
	ofstream fout(fileName + ".dys", ios::out);
	ifstream fin(fileName + ".dyd");
	string buffer;
	while (getline(fin, buffer)) {
		fout << buffer << endl;
	}
	fin.close();
	fout.close();
}


void SynaticParser::write() {
	writeVar(varTable);
	writePro(procTable);
}

void SynaticParser::Advance() {
	index++;
	// 检测越界
	if (index < numOfPhrases) {
		sym = phrases[index];
	}
	if (!sym.wordSymbol.compare("ELON")) {
		lineCount++;
		index++;
		if (index < numOfPhrases) {
			sym = phrases[index];
		}
	}
	if (!sym.wordSymbol.compare("EOF")) {
		lineCount++;
		index++;
	}
}

string SynaticParser::nextWord() {
	int tmp = index + 1;
	if (tmp < numOfPhrases) {
		if (!phrases[tmp].wordSymbol.compare("ELON")) {
			tmp++;
			if (tmp < numOfPhrases) {
				return phrases[tmp].wordSymbol;
			}
		}
		else
			return phrases[tmp].wordSymbol;
	}
}

// <程序>→<分程序>
void SynaticParser::MainProcedure() {
	SubProcedure();
	return;
};

// <分程序>→begin <说明语句表>;<执行语句表> end
void SynaticParser::SubProcedure() {
	if (!sym.wordSymbol.compare("begin")) {
		Advance();
		DeclareStatementTable();
		if (!sym.wordSymbol.compare(";")) {
			Advance();
			ExecuteStatementTable();
			if (!sym.wordSymbol.compare("end")) {
				Advance();
			}
			else
				writeErr("  Sub Procedure Matching Error! Lack of Keyword: end");
		}
		// 分程序错误，是否缺少 ;
		else if (index != numOfPhrases) {
			// function拼写错误
			if (phrases[index - 1].wordSymbol != "ELON") {
				writeErr("  Serious Syntax Error: invalid " + phrases[index - 1].wordSymbol);
				exit(0);
			}
			lineCount--;
			writeErr("  Sub Procedure Missing Error! Lack of Symbol ';'");
			Token tmp;
			tmp.wordSymbol = ";";
			tmp.wordType = "23";
			phrases.insert(phrases.begin() + index, tmp);
			init();
			SubProcedure();
		}
	}
	// 分程序错误，是否缺少/错误 begin
	else {
		writeErr("  Sub Procedure Missing Error! Lack of Keyword: begin");
		// 缺失
		if (!sym.wordSymbol.compare(ptype)) {
			Token tmp;
			tmp.wordSymbol = "begin";
			tmp.wordType = "01";
			phrases.insert(phrases.begin() + index, tmp);
			sym = phrases[index];
			SubProcedure();
		}
		// 错误
		else {
			sym.wordSymbol = "begin";
			sym.wordType = "01";
			SubProcedure();
		}
	}
};

/*
<说明语句表>→<说明语句>│<说明语句表>;<说明语句>

消除左递归：
<说明语句表>→<说明语句><说明语句表A>
<说明语句表A>→;<说明语句><说明语句表A>│空
*/
// <说明语句表>→<说明语句><说明语句表A>
void SynaticParser::DeclareStatementTable() {
	DeclareStatement();
	DeclareStatementTableA();
}

// <说明语句表A>→;<说明语句><说明语句表A>
void SynaticParser::DeclareStatementTableA() {
	if (!sym.wordSymbol.compare(";")) {
		if (!nextWord().compare(ptype)) {
			Advance();
			DeclareStatement();
			DeclareStatementTableA();
		}
	}
	else
		return;
}

// <说明语句>→<变量说明>│<函数说明>
void SynaticParser::DeclareStatement() {
	if (!sym.wordSymbol.compare(ptype)) {
		if (!nextWord().compare("function")) {
			FunctionDeclare();
		}
		else
			VarDeclare();
	}
	// 说明语句出错,是否integer错误
	else {
		writeErr("  Declare Statement Missing Error! Lack of Keyword: integer");
		phrases[index].wordSymbol = ptype;
		phrases[index].wordType = "03";
		init();
		SubProcedure();
	}
}

// <函数说明>→integer function <标识符>(<参数>);<函数体>
void SynaticParser::FunctionDeclare() {
	if (!sym.wordSymbol.compare(ptype)) {
		Advance();
		if (!sym.wordSymbol.compare("function")) {
			Advance();
			pname = sym.wordSymbol;
			plev++;
			vlev++;
			vproc.push(pname);
			procFlag = true;
			Identifier();
			if (!sym.wordSymbol.compare("(")) {
				Advance();
				Parameter();
				if (!sym.wordSymbol.compare(")")) {
					Advance();
					if (!sym.wordSymbol.compare(";")) {
						Advance();
						FunctionBody();
					}
					else
						writeErr("  Function Declare Missing Error! Lack of Symbol ';'");
				}
				else
					writeErr("  Function Declare Matching Error! Lack of Symbol ')'");
			}
			else {
				writeErr("  Function Declare Missing Error! Lack of Symbol '('");
			}
		}
		else
			writeErr("  Function Declare Missing Error! Lack of Keyword: function");
	}
	else
		writeErr("  Function Declare Missing Error! Lack of Keyword: integer");
	UpdatePro();
}

// 更新procTable
void SynaticParser::UpdatePro() {
	vector<string> tmp;
	tmp.push_back(checkLen(pname));
	tmp.push_back(ptype);
	tmp.push_back(to_string(plev));
	for (int i = 0; i < varTable.size(); i++) {
		if (!varTable[i][0].compare(checkLen(firstVar))) {
			fadr = varTable[i][5];
		}
		if (!varTable[i][0].compare(checkLen(lastVar))) {
			ladr = varTable[i][5];
		}
	}
	tmp.push_back(fadr);
	tmp.push_back(ladr);
	procTable.push_back(tmp);
	vproc.pop();
	pname = vproc.top();
	plev--;
	vlev--;
	procFlag = false;
}

// <标识符>→<字母>│<标识符><字母>│<标识符><数字>
string SynaticParser::Identifier() {
	if (!sym.wordType.compare(IdentifierType)) {
		string tmp = sym.wordSymbol;
		Advance();
		return tmp;
	}
	else {
		writeErr("  Identifier Missing Error! Not Identifier");
		return "error";
	}
}

// <参数>→<变量>
void SynaticParser::Parameter() {
	Variable(FormalParameter);
}

// <变量>→<标识符>
void SynaticParser::Variable(int vkind) {
	string vname = Identifier();
	bool flag = true;
	// 判断varTable是否存在变量，false: 存在
	for (int i = 0; i < varTable.size(); i++) {
		if (!checkLen(vname).compare(varTable[i][0]) && !checkLen(pname).compare(varTable[i][1])) {
			flag = false;
			break;
		}
	}
	// 检测重复定义
	if (flag == false && vkind == ActualParameter) {
		writeErr("  Duplicate Definition Error! Variable " + vname + " is duplicately defined");
	}
	// 使用该变量，未定义
	if (vkind == UseParameter && flag && vname != pname) {
		writeErr("  Undefined Error! Parameter " + vname + " is not defined before usage");
	}
	else if (flag && vname != pname) {
		vector<string> tmp;
		tmp.push_back(checkLen(vname));
		tmp.push_back(checkLen(vproc.top()));
		tmp.push_back(to_string(vkind));
		tmp.push_back(ptype);
		tmp.push_back(to_string(vlev));
		tmp.push_back(to_string(vadr));
		varTable.push_back(tmp);
		vadr++;
	}
	if (procFlag && vname != pname && vkind == FormalParameter) {
		lastVar = vname;
	}
}

// <函数体>→begin <说明语句表>;<执行语句表> end
void SynaticParser::FunctionBody() {
	if (!sym.wordSymbol.compare("begin")) {
		Advance();
		DeclareStatementTable();
		if (!sym.wordSymbol.compare(";")) {
			Advance();
			ExecuteStatementTable();
			if (!sym.wordSymbol.compare("end")) {
				Advance();
			}
			else
				writeErr("  Function Body Matching Error! Lack of Keyword: end");
		}
		else if (phrases[index-3].wordSymbol == ptype) {
			lineCount--;
			writeErr("  Function Body Missing Error! Lack of Symbol ';'");
			Token tmp;
			tmp.wordSymbol = ";";
			tmp.wordType = "23";
			phrases.insert(phrases.begin() + index, tmp);
			init();
			SubProcedure();
		}
		else {
			writeErr("  Serious Syntax Error: Function Body Missing Error! Lack of Symbol ';'");
			exit(0);
		}
	}
	else
		writeErr("  Function Body Missing Error! Lack of Keyword: begin");
}

// <变量说明>→integer <变量>
void SynaticParser::VarDeclare() {
	if (!sym.wordSymbol.compare(ptype)) {
		Advance();
		if (plev) {
			if (firstVar == "")
				firstVar = sym.wordSymbol;
			Variable(FormalParameter);
		}
		else
			Variable(ActualParameter);
	}
	else
		writeErr("  Variable Declare Missing Error! Lack of Keyword: integer");
}

// <执行语句表>→<执行语句>│<执行语句表>;<执行语句>
// 消除左递归：
// <执行语句表>→<执行语句><执行语句表A>
// <执行语句表A>→;<执行语句><执行语句表A>│空
void SynaticParser::ExecuteStatementTable() {
	ExecuteStatement();
	ExecuteStatementTableA();
}

void SynaticParser::ExecuteStatementTableA() {
	if (!sym.wordSymbol.compare(";")) {
		Advance();
		ExecuteStatement();
		ExecuteStatementTableA();
	}
	else
		return;
}

// <执行语句>→<读语句>│<写语句>│<赋值语句>│<条件语句>
void SynaticParser::ExecuteStatement() {
	if (!sym.wordSymbol.compare("read")) {
		ReadStatement();
	}
	else if (!sym.wordSymbol.compare("write")) {
		WriteStatement();
	}
	else if (!sym.wordType.compare(IdentifierType)) {
		AssignStatement();
	}
	else if (!sym.wordSymbol.compare("if")) {
		ConditionStatement();
	}
	else {
		writeErr("  Execute Statement Missing Error! Symbol '" + sym.wordSymbol + "' should not be here");
		toNext();
	}
}

// <读语句>→read(<变量>)
void SynaticParser::ReadStatement() {
	if (!sym.wordSymbol.compare("read")) {
		Advance();
		if (!sym.wordSymbol.compare("(")) {
			Advance();
			Variable(UseParameter);
			if (!sym.wordSymbol.compare(")")) {
				Advance();
			}
			else
				writeErr("  Read Statement Matching Error! Lack of Symbol ')'");
		}
		else
			writeErr("  Read Statement Missing Error! Lack of Symbol '('");
	}
	else
		writeErr("  Read Statement Missing Error! Lack of Symbol: read");
}

// <写语句>→write(<变量>)
void SynaticParser::WriteStatement() {
	if (!sym.wordSymbol.compare("write")) {
		Advance();
		if (!sym.wordSymbol.compare("(")) {
			Advance();
			Variable(UseParameter);
			if (!sym.wordSymbol.compare(")")) {
				Advance();
			}
			else
				writeErr("  Write Statement Matching Error! Lack of Symbol ')'");
		}
		else
			writeErr("  Write Statement Missing Error! Lack of Symbol '('");
	}
	else
		writeErr("  Write Statement Missing Error! Lack of Symbol: write");
}

// <赋值语句>→<变量>:=<算术表达式>
void SynaticParser::AssignStatement() {
	Variable(UseParameter);
	if (!sym.wordSymbol.compare(":=")) {
		Advance();
		MathExpression();
	}
}


// <算术表达式>→<算术表达式>-<项>│<项>
// 消除左递归：
// <算术表达式>→<项><算术表达式A>
// <算术表达式A>→-<项><算术表达式A>|空
void SynaticParser::MathExpression() {
	Term();
	MathExpressionA();
}

void SynaticParser::MathExpressionA() {
	if (!sym.wordSymbol.compare("-")) {
		Advance();
		Term();
		MathExpressionA();
	}
	else
		return;
}

// <项>→<项>*<因子>│<因子>
// 消除左递归：
// <项>→<因子><项A>
// <项A>→*<因子><项A>│空
void SynaticParser::Term() {
	Factor();
	TermA();
}

// <因子>→<变量>│<常数>│<函数调用>
// 变量判断须在函数调用判断后
void SynaticParser::Factor() {
	if (!sym.wordType.compare(DigitType)) {
		Constant();
	}
	else if (!nextWord().compare("(")) {
		FunctionCall();
	}
	else if (!sym.wordType.compare(IdentifierType)) {
		Variable(UseParameter);
	}
	else
		writeErr("  Factor Missing Error! It is not any identifier or digit or (");
}

// <项A>→*<因子><项A>│空
void SynaticParser::TermA() {
	if (!sym.wordSymbol.compare("*")) {
		Advance();
		Factor();
		TermA();
	}
	else
		return;
}

// <常数>→<无符号整数>
void SynaticParser::Constant() {
	UnsignedInteger();
}

void SynaticParser::UnsignedInteger() {
	if (!sym.wordType.compare(DigitType)) {
		Advance();
	}
}

// <函数调用>→<标识符>(<算数表达式>)
void SynaticParser::FunctionCall() {
	Identifier();
	if (!sym.wordSymbol.compare("(")) {
		Advance();
		MathExpression();
		if (!sym.wordSymbol.compare(")")) {
			Advance();
		}
		else {
			lineCount--;
			writeErr("  Function Call Matching Error! Lack of Symbol ')'");
			lineCount++;
		}
	}
	else
		writeErr("  Function Call Missing Error! Lack of Symbol '('");
}

// <条件语句>→if<条件表达式>then<执行语句>else<执行语句>
void SynaticParser::ConditionStatement() {
	if (!sym.wordSymbol.compare("if")) {
		Advance();
		ConditionExpression();
		if (!sym.wordSymbol.compare("then")) {
			Advance();
			ExecuteStatement();
			if (!sym.wordSymbol.compare("else")) {
				Advance();
				ExecuteStatement();
			}
				//writeErr("  Condition Statement Error! Lack of Symbol: else");
		}
		else
			writeErr("  Condition Statement Matching Error! Lack of Symbol: then");
	}
	else
		writeErr("  Condition Statement Missing Error! Lack of Symbol: if");
}

// <条件表达式>→<算术表达式><关系运算符><算术表达式>
void SynaticParser::ConditionExpression() {
	MathExpression();
	RelationOperator();
	MathExpression();
}

// <关系运算符>→<│<=│>│>=│=│<>
void SynaticParser::RelationOperator() {
	if (12 <= stoi(sym.wordType) and stoi(sym.wordType) <= 17) {
		Advance();
	}
	else
		writeErr("  Relation Operator Missing Error! Invalid Operator");
}

/*
int main() {
	string name;
	cout << "Input file path + name: ";
	cin >> name;
	ifstream in(name + ".dyd");
	// 初始化
	ofstream errOut(name + ".err");
	errOut.close();
	ofstream varOut(name + ".var");
	varOut.close();
	ofstream proOut(name + ".pro");
	proOut.close();

	string buffer;
	vector<Token> tokens;
	// 读取dyd文件
	while (getline(in, buffer)) {
		Token tmp;
		buffer.erase(0, buffer.find_first_not_of(" "));
		int space = buffer.find_first_of(" ");
		tmp.wordSymbol = buffer.substr(0, space);
		tmp.wordType = buffer.substr(space + 1, buffer.length());
		tokens.push_back(tmp);
	}
	in.close();
	SynaticParser parser(tokens);
	parser.fileName = name;
	parser.MainProcedure();
	parser.write();

	ifstream err(name + ".err");
	char ch;
	err >> ch;
	if (err.eof()) {
		parser.writeDys();
	}

}
*/