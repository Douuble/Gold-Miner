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

int pass = 0;                         //���ڱ���Ƿ������Ϸ
int next = 1;
int flag = 1;
int newu = 0;
char tname[20], tpassword[20];


class Sign          //�û���
{
public:
	char* score[20];
	int i = 0;       //����ָʾ����
	int n = 0;       //������Ǵ洢���û�����
	void Register();
	void Login();
	void Save();
	void Read();
	Sign()         //���캯�����ำ��ֵ
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

enum MINE {          //ö��ͼƬ����
	goldb = 1,       //����
	golds = 2,       //С���
	stoneb = 3,      //��ʯ��
	stones = 4,      //Сʯ��
};

enum MINE_TYPE                  //��Ʒ��״̬����ö��
{	//����
	LEFT,             //��  
	RIGHT,            //��
	//״̬
	M_NORMAL,         //�ڶ�
	M_LONG,           //�쳤
	M_SHORT,          //����
	M_DONE,           //����
};

typedef struct Object {     //��Ʒ��㶨��
	int value;              //��ֵ
	bool exit;              //�������
	int type;               //����
	int get;                //�Ƿ�ץȡ
	int x, y;               //����
	struct Object *next;
}Object,*Ob;

struct Hook {         //���ӽṹ�嶨��
	int x, y;         //��ʼλ������
	int vx,vy;        //�ٶȷ���
	int xf, yf;       //����λ������
	int angle;        //�ڶ��Ƕ�
	int direction;    //�ڶ�����
	int width;        //���
	int length;       //����
	int get;          //��1��ʶץȡ�ɹ�����0��ʶץȡδ�ɹ�
	int state;        //���Ա�ʶ�쳤�����̺�ҡ������״̬
}hook;

struct Interface {     //��Ļ����
	int goal=0;        //Ŀ�����
	int score=0;       //��ҷ���
	int time;          //����ʱ
	int level = 1;     //�ȼ�
}in;

IMAGE hok, hhok, hhhok, hcha1, cha1;

void Sign::Read() { //�ļ���ȡ����
	FILE* fp = fopen("User.txt", "r");
	if (fp == NULL)
	{
		perror("Error");
		exit(0);
	}
	while (fscanf(fp, "%s %s %s\n", name[i], password[i], score[i]) == 3) {  // ѭ��������Ϸ��Ϣ
		i++;
	}
	n = i;                                               //���ڴ洢�Ѷ�ȡ����Ϣ��
	fclose(fp);

}                                           

void Cursor(int op)                                            //�����˸                 
{
	while (!_kbhit())
	{
		setlinecolor(RGB(0, 0, 0));
		setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 1);		
		setrop2(R2_XORPEN); //��������ͼ��ʽ	
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
	loadimage(&login, "./ע��.jpg");
	putimage(0, 0, &login);
	int nlogin = 1;
	while (nlogin)
	{
		settextstyle(30, 0, "����");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(99, 58, 30));

		if (MouseHit())
		{
			int euser = 0;
			MOUSEMSG log = GetMouseMsg();
			switch (log.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (log.x > 500 && log.x < 745 && log.y>235 && log.y < 270)       //�û�����
				{
					Cursor(1);
					scanf("%s", tname);
					outtextxy(510, 235, tname);
				}
				if (log.x > 500 && log.x < 745 && log.y>295 && log.y < 340)       //�����
				{
					Cursor(2);
					scanf("%s", tpassword);
					outtextxy(510, 300, tpassword);

				}
				euser = 1;
				if (log.x > 560 && log.x < 695 && log.y>360 && log.y < 400)       //ע�ᰴť
				{
					n = i + 1;
					for (int j = 0; j < n; j++)
					{
						if (strcmp(tname, name[j]) == 0) {
							settextcolor(RGB(247, 206, 75));
							outtextxy(790, 320, "*�û����Ѵ��ڣ������ԣ�");
							Sleep(2000);
							Login();
						}
						else {
							HWND hwnd = GetHWnd();
							MessageBox(hwnd, "ע��ɹ���������ת����¼ҳ��", "��ת��ʾ", MB_OK);
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

void Sign::Register()                       //��¼ע��ҳ��
{
	char ttname[20],ttpassword[20];
	IMAGE reg, cursor, cursor2, game;
	loadimage(&reg, "./��¼.jpg");
	putimage(0, 0, &reg);
	while (!pass)
	{
		settextstyle(30, 0, "����");
		setbkmode(TRANSPARENT);
		settextcolor(RGB(99, 58, 30));

		if (MouseHit())
		{
			int nuser = 1;
			MOUSEMSG reg = GetMouseMsg();
			switch (reg.uMsg)
			{
			case WM_LBUTTONDOWN:
				if (reg.x > 500 && reg.x < 745 && reg.y>235 && reg.y < 270)       //�û�����
				{
					Cursor(1);

					scanf("%s", ttname);
					outtextxy(510, 235, ttname);
				}
				if (reg.x > 500 && reg.x < 745 && reg.y>295 && reg.y < 340)       //�����
				{
					Cursor(2);

					scanf("%s", ttpassword);
					outtextxy(510, 300, ttpassword);

				}
				nuser = 0;
				if (reg.x > 560 && reg.x < 695 && reg.y>360 && reg.y < 400)       //��¼��ť
				{
					for (int j = 0; j <= n; j++)
					{
						if (strcmp(tname, name[j]) == 0|| strcmp(tname, ttname) == 0)                  //�ж��˺��Ƿ����
						{
							if (strcmp(tpassword, password[j]) == 0|| strcmp(tpassword, ttpassword) == 0)      //�ж������Ƿ���ȷ
							{
								loadimage(&game, "./������Ϸ.jpg");
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
								pass = 1;            //��һ���ڽ���ѭ��
								break;
							}
							else
							{
								settextcolor(RGB(247, 206, 75));
								outtextxy(800, 320, "*�������������");
								Sleep(2000);
								Register();
							}
						}
						else
						{
							if (flag==1) {
								settextcolor(RGB(247, 206, 75));
								outtextxy(790, 235, "*�û������ڣ�������");
								Sleep(2000);
								Register();
							}
						}
					}
				}
				if (reg.x > 580 && reg.x < 750 && reg.y>430 && reg.y < 455)       //ע����ת
				{
					Login();
				}
			}
		}
	}
}

void Mainmenu() {                            //���˵�
	//Sign S;
	IMAGE menu;
	loadimage(&menu, "./��ҳ��.jpg");
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
					settextstyle(25, 0, "����");
					setbkmode(TRANSPARENT);
					outtextxy(465, 500, "^^������ʼ��ť�Լ���^^");
				}
				break;
			}
		}
	}Sleep(1000);
}

clock_t start, end;
int timesum;		//��ʱ��
int timeuse;		//������ʱ��
int timeleft;	//ʣ�µ�ʱ��

void Time(int x)     //����ʱ����
{
	start = clock();
	timesum = x;	//��ʱ���ʼ��
	timeuse = 0;	//������ʱ�����
	timeleft = 0;	//ʣ��ʱ���ʼ��

}


void Button() {
	int press = 1;

	while (press) {
		if (MouseHit()) {
			MOUSEMSG start = GetMouseMsg();
			switch (start.uMsg) {
			case WM_LBUTTONDOWN:
				if (start.x > 810 && start.x < 870 && start.y>110 && start.y < 155)       //��ʼ��ť
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
	settextstyle(25, 0, "����");
	setbkmode(TRANSPARENT);
	//��������
	loadimage(&bk, "./����.jpg");
	loadimage(&list, "./״̬��.jpg");
	putimage(0, 0, &bk);
	putimage(950, 0, &list);

	//���ﲿ��
	loadimage(&cha1, "./����1.jpg");
	loadimage(&hcha1, "./����1����.jpg");
	putimage(450, 22, &hcha1, NOTSRCERASE);
	putimage(450, 22, &cha1, SRCINVERT);

	//��ť���Ʋ���
		if (MouseHit()) {
			MOUSEMSG start = GetMouseMsg();
			switch (start.uMsg) {
			case WM_LBUTTONDOWN:
				if (start.x > 808 && start.x < 870 && start.y>110 && start.y < 150)       //��ʼ��ť
				{
					Button();
				}
			
			}
		}

	//���߲���
	Object* p;
	p = L;
	while (p != NULL)
	{
		if (p->exit) {
			switch (p->type) {
			case 1:
				loadimage(&gold, "./����.jpg");
				loadimage(&hgold, "./��������.jpg");
				putimage(p->x, p->y, &hgold, NOTSRCERASE);
				putimage(p->x, p->y, &gold, SRCINVERT);
				p->value = 60;
				hook.vx = 30;
				hook.vy = 30;
				break;
			case 2:
				loadimage(&mgold, "./С���.jpg");
				loadimage(&hmgold, "./С�������.jpg");
				putimage(p->x, p->y, &hmgold, NOTSRCERASE);
				putimage(p->x, p->y, &mgold, SRCINVERT);
				p->value = 30;
				hook.vx = 25;
				hook.vy = 25;
				break;
			case 3:
				loadimage(&stone, "./ʯͷ.jpg");
				loadimage(&hstone, "./ʯͷ����.jpg");
				putimage(p->x, p->y, &hstone, NOTSRCERASE);
				putimage(p->x, p->y, &stone, SRCINVERT);
				p->value = 10;
				hook.vx = 25;
				hook.vy = 25;

				break;
			case 4:
				loadimage(&sstone, "./Сʯͷ.jpg");
				loadimage(&hsstone, "./Сʯͷ����.jpg");
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

	//�������ȼ�����ʷ��¼����
	in.goal = in.level * 40;
	TCHAR s1[10], s2[10], s3[10];
	_stprintf_s(s1, "%d", in.goal);
	_stprintf_s(s2, "%d", in.score);
	_stprintf_s(s3, "%d", in.level);
	outtextxy(115, 55, s1);
	outtextxy(115, 18, s2);
	outtextxy(925, 55, s3);



	//����ʱ����
	TCHAR stime[10];
	end = clock();  		//ֹͣ��ʱ
	timeuse = (end - start) / CLK_TCK;
	timeleft = timesum - timeuse;
	settextcolor(RGB(122, 86, 4));
	settextstyle(25, 0, "����");
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
	if (hook.xf <= 15 || hook.xf >= 900 || hook.yf >= 600) {      //�����߿����
		hook.state = M_SHORT;
	}

	while (p != NULL) {                  //ѭ���жϸ�����Ʒ
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

	while (hook.state == M_SHORT) {                           //���ӻ���
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
		loadimage(&cha1, "./����2.jpg");
		loadimage(&hcha1, "./����2����.jpg");
		Sleep(20);
		putimage(450, 22, &hcha1, NOTSRCERASE);
		putimage(450, 22, &cha1, SRCINVERT);
		Sleep(300);
		cleardevice();
		if (flag == 0) {
			if (hook.yf <= 115 || p->y <= 95) { 		//��������ʼλ�ý���
				hook.state = M_NORMAL;
				p->exit = false;
			}
		}
		else if (flag == 1) {
			if (hook.yf <= 115) { 		//��������ʼλ�ý���
				hook.state = M_NORMAL;
			}
		}
	}
}

void Control(Ob& L) {                     //�����¼�ʵ�ַŹ���
	BeginBatchDraw();
	loadimage(&hok, "./����.jpg");
	loadimage(&hhok, "./��������.jpg");

	if (_kbhit()) {               //�Ź��Ӳ�ʵ�������쳤
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

				//���ﶯ̬Ч��
				line(hook.x + hook.length, hook.y, hook.xf + hook.length + 5, hook.yf);
				loadimage(&cha1, "./����2.jpg");
				loadimage(&hcha1, "./����2����.jpg");
				Sleep(20);
				putimage(450, 22, &hcha1, NOTSRCERASE);
				putimage(450, 22, &cha1, SRCINVERT);
				Sleep(300);

				cleardevice();

			}break;
		}
	}
	EndBatchDraw();  //��ֹƵ��
}

void Hook(Ob& L) {                         //���Ӱڶ�
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
		if (hook.angle > MAX_ANGLE)     //��else if��ͬʵ���ڽǶȷ�Χ�ڵİڶ�
		{
			hook.direction = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE)
		{
			hook.direction = RIGHT;
		}
		hook.xf = hook.x + sin(PI / 180 * hook.angle) * 35;
		hook.yf = hook.y + cos(PI / 180 * hook.angle) * 15;

		loadimage(&hok, "./����.jpg");
		loadimage(&hhok, "./��������.jpg");

		putimage(hook.xf, hook.yf, &hhok, NOTSRCERASE);
		putimage(hook.xf, hook.yf, &hok, SRCINVERT);

		setlinecolor(BLACK);
		setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 2);

		line(hook.x + hook.length, hook.y, hook.xf + hook.length + 5, hook.yf);
		Sleep(50);
		cleardevice();
	}
	EndBatchDraw();  //��ֹƵ��
}

void Initialize(Ob &L) {       //����Ʒ��ʼ��(����洢)
	srand(GetTickCount());
	//��ʼ����Ʒ��Ϣ
	//ʯͷ
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
	////���е�rand()%(n-m+1)+m����һ����ʽ����¼һ�·����Ժ���ġ�

	//�������10~30�����������

	//	srand((int)time(0));

	//int a = rand() % (21) + 10;


	int x = 455;                   //��ʼ������
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

void Pass() {//�����жϲ���
	IMAGE pass, npass;
	loadimage(&pass, "./����.jpg");
	loadimage(&npass, "./����ʧ��.jpg");

	if (timeleft == 0) {
		if (in.goal <= in.score) {        //�ɹ�
			putimage(0, 0, &pass);
			in.level++;
			Sleep(3000);
		}
		else {                            //ʧ��
			next = 0;
			putimage(0, 0, &npass);
			Sleep(3000);
			S.Save();
		}
	}
}

int main() { 
	Ob L;//���������
	initgraph(1100, 618, SHOWCONSOLE);
	S.Read();
	int time = 65;
	Mainmenu();            //��ҳ����ʾ
	setrop2(R2_COPYPEN);
	while (next) {
		Initialize(L);          //��Ϸ�����ʼ��
		time -= 5;
		Time(time);
		Hook(L);
		Pass();
	}
		closegraph();
	return 0;
}

