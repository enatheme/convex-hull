#ifndef MAIN
#define MAIN
#include "main.h"
#endif

/*program to blablabla*/

void display_error(int error_code)
{
	switch(error_code)
	{
		case 3:
			printf("too large number\n");
		break;
		case 4:
			printf("not a number\n");
		break;
		case 5:
			printf("just one cardinate\n");
		break;
		case 6:
			printf("too much cardinates\n");
		break;
		default:
			printf("ERROR\n");
		break;
	}
	printf("\nERROR %ld\n", error_code);
	exit(1);
}

//reading of the numbers file
//send 1 if something is wrong (not all numbers, too much numbers, too large or negative)
int read_src_file(FILE* src_file, A_points *all_points)
{
	char str_temp[MAX_SIZE_CAR] = {}; //we need a temporal array of char
	char temp_char = 0, x_allo = 0; //x_allo give us a boolean to check if we already read x
	unsigned int i = 0, j = 0, temp_x = 0; //j counter of numbers
	
	
	//we loop until the end of the file
	
	while(temp_char != EOF)
	{
		//ok here we should be able to recognize each numbers
		do
		{
			temp_char = getc(src_file);	
			if(temp_char != EOF)
			{
				//we check if the char read is a number or a space or a endofline, if not we stop the program
				if (!(((temp_char >= '0') && (temp_char <= '9')) || (temp_char == ' ') || (temp_char == '\n')) || (temp_char == EOF) || (temp_char == '\0'))
				{
					display_error(4);
				}
	
	
				//end of x cardinate
				if (temp_char == ' ')
				{
					//if we already alocated x and we have another space => failure
					if(x_allo == 1)
					{
						display_error(6);
					}

					char *endptr; //var needed by strtol
					temp_x = strtol(str_temp, &endptr, 10); //we convert the string into an integer
					x_allo = 1; //we don't forget that we allocate x
					str_temp[0] = '\0';
					str_temp[1] = '\0';
					str_temp[2] = '\0';
					str_temp[3] = '\0';
					i = 0;
				}
	
	
				//end of the line detected, we try to affect the entry
				else if (temp_char == '\n') 
				{
					//if x is not allocated, that's mean we have just one cardiante => failure
					if (x_allo == 0)
					{
						display_error(5);
					}
					char *endptr; //var needed by strtol
					all_points->points[j].id = j;
					all_points->points[j].x = temp_x;
					all_points->points[j].y = strtol(str_temp, &endptr, 10);
					all_points->points[j].is_ch = 0;
	
					j++;
					i = 0;
					str_temp[0] = '\0';
					str_temp[1] = '\0';
					str_temp[2] = '\0';
					str_temp[3] = '\0';
					x_allo = 0;
				}
				else
				{
					//if the cardinate is larger than 999 we stop the program
					if(i > MAX_SIZE_CAR - 1)
					{
						printf("here %d, %d\n", i, j);
						display_error(3);
					}
	
					str_temp[i] = temp_char;
					i++;
				}
			}
	
		} while((temp_char != '\n') && (temp_char != EOF));
	}

	//if j is less than the number of command we sendback 0, else we send 1
	if (j == NB_POINTS)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

//function to read the command file and initialise it
void read_file(A_points *all_points)
{
	FILE* file_src = fopen(FILE_PATH, "r");
	//if we cannot open the configuration file... what we can do ? no srsly, we are screwed, or something like that !
	if(file_src == NULL)
	{
		fprintf(stdout, "\nFile reading failed, you can :\nThe program will quit.\n");
		//stop the program
		//on est pas des gorets, on ferme les fichiers !
		if(file_src != NULL)
		{
			fclose(file_src);
		}
		exit(0);
	}
	
	
	//if we can open the source file... we read it !
	else
	{
		if (read_src_file(file_src, all_points))
		{
			#ifndef DEBUG
			fprintf(stdout, "\nNumbers loaded.");
			#endif
		}
		else
		{
			file_src = NULL;
			fprintf(stdout, "\nError at the reading of numbers, too much, or too large, or negative.");
		}
	}
	
	//on est pas des gorets, on ferme les fichiers !
	if(file_src != NULL)
	{
		fclose(file_src);
	}
}


//function to the quicksort
int partition(A_points *all_points, int l, int r) 
{
	float pivot;
	int i, j;
	Point t;
	pivot = all_points->points[l].angle;
	i = l; j = r+1;
		
	while( 1)
	{
		do 
		{
			++i; 
		}
		while( all_points->points[i].angle <= pivot && i <= r );
		
		do 
		{
			--j; 
		}
		while( all_points->points[j].angle > pivot );
		
		if( i >= j ) 
		{
			break;
		}
		t = all_points->points[i]; 
		all_points->points[i] = all_points->points[j]; 
		all_points->points[j] = t;
	}
	t = all_points->points[l]; 
	all_points->points[l] = all_points->points[j]; 
	all_points->points[j] = t;
	return j;
}

//quicksort algorithm 
void quickSort(A_points *all_points, int l, int r)
{
   int j;

   if( l < r ) 
   {
   	// divide and conquer
        j = partition(all_points, l, r);
       quickSort(all_points, l, j-1);
       quickSort(all_points, j+1, r);
   }
	
}





void graham_scan(A_points *all_points)
{
	//select rightmost lowest point p0
	unsigned int lowest_y = MAX_Y, p0 = 0;
	unsigned int point_sorted[NB_POINTS];
	unsigned int temp_stack[NB_POINTS] = {};
	unsigned int index_stack = 0;
	for (int i = 0 ; i < NB_POINTS ; i++)
	{
		if (all_points->points[i].y < lowest_y)
		{
			lowest_y = all_points->points[i].y;
			p0 = all_points->points[i].id;
		}
		if (all_points->points[i].y == lowest_y && all_points->points[i].x > all_points->points[p0].x)
		{
			lowest_y = all_points->points[i].y;
			p0 = all_points->points[i].id;
		}
	}
	
	//sort S angularly around p0
	/*to calculate the angle we take the trigo : 
	 *          x C
	 * 	       /|
	 * 	      / |
	 * 	     /  |
	 *      /   |
	 *     /    |
	 *    /     |
	 *   /      |
	 * Bx-------x A
	 * 
	 * B is the p0
	 * 
	 * CA/BA => p1.y - p0.y / p1.x - p0.x
	 * 
	 * if the result is positiv, the smallest value is the first
	 * if the result is negativ, the smallest value is the latest
	 * 
	 */
	
	for(int i = 0 ; i < NB_POINTS ; i++)
	{
		if(i == p0)
		{
			all_points->points[i].angle = 0;
		}
		else
		{
			all_points->points[i].angle = 180 * atanf(((all_points->points[i].y - all_points->points[p0].y) * 1.0 / (all_points->points[i].x - all_points->points[p0].x * 1.0))) / M_PI;
			//if the angle is negative, we should add 180 degres
			if (all_points->points[i].angle < 0)
			{
				all_points->points[i].angle+= 180;
			}
		}
	}
	
	/*
	 * we sort points
	 */


 


	//buble sort
	/*for (int i = 0 ; i < NB_POINTS - 1 ; i++)
	{
		if(all_points->points[i].angle > all_points->points[i + 1].angle)
		{
			Point temp = all_points->points[i];
			all_points->points[i] = all_points->points[i + 1];
			all_points->points[i + 1] = temp;
			for (int j = i ; j != 0 ; j--)
			{
				if(all_points->points[j].angle < all_points->points[j - 1].angle)
				{
					Point temp2 = all_points->points[j];
					all_points->points[j] = all_points->points[j - 1];
					all_points->points[j - 1] = temp2;
				}
					
			}
		}
	}*/

	//sorting !
	quickSort(all_points, 0, NB_POINTS - 1);
	#ifndef DEBUG
	for (int i = 0 ; i < NB_POINTS ; i++)
	{
		printf("%d - %d\n", all_points->points[i].x, all_points->points[i].y);
	}
	#endif

	//push p0 and p1
	temp_stack[index_stack] = 0;
	index_stack++;
	temp_stack[index_stack] = 1;
	
	//while i < n
	int w = 2;
	while(w < NB_POINTS)
	{
		//(Bx-Ax)*(Cy-Ay) - (By-Ay)*(Cx-Ax)
		int ax, ay, bx, by, cx, cy;
		ax = all_points->points[temp_stack[index_stack - 1]].x;
		ay = all_points->points[temp_stack[index_stack - 1]].y;
		bx = all_points->points[temp_stack[index_stack]].x;
		by = all_points->points[temp_stack[index_stack]].y;
		cx = all_points->points[w].x;
		cy = all_points->points[w].y;
		/*
		* we calculate the determinant
		* if deter > 0 ccw
		* deter = 0 on the line
		* deter < 0 cw
		*/
		int deter = ((bx - ax) * (cy - ay) - (by - ay) * (cx - ax)); 
		#ifndef DEBUG
		printf("\n==>deter = %d || 1x = %d 1y = %d _ 2x = %d 2y = %d _ 3x = %d 3y = %d\n", deter, ax, ay, bx, by, cx, cy);
		printf("A = %d B = %d C = %d", temp_stack[index_stack - 1], temp_stack[index_stack], w);
		#endif
		if (deter > 0)
		{
			#ifndef DEBUG
			printf("\nADD : %d | ", w);
			#endif
			//push(i)
			index_stack = index_stack + 1;
			temp_stack[index_stack] = w;
			
			//i++
			w++;
			
		
		}
		//else
		else
		{
			#ifndef DEBUG
			printf("\nDROP : %d | ", temp_stack[index_stack]);
			#endif
			//pop pt1 from the stack
			index_stack = index_stack - 1;
		}
		#ifndef DEBUG
		printf("STACK ");
		for (int j = 0 ; j < index_stack + 1; j++)
		{
			printf("|%d| ", temp_stack[j]);
		}
		#endif
	}
	#ifndef DEBUG
	printf("\n\nFinal : \n");
	for (int j = 0 ; j < index_stack + 1; j++)
	{
		printf("%d x = %d y = %d\n", temp_stack[j], all_points->points[temp_stack[j]].x, all_points->points[temp_stack[j]].y);
	}
	printf("Number of elements in the CH : %d\n", index_stack + 1);
	#endif

	//we assign 1 to the ch points
	for (int j = 0 ; j < index_stack + 1; j++)
	{
		all_points->points[temp_stack[j]].is_ch = 1;
	}

		
}

void export_img(A_points *all_points)
{
	
	//we will create an array of pixel of size MAX_X by MAX_Y
	Image img;
	//we initialize all pixel white (0 0 0)
	for (int i = 0 ; i < MAX_X ; i++)
	{
		for (int j = 0 ; j < MAX_Y ; j++)
		{
			img.pix[i][j].red = 255;
			img.pix[i][j].green = 255;
			img.pix[i][j].blue = 255;
		}
	}

	//loop to draw pixel
	for (int i = 0 ; i < NB_POINTS ; i++)
	{
		if(all_points->points[i].is_ch)
		{
			img.pix[all_points->points[i].x][MAX_Y - all_points->points[i].y].green = 255;
			img.pix[all_points->points[i].x][MAX_Y - all_points->points[i].y].blue = 255;
		}
		else
		{
			img.pix[all_points->points[i].x][MAX_Y - all_points->points[i].y].green = 0;
			img.pix[all_points->points[i].x][MAX_Y - all_points->points[i].y].blue = 0;
		}
		img.pix[all_points->points[i].x][MAX_Y - all_points->points[i].y].red = 0;
	}

	//export of the image
	FILE *export_img = fopen("export.ppm", "w");
	fputs("P3\n", export_img);
	//fputs((string)MAX_X, export_img);
	fprintf(export_img, "%d", MAX_X);
	fputs(" ", export_img);
	//fputs(MAX_Y, export_img);
	fprintf(export_img, "%d", MAX_Y);
	fputs("\n255\n", export_img);
	
	//loop to write the convex hull into the image
	for (int i = 0 ; i < MAX_X ; i++)
	{
		for (int j = 0 ; j < MAX_Y ; j++)
		{
			fprintf(export_img, "%d %d %d ", img.pix[j][i].red, img.pix[j][i].green, img.pix[j][i].blue);
		}
		fputs("\n", export_img);
	}

	fclose(export_img);
}

int main()
{
	#ifndef DEBUG
	printf("\n");
	#endif
	A_points all_points;
	A_points *ptr_all_points = & all_points;
	read_file(ptr_all_points);
	timestamp_t t0 = get_timestamp();
	graham_scan(ptr_all_points);
	timestamp_t t1 = get_timestamp();
	export_img(&all_points);
	#ifndef DEBUG
	printf("\n");
	#endif
	float time = (t1 - t0) / 1000000.0L;
	printf("TIME : %f secs\n", time);
}
