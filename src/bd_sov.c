#include"bd_sov.h"


const int speed_arr[] =
    {B230400, B115200, B38400, B19200, B9600, B4800, B2400, B1200, B300 };
const int name_arr[] = {230400, 115200, 38400, 19200, 9600, 4800, 2400, 1200, 300 };


int dev_open(char dev[])
{
    int fd, flag;
    

    speed_t baud_rate_i, baud_rate_o;

    fd = open(dev, O_RDWR | O_NONBLOCK );
    
    is_open(fd, dev); 
    
    /*
    struct termios term;
    flag = tcgetattr(fd, &term);
    baud_rate_i = cfgetispeed(&term);
    baud_rate_o = cfgetospeed(&term);
    printf("output baudrate before = %d，input baudrate = %d\n",baud_rate_i, baud_rate_o);
	*/
	
    set_speed(fd, 115200);
    
	/*
    flag = tcgetattr(fd, &term);
    baud_rate_i = cfgetispeed(&term);
    baud_rate_o = cfgetospeed(&term);
   	printf("baudrate now = %d，，input baudrate = %d\n",baud_rate_i, baud_rate_o);
   	*/

    if (set_parity(fd, 8, 1, 'N') == FALSE) 
	{
		printf("Set Parity Error\n");
		exit(1);
	
    }
    return fd;
}

void set_speed(int fd, int speed)
{
    unsigned int i;
    int status;
    struct termios Opt;
    tcgetattr(fd, &Opt);
    for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) 
	{
		if (speed == name_arr[i]) 
		{
	    	tcflush(fd, TCIOFLUSH);
	    	cfsetispeed(&Opt, speed_arr[i]);
	    	cfsetospeed(&Opt, speed_arr[i]);
	    	status = tcsetattr(fd, TCSANOW, &Opt);
	    	if (status != 0) 
			{
				perror("tcsetattr fd");
				return;
	    	}
	    	tcflush(fd, TCIOFLUSH);
		}
    }
}




int set_parity(int fd, int databits, int stopbits, int parity)
{
    struct termios options;
    if (tcgetattr(fd, &options) != 0) {
		perror("Setup Serial");
		return (FALSE);
    }
    options.c_cflag &= ~CSIZE;
    /* set databits */
    switch (databits) {		
    	case 7:
				options.c_cflag |= CS7;
				break;
    	case 8:
				options.c_cflag |= CS8;
				break;
    	default:
				fprintf(stderr, "Unsupported data size\n");
				return (FALSE);
    }
    
    switch (parity) {
    	case 'n':
    	case 'N':
				// options.c_cflag &= ~PARENB;   /* Clear parity enable */
				// options.c_iflag &= ~INPCK;     /* Enable parity checking */
				options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);	/*Input */
				options.c_oflag &= ~OPOST;	/*Output */
				break;
    	case 'o':
    	case 'O':
				options.c_cflag |= (PARODD | PARENB);	/* set Odd parity */
				options.c_iflag |= INPCK;	/* Disnable parity checking */
				break;
    	case 'e':
    	case 'E':
			options.c_cflag |= PARENB;	/* Enable parity */
			options.c_cflag &= ~PARODD;	/* set Even parity */
			options.c_iflag |= INPCK;	/* Disnable parity checking */
			break;
	    case 'S':
	    case 's':			/*as no parity */
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
	    default:
			fprintf(stderr, "Unsupported parity\n");
			return (FALSE);
    }
    
	/* set stopbits*/
    switch (stopbits) {
	    case 1:
			options.c_cflag &= ~CSTOPB;
			break;
	    case 2:
			options.c_cflag |= CSTOPB;
			break;
	    default:
			fprintf(stderr, "Unsupported stop bits\n");
			return (FALSE);
    }
	/* set input parity option */
    if ((parity != 'n') && (parity != 'N'))
		options.c_iflag |= INPCK;

    options.c_cc[VTIME] = 5;	// 0.5 seconds
    options.c_cc[VMIN] = 1;

    options.c_cflag &= ~HUPCL;
    options.c_iflag &= ~INPCK;
    options.c_iflag |= IGNBRK;
    options.c_iflag &= ~ICRNL;
    options.c_iflag &= ~IXON;
    options.c_lflag &= ~IEXTEN;
    options.c_lflag &= ~ECHOK;
    options.c_lflag &= ~ECHOCTL;
    options.c_lflag &= ~ECHOKE;
    options.c_oflag &= ~ONLCR;

    tcflush(fd, TCIFLUSH);	/* Update the options and do it NOW */
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
		perror("Setup Serial");
		return (FALSE);
    }

    return (TRUE);
}



// open file
void is_open(int fd, char *filename)
{
	if(fd == -1)
	{
		printf("Not open %s\n",filename);
		exit(1);
	}
	else
		printf("open %s sccussful!\n",filename);
}


//string is null?
int is_null(char *str)
{
	int len = strlen(str);
	if (len == 0)
		return 0;
	else
		return 1;
}


// return section in '$'
int GZV_sep(char *str_in, char *str_re,int pos)
{
	int i = pos, j = 1;
	str_re[0]= '$';
	int len = strlen(str_in);
	for(;str_in[i]!= '$' && i < len; ++i);
		++i;

	while(str_in[i]!= '$' && i<len)
	{
		str_re[j]=str_in[i];
		++i;
		++j;
	}
	str_re[j] = '\0';

	return i-1;
}

//return UTC time
int UTC_time(char *str, struct BD_UTC_TIME *ft)
{
	char hour[5], minute[5], second[10];
	int i,len;
	
	if(is_null(str))
	{
		len = strlen(str);
		for(i=0; i<len; ++i)
			if(i < 2)
				hour[i] = str[i];
			else if (i < 4)
				minute[i-2] = str[i];
			else 
				second[i-4] = str[i];
		
		hour[2] = '\0';
		minute[4] = '\0';
		second[i] = '\0';

		ft->hour 	=  atoi(hour);
		ft->minute 	=  atoi(minute);
		ft->second 	=  atoi(second);
		return 0;
	}
	else
	{
		printf("Time Error!\n");	
		return 1;
	}		
}

// return partion GPGGA
void GPGGA_sov(char *str, struct BD_GPGGA *fg)
{
	int i,len,dot_num=0;
	int num[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	len = strlen(str);

	for(i=0;i<len;i++)
	{
		if(str[i] == ',')
			dot_num++;

		switch(dot_num)
		{
			case 1:
				{
					if(str[i] != ',')
					{
						fg->bd_time[num[1]]=str[i];
						++num[1];
		
					}

					break;
				}
			case 2:
				{
					if(str[i] != ',')
					{
						fg->bd_latitude[num[2]]=str[i];
						++num[2];
						
					}

					break;
				}
			case 3:
				{
					if(str[i] != ',')
					{
						fg->N_or_S=str[i];
						
					}

					break;
				}
			case 4:
				{
					if(str[i] != ',')
					{
						fg->bd_longitude[num[4]]=str[i];
						++num[4];
					}

					break;
				}
			case 5:
				{
					if(str[i] != ',')
					{
						fg->E_or_W=str[i];
						
					}

					break;
				}
		}
		
	}
	fg->bd_time[num[1]]='\0';
	fg->bd_latitude[num[2]]='\0';
	fg->bd_longitude[num[4]]='\0';
	
}

//return partion GPZDA
void GPZDA_sov(char *str, struct BD_GPZDA *fz)
{
	int i,len,dot_num=0;
	int num[10]={0,0,0,0,0,0,0,0,0,0};
	len = strlen(str);

	for(i=0;i<len;i++)
	{
		if(str[i] == ',')
			dot_num++;

		switch(dot_num)
		{
			case 1:
				{
					if(str[i] != ',')
					{
						fz->bd_time[num[1]]=str[i];
						++num[1];
					}

					break;
				}
			case 2:
				{
					if(str[i] != ',')
					{
						fz->bd_day[num[2]]=str[i];
						++num[2];
					}

					break;
				}
			case 3:
				{
					if(str[i] != ',')
					{
						fz->bd_month[num[3]]=str[i];
						++num[3];
					}

					break;
				}
			case 4:
				{
					if(str[i] != ',')
					{
						fz->bd_year[num[4]]=str[i];
						++num[4];
					}

					break;
				}
		}
		
	}
	fz->bd_time[num[1]]='\0';
	fz->bd_day[num[2]]='\0';
	fz->bd_month[num[3]]='\0';
	fz->bd_year[num[4]]='\0';
	
}

//return partion GPVTG 
void GPVTG_sov(char *str, struct BD_GPVTG *fv)
{
	int i,len,dot_num=0;
	int num[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	len = strlen(str);

	for(i=0;i<len;i++)
	{
		if(str[i] == ',')
			dot_num++;
			
		switch(dot_num)
		{
			
			case 3:
				{
					if(str[i] != ',')
					{
						fv->degree_BaseMagnitude[num[3]]=str[i];
						++num[3];
					}

					break;
				}
			case 7:
				{
					if(str[i] != ',')
					{
						fv->velocity_kilomile[num[7]]=str[i];
						++num[7];
					}

					break;
				}
		}
		
	}
	fv->degree_BaseMagnitude[num[3]]='\0';
	fv->velocity_kilomile[num[7]]='\0';
	
}

