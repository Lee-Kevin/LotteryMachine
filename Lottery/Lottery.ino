// demo for lottery 2015
// loovee
// 2016-1-11
#include <Adafruit_NeoPixel.h>
#include <Hercules.h>
#include <EEPROM.h>
#include "SoftwareSerial.h"
#include "Adafruit_Thermal.h"

// PLEASE MODIFY THIS NUMBER WHEN ADD NUM TO LIST
#define ULK_NUM     140                 // the number of unlucky guy
// THIS LIST IS GET FROM THE 4-5TH REWARD
// THIS LIST IS GET FROM WANT ZHAOWU
int UnluckyList[ULK_NUM] = 
{
298,
169,
273,
114,
39,
231,
288,
5,
246,
101,
202,
259,
251,
24,
26,
235,
45,
40,
254,
222,
284,
66,
51,
176,
48,
151,
185,
37,
177,
199,
056,
217,
198,
76,
47,
99,
54,
197,
189,
311,
9,
50,
35,
253,
73,
230,
196,
131,
155,
141,
309,
91,
122,
156,
93,
191,
12,
92,
270,
30,
36,
228,
7,
252,
103,
97,
245,
22,
84,
183,
10,
82,
61,
208,
108,
74,
266,
236,
277,
226,
112,
287,
58,
143,
21,
278,
118,
182,
221,
88,
307,
274,
218,
250,
301,
157,
184,
62,
18,
126,
121,
100,
75,
148,
205,
29,
125,
174,
271,
111,
94,
107,
104,
229,
139,
167,
67,
33,
129,
78,
233,
215,
213,
89,
3,
203,
206,
59,
105,
123,
90,
171,
41,
124,
165,
247,
46,
79,
234,
49,
};

// FUNCTION SWITCH
#define PRINTER_EN  1
#define MOTOR_EN    1

// THIS LIST IS GET FROM HR - JIESHAN
#define ULK_NUM2    92
int UnluckyList2[ULK_NUM2] = 
{
    2,   25, 34, 37, 41, 46, 75, 90,104,106,
    107,111,113,115,118,119,120,127,143,147,
    158,159,160,167,175,180,181,190,194,199,
    200,202,203,211,212,215,216,219,227,229,
    237,250,263,264,265,266,267,268,269,270,
    271,272,273,274,275,276,277,278,279,280,
    281,282,283,284,285,286,287,288,289,290,
    291,292,293,294,295,296,297,298,299,300,
    301,302,303,304,304,306,307,308,309,310,
    311,312,
};

// PRINTER
#define TX_PIN A5 // Arduino transmit  YELLOW WIRE  labeled RX on printer
#define RX_PIN A4 // Arduino receive   GREEN WIRE   labeled TX on printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

// PIN DEFINE
#define PIN_2812    12

#define NUM_START   1
#define NUM_END     312

#define PIN_BTN     A1
#define PIN_LED     A0
#define BTH_HIGH    LOW

#define MOTOR_SPEED 15

// WS2812B LED
Adafruit_NeoPixel strip = Adafruit_NeoPixel(9, PIN_2812, NEO_GRB + NEO_KHZ800);

int __R = -1;
int __G = -1;
int __B = -1;

int __GnumEEP = 0;

// CLEAR EEPROM, TO START A NEW ROUND
void clearEEP()
{
    __GnumEEP = 0;
    for (int i = 0; i < 512; i++)
        EEPROM.write(i, 0);
}

// ADD A NUMBER TO EEPROM
void addNumEEP(int num)
{
    __GnumEEP = EEPROM.read(0);
    __GnumEEP++;
    
    Serial.print("Add Num to List - ");
    Serial.println(__GnumEEP);
    
    if(num>=200)
    {
        EEPROM.write(2*__GnumEEP-1, 1);
        EEPROM.write(2*__GnumEEP, num-200);
    }
    else
    {
        EEPROM.write(2*__GnumEEP-1, 0);
        EEPROM.write(2*__GnumEEP, num);
    }
}

// SET COLOR TO WS2812B
void setRGB(int r, int g, int b)
{
    if(r == __R && g == __G && b == __B)return;
    __R = r;
    __G = g;
    __B = b;
    
    for(int i=0; i< strip.numPixels(); i++)
    {
        strip.setPixelColor(i, r, g, b);
    }
    strip.show();
}

// ADD A NUMBER TO EEPROM
void addList(int num)
{
    __GnumEEP = EEPROM.read(0);
    __GnumEEP++;
    EEPROM.write(0, __GnumEEP);
    
    Serial.print("Add Num to List - ");
    Serial.println(__GnumEEP);
    
    if(num>=200)
    {
        EEPROM.write(2*__GnumEEP-1, 1);
        EEPROM.write(2*__GnumEEP, num-200);
    }
    else
    {
        EEPROM.write(2*__GnumEEP-1, 0);
        EEPROM.write(2*__GnumEEP, num);
    }
}

// FIND IF A NUMBER IS IN LIST
bool findList(int num)
{
    for(int i=0; i<ULK_NUM; i++)
    {
        if(num == UnluckyList[i])return 1;
    }
    
    for(int i=0; i<ULK_NUM2; i++)
    {
        if(num == UnluckyList2[i])return 1;
    }

    for(int i=0; i<__GnumEEP; i++)
    {
        int tmp = 0;
        if(1==EEPROM.read(2*i+1))tmp+=200;
        tmp += EEPROM.read(2*i+2);

        if(tmp == num)return 1;
    }
    return 0;
}

void setup()
{
    Serial.begin(115200);
    pinMode(PIN_BTN, INPUT);
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_BTN, HIGH);
    digitalWrite(PIN_LED, HIGH);
    
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    
    MOTOR.begin();                      // initialize
    
    // NOTE: SOME PRINTERS NEED 9600 BAUD instead of 19200, check test page.
    mySerial.begin(19200);  // Initialize SoftwareSerial
    //Serial1.begin(19200); // Use this instead if using hardware serial
    printer.begin();        // Init printer (same regardless of serial type)
    
    __GnumEEP = EEPROM.read(0);
    
    Serial.println("sys started!");
    Serial.println("Lottery of annual meeting 2016");
    Serial.print("num of EEP = ");
    Serial.println(__GnumEEP);
}

void loop()
{
    if(digitalRead(PIN_BTN) == BTH_HIGH)
    {
        delay(20);
 
        unsigned long timer_start = micros();
        
        if(digitalRead(PIN_BTN) == BTH_HIGH)
        {
#if MOTOR_EN
            MOTOR.setSpeedDir(MOTOR_SPEED, DIRR);
#endif
            setRGB(5, 69, 175);
            
            digitalWrite(PIN_LED, LOW);
            while(digitalRead(PIN_BTN) == BTH_HIGH);
            delay(10);
            while(digitalRead(PIN_BTN) == BTH_HIGH);
            delay(10);
            while(digitalRead(PIN_BTN) == BTH_HIGH);
            delay(10);
            while(digitalRead(PIN_BTN) == BTH_HIGH);
            delay(10);

START:
            timer_start = micros() - timer_start;
            
            int luckguy = timer_start%1000;
            
            luckguy = map(luckguy, 0, 999, NUM_START, NUM_END);
            
            if(findList(luckguy))
            {
                Serial.print("Ag: ");
                Serial.println(luckguy);
                goto START;
            }
            
            Serial.println(luckguy);
            digitalWrite(PIN_LED, HIGH);
#if MOTOR_EN
            MOTOR.setSpeedDir(10, DIRR);
#endif
            printNum(luckguy);
#if MOTOR_EN
            MOTOR.setSpeedDir(0, DIRR);
#endif
            addList(luckguy);
        }
    }
    
    setRGB(0, 255, 0);
    
    if(Serial.available())
    {
        if('c' == Serial.read() || 'C' == Serial.read())
        {
            Serial.println("clear eeprom");
            clearEEP();
            Serial.println("clear ok");
        }
    }
}

// PRINT A NUMBER
void printNum(int num)
{
#if PRINTER_EN
    printer.inverseOn();
    printer.println(F("Seeedstudio"));
    printer.inverseOff();
    
    printer.println(F("Lottery of annual meeting"));
    printer.println(F("2016-1-14"));
    
    printer.println(F("Lucky number is: \r\n"));
    
    printer.justify('C');
    printer.setSize('L');           // Set type size, accepts 'S', 'M', 'L'
    printer.println(num); 
    printer.setDefault();           // Restore printer to defaults
    printer.println("\r\n\r\n");
#endif
}

// END FILE