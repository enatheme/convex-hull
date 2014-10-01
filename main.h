#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif

#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#endif

#ifndef TIME
#define TIME
#include <time.h>
#endif

#ifndef UNISTD
#define UNISTD
#include <unistd.h>
#endif

//#define DEBUG

#ifndef SYSTIME
#define SYSTIME
#include <sys/time.h>
#endif

#ifndef MATH
#define MATH
#include <math.h>
#define M_PI 3.14159265358979323846 //constant M_PI dropped at the C99 normalization
#endif

#ifndef STRING
#define STRING
#include <string.h>
#endif

//definition of global constant
#define NB_POINTS 100 //max points to read
#define MAX_X 100 //the maximal cardinate x for a point, if the point is outside than the value, the program stop with an error message
#define MAX_Y 100 //the maximal cardinate y for a point, if the point is outside than the value, the program stop with an error message
#define MIN_X 0 //the minimal cardinate x for a point, ...
#define MIN_Y 0 //the minimal cardiante y for a point, ...
#define FILE_PATH "generate.out" //the path to the generate file of points
#define MAX_SIZE_CAR 4 //define the size maximal for a cardinate, for example, for MAX_X and MAX_Y 1000 the size is 4

//counter

typedef unsigned long long timestamp_t;

static timestamp_t
get_timestamp ()
{
	struct timeval now;
	gettimeofday (&now, NULL);
	return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

//structures
typedef struct Point Point;
struct Point
{
	unsigned int id; //we need an id
	unsigned int x; //the x cardinate of the point
	unsigned int y; //the y cardinate of the point
	float angle; //the angle calculate with the lowest point
	unsigned char is_ch; //a boolean for the displaying, is the point is a part of the convex hull ?
};

//the array of all points
typedef struct A_points A_points;
struct A_points
{
	Point points[NB_POINTS];
};

//pixel !
typedef struct Pixel Pixel;
struct Pixel
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

//image !
typedef struct Image Image;
struct Image
{
	Pixel pix[MAX_X][MAX_Y];
};


void graham_scan(A_points *all_points);
void export_img(A_points *all_points);
void quickSort(A_points *all_points, int l, int r);
int partition(A_points *all_points, int l, int r);
void read_file(A_points *all_points);
int read_src_file(FILE* src_file, A_points *all_points);
void display_error(int error_code);
