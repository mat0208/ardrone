#include "video.h"
#include "blocksum.h"

#include <stdlib.h>

void video_blocksum(struct img_struct* img1, struct img_struct* img2, int* dx_out, int* dy_out) 
{
	int h=img1->h;
	int w=img1->w;
	unsigned char* buf1 = img1->buf;
	unsigned char* buf2 = img2->buf;
	
	int dmax = 5;
	int min_sum = 2000000000;
	int min_dx = -99;
	int min_dy = -99;
	/** @todo search first around center, search in a lower resolution image... */
	for(int dy=-dmax;dy<=dmax;dy++) {
		for(int dx=-dmax;dx<=dmax;dx++) {
			int sum=0;
			for(int y=dmax;y<h-dmax && sum < min_sum;y++) {
				int i1 = y*w + dmax;
				int i2 = (y+dy)*w + dmax+dx;
				
				
				for(int x=dmax;x<w-dmax;x++) {
					//printf("x=%d y=%d i1=%d i2=%d\n",x,y,i1,i2);
					sum += abs(buf1[i1*2+1] - buf2[i2*2+1]);
					i1++;
					i2++;
				}
			}
			if(min_sum>sum) {
				min_sum = sum;
				min_dx = dx;
				min_dy = dy;
			}
		}
	}
			
	*dx_out=min_dx;
	*dy_out=min_dy;
}
