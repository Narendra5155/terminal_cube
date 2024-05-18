#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#include<time.h>
typedef struct Buffer Buffer;

struct Buffer{
    char *buffer;
    float *zbuffer;
    int w;
    int h;
};
int L_len=9;
char map[]=".:-=+*#%@$";
float ldirection[3]={0,0,-1};
float rotInc[3]={2,2,0.4};
//char map[]="..,,--~~::;;==!!**##$$@@";
//map[4];
float A=0;
float B=0;
float C=0;
float sa,sb,sc,ca,cb,cc;
float distance=200;
float K1=100;
float calcX(float x,float y,float z){
    return cc * cb * x + cc * sb * sa * y - cc * sb * ca * z + 
           sc * ca * y + sc * sa * z;
}
float calcY(float x,float y,float z){
    return cc * ca * y + cc * sa * z - sc * cb * x - 
           sc * sb * sa * y + 
           sc * sb * ca * z ;
}
float calcZ(float x,float y,float z){
    return sb * x - cb * sa * y + cb * ca * z;
}
void calculateRotation(float x,float y,float z,Buffer *buff,char ch,int *nor){
    float xl=calcX(x,y,z); // remove the two later when rendering 3D
    float yl=calcY(x,y,z);
    float zl=calcZ(x,y,z);
    float L=(ldirection[0]*calcX(nor[0],nor[1],nor[2])+ldirection[1]*calcY(nor[0],nor[1],nor[2])+ldirection[2]*calcZ(nor[0],nor[1],nor[2]));//+distance;
    zl=zl+distance;
    float iz=1/zl;
    int xp=buff->w/2+(xl*K1*iz*2);
    int yp=buff->h/2-(yl*K1*iz);
    int idx=(yp*buff->w)+xp;
    int aidx;
    if(L>0)
        aidx=L_len*L;
    else
        aidx=0;
    if(idx>=0 && idx<(buff->w*buff->h) && buff->zbuffer[idx]<iz){
        buff->buffer[idx]=map[aidx];
        buff->zbuffer[idx]=iz;
    }
}
void initBuffer(Buffer *buff){
    int size=buff->w*buff->h;
    buff->buffer=(char*)malloc(size);
    buff->zbuffer=(float*)malloc(size*sizeof(float));
}
int resetBuffer(Buffer *buff){
    int size=buff->w*buff->h;
    memset(buff->buffer,' ',size);
    memset(buff->zbuffer,0,size*sizeof(float));
}
int main(){
    char *screen;
    int swidth=160;
    int sheight=44;
    int bwidth=160;
    int bheight=40;
    screen=(char*)malloc(sheight*swidth*sizeof(char));
    memset(screen,' ',swidth*sheight);
    Buffer buff1;
    buff1.h=bheight;
    buff1.w=bwidth;
    initBuffer(&buff1);
    resetBuffer(&buff1);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    int w=info.dwSize.X;
    int h=info.dwSize.Y;
    HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
   // HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    DWORD words_written;
    COORD origin;
    origin.X=0;
    origin.Y=0;
    int cubeWidth=20;
    float increment=0.6;
    clock_t start,end;
    double deltatime;
    printf("\033[?25l");
    int normal[6][3]={0,0,1,1,0,0,-1,0,0,0,-1,0,0,0,-1,0,1,0};
    SetConsoleActiveScreenBuffer(console);
    while(!(GetAsyncKeyState('Q')& 0x8000))
    {
        start=clock();
        resetBuffer(&buff1);
        memset(screen,' ',swidth*sheight);

        //Pre calculating sin and cos
        sa=sin(A);
        sb=sin(B);
        sc=sin(C);
        ca=cos(A);
        cb=cos(B);
        cc=cos(C);
        for(float cubeX=-cubeWidth;cubeX<cubeWidth;cubeX+=increment){
            for(float cubeY=-cubeWidth;cubeY<cubeWidth;cubeY+=increment){
                calculateRotation(cubeX, cubeY, cubeWidth, &buff1,'@',normal[0]);
                calculateRotation(cubeWidth, cubeY, -cubeX, &buff1,'#',normal[1]); 
                calculateRotation(-cubeWidth, cubeY, cubeX, &buff1,';',normal[2]);
                calculateRotation(cubeX, -cubeWidth, cubeY, &buff1,'?',normal[3]);
                calculateRotation(cubeX, -cubeY, -cubeWidth, &buff1,'/',normal[4]);
                calculateRotation(cubeX, cubeWidth, -cubeY, &buff1,'*',normal[5]);
 
            }
        }
        
       /*  printf("\033[1;1H");
        for (int k=0; k<buff1.h*buff1.w;k++){
            putchar((k%buff1.w)?buff1.buffer[k]:'\n');
        } */
        if((GetAsyncKeyState('R')& 0x8000)){
            GetConsoleScreenBufferInfo(console,&info);
            w=info.dwSize.X;
            h=info.dwSize.Y;
            buff1.h=h;
            buff1.w=w;
            free(buff1.buffer);
            free(buff1.zbuffer);
            initBuffer(&buff1);
            resetBuffer(&buff1);

            sprintf(screen+(h-1)*w,"%d %d",h,w);
        }

        for(int j=0; (j<h && j<buff1.h) ; j++){
            for(int k=0; (k<w && k<buff1.w) ; k++ ){
                screen[j*w+k]=buff1.buffer[j*buff1.w+k];
            }
        } 
        //sprintf(screen,"%d %d",h,w);
        WriteConsoleOutputCharacter(console,screen,swidth*sheight,origin,&words_written);
        end=clock();
        deltatime=(double)(end-start)/CLOCKS_PER_SEC;
        A+=rotInc[0]*deltatime;
        B+=rotInc[1]*deltatime;
        C+=rotInc[2]*deltatime;
        /* A+=0.05;
        B+=0.05;
        C+=0.01; */
    }
    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    printf("%lf",deltatime);
    return 0;
}

