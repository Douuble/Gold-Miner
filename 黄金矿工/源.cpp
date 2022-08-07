#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include <time.h>
#include<math.h>
#include<mmsystem.h>
#pragma comment(lib"winmm.lib")
#define PI 3.1415
#define MAX_OBJECT 6
#define MAX_ANGLE 85
#define MaxSize 20

int pass = 0;                         //用于标记是否进入游戏
int next = 1;
int flag = 1;
int newu = 0;
char tname[20], tpassword[20];


class Sign          //用户类
{
public:
	char* score[20];
	int i = 0;       //用来指示数组
	int n = 0;       //用来标记存储的用户个数
	void Register();
	void Login();
	void Save();
	void Read();
	Sign()         //构造函数对类赋初值
	{
		for (int i = 0; i < MaxSize; i++) {
			name[i] = new char[20];
			password[i] = new char[20];
			score[i] = new char[20];
		}
	}
private:
	char* name[20];
	char* password[20];
};

FILE* fp;
Sign S;

enum MINE {          //枚举图片类型
	goldb = 1,       //大金块
	golds = 2,       //小金块
	stoneb = 3,      //大石块
	stones = 4,      //小石块
};

enum MINE_TYPE                  //物品及状态类型枚举
{	//方向
	LEFT,             //左  
	RIGHT,            //右
	//状态
	M_NORMAL,         //摆动
	M_LONG,           //伸长
	M_SHORT,          //缩短
	M_DONE,           //结束
};

typedef struct Object {     //物品结点定义
	int value;              //价值
	bool exit;              //存在与否
	int type;               //类型
	int get;                //是否抓取
	int x, y;               //坐标
	struct Object *next;
}Object,*Ob;

struct Hook {         //钩子结构体定义
	int x, y;         //起始位置坐标
	int vx,vy;        //速度分量
	int xf, yf;       //最终位置坐标
	int angle;        //摆动角度
	int direction;    //摆动方向
	int width;        //宽度
	int length;       //长度
	int get;          //用1标识抓取成功，用0标识抓取未成功
	int state;        //用以标识伸长、缩短和摇摆三种状态
}hook;

struct Interface {     //屏幕参数
	int goal=0;        //目标分数
	int score=0;       //玩家分数
	int time;          //倒计时
	int level = 1;     //等级
}in;

IMAGE hok, hhok, hhhok, hcha1, cha1;

void Sign::Read() { //文件读取操作
	FILE* fp = fopen("User.txt", "r");
	if (fp == NULL)
	{
		perror("Error");
		exit(0);
	}
	while (fscanf(fp, "%s %s %s\n", name[i], password[i], score[i]) == 3) {  // 循环读入游戏信息
		i++;
	}
	n = i;                                               //用于存储已读取的信息数
	fclose(fp);

}                                           

void Cursor(int op)                                            //光标闪烁                 
{
	while (!_kbhit())
	{
		setlinecolor(RGB(0, 0, 0));
		setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 1);		
		setrop2(R2_XORPEN); //设置异或绘图方式	
		switch (op) {
		case 1:
			line(505, 235, 505, 265);
			Sleep(400);
			setlinecolor(WHITE);
			line(505, 235, 505, 265);
			Sleep(400);
			break;
		case 2:
			line(510, 298, 510, 330);
			Sleep(400);
			setlinecolor(WHITE);
			line(510, 298, 510, 330);
			Sleep(400);
			break;
		}
	}
	setlinecolor(BLACK);
	switch (op) {
	case 1:
		line(505, 235, 505, 265); break;
	case 2:
		line(510, 298, 510, 330); break;
	}
}

void Sign::Login()
{
	int k = 0;
	char ch;
	IMAGE login;
	loadimage(&login, "./注册.jpg");
	putimage(0, 0, &login);
	int nlogin = 1;
	while (nlogin)
	{
		settextstyle(30, 0, "隶书");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(99, 58, 30));

		if (MouseHit())
		{
			int euser = 0;
			MOUSEMSG log = GetMouseMsg();
			switch (log.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (log.x > 500 && log.x < 745 && log.y>235 && log.y < 270)       //用户名框
				{
					Cursor(1);
					scanf("%s", tname);
					outtextxy(510, 235, tname);
				}
				if (log.x > 500 && log.x < 745 && log.y>295 && log.y < 340)       //密码框
				{
					Cursor(2);
					scanf("%s", tpassword);
					outtextxy(510, 300, tpassword);

				}
				euser = 1;
				if (log.x > 560 && log.x < 695 && log.y>360 && log.y < 400)       //注册按钮
				{
					n = i + 1;
					for (int j = 0; j < n; j++)
					{
						if (strcmp(tname, name[j]) == 0) {
							settextcolor(RGB(247, 206, 75));
							outtextxy(790, 320, "*用户名已存在！请重试！");
							Sleep(2000);
							Login();
						}
						else {
							HWND hwnd = GetHWnd();
							MessageBox(hwnd, "注册成功！即将跳转至登录页面", "跳转提示", MB_OK);
							newu = 1;
							Register();
							nlogin = 0;
							i++;
							break;
						}
					}
				}break;
			}
		}
	}
}

void Sign::Register()                       //登录注册页面
{
	char ttname[20],ttpassword[20];
	IMAGE reg, cursor, cursor2, game;
	loadimage(&reg, "./登录.jpg");
	putimage(0, 0, &reg);
	while (!pass)
	{
		settextstyle(30, 0, "隶书");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(99, 58, 30));

		if (MouseHit())
		{
			int nuser = 1;
			MOUSEMSG reg = GetMouseMsg();
			switch (reg.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (reg.x > 500 && reg.x < 745 && reg.y>235 && reg.y < 270)       //用户名框
				{
					Cursor(1);

					scanf("%s", ttname);
					outtextxy(510, 235, ttname);
				}
				if (reg.x > 500 && reg.x < 745 && reg.y>295 && reg.y < 340)       //密码框
				{
					Cursor(2);

					scanf("%s", ttpassword);
					outtextxy(510, 300, ttpassword);

				}
				nuser = 0;
				if (reg.x > 560 && reg.x < 695 && reg.y>360 && reg.y < 400)       //登录按钮
				{
					for (int j = 0; j <= n; j++)
					{
						if (strcmp(tname, name[j]) == 0|| strcmp(tname, ttname) == 0)                  //判断账号是否存在
						{
							if (strcmp(tpassword, password[j]) == 0|| strcmp(tpassword, ttpassword) == 0)      //判断密码是否正确
							{
								loadimage(&game, "./进入游戏.jpg");
								putimage(0, 0, &game);
								Sleep(800);
								if (newu == 1) {
									password[n] = tpassword;
									name[n] = tname;
								}
			/*					else { 
									password[j] = tpassword;
								name[j] = tname;}*/
								flag = 0;
								pass = 1;            //置一用于结束循环
								break;
							}
							else
							{
								settextcolor(RGB(247, 206, 75));
								outtextxy(800, 320, "*密码错误！请重试");
								Sleep(2000);
								Register();
							}
						}
						else
						{
							if (flag==1) {
								settextcolor(RGB(247, 206, 75));
								outtextxy(790, 235, "*用户不存在！请重试");
								Sleep(2000);
								Register();
							}
						}
					}
				}
				if (reg.x > 580 && reg.x < 750 && reg.y>430 && reg.y < 455)       //注册跳转
				{
					Login();
				}
			}
		}
	}
}

void Mainmenu() {                            //主菜单
	//Sign S;
	IMAGE menu;
	loadimage(&menu, "./主页面.jpg");
	putimage(0, 0, &menu);
	while (flag) {
		if (MouseHit()) {
			MOUSEMSG main = GetMouseMsg();
			switch (main.uMsg) {
			case WM_LBUTTONDOWN:
				if (main.x > 50 && main.x < 360 && main.y>10 && main.y < 330) {
					cleardevice();
					HWND window = GetHWnd();
					S.Register();
				}
				else {
					settextcolor(RGB(254, 255, 30));
					settextstyle(25, 0, "楷体");
					setbkmode(TRANSPARENT);
					outtextxy(465, 500, "^^请点击开始按钮以继续^^");
				}
				break;
			}
		}
	}Sleep(1000);
}

clock_t start, end;
int timesum;		//总时间
int timeuse;		//经过的时间
int timeleft;	//剩下的时间

void Time(int x)     //倒计时函数
{
	start = clock();
	timesum = x;	//总时间初始化
	timeuse = 0;	//经过的时间归零
	timeleft = 0;	//剩余时间初始化

}


void Button() {
	int press = 1;

	while (press) {
		if (MouseHit()) {
			MOUSEMSG start = GetMouseMsg();
			switch (start.uMsg) {
			case WM_LBUTTONDOWN:
				if (start.x > 810 && start.x < 870 && start.y>110 && start.y < 155)       //开始按钮
				{
					press = 0;
				}
				break;
			
			}
		}
	}
}

void Display(Ob& L) {
	BeginBatchDraw();
	IMAGE  cha1, hcha1, bk, list, gold, hgold, mgold, hmgold, stone, hstone, sstone, hsstone;
	settextcolor(RGB(122, 86, 4));
	settextstyle(25, 0, "楷体");
	setbkmode(TRANSPARENT);
	//背景部分
	loadimage(&bk, "./背景.jpg");
	loadimage(&list, "./状态栏.jpg");
	putimage(0, 0, &bk);
	putimage(950, 0, &list);

	//人物部分
	loadimage(&cha1, "./人物1.jpg");
	loadimage(&hcha1, "./人物1遮罩.jpg");
	putimage(450, 22, &hcha1, NOTSRCERASE);
	putimage(450, 22, &cha1, SRCINVERT);

	//按钮控制部分
		if (MouseHit()) {
			MOUSEMSG start = GetMouseMsg();
			switch (start.uMsg) {
			case WM_LBUTTONDOWN:
				if (start.x > 808 && start.x < 870 && start.y>110 && start.y < 150)       //开始按钮
				{
					Button();
				}
			
			}
		}

	//道具部分
	Object* p;
	p = L;
	while (p != NULL)
	{
		if (p->exit) {
			switch (p->type) {
			case 1:
				loadimage(&gold, "./大金矿.jpg");
				loadimage(&hgold, "./大金矿遮罩.jpg");
				putimage(p->x, p->y, &hgold, NOTSRCERASE);
				putimage(p->x, p->y, &gold, SRCINVERT);
				p->value = 60;
				hook.vx = 30;
				hook.vy = 30;
				break;
			case 2:
				loadimage(&mgold, "./小金矿.jpg");
				loadimage(&hmgold, "./小金矿遮罩.jpg");
				putimage(p->x, p->y, &hmgold, NOTSRCERASE);
				putimage(p->x, p->y, &mgold, SRCINVERT);
				p->value = 30;
				hook.vx = 25;
				hook.vy = 25;
				break;
			case 3:
				loadimage(&stone, "./石头.jpg");
				loadimage(&hstone, "./石头遮罩.jpg");
				putimage(p->x, p->y, &hstone, NOTSRCERASE);
				putimage(p->x, p->y, &stone, SRCINVERT);
				p->value = 10;
				hook.vx = 25;
				hook.vy = 25;

				break;
			case 4:
				loadimage(&sstone, "./小石头.jpg");
				loadimage(&hsstone, "./小石头遮罩.jpg");
				putimage(p->x, p->y, &hsstone, NOTSRCERASE);
				putimage(p->x, p->y, &sstone, SRCINVERT);
				p->value = 5;
				hook.vx = 20;
				hook.vy = 20;
				break;
			}
		}
		if (!p->exit && p->get == 0) {
			Object* s;
			int temp = 0;
			temp = p->value;
			in.score += temp;
			p->get = 1;
		}
		p = p->next;
	}

	//分数、等级及历史记录部分
	in.goal = in.level * 40;
	TCHAR s1[10], s2[10], s3[10];
	_stprintf_s(s1, "%d", in.goal);
	_stprintf_s(s2, "%d", in.score);
	_stprintf_s(s3, "%d", in.level);
	outtextxy(115, 55, s1);
	outtextxy(115, 18, s2);
	outtextxy(925, 55, s3);



	//倒计时部分
	TCHAR stime[10];
	end = clock();  		//停止计时
	timeuse = (end - start) / CLK_TCK;
	timeleft = timesum - timeuse;
	settextcolor(RGB(122, 86, 4));
	settextstyle(25, 0, "楷体");
	setbkmode(TRANSPARENT);
	_stprintf_s(stime, "%d", timeleft);
	outtextxy(925, 18, stime);

	if (timeleft == 0) {
		S.score[S.i] = (char *)in.score;
		hook.state = M_DONE;
	}
	EndBatchDraw();
}


void Judge(Ob& L) {
	int tag = 1;
	int flag = 1;
	Ob p;
	p = L;
	if (hook.xf <= 15 || hook.xf >= 900 || hook.yf >= 600) {      //超出边框回缩
		hook.state = M_SHORT;
	}

	while (p != NULL) {                  //循环判断各个物品
		switch (p->type) {
		case 1:
			if (abs(51 + p->x - hook.xf) <= 30 && abs(46 + p->y - hook.yf) <= 25) {
				hook.state = M_SHORT;
				flag = 0;
				p->x = p->x - sin(PI / 180 * hook.angle) * hook.vx;
				p->y = p->y - cos(PI / 180 * hook.angle) * hook.vy;
			}				
			break;
		case 2:
			if (abs(25 + p->x - hook.xf) <= 20 && abs(23 + p->y - hook.yf) <= 14) {
				hook.state = M_SHORT;
				flag = 0;
				p->x = p->x - sin(PI / 180 * hook.angle) * hook.vx;
				p->y = p->y - cos(PI / 180 * hook.angle) * hook.vy;
			}				
			break;
		case 3:
			if (abs(-hook.xf + 45 + p->x) <= 30 && abs(-hook.yf + 39 + p->y) <= 21) {
				hook.state = M_SHORT;
				flag = 0;
				p->x = p->x - sin(PI / 180 * hook.angle) * hook.vx;
				p->y = p->y - cos(PI / 180 * hook.angle) * hook.vy;

			}				
			break;
		case 4:
			if (abs(-hook.xf + 22 + p->x) <= 20 && abs(-hook.yf + 19 + p->y) <= 11) {
				hook.state = M_SHORT;
				flag = 0;
				p->x = p->x - sin(PI / 180 * hook.angle) * hook.vx;
				p->y = p->y - cos(PI / 180 * hook.angle) * hook.vy;
			}
			break;
		}
		if (flag == 1) {
			p = p->next;
		}
		else break;
	}

	while (hook.state == M_SHORT) {                           //绳子回缩
		if (flag == 0) {
			p->x = p->x - sin(PI / 180 * hook.angle) * hook.vx;
			p->y = p->y - cos(PI / 180 * hook.angle) * hook.vy;
		}

		hook.xf = hook.xf - sin(PI / 180 * hook.angle) * hook.vx;
		hook.yf = hook.yf - cos(PI / 180 * hook.angle) * hook.vy;
		Display(L);
		putimage(hook.xf, hook.yf, &hhok, NOTSRCERASE);
		putimage(hook.xf, hook.yf, &hok, SRCINVERT);
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);
		line(hook.x + hook.length, hook.y, hook.xf + hook.length + 5, hook.yf);
		loadimage(&cha1, "./人物2.jpg");
		loadimage(&hcha1, "./人物2遮罩.jpg");
		Sleep(20);
		putimage(450, 22, &hcha1, NOTSRCERASE);
		putimage(450, 22, &cha1, SRCINVERT);
		Sleep(300);
		cleardevice();
		if (flag == 0) {
			if (hook.yf <= 115 || p->y <= 95) { 		//回缩到起始位置结束
				hook.state = M_NORMAL;
				p->exit = false;
			}
		}
		else if (flag == 1) {
			if (hook.yf <= 115) { 		//回缩到起始位置结束
				hook.state = M_NORMAL;
			}
		}
	}
}

void Control(Ob& L) {                     //键盘下键实现放钩子
	BeginBatchDraw();
	loadimage(&hok, "./钩子.jpg");
	loadimage(&hhok, "./钩子遮罩.jpg");

	if (_kbhit()) {               //放钩子并实现绳子伸长
		char input = _getch();
		switch (input) {
		case 80:
			hook.state = M_LONG;
			while (hook.state == M_LONG) {
				Judge(L);
				Display(L);
				hook.xf = hook.xf + sin(PI / 180 * hook.angle) * hook.vx;
				hook.yf = hook.yf + cos(PI / 180 * hook.angle) * hook.vy;
				putimage(hook.xf, hook.yf, &hhok, NOTSRCERASE);
				putimage(hook.xf, hook.yf, &hok, SRCINVERT);
				setlinecolor(BLACK);
				setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);

				//人物动态效果
				line(hook.x + hook.length, hook.y, hook.xf + hook.length + 5, hook.yf);
				loadimage(&cha1, "./人物2.jpg");
				loadimage(&hcha1, "./人物2遮罩.jpg");
				Sleep(20);
				putimage(450, 22, &hcha1, NOTSRCERASE);
				putimage(450, 22, &cha1, SRCINVERT);
				Sleep(300);

				cleardevice();

			}break;
		}
	}
	EndBatchDraw();  //防止频闪
}

void Hook(Ob& L) {                         //钩子摆动
	BeginBatchDraw();
	Object* p;
	p = L;
	start = clock();
	IMAGE hok, hhok, hhhok;
	while (hook.state == M_NORMAL)
	{
		Display(L);
		Control(L);
		if (hook.direction == RIGHT)
		{
			hook.angle++;
		}
		else
		{
			hook.angle--;
		}
		if (hook.angle > MAX_ANGLE)     //与else if共同实现在角度范围内的摆动
		{
			hook.direction = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE)
		{
			hook.direction = RIGHT;
		}
		hook.xf = hook.x + sin(PI / 180 * hook.angle) * 35;
		hook.yf = hook.y + cos(PI / 180 * hook.angle) * 15;

		loadimage(&hok, "./钩子.jpg");
		loadimage(&hhok, "./钩子遮罩.jpg");

		putimage(hook.xf, hook.yf, &hhok, NOTSRCERASE);
		putimage(hook.xf, hook.yf, &hok, SRCINVERT);

		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);

		line(hook.x + hook.length, hook.y, hook.xf + hook.length + 5, hook.yf);
		Sleep(50);
		cleardevice();
	}
	EndBatchDraw();  //防止频闪
}

void Initialize(Ob &L) {       //各物品初始化(链表存储)
	srand(GetTickCount());
	//初始化物品信息
	//石头
	L = new Object;
	L->next = NULL;
	Object* p, * q;
	p = L;
	for (int i = 0; i < MAX_OBJECT; i++) {
		q = new Object;
		q->exit = true;
		q->value = 0;
		q->x = rand() % 821+30;
		q->y = rand() % 317 + 200;
		q->type = rand() % 4 + 1;
		p->get = 0;
		q->next = p->next;
		p->next = q;
		p = q;
	}
	////其中的rand()%(n-m+1)+m算是一个公式，记录一下方便以后查阅。

	//比如产生10~30的随机整数：

	//	srand((int)time(0));

	//int a = rand() % (21) + 10;


	int x = 455;                   //初始化钩子
	int y = 90;
	hook.width = 13;
	hook.length = 7;
	hook.x = x + hook.width;
	hook.y = y + hook.length;
	hook.xf = hook.x;
	hook.yf = hook.y;
	hook.angle = 0;
	hook.direction = RIGHT;
	hook.vx = 30;
	hook.vy = 30;
	hook.state = M_NORMAL;
}

void Sign::Save() {
	n++;
	fp = fopen("User.txt", "w");
	if (fp == NULL) {
		perror("Error");
		exit(0);
	}
	else {
		for (i = 0; i < n; i++) {
			fprintf(fp, " % s % s\n", name[i], password[i]);
		}
	}fclose(fp);
}

void Pass() {//过关判断部分
	IMAGE pass, npass;
	loadimage(&pass, "./过关.jpg");
	loadimage(&npass, "./闯关失败.jpg");

	if (timeleft == 0) {
		if (in.goal <= in.score) {        //成功
			putimage(0, 0, &pass);
			in.level++;
			Sleep(3000);
		}
		else {                            //失败
			next = 0;
			putimage(0, 0, &npass);
			Sleep(3000);
			S.Save();
		}
	}
}

int main() { 
	Ob L;//程序主框架
	initgraph(1100, 618, SHOWCONSOLE);
	S.Read();
	int time = 65;
	Mainmenu();            //主页面显示
	setrop2(R2_COPYPEN);
	while (next) {
		Initialize(L);          //游戏界面初始化
		time -= 5;
		Time(time);
		Hook(L);
		Pass();
	}
		closegraph();
	return 0;
}

