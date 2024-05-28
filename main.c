
#include"console.h"
#include<math.h>
#include<stdio.h>
#include<time.h>
#include<wchar.h>
#include<stdlib.h>
#include<windows.h>

const float radtoDEG=180/3.14159265359;      //Defining The value of pi and 2pi for degree calc
const  float Twopi=6.28318530718;
HWND hWnd;                                  // Handle Variable for the current window
double deltatime=0.0;                         // Delta Time Variable
double collectedtime=0.0;
const wchar_t initial=L'.';                  // Background Shade
int illumination_switch=1;
const int L_len[2]={5,9};
const wchar_t map[2][10]={L" ░▒▓█",L" ▁▂▃▄▅▆▇█"};
/* const int L_len=9;                          //Length of the illumination and illumination gradient
const wchar_t map[]=L" ▁▂▃▄▅▆▇█"; */
const float ldirection[3]={0,0.5,-0.86602540378};     // Illumination Vector
float horizontal_offset=0.0;                  // offset for the screen buffer
float vertical_offset=0.0;
const float rotInc[3]={2.0,2.0,.4};                   //Rotation Increment for the three axis roation
float cubeWidth=20.0;                           //Define cube properties
float A=0.0;                                  //Defining the rotation angle variable
float B=0.0;
float C=0.0;
float sa,sb,sc,ca,cb,cc;                    //Variable to precomute sine and cosine
float distance=200.0;                         //Distance form Camera and the screen distance from
float K1=100.0;  
TrigRaito raito;

//Main function call


int main(){
    
    //Get the cureent console buffer information

    hWnd = GetForegroundWindow();
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE stdHand=GetStdHandle(STD_INPUT_HANDLE);
    DWORD oldMode,newMode;
    GetConsoleMode(stdHand,&oldMode);
    newMode=ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT |ENABLE_EXTENDED_FLAGS|ENABLE_PROCESSED_INPUT;
    SetConsoleMode(stdHand,newMode);
    HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    DWORD conMode;
    conMode=ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT |ENABLE_EXTENDED_FLAGS|ENABLE_PROCESSED_INPUT;
    SetConsoleMode(console,conMode);
    GetConsoleScreenBufferInfo(console,&info);
    
    
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
    SetConsoleActiveScreenBuffer(console);

    INPUT_RECORD inRecord[128]={};
    DWORD num;



    //SetConsoleMode(console,conMode);
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
    int loopOver=0;
    
    //Animation loop Start
    while(!loopOver)
    {
        //Start the clock
        QueryPerformanceCounter(&start);
        QueryPerformanceFrequency(&frequency);

        //Pre calculating sin and cos
        CalclulateAngles(&raito);
        
        
        //Plotting the points in the cube buffer

        for(float cubeX=-cubeWidth;cubeX<cubeWidth;cubeX+=increment){
            for(float cubeY=-cubeWidth;cubeY<cubeWidth;cubeY+=increment){
                calculateRotation(cubeX, cubeY, cubeWidth, &buff1,&raito,normal[0]);
                calculateRotation(cubeWidth, cubeY, -cubeX, &buff1,&raito,normal[1]); 
                calculateRotation(-cubeWidth, cubeY, cubeX, &buff1,&raito,normal[2]);
                calculateRotation(cubeX, -cubeWidth, cubeY, &buff1,&raito,normal[3]);
                calculateRotation(cubeX, -cubeY, -cubeWidth, &buff1,&raito,normal[4]);
                calculateRotation(cubeX, cubeWidth, -cubeY, &buff1,&raito,normal[5]);
 
            }
        }
        
       // illumination_switch=1;
        //inRecord[1].Event.KeyEvent.bKeyDown.
    GetNumberOfConsoleInputEvents(stdHand,&num);
    if(num>0){
        ReadConsoleInput(stdHand,inRecord,128,&num);
        for(unsigned int i=0;i<num;i++){
            switch(inRecord[i].EventType){
                case KEY_EVENT:
                    switch(inRecord[i].Event.KeyEvent.wVirtualKeyCode){
                         case'S':
                         if(pauseFlag)
                             A+=0.5*collectedtime;
                            break;
                         case'W':
                         if(pauseFlag)
                             A-=0.5*collectedtime;
                            break;
                         case'D':
                         if(pauseFlag)
                             B+=0.5*collectedtime;
                            break;
                         case'A':
                         if(pauseFlag)
                             B-=0.5*collectedtime; 
                            break;
                         case'E':
                         if(pauseFlag)
                             C+=0.5*collectedtime;
                            break;
                         case'Q':
                         if(pauseFlag)
                             C-=0.5*collectedtime;
                            break;
                         case'K':
                         if(pauseFlag)
                             K1+=20*collectedtime;
                            break;
                         case'J':
                         if(pauseFlag)
                             K1-=20*collectedtime;
                            break;
                         case'C':
                         if(pauseFlag)
                             distance-=20*collectedtime;
                            break;
                         case'V':
                         if(pauseFlag)
                             distance+=20*collectedtime;
                            break;
                         case'M':
                         if(pauseFlag)
                             cubeWidth+=5*collectedtime;
                            break;
                         case'N':
                         if(pauseFlag)
                             cubeWidth-=5*collectedtime;
                            break;
                         case'P':
                            if(!inRecord[i].Event.KeyEvent.bKeyDown)
                            pauseFlag=(pauseFlag==1)?0:1;
                            break;
                         case 'R':
                            if(!inRecord[i].Event.KeyEvent.bKeyDown){
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
                            break;
                        case VK_SPACE:
                            loopOver=1;
                            break;
                        case VK_LEFT:
                                horizontal_offset-=15*collectedtime;
                            break;
                        case VK_RIGHT:
                                horizontal_offset+=15*collectedtime;
                            break;
                        case VK_DOWN:
                                vertical_offset+=10*collectedtime;
                            break;
                        case VK_UP:
                                vertical_offset-=10*collectedtime;
                            break;
                        case 'I':
                        if(!inRecord[i].Event.KeyEvent.bKeyDown)
                                illumination_switch=(illumination_switch==1)?0:1;
                            break;
                         
                    }
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    swidth=inRecord[i].Event.WindowBufferSizeEvent.dwSize.X;
                    sheight=inRecord[i].Event.WindowBufferSizeEvent.dwSize.Y; 
                    buff1.w=inRecord[i].Event.WindowBufferSizeEvent.dwSize.X;
                    buff1.h=inRecord[i].Event.WindowBufferSizeEvent.dwSize.Y;
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
                    break;
                case MOUSE_EVENT:

                if(inRecord[i].Event.MouseEvent.dwButtonState==FROM_LEFT_1ST_BUTTON_PRESSED){
                    vertical_offset=inRecord[i].Event.MouseEvent.dwMousePosition.Y-sheight/2;
                    horizontal_offset=inRecord[i].Event.MouseEvent.dwMousePosition.X-swidth/2;
                }
            } 
        }
        collectedtime=0.0;
    }
    else{
        collectedtime+=(collectedtime<0.1)?deltatime:0;
    }
        
        //Refreshing the window

        /* if(isKeydown('R')){
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
        } */

        //Writing the data to the screen buffer from the cube buffer

        for(int j=0;  j<buff1.h && j<sheight; j++){
            for(int k=0; k<buff1.w && k<swidth; k++){
                screen[j*swidth+k]=buff1.buffer[j*buff1.w+k];
            }
        } 

        //Pause The animation

        if(!pauseFlag){
            swprintf(screen,swidth*sheight,L"[FPS : %5d]",(int)(1/deltatime));
            swprintf(screen+swidth,swidth*sheight,L"[Press R to Refresh]");
            swprintf(screen+swidth*2,swidth*sheight,L"[Press P to Pause]");
            A+=rotInc[0]*deltatime;
            B+=rotInc[1]*deltatime;
            C+=rotInc[2]*deltatime;
        }
        else{
            /* pauseFlag=1;
            //keyCheck();
            if(isKeydown('O'))
                pauseFlag=0; */
            
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
        

        /* if(isKeydown(VK_LEFT))
                horizontal_offset-=15*deltatime;
        if(isKeydown(VK_RIGHT))
                horizontal_offset+=15*deltatime;
        if(isKeydown(VK_DOWN))
                vertical_offset+=10*deltatime;
        if(isKeydown(VK_UP))
                vertical_offset-=10*deltatime;
        if(isKeydown('I'))
                illumination_switch=0; */
        
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
    system("cls");
    //Print the last known resolution and FPS
    printf("Screen Width : %d\nScreen Height: %d\nFPS :%.2f\n",swidth,sheight,1/deltatime);
    return 0;

}
