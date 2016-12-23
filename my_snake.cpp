#include "stdafx.h"
#include "my_snake.h"
#include <time.h>

/* 宏定义 */
/************************************************************************/


//颜色
#define RED      1.0f, 0.0f, 0.0f
#define GREEN    0.0f, 1.0f, 0.0f
#define BLUE     0.0f, 0.0f, 1.0f


//布局大小
#define MAP_LENGTH   20
#define MAP_HEIGHT    20


//方向
#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#define GAME_SPEED 400


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
	{ RED },						//color[0]
	{ BLUE },						//color[1]
	{ GREEN },						//...
{ 0.5,0.5f,0.5f },
{ 0.8f,0.8f,0.0f }
};

Snake snake;						//蛇
Food food;							//食物

int current_direction = RIGHT;				//移动方向 默认向右
int control_direction;


int direction_x[] = { 0,0,-1,1 };
int direction_y[] = { 1,-1,0,0 };

int is_pause = 0;						//暂停标志



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
		node->color = i % 4;								//node颜色

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
	do
	{
		srand(time(NULL));									//随机数种子
		int x = rand() % MAP_LENGTH ;				     	//随机产生 x y
		int y = rand() % MAP_HEIGHT ;
		food.pos = { x,y };									//食物坐标赋值
		food.color = rand() % 4 ;							//随机颜色

	} while (in_snake(food.pos));							//in_snake 返回1 重新生成
}



//死亡条件 下一步是否在蛇身
int is_died()
{
	int x = snake.head->pos.x + direction_x[current_direction];
	int y = snake.head->pos.y + direction_y[current_direction];

	for (Node* node = snake.head; node != NULL; node = node->next)
	{
		if (node->pos.x == x && node->pos.y == y)							//接下来的蛇头在蛇身
		{
			//死亡


			return 1;
		}
	}

	return 0;																//正常状态
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
	int head_color = snake.head->color;
	Node* head = snake.head;
	for (Node* node = snake.head->next; node != NULL; node = node->next )
	{
		node->pre->color = node->color;										//颜色前移一位

	}

	snake.tail->pos.x = head->pos.x + direction_x[direction];				//坐标变为头的下一步
	snake.tail->pos.y = head->pos.y + direction_y[direction];
	snake.tail->color = head_color;


	Node* new_tail = snake.tail->pre;
	new_tail->next = NULL;												    //倒数第二 成为新尾
	snake.tail->pre = NULL;
	snake.tail->next = head;
	snake.head->pre = snake.tail;											//尾变头

	snake.head = snake.tail;												//更新头部
	snake.tail = new_tail;												   //更新尾巴


	//蛇头撞墙
	if (snake.head->pos.x < 0)												//左边撞墙 蛇头从右边出来
	{
		snake.head->pos.x = MAP_LENGTH;
	}

	if (snake.head->pos.x > MAP_LENGTH)
	{
		snake.head->pos.x = 0;												//右边撞墙  蛇头从左边出来
	}

	if (snake.head->pos.y < 0 )												//下边撞墙  从上面出来
	{
		snake.head->pos.y = MAP_HEIGHT;
	}

	if (snake.head->pos.y > MAP_HEIGHT)										//上边出墙 从下面出来
	{
		snake.head->pos.y = 0;
	}

	current_direction = direction;											//更新当前方向
}




void display()
{
	float x_unit = 20.0f / MAP_LENGTH;
	float y_unix = 20.0f / MAP_HEIGHT;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();														//原点回到窗口中心


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);								//绘制线
	gluOrtho2D(0.0f, 20.0f, 0.0f, 20.0f);									//视角裁剪


	//绘制网格
	glBegin(GL_QUADS);
		glColor3f(BLUE);
		for (int i = 0; i < MAP_HEIGHT; i++)
		{
			for (int j = 0; j < MAP_LENGTH; j++)
			{
				glVertex3f(j * x_unit, i * y_unix, 0.0f);
				glVertex3f(j * x_unit, (i+1) * y_unix, 0.0f);
				glVertex3f((j+1) * x_unit, (i+1) * y_unix, 0.0f);
				glVertex3f((j+1) * x_unit, i * y_unix, 0.0f);
			}
		}
	glEnd();
	//结束网格绘制

	//填充模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//绘制蛇身
	glBegin(GL_QUADS);
		for(Node* node = snake.head; node != NULL; node = node->next)
		{
			int c = node->color;
			glColor3f(color[c].r, color[c].g, color[c].b);
			glVertex3f(node->pos.x * x_unit, node->pos.y * y_unix, 0.0f);
			glVertex3f(node->pos.x * x_unit, (node->pos.y+1) * y_unix, 0.0f);
			glVertex3f((node->pos.x+1) * x_unit, (node->pos.y+1) * y_unix, 0.0f);
			glVertex3f((node->pos.x + 1) * x_unit, node->pos.y * y_unix, 0.0f);

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

void InitGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

}


//方向键回调
void change_direction(int key, int x, int y)
{
	if (is_pause)
	{
		return;
	}


	switch (key)
	{
		case GLUT_KEY_UP:
			if (current_direction != DOWN)
			{
				control_direction = UP;
			}
			break;;
		case GLUT_KEY_DOWN:
			if (current_direction != UP)
			{
				control_direction = DOWN;
			}
			break;
		case GLUT_KEY_LEFT:
			if (current_direction != RIGHT)
			{
				control_direction = LEFT;
			}
			break;
		case GLUT_KEY_RIGHT:
			if (current_direction != LEFT)
			{
				control_direction = RIGHT;

			}
	default:
		break;
	}
}


//普通按键消息 处理暂停
void key_pressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		
	default:


		break;
	}

}

//定时回调
void onTimer(int value)
{
	if (!value && !is_pause)
	{
		if (next_is_food())
		{
			eat_food(food);
			generate_food();
		}

		snake_move(control_direction);

		glutTimerFunc(GAME_SPEED, onTimer, 0);						//重复调用

	}
}



int main(int argc, char** argv)
{
	init_snake(3);													//蛇的初始化

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);				
	glutInitWindowSize(600, 600);
	glutCreateWindow("hello");

	InitGL();

	glutDisplayFunc(display);										//绘制
	glutIdleFunc(display);											

	glutKeyboardFunc(key_pressed);									//普通按键回调
	glutSpecialFunc(change_direction);								//方向键回调
		
	glutTimerFunc(GAME_SPEED, onTimer, 0);

	glutMainLoop();
	return 0;

}
