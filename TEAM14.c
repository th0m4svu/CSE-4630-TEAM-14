#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <ev3.h>
#include "TEAM14.h"


//Our defined variables to avoid having to go through every function and changing the values
#define TURNSPEED  5
#define TURNANGLE 166
#define STRAIGHTSPEED 10
#define STRAIGHTANGLE 512
#define ERRORMAGNITUDE 1


//Our variables used to convert from mm to an integer for our array
double tile_length = 0.305; /* length of tile */
int manhattan_grid[13][19]; /* dimensions for grid */
int grid_width = sizeof(manhattan_grid)/sizeof(manhattan_grid[0]); //10
int grid_length = sizeof(manhattan_grid[0])/sizeof(manhattan_grid[0][0]); //16
char path_instruct[160];

//This is used to keep track of the orientation for our robot, later used 
//in our executing path function
char orientation = 'n';
int increment = 0;

//Tones for our robot to play to let audience know orientation
Tone a[2] = {
		{TONE_C4, NOTE_HALF},
		{TONE_F6, NOTE_EIGHT}
};


//This is our function to move forward 1 full tile
void moveforward()
{
    /*
    We switched over from RotateMotor to OnFwdSync because the motors were starting seperately.
    For RotateMotor we had to calculate the diameter of the wheel, and then find the circumference. Then we would 
    divide the length of a tile by the circumference to find the proportions of the wheel to the tile. We would then
    multiply this value by 360 to find the angle of how much we need to turn the wheel by to achieve a full tile.

    For OnFwdSync we only had to time the functino and find the exact time it would take to move a full tile
    (This method was much for quicker and also resulted in eleiminating the problem of motors starting at different times)
    */
	OnFwdSync(OUT_AD, STRAIGHTSPEED);
	Wait(5000);

    //This function is here to ensure that the motors are at a complete stop before moving on to the next function
	Off(OUT_AD);
	Wait(500);

    //When using Rotate motor we made sure to set direction before hand to make sure tires were rotating the correct direction
	//SetDirection(OUT_AD, OUT_FWD);
	//RotateMotor(OUT_AD,STRAIGHTSPEED,STRAIGHTANGLE);
	//RotateMotor(OUT_AD,STRAIGHTSPEED,STRAIGHTANGLE/2);
	//RotateMotor(OUT_AD,STRAIGHTSPEED,STRAIGHTANGLE/2);


	/*
    We tried eliminating the offset of moving off the grid by keeping track of the motor counts and compensating for it. However, even if a motor
    was lagging behind the robot didn't process this and the counts showed that it was perfect when in reality it was not. We ended up dropping
    this method of error fixing because it wasn't possible given the robot wasn't processing any errors.

    int i=0;
	for(i=0;i<5;i++)
	{
		ResetRotationCount(OUT_A);
		ResetRotationCount(OUT_D);
		RotateMotor(OUT_AD,STRAIGHTSPEED,STRAIGHTANGLE/5);
		if(MotorRotationCount(OUT_A)<MotorRotationCount(OUT_D))
			RotateMotor(OUT_A, STRAIGHTSPEED, (MotorRotationCount(OUT_D)-MotorRotationCount(OUT_A))*ERRORMAGNITUDE);
		else if(MotorRotationCount(OUT_D)<MotorRotationCount(OUT_A))
			RotateMotor(OUT_D, STRAIGHTSPEED, (MotorRotationCount(OUT_A)-MotorRotationCount(OUT_D))*ERRORMAGNITUDE);
	}
    */
	return;
}


/*
Clockwise turn function (This will be the same concept as the Counterclockwise turn function, except orientation will be incrementing backwards
instead)
*/
void turnright()
{
    /*
    This is the core of our turn function that does the actual movement. Before anything we had to set the direction of both motors
    (one going forward and the other going in reverse based on if it's turning right or turning left). Here we had to calculate the distance
    between both tires to calculate the turnangle required. Calculation from here on was similar to the calculation done for moveforward();.
    We had to undershoot the angle here a bit after testing because we realized when we turned right, the robot would jerk a bit towards the right.
    (This was the same for turning left, it would jerk left after). So to compensate we reduced the turnangle a bit.
    */
	SetDirection(OUT_A, OUT_FWD);
	SetDirection(OUT_D, OUT_REV);

    //We used the NoWait function because we needed both motors to rotate at the same time. Using RotateMotor(OUT_AD...) didn't work here for some reason
    //and both wheels would move forward instead, so we had to do it seperately.
	RotateMotorNoWait(OUT_A, TURNSPEED, TURNANGLE);
	RotateMotorNoWait(OUT_D, TURNSPEED, TURNANGLE);

    //Again this wait function is used to ensure that the turn is done and the motors are off before any other task is executed next
	Wait(3000);

    //After turning the orientation of the robot changed, so we have to keep track of it. Turning right results in the 
    //direction changing in the clockwise direction
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


//The concept here will be the same as the turnright(); function, please refer there for any information
void turnleft()
{
	SetDirection(OUT_A, OUT_REV);
	SetDirection(OUT_D, OUT_FWD);
	RotateMotorNoWait(OUT_A, TURNSPEED, TURNANGLE);
	RotateMotorNoWait(OUT_D, TURNSPEED, TURNANGLE);
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


//Used to print out our virtual map (Alongside the changes we made for obstacles and other locations)
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


//This function is used to create our manhattan grid.
void propagation()
{
    //This is the current value starting from the goal value which is 0
    int current = 0;
    int y, x;

    //This loop is the amount of times it will iterate through the map. This is more of a brute force method and isn't very efficient.
    //But it ensures that no matter what type of setup we have, the propagation will execute successfully
    while(current<(grid_length*grid_width))
    {
        //These for loops are used to iterate through the entire map
        for (y = 0; y < grid_width; y++)
        {
            for (x = 0; x < grid_length; x++)
            {
                //This if statement will check if the current array has the value of "current", if it does then it will
                //Change the value of every single value next to it if it has the value of -1 to (current+1). We will continue
                //to look through the rest of the array for any other values that = current.
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
        }

        //Current will increment and we will look for the new value instead.
        current++;
    }
}

//This is our function used to find the path for our robot to take. This is global because we find the full path before the robot moves.
void pathfinding()
{

    //Converting measurements into integers for our array
    int x_start = (int) ceil(start[0]/tile_length);
    int y_start = (int) ceil(start[1]/tile_length);

    int x = (grid_width-y_start)-2;
    int y = x_start+1;

    int top, right, bottom, left;
    int curr_pos[2] = {x,y};
    int currentvalue = 98;

    //This while loop will continue until we successfully find a block with a value of 0 (meaning we found the goal)
    while(currentvalue!=0)
    {
        //We keep track of every block next to us to know which block we need to switch to 
        x = curr_pos[0];
        y = curr_pos[1];
        top = manhattan_grid[x-1][y];
        right = manhattan_grid[x][y+1];
        bottom = manhattan_grid[x+1][y];
        left = manhattan_grid[x][y-1];

        //--- The way our robot prioritizes which direction to go in terms of equal cases is in the following order: top, right, bottom, left.
        //At the end of each if statement we also add the direction of what each decision will put us in and add it to an array of instructions.
        //This will allow us to keep track of our instructions.

        //If the top block has the smallest value this is the direction we will take, swithcing the value of current value, and current position
        //to the new block.
        if(top<=bottom && top<=right &&  top<=left) //Going up
        {
            curr_pos[0] = x-1;
            curr_pos[1] = y;
            currentvalue = manhattan_grid[x-1][y];
            manhattan_grid[x-1][y] = 98;
            path_instruct[increment] = 'n';
            increment++;

        }
        //If the right block has the smallest value this is the direction we will take, swithcing the value of current value, and current position
        //to the new block.
        else if(right<=top  && right<=left && right<=bottom) //Going right
        {
            curr_pos[0] = x;
            curr_pos[1] = y+1;
            currentvalue = manhattan_grid[x][y+1];
            manhattan_grid[x][y+1] = 98;
            path_instruct[increment] = 'e';
            increment++;
        }
        //If the bottom block has the smallest value this is the direction we will take, swithcing the value of current value, and current position
        //to the new block.
        else if(bottom<=top  && bottom<=right && bottom<=left) //Going down
        {
            curr_pos[0] = x+1;
            curr_pos[1] = y;
            currentvalue = manhattan_grid[x+1][y];
            manhattan_grid[x+1][y] = 98;
            path_instruct[increment] = 's';
            increment++;
        }
        //If the left block has the smallest value this is the direction we will take, swithcing the value of current value, and current position
        //to the new block.
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


//This is our function for our robot to move down the path
void godownpath()
{
    //Using the array of instructions we created from the pathfinding, we will be able to perform movements based on our current orientation, and the next.
    int i = 0;
    //increment is the global variable we created for the amount of instructions inside our array, so we will only iterate until this value.
    while(i < increment)
    {
        //These if instructions will perform specific movements based on the current orientation, and the current instruction in the array.
        //It will perform the function, then increment the value, orientation will also be changed if the movement requires a turn.
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
        //This is just used to keep track of the power and rotation count of each motor.
        TermPrintln("MotorPower[A]: %d \t MotorPower[B]: %d\n", MotorPower(OUT_A), MotorPower(OUT_D));
        TermPrintln("RotationCount[A]: %d \t RotationCount[B]: %d\n", MotorRotationCount(OUT_A),MotorRotationCount(OUT_D));
    }
}



int main(void)
{
    //This function is used to initialize all EV3 functions.
	int InitEV3(void);
    //At the beginning of every program we will reset the counts so they are correct and don't continue on from last run.
	ResetRotationCount(OUT_A);
	ResetRotationCount(OUT_D);
	TermPrintln("Final Demo\n");

    //These functions in the following order will: Setup our virtual map, propogate to create our manhattan grid, find a path, and go down the path
	grid_setup();
	propagation();
	pathfinding();
	godownpath();


	/*Basic testing for error fixing of a turn to make it perfect. Again the robot wasn't processing any errors so we figured out that utilizing error fixing
    and trying to compensate wasn't possible
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

    //When the robot is done, we can press exit to move on to the closing of all EV3 functions as well as returning to exit
	TermPrintf("Press ENTER to exit");
	ButtonWaitForPress(BUTTON_ID_ENTER);
	FreeEV3();
	return 0;

}
