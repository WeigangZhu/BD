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



int dev_open(char dev[]);
int set_parity(int fd, int databits, int stopbits, int parity);
void set_speed(int fd, int speed);
void is_open(int fd, char *filename);  // open file
int is_null(char *str);  //string is null?


int GZV_sep(char *str, char *str_re, int pos);
int UTC_time(char *str, struct BD_UTC_TIME *ft);
void GPZDA_sov(char *str, struct BD_GPZDA *fz);
void GPVTG_sov(char *str, struct BD_GPVTG *fv);
void GPGGA_sov(char *str, struct BD_GPGGA *fg);
#endif
