//14-08-22 out gain now in db , encoder rotation direction can now be be configured and saved
// 21-6-22 in & out gain added and scanned during boot 
//18/6/22 out of range param detected are replaced with default value.+ long push to call specific menu TRig input (1sec)
// 31/5/2022 graphic ok, EEPROM ok, synth ok after adding void type to main functions fluxwrite in fluxamasynth library (bug!)
#include <Arduino.h>
//#include <U8g2lib.h> // huge library,but I only use basic u8x8 font no U8g2
#include <U8x8lib.h>
#include <EEPROM.h>
//#include <PREFERENCES.h> old fashion EPROM used : everything in one pool !!
#include <Fluxamasynth.h> // largely increased library, dont use the default one !
//#include <HardwareSerial.h>
//#include <Wire.h>
//#include <PgmChange.h>
#include <ESP32Encoder.h>  //ok
#include <ezButton.h> // ok with 10msec debouncing !!!!

Fluxamasynth synth = Fluxamasynth();
                          //0=default, 1 power,...
char const * drbname [47][5] =  // (81-35) +1 = 47 drum useful notes
{  { "Kick2       ","Kick2       ","*JazzBDrum2 ","Kick2       ","*Kick2      " },
   { "Kick        ","Kick        ","*JazzBDrum  ","Kick        ","*Kick       " },
   { "SideStick   ","SideStick   ","SideStick   ","SideStick   ","*RimShot    " },//37
   { "Snare       ","*Gated Snare","*BrushTap   ","*SnareDrum  ","*Snare      " },
   { "HandClap    ","HandClap    ","*BrushSlap  ","*Castanets  ","*HandClap   " },
   { "Snare2      ","Snare2      ","*BrushSwirl ","*SnareDrum2 ","*ElecSnare  " }, //40
   { "LowFloorTom ","LowFloorTom ","LowFloorTom ","*Timpani_F  ","*Ac.LowTom  " },
   { "Closed HiHat","Closed HiHat","Closed HiHat","*Timpani_Fs ","*ClosedHiHat" },
   { "HighFloorTom","HighFloorTom","HighFloorTom","*Timpani_G  ","*Ac.LowTom2 " },//43
   { "Pedal HiHat ","Pedal HiHat ","Pedal HiHat ","*Timpani_Gs ","*OpenHiHat2 " },
   { "Low Tom     ","Low Tom     ","Low Tom     ","*Timpani_A  ","*Ac.MidTom  " },
   { "Open HiHat  ","Open HiHat  ","Open HiHat  ","*Timpani_As ","*OpenHiHat  " }, //46
   { "LowMid Tom  ","LowMid Tom  ","LowMid Tom  ","*Timpani_B  ","*Ac.MidTom2 " },
   { "HighMid Tom ","HighMid Tom ","HighMid Tom ","*Timpani_C  ","*Ac.HighTom " },
   { "Crash Cymbal","Crash Cymbal","Crash Cymbal","*Timpani_Cs ","*CrashCymbal" }, //49
   { "High Tom    ","High Tom    ","High Tom    ","*Timpani_D  ","*Ac.HighTom2" },
   { "Ride Cymbal ","Ride Cymbal ","Ride Cymbal ","*Timpani_Ds ","*RideCymbal " },
   { "ChinesCymbal","ChinesCymbal","ChinesCymbal","*Timpani_E  ","------------" }, //52
   { "RideBell    ","RideBell    ","RideBell    ","*Timpani_F2 ","------------" },
   { "Tambourine  ","Tambourine  ","Tambourine  ","Tambourine  ","*Tambourine"  },
   { "SplashCymbal","SplashCymbal","SplashCymbal","SplashCymbal","------------" }, //55
   { "Cowbell     ","Cowbell     ","Cowbell     ","Cowbell     ","*Cowbell    " },
   { "CrashCymbal2","CrashCymbal2","CrashCymbal2","CrashCymbal2","------------" },
   { "Vibra Slap  ","Vibra Slap  ","Vibra Slap  ","Vibra Slap  ","------------" }, //58
   { "Ride Cymbal2","Ride Cymbal2","Ride Cymbal2","Ride Cymbal2","------------" },
   { "High Bongo  ","High Bongo  ","High Bongo  ","High Bongo  ","High Bongo  " },
   { "Low Bongo   ","Low Bongo   ","Low Bongo   ","Low Bongo   ","Low Bongo   " }, //61
   { "MutedH.Conga","MutedH.Conga","MutedH.Conga","MutedH.Conga","MutedH.Conga" },
   { "Open H.Conga","Open H.Conga","Open H.Conga","Open H.Conga","Open H.Conga" },
   { "LowConga    ","LowConga    ","LowConga    ","LowConga    ","LowConga    " }, //64
   { "High Timbale","High Timbale","High Timbale","High Timbale","High Timbale" },  //65
   { "Low Timbale ","Low Timbale ","Low Timbale ","Low Timbale ","Low Timbale " },
   { "High Agogo  ","High Agogo  ","High Agogo  ","High Agogo  ","High Agogo  " },  //67
   { "Low Agogo   ","Low Agogo   ","Low Agogo   ","Low Agogo   ","Low Agogo   " },  
   { "Cabasa      ","Cabasa      ","Cabasa      ","Cabasa      ","Cabasa      " },
   { "Maracas     ","Maracas     ","Maracas     ","Maracas     ","Maracas     " }, //70
   { "ShortWhistle","ShortWhistle","ShortWhistle","ShortWhistle","ShortWhistle" },  
   { "LongWhistle ","LongWhistle ","LongWhistle ","LongWhistle ","LongWhistle" },
   { "ShortGuiro  ","ShortGuiro  ","ShortGuiro  ","ShortGuiro  ","*VibraSlap  " }, //73
   { "LongGuiro   ","LongGuiro   ","LongGuiro   ","LongGuiro   ","------------" },  
   { "Claves      ","Claves      ","Claves      ","Claves      ","*Claves     " },
   { "H.WoodBlock ","H.WoodBlock ","H.WoodBlock ","H.WoodBlock ","------------" }, //76
   { "LowWoodBlock","LowWoodBlock","LowWoodBlock","LowWoodBlock","------------" },  
   { "Muted Cuica ","Muted Cuica ","Muted Cuica ","Muted Cuica ","------------" },
   { "Open Cuica  ","Open Cuica  ","Open Cuica  ","Open Cuica  ","------------" }, //79  
   { "MuteTriangle","MuteTriangle","MuteTriangle","MuteTriangle","------------" },  
   { "OpenTriangle","OpenTriangle","OpenTriangle","OpenTriangle","------------" }  //81
    } ;


//------------------ ENCODER ---------------------------------------------------------
ESP32Encoder encoder1; // left menu
ESP32Encoder encoder2; // center menu 
#define ENC1A 19 // GPIO19 = D19
#define ENC1B 18
#define ENC2A 15
#define ENC2B 14
int64_t enc1 =0; int64_t enc2 =0; int cnt =0;
byte encrotation =0; // used to store & retrieve clockwize or anticlockwise behavior of the 2 encoders
// the behavior cab be saved for each encoder, but you need to save to eprom, then reload to take effect
//------------ INPUT OUTPUT ------------------------------------------------------
// MIDI on TX2 (D17), OLED SDA = GPIO21, SCL= GPIO22 /////////

//------------- SELECTION SWITCHS ----------------------------------------------
#define B0 25 // DIGITAL IN GPIO25 : action menu left GPIO25 = D25
#define B1 26 // DIGITAL IN GPIO26 ; action menu central 
#define B2 27 // DIGITAL IN GPIO27 : action menu right
bool bt0 , bt1 , bt2 , bt0old, bt1old, bt2old , launcheprom ;
byte btselect = 255 ; // 0,1 or 2 if button pressed, 255 otherwise 
//---------- TRIG INPUTS 1..<NBR_OF_TRIG> ---AND DEBOUNCING -------------------------------------------
#define TRIG1 32 // Trigger 1   12 13  marche-pas ???? 4.// 32 33 OK if init first  34 35 : OK
#define TRIG2 33 // Trigger 2  
#define TRIG3 34 // Trigger 3  
#define TRIG4 35 // Trigger 4  
#define TRIG5 36 // Trigger 5  
#define TRIG6 39 // Trigger 6  
#define NBR_OF_TRIG 6 // number of input triggers to start each a drumnote  with 0< i < NBR_OF_TRIG 
#define DEBOUNCE_TIME 10 // the debounce time in millisecond, increase this time if it still chatters

ezButton button1(TRIG1); // create ezButton object that attach to pin GPIO
ezButton button2(TRIG2); 
ezButton button3(TRIG3); 
ezButton button4(TRIG4); 
ezButton button5(TRIG5); 
ezButton button6(TRIG6); 
byte trigpush[NBR_OF_TRIG] ; // to display corresponding trig menu when you keep the trig switch pressed during one second...
byte trigread =0; // temporary variable to store digital read of each trigger in the loop
int const tin[NBR_OF_TRIG] = {TRIG1,TRIG2,TRIG3,TRIG4,TRIG5,TRIG6} ; // used for the long press detection in the loop

//--------------- 6 Zones defined to display something ------------------------------
// FONTs are 8*8 128/8 = 16 char on 8 rows (fixed width, not proportional but easier to manage ) 
#define ZC0 0 //0 = upper left  
#define ZL0 0 // upper line 
#define ZB0 7 // size of zone 0 used to display something
#define ZC1 8 //  = start of upper right
#define ZL1 0
#define ZB1 7  // reste 2 blancs au milieu
#define ZC2 5  //  middle of screen to display result of action on 3 buttons and (current value during start)
#define ZL2 3  // middle line 03h = 4D
#define ZB2 7
#define ZC3 0   // 3 = lower left button
#define ZL3 6 //  
#define ZB3 5
#define ZC4 5  //  lower middle button
#define ZL4 6  //
#define ZB4 5 
#define ZC5 11  // lower right button
#define ZL5 6 // 
#define ZB5 5
#define ZC6 0  // middle line from left
#define ZL6 3 // 
#define ZB6 12 // middle right to have the longer text area 

#define NB_ZONES 6 // ZB6 is on the same line as ZB2
#define NB_ML 12 // total number of menus items on the left   
#define NB_MC 10 // total number of menus items on the center 
#define EEPROM_SIZE (NB_ML * NB_MC)

byte lc =0 ;      //left choice - left pot / encoder
byte old_lc =0 ;  // old value of left choice, used to debounce 
byte     cc =0 ;  // center choice : middle pot / encoder
byte old_cc =0 ;  // old value of center choice, used to debounce
byte  mcval[NB_ML][NB_MC];  //  current value (matrix is stored in ROM with EEPROM.write)

byte cval =0; ; // used to contain current value of mcval[lc][cc] according to position of left & center encoders
byte i=0 ;  // ????????????
byte const maxccbylc[NB_ML] = {8,7,7,6,7,7,6,6,6,6,6,6} ; //how many center menus to display for each left menu ?


unsigned long controltime; //do somethinhg at regular interval in loop()
unsigned long playtime; // time to play a note from the demo sequence (if active !!)
unsigned int bpm = 220 ;  // default bpm = 30000/ periodbpmn, pourquoi pas 60 * 1000, mystère...
uint16_t  periodbpm = 30000/bpm ;  // in millisec = 30000 /bpm

# define bass 36         // MIDI note numbers for several GM drum sounds used in the demo
# define snare 38
# define hihatC 42
# define hihatP 44
# define hihatO 46
# define DRUMCHAN  9              // MIDI DRUMCHAN number 9 = drums ( = 10 in decimal )

byte tempo   = 120;       // Initial tempo
byte myKick  = 36 ;// DR_Kick;
byte mySnare = 38 ; //DR_Snare;

//int tickNo;                   // Our tick number variable used in the demo
bool demo = true ;   // play demo or not

byte patternSize = 16;  // A pattern of max 16 ticks 
     //(actualy 15, because we loop and 16 is tick no.1.. you dig it ?)
byte tickNo;            // Our tick number variable


/*// DEMO PAttern : every array cell contains the velocity of the note played 
// Tick                1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
const byte bd  [] = {127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 90,  0}; //Bassdrum
const byte sn  [] = {  0,  0,  0,  0,127,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}; //Snare
const byte hho [] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127,  0}; //Hihat Open
const byte hhc [] = {127, 40, 80, 40,127, 40, 80, 40,127, 40, 80, 40,127,  0,  0}; //Hihat Close
const byte hhp [] = {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,127}; //Hihat Pedal
*/

// DEMO PAttern : every array cell contains the velocity of the note played 
// https://cecm.indiana.edu/361/drumpatterns.html FUNKY PRESIDENT
// Tick                1    2    3    4  / 5    6   7   8  /   9    10  11  12  / 13   14   15  16
const byte bd  [] = { 100, 0  ,  0 , 100, 0  ,  0 , 0  , 100, 0  , 100, 100, 0  , 0 , 0  , 0  , 100}; //Bassdrum = mykicks
const byte sn  [] = {  0 , 0  , 0  ,  0 , 100,  0 , 0  , 0  ,  0 ,  0 , 0  ,  0 , 100,  0 , 0 , 0 }; //Snare
const byte hho [] = {  0 , 0  ,  0 ,  0 ,  0 , 0   ,  0 ,  0 ,  0 ,  0  , 127,  0 ,  0 ,  0  , 0  , 0  }; //Hihat Open
const byte hhc [] = { 127, 0 , 127, 0   , 127, 0  , 127  , 0  , 127, 0  , 0  , 0  , 127,  0 , 127, 0  }; //Hihat Close

const byte hhp [] = { 0  ,  0 ,   0,  0 , 0  ,  0 , 0  ,  0 , 0  ,  0 ,  0 ,  0 ,  0 ,  0, 0  , 0  }; //Hihat Pedal





//char const *Note[] = { "C","C#","D","D#","E","F","F#","G","G#","A","A#","B" };


char *nom0 ;  // menu left name
char *nom1;   // menu center name
char *nombt0; // name displayed above button left
char *nombt1; // name displayed above button center
char *nombt2; // name displayed above button right
byte defv ;   // default MIDI value to use
byte minv ;   // minimum value allowed for this selection
byte maxv ;   // maximum value allowed for this selection 
byte incr;    // increment value for button - + 
bool (*dispint)(); //  mcval : function used to display current value selected 


//---------------- DRUMKITS --------------------------------------------------------------------
typedef struct {
char const *pgname ;
byte pgval ;
  } tpdrums ;
 
 tpdrums pdrums[5] = {
 {"Standard",0}, {"Power",16}, {"Jazz ",40}, {"Orchestral.",48}, {"CM32/64 ",127}
 } ;

const byte dset=5 ;
//dprogram = 0; // default drum program = standard

typedef struct {
//byte row ;
//byte col ;
byte note ;
char *name ;
} t_set ;

typedef struct {
  byte l;
  byte c;
} t_lc ;

t_lc dbankidx, ingain , outgain ; //scanned during boot
t_lc trigprogidx[NBR_OF_TRIG] ; t_lc trigvolidx[NBR_OF_TRIG] ;  //scanned during boot

void hitDrum( byte drumNr, byte velocity )
{
  synth.noteOn ( DRUMCHAN, drumNr, velocity ); // play a note
  synth.noteOff( DRUMCHAN, drumNr );         // send note off
}
// --------------- OLED ROUTINES -------------------------------------------------------------------------------------------
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

const uint8_t zone[NB_ZONES][6] = { {ZC0,ZL0,ZB0},{ZC1,ZL1,ZB1},{ZC2,ZL2,ZB2},{ZC3,ZL3,ZB3},{ZC4,ZL4,ZB4},{ZC5,ZL5,ZB5}   } ;   

void printcz(uint8_t z ,const char *c)
{
 for ( i=1;i<zone[z][2];i++)
 {u8x8.draw1x2String(i+zone[z][0] , zone[z][1] ," "); 
 }
 u8x8.draw1x2String(zone[z][0]   ,zone[z][1], c); 
 }

void printcz0(const char *c)
{u8x8.draw1x2String(ZC0+1,ZL0,"      ");
 u8x8.draw1x2String(ZC0,ZL0, c); }

void printcz1(const char *c)
{u8x8.draw1x2String(ZC1+1,ZL1,"      ");
 u8x8.draw1x2String(ZC1,ZL1, c); }

void printcz2(const char *c)
{u8x8.draw1x2String(ZC6,ZL2,"                ");
 u8x8.draw1x2String(ZC2,ZL2, c); }   

void printcz3(const char *c)
{u8x8.draw1x2String(ZC3,ZL3,"     ");
 u8x8.draw1x2String(ZC3,ZL3, c); }

void printcz4(const char *c)
{u8x8.draw1x2String(ZC4,ZL4,"     ");
 u8x8.draw1x2String(ZC4,ZL4, c); }

void printcz5(const char *c)
{u8x8.draw1x2String(ZC5,ZL5,"     ");
 u8x8.draw1x2String(ZC5,ZL5, c); }

void printcz6(const char *c)
{u8x8.draw1x2String(ZC6,ZL6,"                ");
 u8x8.draw1x2String(ZC6,ZL6, c); }

 void printz0(byte var)
{u8x8.draw1x2String(ZC0+1,ZL0,"      ");
 u8x8.draw1x2String(ZC0,ZL0, String(var).c_str()); }
  
 void printz1(byte var)
{u8x8.draw1x2String(ZC1+1,ZL1,"      ");
 u8x8.draw1x2String(ZC1,ZL1, String(var).c_str()); }
 void printz2(byte var)
{u8x8.draw1x2String(ZC6,ZL2,"                ");
 u8x8.draw1x2String(ZC2,ZL2, String(var).c_str()); }
 void printz3(byte var)
{u8x8.draw1x2String(ZC3+1,ZL3,"    ");
 u8x8.draw1x2String(ZC3,ZL3, String(var).c_str()); }
 void printz4(byte var)
{u8x8.draw1x2String(ZC4+1,ZL4,"    ");
 u8x8.draw1x2String(ZC4,ZL4, String(var).c_str()); }
void printz5(byte var)
{u8x8.draw1x2String(ZC5+1,ZL5,"    ");
 u8x8.draw1x2String(ZC5,ZL5, String(var).c_str()); }
void printz6(byte var)
{u8x8.draw1x2String(ZC6+1,ZL6,"               ");
 u8x8.draw1x2String(ZC6,ZL6, String(var).c_str()); }

bool int_LCD( ) { printz2(cval);return true ; }

bool pc_LCD() { uint16_t val = (100 * cval ) >>7 ;printz2(val) ;return true ;  }

bool empty_LCD() {u8x8.draw1x2String(ZC6,ZL6,"                "); return true ;   }

bool midi_LCD( )
{  
switch(cval)
  { case  0: printcz2("OMNI");     break;
    case 17: printcz2("OMNI");     break;
    default: printz2(cval);        break;
  }return true ;  }

bool  midireset_LCD()
{  
switch(cval)
  { case 0:  printcz2(" MIDI ");  break;
    case 1:  printcz2(" GM   ");  break;
    case 2:  printcz2(" GS   ");  break;
    default: printz2(cval);        break;
  } return true ;  }

bool clock_Div[]={0, 6 , 8 , 12 , 24};

bool clock_LCD( )
{  
  switch(cval)
  {case 0:  printcz2("INT")   ;  break;
   case 1:  printcz2("EXT/6") ;  break;
   case 2:  printcz2("EXT/8") ;  break;
   case 3:  printcz2("EXT/12");  break;
   case 4:  printcz2("EXT/24");  break;
   default: printcz2("Unknow");  break;
  }  return true ;}

bool revprog_LCD()
{  
   switch(cval)
  { case 0: printcz2("Room1  ");  break;
    case 1: printcz2("Room2  ");  break;
    case 2: printcz2("Room3  ");  break;
    case 3: printcz2("Hall   ");  break;
    case 4: printcz2("Hall 2 ");  break;
    case 5: printcz2("Plate  ");  break;
    case 6: printcz2("Delay  ");  break;
    case 7: printcz2("PanDela");  break;
  } return true ;  }

bool chorprog_LCD( )
{ 
   switch(cval)
  { case 0: printcz2("Chorus1");  break;
    case 1: printcz2("Chorus2");  break;
    case 2: printcz2("Chorus3");  break;
    case 3: printcz2("Chorus4");  break;
    case 4: printcz2("FBChors");  break;
    case 5: printcz2("Flanger");  break;
    case 6: printcz2("ShrtDel");  break;
    case 7: printcz2("FBDelay");  break;
  } return true ;   }

bool onoff_LCD()
{  
  switch(cval)
  { case 0:  printcz2("OFF");    break;
    case 1:  printcz2("ON ") ;    break;
    default: printz2( cval); break;
  } return true ; }


bool clip_LCD()
{  
   switch(cval)
  { case 0:   printcz2("Soft ") ;  break;
    case 127: printcz2("Hard ");   break;
    default: printz2( cval); break;
  } return true ;   }

bool pdrums_LCD( )
{  printcz2( pdrums[mcval[lc][cc]].pgname);    
  //u8x8.draw1x2String(ZC2,ZL2, String(pdrums[mcval[lc][cc]].pgname).c_str()); 
return true ; }


bool dnote_LCD( ) // drum name based on bank and note number
{   //empty_LCD(); 
   //strcpy_P(dname, (char *)pgm_read_word(&(tabledname[cval-35]))); // first drum note in STD mode is MIDI note 35
    // during setup, the position of bank select is scanned and placed inside dbankidx l + c 
    printcz6(  drbname [-35+mcval[lc][cc]][ mcval[dbankidx.l][dbankidx.c] ]  ) ;
   return true ;
   }

bool nomic_LCD() {if (cval == 0) printcz6("RE CH       p=48") ; // Reverb + Chorus + MICRO DISABLED 
             else if (cval == 1) printcz6("RE CH EQ    p=39") ; // reverb + chorus + equalizer MICRO DISABLED 
             else if (cval == 2) printcz6("RE CH EQ SP p=38") ; return true ; } //all except micro +( echoe exclusive to micro) 

bool   mic_LCD() {if (cval == 0) printcz6("RE CH       p=44") ; // PARAMETERS WITH MICRO ENABLED : reverb + chorus
             else if (cval == 1) printcz6("RE CH EQ    p=36") ; // reverb + chorus + equalizer
             else if (cval == 2) printcz6("RE CH EQ SP p=35") ; return true ; } // all active 

bool gout_LCD() // Gain output

{  u8x8.draw1x2String(ZC2,ZL2,"    "); u8x8.draw1x2String(ZC2,ZL2, String(cval-57).c_str()); u8x8.draw1x2String(ZC2+4,ZL2,"dB ");  
//u8x8.draw1x2String(ZC2,ZL2, String((int)cval-57).c_str()); u8x8.draw1x2String(ZC2+3,ZL2," dB"); 
   return true ;
}

bool encoder_LCD() // need to save to eprom then restart to take effect
{ switch(cval)
  { case 0: printcz2("None ") ;  break;
    case 1: printcz2("Left ");   break;
    case 2: printcz2("Right");   break;
    case 3: printcz2("Both ");   break;
   default: printz2( cval);      break;
    } return true ;  
}

//---------- MIDI TEST ONLY ------------------------------------------------------------------------------
// this test uses only hardwareserial, not fluxamasynth : useful for debug MIDI 
// plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that  data values are less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial2.write(cmd);  Serial2.write(pitch);  Serial2.write(velocity);
}
 bool play()
{
for (int note = 0x1E; note < 0x5A; note ++) {
    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(0x90, note, 0x45);
    delay(100);
    //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
    noteOn(0x90, note, 0x00);
    //char[] ab = "b" ; 
    delay(100); 
  }  return true ;
}
//--------------------------------------------------------------------------------------------------------------
typedef bool (*pt2Function)(); //adress of a print function
typedef bool (*pt2Funcb)(byte); // adress of a todo function with a optional byte var (pass 0 if not required ) 

typedef const struct  {
byte  tposl ;byte tposc ;char *tnom0 ;char *tnom1 ; char  *tnombt0 ; char *tnombt1 ; char *tnombt2 ; byte tdefv ; byte tminv;
 byte tmaxv; byte tincr; pt2Function toprint; pt2Funcb todo ;
} t_set2 ;

bool midigmgs(byte b )
 { switch (cval) 
    { case 0: synth.midiReset() ; break; 
      case 1: synth.GM_Reset()  ; break; 
      case 2: synth.GS_Reset()  ; break;  } 
    return true ;} // RETURN immediatly 0,1 or 2 instead of small increments between 0 and 2

bool setdemo(byte b) {
   if ( cval ==1)  demo = true ; else  demo = false;  
   return true; 
      } // 0 0

bool setencoder(byte b) { // invert encoder's rotation,  0 = default no invertion 1=left , 2=right, 3=both
      return true; 
      } // 0 6

bool setdbank(byte b) { // 0=std 1=power,... 1 Drum bank  change
 //dprogram = mcval[lc][cc] ; return true ;
//printz0(mcval[lc][cc]) ; printz1(pdrums[ mcval[lc][cc]  ].pgval ) ;
synth.drumset(pdrums[ cval ].pgval) ;
//synth.programChange( DRUMCHAN, pdrums[ mcval[lc][cc]  ].pgval );
 // Serial2.write(0xc9);Serial2.write(byte(48));
return true ;
}

bool setperiodbpm(byte b) // 0.2 
{ 
  periodbpm = 30000/(unsigned int) (1+cval); return true ; 
  //periodbpm = constrain (periodbpm, 30000/ (t23[lc][cc].tminv) ,30000/t23[lc][cc].tmaxv) ;
//printz2(periodbpm) ; delay(5000) ;
} //avoid divide by zero 

typedef struct {
  uint8_t progidx;
  uint8_t volidx ; 
  uint8_t panidx ; 
  uint8_t reverbidx ; 
  uint8_t chorusidx ;
  uint8_t pitchidx ;
} trig_t; // all param configurable for each trigg drum

trig_t triginput[NBR_OF_TRIG] ; // trig values are stored in this vector during startup scan

//========================== EEPROM READ WRITE ======================================================================
void loadfromeprom()
{   
  int adr=0; printcz2("READING"); delay(3000) ; u8x8_ClearDisplay ;
   for (int i=0;i<NB_ML; i++)
       {  for (int j=0; j< maxccbylc[i]; j++)
          {mcval[i][j] =  byte(EEPROM.read(adr)) ;
      //printz0(i); printz1(j) ;  printz2( byte(EEPROM.read(adr))) ; printcz3("load") ; delay(3000) ;
          adr++;  
          u8x8_ClearDisplay ;
          } 
       }
  //u8x8.clear() ; printcz0("ROM_0:"); printz2(EEPROM.read(0)) ;delay(3000) ; u8x8.clear() ;
  //u8x8.clear() ; printcz0("ROM_1:"); printz2(EEPROM.read(1)) ;delay(3000) ; u8x8.clear() ;
  //u8x8.clear() ; printcz0("mcval00:"); printz2(mcval[0][0]) ;delay(3000)  ; u8x8.clear() ;
  //Serial1.end(true) ; 
}          

void savetoeprom()
{  int adw=0; printcz2("SAVING.");
   for (int i=0;i<NB_ML; i++)
      { for (int j=0; j< maxccbylc[i]; j++)
       { EEPROM.write(adw,mcval[i][j]);   
     // if (i==0) {u8x8.clear(); printz0(i); printz1(j) ; printz2(mcval[i][j]); printz3(adw) ; delay(3000) ; }
      adw++;
       } 
      }  
    EEPROM.commit();
   printcz2("SAVED..");
 }

 bool eprom (byte v) { // eprom function disabled in load_synth_and_trig during setup to avoid saving or loading on each boot
  if ( (v == 1) && launcheprom) savetoeprom(); else if ((v ==0) && launcheprom) loadfromeprom()  ; return true ; }

// ============================ AUDIO IN OUT ========================================================================================
// Hardware config measured when Vout is max before clipping  (values are peak to peak)
//   MIC IN   Volume_in   GAIN_IN (6 bits )   GAIN_OUT (6 bits)    Vout 
//   600mv     max        27(default)         57(default)           2V
//   3V1       max         6                  57(default)           1V
//   3V1       max         6                  63(=max)              2V 
//   3V1       max         8                  63(=max)              3V1
//======================== AUDIO IN ===========================================================================================
bool nomic(byte b) { // OPTIONS WITH NO AUDIO MIC 
 {if     (cval == 0) synth.nomicrevchor(); 
 else if (cval == 1) synth.nomiceq() ; 
 else                synth.nomiceqsp()  ;} 
 return true ; } // selection of 0,1 or 2 instead of default - + is managed in the loop when only 3 possible choices (maxv = 2)

bool  mic(byte b) { // OPTIONS WITH MICRO AUDIO IN ACTIVE 
 {if     (cval == 0) synth.micrevchor(); 
 else if (cval == 1) synth.miceq() ; 
 else                synth.miceqsp()  ;} 
  return true ; } // selection of 0,1 or 2 instead of default - + is managed in the loop when only 3 possible choices (maxv = 2)

bool setmicvol(byte b) { synth.micvol(b) ; return true ;}  // AudioIN MIC Volume 

bool setingain(byte b)  { synth.setgain( cval,  mcval[outgain.l][outgain.c] ) ;
printz0(HI_NIB(cval)) ;
printz1(LO_NIB(cval)) ;
printz6(cval) ;
printz3(HI_NIB(mcval[outgain.l][outgain.c]))  ;
 printz4(LO_NIB(mcval[outgain.l][outgain.c])) ;
 ; return true;  } // set MIC input GAIN (high byte)   ) 

//==================== AUDIO OUT ================================================================================================
bool setoutgain(byte b) { synth.setgain( mcval[ingain.l][ingain.c], cval  ) ; 
byte  highr = mcval[ingain.l][ingain.c] & 0b00111111 ; //ADCMUTE = 0 INMUTE = 0
byte  lowr =  cval  & 0b01111111 ; //DACMUTE = 0 DACSEL = 1
 
 byte vv3 = (HI_NIB(highr))>>4 ;
 byte vv2 = LO_NIB(highr) ;
 byte vv1 = (HI_NIB(lowr))>>4 ;
 byte vv0 = LO_NIB(lowr) ;

//printz0(vv3) ;printz1(vv2) ;printz6(cval) ;printz3(vv1)  ;printz4(vv0) ;
return true;  } // SET audio OUT GAIN (low byte) 
//----------------------------------------------------------
// Redefinitions of methods in order to call them via their adress, not allowed with instantiated classes
bool setmastervol(byte b) { synth.setMasterVolume(b)  ;  return true ;  }
bool setgsvol(byte b )    { synth.GS_MasterVolume(b)  ; return true ;  }
bool setmasterpan(byte b) { synth.MasterPan(b)        ; return true ;  }
bool setclip(byte b)      { synth.setClippingMode(b)  ; return true ;  }

bool goback(byte b)       { if (cc >0) cc-1; else cc=0; return true ;  } // oups... went too far 

bool setchorsend(byte b)  { synth.setChorusSend(DRUMCHAN,b)  ; return true ;  } 
bool setchorlevel(byte b) { synth.setChorusLevel(b)      ; return true ;  }
bool setchorprog(byte b)  { synth.setChorusProgram(b)    ; return true ;  }
bool setchordelay(byte b) { synth.setChorusDelay(b)      ; return true ;  }
bool setchorfeed(byte b)  { synth.setChorusFeedback(b)   ; return true ;  }  
bool setchorrate(byte b)  { synth.setChorusRate(b)       ; return true ;  }  
bool setchordepth(byte b) { synth.setChorusDepth(b)      ; return true ;  }  

bool setrevsend(byte b)  { synth.setReverbSend(DRUMCHAN,b)  ; return true ;  } 
bool setrevlev(byte b)   { synth.setReverbLevel(b)      ; return true ;  }
bool setrevprog(byte b)  { synth.setReverbProgram(b)    ; return true ;  }
bool setrevtime(byte b)  { synth.setReverbTime(b)       ; return true ;  }
bool setrevfeed(byte b)  { synth.setReverbFeedback(b)   ; return true ;  }  
bool setrevchar(byte b)  { synth.setReverbCharacter(b)  ; return true ;  }  

bool setenvat(byte b)    { synth.setEnvAttack(DRUMCHAN,b)  ; return true ;  } 
bool setenvdec(byte b)   { synth.setEnvDecay(DRUMCHAN,b)   ; return true ;  } 
bool setenvrel(byte b)   { synth.setEnvRelease(DRUMCHAN,b) ; return true ;  } 
bool setlforate(byte b)  { synth.setLfoRate(b)             ; return true ;  } 
bool setsurrvol(byte b)  { synth.setSurroundVolume(b)     ; return true ;  } 
bool setsurrdel(byte b)  { synth.setSurroundDelay(b)      ; return true ;  } 
bool setsurrmono(byte b) { synth.surroundMonoIn(b)        ; return true ;  } 

bool setvibrate(byte b)   { synth.setVibratoRate(DRUMCHAN,b)  ; return true ;  }
bool setvibdepth(byte b)  { synth.setVibratoDepth(DRUMCHAN,b) ; return true ;  }
bool setvibdelay(byte b)  { synth.setVibratoDelay(DRUMCHAN,b) ; return true ;  }
bool settvfcut(byte b)    { synth.setTvfCutoff(DRUMCHAN,b)    ; return true ;  }
bool settvfres(byte b)    { synth.setTvfResonance(DRUMCHAN,b) ; return true ;  }
bool setpostGM(byte b)    { synth.postprocGeneralMidi(b)      ; return true ;  }
bool setpostrevch(byte b) { synth.postprocReverbChorus(b)     ; return true ;  }

// t23[lc][cc].tposc contains the trig number 0.. NBR_OF_TRIG
bool trigprog(byte b)  ; // see below for the definition 
bool trigvol(byte b)  ;
bool trigpan(byte b)   ;
bool trigreverb(byte b) ;
bool trigchor(byte b)   ;
bool trigpitch(byte b)  ;



//// UPDATE MAXCCBYLC IF YOU ADD A ROW OR COLUMN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//--------- MOST PARAMETERS TO USE & DISPLAY AND FUNCTIONS TO CALL ARE STORED HERE ---------------------------  
// Some labels like "DBank" are scanned during setup in order to locate their position and find their value in mcval[][]
//So, don't mess with their title !!!!!!!!!!!!!!!!
 t_set2 const  t23[NB_ML][NB_MC] =
 { { {0,0,"Sequenc"," DEMO  "," OFF ","     "," ON  ",0  ,0  ,1  ,1  ,&onoff_LCD  , &setdemo}, // default , min , max , increment, print , exec
     {0,1,"DBank  ","Change ","stand","  -  "," +   ",0  ,0  ,4  ,1  ,&pdrums_LCD , &setdbank},
     {0,2,"Sequenc"," BPM   ","     ","  -  "," +   ",120,60 ,255,1  ,&int_LCD    , &setperiodbpm}, 
     {0,3,"AudioIN"," OFF   "," /   "," EQ  ","EQ+SP",0  ,0  ,2  ,1  ,&nomic_LCD  , &nomic},
     {0,4,"AudioIN"," ON    "," /   "," EQ  ","EQ+SP",0  ,0  ,2  ,1  ,&mic_LCD    , &mic},           //8
     {0,5,"AudioIN","Volume "," 64  ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD     , &setmicvol},
     {0,6,"AudioIN"," Gain  "," 0db ","  -  "," +   ",27 ,0  ,63 ,1  ,&pc_LCD     , &setingain},
     {0,7,"Aud OUT"," Gain  "," 0db ","  -  "," +   ",57 ,17 ,63 ,1  ,&gout_LCD   , &setoutgain} },
     //----------------------- GLOBAL MAIN SETTINGS -------------------------------------
   { {1,0,"Global "," RESET ","MIDI "," GM  "," GS  ",0  ,0  ,2  ,1  ,&midireset_LCD , &midigmgs},
     {1,1,"Global "," MVOL  "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD        , &setmastervol},
     {1,2,"Global "," GSVOL "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD        , &setgsvol},
     {1,3,"Global "," PAN   "," med ","  -  "," +   ",60 ,0  ,127,1  ,&pc_LCD        , &setmasterpan},
     {1,4,"Global "," EPROM ","Load ","     ","Save ",0  ,0  ,1  ,1  ,&empty_LCD     , &eprom},
     {1,5,"Global "," CLIP  ","Soft ","     ","Hard ",0  ,0  ,127,127,&clip_LCD      , &setclip},           //7
     {1,6,"Encoder","Invert?","None ","  -  "," +   ",0  ,0  ,3  ,1  ,&encoder_LCD   , &setencoder} },          
     //-------------------------GLOBAL CHORUS -----------------------------------------------
   { {2,0,"Chorus "," Send  "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchorsend},
     {2,1,"Chorus "," Level "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchorlevel},
     {2,2,"Chorus "," Prog  ","Chor3","  -  "," +   ",2  ,0  ,7  ,1  ,&chorprog_LCD , &setchorprog},
     {2,3,"Chorus "," Delay "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchordelay},
     {2,4,"Chorus ","Feedbkc"," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchorfeed},
     {2,5,"Chorus "," Rate  "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchorrate},
     {2,6,"Chorus "," Depth "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD       , &setchordepth} },    //7
     //------------------------ GLOBAL REVERB ----------------------------------------------
    {{3,0,"Reverb "," Send  "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD      , &setrevsend},
     {3,1,"Reverb "," Level "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD      , &setrevlev},
     {3,2,"Reverb "," Prog  ","Hall2","  -  "," +   ",4  ,0  ,7  ,1  ,&revprog_LCD , &setrevprog},
     {3,3,"Reverb "," Time  "," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD      , &setrevtime},
     {3,4,"Reverb ","Feedbck"," min ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD      , &setrevfeed},
     {3,5,"Reverb "," Char  "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD      , &setrevchar},         //6
     {3,6,"Reverb ",".......",".....",".....",".....",4  ,0  ,127,10 ,&pc_LCD      , &setrevchar} },        
     //-------------------- ADR ----------------------------------------------------------
    {{4,0,"  ADR  ","Attack "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setenvat},
     {4,1,"  ADR  ","Decay  "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setenvdec},
     {4,2,"  ADR  ","Release"," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setenvrel},
     {4,3,"  LFO  "," Rate  "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD    , &setlforate},
     {4,4,"Suround","Volume "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD    , &setsurrvol},
     {4,5,"Suround"," Delay "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD    , &setsurrdel},
     {4,6,"Suround"," Mono  "," OFF ","     "," ON  ",0  ,0  ,1  ,1  ,&onoff_LCD , &setsurrmono}, },       //7
      //-------------- LFO FILTER --------------------------------------------------------
    {{5,0,"Vibrato"," Rate  "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setvibrate},
     {5,1,"Vibrato"," Depth "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setvibdepth},
     {5,2,"Vibrato"," Delay "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &setvibdelay},
     {5,3,"Filter "," Cut   "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &settvfcut},
     {5,4,"Filter "," Res   "," def ","  -  "," +   ",64 ,0  ,127,10 ,&pc_LCD    , &settvfres},
     {5,5,"PostPro","  GM   "," OFF ","     "," ON  ",0  ,0  ,1  ,1  ,&onoff_LCD , &setpostGM},                    //87767766666
     {5,6,"PostPro","Re/Ch  "," OFF ","     "," ON  ",0  ,0  ,1  ,1  ,&onoff_LCD , &setpostrevch}, },        //7
     // ----------------------- DRUM TRIGGER PROGRAMS ( NOTE) ----- -----------------------------
    {{6,0,"Dprog  ","Trig 1 ","     ","  -  "," +   ",35 ,35 ,81 ,1  ,&dnote_LCD , &trigprog}, // drum note between 35 & 81 
     {6,1,"Dprog  ","Trig 2 ","     ","  -  "," +   ",36 ,35 ,81 ,1  ,&dnote_LCD , &trigprog},
     {6,2,"Dprog  ","Trig 3 ","     ","  -  "," +   ",37 ,35 ,81 ,1  ,&dnote_LCD , &trigprog},
     {6,3,"Dprog  ","Trig 4 ","     ","  -  "," +   ",38 ,35 ,81 ,1  ,&dnote_LCD , &trigprog},
     {6,4,"Dprog  ","Trig 5 ","     ","  -  "," +   ",39 ,35 ,81 ,1  ,&dnote_LCD , &trigprog},
     {6,5,"Dprog  ","Trig 6 ","     ","  -  "," +   ",40 ,35 ,81 ,1  ,&dnote_LCD , &trigprog},            //6
     {6,6,"Dprog  ","Trig 7 ",".....","....."," ON  ",41 ,35 ,81 ,1  ,&dnote_LCD , &trigprog}, },          
     // ----------------------- DRUM TRIGGER VOLUME --------------------------------------
    {{7,0,"DrumVol","Trig 1 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},  
     {7,1,"DrumVol","Trig 2 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},
     {7,2,"DrumVol","Trig 3 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},   // max vol = 127
     {7,3,"DrumVol","Trig 4 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},
     {7,4,"DrumVol","Trig 5 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},
     {7,5,"DrumVol","Trig 6 "," max ","  -  "," +   ",127,0  ,127,10 ,&pc_LCD , &trigvol},            // 6
     {7,6,"DrumVol","Trig 7 ",".....",".....",".....",127,0  ,127,10 ,&pc_LCD , &trigvol}, },          
    // ----------------------- DRUM TRIGGER PAN -----------------------------------------
   { {8,0,"DrumPan","Trig 1 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},  
     {8,1,"DrumPan","Trig 2 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},
     {8,2,"DrumPan","Trig 3 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},
     {8,3,"DrumPan","Trig 4 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},   // middle = 64 
     {8,4,"DrumPan","Trig 5 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},
     {8,5,"DrumPan","Trig 6 "," Med ","  -  "," +   ",64 ,0  ,127,5  ,&pc_LCD   , &trigpan},          //6
     {8,6,"DrumPan","Trig 7 ",".....",".....",".....",64 ,0  ,127,5  ,&pc_LCD   , &trigpan}, },           
    // --------------------- DRUM TRIGG REVERB -------------------------------------------
    {{9,0,"DReverb","Trig 1 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},  
     {9,1,"DReverb","Trig 2 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},
     {9,2,"DReverb","Trig 3 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},
     {9,3,"DReverb","Trig 4 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},
     {9,4,"DReverb","Trig 5 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},
     {9,5,"DReverb","Trig 6 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb},         //6
     {9,6,"DReverb","Trig 7 ",".....",".....",".....",0  ,0  ,127,10 ,&pc_LCD   , &trigreverb}, },
    //----------------------- DRUM TRIGGER CHORUS ------------------------------------------
   {{10,0,"DChorus","Trig 1 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},  
    {10,1,"DChorus","Trig 2 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},  
    {10,2,"DChorus","Trig 3 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},  
    {10,3,"DChorus","Trig 4 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},  
    {10,4,"DChorus","Trig 5 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},  
    {10,5,"DChorus","Trig 6 "," 0   ","  -  "," +   ",0  ,0  ,127,10 ,&pc_LCD , &trigchor},          //6
    {10,6,"DChorus","Trig 7 ",".....",".....",".....",0  ,0  ,127,10 ,&pc_LCD , &trigchor},   },
    //---------------------- DRUM TRIGGER PITCH ---------------------------------------------
   {{11,0,"DPitch ","Trig 1 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch},  
    {11,1,"DPitch ","Trig 2 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch}, // 64 = no change
    {11,2,"DPitch ","Trig 3 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch}, // FUN > 70  !!
    {11,3,"DPitch ","Trig 4 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch},
    {11,4,"DPitch ","Trig 5 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch},
    {11,5,"DPitch ","Trig 6 "," def ","  -  "," +   ",64 ,0  ,127,1  ,&pc_LCD , &trigpitch},              //6
    {11,6,"DPitch ","Trig 7 ",".....",".....",".....",64 ,0  ,127,1  ,&pc_LCD , &trigpitch}, }
    } ; 


void load_synth_and_trig() 
// LOAD FROM EEPROM FIRST !!!!!// What this function does : 
// 1) Check if all stored values (in mcval) are between  tminv <==> tmaxv. If not, replace with default tdefv in RAM, savetoeprom, ask to restart
// This will occur generally after first start of the module when no selection have been choosen yet by the user and the eeprom is empty.
// 2) execute all functions stored in t23.todo with param from corresponding mcval in order to update the synth and other params like tempo.
// this is excatly similar to what is done when the user pushs one of the 3 selection switchs (same function call behind)
// 3) Find index (position in the t23 structure) for some critical values related to drumbank and drum trigger for later use..
// Don't touch the name stored in t23.[][]tnom0 if you intend to scan it !!!!!!!!!! This is not a relational database : need to match the whole name with fullscan.
// Why is 3) required ? When you are on a specific menu, and the function to call depends on valus stored on another menu
// Example : try to adjust the volume or pan of trig[6] if you don't know the drumkit and drum program selected for trig[6]!!
// After initial scan, you know that Drumbank is in mcval[dbankidx.l][dbankidx.c]
// and that drum program for trig6 is in            mcval[trigprogidx[5].l][trigprogidx[5].c]
{
   bool foundbank = false ; bool foundrotation = false; bool foundgain=false; bool needreload = false; int foundprog=0 ; int foundvol=0 ;
   launcheprom = false ;  // disable the eprom function in this initialization loop (but savetoeprom is allowed if required below )
   // the eprom function should normally be started only manually in the loop
   for (int i=0;i<NB_ML; i++)
        {  
        for (int j=0; j<maxccbylc[i]; j++)
        // example : 6,0,"Dprog  ",  "Trig 1 " t23[i][j].nom0 ="DProg  "   t23[i][j].tposl = 6 , t23[i][j].tposc = 0 => triginput[0].progidx=6            
        {    // where did I store this value again ?????? in position one or six ?????????
        //printz0(i); printz1(j) ; printz2(mcval[i][j]) ; 
        //1)check if some values out of ranges --------------------------------------------------------------------
        cval = mcval[i][j] ;
        if ( (cval >=t23[i][j].tminv) and (cval <=  t23[i][j].tmaxv)  ) // if RAM value inside boundaries  // (remember to load EPROM => RAM first !!! )
        { t23[i][j].todo(cval); }          // then execute corresponding function "todo" stored in the EPROM matrix 
        else { mcval[i][j] = t23[i][j].tdefv ; // otherwise replace wrong value by default value, save to EPROM and ask to restart  
                u8x8.clear();   printz0(i); printz1(j); printz4(mcval[i][j]); printcz6("Val out of range"); delay(100);
               needreload = true;
              delay(1000); 
              }
        // 2) scan to find some critical values  
        if      ( t23[i][j].tnom0 == "DBank  ")                                   { dbankidx.l       = i ; dbankidx.c       = j ; foundbank = true ; } 
        else if ( t23[i][j].tnom0 == "Encoder")                                   { encrotation      =     mcval[i][j]          ; foundrotation = true; } 
        else if ( t23[i][j].tnom0 == "Dprog  ")                                   { trigprogidx[j].l = i ; trigprogidx[j].c = j ; foundprog++      ; } 
        else if ( t23[i][j].tnom0 == "DrumVol")                                   { trigvolidx[j].l  = i ; trigvolidx[j].c  = j ; foundvol++       ; } 
        else if ( t23[i][j].tnom0 == "AudioIN" and t23[i][j].tnom1 == " Gain  " ) { ingain.l         = i ; ingain.c         = j ; foundgain = true ; } 
        else if ( t23[i][j].tnom0 == "Aud OUT" and t23[i][j].tnom1 == " Gain  " ) { outgain.l        = i ; outgain.c        = j ; foundgain = true ; } 
        }
       } 
   if ( not foundbank  )           {                printcz6("Dbank not found ")                     ; delay(5000); } // location of drumbank not found 
   if ( not foundrotation )        {                printcz6("Enc. not found  ")                     ; delay(5000); } // location of drumbank not found 
   if ( foundprog != NBR_OF_TRIG ) {                printcz6("Nbr of DPROG <> ");printz4(NBR_OF_TRIG); delay(5000); } // nbr of trigger values found is incorrect 
   if ( foundvol  != NBR_OF_TRIG ) {                printcz6("Nbr of DVOL  <> ");printz4(NBR_OF_TRIG); delay(5000); } // nbr of trigger values found is incorrect 
   if ( not foundgain  )           {                printcz6("Gain not found  ")                   ; delay(5000); } // location of drumbank not found 
   if ( needreload)                { savetoeprom(); printcz6("Need to restart ")  ; delay(5000); } // save new default values 
   launcheprom = true ;// authorize again eprom function 
  }
  
// execute function with *t23[0][1].toprint ou todo or  *t23[0][1].todo(12);  if todo call has ALWAYS a parameter : http://www.newty.de/fpt/fpt.html

//========= TRIG RELATED FUNCTIONS =======================================================================
void playtrig(byte idx)  // 0..(NBR_OF_TRIG-1)
{     
byte prog =    mcval[trigprogidx[idx].l][trigprogidx[idx].c] ;
//byte prog =  mcval[ triginput[idx].progidx][idx] ; // ze drum program for thiz trigg
byte vol =     mcval[trigvolidx[idx].l][trigvolidx[idx].c] ;
//byte vol =   mcval[ triginput[idx].volidx ][idx] ; // ze volume  of ze trig presently pressed hard
hitDrum( prog, vol );                   // envoie la sauce pèpère ...    
}

bool trigprog(byte b) // param is always mcval[lc][cc] for all functions call from loop, but we need idx = cc here
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ;                   hitDrum( pnr, 0x7F ); return true ;  }

bool trigvol(byte b) // param is always mcval[lc][cc] for all functions call from loop, but we need prognr and idx = cc here
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ; synth.setDrumVol(DRUMCHAN, pnr, b ) ; hitDrum( pnr, b ); return true ;  }
//synth.setDrumVol (DRUMCHAN, dnote[i], mcval[7][i] ) ;//hitDrum( byte dnote[i], byte 127 )// drum note , drum volume 

bool trigpan(byte b)  // param is always mcval[lc][cc] for all functions call from loop, but we need prognr and idx = cc here  
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ; synth.setDrumPan(DRUMCHAN, pnr, b ) ; hitDrum( pnr, 0x7F); return true ;  }

bool trigreverb(byte b) // param is always mcval[lc][cc] for all functions call from loop, but we need prognr and idx = cc here
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ; synth.setDrumReverb(DRUMCHAN, pnr, b ) ; hitDrum( pnr, 0x7F); return true ;  }

bool trigchor(byte b) // param is always mcval[lc][cc] for all functions call from loop, but we need prognr and idx = cc here
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ; synth.setDrumChorus(DRUMCHAN, pnr, b ) ; hitDrum( pnr, 0x7F); return true ;  }

bool trigpitch(byte b) // param is always mcval[lc][cc] for all functions call from loop, but we need prognr and idx = cc here
 { byte pnr = mcval[trigprogidx[cc].l][trigprogidx[cc].c] ;  synth.setDrumPitch(DRUMCHAN, pnr, b ) ; hitDrum( pnr, 0x7F); return true ;  }

//------ END TRIG RELATED FUNCTIONS -----------------------------------------------------------------------------


void readpots()
{
// ----------- READ both encoder movements and increase / decrease lc & cc according to their direction (not their counter)
 enc1 = encoder1.getCount() ;
 if        (enc1 <0 && lc>0 )       {lc-- ; cc = constrain(cc, 0, -1+maxccbylc[lc]) ;  } 
 else { if (enc1 >0 && lc<NB_ML-1 ) {lc++ ; cc = constrain(cc, 0, -1+maxccbylc[lc]) ;  }
       }
       encoder1.setCount(0);

/* // TRUTH TABLE FOR THE LEFT ENCODER /////////////////////////////////////////////////////////
enc lc
 0   x        do nothing
 -   0        lc=0 then do nothing
 -   >0       lc--  and  reset counter 
 +   <NB_ML-1 lc++  and reset counter 
 +   NB_ML -1  lc=NB_ML_1 do nothing 
*/ // 

/* // TRUTH TABLE FOR THE CENTER ENCODER /////////////////////////////////////////////////////////
enc cc
 0   x                  do nothing
 -   0                  do nothing
 -   >0                 cc-- then reset counter 
 +   <-1+maxccbylc[lc]  cc++ then reset counter 
 +   =-1+maxccbylc[lc]  do nothing 
*/ // 
enc2 = encoder2.getCount() ;
 if   (      enc2 <0 && cc>0 )                {cc-- ;  } 
 else { if ( (enc2 >0) && (cc < -1+maxccbylc[lc]) ) {cc++ ;  }
       }
encoder2.setCount(0);
//printz3(enc1); printz2(enc2); printz4(lc) ; printz5(cc) ; 

};   

void onscreen(byte lft, byte rght) //fetch & display all menu zones on screen + formatted variable
 { 

//1) FETCH DATA from the T23 MATRIX ////////////////
nom0    = t23[lc][cc].tnom0 ;   //upper left menu label
nom1    = t23[lc][cc].tnom1 ;   // upper right menu label
nombt0  = t23[lc][cc].tnombt0 ; //lower left switch label
nombt1  = t23[lc][cc].tnombt1 ; //lower mid switch label
nombt2  = t23[lc][cc].tnombt2 ; // lower switch right label
defv    = t23[lc][cc].tdefv ;   // default value (displayed on the lower left)
minv    = t23[lc][cc].tminv ;   // min value allowed
maxv    = t23[lc][cc].tmaxv ;   // max value allowed 
incr    = t23[lc][cc].tincr ;   // increment value used when you click on middle or right selection switchs
dispint = t23[lc][cc].toprint ; // pointer to a print function called when you click on a switch

//2) DISPLAY ALL CORRESPONDING INFO ON SCREEN  //
//u8x8.clear();
printcz0(nom0) ;           
printcz1(nom1) ; 
dispint();  // execute the corresponding xxx_LCD display function on zone 2
if (nombt0 == "     " ) printz3(defv ) ;  else printcz3( nombt0) ; // display specific action or default value
printcz4( nombt1) ;
printcz5( nombt2) ;

};

//===========================================================================================================
 //----------------------SETUP ----------------------------------------------------------------------------
void setup(void)
{
//----------------INIT 3 SELECTION SWITCHS ---------------------------------------------------------------
 pinMode(B0, INPUT); pinMode(B1, INPUT); pinMode(B2, INPUT); 

//------------ INIT GRAPHIC OLED -----------------------------------------------------------------------------
 u8x8.begin();
 //u8x8.setBusClock(1000000); // to increase refresh of screen if necessary , not shure all OLED screens support that
 u8x8.setFont(u8x8_font_amstrad_cpc_extended_r); //standard non proportional basic font for 16 char in one line
//u8x8.setFont(u8x8_font_courB18_2x3_f  );
// u8x8.setCursor(0, 0);

//-------------INIT MIDI DRUM LOOP PARAMETERS ----------------------------------------------------------------------
Serial2.begin(31250) ; // MIDI baud rate 8N1 : no start bit 8 bits data one stop bit 

 //Synths params used by the demo sequence  
 synth.midiReset(); // Do a complete MIDI reset
 synth.GS_Reset();  // Enable GS mode          
 synth.programChange(DRUMS_Std, DRUM_CHAN, DR_Snare2); // Drums_std = 0, Drum_chan = 9 , Dr_snares = 40 sur drums standards
 synth.programChange(0, 1, 0); //0: default (127 : MT32) , canal 1, instrument 0 = grand piano
 synth.setChannelVolume( DRUMCHAN, 127 ); // max DRUMCHAN volume
 synth.setReverbSend( DRUMCHAN, 100 );
 //synth.setDrumMix( DRUMCHAN,DrumNr,Level,Pan,Reverb,Chorus )
 synth.setDrumMix( DRUMCHAN, myKick,         120, 0x40   , 80,  0   ); // 0x40 = CTV_CENTER
 synth.setDrumMix( DRUMCHAN, mySnare,        110, 0x40-10, 72,  127 );
 synth.setDrumMix( DRUMCHAN, DR_ClosedHiHat, 120, 0x40+10, 110, 0  );
 synth.setDrumMix( DRUMCHAN, DR_OpenHiHat,   115, 0x40-10, 110, 0  );
 synth.setDrumMix( DRUMCHAN, DR_PedalHiHat,  125, 0x40+10, 110, 0  );

//-------1)EEPROM ==> RAM 2) RAM ==> params  & init SYNTH ---UPLOAD FLASH DATA into RAM mcval, then retrieve index of key variables and init synth-------------------
if (!EEPROM.begin(EEPROM_SIZE))   { u8x8.draw1x2String(ZC1, ZL1,"failed to initialise EEPROM"); delay(1000000);   }
   //savetoeprom();
   loadfromeprom();
   u8x8.clear();

load_synth_and_trig() ; // place this AFTER loading fLASH ==> RAM : execute all todo functions and load param stored on other lines than the current one (lc cc)

//--------------- INIT 2 ENCODERS --------------------------------------------------------------------------
	ESP32Encoder::useInternalWeakPullResistors=UP; //or =DOWN
  // encrotation value is retrieved during setup : function load_synth_and_trig
    switch (encrotation) 
    { case 0: encoder1.attachSingleEdge(ENC1A, ENC1B); encoder2.attachSingleEdge(ENC2A, ENC2B) ; break; // default behavior, no invertion
      case 1: encoder1.attachSingleEdge(ENC1B, ENC1A); encoder2.attachSingleEdge(ENC2A, ENC2B) ; break; // invert left encoder 
      case 2: encoder1.attachSingleEdge(ENC1A, ENC1B); encoder2.attachSingleEdge(ENC2B, ENC2A) ; break; // invert right encoder 
      case 3: encoder1.attachSingleEdge(ENC1B, ENC1A); encoder2.attachSingleEdge(ENC2B, ENC2A) ; break; // invert both
    default : encoder1.attachSingleEdge(ENC1A, ENC1B); encoder2.attachSingleEdge(ENC2A, ENC2B) ; break; // default behavior, no invertion
    }
  encoder1.clearCount();                   encoder2.clearCount();
  encoder1.setFilter(1023);                encoder2.setFilter(1023);
	encoder1.setCount(0);                    encoder2.setCount(0); // set starting count value after attaching
// we only use the direction of the count (<0 or >0) not the count value itself 

// ------------------ play 10 random notes with the fluxamasynth library  --------------------------------------------------------------
for (int i=0; i<10; i++  )
    {int note = int(random(36,100));
        synth.noteOn(0x90, note, 127);
        delay(100);
        synth.noteOff(0x90, note);
    }

//--------------------------------------------
  lc =0 ; cc=0 ; //leftchoice central_choice corresponding to the first menu to display
  cval = mcval[0][0];
  //u8x8.clear(); printcz0("cval"); printz2(cval) ; delay(4000) ; u8x8.clear();
  onscreen(0,0) ;   //fetch FLASH data and display INIT menu zones + formatted variable
  controltime = 0; //=millis(); used to check time elpased in the main loop
  playtime = 0; //=millis(); used to check time elapsed for the demo sequencer 
  
  //--------------- INIT TRIGGERS -------------------------------------------------------
  pinMode(TRIG1, INPUT_PULLDOWN);
  pinMode(TRIG2, INPUT_PULLDOWN);
  pinMode(TRIG3, INPUT_PULLDOWN);
  pinMode(TRIG4, INPUT_PULLDOWN);
  pinMode(TRIG5, INPUT_PULLDOWN);
  pinMode(TRIG6, INPUT_PULLDOWN);

  button1.setDebounceTime(DEBOUNCE_TIME); // set debounce time to xx milliseconds
  button2.setDebounceTime(DEBOUNCE_TIME); 
  button3.setDebounceTime(DEBOUNCE_TIME); 
  button4.setDebounceTime(DEBOUNCE_TIME); 
  button5.setDebounceTime(DEBOUNCE_TIME); 
  button6.setDebounceTime(DEBOUNCE_TIME); 

for (int i=0;i<NBR_OF_TRIG;i++) {   trigpush[i]=0 ;  } ; // reset history of trig switchs pushed
 

}//-------------------------- END SETUP--------------------------------------------------------------------------
//===============================================================================================================

void loop() //=========================L O O P ==================================================================
{//
 button1.loop(); // MUST call the loop() function first
 button2.loop(); // MUST call the loop() function first
 button3.loop(); // MUST call the loop() function first
 button4.loop(); // MUST call the loop() function first
 button5.loop(); // MUST call the loop() function first
 button6.loop(); // MUST call the loop() function first


 if (button1.isReleased()) { playtrig(0); }
 if (button2.isReleased()) { playtrig(1); }
 if (button3.isReleased()) { playtrig(2); }
 if (button4.isReleased()) { playtrig(3); }
 if (button5.isReleased()) { playtrig(4); }
 if (button6.isReleased()) { playtrig(5); }

if ( (millis() - playtime) > periodbpm ) // 1/0.1 sec = 10 hz ???periodbpm )  // 150  periodbpm = 30000 / bpm 
{
  playtime = millis();
 
 if (demo)
 {   // play demokit
     hitDrum( myKick, bd[tickNo] );
     hitDrum( mySnare, sn[tickNo] );
     hitDrum( DR_ClosedHiHat, hhc[tickNo] );
     hitDrum( DR_OpenHiHat, hho[tickNo] );
     hitDrum( DR_PedalHiHat, hhp[tickNo] );
    /* play mykit based on trigdrum1 => trigdrum4 instead of the demo drums
     hitDrum( mcval[6][0], mcval[7][0] );
     hitDrum( mcval[6][1], mcval[7][1] );
     hitDrum( mcval[6][2], mcval[7][2] );
     hitDrum( mcval[6][3], mcval[7][3] );
     */
   if (tickNo < patternSize)
   {tickNo++;} // next beat
   else {tickNo =0;}
}// end if demo
} // end if millis

if   ((millis() - controltime) > 100  )    // x ms elapsed ?
{  controltime = millis();

// =============== DETECT LONG PUSH ON ANY TRIGGER SWITCH ==========================================================================================

for (int i=0;i<NBR_OF_TRIG;i++)  // Detect long push on any trigger switch => display corresponding trig menu
{
  trigread = digitalRead(tin[i]) ;
 if (trigread !=0) 
    { 
     if (trigpush[i] == 9) {trigpush[i]=0; lc = trigprogidx[i].l; old_lc = lc ;cc = trigprogidx[i].c ;old_cc = cc; cval = mcval[lc][cc]; onscreen(lc,cc) ; } // You did it !!
     else {trigpush[i]++ ; } // keep going !!
    }
 else  { trigpush[i] = 0 ; }// sorry guy, maybe next time try to push harder and longer
 //  NOT DETECTED 
}

 // scan buttons before pots AND after reading pots for debouncing 
 bt0old  = digitalRead(B0); bt1old  = digitalRead(B1); bt2old  = digitalRead(B2); // read selection switches
 
// ==== UPDATE SCREEN IF ENCODER MOVED ===========================================================
readpots(); // read both encoders, and if any change, increase or decrease lc cc by 1 according to encoder's movements +-
if ( (old_lc!=lc)|| (old_cc!=cc)) // encoder movement detected  
{cval = mcval[lc][cc];    // get corresponding variables 
 old_lc=lc ; old_cc=cc ;
 onscreen(lc,cc); // fetch menu data and display all on screen 
} //endif pot change

//-------------- SELECTION SWITCH PRESSED ? ---------------------------------------------------------------------------------
bt0 = digitalRead(B0); bt1 = digitalRead(B1); bt2  = digitalRead(B2); //read selection switchs again
// ScanBtn();  // S C A N B U T O N S -----------------------      
if ( bt2 && bt2old) {btselect = 2;} 
else {   if ( bt1 && bt1old) { btselect =1;} else { if ( bt0 && bt0old) {btselect = 0;} else {btselect =255;}
     }}  // store the button pressed in btselect :0 left ,1 central,2 right  or 255 if not selected
        
//------------- YES DO SOMETHING ------------------------------------------------------------------------------------------
if (btselect<3) // only manage real actions : 0 , 1 or 2
{
  switch (btselect) 
  {
  case 0:  {               {mcval[lc][cc] = defv ; }                                       }   break;   // push on the left gives usually default value
  case 1:  { if (maxv ==2) {mcval[lc][cc] = 1 ;    }  //push on the right gives 2 if only 3 values possible (0,1,2)
             else          {mcval[lc][cc] = constrain( (mcval[lc][cc])-(incr),minv,maxv);  } } break; // decrease value between minv and maxv 
  case 2:  { if (maxv ==2) {mcval[lc][cc] = 2 ;    } // push in the middle give immediatly 1 if only 3 values possible
             else          {mcval[lc][cc] = constrain( (mcval[lc][cc])+(incr),minv,maxv);  }  }break; // increase value between minv and maxv 
               
  }//end switch
cval=mcval[lc][cc] ;
t23[lc][cc].todo(cval); // execute the function stored on the corresponding line in the abyssal matrix t23 (lc cc)
// the param passed to todo is always cval, not always ideal for the trig functions for example where the index is more useful

dispint(); // execute the display function stored on the corresponding line on the left of todo  (lc cc) 
           // only the central line related to the variable is updated on screen after a selection switch is pressed

} // do nothing with 255 or other values 

}// end if millis > xxx
  
} //end loop ---------------------C'EST FINI -------Go back to work  !!---------------------------------------------------------------------------
