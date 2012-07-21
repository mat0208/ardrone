#include "../video/video.h"
#include "../video/blocksum.h"
#include "../util/util.h"
#include "horizontal_velocities.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct vid_struct vid;

struct img_struct* img_old;
struct img_struct* img_new;

/** these are protected by mutex */
float x, y;
unsigned long seqNum;

/** width of a pixel in meters at 1 m height */
#define CAM_HEIGHT_SCALE_FACTOR 0.01

pthread_t horizontal_velocities_thread;
pthread_mutex_t velocity_access_mutex=PTHREAD_MUTEX_INITIALIZER;


void *horizontal_velocities_thread_main(void *data)
{

    video_GrabImage(&vid, img_old);
    for (;;) {
                float start=util_timestamp();
		video_GrabImage(&vid, img_new);

                int dxi;
                int dyi;
		video_blocksum(img_old, img_new, &dxi, &dyi);
		
		if(dxi!=0 || dyi!=0) {
			//swap buffers
			struct img_struct* tmp = img_new;
			img_new=img_old;
			img_old=tmp;
		}
		
                pthread_mutex_lock(&velocity_access_mutex);     
		x+=dxi;
		y+=dyi;
		seqNum++;
		
                pthread_mutex_unlock(&velocity_access_mutex);     
                float endTime=util_timestamp();
                printf("Loop took %3.1f ms\n", (endTime-start)*1000 );
    }

    video_Close(&vid);
	
    return 0;
}


int horizontal_velocities_init(struct horizontal_velocities_struct *hv)
{
	vid.device = (char*)"/dev/video2";
	
	printf("Device is %s\n",vid.device);
	vid.w=320;
	vid.h=240;
	vid.n_buffers = 1;
	if(video_Init(&vid) !=0) exit(-1);

	img_old = video_CreateImage(&vid);
	img_new = video_CreateImage(&vid);
	
	x=0;
	y=0;
	seqNum=0;
	
	hv->xv=0;
	hv->yv=0;
	hv->dt=0;
	hv->prevX=0;
	hv->prevY=0;
	hv->prevTime=util_timestamp();

        int rc = pthread_create(&horizontal_velocities_thread, NULL, horizontal_velocities_thread_main, NULL);
        if(rc) {
                printf("ctl_Init: Return code from pthread_create(hor_vel_thread) is %d\n", rc);
                return 202;
        }
        return 0;
}


void horizontal_velocities_getSample(struct horizontal_velocities_struct *hv, struct att_struct *att)
{
     pthread_mutex_lock(&velocity_access_mutex);
     
     double dy=y-hv->prevY;
     double dx=x-hv->prevX;
     double time=util_timestamp();
     double dt=time-hv->prevTime;
     
     /**     @todo kalman or luenberger for filtering */  
     if(dt>0) {
        hv->xv=dx/dt;
        hv->yv=dy/dt;
        hv->dt=dt;
        hv->prevX=x;
        hv->prevY=y;
        hv->prevTime=time;
     }
     hv->seqNum=seqNum;
     pthread_mutex_unlock(&velocity_access_mutex);     
}  


void horizontal_velocities_print(struct horizontal_velocities_struct *hv)
{
	printf("seq=%ld   xv=%5.1f,yv=%5.1f, dt=%4.1f\n"
	        ,hv->seqNum
		,hv->xv, hv->yv
		,hv->dt*1000
	);
}


