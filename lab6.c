#include <stdio.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t ord;

typedef struct {
	int digit;
}place;


void *consume (void *mass)
{

	place *data = (place*) mass;
	sleep(1);
	while (1)
	{
		if((*data).digit < 2*LINES/3)
		(*data).digit++;
		
		usleep(10);
	}

}

void *producer (void *mass)
{
	place *fata = (place*) mass;
	sleep(1);	
	while (1)
	{	
		if((*fata).digit > LINES/3)
		(*fata).digit--;

		usleep(10);
	}

}


int main()
{
	initscr();
	keypad(stdscr,TRUE);
	curs_set(FALSE);

	for (int i = -5; i <= 5;i++)
	{
	mvaddch(LINES/3, COLS/2 + i,'-');
	mvaddch(2*LINES/3,COLS/2 + i,'-');
	}

	for (int i = LINES/3;i <= 2*LINES/3;i++)
	{
	mvaddch(i,COLS/2 - 5,'|');
	mvaddch(i,COLS/2 + 5,'|');
	}	

	place mesto;
	
	mesto.digit = 2*LINES/3;

	pthread_t first_th, second_th;
	pthread_mutex_init(&ord,NULL);
	
	pthread_create (&first_th,NULL,consume,(void*)&mesto);	

	pthread_create (&second_th,NULL,producer,(void*)&mesto);
	while(1)
	{
		pthread_mutex_lock(&ord);	

		if(mesto.digit != 2*LINES/3)
		for(int i = 1; 2*LINES/3 - i > mesto.digit;i++)
		mvaddstr(2*LINES/3 - i,COLS/2 - 4,"#########");//2*LINES/3 - mesto.digit - текущая заполненость 
			
		if(mesto.digit != LINES/3)	
		for(int i = 1; LINES/3 + i < mesto.digit;i++)	
		mvaddstr(LINES/3 + i,COLS/2 - 4,"         ");

		
		refresh();
		usleep(10);
		pthread_mutex_unlock(&ord);
	}


	void *retral;
	pthread_mutex_destroy(&ord);
	pthread_join(first_th,&retral);

	endwin();
	return 0;
}
