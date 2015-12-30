#define _CRT_SECURE_NO_WARNINGS
#include "map.h"
#include <vector>
#include <ctime>

vector<string>* dict = new vector<string>[35];
ifstream file;

//단어사전 생성 함수
void makeWordbook(){
	int numWord;
	string word;
	file >> numWord;
	while (numWord--){
		file >> word;
		dict[word.length()].push_back(word);
	}
}

int fillMap(Map& m, QuizSet& qs, int index, int cnt){
	if (cnt == qs.hori + qs.vert + 1)
		return index;

	int len = qs.quizSet[index].len;
	string beforeState = m.makeStr(qs.quizSet[index]);
	int cleared = 0;

	//단어장 처음부터 끝까지 탐색하면서 맵에 맞는 단어 검색
	for (int it = 0; it < dict[len].size(); it++){
		string word = dict[len][it];
		if (qs.quizSet[index].matchStr(beforeState, word)){
			cout << index << "번 단어 : " << word << endl;
			m.putStr(qs.quizSet[index], word);
			qs.quizSet[index].fill = true;
			if (cleared = fillMap(m, qs, qs.selectNextQuiz(index), cnt + 1)){
				break;
			}
		}
	}
	//만약 하나도 맞는단어가 없다면 채웠던 단어를 비운다.
	if (!cleared){
		m.putStr(qs.quizSet[index], beforeState);
		qs.quizSet[index].fill = false;
		cout << index << "번 실패************************" << endl;
	}
	return cleared;
}

int main(void) {
	int numCase;
	int vert, hori;
	time_t s, f;

	file.open("input.txt");
	makeWordbook();

	file >> numCase;
	while (numCase--){
		Map m(file);
		file >> hori >> vert;
		QuizSet qs(file, m, hori, vert);
		/*
		for (int i = 0; i < 19; i++){
			cout << i << "(" << qs.quizSet[i].row << ',' << qs.quizSet[i].col << ')' << "   |   ";
			for (int j = 0; j < qs.quizSet[i].connected.size(); j++){
				cout << qs.quizSet[i].connected[j] << ' ';
			}
			cout << endl;
		}
		*/
		time(&s);
		fillMap(m, qs, 0, 1);
		time(&f);

		//cout << f - s << endl;

		m.printMap();
		//cout << qs.quizComplete();



		m.~Map();
		qs.~QuizSet();
	}

	file.close();
	return 0;
}