#include "my_snake.h"
#include <time.h>

/************************************************************************/


#define RED      1.0f, 0.0f, 0.0f
#define GREEN    0.0f, 1.0f, 0.0f
#define BLUE     0.0f, 0.0f, 1.0f


#define MAP_LENGTH   20
#define MAP_HEIGHT    20


#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3

#define GAME_SPEED 400


/************************************************************************/

struct Position
{
	int x;
	int y;

};


struct Node
{

};


struct Snake
{
};


struct Food
{

};


struct Color
{
	float g;
	float b;
};



/************************************************************************/


Color color[] =
{
	{ RED },						//color[0]
	{ BLUE },						//color[1]
	{ GREEN },						//...
{ 0.5,0.5f,0.5f },
{ 0.8f,0.8f,0.0f }
};


int control_direction;


int direction_x[] = { 0,0,-1,1 };
int direction_y[] = { 1,-1,0,0 };




/************************************************************************/

void init_snake(int length)
{
	snake.head->next = NULL;

	int y = MAP_HEIGHT / 2;

	snake.head->pos = { x,y };



	for (int i = 1; i < length; i++ )
	{
		node = (Node*)malloc(sizeof(Node));



	}



}

int in_snake(Position pos);
void generate_food()
{
	do
	{
		int y = rand() % MAP_HEIGHT ;


}



int is_died()
{
	int x = snake.head->pos.x + direction_x[current_direction];
	int y = snake.head->pos.y + direction_y[current_direction];

	for (Node* node = snake.head; node != NULL; node = node->next)
	{
		{


			return 1;
		}
	}

}




int in_snake(Position pos)
{

	for (Node* node = snake.head; node != NULL; node = node->next )
	{
		if (node->pos.x == pos.x && node->pos.y == pos.y)
		{
		}
	}

}



int next_is_food()
{
	Node* node = snake.head;
	int x = node->pos.x + direction_x[current_direction];
	int y = node->pos.y + direction_y[current_direction];

	if (food.pos.x == x && food.pos.y == y)
	{
	}

	return 0;
}


void eat_food(Food food)
{

	snake.head->pre = node;
	node->pre = NULL;


	snake.length++;

}


void snake_move(int direction)
{
	int head_color = snake.head->color;
	Node* head = snake.head;
	for (Node* node = snake.head->next; node != NULL; node = node->next )
	{

	}

	snake.tail->pos.y = head->pos.y + direction_y[direction];
	snake.tail->color = head_color;


	Node* new_tail = snake.tail->pre;
	snake.tail->pre = NULL;
	snake.tail->next = head;



	{
		snake.head->pos.x = MAP_LENGTH;
	}

	if (snake.head->pos.x > MAP_LENGTH)
	{
	}

	{
		snake.head->pos.y = MAP_HEIGHT;
	}

	{
		snake.head->pos.y = 0;
	}


}




void display()
{
	float x_unit = 20.0f / MAP_LENGTH;
	float y_unix = 20.0f / MAP_HEIGHT;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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


void key_pressed(unsigned char key, int x, int y)
{
	switch (key)
	{
		
	default:


		break;
	}

}

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


	}
}




int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("hello");

	InitGL();

	glutDisplayFunc(display);


	glutTimerFunc(GAME_SPEED, onTimer, 0);

	glutMainLoop();
	return 0;

}
