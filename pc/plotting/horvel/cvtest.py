from cv2 import cv
import numpy as N
import time

def loadImg(num):
    return cv.LoadImage("logs/img_{:05}.pgm".format(num))

def loadImgM(num):
    return cv.LoadImageM("logs/img_{:05}.pgm".format(num),cv.CV_LOAD_IMAGE_GRAYSCALE)

           

def blocksum(mat1,mat2,doMap=False):
   
    dmax = 15
    min_sum = 2000000000
    min_dx = -99
    min_dy = -99

    if doMap:
        output=N.zeros( [dmax*2+1,dmax*2+1] )

    i1=N.asarray(mat1).astype(int)
    i2=N.asarray(mat2).astype(int)
          

    
    h=mat1.height
    w=mat1.width
    xmargin=max(w/4,dmax)
    ymargin=max(h/4,dmax)

    d1=i1[ymargin   :h-ymargin]
    c1=d1[:,xmargin   :w-xmargin ]

    for dy in range(-dmax,dmax+1):
        rows2=i2[ymargin+dy:h-ymargin+dy]
        for dx in range(-dmax,dmax+1):

            c2=rows2[:,xmargin+dx:w-xmargin+dx]

                    
            diff=abs(c1-c2)
            imgSum=diff.sum()

        
      #      print dy,dx,' ',imgSum
            if doMap:
                output[dy+dmax][dx+dmax]=imgSum

            if imgSum < min_sum: 
                min_sum = imgSum
                min_dx = dx
                min_dy = dy


    if doMap:
        output=output-output.min()
        output=output/output.max()
        output = 1-output
    else:
        output=None
    return (min_dy,min_dx,min_sum, output)


    


i1=loadImg(164)
i2=loadImg(165)
start=time.clock()
(y,x,minsum,field)=blocksum(loadImgM(164),loadImgM(165))
print (time.clock()-start)*1000
print y,x
#fieldWindow=cv.NamedWindow('field',0)
#'cv.ShowImage('field',cv.fromarray(field))
cv.WaitKey()
