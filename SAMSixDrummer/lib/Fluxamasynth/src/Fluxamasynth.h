/*  
-------------------------------------------------------
Fluxamasynth.h

A library for using the Modern Device Fluxamasynth instrument building and generative music platform.
More info is at www.moderndevice.com/fluxamasynth

-------------------------------------
This software is in the public domain.
Modified 4/2011 R.McGinnis
Updated 2018 Modern Device
------------------------------------------------------- 
*/
#pragma once

#include <Arduino.h>
#include "PgmChange.h"
#include "Fluxamasynth.h"

#define FLUXAMASYNTH_ESP32       1
#define FLUXAMASYNTH_SHIELD      2
#define FLUXAMASYNTH_FOR_FEATHER 3
#define FLUXAMASYNTH_PI          4

#define HI_NIB(b)       ((b) & 0xF0)    //!< Get high nibble.
#define LO_NIB(b)       ((b) & 0x0F)    //!< Get low nibble.

#define MAX_MDATA       0x7F            //!< Max possible MIDI data value.

#define MIDICMD(cmd)    HI_NIB(cmd)     //!< Get MIDI command nibble.
#define MIDICHAN(cmd)   LO_NIB(cmd)     //!< Get MIDI channel nibble.

#ifndef NO_DATA_MASKING
#define MIDIDATA(dat)   byte((dat) & 0x7F)  //!< Mask MIDI 7-bit data.
#else   // Save a little code space and execution time at the expense of safety.
#define MIDIDATA(dat)   (dat)           //!< Unmasked MIDI 7-bit data. Careful with that axe!
#endif

#define MIDICOMM(cmd,chan)    byte(MIDICMD(cmd) | MIDICHAN(chan)) //!< Make a MIDI command byte, cmd=8#-F#,chan=0-F (hex).
#define _MIDICOMM(cmd,chan)   byte((cmd) | MIDICHAN(chan)) //!< Make a MIDI command byte ('cmd' not masked).
#define __MIDICOMM(cmd,chan)  byte((cmd) | (chan)) //!< Make a MIDI command byte ('cmd' and ''chan' not masked).
//@}
//! @defgroup _mcmd MIDI Command byte constants
//! Command bytes are defined as channel 1 messages (channel = 0).
//@{

#define ME_NOTEOFF      0x80  //!< {8x kk vv} Note OFF voice message.
#define ME_NOTEON       0x90  //!< {9x kk vv} Note ON voice message.
#define ME_POLYTOUCH    0xA0  //!< {Ax kk vv} Polyphonic key aftertouch. (Not supported by SAM2195).
#define ME_CONTROL      0xB0  //!< {Bx cc vv} Controller change.
#define ME_PROGCHANGE   0xC0  //!< {Cx pp}    Program change.
#define ME_CHANTOUCH    0xD0  //!< {Dx vv}    Channel aftertouch.
#define ME_PITCHBEND    0xE0  //!< {Ex ll hh} Pitchbend change (14bit; ll/hh).
#define ME_SYSTEM       0xF0  /*!< @brief {Fx} System Common / System RealTime messages.
//       Only SYSEX (F0h/F7h) and RESET (FFh) are supported by SAM2195.
#define ME_SYSEX        0xF0  //!< {F0 id dd..dd F7} System Exclusive message.
#define ME_EOX          0xF7  //!< End of System Exclusive {F7}.
#define ME_RESET        0xFF  //!< Reset all receivers to power-up status.
#define SXID_ROLAND     0x41  //!< Roland sysex id (GS is a Roland standard).
#define SXID_REALTIME   0x7F  //!< RealTime Universal SysEx id.
#define SXID_NON_REALTM 0x7E  //!< Non-RealTime Universal SysEx id.
#define SXM_GS          0x42  //!< GS sysex model id.

#define SX_ALLDEVS      0x7F  //!< All devices (Special device id for sx rt/nrt packets).
#define SX_ALLCHANS     0x7F  //!< All channels (Special channel nr for sx rt/nrt packets).
#define NOTE_C    0
#define NOTE_Cs   1
#define NOTE_D    2
#define NOTE_Ds   3
#define NOTE_E    4
#define NOTE_F    5
#define NOTE_Fs   6
#define NOTE_G    7
#define NOTE_Gs   8
#define NOTE_A    9
#define NOTE_As   10
#define NOTE_B    11

#define VEL_FORTEFORTIS 127   //!< Forte fortizzimo (bombastic)
#define VEL_FORTISSIMO  112	  //!< Fortizzimo (very strong)
#define VEL_FORTE       96	  //!< Forte (strong)
#define VEL_MEZZOFORTE  80	  //!< Mezzo forte (half-strong)
#define VEL_MEZZOPIANO  64	  //!< Mezzo piano (half-calm)
#define VEL_PIANO       48	  //!< Piano (calm)
#define VEL_PIANISSIMO  32	  //!< Pianizzimo (very soft)
#define VEL_PIANOPIANIS 16	  //!< Piano pianizzimo (extremely soft)
#define VEL_MEDIUM      VEL_MEZZOFORTE //!< Half-strong
#define CT_BANKSELECT   0x00  //!< Bank select, for synth with > 128 patches.
#define CT_WHEEL        0x01  //!< Modulation wheel.
#define CT_BREATHCTRL   0x02  //!< Breath controller.
#define CT_FOOTCTRL     0x04  //!< Foot controller (volume, wahwah &c).
#define CT_PORTATIME    0x05  //!< Portamento time
#define CT_DATAENTRY    0x06  //!< Data entry (for RPN and NRPN).
#define CT_VOLUME       0x07  //!< Channel volume.
#define CT_PAN          0x0A  //!< Channel panoration (L <-> R).
#define CT_EXPRESSION   0x0B  //!< Expression controller.
#define CT_GEN_1        0x10  //!< General purpose 1 continuous controller.
#define CT_GEN_2        0x11  //!< General purpose 2 continuous controller.
#define CT_GEN_3        0x12  //!< General purpose 3 continuous controller.
#define CT_GEN_4        0x13  //!< General purpose 4 continuous controller.

#define CT_DAMPER       0x40  //!< 64 - Damper pedal (Sustain) On/Off.
#define CT_PORTAMENTO   0x41  //!< 65 - Portamento On/Off.
#define CT_SOSTENUTO    0x42  //!< 66 - Sostenuto pedal On/Off.
#define CT_SOFT         0x43  //!< 67 - Soft pedal On/Off.
#define CT_REVERBPROG   0x50  //!< 80 - Set reverb program, 0-7. (SAM, Global i.e CH0)
#define CT_CHORUSPROG   0x51  //!< 81 - Set chorus program, 0-7. (SAM, Global i.e CH0)
#define CT_GEN_SW3      0x52  //!< 82 - General purpose switch 3.
#define CT_GEN_SW4      0x53  //!< 83 - General purpose switch 4.

#define CT_REVERB       0x5B  //!< 91 - Reverb depth.
#define CT_TREMOLO      0x5C  //!< 92 - Tremolo depth.
#define CT_CHORUS       0x5D  //!< 93 - Chorus depth.
#define CT_DETUNE       0x5E  //!< 94 - Detune amount.
#define CT_PHASER       0x5F  //!< 95 - Phaser depth.

#define CT_DATAINC      0x60  //!< 96 - Value +1.
#define CT_DATADEC      0x61  //!< 97 - Value -1.
#define CT_NONREG_LSB   0x62  //!< 98 - NRPN (non-registered parameter number) LSB.
#define CT_NONREG_MSB   0x63  //!< 99 - NRPN MSB (high part of number).
#define CT_REG_LSB      0x64  //!< 100 - RPN (registered parameter number) LSB.
#define CT_REG_MSB      0x65  //!< 101 - RPN MSB (most significant byte).

#define CT_ALLSOUNDOFF  0x78  //!< 120 - Silence all outputs.
#define CT_RESETCTRL    0x79  //!< 121 - Reset all controllers to defaults.
#define CT_LOCALCTRL    0x7A  //!< 122 - Local control On/Off (vv=$7F/$00).
#define CT_ALLNOTESOFF  0x7B  //!< 123 - Turn off all oscillators.
#define CT_OMNI_OFF     0x7C  //!< 124 - Omni mode Off (vv=0).
#define CT_OMNI_ON      0x7D  //!< 125 - Omni mode On (vv=0).
#define CT_MONO_ON      0x7E  //!< 126 - Mono mode On (Poly Off) (vv=Nr chans(Omni off) or 0(Omni on)).
#define CT_POLY_ON      0x7F  //!< 127 - Poly mode On (Mono Off) (vv=0).

#define CTV_SWITCHON     0x7F    //!< ON (3rd byte of midi switch controller)
#define CTV_SWITCHOFF    0x00    //!< OFF (3rd byte of midi switch controller)

#define CTV_CENTER       0x40    //!< Neutral (center) value for 7bit bipolar ctrl values
#define CTV_CENTER14     0x2000  //!< Neutral (center) value for 14bit bipolar ctrl values
#define CTV_MAX14        0x3FFF  //!< Maximum 14 bit control value
#define CTV_MASK14       0x3FFF  //!< Mask 14 bits of word value

// Make a 14-bit control value from low/high bytes.
#define CTV_VAL14(lo,hi)    word(((hi & 0x7F) << 7)| (lo & 0x7F))

// Make a 14-bit control value, *no masking*.
#define _CTV_VAL14(lo,hi)   word((hi << 7)| lo )

// Extract low 7 bits of a 14-bit control value.
#define CTV_LOW(val)        byte(val & 0x7F)

// Extract high 7 bits of a 14-bit control value.
#define CTV_HIGH(val)       byte((val >> 7) & 0x7F)

// Extract high 7 bits of a 14-bit control value, *no masking*.
#define _CTV_HIGH(val)      byte(val >> 7)

byte ccByte( int8_t Value );
    /**<
    @brief Convert a signed 8-bit value to a biased MIDI control value.
    @param Value  Signed 8-bit value, -64..+63.
    @return Return a biased control value, 0-127.
    */

word ccWord( short Value );
    /**<
    @brief Convert a signed 14-bit value to a biased MIDI control value.
    @details The equivalent of ((Value + CTV_CENTER14) & CT_MASK14).
    @param Value Signed 14-bit value, -8192..+8191.
    @return Return a biased control value, 0x0000-0x3FFF.
    */
    
//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _eqcv Equalizer Frequency Control Values.
//! @brief Convert corner frequency in Hz to corresponding SAM2195 control value.
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

byte EqBassCtrl( word Hz );
    /**<
    @brief Compute an Equalizer BASS frequency parameter.
    @param Hz Corner frequency, in Hz.
    @return Returns an appropriate control value 0-127.
    @sa \ref FlexamySynth::setEqFrequency, \ref EqBassHz
    */

byte EqMidCtrl( word Hz );
    /**<
    @brief Compute an Equalizer MID frequency parameter.
    @param Hz  Corner frequency, in Hz.
    @return Returns an appropriate control value 0-127.
    @sa \ref FlexamySynth::setEqFrequency, \ref EqMidHz
    */
    
byte EqTrebleCtrl( word Hz );
    /**<
    @brief Compute an Equalizer TREBLE frequency parameter.
    @param Hz  Corner frequency, in Hz.
    @return Returns an appropriate control value 0-127.
    @sa \ref FlexamySynth::setEqFrequency, \ref EqTrebleHz
    */
    
//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _eqbc Equalizer Control back-convert
//! @brief Convert EQ control value back to Hz.
//! @details (Can also tell you how/if your set frequency was quantized.)
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

word EqBassHz( byte ctrlVal );
    /**<
    @brief Compute a frequency from a control value. 
    @details The inverse of \ref EqBassCtrl.
    @param ctrlVal Control value, 0-127 (7Fh).
    */

word EqMidHz( byte ctrlVal );
    /**<
    @brief Compute a frequency from a control value.
    @details The inverse of \ref EqMidCtrl.
    @param ctrlVal Control value, 0-127 (7Fh).
    */

word EqTrebleHz( byte ctrlVal );
    /**<
    @brief Compute a frequency from a control value.
    @details The inverse of \ref EqTrebleCtrl.
    @param ctrlVal Control value, 0-127 (7Fh).
    */










#if defined(ESP_PLATFORM)
    #define FS_PLATFORM FLUXAMASYNTH_ESP32
  #elif defined(ARDUINO_AVR_FEATHER32U4)
    #define FS_PLATFORM FLUXAMASYNTH_FOR_FEATHER
#elif defined(ARDUINO_SAMD_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)
    #define FS_PLATFORM FLUXAMASYNTH_FOR_FEATHER_M0
  #elif defined(ARDUINO)
    #define FS_PLATFORM FLUXAMASYNTH_SHIELD
  #elif defined(RASPBERRY_PI)
    #define FS_PLATFORM FLUXAMASYNTH_PI
#endif

#ifndef FS_PLATFORM
#define FS_PLATFORM FLUXAMASYNTH_SHIELD
#endif

#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
#include <SoftwareSerial.h>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
class Fluxamasynth
{
//private:
public:
#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
  	SoftwareSerial synth;
#endif
	     
  
	Fluxamasynth();
#ifdef FLUXAMASYNTH_SHIELD  
    Fluxamasynth(byte rxPin, byte txPin);
#endif

void _sendPartParameter( byte Part, byte ParmNr, byte CtrlVal );
    /**<
    @brief Send a 'Patch Part Parameter' group 1*h block.
    @param Part    Part nr or midi channel.
    @param ParmNr  LSB of parameter index. (Address e.g. 40h 1*h ParmNr).
    @param CtrlVal Control value, 0-7Fh.
    */

 void _sendModParameter( byte Channel, byte ParmNr, byte CtrlVal );
    /**<
    @brief Send Modulation control (patch part param 2*h group).
    @param Channel Midi channel.
    @param ParmNr  LSB of parameter index. (Address e.g. 40h 2*h ParmNr).
    @param CtrlVal Control value, 0-7Fh.
    */

    void _sendDreamControl( byte FuncNr, byte Value );
    /**<
    @brief Send a Dream NRPN 37**h parameter.
    @param FuncNr LSB of NRPN nr.
    @param Value  Control value.
    */
//protected: 
byte _runStat; 

//public: //-------------------------------------------------------------------------------

void writeMidiCmd( byte Cmd );  
    /**<
    @brief Send a MIDI command.
    @details Manages MIDI running status (saves communication bandwidth).
    @param Cmd  A midi command byte. High nibble=Cmd, low nibble=Channel.
    */

 void sendParameterData( byte *Data, word Length ); 
    /**<
    @brief Send a SysEx DT1 Patch Parameter packet
    @param Data  The block must begin with 2 LSB parameter address,
      followed by the parameter data (commonly 1 byte).
      The MSB parameter address (40h) is hard coded in the function.
    @param  Length  Nr of bytes in the Data block.
    */

 void RPN_Control( byte Channel, byte rpnHi, byte rpnLo, byte Data );
    /**<
    @brief Send a Registered Parameter Number control message.
    @param Channel MIDI channel, 0-15
    @param rpnHi   High part of RPN number
    @param rpnLo   Low part of RPN number
    @param Data    Control value
    */
    
    void NRPN_Control( byte Channel, byte nrpnHi, byte nrpnLo, byte Data );
    /**<
    @brief Send a Non-Registered Parameter Number control message.
    @param Channel MIDI channel, 0-15
    @param nrpnHi  High part of NRPN number
    @param nrpnLo  Low part of NRPN number
    @param Data    Control value
    */

void dataEntry( byte Channel, byte Data );
    /**<
    @brief Provide (more) data to RPN and NRPN.
    @param Channel MIDI channel, 0-15
    @param Data    Control value
    */

   //!\cond nodoc
    #ifdef HAVE_14B_CONTROLLER
    void RPN_ControlW( byte Channel, byte rpnHi, byte rpnLo, word Data ); // 14bit
    void NRPN_ControlW( byte Channel, byte rpnHi, byte rpnLo, word Data ); // 14bit
    void dataEntryW( byte Channel, word Data ); // 14bit
    #endif //!\endcond

    void fluxWrite(byte c); //virtual size_t fluxWrite(byte c);
    void fluxWrite(byte *buf, int cnt); //virtual size_t fluxWrite(byte *buf, int cnt);
    void noteOn(byte channel, byte pitch, byte velocity);
    void noteOff(byte channel, byte pitch);
    void controlChange( byte Channel, byte CtrlNr, byte Value );
    /**<
    @brief Send a Control Change message [B* cc vv].
    @param Channel MIDI channel, 0-15
    @param CtrlNr  Controller nr, 0-127, e.g \ref CT_VOLUME, \ref CT_CHORUS.
    @param Value   Control value, 0-127
    */
    void setControlValue( byte Channel, byte CtrlNr, word Value );
    /**<
    @brief Send a big value (14 bit) Control Change message [B* cc hh cc+20h ll].
    @param Channel MIDI channel, 0-15
    @param CtrlNr  Controller nr, 0-127, e.g \ref CT_WHEEL, \ref CT_EXPRESSION.
    @param Value   Control value, 0-127
    */
    void programChange (byte bank, byte channel, byte v);
    void pitchBend(byte channel, int v);
    void pitchBendRange(byte channel, byte v);
    void midiReset();
    void GM_Reset();
    void GS_Reset();
    void setChannelVolume(byte channel, byte level);
    void allNotesOff(byte channel);
    void setMasterVolume(byte level);
    void GS_MasterVolume( byte Level ); // 0-127
    void MasterPan( byte Pan ); // 00h-40h-7Fh (40h == center)
    void setClippingMode( byte Mode );
    #define SOFT_CLIP   0x00  //!< Soft clipping mode
    #define HARD_CLIP   0x7F  //!< Hard clipping mode
    void setOutputLevel( byte Level ); // 0-127
///////////////////////// REVERB ) MOST PARAM are GLOBAL (not by channel) /////////
    void setReverbSend( byte Channel, byte Level ); // ch 015 , level : 0-127   
    void setReverb(byte channel, byte program, byte level, byte delayFeedback);
    void setReverbLevel( byte MasterLevel ); // 0-127
    void setReverbProgram( byte Program );
    #define REV_ROOM1       0  //!< Room 1 reverb program.
    #define REV_ROOM2       1  //!< Room 2 reverb program.
    #define REV_ROOM3       2  //!< Room 3 reverb program.
    #define REV_HALL1       3  //!< Hall 1 reverb program.
    #define REV_HALL2       4  //!< Hall 2 reverb program.
    #define REV_PLATE       5  //!< Plate reverb program.
    #define REV_DELAY       6  //!< Delay reverb program.
    #define REV_PANDELAY    7  //!< Panning delay reverb program.

    #define REV_DEFLEVEL  0x64 //!< Default reverb master level
    #define REV_DEFCHAR   0x04 //!< Default reverb character

    void setReverbTime( byte Time ); // 0-127
    void setReverbFeedback( byte Feedback ); // 0-127
    // Only valid for REV_DELAY and REV_PANDELAY.
    void setReverbCharacter( byte Character ); // 0-7
///////////////////////////// CHORUS  - MOST PARAMS are GLOBAL /////////////////////////////////

   void setChorus(byte channel, byte program, byte level, byte feedback, byte chorusDelay);
   void setChorusSend( byte Channel, byte Level ); // ch 0-15 ; level 0-127   
   void setChorusLevel( byte MasterLevel ); // 0-127
   void setChorusProgram( byte Program );
   #define CHO_CHORUS1     0  //!< Chorus 1 program.
    #define CHO_CHORUS2     1  //!< Chorus 2 program.
    #define CHO_CHORUS3     2  //!< Chorus 3 program.
    #define CHO_CHORUS4     3  //!< Chorus 4 program.
    #define CHO_FEEDBACK    4  //!< Feedback chorus program.
    #define CHO_FLANGER     5  //!< Flanger program.
    #define CHO_SHORTDELAY  6  //!< Short delay chorus program.
    #define CHO_FBDELAY     7  //!< Feedback delay chorus program.
    void setChorusDelay( byte Delay ); // 
    void setChorusFeedback( byte Feedback );
    void setChorusRate( byte Rate );
    void setChorusDepth( byte Depth );
//-----------------------------------
    void pan(byte channel, byte value);
//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _eq SAM2195 Equalizer Control
//! @brief Change settings for the parametric equalizer.
//!
//! The low-band filter frequency is 0-4700 Hz, the two mid-band
//! filters are 0-4200 Hz, and the high-band filter 0-18750 Hz.
//! (The actual lower frequency limit of each band is as yet undetermined,
//! so it's uncertain what frequency control value 0 correspond to).
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    #define EQ_BASS     0   //!< EQ Band 0: 0-4.7 kHz, +-12 dB [default 444Hz(0Ch) +6dB(60h)]
    #define EQ_LOWMID   1   //!< EQ Band 1: 0-4.2 kHz, +-12 dB [default 893Hz(1Bh) 0dB(40h)]
    #define EQ_HIGHMID  2   //!< EQ Band 2: 0-4.2 kHz, +-12 dB [default 3770kHz(72h) 0dB(40h)]
    #define EQ_TREBLE   3   //!< EQ Band 3: 0-18.75 kHz, +-12 dB [default 9449Hz(40h) +6dB(60h)]
    void setEQ(byte channel, byte lowBand, byte medLowBand, byte medHighBand, byte highBand,
               byte lowFreq, byte medLowFreq, byte medHighFreq, byte highFreq);

    void setEqualizer( byte BandNr, byte Freq, byte Gain );
    //@brief Change settings for one of 4 parametric equalizers.
    
    // Example: setEqualizer( EQ_BASS, EqBassCtrl( 220 ), 0x60 );
    // @param BandNr Equalizer nr, 0-3, or one of the \ref EQ_BASS "EQ_nnn" values.
    //@param Freq Corner frequency, 0-127. See \ref setEqFrequency.


    void setEqFrequency( byte BandNr, byte Frequency );
    /**< 
    @brief Change corner frequency for one of 4 parametric equalizers.
    The \ref EqBassCtrl, \ref EqMidCtrl, and \ref EqTrebleCtrl
    functions compute an appropriate Frequency value for the filter, 
    given an argument in Hz. EqBassCtrl is used for the EQ_BASS filter,
    EqMidCtrl for EQ_LOWMID and EQ_HIGHMID, and EqTrebleCtrl for EQ_TREBLE.
    @param BandNr Equalizer nr, 0-3, or one of the \ref EQ_BASS "EQ_nnn" values.
    @param Frequency Corner frequency, 0-127.
    */

    void setEqGain( byte BandNr, byte Gain );
    /**< 
    @brief Change gain for one of 4 parametric equalizers.
        
    Set the filter gain +-12 dB, in steps of 0.1875 dB.
    You can use the \ref EQL_P_12dB "EQL_nnn" constants for common gain values.
    @param BandNr Equalizer nr, 0-3, or one of the \ref EQ_BASS "EQ_nnn" values.
    @param Gain   Gain +-12dB, 00h-40h-7Fh (40h == 0dB).
    */
    #define EQL_P_12dB  0x7F  //!< +12 dB EQ gain
    #define EQL_P_9dB   0x70  //!<  +9 dB EQ gain
    #define EQL_P_6dB   0x60  //!<  +6 dB EQ gain
    #define EQL_P_3dB   0x50  //!<  +3 dB EQ gain
    #define EQL_0dB     0x40  //!<   0 dB EQ gain
    #define EQL_M_3dB   0x30  //!<  -3 dB EQ gain
    #define EQL_M_6dB   0x20  //!<  -6 dB EQ gain
    #define EQL_M_9dB   0x10  //!<  -9 dB EQ gain
    #define EQL_M_12dB  0x00  //!< -12 dB EQ gain

//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _3d SAM2195 3D Surround Control
//! @brief Set volume, time, and input mode of the 3D surround unit.
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void setSurroundVolume( byte Level );
    /**<
    @brief Set 3D surround level.
    @param Level  Surround level, 0-127.
    */

    void setSurroundDelay( byte Time );
    /**<
    @brief Set 3D surround time.
    @param Time  Surround delay, 0-127.
    */

    void surroundMonoIn( boolean Mono );
    /**<
    @brief Switch 3D surround input between mono and stereo mode.
    @param Mono  True for mono, false for stereo input.
    */


//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _post SAM2195 Post Processing Control
//! @brief   Signal routing (post processing on/off).
//! @details Post processing effects are the equalizer and 3D surround unit.
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void postprocGeneralMidi( boolean On );
    /**< 
    @brief Turn GM post processing on or off.
    @param On  True to turn on, false to turn off.
    */

    void postprocReverbChorus( boolean On );
    /**<
    @brief Turn post processing on or off for the reverb and chorus unit.
    @param On  True to turn on, false to turn off.
    */

//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _gmctrl SAM2195 Special GM Controls
//! @brief General Midi controls (SAM2195 NRPN 0x37## controls)
//! 
//! @note At the time of this writing, it is undetermined if the basic\n
//! GM controls in SAM2195 support individual channels or are global.\n
//! Both implementations are provided so you may find out.\n
//! Please report your findings to the lib distribution page.
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // Channel 0 (Midi cmd byte == 0xB0)
    
    void GM_ReverbSend( byte Level );
    /**< 
    @brief General Midi reverb send.
    @param Level  Send level, 0-127, default 64.
    */

    void GM_ChorusSend( byte Level );
    /**<
    @brief General Midi chorus send.
    @param Level  Send level, 0-127, default 64.
    */

    void GM_Volume( byte Level );
    /**<
    @brief General Midi volume.
    @param Level  Output level, 0-127.
    */

    void GM_Pan( byte Pan );
    /**< 
    @brief General Midi panoration.
    @param Pan  Pan position, 00h-40h-7Fh (40h == center).
    */

    // Channel specific (Midi cmd byte == 0xB#)
    
    void GM_ReverbSend( byte Channel, byte Level );
    /**<
    @brief General Midi reverb send.
    @param Channel MIDI channel, 0-15.
    @param Level   Send level, 0-127, default 64.
    */

    void GM_ChorusSend( byte Channel, byte Level );
    /**< 
    @brief General Midi chorus send.
    @param Channel MIDI channel, 0-15.
    @param Level   Send level, 0-127, default 64.
    */

    void GM_Volume( byte Channel, byte Level );
    /**<
    @brief General Midi volume.
    @param Channel MIDI channel, 0-15.
    @param Level   Output level, 0-127.
    */

    void GM_Pan( byte Channel, byte Pan );
    /**<
    @brief General Midi panoration.
    @param Channel MIDI channel, 0-15.
    @param Pan     Pan position, 0-127 (40h == center).
    */





    void setTuning(byte channel, byte coarse, byte fine);
    void setVibrate(byte channel, byte rate, byte depth, byte mod);
void setLfoRate( byte CtrlVal );
    /**<
    @brief Set LFO rate (frequency) for all channels.
    @param CtrlVal  LFO rate control, 0-127 (default==64)
    @note The LFO rate is global for all 16 channels.
    */

///////////////// FILTER ---------------------------------------------
    void setTVF(byte channel, byte cutoff, byte resonance);
   
 void setTvFilter( byte Channel, byte CutoffFreq, byte Resonance );
    /**<
    @brief Change Time-Variant Filter settings for the channel.
    @param Channel    MIDI channel, 0-15
    @param CutoffFreq Filter corner frequency, 0-127
    @param Resonance  Filter resonance, 0-127
    */
    
    void setTvfCutoff( byte Channel, byte CutoffFreq );
    /**<
    @brief Set cutoff frequency of the time variant filter.
    @param Channel    MIDI channel, 0-15
    @param CutoffFreq Filter corner frequency, 0-127
    */
    
    void setTvfResonance( byte Channel, byte Resonance );
    /**<
    @brief Set resonance of the time variant filter.
    @param Channel    MIDI channel, 0-15
    @param Resonance  Filter resonance, 0-127
    */

///////////// ENVELOPPE ------------------------------
    void setEnvelope(byte channel, byte attack, byte decay, byte release);
 
    void setEnvAttack( byte Channel, byte Attack );
    /**<
    @brief Change envelope attack time for the channel.
    @param Channel  MIDI channel, 0-15
    @param Attack   Initial peak rise time of envelope, 0-127
    */

    void setEnvDecay( byte Channel, byte Decay );
    /**<
    @brief Change envelope decay time for the channel.
    @param Channel  MIDI channel, 0-15
    @param Decay    Falloff time to sustain of envelope, 0-127
    */
    
    void setEnvRelease( byte Channel, byte Release );
    /**<
    @brief Change envelope release time for the channel.
    @param Channel  MIDI channel, 0-15
    @param Release  Key release fadeout time, 0-127
    */

    
    void setScaleTuning(byte channel, byte v1, byte v2, byte v3, byte v4, byte v5, byte v6,
                        byte v7, byte v8, byte v9, byte v10, byte v11, byte v12);
    void setModWheel(byte channel, byte pitch, byte tvtCutoff, byte amplitude, byte rate, byte pitchDepth, byte tvfDepth, byte tvaDepth);
	void allDrums();

//////////// VIBRATO -----------------------------------------------------------
 void setVibratoRate( byte Channel, byte Rate );
    /**<
    @brief Set vibrato rate. 
    @param Channel  MIDI channel, 0-15
    @param Rate     Vibrato rate, 0-127.
    */

    void setVibratoDepth( byte Channel, byte Depth );
    /**<
    @brief Set vibrato depth. 
    @param Channel  MIDI channel, 0-15
    @param Depth    Vibrato depth, 0-127.
    */
    
    void setVibratoDelay( byte Channel, byte Delay );
    /**<
    @brief Set vibrato delay. 
    @param Channel  MIDI channel, 0-15
    @param Delay    Vibrato delay, 0-127.
    */
////////////////// DRUMS ------------------------------------------------------------------
//@} ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//! @defgroup _drum SAM2195 Advanced Drum Settings
//! @brief Allow precise control of the drum synth mix and effects.
//@{ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //! @brief Change the pitch of a single drum.

    void setDrumPitch ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Semitone   //!< Semitones up or down, 00h-40h-7Fh (40h == no detune).
        );
    
    //! @brief Advanced drum mix settings.


    void drumset (byte drumkit) ; // 0=STD

    void setDrumMix ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Level,     //!< Sound level, 0-127.
        byte Pan,       //!< Pan setting, 00h-40h-7Fh (40h == center).
        byte Reverb,    //!< Reverb send, 0-127.
        byte Chorus     //!< Chorus send, 0-127.
        );

    void setDrumVol ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Level     //!< Sound level, 0-127.
        );

    void setDrumPan ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Pan       //!< Pan setting, 00h-40h-7Fh (40h == center).
        );

    void setDrumReverb ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Reverb    //!< Reverb send, 0-127.
        );

    void setDrumChorus ( 
        byte Channel,   //!< MIDI channel, 0-15
        byte DrumNr,    //!< Drum nr, see \ref PgmChange.h
        byte Chorus     //!< Chorus send, 0-127.
        );        
        
    void mikealleffects()  ; // poly = 35
    void nomicrevchor()    ; // no mike  reverb + chorus   
    void nomiceq()         ; //      no mike, reverb + chorus + EQ 
    void nomiceqsp()       ; //    no mike, reverb + chorus + eq +spatial  
    
    void micrevchor()      ; //  mike  reverb + chorus   
    void miceq()           ; //       mike, reverb + chorus + EQ 
    void miceqsp()         ; //     mike, reverb + chorus + eq +spatial  
    void resetallnomike()  ; // default no mike, no echo   
    void micvol( byte vol) ; // MIC Volume NRPN 3724h
    void setgain( byte highreg, byte lowreg ) ; // write full register containing in gain AND out gain
    
};

