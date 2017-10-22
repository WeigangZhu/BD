#ifndef BD_SOV_H_
#define BD_SOV_H_

#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>



#define READ_NUM 256
#define ARR_MAX 20

#define FALSE -1
#define TRUE   0
#define MAX_TRY 100



void set_speed(int, int);
int set_Parity(int, int, int, int);

struct BD_GPZDA
{
	char bd_start[ARR_MAX];
	char bd_time[ARR_MAX];
	char bd_day[ARR_MAX];
	char bd_month[ARR_MAX];
	char bd_year[ARR_MAX];
	char bd_time_zone[ARR_MAX];
};

struct BD_GPGGA
{
	char bd_start[ARR_MAX];
	char bd_time[ARR_MAX];
	char bd_latitude[ARR_MAX];
	char N_or_S;
	char bd_longitude[ARR_MAX];
	char E_or_W;
	char bd_state;
	/* ... */
};

struct BD_GPVTG
{
	char degree_BaseRealNorth[ARR_MAX];
	char degree_BaseMagnitude[ARR_MAX];
	char velocity_knot[ARR_MAX];
	char velocity_kilomile[ARR_MAX];
	char bd_mode[ARR_MAX];
};

struct BD_UTC_TIME
{
	int hour;
	int minute;
	float second;
};

void test_re(int para);
void is_open(int fd, char *filename);  // open file
int is_null(char *str);  //string is null?
int flag_mode(char ); // return which data section, GPGGA,GPZDA or GPVTG
int dev_open(char dev[]);
double Trans(int ); // return 10^i
double Trans_verse(int );//return 10^(-i)

float char2num(char *); // char to float
int char2str(char *str_in,char *str_re,int pos) ; // char to string 
int GZV_sep(char *str, char *str_re,int pos);

void GPGGA_sov(char *str, struct BD_GPGGA *fg);
#endif
