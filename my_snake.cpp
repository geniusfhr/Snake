#include "stdafx.h"
#include "my_snake.h"
#include <time.h>
#include <windows.h>

/* 宏定义 */
/************************************************************************/

//颜色
#define RED      1.0f, 0.0f, 1.0f
#define GREEN    0.0f, 1.0f, 0.0f
#define BLUE     0.0f, 0.0f, 1.0f
#define YELLOW   1.0f, 1.0f, 0.0f
#define WHITE    1.0f, 1.0f, 1.0f 


//布局大小
#define MAP_LENGTH   20
#define MAP_HEIGHT    20


//方向
#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#define ESCAPE 27


/* 结构体类型定义 */
/************************************************************************/

//坐标Position定义
struct Position
{
	int x;
	int y;

};


//蛇身节点定义
struct Node  
{
	Position pos;					//坐标
	Node* pre;						//指向前节点
	Node* next;						//指向后节点
	int color;						//蛇身颜色

};


//蛇定义
struct Snake
{
	Node* head;						//指向蛇头
	Node* tail;						//指向蛇尾
	int length;						//蛇身长度
};


//食物定义
struct Food
{
	Position pos;					//食物坐标
	int color;						//食物颜色

};


//颜色定义
struct Color
{
	float r;						// rgb分量确定
	float g;
	float b;
};



/* 全局变量定义 */
/************************************************************************/

//颜色数组
Color color[] =
{
	{ RED    },						//color[0]
	{ GREEN   },						//color[1]
	{ BLUE  },						//...
	{ YELLOW },
	{ WHITE  }
};

Snake snake;						//蛇
Food food;							//食物

int current_direction = RIGHT;				//移动方向 默认向右
int control_direction;


int direction_x[4] = { 0,0,-1,1 };
int direction_y[4] = { 1,-1,0,0 };



int is_pause = 0;						//暂停标志 
int is_finish = 0;						//结束标志

int glut_window;								//glut窗口id

int game_speed = 500;





/************************************************************************/

//蛇身初始化  length
void init_snake(int length) 
{
	snake.head = (Node*)malloc(sizeof(Node));				//初始化蛇头
	snake.head->color = 0;									//蛇头颜色
	snake.head->pre = NULL;									//蛇头前后指向为空
	snake.head->next = NULL;
	
	int x = MAP_LENGTH / 2;									//蛇头位置
	int y = MAP_HEIGHT / 2;									

	snake.head->pos = { x,y };

	Node* pre_node = snake.head;							//指向前一蛇身的指针
	Node* node;												//指向当前蛇身的指针 临时变量

	for (int i = 1; i < length; i++ )
	{
		node = (Node*)malloc(sizeof(Node));
		node->pre = pre_node;								//向前连接
		node->next = NULL;									//后为空
		pre_node->next = node;								//pre指向node
		node->color = i % 5;								//node颜色   0 -- 4 依次
		
		node->pos.x = pre_node->pos.x - 1;					//横坐标-1
		node->pos.y = pre_node->pos.y;						//纵坐标不变

		pre_node = node;									//更新pre 设为当前node		

	}

	snake.tail = pre_node;									//设置蛇尾
	snake.length = length;									//蛇身长度

	generate_food();										//随机生成食物
	
}

int in_snake(Position pos);
//随机生成食物
void generate_food()
{
	srand(time(NULL));
	
	do
	{
		int x = 1 + rand() % (MAP_LENGTH - 2);					//随机产生 x y		
		int y = 1 + rand() % (MAP_HEIGHT - 2);
		food.pos = { x,y };									//食物坐标赋值
		food.color = rand() % 5;						    //随机颜色

	} while (in_snake(food.pos));							//重新生成
	
							
	
}



//死亡条件 下一步是否在蛇身 
int is_died()
{
	int x = snake.head->pos.x + direction_x[current_direction];
	int y = snake.head->pos.y + direction_y[current_direction];

	Position pos = { x,y };
	
	return in_snake(pos);
	
	
}


//判断pos是否在蛇身上  1 -- 在   0 -- 不在
int in_snake(Position pos)
{
	for (Node* node = snake.head; node != NULL; node = node->next )
	{
		if (node->pos.x == pos.x && node->pos.y == pos.y)
		{
			return 1;									//return 1 在蛇身上
		}
	}

	return 0;											//不在
}


//判断下一个是否是食物
int next_is_food()
{
	Node* node = snake.head;
	int x = node->pos.x + direction_x[current_direction];
	int y = node->pos.y + direction_y[current_direction];
	
	if (food.pos.x == x && food.pos.y == y)
	{
		return 1;											//下一个移动方向是食物				
	}
	
	return 0;
}


//吃掉食物
void eat_food(Food food)
{
	Node* node = (Node*)malloc(sizeof(Node));				//食物节点做头
	node->pos = food.pos;									//食物节点的坐标为蛇头坐标
	node->color = food.color;								//更新颜色

	node->next = snake.head;								//连接食物和蛇头
	snake.head->pre = node;
	node->pre = NULL;

	snake.head = node;										//更新食物为蛇头

	snake.length++;

}


//贪吃蛇向 direction 方向移动
void snake_move(int direction)
{
	int head_x = snake.head->pos.x + direction_x[direction];
	int head_y = snake.head->pos.y + direction_y[direction];


	//蛇头撞墙
	if (head_x == 0 || head_x == MAP_LENGTH - 1 || head_y == 0 || head_y == MAP_HEIGHT - 1)
	{
		is_finish = 1;
		return;

	}
	
	int head_color = snake.head->color;
	Node* head = snake.head;
	for (Node* node = snake.head->next; node != NULL; node = node->next )					
	{
		node->pre->color = node->color;					//颜色前移一位
	}

	snake.tail->pos.x = head->pos.x + direction_x[direction];			//坐标变为头的下一步
	snake.tail->pos.y = head->pos.y + direction_y[direction];
	snake.tail->color = head_color;
	

	Node* new_tail = snake.tail->pre;
	new_tail->next = NULL;									    //倒数第二 成为新尾


	snake.tail->pre = NULL;									
	snake.tail->next = head;
	snake.head->pre = snake.tail;								//尾变头

	snake.head = snake.tail;									//更新头部
	snake.tail = new_tail;								        //更新尾巴
	

}

void drawString(const char* str) //屏幕显示字体  
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) {
		isFirstCall = 0;
		// 申请MAX_CHAR个连续的显示列表编号  
		lists = glGenLists(128);
		// 把每个字符的绘制命令都装到对应的显示列表中  
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 128, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符  
	for (; *str != '\0'; ++str) {
		glCallList(lists + *str);
	}
}


char int_to_char(int i )
{
	return '0' + i;

}


void drawInformation()
{
	glColor3f(WHITE);

	glRasterPos2i(23, 10);  //起始位置  
	drawString("Current Food Position: ");   //输出的字符串
	
	char x1 = int_to_char(food.pos.x / 10);
	char x2 = int_to_char(food.pos.x % 10);

	char y1 = int_to_char(food.pos.y / 10);
	char y2 = int_to_char(food.pos.y % 10);

	char food_position[] = { x1,x2,' ',y1,y2,'\0' };


	glRasterPos2f(23.5, 9);  //起始位置  
	drawString(food_position);   //输出的字符串 



	glRasterPos2f(23, 15);			//起始位置  
	drawString("Speed Control:");   //输出的字符串 
	

	glRasterPos2f(23.5, 14);
	drawString("1 -- UP");
	

	glRasterPos2f(23.5, 13);
	drawString("0 -- DOWN");

	glRasterPos2f(23, 8);
	drawString("Current Score:");

	glRasterPos2f(23.5, 7);
	
	char s1 = int_to_char(snake.length / 10);
	int s2 = int_to_char(snake.length % 10);
	char s[] = { s1,s2,'\0' };

	drawString(s);

	if (is_finish)
	{
		glRasterPos2f(23, 3);											//起始位置  
		drawString("*** GAME OVER ****");										//游戏结束信息
	}

}


void display()
{
	static int flag = 1;
	
	float x_unit = 20.0f / MAP_LENGTH;
	float y_unit = 20.0f / MAP_HEIGHT;
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();								//原点回到窗口中心
	
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//绘制线
	gluOrtho2D(0.0f, 30.0f, 0.0f, 20.0f);			//视角裁剪

	drawInformation();										//绘制右侧信息面板

		// feel fine here enjoy programming
	glBegin(GL_QUADS);
		glColor3f(WHITE);
		glVertex3f(0.0f, 0.0f, 0.0f);											//下边界
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(MAP_LENGTH*x_unit, 1.0f, 0.0f);
		glVertex3f(MAP_LENGTH * x_unit, 0.0f, 0.0f);

		glVertex3f(0.0f, (MAP_HEIGHT - 1)*y_unit, 0.0f);							//上边界
		glVertex3f(0.0f, MAP_HEIGHT, 0.0f);
		glVertex3f(MAP_LENGTH * x_unit, MAP_HEIGHT * y_unit, 0.0f);
		glVertex3f(MAP_LENGTH * x_unit, (MAP_HEIGHT - 1) * y_unit, 0.0f);

		glVertex3f(0.0f, 0.0f, 0.0f);											//左边界
		glVertex3f(0.0f, MAP_HEIGHT*y_unit, 0.0f);
		glVertex3f(1 * x_unit, MAP_HEIGHT * y_unit, 0.0f);
		glVertex3f(1 * x_unit, 0.0f, 0.0f);

		glVertex3f((MAP_LENGTH - 1)*x_unit, 0.0f, 0.0f);						    //右边界
		glVertex3f((MAP_LENGTH - 1)*x_unit, MAP_HEIGHT*y_unit, 0.0f);
		glVertex3f(MAP_LENGTH * x_unit, MAP_HEIGHT*y_unit, 0.0f);
		glVertex3f(MAP_LENGTH * x_unit, 0.0f, 0.0f);
	glEnd();



	//填充模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//绘制蛇身
	glBegin(GL_QUADS);
		for(Node* node = snake.head; node != NULL; node = node->next)
		{
			int c = node->color;
			glColor3f(color[c].r, color[c].g, color[c].b);
			glVertex3f(node->pos.x * x_unit, node->pos.y * y_unit, 0.0f);
			glVertex3f(node->pos.x * x_unit, (node->pos.y+1) * y_unit, 0.0f);
			glVertex3f((node->pos.x+1) * x_unit, (node->pos.y+1) * y_unit, 0.0f);
			glVertex3f((node->pos.x + 1) * x_unit, node->pos.y * y_unit, 0.0f);

		}
	glEnd();


	//绘制食物
	glBegin(GL_QUADS);
		int fc = food.color;
		glColor3f(color[fc].r, color[fc].g, color[fc].b);
		glVertex3f(food.pos.x, food.pos.y, 0.0f);
		glVertex3f(food.pos.x, food.pos.y+1, 0.0f);
		glVertex3f(food.pos.x+1, food.pos.y+1, 0.0f);
		glVertex3f(food.pos.x+1, food.pos.y, 0.0f);
	glEnd();

	
	glutSwapBuffers();

}
void InitGL()          // We call this right after our OpenGL window is created.  
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // This Will Clear The Background Color To Black  
	glClearDepth(1.0);              // Enables Clearing Of The Depth Buffer  
	glDepthFunc(GL_LESS);               // The Type Of Depth Test To Do  
	glEnable(GL_DEPTH_TEST);            // Enables Depth Testing  
	glShadeModel(GL_SMOOTH);            // Enables Smooth Color Shading  

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();               // Reset The Projection Matrix  

	
}


//方向键回调
void change_direction(int key, int x, int y)
{
	if (is_pause)						//暂停状态
	{
		return;
	} 

	switch (key)
	{
		case GLUT_KEY_UP:
			if (current_direction != DOWN)
			{
				current_direction = UP;
			}
			break;;
		case GLUT_KEY_DOWN:
			if (current_direction != UP)
			{
				current_direction = DOWN;
			}
			break;
		case GLUT_KEY_LEFT:
			if (current_direction != RIGHT)
			{
				current_direction = LEFT;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (current_direction != LEFT)
			{
				current_direction = RIGHT;
				
			}
	default:
		break;
	}

}

void onTimer(int value);

//普通按键消息 处理暂停
void key_pressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		//PAUSE
	case ESCAPE:
		glutDestroyWindow(glut_window);
		exit(0);
		break;

	case '1':
		if (game_speed >= 200)
			game_speed = game_speed - 50;
		break;

	case '0':
		if (game_speed <= 1000)
			game_speed = game_speed + 50;
		break;

	case ' ':
		is_pause = !is_pause;							//空格切换暂停状态
		if (is_pause == 0)								//继续运行
		{
			glutTimerFunc(game_speed, onTimer, is_finish);		//继续调用
		}
		break;

	default:
		break;
	}
}


void onTimer(int value)
{
	if (!value && !is_pause)
	{
		if (next_is_food())	{
			eat_food(food);
			generate_food();
		}
		
		if (is_died()) {
			is_finish = 1;

		} else {
			snake_move(current_direction);
		}
		
		glutTimerFunc(game_speed, onTimer, is_finish);					//重复调用

	}
}


int main(int argc, char** argv)
{
	init_snake(2);													//蛇的初始化

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 800);

	glut_window = glutCreateWindow("SNAKE");								//绘制窗口
	
	InitGL();																//初始化openGL
	
	glutDisplayFunc(display);
	glutIdleFunc(display);													//重复绘制

	glutKeyboardFunc(key_pressed);
	glutSpecialFunc(change_direction);

	glutTimerFunc(0, onTimer, 0);											//定时器
	
	glutMainLoop();
	return 0;

}





















