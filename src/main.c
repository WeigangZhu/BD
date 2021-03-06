#include"bd_sov.h"

int main(int argc, char **argv)
{
	int fd;
	fd = dev_open("/dev/ttyUSB0");

	char str[READ_NUM], str_GZV[100];

	int len, i;
	double degree, velocity, longitude, latitude;
	
	struct BD_GPGGA GPGGA;
	struct BD_GPZDA GPZDA;
	struct BD_GPVTG GPVTG;
	struct BD_UTC_TIME UTC_TIME;

	while ((len = read(fd, str, READ_NUM))) 
	{
		usleep(200000);
		
		for (i = 0; i < len; ++i) 
		{
			i = GZV_sep(str,str_GZV,i);
			
			if(str_GZV[3] == 'G')
			{	
				GPGGA_sov(str_GZV,&GPGGA);
				
				if(!UTC_time(GPGGA.bd_time, &UTC_TIME))
					printf("%.2d:%.2d:%.2f\n",UTC_TIME.hour,UTC_TIME.minute,UTC_TIME.second);	
				
				latitude  = atof(GPGGA.bd_latitude);
				longitude = atof(GPGGA.bd_longitude);
				printf("GPGGA.bd_latitude  = %.10lf %c\n",latitude/100,GPGGA.N_or_S);
				printf("GPGGA.bd_longitude = %.10lf %c\n",longitude/100,GPGGA.E_or_W);
			}

			if(str_GZV[3]  == 'Z')
			{	
				GPZDA_sov(str_GZV,&GPZDA);
				
				// printf("GPZDA.bd_time = %s\n",GPZDA.bd_time);
				if(!UTC_time(GPZDA.bd_time, &UTC_TIME))
				{
					
					printf("UTC_TIME.hour = %.2d\n",UTC_TIME.hour);
					printf("UTC_TIME.minute = %.2d\n",UTC_TIME.minute);
					printf("UTC_TIME.second = %f\n",UTC_TIME.second);	
				}
					
				printf("GPZDA.bd_day = %s\n",GPZDA.bd_day);
				printf("GPZDA.bd_month = %s\n",GPZDA.bd_month);
				printf("GPZDA.bd_year = %s\n\n",GPZDA.bd_year);
					
					
			}

			if(str_GZV[3] == 'V')
			{	
				GPVTG_sov(str_GZV,&GPVTG);	
				
				degree  = atof(GPVTG.degree_BaseMagnitude);
				velocity = atof(GPVTG.velocity_kilomile);
				printf("GPGGA.degree_BaseMagnitude  = %.10lf degree\n", (degree/10));
				printf("GPGGA.velocity_kilomile = %.10lf km/h\n\n\n", velocity);
			}

		}
	}

	close(fd);
	exit(0);

}



