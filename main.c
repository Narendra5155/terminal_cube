
#include<console.h>
#include<math.h>
#include<stdio.h>
#include<time.h>
#include<wchar.h>
#include<stdlib.h>
#include<windows.h>

const float radtoDEG=180/3.14159265359;      //Defining The value of pi and 2pi for degree calc
const  float Twopi=6.28318530718;                         
double deltatime=0.0;                         // Delta Time Variable
double collectedtime=0.0;
const wchar_t initial=L' ';                  // Background Shade
int illumination_switch=1;
const int L_len[2]={5,9};
const wchar_t map[50][50]={L" ░▒▓█",L".▁▂▃▄▅▆▇█"};      //Length of the illumination and illumination gradient
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
int normal[6][3]={                          //The normal vectors of the six of the cube faces
     0, 0, 1,
     1, 0, 0,
    -1, 0, 0,
     0,-1, 0,
     0, 0,-1,
     0, 1, 0
};
    

//Main function call


int main(){
    
    //Get the cureent console buffer information
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);
    
    
    HANDLE stdHand=GetStdHandle(STD_INPUT_HANDLE);
    DWORD oldMode,newMode,num, words_written;
    INPUT_RECORD inRecord[128]={};
    COORD origin={0,0};
    
    GetConsoleMode(stdHand,&oldMode);
    newMode=ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT |ENABLE_EXTENDED_FLAGS|ENABLE_PROCESSED_INPUT;
    SetConsoleMode(stdHand,newMode);
    
    //Generate a new console buffer
    HANDLE console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    //swap the generated buffer with the standard buffer
    SetConsoleActiveScreenBuffer(console);
    
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
    

    
    //Inrement for the coordinate calculation
    float increment=0.6;
    
    //Pause flag
    int pauseFlag=0;
    int loopOver=0;

    //Set up the system clock variables
    LARGE_INTEGER start,end,frequency;
    LONGLONG elapsedticks;

    
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
                                A=0;
                                B=0;
                                C=0;
                                K1=100;
                                distance=200;
                                horizontal_offset=0;
                                vertical_offset=0;
                                cubeWidth=20;
                            }
                            break;
                        case VK_SPACE:
                            loopOver=1;
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
    else
        collectedtime+=(collectedtime<0.1)?deltatime:0;
    
        
        //Writing the data to the screen buffer from the cube buffer

        for(int j=0;  j<buff1.h && j<sheight; j++){
            for(int k=0; k<buff1.w && k<swidth; k++){
                screen[j*swidth+k]=buff1.buffer[j*buff1.w+k];
            }
        } 

        //Pause The animation

        if(!pauseFlag){
            wchar_t out[100];
            swprintf(out, 100, L"[FPS : %5d]", (int)(1 / deltatime));
            printtoScreen(out, screen + 0 * swidth);
            swprintf(out,100,L"[Press R to Refresh]");
            printtoScreen(out, screen + 1 * swidth);
            swprintf(out,100,L"[Press P to Pause]");
            printtoScreen(out, screen + 2 * swidth);
            A+=rotInc[0]*deltatime;
            B+=rotInc[1]*deltatime;
            C+=rotInc[2]*deltatime;
        }
        else{
            wchar_t out[100];
            swprintf(out,100,L"[X Rotation : %5.2f°,S-W]",A*radtoDEG);
            printtoScreen(out, screen + 0 * swidth);
            swprintf(out,100,L"[Y Rotation : %5.2f°,A-D]",B*radtoDEG);
            printtoScreen(out, screen + 1 * swidth);
            swprintf(out,100,L"[Z Rotation : %5.2f°,Q-E]",C*radtoDEG);
            printtoScreen(out, screen + 2 * swidth);
            swprintf(out,100,L"[K value    : %5.2f ,J-K]",K1);
            printtoScreen(out, screen + 3 * swidth);
            swprintf(out,100,L"[Camera     : %5.2f ,C-V]",distance);
            printtoScreen(out, screen + 4 * swidth);
            swprintf(out,100,L"[FPS        : %5d     ]",(int)(1/deltatime));
            printtoScreen(out, screen + 5 * swidth);
            swprintf(out,100,L"[Cube Width : %5.2f,N-M ]",cubeWidth);
            printtoScreen(out, screen + 6 * swidth);
            swprintf(out,100,L"[Press O to Resume      ]");
            printtoScreen(out, screen + 7 * swidth);
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
    SetStdHandle(STD_INPUT_HANDLE,stdHand);
    SetConsoleMode(stdHand,oldMode);

    //Free the console handle 
    CloseHandle(console);

    //Print the last known resolution and FPS
    printf("Screen Width : %d\nScreen Height: %d\nFPS :%.2f\n",swidth,sheight,1/deltatime);
    
    return 0;

}
