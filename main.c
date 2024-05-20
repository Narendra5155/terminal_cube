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
void Error(int n);
float calcX(float x,float y,float z);
float calcY(float x,float y,float z);
float calcZ(float x,float y,float z);
void calculateRotation(float x,float y,float z,Buffer *buff,wchar_t ch,int *nor);
void initBuffer(Buffer *buff);
int resetBuffer(Buffer *buff);
int isKeydown(u_int x);
void keyCheck();

//Define the buffer structure for the 3D object


const float radtoDEG=180/3.14159265359;      //Defining The value of pi and 2pi for degree calculations
const  float Twopi=6.28318530718;

HWND hWnd;                                  // Handle Variable for the current window

double deltatime=0;                         // Delta Time Variable

const wchar_t initial=L'.';                  // Background Shade

int illumination_switch=1;
const int L_len[2]={5,9};
const wchar_t map[2][10]={L" ░▒▓█",L" ▁▂▃▄▅▆▇█"};

/* const int L_len=9;                          //Length of the illumination and illumination gradient
const wchar_t map[]=L" ▁▂▃▄▅▆▇█"; */

float ldirection[3]={0,0.5,-0.86602540378};     // Illumination Vector

float horizontal_offset=0;                  // offset for the screen buffer
float vertical_offset=0;



float rotInc[3]={2,2,.4};                   //Rotation Increment for the three axis roation

float cubeWidth=20;                           //Define cube properties
float A=0;                                  //Defining the rotation angle variable
float B=0;
float C=0;

float sa,sb,sc,ca,cb,cc;                    //Variable to precomute sine and cosine


float distance=200;                         //Distance form Camera and the screen distance from  
float K1=100;                               //the object change this to change depth perception



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


void calculateRotation(float x,float y,float z,Buffer *buff,wchar_t ch,int *nor){
    
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
        //buff->buffer[idx]=map[illumination_switch][aidx];
        buff->buffer[idx]=ch;
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

//Main function call


int main(){
    
    
    //Get the cureent console buffer information

    hWnd = GetForegroundWindow();
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    
    
    //Set the screen buffer metadata

    wchar_t *screen;
    int swidth=info.dwSize.X;
    int sheight=info.dwSize.Y;
    screen=(wchar_t*)malloc(sheight*swidth*sizeof(wchar_t));
    if(screen==NULL)
        Error(1);
    wmemset(screen,initial,swidth*sheight);

    
    //Set the size of the Cube buffer
    
    Buffer buff1;
    buff1.h=info.dwSize.Y;
    buff1.w=info.dwSize.X;
    initBuffer(&buff1);
    resetBuffer(&buff1);
    
    //The normal vectors of the six of the cube faces
    int normal[6][3]={0,0,1,1,0,0,-1,0,0,0,-1,0,0,0,-1,0,1,0};
    
    //Generate a new console buffer

    HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    DWORD words_written;
    COORD origin;
    origin.X=0;
    origin.Y=0;
    
    //Inrement for the coordinate calculation
    float increment=0.6;
    
    //Pause flag
    int pauseFlag=0;

    //Set up the system clock variables
    LARGE_INTEGER start,end,frequency;
    LONGLONG elapsedticks;

    //swap the generated buffer with the standard buffer
    SetConsoleActiveScreenBuffer(console);
    
    //Animation loop Start
    while(!isKeydown(VK_SPACE))
    {
        //Start the clock
        QueryPerformanceCounter(&start);
        QueryPerformanceFrequency(&frequency);

        //Pre calculating sin and cos
        sa=sin(A);
        sb=sin(B);
        sc=sin(C);
        ca=cos(A);
        cb=cos(B);
        cc=cos(C);
        
        
        //Plotting the points in the cube buffer

        for(float cubeX=-cubeWidth;cubeX<cubeWidth;cubeX+=increment){
            for(float cubeY=-cubeWidth;cubeY<cubeWidth;cubeY+=increment){
                calculateRotation(cubeX, cubeY, cubeWidth, &buff1,L' ',normal[0]);
                calculateRotation(cubeWidth, cubeY, -cubeX, &buff1,L'░',normal[1]); 
                calculateRotation(-cubeWidth, cubeY, cubeX, &buff1,L'▒',normal[2]);
                calculateRotation(cubeX, -cubeWidth, cubeY, &buff1,L'▓',normal[3]);
                calculateRotation(cubeX, -cubeY, -cubeWidth, &buff1,L'█',normal[4]);
                calculateRotation(cubeX, cubeWidth, -cubeY, &buff1,L'▅',normal[5]);
 
            }
        }
        
        illumination_switch=1;
        //Refreshing the window

        if(isKeydown('R')){
            GetConsoleScreenBufferInfo(console,&info);
            swidth=info.dwSize.X;
            sheight=info.dwSize.Y; 
            buff1.w=info.dwSize.X;
            buff1.h=info.dwSize.Y;
            free(buff1.buffer);
            free(buff1.zbuffer);
            initBuffer(&buff1);
            resetBuffer(&buff1);
            free(screen); 
            screen=(wchar_t*)malloc(swidth*sheight*sizeof(wchar_t));
            if(screen==NULL)
                Error(1);
            wmemset(screen,initial,swidth*sheight);
            vertical_offset=0;
            horizontal_offset=0;
        }

        //Writing the data to the screen buffer from the cube buffer

        for(int j=0,l=vertical_offset;  j<buff1.h && l<sheight; j++,l++){
            for(int k=0,m=horizontal_offset; k<buff1.w && m<swidth; k++,m++){
                screen[l*swidth+m]=buff1.buffer[j*buff1.w+k];
            }
        } 

        //Pause The animation

        if(!isKeydown('P') && !pauseFlag){
            swprintf(screen,swidth*sheight,L"[FPS : %.2f]",1/deltatime);
            swprintf(screen+swidth,swidth*sheight,L"[Press R to Refresh]");
            swprintf(screen+swidth*2,swidth*sheight,L"[Press P to Pause]");
            A+=rotInc[0]*deltatime;
            B+=rotInc[1]*deltatime;
            C+=rotInc[2]*deltatime;
        }
        else{
            pauseFlag=1;
            keyCheck();
            if(isKeydown('O'))
                pauseFlag=0;
            
            //Print the info in the pause mode

            swprintf(screen+swidth*0,swidth*sheight,L"[X Rotation : %5.2f°,S-W]",A*radtoDEG);
            swprintf(screen+swidth*1,swidth*sheight,L"[Y Rotation : %5.2f°,A-D]",B*radtoDEG);
            swprintf(screen+swidth*2,swidth*sheight,L"[Z Rotation : %5.2f°,Q-E]",C*radtoDEG);
            swprintf(screen+swidth*3,swidth*sheight,L"[K value    : %5.2f ,J-K]",K1);
            swprintf(screen+swidth*4,swidth*sheight,L"[Camera     : %5.2f ,C-V]",distance);
            swprintf(screen+swidth*5,swidth*sheight,L"[FPS        : %5d     ]",(int)(1/deltatime));
            swprintf(screen+swidth*6,swidth*sheight,L"[Cube Width : %5.2f,N-M ]",cubeWidth);
            swprintf(screen+swidth*7,swidth*sheight,L"[Press O to Resume      ]");
        }
        
        //Keep the value of rotation angle within bounds

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
        

        if(isKeydown(VK_LEFT))
                horizontal_offset-=15*deltatime;
        if(isKeydown(VK_RIGHT))
                horizontal_offset+=15*deltatime;
        if(isKeydown(VK_DOWN))
                vertical_offset+=10*deltatime;
        if(isKeydown(VK_UP))
                vertical_offset-=10*deltatime;
        if(isKeydown('I'))
                illumination_switch=0;
        
        //Write the screen buffer to the console

        WriteConsoleOutputCharacterW(console,screen,swidth*sheight,origin,&words_written);
        
        //resetting the buffers for the next frame
        resetBuffer(&buff1);
        wmemset(screen,initial,swidth*sheight);
        
        //delta time calculation
        QueryPerformanceCounter(&end);
        elapsedticks=end.QuadPart-start.QuadPart;
        deltatime=(double)(elapsedticks)/frequency.QuadPart;

    }

    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE)); 
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    //Free the console handle 
    CloseHandle(console);
    
    //Print the last known resolution and FPS
    printf("Screen Width : %d\nScreen Height: %d\nFPS :%.2f\n",swidth,sheight,1/deltatime);
    return 0;

}
