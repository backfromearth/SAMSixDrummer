/*========================================================================*//**

    @file PgmChange.h
    @brief Defined values for Program Change messages for the ATSAM2195
    @author R.McGinnis
    @author L.Nystrom
    
*//* CHANGE LOG ===============================================================

    R.McGinnis  4/2011  (begun)
    L.Nystrom   5/2012  Changed bank prefixes to GM_ and MT32_
       -"-       -"-    Minor modifications to some GM patch names.
       -"-       -"-    Corrected all patch nr, MIDI data is 0-127, not 1-128.
       -"-       -"-    Changed MT32 patch id's to use original MT-32 names.
    L.Nystrom   5/2012  Added drum identifiers for all 5 drum sets.
       -"-       -"-    Removed min/max volume/velocity id's don't belong here.

=============================================================================*/



#ifndef _PgmChange_h_inc_
#define _PgmChange_h_inc_













#define PATCH_BANK0     0
#define PATCH_BANK1     127

//-----------------------------------------------------------------------------
// General Midi Patch Bank. Program Change: Bank 0
//-----------------------------------------------------------------------------
//! @name General Midi Patch Bank.
//! Program Change: Bank 0
//@{
#define BANK_GM         PATCH_BANK0
//@}
//! @name Piano sounds
//@{
#define GM_GrandPiano           0 
#define GM_BrightPiano          1 
#define GM_ElecGrandPiano       2 
#define GM_HonkyTonkPiano       3 
#define GM_ElecPiano1           4 
#define GM_ElecPiano2           5 
#define GM_Harpsichord          6 
#define GM_Clavinet             7 
//@}
//! @name Chromatic Percussion
//@{
#define GM_Celesta              8 
#define GM_Glockenspiel         9
#define GM_MusicBox             10 
#define GM_Vibraphone           11 
#define GM_Marimba              12 
#define GM_Xylophone            13 
#define GM_TubularBells         14 
#define GM_Dulcimer             15 
//@}
//! @name Organs
//@{
#define GM_DrawbarOrgan         16 
#define GM_PercussiveOrgan      17 
#define GM_RockOrgan            18 
#define GM_ChurchOrgan          19 
#define GM_ReedOrgan            20 
#define GM_Accordion            21 
#define GM_Harmonica            22 
#define GM_TangoAccordion       23 
//@}
//! @name Guitars
//@{
#define GM_AcouGuitarNylon      24 
#define GM_AcouGuitarSteel      25 
#define GM_JazzGuitar           26 
#define GM_CleanGuitar          27
#define GM_MutedGuitar          28
#define GM_OverdriveGuitar      29
#define GM_DistGuitar           30
#define GM_GuitarHarmon         31
//@}
//! @name Basses
//@{
#define GM_AcousticBass         32
#define GM_FingerBass           33
#define GM_PickedBass           34
#define GM_FretlessBass         35
#define GM_SlapBass1            36
#define GM_SlapBass2            37
#define GM_SynthBass1           38
#define GM_SynthBass2           39
//@}
//! @name Strings
//@{
#define GM_Violin               40
#define GM_Viola                41
#define GM_Cello                42
#define GM_Contrabass           43
#define GM_TremoloStrings       44
#define GM_PizzicatoStrings     45
#define GM_OrchestralHarp       46
#define GM_Timpani              47
//@}
//! @name Ensembles
//@{
#define GM_StringEnsemble1      48
#define GM_StringEnsemble2      49
#define GM_SynthStrings1        50
#define GM_SynthStrings2        51
#define GM_ChoirAahs            52
#define GM_VoiceOohs            53
#define GM_SynthVoice           54
#define GM_OrchestraHit         55
//@}
//! @name Brass
//@{
#define GM_Trumpet              56
#define GM_Trombone             57
#define GM_Tuba                 58
#define GM_MutedTrumpet         59
#define GM_FrenchHorn           60
#define GM_BrassSection         61
#define GM_SynthBrass1          62
#define GM_SynthBrass2          63
//@}
//! @name Reeds
//@{
#define GM_SopranoSax           64
#define GM_AltoSax              65
#define GM_TenorSax             66
#define GM_BaritoneSax          67
#define GM_Oboe                 68
#define GM_EnglishHorn          69
#define GM_Bassoon              70
#define GM_Clarinet             71
//@}
//! @name Pipes
//@{
#define GM_Piccolo              72
#define GM_Flute                73
#define GM_Recorder             74
#define GM_PanFlute             75
#define GM_BlownBottle          76
#define GM_Shakuhachi           77
#define GM_Whistle              78
#define GM_Ocarina              79
//@}
//! @name Synth Leads
//@{
#define GM_Lead_Square          80
#define GM_Lead_Sawtooth        81
#define GM_Lead_Calliope        82
#define GM_Lead_Chiff           83
#define GM_Lead_Charang         84
#define GM_Lead_Voice           85
#define GM_Lead_Fifths          86
#define GM_Lead_BassLead        87
//@}
//! @name Synth Pads
//@{
#define GM_Pad_NewAge           88
#define GM_Pad_Warm             89
#define GM_Pad_Polysynth        90
#define GM_Pad_Choir            91
#define GM_Pad_Bowed            92
#define GM_Pad_Metallic         93
#define GM_Pad_Halo             94
#define GM_Pad_Sweep            95
//@}
//! @name Synth Effects
//@{
#define GM_FX_Rain              96
#define GM_FX_Soundtrack        97
#define GM_FX_Crystal           98
#define GM_FX_Atmosphere        99
#define GM_FX_Brightness        100
#define GM_FX_Goblins           101
#define GM_FX_Echoes            102
#define GM_FX_SciFi             103
//@}
//! @name Ethnic instruments
//@{
#define GM_Sitar                104
#define GM_Banjo                105
#define GM_Shamisen             106
#define GM_Koto                 107
#define GM_Kalimba              108
#define GM_Bagpipe              109
#define GM_Fiddle               110
#define GM_Shanai               111
//@}
//! @name Percussive instruments
//@{
#define GM_TinkleBell           112
#define GM_Agogo                113
#define GM_SteelDrums           114
#define GM_Woodblock            115
#define GM_TaikoDrum            116
#define GM_MelodicTom           117
#define GM_SynthDrum            118
#define GM_ReverseCymbal        119
//@}
//! @name Sound Effects
//@{
#define GM_GuitarFretNoise      120
#define GM_BreathNoise          121
#define GM_Seashore             122
#define GM_BirdTweet            123
#define GM_TelephoneRing        124
#define GM_Helicopter           125
#define GM_Applause             126
#define GM_Gunshot              127
//@}

//-----------------------------------------------------------------------------
// Emulated Roland MT-32 Patch Bank. Program Change: Bank 127
//-----------------------------------------------------------------------------
//! @name Emulated Roland MT-32 Patch Bank.
//! Program Change: Bank 127
//@{
#define BANK_MT32       PATCH_BANK1
//@}
//! @name MT-32 Sounds
//@{
#define MT32_AcouPiano1     0
#define MT32_AcouPiano2     1
#define MT32_AcouPiano3     2
#define MT32_ElecPiano1     3
#define MT32_ElecPiano2     4
#define MT32_ElecPiano3     5
#define MT32_ElecPiano4     6
#define MT32_Honky_Tonk     7
#define MT32_ElecOrg1       8
#define MT32_ElecOrg2       9
#define MT32_ElecOrg3       10
#define MT32_ElecOrg4       11
#define MT32_PipeOrg1       12
#define MT32_PipeOrg2       13
#define MT32_PipeOrg3       14
#define MT32_Accordion      15
#define MT32_Harpsi1        16
#define MT32_Harpsi2        17
#define MT32_Harpsi3        18
#define MT32_Clavi1         19
#define MT32_Clavi2         20
#define MT32_Clavi3         21
#define MT32_Celesta1       22
#define MT32_Celesta2       23
#define MT32_SynBrass1      24
#define MT32_SynBrass2      25
#define MT32_SynBrass3      26
#define MT32_SynBrass4      27
#define MT32_SynBass1       28
#define MT32_SynBass2       29
#define MT32_SynBass3       30
#define MT32_SynBass4       31
#define MT32_Fantasy        32
#define MT32_HarmoPan       33
#define MT32_Chorale        34
#define MT32_Glasses        35
#define MT32_Soundtrack     36
#define MT32_Atmosphere     37
#define MT32_WarmBell       38
#define MT32_FunnyVox       39
#define MT32_EchoBell       40
#define MT32_IceRain        41
#define MT32_Oboe2001       42
#define MT32_EchoPan        43
#define MT32_DrSolo         44
#define MT32_Schooldaze     45
#define MT32_Bellsinger     46
#define MT32_SquareWave     47
#define MT32_StrSect1       48
#define MT32_StrSect2       49
#define MT32_StrSect3       50
#define MT32_Pizzicato      51
#define MT32_Violin1        52
#define MT32_Violin2        53
#define MT32_Cello1         54
#define MT32_Cello2         55
#define MT32_Contrabass     56
#define MT32_Harp1          57
#define MT32_Harp2          58
#define MT32_Guitar1        59
#define MT32_Guitar2        60
#define MT32_ElecGtr1       61
#define MT32_ElecGtr2       62
#define MT32_Sitar          63
#define MT32_AcouBass1      64
#define MT32_AcouBass2      65
#define MT32_ElecBass1      66
#define MT32_ElecBass2      67
#define MT32_SlapBass1      68
#define MT32_SlapBass2      69
#define MT32_Fretless1      70
#define MT32_Fretless2      71
#define MT32_Flute1         72
#define MT32_Flute2         73
#define MT32_Piccolo1       74
#define MT32_Piccolo2       75
#define MT32_Recorder       76
#define MT32_PanPipes       77
#define MT32_Sax1           78
#define MT32_Sax2           79
#define MT32_Sax3           80
#define MT32_Sax4           81
#define MT32_Clarinet1      82
#define MT32_Clarinet2      83
#define MT32_Oboe           84
#define MT32_EnglHorn       85
#define MT32_Bassoon        86
#define MT32_Harmonica      87
#define MT32_Trumpet1       88
#define MT32_Trumpet2       89
#define MT32_Trombone1      90
#define MT32_Trombone2      91
#define MT32_FrHorn1        92
#define MT32_FrHorn2        93
#define MT32_Tuba           94
#define MT32_BrsSect1       95
#define MT32_BrsSect2       96
#define MT32_Vibe1          97
#define MT32_Vibe2          98
#define MT32_SynMallet      99
#define MT32_WindBell       100
#define MT32_Glock          101
#define MT32_TubeBell       102
#define MT32_Xylophone      103
#define MT32_Marimba        104
#define MT32_Koto           105
#define MT32_Sho            106
#define MT32_Shakuhachi     107
#define MT32_Whistle1       108
#define MT32_Whistle2       109
#define MT32_BottleBlow     110
#define MT32_BreathPipe     111
#define MT32_Timpani        112
#define MT32_MelodicTom     113
#define MT32_DeepSnare      114
#define MT32_ElecPerc1      115
#define MT32_ElecPerc2      116
#define MT32_Taiko          117
#define MT32_TaikoRim       118
#define MT32_Cymbal         119
#define MT32_Castanets      120
#define MT32_Triangle       121
#define MT32_OrcheHit       122
#define MT32_Telephone      123
#define MT32_BirdTweet      124
#define MT32_OneNoteJam     125
#define MT32_WaterBells     126
#define MT32_JungleTune     127
//@}

//-----------------------------------------------------------------------------
// Drums (channel 10, i.e 9)
//-----------------------------------------------------------------------------

//! @name Drum Sets
//! Program Change numbers for the drum channel.
//@{

#define DRUM_CHAN           9   //!< Standard drum channel (a.k.a. CH10).

#define DRUMS_Std           0   //!< Standard drum set.
#define DRUMS_Power         16  //!< Power drum set.
#define DRUMS_Brush         40  //!< Jazz drum set.
#define DRUMS_Orchestra     48  //!< Orchestral percussion.
#define DRUMS_CM64          127 //!< CM-32/64 drum set (partial).

//@}
//! @name Standard drumset (0)
//! Sounds with the same Exc# number are mutually exclusive.
//@{

#define DR_Kick2            35  //!< Keyboard position = B1
#define DR_Kick             36  // Keyboard position = C2
#define DR_SideStick        37  // Keyboard position = et.c..
#define DR_Snare            38
#define DR_HandClap         39
#define DR_Snare2           40
#define DR_LowFloorTom      41
#define DR_ClosedHiHat      42  //!< Exc.1
#define DR_HighFloorTom     43
#define DR_PedalHiHat       44  //!< Exc.1
#define DR_LowTom           45
#define DR_OpenHiHat        46  //!< Exc.1
#define DR_LowMidTom        47
#define DR_HighMidTom       48
#define DR_CrashCymbal      49
#define DR_HighTom          50
#define DR_RideCymbal       51
#define DR_ChineseCymbal    52
#define DR_RideBell         53
#define DR_Tambourine       54
#define DR_SplashCymbal     55
#define DR_Cowbell          56
#define DR_CrashCymbal2     57
#define DR_VibraSlap        58
#define DR_RideCymbal2      59
#define DR_HighBongo        60
#define DR_LowBongo         61
#define DR_MuteHighConga    62
#define DR_OpenHighConga    63
#define DR_LowConga         64
#define DR_HighTimbale      65
#define DR_LowTimbale       66
#define DR_HighAgogo        67
#define DR_LowAgogo         68
#define DR_Cabasa           69
#define DR_Maracas          70
#define DR_ShortWhistle     71  //!< Exc.2
#define DR_LongWhistle      72  //!< Exc.2
#define DR_ShortGuiro       73  //!< Exc.3
#define DR_LongGuiro        74  //!< Exc.3
#define DR_Claves           75
#define DR_HighWoodBlock    76
#define DR_LowWoodBlock     77
#define DR_MuteCuica        78  //!< Exc.4
#define DR_OpenCuica        79  //!< Exc.4
#define DR_MuteTriangle     80  //!< Exc.5
#define DR_OpenTriangle     81  //!< Exc.5

//@}
//! @name Power set (16)
//! Includes a gated snare sound.\n
//! Undeclared drum nr's have the same sound as the std set (0)
//@{

#define DRP_GatedSnare      38  // Overrides Snare 1

//@}
//! @name Brush (Jazz) drumset (40)
//! Includes brush stick sounds.\n
//! Undeclared drum nr's have the same sound as the std set (0)
//@{

#define DRB_JazzBassDrum2   35  // Overrides Kick2
#define DRB_JazzBassDrum    36  // Overrides Kick
#define DRB_BrushTap        38  // Overrides Snare
#define DRB_BrushSlap       39  // Overrides HandClap
#define DRB_BrushSwirl      40  // Overrides Snare2

//@}
//! @name Orchestral percussion (48)
//! Includes tuned orchestral Timpani.\n
//! Undeclared drum nr's have the same sound as the std set (0)
//@{

#define DRO_ClosedHiHat     27  // Exc.1
#define DRO_PedalHiHat      28  // Exc.1
#define DRO_OpenHiHat       29  // Exc.1
#define DRO_RideCymbal      30
#define DRO_SnareDrum       38
#define DRO_Castanets       39
#define DRO_SnareDrum2      40
#define DRO_Timpani_F       41
#define DRO_Timpani_Fs      42
#define DRO_Timpani_G       43
#define DRO_Timpani_Gs      44
#define DRO_Timpani_A       45
#define DRO_Timpani_As      46
#define DRO_Timpani_B       47
#define DRO_Timpani_C       48
#define DRO_Timpani_Cs      49
#define DRO_Timpani_D       50
#define DRO_Timpani_Ds      51
#define DRO_Timpani_E       52
#define DRO_Timpani_F2      53
#define DRO_Applause        88

//@}
//! @name CM-64/32 [Partial] (127)
//! Percussion 60-72 have the same sound as the std set (0).\n
//! Other drum nr's have no sound.
//@{

#define DRM_Kick2           35
#define DRM_Kick            36
#define DRM_RimShot         37
#define DRM_Snare           38
#define DRM_HandClap        39
#define DRM_ElecSnare       40
#define DRM_AcoustLowTom    41
#define DRM_ClosedHiHat     42  // Exc.1
#define DRM_AcoustLowTom2   43
#define DRM_OpenHiHat2      44
#define DRM_AcoustMidTom    45
#define DRM_OpenHiHat       46  // Exc.1
#define DRM_AcoustMidTom2   47
#define DRM_AcoustHighTom   48
#define DRM_CrashCymbal     49
#define DRM_AcoustHighTom2  50
#define DRM_RideCymbal      51
#define DRM_Tambourine      54
#define DRM_Cowbell         56
#define DRM_VibraSlap       73
#define DRM_Claves          75
#define DRM_Applause        82
#define DRM_Helicopter      94
#define DRM_GunShot         96
#define DRM_Birds           102
#define DRM_SeaShore        106










//@}
#endif //ndef _PgmChange_h_inc_