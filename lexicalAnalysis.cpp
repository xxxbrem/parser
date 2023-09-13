#include "lexicalAnalysis.h"
#include <fstream>

// ��¼��ǰ�к�
// int lexicalLine = 0;

void Lexical::writeErr(string s) {
	ofstream fout(fileName + ".err", ios::app);
	fout << s << endl;
	fout.close();
}

// ����λ����16λ����鳤���Ƿ����
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


// �����ؼ��ֺ��û��Զ����ʶ���ĺ���
Token Lexical::lexWord() {
	Token token;
	int i;
	// �ڵ�һ���ַ�Ϊ��ĸ������£�������Ϊ���ֻ���ĸ������Ϊһ������
	for (i = 0; isdigit(*ch) || isalpha(*ch); ch++) {
		temp[i++] = *ch;
	}
	temp[i] = '\0';
	string s = (string)temp;
	// s��keyword��
	if (keyword.find(s) != keyword.end()) {
		token.wordSymbol = checkLen(s);
		token.wordType = keyword[s];
	}
	// s����keyword�У���Ϊ��־��
	else {
		token.wordSymbol = checkLen(s);
		token.wordType = "10";
	}
	return token;
}


// ���������ĺ���
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


// ����������ĺ���������=, <>, <=, <, >=, >, -, *, :=
Token Lexical::lexOperators() {
	Token token;
	if (*ch == '<' || *ch == '>' || *ch == ':') {
		// >=, <=, :=
		if (*(ch + 1) == '=') {
			temp[0] = *ch;
			temp[1] = *(++ch);
			temp[2] = '\0';
		}
		// ð�Ų�ƥ��
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


// ������������ţ��ֺţ��ĺ���
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
		// �ж��Ƿ��ǿո�
		while (isspace(*ch)) {
			ch++;
		}
		// �ж��Ƿ�����ĸ
		if (isalpha(*ch)) {
			result.push_back(lexWord());
		}
		// �ж��Ƿ�������
		else if (isdigit(*ch)) {
			result.push_back(lexDigit());
		}
		// �ж��Ƿ��������
		else if (operate.find(*ch) != string::npos) {
			result.push_back(lexOperators());
		}
		// �ж��Ƿ��ǽ��
		else if (delimiter.find(*ch) != string::npos) {
			result.push_back(lexDelimiter());
		}
		// �Ƿ��ַ�
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
	// �������
	ifstream in(name + ".pas");
	// д�ʷ�����
	ofstream off(name + ".dyd");
	// ��ʼ��.err
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
		// ÿ�м���ELON
		off << "            ELON" << ' ' << "24" << endl;
		all.push_back(s);
	}
	// ��β����EOF
	off << "             EOF" << ' ' << "25" << endl;
	in.close();
	off.close();
	return 0;
}
*/