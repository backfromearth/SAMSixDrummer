/*  
-------------------------------------------------------
Fluxamasynth.cpp

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
#include "Fluxamasynth.h"

#if (FS_PLATFORM == FLUXAMASYNTH_SHIELD)
                                                       
Fluxamasynth::Fluxamasynth() : synth(255, 4) {      
    synth.begin(31250);
}

Fluxamasynth::Fluxamasynth(byte rxPin, byte txPin) : synth(rxPin, txPin) {
    synth.begin(31250);
}

size_t Fluxamasynth::fluxWrite(byte c) {
    synth.write(c); return 0;
}

size_t Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        synth.write(buf[i]);
    }
} return 0;
#endif

#if (FS_PLATFORM == FLUXAMASYNTH_ESP32) 
#include <HardwareSerial.h>

HardwareSerial synth32(2);

Fluxamasynth::Fluxamasynth() {  
    synth32.begin(31250);
	while (!synth32);
}

void Fluxamasynth::fluxWrite(byte c) {
    synth32.write(c);
}

void Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        synth32.write(buf[i]); 
    }
}

#endif

#if (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER)  || (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER_M0)

Fluxamasynth::Fluxamasynth() {  
#if (FS_PLATFORM == FLUXAMASYNTH_FOR_FEATHER)
	Serial1.begin(31250);
#endif
}

size_t Fluxamasynth::fluxWrite(byte c) {
    Serial1.write(c);
}

size_t Fluxamasynth::fluxWrite(byte *buf, int cnt) {
    for (int i=0; i<cnt; i++) {
        Serial1.write(buf[i]);
    }
}
#endif

void Fluxamasynth::writeMidiCmd( byte Cmd )
{
    if (Cmd != _runStat)    // MIDI running status changed
    {
        _runStat = Cmd;
       this->fluxWrite( Cmd );   // Write new command byte
    }                       // else we're done!
}


void Fluxamasynth::sendParameterData( byte *data, word length ) 
{
    // Send a Parameter Control sysex message.
    // All but two of the sysex blocks that Atmel Dream 2195 respond to
    // use a common Roland GS compatible header, so it's hard coded here.
    // All invokations must skip the header, and the tailing sysex end (0xF7)

    byte head[6] =   // (Field comments assume GS packet compatibility)
    {
        0xF0 ,       // ME_SYSEX,    // F0h
        0x41 ,       //SXID_ROLAND, // Roland id (41h)
        0x00,        // Device nr
        0x42 ,       //SXM_GS,      // Model id (GS)
        0x12,        // Command id (DT1 - Data One Way) 
        0x40         // Parameter address MSB
        // 'data' array begin with two address bytes
        // followed by parameter data
    };
    byte tail[2] = 
    { 
        0x00,       // Checksum (ignored by 2195; transmit for compatibilty)
        0xF7       // ME_EOX      // End of exclusive
    };
   byte  _runStat = 0xF0 ; //ME_SYSEX;
this->fluxWrite( head, 6 );
this->fluxWrite( data, length );
this->fluxWrite( tail, 2 );
}

void Fluxamasynth::_sendPartParameter( byte Part, byte ParmNr, byte CtrlVal ) 
{ const byte a = 0x10 ;  // lowByte(0x10);
    byte sxdata[3] = {  byte(a | MIDICHAN( Part )), ParmNr, CtrlVal };
    sendParameterData( sxdata,3 );
}

//# Send mod control parameter (patch part param 0x2p group)
    
void Fluxamasynth::_sendModParameter( byte Channel, byte ParmNr, byte CtrlVal ) 
{
    byte sxdata[3] = { (byte)(0x20 | MIDICHAN( Channel )), ParmNr, CtrlVal };
    sendParameterData( sxdata,3 );
}

void Fluxamasynth::_sendDreamControl( byte FuncNr, byte Value ) 
{
    NRPN_Control( 0x00 ,0x37, FuncNr, Value );
}

//-----------------------------------------------------------------------------
// Registered / Non-Registered Parameter Numbers
//-----------------------------------------------------------------------------

void Fluxamasynth::RPN_Control( byte Channel, byte rpnHi, byte rpnLo, byte Value ) 
{
    byte data[6] = // [Bx] 65 hi 64 lo 06 vh
    {
        0x65, MIDIDATA( rpnHi ), 0x64, MIDIDATA( rpnLo ),
        0x06, MIDIDATA( Value )
    };
    writeMidiCmd(_MIDICOMM( ME_CONTROL, Channel ));
   this->fluxWrite( data, 6 );
}

void Fluxamasynth::NRPN_Control( byte Channel, byte nrpnHi, byte nrpnLo, byte Value ) 
{
    byte data[6] = // [Bx] 63 hi 62 lo 06 val
    {
        0x63, MIDIDATA( nrpnHi ), 0x62, MIDIDATA( nrpnLo ),
        0x06, MIDIDATA( Value )
    };
    writeMidiCmd(_MIDICOMM( 0xb0, Channel ));
    this->fluxWrite( data, 6 );
}
///////////////////-------------------------------------------------------------
void Fluxamasynth::noteOn(byte channel, byte pitch, byte velocity) {
    byte command[3] = { byte(0x90 | (channel & 0x0f)), pitch,velocity };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::noteOff(byte channel, byte pitch) {
    byte command[3] = { byte(0x80 | (channel & 0x0f)), pitch, byte(0x00) };
    this->fluxWrite(command, 3);
}


void Fluxamasynth::drumset(byte drumkit) {
    // drumchannel = 10 (09h) bank is either 1,STD 17,BRUSH 41,49,127 for drumkits 
    byte command[2] = { byte(0xc9), drumkit };
    this->fluxWrite(command, 2);
}                   


void Fluxamasynth::programChange(byte bank, byte channel, byte v) {
    // bank is either 0 GM or 127 MT-32, for instruments only, not for drums
    byte command[3] = { byte(0xB0 | (channel & 0x0f)), 0x00, bank };
    this->fluxWrite(command, 3);
    this->fluxWrite(0xc0 | (channel & 0x0f));
    this->fluxWrite(v);
}


void Fluxamasynth::controlChange( byte Channel, byte CtrlNr, byte Value ) 
{
    byte data[2] = { MIDIDATA( CtrlNr ), MIDIDATA( Value ) };
    writeMidiCmd(_MIDICOMM( ME_CONTROL, Channel ));
   this->fluxWrite( data, 2 );
}

// Set 14bit continuous controller, e.g ModWheel, Expression, Volume, Pan
// (Not sure if 14bit continuous controllers are supported by 2195)
// Gave this it's own name instead of overloading controlChange.
void Fluxamasynth::pitchBend(byte channel, int v) {
    // v is a value from 0 to 1023
    // it is mapped to the full range 0 to 0x3fff
    v = map(v, 0, 1023, 0, 0x3fff);
    byte command[3] = { byte(0xe0 | (channel & 0x0f)), byte(v & 0x00ef), byte(v >> 7) };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::pitchBendRange(byte channel, byte v) {
    // Also called pitch bend sensitivity
    //BnH 65H 00H 64H 00H 06H vv
    byte command[7] = {byte(0xb0 | (channel & 0x0f)), 0x65, 0x00, 0x64, 0x00, 0x06,byte (v & 0x7f)};
    this->fluxWrite(command, 7);
}

void Fluxamasynth::midiReset() {
    this->fluxWrite(0xff);
}


void Fluxamasynth::GM_Reset() // GM - General MIDI reset            //// 1.0 bouton centre
{
    byte command[6] = { 0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7 };  

    this->fluxWrite( command, 6 );
}
// ME_SYSEX 0xF0 // {F0 id dd..dd F7} System Exclusive Message

void Fluxamasynth::GS_Reset() // GS - Reset GS settings            //// 1.0 bouton droit
{
    byte sxdata[3] = { 0, 0x7F, 0 };
    sendParameterData( sxdata, 3 );
}




void Fluxamasynth::setChannelVolume(byte channel, byte level) {
    byte command[3] = { byte(0xb0 | (channel & 0x0f)), 0x07, level };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::allNotesOff(byte channel) {
    // BnH 7BH 00H
    byte command[3] = { byte(0xb0 | (channel & 0x0f)), 0x7b, 0x00 };
    this->fluxWrite(command, 3);
}

void Fluxamasynth::setMasterVolume(byte level) {
    //F0H 7FH 7FH 04H 01H 00H ll F7H  Standard SYSEX GM page 27
    byte command[8] = { 0xf0, 0x7f, 0x7f, 0x04, 0x01, 0x00,byte (level & 0x7f), 0xf7 };
    this->fluxWrite(command, 8);
}

void Fluxamasynth::GS_MasterVolume(byte level) // 0..127 default 127
{
   // F0 41 00 42 12 40 00 04 vv xx F7        ---- xx = don't care 
   byte command[11] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x00,0x04, byte(level & 0x7f), 0x07,0xf7 };
    this->fluxWrite(command, 11);
}

void Fluxamasynth::MasterPan( byte Pan ) // 00h-40h-7Fh (40h == center)
{
  //byte sxdata[11] = { 0,6, MIDIDATA( Pan )}; // F0 41 00 42 12 40 00 06 vv xx F7
  //sendParameterData( sxdata, 3 );
  byte sxdata[11] = { 0xf0 ,0x41 ,0x00 ,0x42, 0x12, 0x40, 0x00, 0x06,byte (Pan & 0x7f), 0x07, 0xf7};
  this->fluxWrite(sxdata, 11);
}


//+ Clipping 

void Fluxamasynth::setClippingMode( byte Mode ) // soft / hard  // 1.5
// NRPN 3713 = B0 63 6E 62 13 
{
    _sendDreamControl( 0x13, Mode );
}

///////////// REVERB ---------------------------------------------------------------
void Fluxamasynth::setReverb(byte channel, byte program, byte level, byte delayFeedback) {
    // Program 
    // 0: Room1   1: Room2    2: Room3 
    // 3: Hall1   4: Hall2    5: Plate
    // 6: Delay   7: Pan delay
    this->fluxWrite(byte(0xb0 | (channel & 0x0f)));
    this->fluxWrite(0x50);
    this->fluxWrite(byte(program & 0x07));
 
    // Set send level
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x5b);
    this->fluxWrite(level & 0x7f);
  
    if (delayFeedback > 0) {
      //F0H 41H 00H 42H 12H 40H 01H 35H vv xx F7H
      byte command[11] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x01, 0x35, byte(delayFeedback & 0x7f), 0x00, 0xf7 };
      this->fluxWrite(command, 11);
    }
}

void Fluxamasynth::setReverbSend( byte Channel, byte Level ) 
{
    controlChange( Channel, 0x5B, Level ); // 5B - Effect 1 depth
}

// Global reverb parameters

void Fluxamasynth::setReverbLevel( byte MasterLevel )
{
    byte sxdata[3] = { 0x01, 0x33, MIDIDATA( MasterLevel )};
    sendParameterData( sxdata, 3 );
}

void Fluxamasynth::setReverbProgram( byte Program ) 
{
    // Reverb programs
    // 0: Room1   1: Room2    2: Room3 
    // 3: Hall1   4: Hall2    5: Plate
    // 6: Delay   7: Pan delay
    byte pgdata[3] = { 0x01, 0x30, byte(Program & 0x07) };
    sendParameterData( pgdata, 3 );
}

void Fluxamasynth::setReverbTime( byte Time ) 
{
    if (Time < 0x80) 
    {
        byte tmdata[3] = { 0x01, 0x34, Time }; //F0H 41H 00H 42H 12H 40H 01H 34H vv xx F7H
        sendParameterData( tmdata, 3 );
    }
}

void Fluxamasynth::setReverbFeedback( byte Feedback ) 
{
    if (Feedback < 0x80) 
    {
        byte fbdata[3] = { 0x01, 0x35, Feedback }; //F0H 41H 00H 42H 12H 40H 01H 35H vv xx F7H
        sendParameterData( fbdata, 3 );
    }
}

void Fluxamasynth::setReverbCharacter( byte Character ) 
{
    if (Character < 8) 
    {
        byte crdata[3] = { 0x01, 0x31, Character }; //F0H 41H 00H 42H 12H 40H 01H 31H vv xx F7H
        sendParameterData( crdata, 3 );
    }
}
////////////////////// CHORUS ----------------------------------------------
void Fluxamasynth::setChorus(byte channel, byte program, byte level, byte feedback, byte chorusDelay) {
    // Program 
    // 0: Chorus1   1: Chorus2    2: Chorus3 
    // 3: Chorus4   4: Feedback   5: Flanger
    // 6: Short delay   7: FB delay
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x51);
    this->fluxWrite(program & 0x07);
 
    // Set send level
    this->fluxWrite(0xb0 | (channel & 0x0f));
    this->fluxWrite(0x5d);
    this->fluxWrite(level & 0x7f);
  
    if (feedback > 0) {
    //F0H 41H 00H 42H 12H 40H 01H 3BH vv xx F7H
	byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3B, byte(feedback & 0x7f), 0x00, 0xf7 };
	this->fluxWrite(command, 11);
    }
  
    if (chorusDelay > 0) {
    // F0H 41H 00H 42H 12H 40H 01H 3CH vv xx F7H
    byte command[11] = { 0xf0, 0x41, byte(0x00), 0x42, 0x12, 0x40, 0x01, 0x3C, byte(chorusDelay & 0x7f), 0x00, 0xf7 };
	this->fluxWrite(command, 11);
    }
}

void Fluxamasynth::setChorusSend( byte Channel, byte Level )                                      // 2.0
{
    controlChange( Channel, 0x5D, Level ); // 5D - Chorus depth
}

void Fluxamasynth::setChorusLevel( byte MasterLevel )  // Master chorus return level                //2.1
{
    byte sxdata[3] = { 0x01, 0x3A, MIDIDATA( MasterLevel )};
    sendParameterData( sxdata, 3 );
}

void Fluxamasynth::setChorusProgram( byte Program)                                            //2.2
{
    // Chorus programs
    // 0: Chorus1   1: Chorus2  2: Chorus3      3: Chorus4
    // 4: Feedback  5: Flanger  6: Short delay  7: FB delay
    byte pgdata[3] = { 0x01, 0x38, byte(Program & 0x07) };
    sendParameterData( pgdata, 3 );
}

void Fluxamasynth::setChorusDelay( byte Delay )                                                  // 2.3
{
    if (Delay < 0x80) 
    {
        byte ddata[3] = { 0x01, 0x3C, Delay }; // F0H 41H 00H 42H 12H 40H 01H 3CH vv xx F7H
        sendParameterData( ddata,3 );
    }
}
void Fluxamasynth::setChorusFeedback( byte Feedback )            // 2.4
{
    if (Feedback < 0x80) 
    {
        byte fdata[3] = { 0x01, 0x3B, Feedback }; //F0H 41H 00H 42H 12H 40H 01H 3BH vv xx F7H
        sendParameterData( fdata,3 );
    }
}
void Fluxamasynth::setChorusRate( byte Rate )                   //2.5
{
    if (Rate < 0x80) 
    {
        byte rdata[3] = { 0x01, 0x3D, Rate }; // F0H 41H 00H 42H 12H 40H 01H 3DH vv xx F7H
        sendParameterData( rdata,3 );
    }
}
void Fluxamasynth::setChorusDepth( byte Depth )                   //2.6
{
    if (Depth < 0x80) 
    {
        byte edata[3] = { 0x01, 0x3E, Depth }; // F0H 41H 00H 42H 12H 40H 01H 3EH vv xx F7H
        sendParameterData( edata,3 );
    }
}
//////////////////////// END CHORUS ------------------------------

void Fluxamasynth::pan(byte channel, byte value) {
  byte command[3] = {
    byte(0xb0 | byte(channel & 0x0f)), (byte) 0x0a, value   };
  this->fluxWrite(command, 3);
}
//////////////////// EQ -------------------------------------
void Fluxamasynth::setEQ(byte channel, byte lowBand, byte medLowBand, byte medHighBand, byte highBand,
           byte lowFreq, byte medLowFreq, byte medHighFreq, byte highFreq) {
    //BnH 63H 37H 62H 00H 06H vv   low band
    //BnH 63H 37H 62H 01H 06H vv   medium low band
    //BnH 63H 37H 62H 02H 06H vv   medium high band
    //BnH 63H 37H 62H 03H 06H vv   high band
    //BnH 63H 37H 62H 08H 06H vv   low freq
    //BnH 63H 37H 62H 09H 06H vv   medium low freq
    //BnH 63H 37H 62H 0AH 06H vv   medium high freq
    //BnH 63H 37H 62H 0BH 06H vv   high freq
    byte command[7] = {byte(0xb0 | (channel & 0x0f)), 0x63, 0x37, 0x62, 0x00, 0x06, byte(lowBand & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x01;
    command[6] = (medLowBand & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x02;
    command[6] = (medHighBand & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x03;
    command[6] = (highBand & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x08;
    command[6] = (lowFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x09;
    command[6] = (medLowFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x0A;
    command[6] = (medHighFreq & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x0B;
    command[6] = (highFreq & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setTuning(byte channel, byte coarse, byte fine) {
    // This will turn off any note playing on the channel
    //BnH 65H 00H 64H 01H 06H vv  Fine
    //BnH 65H 00H 64H 02H 06H vv  Coarse
    byte command[7] = {byte(0xb0 | (channel & 0x0f)), 0x65, 0x00, 0x64, 0x01, 0x06, byte(fine & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x02;
    command[6] = (coarse & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setVibrate(byte channel, byte rate, byte depth, byte mod) {
    //BnH 63H 01H 62H 08H 06H vv  Rate
    //BnH 63H 01H 62H 09H 06H vv  Depth
    //BnH 63H 01H 62H 0AH 06H vv  Delay modify
    byte command[7] = {byte(0xb0 | (channel & 0x0f)), 0x63, 0x01, 0x62, 0x08, 0x06, byte(rate & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x09;
    command[6] = (depth & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x0A;
    command[6] = (mod & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setVibratoRate( byte Channel, byte RateVal ) 
{
    NRPN_Control( Channel, 1, 0x08, RateVal );
}

void Fluxamasynth::setVibratoDepth( byte Channel, byte DepthVal ) 
{
    NRPN_Control( Channel, 1, 0x09, DepthVal );
}
    
void Fluxamasynth::setVibratoDelay( byte Channel, byte DelayVal ) 
{
    NRPN_Control( Channel, 1, 0x0A, DelayVal );
}

// LFO

void Fluxamasynth::setLfoRate( byte CtrlVal ) 
{
    _sendModParameter( 0, 0x03, CtrlVal ); // Rate is common to all channe
}

/////////////////////// FILTER --------------------------------------------------------
void Fluxamasynth::setTVF(byte channel, byte cutoff, byte resonance) {
    //BnH 63H 01H 62H 20H 06H vv  Cutoff
    //BnH 63H 01H 62H 21H 06H vv  Resonance
    byte command[7] = {byte(0xb0 | byte(channel & 0x0f)), 0x63, 0x01, 0x62, 0x20, 0x06,byte (cutoff & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x21;
    command[6] = (resonance & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setTvfCutoff( byte Channel, byte CutoffFreq ) 
{
    NRPN_Control( Channel, 1, 0x20, CutoffFreq );
}
    
void Fluxamasynth::setTvfResonance( byte Channel, byte Resonance ) 
{
    NRPN_Control( Channel, 1, 0x21, Resonance );
}



////////////////////// ENVELOPPE ------------------------------------------------------
void Fluxamasynth::setEnvelope(byte channel, byte attack, byte decay, byte release) {
    //BnH 63H 01H 62H 63H 06H vv
    //BnH 63H 01H 62H 64H 06H vv
    //BnH 63H 01H 62H 66H 06H vv
    byte command[7] = {byte(0xb0 | byte(channel & 0x0f)), 0x63, 0x01, 0x62, 0x63, 0x06, byte(attack & 0x7f)};
    this->fluxWrite(command, 7);
    command[4] = 0x64;
    command[6] = (decay & 0x7f);
    this->fluxWrite(command, 7);
    command[4] = 0x66;
    command[6] = (release & 0x7f);
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setEnvAttack( byte Channel, byte Attack ) 
{
    NRPN_Control( Channel, 1, 0x63, Attack );
}

void Fluxamasynth::setEnvDecay( byte Channel, byte Decay ) 
{
    NRPN_Control( Channel, 1, 0x64, Decay );
}

void Fluxamasynth::setEnvRelease( byte Channel, byte Release ) 
{
    NRPN_Control( Channel, 1, 0x66, Release );
}

/////////////////// SURROUND ----------------------------------------------------------

void Fluxamasynth::setSurroundVolume( byte Level )   // 4.4
{
    _sendDreamControl( 0x20, Level );
}

void Fluxamasynth::setSurroundDelay( byte Time )   // 4.5
{
    _sendDreamControl( 0x2C, Time );
}

void Fluxamasynth::surroundMonoIn( boolean Mono ) //4.6
{
    _sendDreamControl( 0x2D, Mono ? 0x7F : 0 );
}




void Fluxamasynth::setScaleTuning(byte channel, byte v1, byte v2, byte v3, byte v4, byte v5, byte v6,
                                  byte v7, byte v8, byte v9, byte v10, byte v11, byte v12) {
    //F0h 41h 00h 42h 12h 40h 1nh 40h v1 v2 v3 ... v12 F7h
    // values are in range 00h = -64 cents to 7fh = +64 cents, center is 40h
    byte command[21] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, byte(0x10 | byte(channel & 0x0f)), 0x40,
        v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, 0xf7 };
    this->fluxWrite(command, 21);
}

void Fluxamasynth::setModWheel(byte channel, byte pitch, byte tvtCutoff, byte amplitude, byte rate, byte pitchDepth, byte tvfDepth, byte tvaDepth) {
    //F0h 41h 00h 42h 12h 40h 2nh 00h vv xx F7h
    byte command[11] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, byte(0x20 |byte (channel & 0x0f)), 0x00, pitch, 0x00, 0xf7 };
    this->fluxWrite(command, 11);
    command[8] = 0x01;
    command[9] = tvtCutoff;
    this->fluxWrite(command, 11);
    command[8] = 0x02;
    command[9] = amplitude;
    this->fluxWrite(command, 11);
    command[8] = 0x03;
    command[9] = rate;
    this->fluxWrite(command, 11);
    command[8] = 0x04;
    command[9] = pitchDepth;
    this->fluxWrite(command, 11);
    command[8] = 0x05;
    command[9] = tvfDepth;
    this->fluxWrite(command, 11);
    command[8] = 0x06;
    command[9] = tvaDepth;
    this->fluxWrite(command, 11);
}
//////////////// POST PROCESS ------------------------------------------------------
void Fluxamasynth::postprocGeneralMidi( boolean On ) // 5.5
{
    _sendDreamControl( 0x18, On ? 0x7F : 0 );
}

void Fluxamasynth::postprocReverbChorus( boolean On )  //5.6
{
    _sendDreamControl( 0x1A, On ? 0x7F : 0 );
}

///////////// DRUMS ------------------------------------------
void Fluxamasynth::allDrums() {
    //F0h 41h 00h 42h 12h 40h 1ph 15h vv xx F7h
    byte command[21] = { 0xf0, 0x41, 0x00, 0x42, 0x12, 0x40, 0x10, 0x15, 0x01, 0x00, 0xf7 };
    this->fluxWrite(command, 11);
	for (byte i=1; i<15; i++) {
	  command[6] = i;
	  this->fluxWrite(command, 11);
    } 
}


void Fluxamasynth::setDrumMix( 
    byte Channel, byte DrumNr, byte Level, byte Pan, byte Reverb, byte Chorus 
    ) 
{
    DrumNr &= 0x7F;
    if (Level < 0x80) NRPN_Control( Channel, 0x1A, DrumNr, Level );
    if (Pan < 0x80)   NRPN_Control( Channel, 0x1C, DrumNr, Pan );
    if (Reverb < 0x80) NRPN_Control( Channel, 0x1D, DrumNr, Reverb );
    if (Chorus < 0x80) NRPN_Control( Channel, 0x1E, DrumNr, Chorus );
}

void Fluxamasynth::setDrumVol( byte Channel, byte DrumNr, byte Level ) //NEW !!!
{
    DrumNr &= 0x7F;
    NRPN_Control( Channel, 0x1A, DrumNr, Level );
}

void Fluxamasynth::setDrumPan( byte Channel, byte DrumNr, byte Pan ) //NEW !!!
{
    //DrumNr &= 0x7F; NRPN_Control( Channel, 0x1C, DrumNr, Pan );
// NRPN_1Crrh=  Bnh 63h 1Ch 62h rr 06h vv Pan of drum instrument note rr (40h = middle) 

 byte command[7] = {byte(0xb0 |byte (Channel & 0x0f)), 0x63, 0x1C, 0x62, byte(DrumNr & 0x7f), 0x06,byte (Pan & 0x7f)};
 this->fluxWrite(command, 7);
}

void Fluxamasynth::setDrumReverb( byte Channel, byte DrumNr, byte Reverb ) //NEW !!!
{
    //DrumNr &= 0x7F;
    //NRPN_Control( Channel, 0x1D, DrumNr, Reverb );
    //NRPN 1Drrh Bnh 63h 1Dh 62h rr 06h vv Reverb send level of drum instrument note rr (vv=00 to 7Fh) 
    byte command[7] = {byte(0xb0 | (byte(Channel & 0x0f))), 0x63, 0x1D, 0x62,byte (DrumNr & 0x7f), 0x06, byte(Reverb & 0x7f)};
    this->fluxWrite(command, 7);
}

void Fluxamasynth::setDrumChorus( byte Channel, byte DrumNr, byte Chorus ) //NEW !!!
{
    byte b0xb0 = 0xb0 ; byte b0x0f = 0x0f ; byte b0x63 = 0x63 ; byte b0x1e = 0x1e ; byte b0x62 = 0x62 ;
    byte b0x7f = 0x7f ; byte b0x06 = 0x06 ;
    //DrumNr &= 0x7F;
    //NRPN_Control( Channel, 0x1E, DrumNr, Chorus );
  //NRPN 1Errh Bnh 63h 1Eh 62h rr 06h vv Chorus send level of drum instrument note rr(vv=00 to 7Fh)

  byte command[7] = {byte(b0xb0 |byte (Channel & b0x0f)), b0x63, b0x1e, b0x62,byte (DrumNr & b0x7f), b0x06,byte (Chorus & b0x7f)};
  this->fluxWrite(command, 7);
}

void Fluxamasynth::setDrumPitch( byte Channel, byte DrumNr, byte Semitone ) 
{
   //NRPN_Control( Channel, 0x18, DrumNr, Semitone );
  //NRPN_18rrh=  Bnh 63h 18h 62h rr 06h vv //Pitch coarse of drum instr. note rr in semitones (vv=40h -> no modif) G

byte command[7] = { byte(0xb0 | byte(Channel & 0x0f)), 0x63, 0x18, 0x62, byte(DrumNr & 0x7f), 0x06, byte(Semitone & 0x7f)};
 //byte command[7] = { (byte)0xb0  | (Channel & (byte)0x0f), (byte)0x63, (byte)0x18, (byte)0x62, (DrumNr & (byte)0x7f), (byte)0x06, (Semitone & (byte)0x7f)};
 this->fluxWrite(command, 7);
} 

void Fluxamasynth::mikealleffects() // poly = 35
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=7Fh =all on
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x7f};
this->fluxWrite(command, 9);
} 

void Fluxamasynth::nomicrevchor() // 
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=30h 
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x30};
this->fluxWrite(command, 9);
} 

 
void Fluxamasynth::nomiceq() //    no mike, reverb + chorus + EQ 33h
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=33h
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x33};
this->fluxWrite(command, 9);
} 

void Fluxamasynth::nomiceqsp() //    no mike, reverb + chorus + EQ + spatial 3bh
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=3bh
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x3b};
this->fluxWrite(command, 9);
} 


void Fluxamasynth::micrevchor() // mike  + reverb + chorus  74
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=xx
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x74};
this->fluxWrite(command, 9);
} 

 
void Fluxamasynth::miceq() //     mike + reverb + chorus + EQ  77h
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=xx
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x77};
this->fluxWrite(command, 9);
} 

void Fluxamasynth::miceqsp() //   mike + reverb + chorus + EQ + spatial 7fh
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=xx
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x7f};
this->fluxWrite(command, 9);
} 

void Fluxamasynth::resetallnomike()  // default no mike, no echo poly = 38   
{ //  NRPN 0375h : effect on/off  with polyphony impact
   //B0h 63h 37h B0h 62h 5Fh B0h 06h vv=45 =default 
byte command[9] = { 0xb0 , 0x63, 0x37, 0xb0, 0x62, 0x5f,0xb0,0x06, 0x45};
this->fluxWrite(command, 9);
} 



void Fluxamasynth::micvol( byte volume) // MIC Volume NRPN 3724h
{
    NRPN_Control( 0, 0x37, 0x24, volume );
}

// MIC input GAIN 6bits : 000000 (-27db) to 111111 (+36db) default is 011011=27d (0db)
// F0h 00h 20h 00h 00h 00h 12h 33h 77h 12h=port_adress vv3 vv2 vv1 vv0 xx F7h
// pad hi byte with && 0011 for ADC IN ING5 ING4 = vv3 
// low byte is ok = ING3 ING2 ING1 ING0 = vv2
// audio in gain and output gain are store in the same 16 bits register
// we first need to know both values in order to change it 
// need to scan both values during bootup an pass them when setting input or output gain
 // OUTPUT GAIN values = 001111 = 15d -58db => 111001 =57d  0db => 111111 = 63d =  +6db
   void Fluxamasynth::setgain( byte highreg, byte lowreg  )  // write full register containing in gain (high reg on 6bits) AND out gain
{
 highreg = highreg & 0b00111111 ; //ADCMUTE = 0 INMUTE = 0
 lowreg =  lowreg  | 0b01000000; // force bit 6 to high : DACSEL = 1
 lowreg =  lowreg  & 0b01111111 ; //force bit 7 to low  : DACMUTE = 0 

 byte vv3 = (HI_NIB(highreg))>>4 ;
 byte vv2 = LO_NIB(highreg) ;
 byte vv1 = (HI_NIB(lowreg))>>4 ;
 byte vv0 = LO_NIB(lowreg) ;

 byte command[16] =   { 0xf0, 0x00, 0x20, 0x00, 0x00, 0x00, 0x12, 0x33, 0x77, 0x12,vv3,  vv2,  vv1, vv0,  0x01, 0xf7 };
 //byte command[16] = { 0xf0, 0x00, 0x20, 0x00, 0x00, 0x00, 0x12, 0x33, 0x77, 0x12,0x01, 0x0b, 0x07,0x09, 0x01, 0xf7 }; // default
 this->fluxWrite(command, 16);
}