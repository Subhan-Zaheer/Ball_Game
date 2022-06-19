#include<unistd.h>
#include <time.h>
#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>
#include<termios.h>
#include<fcntl.h>
 /* Game code*/

const int height = 20;
const int width = 40;

//enum ball_direction
//{

//	Stop = 0, Right = 1, Left = 2, Up_right = 3, Down_right = 4, Up_left = 5, Down_left = 6

//};
 int direction;

int ball_x_coordinate;
int ball_y_coordinate;

int pad_1_y_coordinate[4];
int pad_2_y_coordinate[4];

int pad_1_x_coordinate;
int pad_2_x_coordinate;

int gameOver = 0;
int movement = 0;

int player_1_score = 0;
int player_2_score = 0;

void* initial();
void* structure();
void* pad_movement();
void* pad_logic();
void* ball_movement();
void* ball_logic();
void* logic();
void* pad_hit_logic();
void* game();
int _kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}


int main()
{
	pthread_t play;
	pthread_create(&play,NULL,game,NULL);
	pthread_join(play,NULL);
	pthread_t padMove;
	pthread_create(&padMove,NULL,pad_movement,NULL);
	pthread_join(padMove,NULL);
	/*thread gameLogic(logic);
	gameLogic.join();*/
	

}

void* game() {
	initial();
	/*thread draw(structure);
	draw.join();*/

	while (!gameOver)

	{

		structure();
		//pad_movement();
		logic();
		//Sleep(500) ;

	}
}

void* initial()

{

	srand((unsigned)time(0));

	direction = 0;

	int a = 9;

	ball_x_coordinate = width / 2;
	ball_y_coordinate = height / 2;

	for (int i = 0; i < 4; i++)
	{

		pad_1_y_coordinate[i] = a;
		pad_2_y_coordinate[i] = a++;

	}

	pad_1_x_coordinate = 1;
	pad_2_x_coordinate = width - 1;

}

void* structure()

{

	system("clear");

	for (int i = 1; i < width + 2; i++)
		printf("\xDC");

	printf("\n");

	for (int i = 1; i <= height; i++)

	{

		for (int j = 1; j <= width; j++)

		{

			if (j == 1)
				printf("\xDD");

			if (i == ball_y_coordinate && j == ball_x_coordinate)
				printf("O");

			else

			{

				int print = 0;

				for (int k = 0; k < 4; k++)
					if ((i == pad_1_y_coordinate[k] && j == pad_1_x_coordinate) || (i == pad_2_y_coordinate[k] && j == pad_2_x_coordinate))
					{

						printf("\xB2");
						print = 1;

					}

				if (!print)
					printf(" ");

			}

			if (j == width - 1)
				printf("\xDE");

		}

		printf("\n");

	}

	for (int i = 1; i < width + 2; i++)
		printf("\xDF");
	sleep(1);	

}

void* pad_movement()

{
	

	if (_kbhit())
		switch (getchar())
		{

		case 'w':
		case 'W':

			for (int i = 0; i < 4; i++)
				pad_1_y_coordinate[i]--;

			if (!movement)

			{

				direction = ((rand() % 6) + 1);
				movement = 1;

			}



			break;

		case 's':
		case 'S':

			for (int i = 0; i < 4; i++)
				pad_1_y_coordinate[i]++;

			if (!movement)

			{

				direction = ((rand() % 6) + 1);
				movement = 1;

			}

			break;

		case 'i':
		case 'I':

			for (int i = 0; i < 4; i++)
				pad_2_y_coordinate[i]--;

			if (!movement)

			{

				direction = ((rand() % 6) + 1);
				movement = 1;

			}

			break;

		case 'k':
		case 'K':

			for (int i = 0; i < 4; i++)
				pad_2_y_coordinate[i]++;

			if (!movement)

			{

			direction = ((rand() % 6) + 1);
				movement = 1;

			}

			break;

		default:

			if (!movement)

			{

				direction = ((rand() % 6) + 1);
				movement = 1;

			}

			break;

		}

}

void* pad_logic()

{

	pad_movement();

	int check_array_down[4] = { 17,18,19,20 };
	int check_array_up[4] = { 1,2,3,4 };

	for (int i = 0; i < 4; i++)

	{

		if (pad_1_y_coordinate[3] < 4)
			pad_1_y_coordinate[i] = check_array_up[i];

		else if (pad_1_y_coordinate[3] > 20)
			pad_1_y_coordinate[i] = check_array_down[i];

		if (pad_2_y_coordinate[3] < 4)
			pad_2_y_coordinate[i] = check_array_up[i];

		else if (pad_2_y_coordinate[3] > 20)
			pad_2_y_coordinate[i] = check_array_down[i];

	}

}

void* ball_logic()

{

	pad_hit_logic();
	ball_movement();

	if (ball_x_coordinate == 0)
	{

		initial();
		player_2_score++;
		movement = 0;

	}

	else if (ball_x_coordinate == width)

	{

		initial();
		player_1_score++;
		movement = 0;

	}

	int direction_change = 0;

	if (ball_y_coordinate == 20 && direction == 6)
	{

		direction = 5;
		direction_change = 1;

	}

	else if (ball_y_coordinate == 20 && direction == 4)
	{

		direction = 3;
		direction_change = 1;

	}

	if (ball_y_coordinate == 1 && direction == 5)
	{

		direction = 6;
		direction_change = 1;

	}

	else if (ball_y_coordinate == 1 && direction == 3)
	{

		direction = 4;
		direction_change = 1;

	}

	if (direction_change)
		pad_hit_logic();

}

void* ball_movement()

{

	switch (direction)

	{

	case 1:

		ball_x_coordinate++;

		break;

	case 2:

		ball_x_coordinate--;

		break;

	case 3:

		ball_y_coordinate--;
		ball_x_coordinate++;

		break;

	case 4:

		ball_y_coordinate++;
		ball_x_coordinate++;

		break;

	case 5:

		ball_y_coordinate--;
		ball_x_coordinate--;

		break;

	case 6:

		ball_y_coordinate++;
		ball_x_coordinate--;

		break;
	}

}

void* pad_hit_logic()

{

	if (ball_x_coordinate - 1 == pad_1_x_coordinate && ball_y_coordinate == pad_1_y_coordinate[0])
	{

		if (direction == 2)
			direction = 3;

		else if (direction == 5)
			direction = 3;

		else
			direction = 3;

	}

	else if (ball_x_coordinate - 1 == pad_1_x_coordinate && ball_y_coordinate == pad_1_y_coordinate[1])
	{

		if (direction == 2)
			direction = 1;

		else if (direction == 6)
			direction = 1;

		else
			direction = 3;

	}

	else if (ball_x_coordinate - 1 == pad_1_x_coordinate && ball_y_coordinate == pad_1_y_coordinate[2])
	{

		if (direction == 2)
			direction = 1;

		else if (direction == 6)
			direction = 4;

		else
			direction = 1;

	}

	else if (ball_x_coordinate - 1 == pad_1_x_coordinate && ball_y_coordinate == pad_1_y_coordinate[3])
	{

		if (direction == 2)
			direction = 4;

		else if (direction == 5)
			direction = 4;

		else
			direction = 4;

	}

	if (ball_x_coordinate + 1 == pad_2_x_coordinate && ball_y_coordinate == pad_2_y_coordinate[0])
	{

		if (direction == 1)
			direction = 5;

		else if (direction == 3)
			direction = 5;

		else
			direction = 5;

	}

	else if (ball_x_coordinate + 1 == pad_2_x_coordinate && ball_y_coordinate == pad_2_y_coordinate[1])
	{

		if (direction == 1)
			direction = 2;

		else if (direction == 4)
			direction = 2;

		else
			direction = 6;

	}

	else if (ball_x_coordinate + 1 == pad_2_x_coordinate && ball_y_coordinate == pad_2_y_coordinate[2])
	{

		if (direction == 1)
			direction = 2;

		else if (direction == 4)
			direction = 6;

		else
			direction = 2;

	}

	else if (ball_x_coordinate + 1 == pad_2_x_coordinate && ball_y_coordinate == pad_2_y_coordinate[3])
	{

		if (direction == 1)
			direction = 6;

		else if (direction == 3)
			direction = 6;

		else
			direction = 6;

	}


}

void* logic()

{
	pthread_t ballLogic;
	pthread_create(&ballLogic,NULL,ball_logic,NULL);
	pthread_t padLogic;
	pthread_create(&padLogic,NULL,pad_logic,NULL);
	pthread_join(ballLogic,NULL);
	pthread_join(padLogic,NULL);
		//ball_logic();
		//pad_logic();
		printf("\nPlayer 1 : %d\nPlayer 2 : %d", player_1_score, player_2_score);
	
	
}
