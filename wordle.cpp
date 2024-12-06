#include<stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<fstream>
#include<conio.h>
#include<windows.h>
#include<graphics.h>
using namespace std;
string ver = "4.0.0";
string date = "20241202";
string config = "\\wordle.conf";
vector<string> keyboard = { "Q W E R T Y U I O P",
						 " A S D F G H J K L",
						 "  Z X C V B N M" };          // 点击版
string user_root;
string progname = "wordle.exe", cppname = "wordle.cpp";
bool havecpp;
vector<string> answers;  // push_back增加数据
vector<string> words;
vector<string> wordmatrix;
HWND window;
int cursorx, cursory;
void clear_screen() {
	solidrectangle(0, 0, 1000, 500);
	cursorx = cursory = 0;
	return;
}        // claer 
void putstring(string s) {
	for (int i = 0; i < (int)(s.size()); i++)
		outtextxy(cursorx, cursory, s[i]), cursorx += 10;
	return;
}  // 在游戏进程中输出
void putstring2(string s) {
	for (int i = 0; i < (int)(s.size()); i++)
		outtextxy(cursorx, cursory, s[i]), cursorx += 30;
	return;
}
void putccc(char c) {
	outtextxy(cursorx, cursory, c);
	cursorx += 10;
	return;
}
void putint(int x) {
	string ss;
	bool neg = false;
	if (x < 0) neg = true, x = -x;
	while (x) {
		ss += (char)(x % 10 + '0');
		x /= 10;
	}
	reverse(ss.begin(), ss.end());
	if ((int)(ss.size()) == 0) ss = "0";
	if (neg) ss = "-" + x;
	putstring(ss);
	return;
}
void button() {
	putstring("  ");
	line(10, cursory - 2, 10, cursory + 18);
	line(10, cursory + 18, 350, cursory + 18);
	line(350, cursory + 18, 350, cursory - 2);
	line(350, cursory - 2, 10, cursory - 2);
	return;
}	// 按钮列表
void button2(int row) {
	line(198, row * 23 - 7, 198, row * 23 + 16);
	line(198, row * 23 + 16, 302, row * 23 + 16);
	line(302, row * 23 + 16, 302, row * 23 - 7);
	line(302, row * 23 - 7, 198, row * 23 - 7);
	return;
}
void changeline() {
	cursory += 20;
	cursorx = 0;
	return;
} // 换行
void changeline2() {
	cursory += 30;
	cursorx = 0;
	return;
}
string readstring(bool soft_keyboard = false) {
	int basex = cursorx;
	string str;
	char c;
	ExMessage EM;
	int curpos = 0;
	while (1) {
		solidrectangle(basex, cursory, 1000, cursory + 20);
		cursorx = basex;
		putstring(str);
		line(basex + curpos * 10, cursory, basex + curpos * 10, cursory + 20);
		while (1) {
			EM = getmessage();
			if (soft_keyboard && EM.lbutton) {
				if (EM.y >= 39 && EM.y <= 108 && EM.x >= 198 && EM.x <= 302) {
					if (EM.y >= 85) {
						if (curpos == 0) continue;
						if (curpos != (int)(str.size()))
							str = str.substr(0, curpos - 1) + str.substr(curpos);
						else
							str = str.substr(0, curpos - 1);
						curpos--;
						break;
					}
					else if (EM.y >= 62) str = "help!";
					solidrectangle(basex, cursory, 1000, cursory + 20);
					cursorx = basex;
					putstring(str);
					changeline();
					return str;
				}
				int R = (EM.y - 40) / 20; int C = EM.x / 10;
				if (R >= 0 && R < 3 && C < (int)(keyboard[R].size()) && keyboard[R][C] != ' ') {
					str = str.substr(0, curpos) + (char)(keyboard[R][C] - 'A' + 'a') + str.substr(curpos);
					curpos++;
					break;
				}
			}
			if (EM.message == WM_CHAR) {
				c = EM.ch;
				if (c == ' ') {
					str = str.substr(0, curpos) + '_' + str.substr(curpos);
					curpos++;
				}
				else if (c == '\n' || c == '\r') {
					solidrectangle(basex, cursory, 1000, cursory + 20);
					cursorx = basex;
					putstring(str);
					changeline();
					return str;
				}
				else if (c == '\t' || c == '\b') continue;
				else if (c >= 33 && c <= 126) {
					str = str.substr(0, curpos) + c + str.substr(curpos);
					curpos++;
				}
				break;
			}
			if (EM.message == WM_KEYDOWN) {
				if (EM.vkcode == VK_LEFT) curpos = max(curpos - 1, 0);
				if (EM.vkcode == VK_RIGHT) curpos = min(curpos + 1, (int)(str.size()));
				if (EM.vkcode == VK_BACK) {
					if (curpos == 0) continue;
					if (curpos != (int)(str.size()))
						str = str.substr(0, curpos - 1) + str.substr(curpos);
					else
						str = str.substr(0, curpos - 1);
					curpos--;
				}
				break;
			}
		}
	}
}    // 读取用户输入
void pause() {
	button();
	putstring("0)   continue");
	ExMessage EM;
	while (1) {
		EM = getmessage();
		if (EM.message == WM_CHAR) {
			if (EM.ch == '0') break;
		}
		if (EM.lbutton) {
			if (EM.y >= cursory - 2 && EM.y <= cursory + 18 && EM.x >= 10 && EM.x <= 290)
				break;
		}
	}
	return;
} // 暂停并增加返回键
void button1() {
	putstring("  ");
	line(200, cursory - 10, 200, cursory + 30);
	line(200, cursory + 30, 550, cursory + 30);
	line(550, cursory + 30, 550, cursory - 10);
	line(550, cursory - 10, 200, cursory - 10);
	return;
}
void pause1() {
	button1();
	putstring("0)   continue");
	ExMessage EM;
	while (1) {
		EM = getmessage();
		if (EM.message == WM_CHAR) {
			if (EM.ch == '0') break;
		}
		if (EM.lbutton) {
			if (EM.y >= cursory - 10 && EM.y <= cursory + 30 && EM.x >= 200 && EM.x <= 550)
				break;
		}
	}
	return;
}
string user;
/*
#define back vals["COLOR_BACK"]
#define text vals["COLOR_TEXT"]
#define red vals["COLOR_RED"]
#define green vals["COLOR_GREEN"]
#define blue vals["COLOR_BLUE"]
#define yellow vals["COLOR_YELLOW"]
#define purple vals["COLOR_PURPLE"]*/
//WHITE
#define back WHITE
#define text BLACK
#define red LIGHTMAGENTA
#define green GREEN	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),10)
#define blue BLUE  //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),9)
#define yellow BLACK  //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),14)
#define purple CYAN   //SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),13)
void scta(int clr) {
	settextcolor(clr);
	return;
}
vector<pair<char, int> > hist[100010];
int cnt[10000];
int col[100010];
int system(string s) {
	return system(s.data());
}
int bad[1000];
string det;
map<string, int> vals;
set<string> inter;
map<string, string> strv;
pair<int, int> rat;
void writeconf();
vector<int> levels;
vector<string> titles;
vector<int> colors;
void clr(int R) {
	scta(text);
	for (int i = 0; i < (int)(colors.size()); i++) {
		if (R >= levels[i] && R < levels[i + 1]) {
			scta(colors[i]);
			break;
		}
	}
	return;
}   // colour
string level(int R) {
	for (int i = 0; i < (int)(titles.size()); i++) {
		if (R >= levels[i] && R < levels[i + 1]) {
			return titles[i];
		}
	}
	return "ERROR";
}   // 赋予水平等级
string encode(int rating, int rounds) {
	string ret = "";
	rounds = rounds * 2900 + rating;
	rounds *= 97;
	rounds += rand() % 96;
	rounds++;
	while (rounds) {
		ret += ((rounds % 26) + 'A');
		rounds /= 26;
	}
	while (ret.size() <= 10) ret += "A";
	reverse(ret.begin(), ret.end());
	return ret;
}
pair<int, int> decode(string rating) {
	int val = 0;
	for (int i = 0; i < (int)(rating.size()); i++) {
		val = (val * 26 + rating[i] - 'A');
	}
	val /= 97;
	return make_pair(val % 2900, val / 2900);
}
void printstate(int rounds, int wid, int rounds_disp) {
	clear_screen();
	//setbkcolor();
	cleardevice();
	scta(text);
	putstring("Wordle > Guess Word"); changeline();
	putstring("Try #"); putint(rounds_disp); changeline();
	int CX = cursorx, CY = cursory;
	button2(2); button2(3); button2(4);
	cursorx = 225; cursory = 40;
	putstring("ENTER");
	cursorx = 215; cursory = 63;
	putstring("GIVE UP"); changeline();
	cursorx = 205; cursory = 86;
	putstring("BACKSPACE"); changeline();
	cursorx = CX; cursory = CY;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < (int)(keyboard[i].size()); j++) {
			if (keyboard[i][j] != ' ') {
				if (bad[keyboard[i][j] - 'A'] == 0) scta(text);
				if (bad[keyboard[i][j] - 'A'] == 1) scta(red);
				if (bad[keyboard[i][j] - 'A'] == 2) scta(yellow);
				if (bad[keyboard[i][j] - 'A'] == 3) scta(text);
			}
			putccc(keyboard[i][j]);
			scta(text);
		}
		changeline(); 
	}
	putint((int)(answers[wid].size()));
	putstring(" letters."); changeline();
	putstring(det); changeline();
	hist[rounds + 1].clear();
	for (int i = 1; i <= rounds; i++) {
		for (int j = 0; j < hist[i].size(); j++) {
			scta(hist[i][j].second);
			putccc(hist[i][j].first);
		}
		changeline();
	}
	scta(text);
	return;
}   // 普通界面
void printstate2(int rounds, int wid, int rounds_disp) {
	clear_screen();
	scta(text);
	putstring("Wordle > Easteregg > Final challenge > Start "); changeline();
	putstring("Try #"); putint(rounds_disp); changeline();
	int CX = cursorx, CY = cursory;
	putint((int)(answers[wid].size()));
	putstring(" letters."); changeline();
	putstring(det); changeline();
	hist[rounds + 1].clear();
	for (int i = 1; i <= rounds; i++) {
		for (int j = 0; j < hist[i].size(); j++) {
			scta(hist[i][j].second);
			putccc(hist[i][j].first);
		}
		changeline();
	}
	scta(text);
	return;
}   // 特殊界面
void savescore(int rounds, int wid) {
	string data;
	data += answers[wid];
	string T = " ";
	for (int i = 1; i <= rounds; i++) {
		if (hist[i][0].second != red && hist[i][0].second != yellow && hist[i][0].second != green)
			continue;
		for (int j = 0; j < hist[i].size(); j++) {
			T[0] = hist[i][j].first;
			data += T;
		}
		for (int j = 0; j < hist[i].size(); j++) {
			if (hist[i][j].second == red) data += "r";
			if (hist[i][j].second == green) data += "g";
			if (hist[i][j].second == yellow) data += "y";
		}
	}
	string hist_name = "HISTORY_";
	string tim, ntim;
	system("date /t > curtimewd.txt");
	ifstream dfin("curtimewd.txt");
	dfin >> tim;
	dfin.close(); system("del curtimewd.txt");
	ntim = tim.substr(0, 4) + tim.substr(5, 2) + tim.substr(8, 2);
	tim = ntim;
	hist_name += tim;
	system("time /t > curtimewd.txt");
	ifstream tfin("curtimewd.txt");
	tfin >> tim;
	tfin.close(); system("del curtimewd.txt");
	ntim = tim.substr(0, 2) + tim.substr(3, 2);
	tim = ntim;
	hist_name += tim;
	int tint = time(NULL);
	tint %= 60;
	T[0] = '0' + tint / 10;
	hist_name += T;
	T[0] = '0' + tint % 10;
	hist_name += T;
	strv[hist_name] = data;
	writeconf();
	return;
}   // 保存成绩
void win(int rounds, int wid) {
	button();
	putstring("0)   continue"); changeline();
	button();
	putstring("1)   save score");
	ExMessage EM;
	while (1) {
		EM = getmessage();
		if (EM.message == WM_CHAR) {
			if (EM.ch == '0') break;
			if (EM.ch == '1') {
				savescore(rounds, wid);
				break;
			}
		}
		if (EM.lbutton) {
			if (EM.y >= cursory - 22 && EM.y <= cursory - 2 && EM.x >= 10 && EM.x <= 290)
				break;
			if (EM.y >= cursory - 2 && EM.y <= cursory + 18 && EM.x >= 10 && EM.x <= 290) {
				savescore(rounds, wid);
				break;
			}
		}
	}
	return;
}
void wordle() {
	clear_screen();
	int id = rand() % (int)(answers.size());
	scta(text);
	int round = 0;
	memset(bad, 0, sizeof(bad));
	det = "";
	for (int i = 0; i < (int)(answers[id].size()); i++) det += "_";
	int hists = 0;
	while (1) {
		hists++;
		round++;
		if (round == vals["TURNS"] + 1) {
			printstate(hists - 1, id, round - 1);
			scta(red);
			putstring("GAME OVER!"); changeline();
			scta(green);
			putstring("ANSWER: "); putstring(answers[id]);
			changeline();
			scta(text);
			int score = 0;
			for (int i = 0; i < (int)(answers[id].size()); i++)
				if (det[i] != '_') score += 100;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < (int)(keyboard[i].size()); j++)
					if (keyboard[i][j] != ' ') {
						if (bad[keyboard[i][j] - 'A'] == 2)  score += 50;
					}
			putstring("Score:");
			clr(score);
			putint(score);
			scta(text);
			changeline();
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + score) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();
			pause();
			return;
		}
		printstate(hists - 1, id, round);
		string ssss;
		while (1) {
			ssss = readstring(true);
			if (ssss != "") break;
			cursory -= 20;
		}
		if (ssss == "help!") {
			printstate(hists - 1, id, round - 1);
			scta(red);
			putstring("YOU GAVE UP!"); changeline();
			scta(green);
			putstring("ANSWER: "); putstring(answers[id]);
			changeline();
			scta(text);
			pause();
			return;
		}
		bool flag = true;
		for (int i = 0; i < (int)(ssss.size()); i++) {
			if (!isalpha(ssss[i])) {
				flag = false;
				break;
			}
		}
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		for (int i = 0; i < (int)(ssss.size()); i++) if (ssss[i] <= 'Z')
			ssss[i] = ssss[i] - 'A' + 'a';
		if ((int)(ssss.size()) != (int)(answers[id].size())) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], purple));
			round--;
			continue;
		}
		flag = false;
		for (int i = 0; i < words.size(); i++) if (words[i] == ssss) flag = true;
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		memset(cnt, 0, sizeof(cnt));
		for (int i = 0; i < (int)(answers[id].size()); i++)
			cnt[answers[id][i] - 'a']++;
		int GR = 0;
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] == answers[id][i]) {
				cnt[ssss[i] - 'a']--;
				col[i] = green;
				det[i] = ssss[i];
				bad[ssss[i] - 'a'] = 3;
				ssss[i] = ssss[i] - 'a' + 'A';
				GR++;
			}
		}
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] >= 'a' && cnt[ssss[i] - 'a']) {
				cnt[ssss[i] - 'a']--;
				col[i] = yellow;
				if (!bad[ssss[i] - 'a']) bad[ssss[i] - 'a'] = 2;
				ssss[i] = ssss[i] - 'a' + 'A';
			}
		}
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] >= 'a') {
				col[i] = red;
				if (!bad[ssss[i] - 'a']) bad[ssss[i] - 'a'] = 1;
				ssss[i] = ssss[i] - 'a' + 'A';
			}
		}
		for (int i = 0; i < (int)(answers[id].size()); i++)
			hist[hists].push_back(make_pair(ssss[i] - 'A' + 'a', col[i]));
		if (GR == (int)(answers[id].size())) {
			printstate(hists, id, round);
			putstring("Guessed correctly after ");
			putint(round);
			putstring(" tries."); changeline();
			putstring("Score:");
			clr(max(25 - round, 5) * 100);
			putint(max(25 - round, 5) * 100);
			changeline();
			scta(text);
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + max(25 - round, 5) * 100) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();
			scta(text);
			win(hists, id);
			return;
		}
	}
	return;
}   // 主程序
void about() {
	clear_screen();
	scta(text);
	putstring("Wordle > About"); changeline();
	putstring("WORDLE (C++) Version ");
	putstring(ver);
	putstring(" (");
	putstring(date);
	putstring(")");
	changeline();
	putstring("All rights reserved.");
	changeline();
	pause();
	return;
}
void rules() {
	clear_screen();
	scta(text);
	putstring("Wordle > Help > Rules"); changeline();
	putstring("You have ");
	putint(vals["TURNS"]);
	putstring(" chances to guess a secret word."); changeline();
	putstring("If you got a letter right,and in the same position,it will be shown like ");
	scta(green); putstring("this"); scta(text);
	putstring("."); changeline();
	putstring("If you got a letter right,but in the wrong position,it will be shown like ");
	scta(yellow); putstring("this"); scta(text);
	putstring("."); changeline();
	putstring("If you got a letter wrong,it will be shown like ");
	scta(red); putstring("this"); scta(text);
	putstring("."); changeline();
	putstring("If the word isn't in the dictionary,it will be shown like ");
	scta(blue); putstring("this"); scta(text);
	putstring("."); changeline();
	putstring("If the word is too long/short,it will be shown like ");
	scta(purple); putstring("this"); scta(text);
	putstring("."); changeline();
	putstring("When you want to give up,type \"help!\" to see the answer.");
	changeline();
	pause();
	return;
}  // 规则
void rating() {
	clear_screen();
	scta(text);
	putstring("Wordle > Help > Rating System"); changeline();
	putstring("In every round of the game,you will be given a score."); changeline();
	putstring("It depends on how quickly you guessed the word correctly."); changeline();
	putstring("Your rating is the average score of all your games."); changeline();
	putstring("Ratings are shown in different colors:"); changeline();
	for (int i = 0; i < (int)(colors.size()); i++) {
		scta(colors[i]);
		if (i == 0) putstring("0000");
		else putint(levels[i]);
		scta(text);
		putstring(" to ");
		scta(colors[i]); putint(levels[i + 1] - 1); scta(text);
		putstring(" : ");
		scta(colors[i]); putstring(titles[i]); scta(text);
		changeline();
	}
	pause();
	return;
}	// rating计分
void scores() {
	clear_screen();
	scta(text);
	putstring("Wordle > Help > History"); changeline();
	putstring("The View History function allows you to view scores in previous games."); changeline();
	putstring("After every game, you have a choice of saving the game score."); changeline();
	putstring("By saving the score, you can view the game later on."); changeline();
	pause();
	return;
}
void accounts() {
	clear_screen();
	scta(text);
	putstring("Wordle > Help > accounts"); changeline();
	putstring("Different Wordle accounts can be stored on the same computer."); changeline();
	putstring("You can change accounts and create accounts as you may."); changeline();
	putstring("Ratings of different accounts don't interfere with each other."); changeline();
	pause();
	return;
}
void help() {
	while (1) {
		clear_screen();
		putstring("Wordle > Help"); changeline();
		int BG = cursory;
		button();
		putstring("1)   rules"); changeline();
		button();
		putstring("2)   rating system"); changeline();
		button();
		putstring("3)   history"); changeline();
		button();
		putstring("4)   accounts"); changeline();
		button();
		putstring("0)   exit help"); changeline();
		int ED = cursory;
		ExMessage EM;
		while (1) {
			EM = getmessage();
			int t = 0;
			if (EM.message == WM_CHAR) {
				t = EM.ch - '0';
				if (t >= 1 && t <= 4);
				else if (t == 0) t = 5;
				else continue;
			}
			else if (EM.lbutton) {
				if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
					t = (EM.y - BG + 22) / 20;
				}
				else continue;
			}
			else continue;
			if (t == 1) rules();
			if (t == 2) rating();
			if (t == 3) scores();
			if (t == 4) accounts();
			if (t == 5) return;
			break;
		}
	}
	return;
}

void readconf() {
	vals.clear();
	ifstream conf(config.data());
	string field, val_string;
	int val_int;
	while (conf >> field) {
		if (field[0] == '[') {
			conf >> val_int;
			vals[field.substr(1, field.size() - 2)] = val_int;
		}
		else {
			conf >> val_string;
			strv[field.substr(1, field.size() - 2)] = val_string;
		}
	}
	conf.close();
	return;
} // 读取配置
void writeconf() {
	ofstream conf(config.data());
	for (pair<string, int> conff : vals) {
		conf << "[" << conff.first << "]" << endl;
		conf << conff.second << endl;
	}
	for (pair<string, string> conff : strv) {
		conf << "<" << conff.first << ">" << endl;
		conf << conff.second << endl;
	}
	conf.close();
	return;
} // 配置
void readscore(string name, string save) {
	scta(text);
	string TOC = name.substr(8, 4);
	TOC += "/"; TOC += name.substr(12, 2);
	TOC += "/"; TOC += name.substr(14, 2);
	TOC += " "; TOC += name.substr(16, 2);
	TOC += ":"; TOC += name.substr(18, 2);
	putstring("Time of Completion: "); putstring(TOC); changeline();
	putstring(save.substr(0, 5)); save = save.substr(5); changeline();
	int sc = 2500;
	while ((int)(save.size())) {
		sc -= 100;
		for (int i = 0; i < 5; i++) {
			if (save[i + 5] == 'r') scta(red);
			if (save[i + 5] == 'g') scta(green);
			if (save[i + 5] == 'y') scta(yellow);
			putccc(save[i]);
		}
		scta(text);
		changeline();
		save = save.substr(10);
	}
	putstring("Score: "); clr(sc); putint(sc); changeline();
	scta(text);
	return;
} // 读取用户保存的做题记录
void history() {
	clear_screen();
	vector<pair<string, string> > available;
	for (pair<string, string> P : strv) {
		if (P.first.substr(0, 8) == "HISTORY_")
			available.push_back(P);
	}
	if ((int)(available.size()) == 0) {
		putstring("Wordle > View History"); changeline();
		putstring("No saved scores. ");
		putstring("Select \"Save Score\" after finishing a game to save the score.");
		changeline();
		pause();
		return;
	}
	reverse(available.begin(), available.end());
	int id = 0, tot = (int)(available.size());
	while (1) {
		clear_screen();
		putstring("Wordle > View History"); changeline();
		putstring("Currently viewing ");
		putint(id + 1);
		putstring(" out of ");
		putint((int)(available.size()));
		changeline();
		readscore(available[id].first, available[id].second);
		int BG = cursory;
		button();
		putstring("1) next"); changeline();
		button();
		putstring("2) last"); changeline();
		button();
		putstring("3) delete entry"); changeline();
		button();
		putstring("0) exit"); changeline();
		int ED = cursory;
		ExMessage EM;
		while (1) {
			EM = getmessage();
			int t = 0;
			if (EM.message == WM_CHAR) {
				t = EM.ch - '0';
				if (t >= 1 && t <= 3);
				else if (t == 0) t = 4;
				else continue;
			}
			else if (EM.lbutton) {
				if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
					t = (EM.y - BG + 22) / 20;
				}
				else continue;
			}
			else continue;
			if (t == 1) id = (id + 1) % ((int)(available.size()));
			if (t == 2) id = (id + tot - 1) % ((int)(available.size()));
			if (t == 3) {
				strv.erase(available[id].first);
				available.erase(available.begin() + id);
				id = id % ((int)(available.size()));
				writeconf();
			}
			if (t == 4) return;
			break;
		}
	}
	return;
} // 记录面板
map<string, string> desc;
map<string, vector<pair<string, int> > > item_i;
map<string, vector<pair<string, string> > > item_s;
void prep_settings() {
	desc["UPDATE_INTERVAL"] = "Update interval";
	item_i["UPDATE_INTERVAL"].push_back({ "1 day",86400 });
	item_i["UPDATE_INTERVAL"].push_back({ "1 week",604800 });
	item_i["UPDATE_INTERVAL"].push_back({ "10 days",864000 });
	item_i["UPDATE_INTERVAL"].push_back({ "30 days",2592000 });
	item_i["UPDATE_INTERVAL"].push_back({ "180 days",15552000 });
	item_i["UPDATE_INTERVAL"].push_back({ "365 days",31536000 });
	item_i["UPDATE_INTERVAL"].push_back({ "never",2e9 });
	desc["DOWNLOAD_HEAD"] = "Download mode";
	item_s["DOWNLOAD_HEAD"].push_back({ "Standard","" });
	item_s["DOWNLOAD_HEAD"].push_back({ "Mirror","https://gitdl.cn/" });
	return;
}
void editi(string name) {
	clear_screen();
	putstring("Wordle > Settings > "); putstring(desc[name]); changeline();
	putstring("Current value:");
	for (pair<string, int> P : item_i[name])
		if (P.second == vals[name]) {
			putstring(P.first);
			changeline();
			break;
		}
	int BG = cursory;
	int id = 0;
	for (pair<string, int> P : item_i[name]) {
		button();
		putint(++id);
		putstring(")   ");
		putstring(P.first);
		changeline();
	}
	button();
	putstring("0)   go back"); changeline();
	int ED = cursory;
	ExMessage EM;
	while (1) {
		EM = getmessage();
		int t = 0;
		if (EM.message == WM_CHAR) {
			t = EM.ch - '0';
			if (t >= 1 && t <= (int)(item_i[name].size()));
			else if (t == 0) t = (int)(item_i[name].size()) + 1;
			else continue;
		}
		else if (EM.lbutton) {
			if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
				t = (EM.y - BG + 22) / 20;
			}
			else continue;
		}
		else continue;
		if (t <= (int)(item_i[name].size())) {
			vals[name] = item_i[name][t - 1].second;
			break;
		}
		break;
	}
	return;
}
void edits(string name) {
	clear_screen();
	putstring("Settings > "); putstring(desc[name]); changeline();
	putstring("Current value:");
	for (pair<string, string> P : item_s[name])
		if (P.second == strv[name]) {
			putstring(P.first);
			changeline();
			break;
		}
	int BG = cursory;
	int id = 0;
	for (pair<string, string> P : item_s[name]) {
		button();
		putint(++id);
		putstring(")   ");
		putstring(P.first);
		changeline();
	}
	button();
	putstring("0)   go back"); changeline();
	int ED = cursory;
	ExMessage EM;
	while (1) {
		EM = getmessage();
		int t = 0;
		if (EM.message == WM_CHAR) {
			t = EM.ch - '0';
			if (t >= 1 && t <= (int)(item_s[name].size()));
			else if (t == 0) t = (int)(item_s[name].size()) + 1;
			else continue;
		}
		else if (EM.lbutton) {
			if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
				t = (EM.y - BG + 22) / 20;
			}
			else continue;
		}
		else continue;
		if (t <= (int)(item_s[name].size())) {
			strv[name] = item_s[name][t - 1].second;
			break;
		}
		break;
	}
	return;
}

void settings() {
	clear_screen();
	putstring("Wordle > Settings"); changeline();
	putstring("Setting doesn't exist now.'"); changeline();
	pause();
	return;
}
void update() {
	clear_screen();
	putstring("Wordle > Update"); changeline();
	putstring("Update doesn't exist now.'"); changeline();
	pause();
	return;
}

void account() {
	clear_screen();
	putstring("Wordle > Change Account"); changeline();
	putstring("Current account:");
	putstring(strv["ACCOUNT"]);
	changeline();
	putstring("New account(leave blank to keep old account):");
	string tmp;
	tmp = readstring();
	if (tmp != "") {
		strv["ACCOUNT"] = tmp;
		if (!strv["RATING_" + strv["ACCOUNT"]].size())
			strv["RATING_" + strv["ACCOUNT"]] = encode(0, 0);
		putstring("Successfully changed account."); changeline();
		writeconf();
		pause();
	}
	return;
}
void username() {
	clear_screen();
	putstring("Wordle > Change Username"); changeline();
	putstring("Current username:");
	putstring(strv["ACCOUNT"]);
	changeline();
	putstring("New username(leave blank to keep old username):");
	string tmp;
	tmp = readstring();
	if (tmp != "") {
		strv["RATING_" + tmp] = strv["RATING_" + strv["ACCOUNT"]];
		strv.erase("RATING_" + strv["ACCOUNT"]);
		strv["ACCOUNT"] = tmp;
		putstring("Successfully changed username."); changeline();
		writeconf();
		pause();
	}
	return;
}
void EntryofFinalChallenge() {
	clear_screen();
	int id = rand() % (int)(answers.size());
	scta(text);
	int round = 0;
	memset(bad, 0, sizeof(bad));
	det = "";
	for (int i = 0; i < (int)(answers[id].size()); i++) det += "_";
	int hists = 0;
	while (1) {
		putstring("Wordle > Easteregg > Final challenge > Start"); changeline();
		hists++;
		round++;
		if (round == vals["TURNS"] + 1) {
			printstate2(hists - 1, id, round - 1);
			scta(red);
			putstring("GAME OVER!"); changeline();
			scta(green);
			putstring("ANSWER: "); putstring(answers[id]);
			changeline();
			scta(text);
			int score = 0;
			for (int i = 0; i < (int)(answers[id].size()); i++)
				if (det[i] != '_') score += 100;
			/*for (int i = 0; i < 3; i++)
				for (int j = 0; j < (int)(keyboard[i].size()); j++)
					if (keyboard[i][j] != ' ') {
						if (bad[keyboard[i][j] - 'A'] == 2)  score += 50;
					}*/
			putstring("Score:");
			clr(score);
			putint(score);
			scta(text);
			changeline();
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + score) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();
			pause();
			return;
		}
		printstate2(hists - 1, id, round);
		string ssss;
		while (1) {
			ssss = readstring();   // 如果是true,那么启用键盘
			if (ssss != "") break;
			cursory -= 20;
		}
		if (ssss == "help!") {
			printstate2(hists - 1, id, round - 1);
			scta(red);
			putstring("YOU GAVE UP!"); changeline();
			scta(green);
			putstring("ANSWER: "); putstring(answers[id]);
			changeline();
			scta(text);
			pause();
			return;
		}
		bool flag = true;
		for (int i = 0; i < (int)(ssss.size()); i++) {
			if (!isalpha(ssss[i])) {
				flag = false;
				break;
			}
		}
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		for (int i = 0; i < (int)(ssss.size()); i++) if (ssss[i] <= 'Z')
			ssss[i] = ssss[i] - 'A' + 'a';
		if ((int)(ssss.size()) != (int)(answers[id].size())) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], purple));
			round--;
			continue;
		}
		flag = false;
		for (int i = 0; i < words.size(); i++) if (words[i] == ssss) flag = true;
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		memset(cnt, 0, sizeof(cnt));
		for (int i = 0; i < (int)(answers[id].size()); i++)
			cnt[answers[id][i] - 'a']++;
		int GR = 0;
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] == answers[id][i]) {
				cnt[ssss[i] - 'a']--;
				col[i] = red;
				bad[ssss[i] - 'a'] = 3;
				ssss[i] = ssss[i] - 'a' + 'A';
				GR++;
			}
		}
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] >= 'a' && cnt[ssss[i] - 'a']) {
				cnt[ssss[i] - 'a']--;
				col[i] = green;
				if (!bad[ssss[i] - 'a']) bad[ssss[i] - 'a'] = 2;
				ssss[i] = ssss[i] - 'a' + 'A';
			}
		}
		for (int i = 0; i < (int)(answers[id].size()); i++) {
			if (ssss[i] >= 'a') {
				col[i] = yellow;
				if (!bad[ssss[i] - 'a']) bad[ssss[i] - 'a'] = 1;
				ssss[i] = ssss[i] - 'a' + 'A';
			}
		} // green->red yellow->green red->yellow调整了颜色顺序
		for (int i = 0; i < (int)(answers[id].size()); i++)
			hist[hists].push_back(make_pair(ssss[i] - 'A' + 'a', col[i]));
		//printstate2(hists - 1, id, round);
		if (GR == (int)(answers[id].size())) {
			printstate2(hists, id, round);
			putstring("Guessed correctly after ");
			putint(round);
			putstring(" tries."); changeline();
			putstring("Score:");
			clr(max(25 - round, 5) * 100);
			putint(max(25 - round, 5) * 100);
			changeline();
			scta(text);
			int scorex = max(25 - round, 5) * 100;
			if (scorex >= 2100) {
				putstring("GPA : 4.3");
				changeline();
				putstring("You won!");
				changeline();
			}
			else if (scorex >= 2000) {
				putstring("GPA : 4.0");
				changeline();
				putstring("Try again!");
				changeline();
			}
			else if (scorex >= 1800) {
				putstring("GPA : 3.7");
				changeline();
				putstring("Try again!");
				changeline();
			}
			else if (scorex >= 1500) {
				putstring("GPA : 3.3");
				changeline();
				putstring("Study more!");
				changeline();
			}
			else {
				putstring("Do more exercise!");
				changeline();
			}
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + max(25 - round, 5) * 100) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();
			scta(text);
			win(hists, id);
			return;
		}
		if (round == 6) {
			putstring("You lose!!!");
			changeline();
			pause();
			return;
		}
	}
}
void Rules_of_Final_Challenge() {
	while (1) {
		vector<pair<string, string> > available;
		for (pair<string, string> P : strv) {
			if (P.first.substr(0, 8) == "HISTORY_")
				available.push_back(P);
		}
		if ((int)(available.size()) >= 20) {      // temporary to 1
			clear_screen();
			putstring("Wordle > Easteregg > Final challenge > Rules of Final challenge"); changeline();
			putstring("As we all know, the rules before are: "); changeline();
			putstring("If you got a letter right,and in the same position,it will be shown like ");
			scta(green); putstring("this"); scta(text);
			putstring("."); changeline();
			putstring("If you got a letter right,but in the wrong position,it will be shown like ");
			scta(yellow); putstring("this"); scta(text);
			putstring("."); changeline();
			putstring("If you got a letter wrong,it will be shown like ");
			scta(red); putstring("this"); scta(text);
			putstring("."); changeline();
			putstring("If the word isn't in the dictionary,it will be shown like ");
			scta(blue); putstring("this"); scta(text);
			putstring("."); changeline();
			putstring("If the word is too long/short,it will be shown like ");
			scta(purple); putstring("this"); scta(text);
			putstring("."); changeline();
			putstring("However, you don't know which colour represents what now."); changeline();
			putstring("For example, to guess waits, you type 'arrow'."); changeline();
			putstring("Now, the letter a and w might not be shown as yellow, but green."); changeline();
			putstring("In another case, letter a might be shown as red");
			changeline();
			pause();
			return;
		}
		else {
			clear_screen();
			putstring("Wordle > Easteregg > Final challenge > Joker"); changeline();
			putstring("No rules here."); changeline();
			IMAGE img;
			loadimage(&img, "joker.jpg", 100, 100);
			putimage(0, 100, &img);
			pause();
			return;
		}
	}
	return;
}
void Finalchallenge() {
	while (1) {
		clear_screen();
		putstring("Wordle > Easteregg > Final challenge"); changeline();
		putstring("Ways to win: To get a higher GPA"); changeline();
		int BG = cursory;
		button();
		putstring("1)   Rules of Special Challenge"); changeline();
		button();
		putstring("2)   Entry"); changeline();
		button();
		putstring("0)   exit"); changeline();
		int ED = cursory;
		ExMessage EM;
		while (1) {
			EM = getmessage();
			int t = 0;
			if (EM.message == WM_CHAR) {
				t = EM.ch - '0';
				if (t == 1 || t == 2);
				else if (t == 0) t = 3;
				else continue;
			}
			else if (EM.lbutton) {
				if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
					t = (EM.y - BG + 22) / 20;
				}
				else continue;
			}
			else
				continue;
			if (t == 1) Rules_of_Final_Challenge();
			if (t == 2) EntryofFinalChallenge();
			if (t == 3) return;
			break;
		}
	}
	return;
}
void Rules_of_Final_Challenge2() {
	while (1) {
		clear_screen();
		putstring("Wordle > easter egg > Final challenge2 > Rules of Final challenge 2");
		changeline();
		putstring("In this challenge, you will try to fill in a 5*5 matrix.");
		changeline();
		putstring("And every column and every row can form a 5-letter word.");
		changeline();
		putstring("You will get some hint. And you can guess the word.");
		changeline();
		putstring("The other rules are the same as before.");
		changeline();
		pause();
		return;
	}
}
//
void printstate3(int rounds, int wid, int rounds_disp) {
	clear_screen();
	scta(text);
	putstring("Wordle > Easteregg > Final challenge > Start "); changeline();
	putstring("Try #"); putint(rounds_disp); changeline();
	int CX = cursorx, CY = cursory;
	putint((int)(wordmatrix[wid].size() * 5));
	putstring(" letters."); changeline();
	for (int i = 0; i < (int)(wordmatrix[wid * 5].size() * 5); i++) {
		putccc(det[i]);
		if (i % 5 == 4)
			changeline();
	}
	changeline();
	hist[rounds + 1].clear();
	for (int i = 1; i <= rounds; i++) {
		for (int j = 0; j < 5; j++) {
			int tempo = 0;
			for (int k=0;k<5;k++)
				if (hist[i][k*5+j].second==green){
					tempo=1;
				}
			if (tempo==1)
				scta(green);
			else
				scta(hist[i][j].second);
			putccc(hist[i][j].first); ///
			//cout << hist[i][j].first;
		}
		//cout << endl;
		changeline();
	}
	//cout << endl;
	scta(text);
	return;
}
void EntryofFinalChallenge2() {
	clear_screen();
	int id = rand() % (int)(wordmatrix.size() / 5);  // 5个一循环
	scta(text);
	int round = 0;
	memset(bad, 0, sizeof(bad));
	det = "";

	id = id - id % 5;
	for (int i = 0; i < 5 * (int)(wordmatrix[id].size()); i++)
		det += "_";
	int hists = 0;
	while (1) {
		putstring("Wordle > Easteregg > Final challenge2 > Start"); changeline();
		hists++;
		round++;
		if (round >= 11) {
			printstate3(hists - 1, id, round - 1);
			scta(red);
			putstring("GAME OVER!"); changeline();
			scta(green);
			putstring("ANSWER: ");
			changeline();
			for (int i = 0; i < 5; i++) {
				putstring(wordmatrix[id + i]);
				changeline();
			}
			scta(text);
			int score = 0;
			for (int i = 0; i < 5 * (int)(wordmatrix[id].size()); i++)
				if (det[i] != '_' && det[i] != '\n') score += 100;
			putstring("Score:");
			clr(score);
			putint(score);
			scta(text);
			changeline();
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + score) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();
			pause();
			return;
		}
		printstate3(hists - 1, id, round);
		/*
		for (int i=0;i<5;i++){
			putstring(wordmatrix[id+i]);
			changeline();
		}
		*/
		string tttt, ssss;
		while (1) {
			tttt = readstring();   // 如果是true,那么启用键盘
			if (tttt != "") break;
			cursory -= 20;
		}
		bool flag = true;
		for (int i = 0; i < (int)(tttt.size()); i++) {
			if (!isalpha(tttt[i])) {
				flag = false;
				break;
			}
		}
		ssss = tttt + tttt + tttt + tttt + tttt;
		//cout << ssss << endl;
		//cout << ssss.size() << endl;
		for (int i = 0; i < (int)(ssss.size()); i++) {
			col[i] = red;
		}
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		
		for (int i = 0; i < (int)(ssss.size()); i++)
			if (ssss[i] <= 'Z')
				ssss[i] = ssss[i] - 'A' + 'a';
		
		if ((int)(tttt.size()) != (int)(wordmatrix[id].size())) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], purple));
			round--;
			continue;
		}
		flag = false;
		for (int i = 0; i < words.size(); i++) if (words[i] == tttt) flag = true;
		if (!flag) {
			for (int i = 0; i < (int)(ssss.size()); i++)
				hist[hists].push_back(make_pair(ssss[i], blue));
			round--;
			continue;
		}
		printstate3(hists - 1, id, round);
		memset(cnt, 0, sizeof(cnt));
		for (int j = 0; j < 5; j++)
			for (int i = 0; i < (int)(wordmatrix[id + j].size()); i++)
				cnt[wordmatrix[id + j][i] - 'a']++;
		
		for (int j = 0; j < 5; j++)
			for (int i = 0; i < (int)(wordmatrix[id + j].size()); i++) {
				if (ssss[j * 5 + i] == wordmatrix[id + j][i]) {
					cnt[ssss[j * 5 + i] - 'a']--;
					col[j * 5 + i] = green;
					det[j * 5 + i] = ssss[j * 5 + i];
					bad[ssss[j * 5 + i] - 'a'] = 3;
					//ssss[j * 5 + i] = ssss[j * 5 + i] - 'a' + 'A';
					//cout << ssss[j * 5 + i] << " ";
					//putccc(ssss[j * 5 + i]);
				}
			}
		cout << endl;
		printstate3(hists - 1, id, round);
		for (int i = 0; i < 5 * (int)(wordmatrix[id].size()); i++) {
			if (col[i] == green || col[i] == blue || col[i] == purple) {
				continue;
			}
			else
			{
				col[i] = red;
				if (!bad[ssss[i] - 'a'])
					bad[ssss[i] - 'a'] = 1;
				//ssss[i] = ssss[i] - 'a' + 'A';
			}
		}
		printstate3(hists - 1, id, round);
		for (int j = 0; j < 5; j++)
			for (int i = 0; i < (int)(wordmatrix[id + j].size()); i++) {
				//hist[hists].push_back(make_pair(ssss[j * 5 + i] - 'A' + 'a', col[j * 5 + i]));
				hist[hists].push_back(make_pair(ssss[j * 5 + i] , col[j * 5 + i]));
			}
				

		int GR = 0;
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				if (det[i * 5 + j] != '_')
					GR++;
		if (GR == 5 * (int)(wordmatrix[id].size())) {
			printstate3(hists, id, round);
			putstring("Guessed correctly after ");
			putint(round);
			putstring(" tries."); changeline();
			if (round <= 10) {
				setcolor(GREEN);
				settextstyle(60, 0, "Congenial");
				putstring2("YOU WON!");
				changeline();
				changeline();
				changeline();
			}
			else {
				setcolor(GREEN);
				settextstyle(60, 0, "Congenial");
				putstring2("YOU LOSE!");
				changeline();
				changeline();
				changeline();
			}
			/*putstring("Score:");
			clr(max(25 - round, 5) * 100);
			putint(max(25 - round, 5) * 100);
			changeline();
			scta(text);
			rat.second = min(rat.second + 1, 1000);
			rat.first = (rat.first * (rat.second - 1) + max(25 - round, 5) * 100) / rat.second;
			strv["RATING_" + strv["ACCOUNT"]] = encode(rat.first, rat.second);
			writeconf();*/
			settextstyle(18, 0, "Congenial");
			setlinecolor(text);
			settextcolor(text);
			pause();
			return;
		}
	}
}
//
void Finalchallenge2() {
	while (1) {
		clear_screen();
		putstring("Wordle > easter egg > Final challenge2"); changeline();
		int BG = cursory;
		button();
		putstring("1)   Rules of Final challenge2"); changeline();
		button();
		putstring("2)   Entry"); changeline();
		button();
		putstring("0)   exit"); changeline();
		int ED = cursory;
		ExMessage EM;
		while (1) {
			EM = getmessage();
			int t = 0;
			if (EM.message == WM_CHAR) {
				t = EM.ch - '0';
				if (t == 1 || t == 2);
				else if (t == 0) t = 3;
				else continue;
			}
			else if (EM.lbutton) {
				if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
					t = (EM.y - BG + 22) / 20;
				}
				else continue;
			}
			else
				continue;
			if (t == 1) Rules_of_Final_Challenge2();
			if (t == 2) EntryofFinalChallenge2();
			if (t == 3) return;
			break;
		}
	}
}
void easteregg() {
	while (1) {
		clear_screen();
		putstring("Wordle > easter egg"); changeline();
		if (rat.first >= 1958) {     // temporary to 0
			putstring("Congratulations to you that you unlock the Eastern egg.");
			changeline();
			putstring("Special challenge for you."); changeline();
			int BG = cursory;
			button();
			putstring("1)   Special Challenge"); changeline();
			button();
			putstring("2)   Special Challenge2"); changeline();
			button();
			putstring("0)   exit"); changeline();
			int ED = cursory;
			ExMessage EM;
			while (1) {
				EM = getmessage();
				int t = 0;
				if (EM.message == WM_CHAR) {
					t = EM.ch - '0';
					if (t == 1 || t == 2);
					else if (t == 0) t = 3;
					else continue;
				}
				else if (EM.lbutton) {
					if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
						t = (EM.y - BG + 22) / 20;
					}
					else continue;
				}
				else
					continue;
				if (t == 1) Finalchallenge();
				if (t == 2) Finalchallenge2();
				if (t == 3) return;
				break;
			}
		}
		else {
			putstring("Nothing here.");
			changeline();
			int BG = cursory;
			button();
			putstring("0)   exit"); changeline();
			int ED = cursory;
			ExMessage EM;
			while (1) {
				EM = getmessage();
				int t = 0;
				if (EM.message == WM_CHAR) {
					t = EM.ch - '0';
					if (t == 0) {
						t = 1;
					}
				}
				else if (EM.lbutton) {
					if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
						t = (EM.y - BG + 22) / 20;
					}
					else continue;
				}
				else continue;
				if (t == 1)
					return;
				break;
			}
		}
		break;
	}
	return;
}

int main(int argc, char** argv) {
	string WW;
	ifstream reader("dictionary.txt");
	while (reader >> WW) words.push_back(WW);
	ifstream reader2("answers.txt");
	while (reader2 >> WW) answers.push_back(WW);
	ifstream reader3("dictionarynew.txt");

	while (getline(reader3, WW)) {
		if (WW.size() != 5) continue;
		wordmatrix.push_back(WW);
	}

	reader.close();
	ifstream test_update("wdupdater.bat");
	if (!test_update) test_update.close();
	else {
		test_update.close();
		system("del wdupdater.bat");
	}
	ifstream cpptest("wordle.cpp");
	if (!cpptest) havecpp = false;
	else havecpp = true;
	cpptest.close();
	for (int i = 0; i < (int)(answers.size()); i++) for (int j = 0; j < answers[i].size(); j++)
		answers[i][j] -= 'A', answers[i][j] += 'a';
	for (int i = 0; i < (int)(words.size()); i++) for (int j = 0; j < words[i].size(); j++)
		words[i][j] -= 'A', words[i][j] += 'a';
	system("date /t > curtimewd.txt");
	ifstream tfin("curtimewd.txt");
	string tim; tfin >> tim;
	tfin.close(); system("del curtimewd.txt");
	string ntim = tim.substr(0, 4) + tim.substr(5, 2) + tim.substr(8, 2);
	tim = ntim;
	system("echo %USERPROFILE% > userrootwd.txt");
	ifstream ufin("userrootwd.txt");
	ufin >> user_root;
	ufin.close(); system("del userrootwd.txt");
	config = user_root + config;
	ifstream tmpconf(config.data());
	if (!tmpconf) {
		tmpconf.close();
		ofstream confgen(config.data());
		confgen << endl;
		confgen.close();
	}
	else tmpconf.close();
	readconf();
	/*
	if (strv["DOWNLOAD_HEAD"] == "") strv["DOWNLOAD_HEAD"] = "";
	if (!vals["LAST_UPDATE"]) vals["LAST_UPDATE"] = time(0);
	if (!vals["UPDATE_INTERVAL"]) vals["UPDATE_INTERVAL"] = 604800;
	if (!vals["TURNS"]) vals["TURNS"] = 20;
	if (!vals["COLOR_BACK"]) vals["COLOR_BACK"] = 16448250;// 657930
	if (!vals["COLOR_TEXT"]) vals["COLOR_TEXT"] = 657930;//16448250
	if (!vals["COLOR_RED"]) vals["COLOR_RED"] = 658170;
	if (!vals["COLOR_GREEN"]) vals["COLOR_GREEN"] = 719370;
	if (!vals["COLOR_BLUE"]) vals["COLOR_BLUE"] = 16386570;
	if (!vals["COLOR_YELLOW"]) vals["COLOR_YELLOW"] = 719610;
	if (!vals["COLOR_PURPLE"]) vals["COLOR_PURPLE"] = 16386810;*/
	window = initgraph(1000, 500,EX_SHOWCONSOLE);
	setbkcolor(back);
	setfillcolor(back);
	setlinecolor(text);
	settextcolor(text);

	solidrectangle(0, 0, 1000, 290);
	settextstyle(18, 0, LPCTSTR("CONSOLAS"));
	scta(text);
	if (!strv["ACCOUNT"].size()) {
		strv["VERSION"] = ver;
		vals["LAST_UPDATE"] = time(0);
		putstring("Welcome to WORDLE ");
		putstring(ver);
		putstring("!");
		changeline();
		putstring("It seems that it's your first time using new Wordle.");
		changeline();
		putstring("Please create a username for yourself: ");
		string tmp;
		tmp = readstring();
		if (tmp == "") {
			tmp = "User";
			putstring("No username set. Default username will be used.");
			changeline();
		}
		if (tmp != "") {
			putstring("Set username to ");
			putstring(tmp);
			putstring(".");
			changeline();
			strv["ACCOUNT"] = tmp;
			strv["RATING_" + strv["ACCOUNT"]] = encode(0, 0);
			writeconf();
		}
		putstring("Let's start with default settings. You can change them later.");
		changeline();
		putstring("You may refer to Help for details about this program.");
		changeline();
		putstring("Press the button or press 0 on the keyboard to start.");
		changeline();
		clear_screen();
		pause1();
	}
	if (!strv["RATING_" + strv["ACCOUNT"]].size()) strv["RATING_" + strv["ACCOUNT"]] = encode(0, 0);
	if (!strv["VERSION"].size()) strv["VERSION"] = ver;
	writeconf();
	if (ver != strv["VERSION"]) {
		strv["VERSION"] = ver;
		vals["LAST_UPDATE"] = time(0);
		putstring("Welcome to WORDLE ");
		putstring(ver);
		putstring("!");
		changeline();
		writeconf();
		pause();
	}
	if (time(0) - vals["LAST_UPDATE"] > vals["UPDATE_INTERVAL"])
		update();
	srand(unsigned(time(NULL)));
	prep_settings();
	levels = { 0,1500,1800,2000,2100,2401 };
	titles = { "Beginner","Learner","Expert","Master","Genius" };
	colors = { text,green,blue,yellow,red };
	//setfillcolor(WHITE); //setfillcolor(BLACK)
	while (1) {
		cout << "TEST" << endl;
		clear_screen();
		//setbkcolor(GREY);
		cleardevice();
		settextcolor(GREEN);
		settextstyle(40	, 0, "Congenial");
		outtextxy(450,0,"Wordle"); changeline(); changeline();
		settextstyle(18, 0, "Congenial");
		settextcolor(BLACK); //settextcolor(WHITE);
		rat = decode(strv["RATING_" + strv["ACCOUNT"]]);
		putstring("Hello,");
		clr(rat.first);
		putstring(strv["ACCOUNT"]);
		scta(text);
		putstring("!");
		changeline();
		putstring("WORDLE (C++) Version ");
		putstring(ver);
		putstring(" (");
		putstring(date);
		putstring(")");
		changeline();
		putstring("As a student of USTC, your aim is to get a higher GPA"); changeline();
		putstring("Your task is to find the way to get the highest GPA 4.3"); changeline();
		putstring("Current Rating:");
		clr(rat.first);
		if (rat.first < 1000) putint(0);
		if (rat.first < 100) putint(0);
		if (rat.first < 10) putint(0);
		putint(rat.first);
		changeline();
		scta(text);
		putstring("Current Level:");
		clr(rat.first);
		putstring(level(rat.first));
		changeline();
		scta(text);
		int BG = cursory;
		button();
		putstring("1)   guess word"); changeline();
		button();
		putstring("2)   help"); changeline();
		button();
		putstring("3)   settings"); changeline();
		button();
		putstring("4)   update"); changeline();
		button();
		putstring("5)   change account"); changeline();
		button();
		putstring("6)   change username"); changeline();
		button();
		putstring("7)   view history"); changeline();
		button();
		putstring("8)   easter egg"); changeline();
		button();
		putstring("9)   about"); changeline();
		button();
		putstring("0)   exit"); changeline();
		int ED = cursory;
		ExMessage EM;
		while (1) {
			EM = getmessage();
			int t = 0;
			if (EM.message == WM_CHAR) {
				t = EM.ch - '0';
				if (t >= 1 && t <= 9);
				else if (t == 0) t = 10;
				else continue;
			}
			else if (EM.lbutton) {
				if (EM.y >= BG - 2 && EM.y <= ED - 2 && EM.x >= 10 && EM.x <= 290) {
					t = (EM.y - BG + 22) / 20;
				}
				else continue;
			}
			else continue;
			if (t == 1) wordle();
			if (t == 2) help();
			if (t == 3) settings();
			if (t == 4) update();
			if (t == 5) account();
			if (t == 6) username();
			if (t == 7) history();
			if (t == 8) easteregg();
			if (t == 9) about();
			if (t == 10) return 0;
			break;
		}
	}
	return 0;
}
// 主页面
