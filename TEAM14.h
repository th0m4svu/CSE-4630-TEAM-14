/*
 * TEAM14.h
 *
 *  Created on: Nov 4, 2021
 *      Author: Richard Tran
 */

#ifndef TEAM14_H_
#define TEAM14_H_

#define MAX_OBSTACLES 25 /* maximum number of obstacles */
int num_obstacles = 23; /* number of obstacles */
double obstacle[MAX_OBSTACLES][2] = /* obstacle locations */
{{.915,0},{0.915, 0.305}, {0.915, 0.61}, {0.915, 0.915}, {0.915, 1.22}, {0.915, 1.525}, {0.915, 1.83}, {1.83, .915},
{1.83, 1.22}, {1.83, 1.525}, { 1.83, 1.83}, {1.83, 2.135}, { 1.83, 2.44}, {1.83, 2.745}, {1.83,3.05},{2.135, 1.22}, {2.745, 1.525},
{2.745, 1.83}, {3.05, 1.22}, {3.355, 0.915}, {3.355, 1.22}, {3.66, 0.915}, {3.66, 1.22}, {-1,-1},{-1,-1}};
double start[2] = {0.305, 0.61}; /* start location */
double goal[2] = {3.66, 1.83}; /* goal location */

#endif /* TEAM14_H_ */
