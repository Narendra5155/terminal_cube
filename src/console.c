#include<windows.h>
#include"console.h"
#include<stdio.h>
#include<math.h>
#include<wchar.h>
#include<stdlib.h>

// pre calculate the sines and cosines

void CalclulateAngles(){
        sa=sin(A);
        sb=sin(B);
        sc=sin(C);
        ca=cos(A);
        cb=cos(B);
        cc=cos(C);
        
}

//X axis Rotation Calculation function(Matrix multipication)
float calcX(float x,float y,float z){
    return cc * cb * x + cc * sb * sa * y - cc * sb * ca * z + 
           sc * ca * y + sc * sa * z;
}


//Y axis Rotation Calculation function(Matrix multipication)

float calcY(float x,float y,float z){
    return cc * ca * y + cc * sa * z - sc * cb * x - 
           sc * sb * sa * y + 
           sc * sb * ca * z ;
}

//Z axis Rotation Calculation function(Matrix multipication)

float calcZ(float x,float y,float z){
    return sb * x - cb * sa * y + cb * ca * z;
}


void calculateRotation(float x,float y,float z,Buffer *buff,char ch,int *nor){
    
    float xl=calcX(x,y,z); 
    float yl=calcY(x,y,z);
    float zl=calcZ(x,y,z);
    
    //Calculate the Luminance value

    float L=(ldirection[0]*calcX(nor[0],nor[1],nor[2]) +
                ldirection[1]*calcY(nor[0],nor[1],nor[2]) +
                ldirection[2]*calcZ(nor[0],nor[1],nor[2]));
    
    zl=zl+distance;
    
    float iz=1/zl;
    
    int xp=buff->w/2+(xl*K1*iz*2);
    int yp=buff->h/2-(yl*K1*iz);
    
    int idx=(yp*buff->w)+xp;
    int aidx;
    if(L>0)
        aidx=L_len[illumination_switch]*L;
    else
        aidx=0;
    
    if(idx>=0 && idx<(buff->w*buff->h) && buff->zbuffer[idx]<iz){
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

//Check the Key Press

int isKeydown(u_int x){
    return (GetAsyncKeyState(x)&0x8000) && (GetForegroundWindow()==hWnd);
}


//Key Bindings

void keyCheck(){

            if(isKeydown('S'))
                A+=0.5*deltatime;
            if(isKeydown('W'))
                A-=0.5*deltatime;
            if(isKeydown('D'))
                B+=0.5*deltatime;
            if(isKeydown('A'))
                B-=0.5*deltatime; 
            if(isKeydown('E'))
                C+=0.5*deltatime;
            if(isKeydown('Q'))
                C-=0.5*deltatime;
            if(isKeydown('K'))
                K1+=20*deltatime;
            if(isKeydown('J'))
                K1-=20*deltatime;
            if(isKeydown('C'))
                distance-=20*deltatime;
            if(isKeydown('V'))
                distance+=20*deltatime;
            if(isKeydown('M'))
                cubeWidth+=5*deltatime;
            if(isKeydown('N'))
                cubeWidth-=5*deltatime;

            /* if(isKeydown(VK_LEFT))
                horizontal_offset-=15*deltatime;
            if(isKeydown(VK_RIGHT))
                horizontal_offset+=15*deltatime;
            if(isKeydown(VK_DOWN))
                vertical_offset+=10*deltatime;
            if(isKeydown(VK_UP))
                vertical_offset-=10*deltatime; */
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

