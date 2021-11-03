#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <ev3.h>
#include "Proj1FinalDemo.h"
double tile_length = 0.305; /* length of tile */
int manhattan_grid[13][19]; /* dimensions for grid */
int grid_width = sizeof(manhattan_grid)/sizeof(manhattan_grid[0]); //10
int grid_length = sizeof(manhattan_grid[0])/sizeof(manhattan_grid[0][0]); //16
char path_instruct[160];
char orientation = 'n';
int increment = 0;
Tone a[2] = {
		{TONE_C4, NOTE_HALF},
		{TONE_F6, NOTE_EIGHT}
};

void moveforward()
{

	SetDirection(OUT_A, OUT_FWD);
	SetDirection(OUT_D, OUT_FWD);
	RotateMotor(OUT_AD, 10, 1);
	RotateMotor(OUT_AD, 10, 500 );
	return;
}

//Clockwise turn
void turnright()
{
	SetDirection(OUT_A, OUT_FWD);
	SetDirection(OUT_D, OUT_REV);
	RotateMotorNoWait(OUT_A, 10, 167);
	RotateMotorNoWait(OUT_D, 10, 167);
	Wait(3000);

	switch(orientation)
	{
		case 'n':{
			orientation = 'e';
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'e':{
			orientation = 's';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 's':{
			orientation = 'w';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'w':{
			orientation = 'n';
			PlayTones(a);
			break;
		}
	}
}

void turnleft()
{
	SetDirection(OUT_A, OUT_REV);
	SetDirection(OUT_D, OUT_FWD);
	RotateMotorNoWait(OUT_A, 10, 167);
	RotateMotorNoWait(OUT_D, 10, 167);
	Wait(3000);

	switch(orientation)
	{
		case 'n':{
			orientation = 'w';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'e':{
			orientation = 'n';
			PlayTones(a);
			break;
		}
		case 's':{
			orientation = 'e';
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'w':{
			orientation = 's';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
	}
}

void grid_setup()
{
    // Set up grid
    int y, x;
    for (y = 0; y < grid_width; y++) {
        for (x = 0; x < grid_length; x++) {
            manhattan_grid[y][x] = -1;
            //Build the wall
            if (x == 0 || x == grid_length - 1)
                    manhattan_grid[y][x] = 99;
            if (y == 0 || y == grid_width - 1)
                    manhattan_grid[y][x] = 99;
        }
    }
    //printf("Grid set up to -1.\n");

    // Set up start and goal
    int x_start = (int) ceil(start[0]/tile_length);
    int y_start = (int) ceil(start[1]/tile_length);
    //manhattan_grid[y_start][x_start] = 98;
    manhattan_grid[(grid_width-y_start)-2][x_start+1] = 98;

    int x_goal = (int) ceil(goal[0]/tile_length);
    int y_goal = (int) ceil(goal[1]/tile_length);
    manhattan_grid[(grid_width-y_goal)-2][x_goal+1] = 0;
    printf("Start and goal set up.\n");

    // Set up obstacles
    int x_obs = 0;
    int y_obs = 0;
    int k;
    for (k = 0; k < num_obstacles; k++) {
        x_obs = (int) ceil(obstacle[k][0]/tile_length);
        y_obs = (int) ceil(obstacle[k][1]/tile_length);
        manhattan_grid[(grid_width-y_obs)-2][x_obs+1] = 99;
    }
    printf("Obstacles set up.\n");
}

void print_grid()
{
    int y, x;
    for (y = 0; y < grid_width; y++) {
        for (x = 0; x < grid_length; x++) {
            printf("%3d ", manhattan_grid[y][x]);
        }
        printf("\n");
    }
}

void propagation()
{
    int current = 0;
    int y, x;
    while(current<(grid_length*grid_width))
    {
        for (y = 0; y < grid_width; y++)
        {
            for (x = 0; x < grid_length; x++)
            {
                if(manhattan_grid[y][x]==current)
                {
                    if(manhattan_grid[y+1][x]==-1)
                        manhattan_grid[y+1][x]=current+1;
                    if(manhattan_grid[y-1][x]==-1)
                        manhattan_grid[y-1][x]=current+1;
                    if(manhattan_grid[y][x-1]==-1)
                        manhattan_grid[y][x-1]=current+1;
                    if(manhattan_grid[y][x+1]==-1)
                        manhattan_grid[y][x+1]=current+1;
                }
            }
            printf("\n");
        }
        current++;
    }
}

void pathfinding()
{
    int x_start = (int) ceil(start[0]/tile_length);
    int y_start = (int) ceil(start[1]/tile_length);

    int x = (grid_width-y_start)-2;
    int y = x_start+1;

    int top, right, bottom, left;
    int curr_pos[2] = {x,y};
    int currentvalue = 98;

    while(currentvalue!=0)
    {
        x = curr_pos[0];
        y = curr_pos[1];
        top = manhattan_grid[x-1][y];
        right = manhattan_grid[x][y+1];
        bottom = manhattan_grid[x+1][y];
        left = manhattan_grid[x][y-1];

        if(top<=bottom && top<=right &&  top<=left) //Going up
        {
            curr_pos[0] = x-1;
            curr_pos[1] = y;
            currentvalue = manhattan_grid[x-1][y];
            manhattan_grid[x-1][y] = 98;
            path_instruct[increment] = 'n';
            increment++;

        }
        else if(right<=top  && right<=left && right<=bottom) //Going right
        {
            curr_pos[0] = x;
            curr_pos[1] = y+1;
            currentvalue = manhattan_grid[x][y+1];
            manhattan_grid[x][y+1] = 98;
            path_instruct[increment] = 'e';
            increment++;
        }
        else if(bottom<=top  && bottom<=right && bottom<=left) //Going down
        {
            curr_pos[0] = x+1;
            curr_pos[1] = y;
            currentvalue = manhattan_grid[x+1][y];
            manhattan_grid[x+1][y] = 98;
            path_instruct[increment] = 's';
            increment++;
        }
        else if(left<=top && left<=right && left<=bottom) //Going left
        {
            curr_pos[0] = x;
            curr_pos[1] = y-1;
            currentvalue = manhattan_grid[x][y-1];
            manhattan_grid[x][y-1] = 98;
            path_instruct[increment] = 'w';
            increment++;
        }
    }
}

void godownpath()
{
    int i = 0;
    while(i < increment)
    {
        //printf("Instruction %d: \n", i+1);
        if((orientation == 'n') && (path_instruct[i] == 'n'))
        {
            moveforward();
            i++;
        }
        else if((orientation == 'n') && (path_instruct[i] == 'e'))
        {
            turnright();
            orientation = 'e';
            moveforward();
            i++;
        }
        else if((orientation == 'n') && (path_instruct[i] == 's'))
        {
            turnright();
            orientation = 'e';
            turnright();
            orientation = 's';
            moveforward();
            i++;
        }
        else if((orientation == 'n') && (path_instruct[i] == 'w'))
        {
            turnleft();
            orientation = 'w';
            moveforward();
            i++;
        }
        else if((orientation == 'e') && (path_instruct[i] == 'n'))
        {
            turnleft();
            orientation = 'n';
            moveforward();
            i++;
        }
        else if((orientation == 'e') && (path_instruct[i] == 'e'))
        {
            moveforward();
            i++;
        }
        else if((orientation == 'e') && (path_instruct[i] == 's'))
        {
            turnright();
            orientation = 's';
            moveforward();
            i++;
        }
        else if((orientation == 'e') && (path_instruct[i] == 'w'))
        {
            turnright();
            orientation = 's';
            turnright();
            orientation = 'w';
            moveforward();
            i++;
        }
        else if((orientation == 's') && (path_instruct[i] == 'n'))
        {
            turnright();
            orientation = 'w';
            turnright();
            orientation = 'n';
            moveforward();
            i++;
        }
        else if((orientation == 's') && (path_instruct[i] == 'e'))
        {
            turnleft();
            orientation = 'e';
            moveforward();
            i++;
        }
        else if((orientation == 's') && (path_instruct[i] == 's'))
        {
            moveforward();
            i++;
        }
        else if((orientation == 's') && (path_instruct[i] == 'w'))
        {
            turnright();
            orientation = 'w';
            moveforward();
            i++;
        }
        else if((orientation == 'w') && (path_instruct[i] == 'n'))
        {
            turnright();
            orientation = 'n';
            moveforward();
            i++;
        }
        else if((orientation == 'w') && (path_instruct[i] == 'e'))
        {
            turnright();
            orientation = 'n';
            turnright();
            orientation = 'e';
            moveforward();
            i++;
        }
        else if((orientation == 'w') && (path_instruct[i] == 's'))
        {
            turnleft();
            orientation = 's';
            moveforward();
            i++;
        }
        else if((orientation == 'w') && (path_instruct[i] == 'w'))
        {
            moveforward();
            i++;
        }
        //printf("\n");
        TermPrintln("MotorPower[A]: %d \t MotorPower[B]: %d\n", MotorPower(OUT_A), MotorPower(OUT_D));
        TermPrintln("RotationCount[A]: %d \t RotationCount[B]: %d\n", MotorRotationCount(OUT_A),MotorRotationCount(OUT_D));
    }
}



int main(void)
{

	// INFO This code template works only with recent versions of the API. If TermPrintln is not found,
	//      please update the API or use the "Hello World EV3 Project Old API" as template.


	//TODO Place here your variables


	//TODO Place here your code
	int InitEV3(void);
	ResetRotationCount(OUT_A);
	ResetRotationCount(OUT_D);
	TermPrintln("Final Demo\n");
	grid_setup();
	propagation();
	pathfinding();
	godownpath();


	/*
	ResetRotationCount(OUT_A);
	ResetRotationCount(OUT_D);
	turnright();
	TermPrintln("MotorPower[A]: %d \t MotorPower[B]: %d\n", MotorPower(OUT_A), MotorPower(OUT_D));
	TermPrintln("RotationCount[A]: %d \t RotationCount[B]: %d\n", MotorRotationCount(OUT_A),MotorRotationCount(OUT_D));
	if(170 != MotorRotationCount(OUT_A))
	{
		SetDirection(OUT_A, OUT_FWD);
		RotateMotor(OUT_A, 10, 170 - MotorRotationCount(OUT_A));
	}*/


	TermPrintf("Press ENTER to exit");
	ButtonWaitForPress(BUTTON_ID_ENTER);
	FreeEV3();
	return 0;
}
