#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Quiz;
int cmp(const void* a, const void* b);

class Map{
public:
	string* map;
	int row;
	int col;
public:
	Map();
	Map(ifstream& file);
public:
	void printMap();
	void putStr(Quiz& q, string str);
	string makeStr(Quiz& q);
};

class Quiz{
public:
	int row, col;
	int len;
	bool fill;
	bool horizon;
	vector<int> connected;
public:
	Quiz();
	Quiz(int r, int c, bool hori);
public:
	void setLen(const Map& m);
	bool matchStr(string origin, string str);
};

class QuizSet{
public:
	Quiz* quizSet;
	int hori, vert;
	
public:
	QuizSet();
	QuizSet(ifstream& file, const Map& m, int h, int v);
public:
	bool quizComplete();
	int selectNextQuiz(int index);
	int problemSearch(int index);
	void setConnected();
};



