/* 

Get real-time contact quality readings

  */

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "emokit/emokit.h"

int quit;
void cleanup(int i){
	fprintf(stdout,"Shutting down\n");
	quit=1; 
}

int main(int argc, char **argv)
{
	struct emokit_device* d;
	signal(SIGINT, cleanup); //trap cntrl c
	

	quit=0;

	d = emokit_create();
	int count=emokit_get_count(d, EMOKIT_VID, EMOKIT_PID);
	printf("Current epoc devices connected: %d\n", count );
	int r = emokit_open(d, EMOKIT_VID, EMOKIT_PID, 1);
	if(r != 0)
	{
		emokit_close(d);
		emokit_delete(d);
		d = emokit_create();
		r = emokit_open(d, EMOKIT_VID, EMOKIT_PID, 0);
		if (r!=0) {
			printf("CANNOT CONNECT: %d\n", r);
			return 1;
		}
	}
	printf("Connected to headset.\n");
	
	if (emokit_read_data(d)<=0) {
		fprintf(stdout,"Error reading from headset\n");
		fflush(stdout);
		emokit_close(d);
		emokit_delete(d);
		return 1;
	}

	struct emokit_frame c;
	while (!quit) {
		if(emokit_read_data(d) > 0) {						
			c = emokit_get_next_frame(d);			
			fprintf(stdout,"\033[H\033[2JPress CTRL+C to exit\n\nContact quality:\nAF3  %4d\tAF4  %4d\n F3  %4d\t F4  %4d\n F7  %4d\t F8  %4d\nFC5  %4d\tFC6  %4d\n T7  %4d\t T8  %4d\n P7  %4d\t P8  %4d\n O1  %4d\t O2  %4d\n",c.cq.AF3, c.cq.AF4, c.cq.F3, c.cq.F4, c.cq.F7, c.cq.F8, c.cq.FC5, c.cq.FC6, c.cq.T7, c.cq.T8, c.cq.P7, c.cq.P8, c.cq.O1, c.cq.O2, c.cq.FC5);
			fflush(stdout);
		} 	
	}
	emokit_close(d);
	emokit_delete(d);
	return 0;
}

