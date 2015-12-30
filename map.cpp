#include "map.h"
#include <cstdlib>
#include <algorithm>



//**************MAP Class*************

Map::Map(){}
Map::Map(ifstream& file){
	file >> row >> col;
	map = new string[row];
	for (int i = 0; i < row; i++){
		file >> map[i];
	}
}

//맵을 출력하는 함수
void Map::printMap(){
	for (int i = 0; i < row; i++){
		cout << map[i] << endl;
	}
}




//사전에서 찾은 string을 맵에 집어넣는 함수
void Map::putStr(Quiz& q, string str){
	int r = q.row, c = q.col;
	if (q.horizon){
		for (int i = 0; i < q.len; i++){
			map[r][c + i] = str[i];
		}
	}
	else{
		for (int i = 0; i < q.len; i++){
			map[r + i][c] = str[i];
		}
	}
}

//퀴즈에 해당하는 원본 스트링을 맵에서 만드는 함수
string Map::makeStr(Quiz& q){
	string str;
	int r = q.row, c = q.col;
	if (q.horizon){
		for (int i = 0; i < q.len; i++){
			str += map[r][c + i];
		}
	}
	else{
		for (int i = 0; i < q.len; i++){
			str += map[r + i][c];
		}
	}

	return str;
}


//**************QUIZ Class*************

Quiz::Quiz(){}
Quiz::Quiz(int r, int c, bool hori){
	row = r - 1;
	col = c - 1;
	len = 0;
	fill = false;
	horizon = hori;
}

//각 퀴즈의 길이를 정하는 함수
void Quiz::setLen(const Map& m){
	int idx = 0;
	if (horizon){
		idx = col;
		while (m.map[row][idx] != '*'){
			len++, idx++;
			if (idx == m.col) break;
		}
	}
	else{
		idx = row;
		while (m.map[idx][col] != '*'){
			len++, idx++;
			if (idx == m.row) break;
		}
	}

	//cout << row << ' ' << col << ' ' << horizon << ' ' << len << endl;
}

//사전에서 찾은 단어가 맵에 들어갈 수 있는지를 확인
bool Quiz::matchStr(string origin, string str){
	bool match = true;
	if (horizon){
		for (int i = 0; i < str.length(); i++){
			//비교 중인 칸이 빈칸(_)이 아니고 있는 글자와 같지도 않다면 틀린 단어
			if (origin[i] != '_' && origin[i] != str[i]){
				match = false;
				break;
			}
		}
	}
	else{
		for (int i = 0; i < str.length(); i++){
			//비교 중인 칸이 빈칸(_)이 아니고 있는 글자와 같지도 않다면 틀린 단어
			if (origin[i] != '_' && origin[i] != str[i]){
				match = false;
				break;
			}
		}
	}
	return match;
}

//**************QUIZSET Class*************

QuizSet::QuizSet(){}
QuizSet::QuizSet(ifstream& file, const Map& m, int h, int v){
	hori = h;
	vert = v;
	quizSet = new Quiz[hori + vert];
	int r, c;
	for (int i = 0; i < hori + vert; i++){
		file >> r >> c;
		quizSet[i] = (i < hori ? Quiz(r, c, true) : Quiz(r, c, false));
		quizSet[i].setLen(m);
	}
	qsort(quizSet, hori + vert, sizeof(Quiz), cmp);
	setConnected();
	
}

//모든 퀴즈를 풀었는지를 체크하는 함수
bool QuizSet::quizComplete(){
	bool check = true;
	for (int i = 0; i < hori + vert; i++){
		if (!(check &= quizSet[i].fill))
			break;
	}
	return check;
}

//다음으로 풀 퀴즈를 선택하는 함수
int QuizSet::selectNextQuiz(int index){
	int idx = -1, t;
	//연결된 퀴즈 중에서 다음 퀴즈 선택
	for (int i = 0; i < quizSet[index].connected.size(); i++){
		t = quizSet[index].connected[i];
		if (!quizSet[t].fill){
			idx = t;
			break;
		}
	}
	//연결된 모든 문제가 풀렸다면 전체 문제 중 가장 길이가 긴 문제 선택
	if (idx == -1){
		for (int i = 0; i < hori + vert; i++){
			if (!quizSet[i].fill){
				idx = i;
				break;
			}
		}
	}
	return idx;
}

//퀴즈 연결관계 댄싱링크를 만드는 함수 
void QuizSet::setConnected(){
	for (int i = 0; i < hori + vert; i++){
		int r = quizSet[i].row, c = quizSet[i].col, l = quizSet[i].len;
		if (quizSet[i].horizon){
			for (int j = 0; j < hori + vert; j++){
				if (!quizSet[j].horizon && quizSet[j].row <= r && r <= quizSet[j].row + quizSet[j].len && c <= quizSet[j].col && quizSet[j].col <= c + l)
					quizSet[i].connected.push_back(j);
			}
		}
		else{
			for (int j = 0; j < hori + vert; j++){
				if (quizSet[j].horizon && quizSet[j].col <= c && c <= quizSet[j].col + quizSet[j].len && r <= quizSet[j].row && quizSet[j].row <= r + l)
					quizSet[i].connected.push_back(j);
			}
		}
		//sort(quizSet[i].connected.begin(), quizSet[i].connected.end(), cmp);
	}
}

int cmp(const void* a, const void* b){
	Quiz* qa = (Quiz*)a;
	Quiz* qb = (Quiz*)b;
	if (qa->len > qb->len)
		return -1;
	else if (qa->len < qb->len)
		return 1;
	else
		return 0;
}
