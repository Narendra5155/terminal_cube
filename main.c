#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#include<time.h>
#include<wchar.h>
typedef struct Buffer Buffer;

struct Buffer{
    wchar_t *buffer;
    float *zbuffer;
    int w;
    int h;
};
const wchar_t initial=L'.';
const int L_len=9;
float horizontal_offset=0;
float vertical_offset=0;
//const wchar_t map[]=L"         ";
const wchar_t map[]=L" ▁▂▃▄▅▆▇█";
float ldirection[3]={0,0.7071,-0.7071};
float rotInc[3]={2,2,.4};
//char map[]="..,,--~~::;;==!!**##$$@@";
//map[4];
float A=2;
float B=2;
float C=2;
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
    float L=(ldirection[0]*calcX(nor[0],nor[1],nor[2]) +
                ldirection[1]*calcY(nor[0],nor[1],nor[2]) +
                ldirection[2]*calcZ(nor[0],nor[1],nor[2]));//+distance;
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
    buff->buffer=(wchar_t*)malloc(size*sizeof(wchar_t));
    buff->zbuffer=(float*)malloc(size*sizeof(float));
}
int resetBuffer(Buffer *buff){
    int size=buff->w*buff->h;
    wmemset(buff->buffer,initial,size);
    memset(buff->zbuffer,0,size*sizeof(float));
}
int main(){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    wchar_t *screen;
    int swidth=info.dwSize.X;
    int sheight=info.dwSize.Y;

    int cubeWidth=20;
    screen=(wchar_t*)malloc(sheight*swidth*sizeof(wchar_t));
    wmemset(screen,initial,swidth*sheight);
    Buffer buff1;
    buff1.h=cubeWidth*2;
    buff1.w=cubeWidth*4;
    initBuffer(&buff1);
    resetBuffer(&buff1);

    HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
   // HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    DWORD words_written;
    COORD origin;
    origin.X=0;
    origin.Y=0;
    float increment=0.6;
    //clock_t start,end;
    double deltatime=0;
    int pauseFlag=0;
    const float radtoDEG=180/3.14159265359;
    const  float Twopi=6.28318530718;
    LARGE_INTEGER start,end,frequency;
    LONGLONG elapsedticks;
    printf("\033[?25l");
    int normal[6][3]={0,0,1,1,0,0,-1,0,0,0,-1,0,0,0,-1,0,1,0};
    SetConsoleActiveScreenBuffer(console);
    while(!(GetAsyncKeyState(VK_SPACE)& 0x8000))
    {
        QueryPerformanceCounter(&start);
        QueryPerformanceFrequency(&frequency);
        resetBuffer(&buff1);
        wmemset(screen,initial,swidth*sheight);

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
            swidth=info.dwSize.X;
            sheight=info.dwSize.Y;
            free(screen); 
            screen=(wchar_t*)malloc(swidth*sheight*sizeof(wchar_t));
            wmemset(screen,initial,swidth*sheight);
            vertical_offset=0;
            horizontal_offset=0;
            /* buff1.h=h;
            buff1.w=w;
            free(buff1.buffer);
            free(buff1.zbuffer);
            initBuffer(&buff1);
            resetBuffer(&buff1); */

            //sprintf(screen+(h-1)*w,"%d %d",h,w);
        }

        for(int j=0,l=vertical_offset;  j<buff1.h && l<sheight; j++,l++){
            for(int k=0,m=horizontal_offset; k<buff1.w && m<swidth; k++,m++){
                screen[l*swidth+m]=buff1.buffer[j*buff1.w+k];
            }
        } 
        if(!(GetAsyncKeyState('P') & 0x8000) && !pauseFlag){
            swprintf(screen,swidth*(sheight),L"[FPS : %.2f]",1/deltatime);
            A+=rotInc[0]*deltatime;
            B+=rotInc[1]*deltatime;
            C+=rotInc[2]*deltatime;
        }
        else{
            pauseFlag=1;
            if(GetAsyncKeyState('S')&0x8000)
                A+=0.5*deltatime;
            if(GetAsyncKeyState('W')&0x8000)
                A-=0.5*deltatime;
            if(GetAsyncKeyState('D')&0x8000)
                B+=0.5*deltatime;
            if(GetAsyncKeyState('A')&0x8000)
                B-=0.5*deltatime; 
            if(GetAsyncKeyState('E')&0x8000)
                C+=0.5*deltatime;
            if(GetAsyncKeyState('Q')&0x8000)
                C-=0.5*deltatime;
            if(GetAsyncKeyState('K')&0x8000)
                K1+=20*deltatime;
            if(GetAsyncKeyState('J')&0x8000)
                K1-=20*deltatime;
            if(GetAsyncKeyState('C')&0x8000)
                distance-=20*deltatime;
            if(GetAsyncKeyState('V')&0x8000)
                distance+=20*deltatime;
            if(GetAsyncKeyState(VK_LEFT)&0x8000)
                horizontal_offset-=15*deltatime;
            if(GetAsyncKeyState(VK_RIGHT)&0x8000)
                horizontal_offset+=15*deltatime;
            if(GetAsyncKeyState(VK_DOWN)&0x8000)
                vertical_offset+=10*deltatime;
            if(GetAsyncKeyState(VK_UP)&0x8000)
                vertical_offset-=10*deltatime;
            if(GetAsyncKeyState('O')&0x8000)
                pauseFlag=0;
            
            swprintf(screen,swidth*sheight,L"[X Rotation : %.2f°,S-W]",A*radtoDEG);
            swprintf(screen+swidth,swidth*(sheight),L"[Y Rotation : %.2f°,A-D]",B*radtoDEG);
            swprintf(screen+swidth*2,swidth*(sheight),L"[Z Rotation : %.2f°,Q-E]",C*radtoDEG);
            swprintf(screen+swidth*3,swidth*(sheight),L"[K value : %.2f,J-K]",K1);
            swprintf(screen+swidth*4,swidth*(sheight),L"[Distance from camera : %.2f,C-V]",distance);
        }
        WriteConsoleOutputCharacterW(console,screen,swidth*sheight,origin,&words_written);
        if(A>Twopi)
            A-=Twopi;
        if(B>Twopi)
            B-=Twopi;
        if(C>Twopi)
            C-=Twopi;
        if(A<0)
            A=Twopi;
        if(B<0)
            B=Twopi;
        if(C<0)
            C=Twopi;
        QueryPerformanceCounter(&end);
        elapsedticks=end.QuadPart-start.QuadPart;
        deltatime=(double)(elapsedticks)/frequency.QuadPart;
        /* A+=0.05;
        B+=0.05;
        C+=0.01; */
    }
    CloseHandle(console);
    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    printf("Screen Width : %d\nScreen Height: %d\n",swidth,sheight);
    return 0;
}

