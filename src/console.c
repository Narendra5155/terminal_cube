#include<windows.h>
#include"console.h"
#include<stdio.h>
#include<math.h>
#include<wchar.h>
#include<stdlib.h>

// pre calculate the sines and cosines

void CalclulateAngles(TrigRaito *pre){
        pre->sa=sin(A);
        pre->sb=sin(B);
        pre->sc=sin(C);
        pre->ca=cos(A);
        pre->cb=cos(B);
        pre->cc=cos(C);
        
}

//X axis Rotation Calculation function(Matrix multipication)
static inline float calcX(float x,float y,float z,TrigRaito *pre){
    return pre->cc * pre->cb * x + pre->cc * pre->sb * pre->sa * y - pre->cc * pre->sb * pre->ca * z + 
           pre->sc * pre->ca * y + pre->sc * pre->sa * z;
}


//Y axis Rotation Calculation function(Matrix multipication)

static inline float calcY(float x,float y,float z,TrigRaito *pre){
    return pre->cc * pre->ca * y + pre->cc * pre->sa * z - pre->sc * pre->cb * x - 
           pre->sc * pre->sb * pre->sa * y + 
           pre->sc * pre->sb * pre->ca * z ;
}

//Z axis Rotation Calculation function(Matrix multipication)

static inline float calcZ(float x,float y,float z,TrigRaito *pre){
    return pre->sb * x - pre->cb * pre->sa * y + pre->cb * pre->ca * z;
}


void calculateRotation(float x,float y,float z,Buffer *buff,TrigRaito *pre,int *nor){
    
    float xl=calcX(x,y,z,pre); 
    float yl=calcY(x,y,z,pre);
    float zl=calcZ(x,y,z,pre);
    
    //Calculate the Luminance value

    float L=(ldirection[0]*calcX(nor[0],nor[1],nor[2],pre) +
                ldirection[1]*calcY(nor[0],nor[1],nor[2],pre) +
                ldirection[2]*calcZ(nor[0],nor[1],nor[2],pre));
    
    zl=zl+distance;
    
    float iz=1/zl;
    
    int xp=buff->w/2+horizontal_offset+(xl*K1*iz*2);
    int yp=buff->h/2+vertical_offset-(yl*K1*iz);
    
    int idx=(yp*buff->w)+xp;
    int aidx;
    if(L>0)
        aidx=L_len[illumination_switch]*L;
    else
        aidx=0;
    
    if(idx>=0 && idx<(buff->w*buff->h) && buff->zbuffer[idx]<iz && xp>=0 && xp<buff->w){
        buff->buffer[idx]=map[illumination_switch][aidx];
        buff->zbuffer[idx]=iz;
    }
}


//Buffer Initialization

void initBuffer(Buffer *buff){
    int size=buff->w*buff->h;
    
    buff->buffer=(wchar_t*)malloc(size*sizeof(wchar_t));
    buff->zbuffer=(float*)malloc(size*sizeof(float));
    if(buff->buffer==NULL || buff->zbuffer==NULL)
        Error(2);
}


//Reseting the buffer to the Background shade

int resetBuffer(Buffer *buff){
    int size=buff->w*buff->h;
    wmemset(buff->buffer,initial,size);
    memset(buff->zbuffer,0,size*sizeof(float));
}



void Error(int n){
FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
SetConsoleActiveScreenBuffer(GetStdHandle(STD_INPUT_HANDLE));
switch (n)
{
case 1:
    fprintf(stderr,"Screen Buffer");
    break;
case 2:
    fprintf(stderr,"Cube Buffer");
default:
    fprintf(stderr,"Unexpected Error");
    break;
}
exit(-1);
}

