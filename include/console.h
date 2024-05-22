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


void Error(int n);
float calcX(float x,float y,float z);
float calcY(float x,float y,float z);
float calcZ(float x,float y,float z);
void calculateRotation(float x,float y,float z,Buffer *buff,char ch,int *nor);
void initBuffer(Buffer *buff);
int resetBuffer(Buffer *buff);
int isKeydown(u_int x);
void keyCheck();


EXTERN const float radtoDEG;     //Defining The value of pi and 2pi for degree calculations
EXTERN const  float Twopi;

EXTERN HWND hWnd;                                  // Handle Variable for the current window

EXTERN double deltatime;                         // Delta Time Variable

EXTERN const wchar_t initial;                  // Background Shade

EXTERN int illumination_switch;
EXTERN const int L_len[2];
EXTERN const wchar_t map[2][10];

/* const int L_len=9;                          //Length of the illumination and illumination gradient
const wchar_t map[]=L" ▁▂▃▄▅▆▇█"; */

EXTERN float ldirection[3];     // Illumination Vector

EXTERN float horizontal_offset;                  // offset for the screen buffer
EXTERN float vertical_offset;

EXTERN float rotInc[3];                   //Rotation Increment for the three axis roation

EXTERN float cubeWidth;                           //Define cube properties
EXTERN float A;                                  //Defining the rotation angle variable
EXTERN float B;
EXTERN float C;

EXTERN float sa,sb,sc,ca,cb,cc;                    //Variable to precomute sine and cosine


EXTERN float distance;                         //Distance form Camera and the screen distance from  
EXTERN float K1;  