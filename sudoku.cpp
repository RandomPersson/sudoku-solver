#include <conio.h>

using namespace std;

// ################################################################
// # Universal coloring function string ccout(string) by RP  v0.0 #
// ################################################################
#include <sstream>
#include <iostream>
#ifdef __unix__
string getBColor(char c){switch(c){case '0':return "40";case '1':return "44";case '2':return "42";case '3':return "46";case '4':return "41";case '5':return "45";case '6':return "43";case '7':return "47";case '8':return "100";case '9':return "104";case 'a':return "102";case 'b':return "106";case 'c':return "101";case 'd':return "105";case 'e':return "103";case 'f':return "107";default:return "255";}}string getFColor(char c){switch(c){case '0':return "30";case '1':return "34";case '2':return "32";case '3':return "36";case '4':return "31";case '5':return "35";case '6':return "33";case '7':return "37";case '8':return "90";case '9':return "94";case 'a':return "92";case 'b':return "96";case 'c':return "91";case 'd':return "95";case 'e':return "93";case 'f':return "97";default:return "255";}}
string ccout(string s){int l=s.length()-1;for(int i=0;i<l;++i){if(s[i]=='&'){cout<<"\033["<<getFColor(s[++i])<<"m";}else if(s[i]=='@'){cout<<"\033["<<getBColor(s[++i])<<"m";}else{cout<<s[i];}}if(l>=1&&(s[l-1]=='&'||s[l-1]=='@'))return "";else if(l>=0)cout<<s[l];return "";}
#else
#ifdef _WIN32
#include <windows.h>
HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);char fcolor=15,bcolor=0;string ccout(string s){int c=0;for(int i=0;i<s.length()-1;++i){if(s[i]=='&'){cout.flush();c=s[++i];fcolor=c>=48&&c<=58?c-48:(c>=97&&c<=102?c-87:15);SetConsoleTextAttribute(hConsole,(bcolor<<4)+fcolor);}else if(s[i]=='@'){cout.flush();c=s[++i];bcolor=c>=48&&c<=58?c-48:(c>=97&&c<=102?c-87:15);SetConsoleTextAttribute(hConsole,(bcolor<<4)+fcolor);}else{cout<<s[i];}}if(s.length()>=2&&(s[s.length()-2]=='&'||s[s.length()-2]=='@'))return "";else if(s.length()>=1)cout<<s[s.length()-1];return "";}
#endif
#endif
void testcolors(){ostringstream ss;for(int i=0;i<10;++i){for(int j=0;j<10;++j){ss<<"&"<<i<<"@"<<j<<"#";ccout(ss.str());ss.str("");}cout<<endl;}ccout("@0&f");}
// ################################################################

bool debug=0;
int basemap[9][9];
COORD pos;

void printmap(int (*map)[9][9], bool resetpos=false) {
	if(resetpos){SetConsoleCursorPosition(hConsole, pos);}
	for (int i=0; i<9; ++i) {
		if (i%3 == 0) ccout("&7----------------------\n");
		for (int j=0; j<9; ++j) {
			if (j%3 == 0) ccout("&7|");
			if ((*map)[i][j]>0) {
				ccout(basemap[i][j]?"&3":"&b");
				cout << (*map)[i][j] << " ";
			}
			else if ((*map)[i][j]==0) ccout("  ");
			else ccout("&8X ");
		}
		ccout("&7|\n");
	}
	ccout("&7----------------------\n");
}
bool isValid(int (*map)[9][9], int x, int y) {
	int n = (*map)[x][y];
	int sx, sy;
	
	for (int i=0; i<9; ++i) {
		if (i!=x && (*map)[i][y]==n) return false;
		if (i!=y && (*map)[x][i]==n) return false;
		sx = (x/3)*3+i%3;
		sy = (y/3)*3+i/3;
		if (sx!=x && sy!=y && (*map)[sx][sy]==n) return false;
	}
	
	return true;
}
bool goDeeper(int (*map)[9][9], int x, int y, bool anim) {
	if(debug){cout << "goDeeper: " << x << "|" << y << endl;cout.flush();}
	bool found = 0;
	for (int i=x; i<9; ++i) {
		for (int j=0; j<9; ++j) {
			if(debug){cout << "  map[" << i << "][" << j << "]: " << (*map)[i][j] << endl;cout.flush();}
			if ((*map)[i][j] == 0) {
				if(debug){cout << "    found empty spot" << endl;cout.flush();}
				found = 1;
				x = i;
				y = j;
				break;
			}
		}
		if (found) break;
	}
	if (!found) return true;
	
	for (int k=1; k<=9; ++k) {
		if(debug){cout << "    k=" << k << endl;cout.flush();}
		(*map)[x][y] = k;
		if (isValid(map, x, y)) {
			if(debug){cout << "    " << k << " is valid" << endl;cout.flush();}
			if(anim)printmap(map, true);
			if(goDeeper(map, x, y, anim)) {
				return true;
			}
		}
	}
	if(debug){cout << "  no valid k found" << endl;}
	if(debug){cout << "  resetting map[" << x << "][" << y << "] back to 0" << endl;cout.flush();}
	(*map)[x][y] = 0;
	return false;
}
bool solve(int (*map)[9][9], bool anim=false) {
	return goDeeper(map, 0, 0, anim);
}
void exampleinput(int (*map)[9][9]) {
	for (int i=0; i<81; ++i) (*map)[i/9][i%9] = 0;
	(*map)[0][0] = 5;
	(*map)[0][4] = 8;
	(*map)[0][7] = 4;
	(*map)[0][8] = 9;
	(*map)[1][3] = 5;
	(*map)[1][7] = 3;
	(*map)[2][1] = 6;
	(*map)[2][2] = 7;
	(*map)[2][3] = 3;
	(*map)[2][8] = 1;
	(*map)[3][0] = 1;
	(*map)[3][1] = 5;
	(*map)[4][3] = 2;
	(*map)[4][5] = 8;
	(*map)[5][7] = 1;
	(*map)[5][8] = 8;
	(*map)[6][0] = 7;
	(*map)[6][5] = 4;
	(*map)[6][6] = 1;
	(*map)[6][7] = 5;
	(*map)[7][1] = 3;
	(*map)[7][5] = 2;
	(*map)[8][0] = 4;
	(*map)[8][1] = 9;
	(*map)[8][4] = 5;
	(*map)[8][8] = 3;
}
bool check(int (*map)[9][9]) {
	return true; //TODO maybe
	
	//maybe not
}
void input(int (*map)[9][9]) {
	system("cls");
	printmap(map, true);
	for (int i=0; i<9; ++i)
		for (int j=0; j<9; ++j) {
			(*map)[i][j] = abs((getch()-48)%10);
			printmap(map, true);
		}
	ccout("&eInput done.&a");
}
void copyTab(int (*from)[9][9], int (*to)[9][9]) {
	for (int i=0; i<9; ++i)
		for (int j=0; j<9; ++j)
			(*to)[i][j] = (*from)[i][j];
}
void init() {
	ccout("&a");
	pos.X = 0;
	pos.Y = 0;
}
int main() {
	init();
	int map[9][9];
	for (int i=0; i<81; ++i) map[i/9][i%9] = -1;
	
	exampleinput(&map);
	input(&map);//change to input(&map) for custom input; 0 is empty space
	copyTab(&map, &basemap);
	printmap(&map, true);
	
	if(check(&map)) {
		if (solve(&map, true)) {
			printmap(&map, true);
			ccout("&aBoard solved.");
		}
		else {
			ccout("&cThis board has no solution.&a");
		}
	}
	else {
		ccout("&cThis board is invalid!&a\n");
	}
	
	ccout("\n&eProgram finished.&7");
	
	return 0;
}

