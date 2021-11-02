#include <stdio.h>
#include <math.h>

#define MAX_OBSTACLES 25 /* maximum number of obstacles */
int num_obstacles = 13; /* number of obstacles */
double obstacle[MAX_OBSTACLES][2] = /* obstacle locations */
{{0.61, 2.743},{0.915, 2.743},{1.219, 2.743},{1.829, 1.219},
{1.829, 1.524},{ 1.829, 1.829}, {1.829, 2.134},{2.743, 0.305},
{2.743, 0.61},{2.743, 0.915},{2.743, 2.743},{3.048, 2.743},
{3.353, 2.743},
{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},
{-1,-1},{-1,-1},{-1,-1}};
double start[2] = {0.305, 1.219}; /* start location */
double goal[2] = {3.658, 1.829}; /* goal location */

double tile_length = 0.305;
int manhattan_grid[13][19];

int main() {

    int grid_width = sizeof(manhattan_grid)/sizeof(manhattan_grid[0]);
    int grid_length = sizeof(manhattan_grid[0])/sizeof(manhattan_grid[0][0]);

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
    printf("Grid set up to -1.\n");

    // Set up start and goal
    int x_start = (int) ceil(start[0]/tile_length);
    int y_start = (int) ceil(start[1]/tile_length);
    //manhattan_grid[y_start][x_start] = 98;
    manhattan_grid[grid_width-y_start][x_start] = 98;
   
    int x_goal = (int) ceil(goal[0]/tile_length);
    int y_goal = (int) ceil(goal[1]/tile_length);
    manhattan_grid[grid_width-y_goal][x_goal] = 0;
    printf("Start and goal set up.\n");

    // Set up obstacles
    int x_obs = 0;
    int y_obs = 0;
    int k, j;
    for (k = 0; k < num_obstacles; k++) {
        x_obs = (int) ceil(obstacle[k][0]/tile_length);
        y_obs = (int) ceil(obstacle[k][1]/tile_length);
        manhattan_grid[grid_width-y_obs][x_obs] = 99;
    }
    printf("Obstacles set up.\n");

    for (y = 0; y < grid_width; y++) {
        for (x = 0; x < grid_length; x++) {
            printf("\t%2d", manhattan_grid[y][x]);
        }
        printf("\n");
    }

    return 0;
}