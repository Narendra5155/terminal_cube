#ifndef CONSOLE_H
#define CONSOLE_H

#include<windows.h>
#include<stdlib.h>
#include<wchar.h>
#define EXTERN extern

typedef struct Buffer Buffer;
struct Buffer{
    wchar_t *buffer;
    float *zbuffer;
    int w;
    int h;
};
typedef struct TrigRaito{
    float sa;
    float sb;
    float sc;
    float ca;
    float cb;
    float cc;
}TrigRaito;

void Error(int n);
void calculateRotation(float x,float y,float z,Buffer *buff,TrigRaito *pre,int *nor);
void CalclulateAngles();
void initBuffer(Buffer *buff);
int resetBuffer(Buffer *buff);
static inline void printtoScreen(const wchar_t *str, wchar_t *screen)
{
    int n = wcslen(str);
    for (int i = 0; i < n ;i++){
        screen[i] = str[i];
    }
}

EXTERN const float radtoDEG;     //Defining The value of pi and 2pi for degree calculations
EXTERN const  float Twopi;

EXTERN HWND hWnd;                                  // Handle Variable for the current window

EXTERN double deltatime;                         // Delta Time Variable

EXTERN const wchar_t initial;                  // Background Shade

EXTERN int illumination_switch;
EXTERN const int L_len[2];
EXTERN const wchar_t map[50][50];

/* const int L_len=9;                          //Length of the illumination and illumination gradient
const wchar_t map[]=L" ▁▂▃▄▅▆▇█"; */

EXTERN const float ldirection[3];     // Illumination Vector

EXTERN float horizontal_offset;                  // offset for the screen buffer
EXTERN float vertical_offset;

EXTERN const float rotInc[3];                   //Rotation Increment for the three axis roation

EXTERN float cubeWidth;                           //Define cube properties
EXTERN float A;                                  //Defining the rotation angle variable
EXTERN float B;
EXTERN float C;

EXTERN float distance;                         //Distance form Camera and the screen distance from  
EXTERN float K1;  

#endif