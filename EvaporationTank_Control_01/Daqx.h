//---------------------------------------------------------------------------
// DAQX.H
//
//   Header for the Enhanced API Data Acquisition Library.
///
//---------------------------------------------------------------------------

#ifndef DAQ_H
#define DAQ_H

#ifdef   __cplusplus
extern "C" {
#endif

#if !defined(_DAQAPI32_)
   #define  DAQAPI   __declspec(dllimport)
#else
   #define  DAQAPI
#endif

/*****************************************************************/
/*                                                               */
/*    Initialization/Error Handling Definitions and Prototypes   */
/*                                                               */
/*****************************************************************/

/* Error Code Definitions */
typedef enum {
   DerrNoError          = 0x00,  /* 0 - No error. */
   DerrBadChannel       = 0x01,  /* 1 - Specified LPT channel was out of range. */
   DerrNotOnLine        = 0x02,  /* 2 - Requested device is not on line. */
   DerrNoDaqbook        = 0x03,  /* 3 - DAQBook is not on the requested channel. */
   DerrBadAddress       = 0x04,  /* 4 - Bad function address. */
   DerrFIFOFull         = 0x05,  /* 5 - FIFO Full detected, possible data corruption. */
   DerrBadDma           = 0x06,  /* 6 - Bad or illegal DMA channel or mode specified for device */
   DerrBadInterrupt     = 0x07,  /* 7 - Bad or illegal INTERRUPT level specified for device */
   DerrDmaBusy          = 0x08,  /* 8 - DMA is currently being used */

   DerrInvChan          = 0x10,  /* 16 - Invalid analog input channel */
   DerrInvCount         = 0x11,  /* 17 - Invalid count parameter */
   DerrInvTrigSource    = 0x12,  /* 18 - Invalid trigger source parameter */
   DerrInvLevel         = 0x13,  /* 19 - Invalid trigger level parameter */
   DerrInvGain          = 0x14,  /* 20 - Invalid channel gain parameter */
   DerrInvDacVal        = 0x15,  /* 21 - Invalid DAC output parameter */
   DerrInvExpCard       = 0x16,  /* 22 - Invalid expansion card parameter */
   DerrInvPort          = 0x17,  /* 23 - Invalid port parameter */
   DerrInvChip          = 0x18,  /* 24 - Invalid chip parameter */
   DerrInvDigVal        = 0x19,  /* 25 - Invalid digital output parameter */
   DerrInvBitNum        = 0x1a,  /* 26 - Invalid bit number parameter */
   DerrInvClock         = 0x1b,  /* 27 - Invalid clock parameter */
   DerrInvTod           = 0x1c,  /* 28 - Invalid time-of-day parameter */
   DerrInvCtrNum        = 0x1d,  /* 29 - Invalid counter number */
   DerrInvCntSource     = 0x1e,  /* 30 - Invalid counter source parameter */
   DerrInvCtrCmd        = 0x1f,  /* 31 - Invalid counter command parameter */
   DerrInvGateCtrl      = 0x20,  /* 32 - Invalid gate control parameter */
   DerrInvOutputCtrl    = 0x21,  /* 33 - Invalid output control parameter */
   DerrInvInterval      = 0x22,  /* 34 - Invalid interval parameter */
   DerrTypeConflict     = 0x23,  /* 35 - An integer was passed to a function requiring a character */
   DerrMultBackXfer     = 0x24,  /* 36 - A second transfer was requested */
   DerrInvDiv           = 0x25,  /* 37 - Invalid Fout divisor */

   /* Temperature Conversion Errors */
   DerrTCE_TYPE         = 0x26,  /* 38 - TC type out-of-range */
   DerrTCE_TRANGE       = 0x27,  /* 39 - Temperature out-of-CJC range */
   DerrTCE_VRANGE       = 0x28,  /* 40 - Voltage out-of-TC-range */
   DerrTCE_PARAM        = 0x29,  /* 41 - Unspecified TC parameter value error */
   DerrTCE_NOSETUP      = 0x2a,  /* 42 - daqCvtTCConvert called before daqCvtTCSetup */

   /* Daqbook  */
   DerrNotCapable       = 0x2b,  /* 43 - Device is incapable of function */

   /* background */
   DerrOverrun          = 0x2c,  /* 44 - A buffer overrun occurred */

   /* Zero and Cal Conversion Errors */
   DerrZCInvParam       = 0x2d,  /* 45 - Unspecified Cal parameter value error */
   DerrZCNoSetup        = 0x2e,  /* 46 - daqCalConvert called before daqCalSetup */
   DerrInvCalFile       = 0x2f,  /* 47 - Cannot open the specified cal file */

   /* environmental errors */
   DerrMemLock          = 0x30,  /* 48 - Cannot lock allocated memory from operating system */
   DerrMemHandle        = 0x31,  /* 49 - Cannot get a memory handle from operating system */

   /* Pre-trigger acquisition errors */
   DerrNoPreTActive     = 0x32,  /* 50 - No pre-trigger configured */

   /* Dac FIFO errors (DaqBoard only) */
   DerrInvDacChan       = 0x33,  /* 51 - DAC channel does not exist */
   DerrInvDacParam      = 0x34,  /* 52 - DAC parameter invalid */
   DerrInvBuf           = 0x35,  /* 53 - Buffer points to NULL or buffer size is zero */
   DerrMemAlloc         = 0x36,  /* 54 - Could not allocate the needed memory */
   DerrUpdateRate       = 0x37,  /* 55 - Could not achieve the specified update rate */
   DerrInvDacWave       = 0x38,  /* 56 - Could not start DAC waveforms because of missing or invalid parameters*/
   DerrInvBackDac       = 0x39,  /* 57 - Could not start DAC waveforms with background transfers*/
   DerrInvPredWave      = 0x3a,  /* 58 - Predefined DAC waveform not supported */

   /* RTD Conversion Errors */
   DerrRtdValue         = 0x3b,  /* 59 - rtdValue out of range */
   DerrRtdNoSetup       = 0x3c,  /* 60 - rtdConvert called before rtdSetup */
   DerrRtdTArraySize    = 0x3d,  /* 61 - RTD Temperature array not large enough */
   DerrRtdParam         = 0x3e,  /* 62 - Incorrect RTD parameter */

   DerrInvBankType      = 0x3f,  /* 63 - Invalid bank type specified */
   DerrBankBoundary     = 0x40,  /* 64 - Simultaneous writes to Dbk cards in different banks not allowed */

   DerrInvFreq          = 0x41,  /* 65 - Invalid scan frequency specified */
   DerrNoDaq            = 0x42,  /* 66 - No Daq 112B/216B device installed */

   DerrInvOptionType    = 0x43,  /* 67 - Invalid option type parameter */
   DerrInvOptionValue   = 0x44,  /* 68 - Invalid option value parameter */

   // NEW API ERROR CODES
   DerrTooManyHandles   = 0x60,  /* 96 - No more handles available to open */
   DerrInvLockMask      = 0x61,  /* 97 - Only a part of the resource is already locked, must be all or none */
   DerrAlreadyLocked    = 0x62,  /* 98 - All or part of the resource was locked by another application */

   DerrAcqArmed         = 0x63,  /* 99 - Operation not available while an acquisition is armed */
   DerrParamConflict    = 0x64,  /* 100 - Each parameter is valid, but the combination is invalid */
   DerrInvMode          = 0x65,  /* 101 - Invalid acquisition mode */
   DerrInvOpenMode      = 0x66,  /* 102 - Invalid file open mode */

   DerrFileOpenError    = 0x67,  /* 103 - Unable to open file */
   DerrFileWriteError   = 0x68,  /* 104 - Unable to write file */
   DerrFileReadError    = 0x69,  /* 105 - Unable to read file */

   DerrInvClockSource   = 0x6a,  /* 106 - Invalid clock source */
   DerrInvEvent         = 0x6b,  /* 107 - Invalid transfer event */

   DerrTimeout          = 0x6c,  /* 108 - Timeout on wait */
   DerrInitFailure      = 0x6d,  /* 109 - Unexpected result occurred while initializing the hardware */
   DerrBufTooSmall      = 0x6e,  /* 110 - Array too small */
   DerrInvType          = 0x6f,  /* 111 - Invalid Dac Device type or info type */

   DerrArraySize        = 0x70,  /* 112 - Array not large enough */
   DerrBadAlias         = 0x71,  /* 113 - Invalid alias names for VXD lookup */
   DerrInvCommand       = 0x72,  /* 114 - Invalid comamnd */
   DerrInvHandle        = 0x73,  /* 115 - Invalid handle */

   DerrNoTransferActive = 0x74,  /* 116 - Transfer not active */
   DerrNoAcqActive      = 0x75,  /* 117 - Acquisition not active */
   DerrInvOpstr         = 0x76,  /* 118 - Invalid op string used for enhanced triggering */
   DerrDspCommFailure   = 0x77,  /* 119 - Device with DSP failed communication */
   DerrEepromCommFailure= 0x78,  /* 120 - Device with Eeprom failed communication */
   DerrInvEnhTrig       = 0x79,  /* 121 - Device using enhanced trigger detected invalid trig type */
   DerrInvCalConstant   = 0x7A,  /* 122 - User calibration constant out of range */
   DerrInvErrorCode     = 0x7B,  /* 123 - Invalid error code */
   DerrInvAdcRange      = 0x7C,  /* 124 - Invalid analog input voltage range parameter */
   DerrInvCalTableType  = 0x7D,  /* 125 - Invalid calibration table type */
   DerrInvCalInput      = 0x7E,  /* 126 - Invalid calibration input signal selection */
   DerrInvRawDataFormat = 0x7F,  /* 127 - Invalid raw data format selection */
   DerrNotImplemented   = 0x80,  /* 128 - Feature/function not implemented yet */
   DerrInvDioDeviceType = 0x81,  /* 129 - Invalid digital I/O device type */
   DerrInvPostDataFormat= 0x82,  /* 130 - Invalid post-processing data format selection */
   DerrDaqStalled       = 0x83,  /* 131 - Low level driver stalled */
   DerrDaqLostPower     = 0x84,  /* 132 - Daq Device has lost power */
   DerrDaqMissing       = 0x85,  /* 133 - Daq Device is missing */
   DerrScanConfig       = 0x86,  /* 134 - Invalid channel scan config */
   DerrInvTrigSense     = 0x87,  /* 135 - Invalid trigger sense parameter */
   DerrInvTrigEvent     = 0x88,  /* 136 - Invalid trigger event parameter */
   DerrInvTrigChannel   = 0x89,  /* 137 - Trigger channel not in scan */
   
   DerrDacWaveformNotActive = 0x8A,  /* 138 - DAC waveform output not active */
   DerrDacWaveformActive    = 0x8B,  /* 139 - DAC waveform output already active */
   DerrDacNotEnoughMemory   = 0x8C,  /* 140 - DAC static waveforms exceed maximum length */
   DerrDacBuffersNotEqual   = 0x8D,  /* 141 - DAC static waveforms unequal length */
   DerrDacBufferTooSmall    = 0x8E,  /* 142 - DAC dynamic waveform buffer too small */
   DerrDacBufferUnderrun    = 0x8F,  /* 143 - DAC dynamic waveform buffer underrun */
   DerrDacPacerOverrun      = 0x90,  /* 144 - DAC pacer overrun */  
   DerrAdcPacerOverrun      = 0x91,  /* 145 - ADC pacer overrun */
   DerrAdcNotReady          = 0x92,  /* 146 - ADC not ready */
   DerrArbitrationFailure   = 0x93,  /* 147 - Internal bus arbitration error */
   DerrDacWaveFileTooSmall  = 0x94,  /* 148 - DAC waveform file too small for requested use */
   DerrDacWaveFileUnderrun  = 0x95,  /* 149 - DAC waveform file buffer underrun */
   DerrDacWaveModeConflict  = 0x96,  /* 150 - DAC waveform mode, buffer, or source conflict */
   DerrTedsInfoInvStatus    = 0x97,  /* 151 - No TEDs available */

   DerrAlreadyCreated       = 0x98,  /* 152 - Device to create already exists */
   DerrParentNotCreated     = 0x99,  /* 153 - Parent device of the device to create does not exist */
   DerrUnableToCreateDevice = 0x9a,  /* 154 - Unable to create the spcified device */
   DerrUnableToDeleteDevice = 0x9b,  /* 155 - Unable to delete the spcified device */
   DerrInvIPAddress         = 0x9c,  /* 156 - Invalid IP address */
   DerrOutputEnableConflict = 0x9d,  /* 157 - DaqBoard500 Timer1/AdcClockOutput are in conflict*/

} DaqError;

/* Handle Type Definition */
typedef  INT   DaqHandleT;

/* Initialization and Locking Prototypes */
DAQAPI DaqHandleT WINAPI   daqOpen(LPSTR daqName);
DAQAPI DaqError WINAPI     daqClose(DaqHandleT handle);
DAQAPI DaqError WINAPI     daqOnline(DaqHandleT handle, PBOOL online);

/* Error Handler Type Definitions */
typedef VOID CALLBACK      DaqErrorHandlerFT(DaqHandleT handle, DaqError errCode);
typedef DaqErrorHandlerFT  *DaqErrorHandlerFPT;

/* Error Handler Function Prototypes */
DAQAPI DaqError WINAPI  daqSetDefaultErrorHandler(DaqErrorHandlerFPT handler);
DAQAPI DaqError WINAPI  daqSetErrorHandler(DaqHandleT handle, DaqErrorHandlerFPT handler);
DAQAPI DaqError WINAPI  daqProcessError(DaqHandleT handle, DaqError errCode);
DAQAPI DaqError WINAPI  daqGetLastError(DaqHandleT handle, DaqError *errCode);
DAQAPI VOID CALLBACK    daqDefaultErrorHandler(DaqHandleT handle, DaqError errCode);
DAQAPI DaqError WINAPI  daqFormatError(DaqError errorNum, PCHAR msg);

/*****************************************************************/
/*                                                               */
/*        Event Handling Definitions and Prototypes              */
/*                                                               */
/*****************************************************************/

/* Transfer Event Definitions */
typedef enum {
   DteAdcData           = 0,
   DteAdcDone           = 1,
   DteDacData           = 2,  // unsupported
   DteDacDone           = 3,  // unsupported
   DteIOData            = 4,  // unsupported
   DteIODone            = 5,  // unsupported
} DaqTransferEvent;

/* Transfer Event Wait Mode Definitions */
typedef enum {
   DwmNoWait            = 0,
   DwmWaitForAny        = 1,
   DwmWaitForAll        = 2,
} DaqWaitMode;

/*  Event Handling Function Prototypes */
DAQAPI DaqError WINAPI  daqSetTimeout(DaqHandleT handle, DWORD mSecTimeout);
DAQAPI DaqError WINAPI  daqWaitForEvent(DaqHandleT handle, DaqTransferEvent event);
DAQAPI DaqError WINAPI  daqWaitForEvents(DaqHandleT *handles, DaqTransferEvent *events,
                           DWORD eventCount, BOOL *eventSet, DaqWaitMode waitMode);


/*****************************************************************/
/*                                                               */
/*        Hardware Information Definitions and Prototypes        */
/*                                                               */
/*****************************************************************/

// Hardware Information Selector Definitions (daqGetHardwareInfo)
typedef enum {
   DhiHardwareVersion   = 0,    // Daq Instrument Type
   DhiProtocol          = 1,    // Comm protocol
//   DhiAdcBits           = 2,  // not implemented
   DhiADmin             = 3,    // ADC Output Low Range
   DhiADmax             = 4,    // ADC Output High Range
//   DhiAdcBlockSize      = 5,  // not implemented
//   DhiDacBlockSize      = 6,  // not implemented
//   DhiIOBlockSize       = 7,  // not implemented
//   DhiMainChInfo        = 8,  // not implemented
//   DhiOptionChInfo      = 9,  // not implemented
//   DhiChanCount         = 10, // not implemented
   DhiHardwareSubType   = 11,   // Daq Instrument Sub Type
} DaqHardwareInfo;

/* General Information Selector Definitions */
typedef enum {
   DdiHardwareVersionInfo      = 0, // (DaqHardwareVersion)
   DdiProtocolInfo             = 1,
   DdiChTypeInfo               = 2,
   DdiChOptionTypeInfo         = 3,
   DdiADminInfo                = 4, // ADC Minimum voltage input level in Volts (FLOAT)
   DdiADmaxInfo                = 5, // ADC Maximum voltage input level in Volts (FLOAT)
   DdiChanCountInfo            = 6,
   DdiNVRAMDateInfo            = 7, // Date String
   DdiNVRAMTimeInfo            = 8, // Time String
   DdiDbk4MaxFreqInfo          = 9,
   DdiDbk4SetBaselineInfo      = 10,
   DdiDbk4ExcitationInfo       = 11,
   DdiDbk4ClockInfo            = 12,
   DdiDbk4GainInfo             = 13, /* internally used by daqAdcSetScan */
   DdiDbk7SlopeInfo            = 14,
   DdiDbk7DebounceTimeInfo     = 15,
   DdiDbk7MinFreqInfo          = 16,
   DdiDbk7MaxFreqInfo          = 17,
   DdiDbk55SlopeInfo           = DdiDbk7SlopeInfo,
   DdiDbk55DebounceTimeInfo    = DdiDbk7DebounceTimeInfo,
   DdiDbk55MinFreqInfo         = DdiDbk7MinFreqInfo,
   DdiDbk55MaxFreqInfo         = DdiDbk7MaxFreqInfo,
   DdiDbk50GainInfo            = 18, /* internally used by daqAdcSetScan */
   DdiWbk12FilterCutOffInfo    = 19,
   DdiWbk12FilterTypeInfo      = 20,
   DdiWbk12FilterModeInfo      = 21,
   DdiWbk12PreFilterModeInfo   = 22,
   DdiWbk13FilterCutOffInfo    = 23,
   DdiWbk13FilterTypeInfo      = 24,
   DdiWbk13FilterModeInfo      = 25,
   DdiWbk13PreFilterModeInfo   = 26,
   DdiWbk14LowPassModeInfo     = 27,
   DdiWbk14LowPassCutOffInfo   = 28,
   DdiWbk14HighPassCutOffInfo  = 29,
   DdiWbk14CurrentSrcInfo      = 30,
   DdiWbk14PreFilterModeInfo   = 31,
   DdiWbk14ExcSrcWaveformInfo  = 32,
   DdiWbk14ExcSrcFreqInfo      = 33,
   DdiWbk14ExcSrcAmplitudeInfo = 34,
   DdiWbk14ExcSrcOffsetInfo    = 35,
   DdiWgcX1Info                = 36,
   DdiWgcX2Info                = 37,
   DdiWgcX5Info                = 38,
   DdiWgcX10Info               = 39,
   DdiWgcX20Info               = 40,
   DdiWgcX50Info               = 41,
   DdiWgcX100Info              = 42,
   DdiWgcX200Info              = 43,
   DdiPreTrigFreqInfo          = 44,
   DdiPostTrigFreqInfo         = 45,
   DdiPreTrigPeriodInfo        = 46,
   DdiPostTrigPeriodInfo       = 47,
   DdiOptNVRAMDateInfo         = 48,
   DdiOptNVRAMTimeInfo         = 49,
   DdiExtFeatures              = 50, // DaqHardwareExtFeatures
   DdipDaqCalibrationTime      = 50, // Personal Daq initial calibration period in ms 
   DdiFifoSize                 = 51, // FIFO capacity in WORD's of data.
   DdiFifoCount                = 52, // Count of WORD's of data currently in the FIFO
   DdiSerialNumber             = 53, // Serial Number String
   DdiAdcClockSource           = 54, // Current Clock Source
   DdiFirmwareVersion          = 55, // Firmware Version (String)
   DdiHardwareVersion          = 56, // Hardware Version (String)
   DdiDriverVersion            = 57, // Driver Version   (String)
   DdiAdcTriggerScan           = 58, // Trigger Scan Number (DWORD)          // Not Implemented
   DdiAdcPreTriggerCount       = 59, // Amount of Pre-Trigger Scans (DWORD)  // Not Implemented
   DdiAdcPostTriggerCount      = 60, // Amount of Post-Trigger Scans (DWORD) // Not Implemented
   DdiDetectSensor             = 61, // Detects the presence of an external sensor (DWORD) // WaveBook Wbk14 only
   DdiWbk12PreFilterCutOffInfo = 62, // Wbk12/Wbk12A pre-filter cutoff freq (FLOAT)
   DdiWbk12PostFilterCutOffInfo= 63, // Wbk12A post-filter cutoff freq      (FLOAT)
   DdiWbk13PreFilterCutOffInfo = 64, // Wbk13/Wbk13A pre-filter cutoff freq (FLOAT)
   DdiWbk13PostFilterCutOffInfo= 65, // Wbk13A post-filter cutoff freq      (FLOAT)
   DdiAdcLastRawTransferCount  = 66, // undocumented.
   DdiWgcX0_2Info			   = 67, // Wbk18 -25 to 25 volt range
   DdiWbk18OpenSensorStatus		=68,
   DdiWbk18PosOverRangeStatus	=69,
   DdiWbk18NegOverRangeStatus	=70,

   DdiWbk18LowPassMode			=71,  // Enumeration value: Bypass, 8 pole and 2 pole
   DdiWbk18LowPassCutOff		=72,  // Enumeration value: Filter setting
   DdiWbk18HighPassCutOff		=73,  // Enumeration value: DC,0.1Hz and 10Hz
   DdiWbk18CurrentSrc			=74,  // Enumeration value: Off and 4mA
   DdiWbk18OverRangeEnable		=75,  // option value is 8 bit mask representing the 8 channels- zero disables all
   DdiWbk18esMode				=76,  // zero,off,continuous output or Swept (sweep through up to 1280 freqs)
   DdiWbk18esFreq				=77,  // 0.01Hz - 5000Hz
   DdiWbk18esAmplitude			=78,  // Enumeration value: 7 amplitudes
   DdiWbk18esFreqCycleTime		=79,  // Swept Mode:freq-to-freq time in seconds (50uS to 85,899,000uS)
   DdiWbk18esFreqDurationTime	=80,  // Swept Mode:total time to sweep all freqs ( 50uS to 85,899,000uS)
   DdiWbk18OverRangeLimit		=81, // 1 - 100 (%)   : percentage of selected range 
   DdiWbk18esImmediate			=82, // used to reset, turn on and turn off the excitation source  
   DdiWbk18esRelay				=83, // used to specify output relay on/off 
   DdiWbk18OpenSensorStatusAll	=84, // returns an 8 bit value indicating status for all 8 channels
   DdiWbk18PosOverRangeStatusAll=85, // returns an 8 bit value indicating status for all 8 channels
   DdiWbk18NegOverRangeStatusAll=86, // returns an 8 bit value indicating status for all 8 channels
   DdiParentName				=87, // NOT USED
   DdiBaseAddress				=88, // NOT USED
   DdiWbk18TEDsStatus			=89, // returns one byte of status:	// 0=TEDs found,Operation complete
																	// 1 = Teds operation busy
																	// 2 = Operation complete, NO TEDs
																	// 4 = Channel is shorted
   DdiWbk18TEDsData				=90, // returns 34 byte array of TEDs info as follows:
									 // info[0] = status
									 // info[1] = amount of teds data available
									 // info[2] = first byte of teds data if available
									 // infor[3..33] = teds data
									
   DdiWbk18TEDsDataCount		=91, // returns the amount of teds data available
   DdiChSubTypeInfo             =92,
   DdiChOptionSubTypeInfo       =93,
   DdiETASTimeStamp             =94,
   DdiETASSenderID              =95,
   DdiETASTopologyReg           =96,
} DaqInfo;


// Detect sensor definitions
typedef enum {
   DdsTEDsFound             = 0, // sensor was not detected
   DdsTEDsBusy              = 1, // sensor was detected
   DdsTEDsNotFound          = 2, // the sensor presence or absense could not be determined
   DdsTEDsShorted			= 3, // channel is shorted
} DaqDetectTEDs;


// Detect sensor definitions
typedef enum {
   DdsNotDetected              = 0, // sensor was not detected
   DdsDetected                 = 1, // sensor was detected
   DdsIndeterminate            = 2, // the sensor presence or absense could not be determined
} DaqDetectSensor;

// Hardware Information Extended Feature Bits.  These values are returned by
// daqGetInfo when the DaqInfo type DdiExtFeatures is specified.
typedef enum {

   // WaveBook Mega-FIFO features.
   DhefFifoOverflowMode = 0x00000001, // FIFO has Overflow Protection mode.
   DhefFifoCycleMode    = 0x00000002, // FIFO has Cycle ("Finite") Mode
   DhefFifoDataCount    = 0x00000004, // FIFO has readable current-WORD's-of-data count

   // WaveBook516 features.
   DhefTrigDigPattern   = 0x00000010, // Can trigger on a digital pattern
   DhefTrigPulseInput   = 0x00000020, // Can trigger on a pulse input
   DhefAcqClkExternal   = 0x00000040, // Can pace acquisition to an external clock

   // WaveBook A series features
   DhefSyncMaster       = 0x00000100, // Can be Sync Master (drive Clock and Trigger)
   DhefSyncSlave        = 0x00000200, // Can be Sync Slave (drive Clock and Trigger)

} DaqHardwareExtFeatures;

/* Hardware Version Definitions */
typedef enum {
   DaqBook100           = 0,
   DaqBook112           = 1,
   DaqBook120           = 2,
   DaqBook200           = 3, // DaqBook/200 or DaqBook/260
   DaqBook216           = 4,
   DaqBoard100          = 5,
   DaqBoard112          = 6,
   DaqBoard200          = 7,
   DaqBoard216          = 8,
   Daq112               = 9,
   Daq216               = 10,
   WaveBook512          = 11,
   WaveBook516          = 12,
   TempBook66           = 13,
   PersonalDaq56        = 14,
   WaveBook516_250      = 15,
   WaveBook512_10V      = 16,
   DaqBoard2000         = 17,
   DaqBoard2001         = 18,
   DaqBoard2002         = 19,
   DaqBoard2003         = 20,
   DaqBoard2004         = 21,
   DaqBoard2005         = 22,
   DaqBook2000          = 23,	// DaqBook/2000A or DaqBook/2000E
   WaveBook512A         = 29,
   WaveBook516A         = 30,	// WaveBook/516A or WaveBook/516E
   WBK25				= 31,
   WBK40				= 32,
   WBK41				= 33,
   DaqBoard1000         = 34,
   DaqBoard1005         = 35,
   DaqLab2000           = 36,
   DaqScan2000          = 37,
   DaqBoard500			= 38,
   DaqBoard505			= 39,
   DaqBoard3000			= 40,
   DaqBoard3001			= 41,
   DaqBoard3005			= 42,
   UsbDaqDevice			= 43,
   PersonalDaq3000		= 44,
   ZonicPod				= 45,

} DaqHardwareVersion;

// Hardware Sub Type Definitions
typedef enum {
	// a hardware sub type of DaqSubTypeDefault
	// indicates that the hardware device type
	// is defined by the hardware version
	DaqSubTypeDefault		= 0,

	// sub types for the DaqBook2000 main type
	DaqSubTypeDaqBook2000A	= 0, // DaqBook/2000A or DaqBook/2000X
	DaqSubTypeDaqBook2000E	= 1, // DaqBook/2000E
	DaqSubTypeDaqBook2001	= 2, // DaqBook/2001
	DaqSubTypeDaqBook2020	= 3, // DaqBook/2020
	DaqSubTypeDaqBook2005	= 4, // DaqBook/2005
	DaqSubTypeES301			= 5, // ES301
	DaqSubTypeES305			= 6, // ES305
	DaqSubTypeES302			= 7, // ES302

	// sub types for the DaqLab2000 main type
	DaqSubTypeDaqLab2001	= 0, // DaqLab/2001
	DaqSubTypeDaqLab2005	= 1, // DaqLab/2005

	// sub types for the DaqScan2000 main type
	DaqSubTypeDaqScan2001	= 0, // DaqScan/2001
	DaqSubTypeDaqScan2002	= 1, // DaqScan/2002
	DaqSubTypeDaqScan2004	= 2, // DaqScan/2004
	DaqSubTypeDaqScan2005	= 3, // DaqScan/2005

	// sub types for the WaveBook516A main type
	DaqSubTypeWaveBook516A	= 0, // WaveBook/516A
	DaqSubTypeWaveBook516E	= 1, // WaveBook/516E
	DaqSubTypeZonicBook618	= 2, // ZonicBook/618
	DaqSubTypeStrainBook616 = 3, // StrainBook/616

	// sub types for the DaqBoard2000 main type
	DaqSubTypeDaqBoard2000	= 0, // DaqBoard/2000
	DaqSubTypeDaqTemp7A		= 1, // DaqTemp/7A

	// sub types for the DaqBoard2001 main type
	DaqSubTypeDaqBoard2001	= 0, // DaqBoard/2001
	DaqSubTypeDaqTemp14A	= 1, // DaqTemp/14A

	// sub types for the DaqBoard2005 main type
	DaqSubTypeDaqBoard2005	= 0, // DaqBoard/2005
	DaqSubTypeDaqTemp7		= 1, // DaqTemp/7
	DaqSubTypeDaqTemp14		= 2, // DaqTemp/14

	// sub types for the UsbDaqDevice main type
	DaqSubTypeUnknownUsbDaqDevice	= 0,
	DaqSubTypeDevelUsbDaqDevice		= 1,
	DaqSubTypeBlankUsbDaqDevice		= 2,

	// sub types for the PersonalDaq3000 main type
	DaqSubTypePersonalDaq3001	= 0,
	DaqSubTypePersonalDaq3005	= 1,

	// sub types for the ZonicPod main type
	DaqSubTypeZonicPod4	= 0,
	DaqSubTypeZonicPod8	= 1,

} DaqHardwareSubType;

/* Protocol Definitions */
typedef enum {
   DaqProtocolNone      = 0,    /* Communications not established       */
   DaqProtocol4         = 1,    /* Standard LPT Port 4-bit mode         */
   DaqProtocol8         = 2,    /* Standard LPT Port 8-bit mode         */
   DaqProtocolSMC666    = 3,    /* SMC 37C666 EPP mode                  */
   DaqProtocolFastEPP   = 4,    /* WBK20/21 Fast EPP mode               */
   DaqProtocolECP       = 5,    /* Enhanced Capability Port             */
   DaqProtocol8BitEPP   = 6,    /* 8-bit EPP mode                       */
   DaqProtocolTCPIP     = 7,    /* TCPIP (eWave)                        */
   DaqProtocolISA       = 100,  /* ISA bus card DaqBoard 100/200        */
   DaqProtocolPcCard    = 200,  /* PCCard for Daq (PCMCIA)              */
   DaqProtocolUSB       = 300,  /* USB protocol (PersonalDaq)           */
   DaqProtocolPCI       = 400,  /* PCI bus card DaqBoard 1000/2000      */
   DaqProtocolCPCI      = 500,  /* Compact PCI bus card DaqBoard 2000   */
} DaqProtocol;

typedef struct {
   char daqName[64];
} DaqDeviceListT;

// Used by daqGetDeviceProperties to get device information
typedef struct {
   DaqHardwareVersion deviceType;
   DWORD              basePortAddress;
   DWORD              dmaChannel;       /* Used for DaqBoard only */
   DWORD              socket;           /* Used for Daq PC Card only */
   DWORD              interruptLevel;
   DaqProtocol        protocol;
   char               daqName[64];
   DWORD              maxAdChannels;
   DWORD              maxDaChannels;
   DWORD              maxDigInputBits;
   DWORD              maxDigOutputBits;
   DWORD              maxCtrChannels;
   DWORD              mainUnitAdChannels;
   DWORD              mainUnitDaChannels;
   DWORD              mainUnitDigInputBits;
   DWORD              mainUnitDigOutputBits;
   DWORD              mainUnitCtrChannels;
   DWORD              adFifoSize;
   DWORD              daFifoSize;
   DWORD              adResolution;
   DWORD              daResolution;
   float              adMinFreq;
   float              adMaxFreq;
   float              daMinFreq;
   float              daMaxFreq;
} DaqDevicePropsT;

/*  Utility Function Prototypes */
DAQAPI DaqError WINAPI  daqGetDriverVersion(PDWORD version);
DAQAPI DaqError WINAPI  daqGetHardwareInfo(DaqHandleT handle, DaqHardwareInfo whichInfo, VOID * info);
DAQAPI DaqError WINAPI  daqGetInfo(DaqHandleT handle, DWORD chan, DaqInfo whichInfo, VOID *info);

DAQAPI DaqError WINAPI  daqGetDeviceCount(DWORD *deviceCount);
DAQAPI DaqError WINAPI  daqGetDeviceList(DaqDeviceListT *deviceList, DWORD *deviceCount);
DAQAPI DaqError WINAPI  daqGetDeviceProperties( LPSTR daqName, DaqDevicePropsT *deviceProps);

DAQAPI DaqHardwareVersion WINAPI daqGetHardwareType(DaqHandleT handle, DaqHardwareSubType *pHwSubType);
DAQAPI VOID WINAPI daqCvtHardwareType(DaqHardwareVersion HwType, DaqHardwareSubType HwSubType, PCHAR pHwTypeStr, DWORD HwTypeStrSize);

/*****************************************************************/
/*                                                               */
/*        Device Inventory Definitions and Prototypes            */
/*                                                               */
/*****************************************************************/

typedef enum
{
	DaqInfoTypeUnknown  = 0, 
	DaqInfoTypeGeneric  = 1,
	DaqInfoTypeTcp      = 2, 
	DaqInfoTypeTcpChild = 3,
	DaqInfoTypeParallel = 4,
	DaqInfoTypePci      = 5,
	DaqInfoTypePcCard   = 6,
} DaqInfoType;

typedef enum
{
	DaqIPModeAutoDetect = 0, // the IP address and other TCP settings are detected based on the serial number of the device
	DaqIPModeManualIP   = 1, // the IP address and other TCP settings are manually configured
} DaqIPModeT;

// TCP info structure
typedef struct _DaqInfoTcpT
{
	DaqIPModeT IPMode;    // IP mode (see DaqIPModeT): required when creating a device, otherwise read-only
	CHAR  SerialNum[32];  // serial number: required when creating an AutoDetect mode device, otherwise read-only
	CHAR  IPAddress[32];  // IP address: required when creating a ManualIP mode device, otherwise read-only
} DaqInfoTcpT, *DaqInfoTcpPT;

// TCP child info structure
typedef struct _DaqInfoTcpChildT
{
	CHAR  ParentAliasName[64]; // parent device alias name: required when creating a device or with DaqInfoFlagsMatchParentAlias, otherwise read-only
	DWORD ExpPort;        // parent device expansion port number: required when creating a device, otherwise read-only
	DaqInfoTcpT Tcp;      // tcp information: read-only
} DaqInfoTcpChildT, *DaqInfoTcpChildPT;

// Parallel port info structure
typedef struct _DaqInfoParallelT
{
	DWORD LPTPort;        // interrupt level: required when creating a device, otherwise read-only
	DaqProtocol Protocol; // protocol (see DaqProtocol): required when creating a device, otherwise read-only
} DaqInfoParallelT, *DaqInfoParallelPT;

// PCI board info structure
typedef struct _DaqInfoPciT
{
	DWORD SerialNum;      // serial number: required when creating an AutoDetect mode device, otherwise read-only
	DWORD Bus;            // pci bus number: required when creating a device, otherwise read-only
	DWORD Slot;           // pci slot number: required when creating a device, otherwise read-only
} DaqInfoPciT, *DaqInfoPciPT;

// PC Card info structure
typedef struct _DaqInfoPcCardT
{
	DWORD Socket;         // socket number: required when creating a device, otherwise read-only
	DWORD BasePortAddr;   // base port address: required when creating a device, otherwise read-only
	DWORD InterruptLevel; // interrupt level: required when creating a device, otherwise read-only
	DWORD DriverInstance; // driver instance: required when creating a device, otherwise read-only
	DWORD DaqModel;       // daq model: required when creating a device, otherwise read-only
} DaqInfoPcCardT, *DaqInfoPcCardPT;

// ISA board info structure
typedef struct _DaqInfoGenericT
{
	DWORD BasePortAddr;   // base port address: required when creating a device, otherwise read-only
	DaqProtocol Protocol; // protocol (see DaqProtocol): required when creating a device, otherwise read-only
	DWORD InterruptLevel; // interrupt level: required when creating a device, otherwise read-only
	DWORD DMAChannel;     // dma channel: required when creating a device, otherwise read-only
} DaqInfoGenericT, *DaqInfoGenericPT;

typedef struct _DaqDevInfoT
{
	CHAR  AliasName[64];  // device alias name: required when creating a device or with DaqInfoFlagsMatchAlias, otherwise read-only
	DaqHardwareVersion DeviceType; // device type (see DaqHardwareVersion): required when creating a device or with DaqInfoFlagsMatchType or DaqInfoFlagsMatchFamily, otherwise read-only
	DaqHardwareSubType DeviceSubType;  // device sub type (see ???): required when creating a device or with DaqInfoFlagsMatchSubType, otherwise read-only
	DWORD Reserved1;      // reserved for future use
	DWORD Reserved2;      // reserved for future use
	DaqInfoType InfoType; // info type (see DaqDevInfoType): required when creating a device, otherwise read-only

	union
	{
		DaqInfoTcpT      Tcp;      // valid if InfoType is DaqInfoTypeTcp
		DaqInfoTcpChildT TcpChild; // valid if InfoType is DaqInfoTypeTcpChild
		DaqInfoParallelT Parallel; // valid if InfoType is DaqInfoTypeParallel
		DaqInfoPciT      Pci;      // valid if InfoType is DaqInfoTypePci
		DaqInfoPcCardT   PcCard;   // valid if InfoType is DaqInfoTypePci
		DaqInfoGenericT  Generic;  // valid if InfoType is DaqInfoTypeGeneric
		CHAR             ReservedInfo[512]; // used to force the size of the info type union to 512 bytes
	};
} DaqDevInfoT, *DaqDevInfoPT;

typedef enum
{
	// these flags enable checking the registry for
	// devices that may or may not have been created
	DaqInfoFlagsCreated      = 0x00000003,
	DaqInfoFlagsNotCreated   = 0x00000002,

	// these flags enable the detection of PCI and
	// ethernet devices
	DaqInfoFlagsDetected     = 0x0000000C,
	DaqInfoFlagsNotDetected  = 0x00000008,

	// these flags enable the testing of detected or
	// created devices by pinging the device (ethernet)
	// or opening the device (PCI and LPT)
	//
	// Note1: must be used in conjunction with
	// DaqInfoFlagsCreated and/or DaqInfoFlagsDetected
	//
	// Note2: for ethernet devices, these flags could be
	// used in conjunction with DaqInfoFlagsDetected to
	// determine whether or not the detected device is on
	// a valid subnet 
	DaqInfoFlagsTestPassed   = 0x00000030,
	DaqInfoFlagsTestFailed   = 0x00000020,

	// DaqInfoFlagsNew will choose the group of new
	// devices that were detected but not yet created
	DaqInfoFlagsNew          = DaqInfoFlagsDetected | DaqInfoFlagsNotCreated,

	// DaqInfoFlagsRemoved will choose the group of
	// devices that were created, but no longer detected
	DaqInfoFlagsRemoved      = DaqInfoFlagsCreated | DaqInfoFlagsTestFailed,

	// DaqInfoFlagsRemoved will choose the group of
	// devices that were created, but no longer detected
	DaqInfoFlagsAvailable    = DaqInfoFlagsCreated | DaqInfoFlagsTestPassed,

	// these flags enable will choose devices that match
	// alias or device type conditions
	DaqInfoFlagsMatchAlias      = 0x00010000,
	DaqInfoFlagsMatchParentAlias= 0x00020000,
	DaqInfoFlagsMatchType       = 0x00030000,
	DaqInfoFlagsMatchSubType    = 0x00040000,
	DaqInfoFlagsMatchProdGroup  = 0x00050000,
	DaqInfoFlagsMatchProdFamily = 0x00060000,
	DaqInfoFlagsMatchMask       = 0x000f0000,

} DaqDevInfoFlagsT;

// DaqX device information commands
DAQAPI DaqError WINAPI  daqGetDeviceInfo(DaqHandleT handle, DaqDevInfoPT pDevInfo);

// DaqX device inventory commands
DAQAPI DaqError WINAPI  daqCreateDevice(DaqDevInfoPT pDevInfo);
DAQAPI DaqError WINAPI  daqDeleteDevice(LPSTR AliasName);
DAQAPI DaqError WINAPI  daqGetDeviceInventory(DaqDevInfoPT pInfoList, PDWORD pInfoListCount, DaqDevInfoPT pMatchInfo, DWORD flags);

/*****************************************************************/
/*                                                               */
/*            Dbk/Wbk Card Definitions and Prototypes                */
/*                                                               */
/*****************************************************************/

/* Dbk Card Expansion Bank Definitions and WaveBook option unit */
/* and chassis definitions                                      */
typedef enum {
   DaetNotDefined      =  0,   // Bank is unknown or undefine the bank
   DaetDbk50           =  1,   // Dbk50 option
   DaetDbk5            =  2,   // Dbk5 option
   DaetDbk2            =  3,   // Dbk2 option
   DaetDbk4            =  4,   // Dbk4 option
   DaetDbk7            =  5,   // Dbk7 option
   DoctWbk11           =  6,   // Wbk11 sample and hold channel
   DoctWbk12           =  7,   // Wbk12 filter card
   DoctWbk13           =  8,   // Wbk13 filter & sample and hold card
   DmctWbk512          =  9,   // WaveBook/512 channel
   DmctWbk10           =  10,  // Wbk10 channel
   DmctWbk14           =  11,  // Wbk14 channel
   DmctWbk15           =  12,  // Wbk15 channel
   DmctResponseDac     =  13,  // Response DAC on WaveBook  
   DmctWbk16           =  14,  // Wbk16 channel
   DmctWbk516          =  15,  // WaveBook/516
   DmctpDaq            =  16,  // PersonalDaq option
   DmctWbk516_250      =  17,  // 250 kHz WaveBook/516
   DoctPga516          =  18,  // WaveBook/516 PGA Board
   DmctWbk512_10V      =  19,  // Wbk/512/10V
   DmctWbk10_10V       =  20,  // Wbk10/10V
   DmctWbk16_SSH       =  21,  // Wbk16 channel with SSH
   DmctWbk10A          =  22,  // Wbk10A channel
   DoctWbk12A          =  23,  // Wbk12A filter card
   DoctWbk13A          =  24,  // Wbk13A filter & sample and hold card
   DmctWbk17           =  25,  // Wbk17 channel
   DmctWbk512A         =  26,  // WaveBook/512A
   DmctWbk516A         =  27,  // WaveBook/516A
   DmctWbk18		   =  28,  // Wbk18 channel
   DaetDbk90		   =  29,  // DBK90
   DaetDbk91		   =  30,  // DBK91
   DaetMaxValue        =  DaetDbk91,
} DaqAdcExpType;

// Expansion and Option Sub Type Definitions
typedef enum {
	// a sub type of DaestDefault
	// indicates that the bank or option device type
	// is defined by the hardware version
	DaestDefault			= 0,

	// sub types for the DaetDbk7 main type
	DaestDbk7				= 0, // DBK7
	DaestDbk55				= 1, // DBK55 (8 channel DBK7)

	// sub types for the DmctWbk516A main type
	DmcstWaveBook516A		= 0, // WaveBook/516A
	DmcstWaveBook516E		= 1, // WaveBook/516E
	DmcstZonicBook618		= 2, // ZonicBook/618
	DmcstStrainBook616		= 3, // StrainBook/616

	// sub types for the DmctWbk16_SSH main type
	DmcstWbk16_SSH			= 0, // WBK16 with SSH
	DmcstStrainBook616Gauge	= 1, // Strain channels of the StrainBook/616

	// sub types for the DmctWbk17 main type
	DmcstWbk17				= 0, // WBK17
	DmcstZonicBook618Tach	= 1, // Tachometer channels of the ZonicBook/618

	// sub types for the DmctWbk18 main type
	DmcstWbk18				= 0, // WBK18
	DmcstZonicBook618ICP	= 1, // ICP channels of the ZonicBook/618
} DaqAdcExpSubType;

DAQAPI DaqAdcExpType WINAPI daqGetChannelType(DaqHandleT handle, DWORD chan, BOOL option, DaqAdcExpSubType *pChSubType);
DAQAPI VOID WINAPI daqCvtChannelType(DaqAdcExpType ChType, DaqAdcExpSubType ChSubType, PCHAR pChTypeStr, DWORD ChTypeStrSize);

/* Dbk Card Option Type Selector Definitions */
typedef enum {
   DcotDbk4MaxFreq      = 0,
   DcotDbk4SetBaseline  = 1,
   DcotDbk4Excitation   = 2,
   DcotDbk4Clock        = 3,
   DcotDbk4Gain         = 4,  // internally used by daqAdcSetScan 

   DcotDbk7Slope        = 0,
   DcotDbk7DebounceTime = 1,
   DcotDbk7MinFreq      = 2,
   DcotDbk7MaxFreq      = 3,

   DcotDbk55Slope        = DcotDbk7Slope,
   DcotDbk55DebounceTime = DcotDbk7DebounceTime,
   DcotDbk55MinFreq      = DcotDbk7MinFreq,
   DcotDbk55MaxFreq      = DcotDbk7MaxFreq,

   DcotDbk50Gain        = 0,  // internally used by daqAdcSetScan 
   
   DcotDbk90StartChanOn	= 0,  
   DcotDbk90StartChanOff= 1,	
   DcotDbk90StartChanOffAll = 0x40000000,
} DaqChanOptionType;


/* Dbk Card and WBK Expansion; Channel and Module Option Type Selector Definitions */
typedef enum {
   // Dbk4
   DdcotDbk4MaxFreq      = 0,
   DdcotDbk4SetBaseline  = 1,
   DdcotDbk4Excitation   = 2,
   DdcotDbk4Clock        = 3,
   DdcotDbk4Gain         = 4,  /* internally used by daqAdcSetScan */

   // Dbk7
   DdcotDbk7Slope        = 0,
   DdcotDbk7DebounceTime = 1,
   DdcotDbk7MinFreq      = 2,
   DdcotDbk7MaxFreq      = 3,

   DdcotDbk55Slope        = DdcotDbk7Slope,
   DdcotDbk55DebounceTime = DdcotDbk7DebounceTime,
   DdcotDbk55MinFreq      = DdcotDbk7MinFreq,
   DdcotDbk55MaxFreq      = DdcotDbk7MaxFreq,

   // Dbk50/51
   DdcotDbk50Gain        = 0,  /* internally used by daqAdcSetScan */

   // DBK90/91
   DdcotDbk90StartChanOn = 0,  
   DdcotDbk90StartChanOff= 1,
   DdcotDbk90StartChanOffAll = DcotDbk90StartChanOffAll,
   DmotDbk90StartChanOffAll = DcotDbk90StartChanOffAll,
   
   /* Wbk module Option type Definitions */

   // Wbk12
   DcotWbk12FilterCutOff      = 0,
   DcotWbk12FilterType        = 1,
   DcotWbk12FilterMode        = 2,
   DcotWbk12PreFilterMode     = 3,
   DcotWbk12PreFilterCutOff   = 4,
   DcotWbk12PostFilterCutOff  = 5,

   // Wbk13
   DcotWbk13FilterCutOff      = 0,
   DcotWbk13FilterType        = 1,
   DcotWbk13FilterMode        = 2,
   DcotWbk13PreFilterMode     = 3,
   DcotWbk13PreFilterCutOff   = 4,
   DcotWbk13PostFilterCutOff  = 5,

   // Wbk14
   DcotWbk14LowPassMode       = 0,
   DcotWbk14LowPassCutOff     = 1,
   DcotWbk14HighPassCutOff    = 2,
   DcotWbk14CurrentSrc        = 3,
   DcotWbk14PreFilterMode     = 4,
   DmotWbk14ExcSrcWaveform    = 5,
   DmotWbk14ExcSrcFreq        = 6,
   DmotWbk14ExcSrcAmplitude   = 7,
   DmotWbk14ExcSrcOffset      = 8,
   DmotWbk14ExcSrcApply       = 9,
   DcotWbk14ExtFilterRange    = 10,
   DcotWbk14DetectSensor      = 11,

   // Wbk16
   DcotWbk16Bridge            = 0,
   DcotWbk16ShuntCal          = 1,
   DcotWbk16InDiag            = 2,
   DcotWbk16OffsetDac         = 3,
   DcotWbk16OutSource         = 4,
   DcotWbk16Inv               = 5,
   DcotWbk16FilterType        = 6,
   DcotWbk16Couple            = 7,
   DcotWbk16Sample            = 8,
   DcotWbk16ExcDac            = 9,
   DcotWbk16IAG               = 10,
   DcotWbk16PGA               = 11,
   DmotWbk16Immediate         = 12,

   // Wbk17
   DcotWbk17Level             = 0, // Range: +/-12.5 Volts
   DcotWbk17Coupling          = 1, // Off, AC, DC
   DcotWbk17FilterType        = 2, // Bypass, 100K, 20K, 30Hz
   DcotWbk17DebounceTime      = 3, // Enumeration or Bypass
   DcotWbk17DebounceTrigger   = 4, // Trigger After/Before Stable
   DcotWbk17Edge              = 5, // Rising/Falling Edge
   DcotWbk17MeasurementMode   = 6, // Enumeration with Bit-Masking
   DcotWbk17TickSize          = 7, // Enumeration; 20ns-20us
   DcotWbk17MapChannel        = 8, // Enumeration; counter or detector 1 - 8 (= "Z" in encoder mode)
   DmotZbkTachPhaseDelay      = 9, // up to 4 seconds in nSec

   // Wbk17 Detection
   DcotWbk17DetectClear       = 20, // Enumeration; clear/reset channel or unit (all)
   DcotWbk17DetectControl     = 21, // Enumeration; set comparison or off/clear
      // Be sure to use DcofSubChannel... DaqChanOptionFlagType + Detector number (1-16)
   DcotWbk17DetectLowLimit    = 22, // 0 - 65535
   DcotWbk17DetectHighLimit   = 23, // 0 - 65535
   DcotWbk17DetectDigComp     = 24, // 0 - 255 : Dig port comparison value 
   DcotWbk17DetectDigMask     = 25, // 0 - 255 : DigMask used by DigComp AND DigOut
   DcotWbk17DetectDigOut      = 26, // 0 - 255 : Dig port output value (on detection true if enabled)
   
	// Wbk18
	DcotWbk18LowPassMode		= 0,  // Enumeration: Bypass, 8 pole and 2 pole
	DcotWbk18LowPassCutOff		= 1,  // Enumeration: 
	DcotWbk18HighPassCutOff		= 2,  // Enumeration: DC,0.1Hz and 10Hz
	DcotWbk18CurrentSrc			= 3,  // Enumeration: Off and 4mA
	DcotWbk18OverRangeEnable	= 4,  // Enumeration: Off and On
	DmotWbk18esMode				= 5,  // zero,off,continuous output or Swept (sweep through up to 1280 freqs)
	DmotWbk18esFreq				= 6,  // 0.01Hz - 5000Hz
	DmotWbk18esAmplitude		= 7,  // Enumeration: 7 amplitudes
	DmotWbk18esFreqCycleTime	= 8,  // Swept Mode:freq-to-freq time in seconds (50uS to 85,899,000uS)
	DmotWbk18esFreqDurationTime	= 9,  // Swept Mode:total time to sweep all freqs ( 50uS to 85,899,000uS)
	DmotWbk18OverRangeLimit		= 10, // 1 - 100 (%)   : percentage of selected range
	DmotWbk18OverRangeEnable	= 11, // option value is 8 bit mask representing the 8 channels- zero disables all
	DmotWbk18esImmediate		= 12, // used to reset, turn on and turn off the excitation source  
	DmotWbk18esRelay			= 13, // used to specify output relay on/off 
	DcotWbk18CurrentSrcImmediate = 14, // Enumeration: Off and 4mA - Action immediate
	DcotWbk18TEDsStatus			= 15,
	DcotWbk18TEDsData			= 16,


   // pDaq option types
   DcotpDaqSlope              = 0,
   DcotpDaqDebounceTime       = 1,
   DcotpDaqMinFreq            = 2,
   DcotpDaqMaxFreq            = 3,
   DcotpDaqPulses             = 4,

   // Pga516 Option Types
   DcotPga516LowPassMode      = 0,
   
	// Base unit options.  chan and flags arguments are ignored
	DbotBaseUnitOption      = 0x8000,	// bit mask that marks all base unit options.
	DbotFifoOverflowMode    = 0x8001,	// True/False, where True enables the mode
	DbotFifoCycleMode       = 0x8002,	// True/False, where True enables the mode
	DbotFifoCycleSize       = 0x8003,	// Cycle buffer length in WORD's
	DbotFifoFlush           = 0x8004,	// Flush all data in the FIFO now.
	DbotFifoNoFlushOnDisarm = 0x8005,	// Disable Buffer Flushing upon Disarm
	DbotIgnoreFirstScan		= 0x8006,	// Ignore First Scan (DaqBook/2000 Series - Used for Slaved GSYNC Mode)

	// Dig I/O, Counter, & Timer Configuration & Control
	DcotDigitalOption       = 0x1000, // bit mask that marks all Digital options.
	DcotP2Local8Mode        = 0x1001, // Input, Output
	DcotP2Exp8Mode          = 0x1002, // Input, Output
	DcotP3Local16Mode       = 0x1003, // Input, Output

	DcotCounterOption       = 0x2000, // bit mask that marks all Counter options.
	DcotCounterCascade      = 0x2001, // Single, Cascaded
	DcotCounterMode         = 0x2002, // Clear on Read, Totalize
	DcotCounterControl      = 0x2003, // Off, On, Immediate Clear
	DmotCounterControl      = 0x2004, // All Counters: Off, On, Immediate Clear
	DcotCounterEdge         = 0x2005, // Falling or Rising Edge Detection

	DcotTimerOption         = 0x4000, // bit mask that marks all Timer options.
	DcotTimerDivisor        = 0x4001, // 16-bit Number (freq = 1MHz / (Divisor + 1))
	DcotTimerControl        = 0x4002, // Off, On
	DmotTimerControl        = 0x4003, // All Timers: Off, On
	

} DaqOptionType;


typedef enum {
   DcofChannel      =  0x00,  // Channel Option; Apply option to one channel only
   DcofModule       =  0x01,  // Module Option; Apply option to whole module
                              // Use only with Dmot... DaqModuleOptionTypes and Dmov... DaqModuleOptionValues

   // SubChannel Identifiers for WBK17 Detection Options
   // Use enum + detection number (1 - 16)
   DcofSubChannelLow  = 0x1000,  // Counter Low Word
   DcofSubChannelHigh = 0x2000,  // Counter High Word

} DaqChanOptionFlagType;

/* Dbk Card and WBK Expansion; Option Value Definitions */
typedef enum {

	// Digital I/O Port Mode Definitions for All DcotDigitalOptions
	DcovDigitalInput        = 0,
	DcovDigitalOutput       = 1,
	
	// Counter Cascade Option Definitions for DcotCounterCascade	
	DcovCounterSingle       = 0,
	DcovCounterCascade      = 1,
	
	// Counter Mode Option Definitions for DcotCounterMode
	DcovCounterClearOnRead  = 0,
	DcovCounterTotalize     = 1,

	// Counter Edge Option Definitions for DcotCounterEdge
	DcovCounterFallingEdge  = 0,
	DcovCounterRisingEdge   = 1,

	// Counter Control Option Definitions for:
	// DcotCounterControl (Individual Channel)
	// DmotCounterControl (All Counters)
	DcovCounterOff          = 0,
	DcovCounterOn           = 1,	
	DcovCounterManualClear  = 2,

	// Timer Control Option Definitions for:
	// DcotTimerControl (Individual Channel)
	// DmotTimerControl (All Counters)
	DcovTimerOff            = 0,
	DcovTimerOn             = 1,

				
	/* Dbk4 cutoff frequencies for DcotMaxFreq option type */
   DcovDbk4Freq18000Hz     = 0,
   DcovDbk4Freq9000Hz      = 1,
   DcovDbk4Freq4500Hz      = 2,
   DcovDbk4Freq2250Hz      = 3,
   DcovDbk4Freq1125Hz      = 4,
   DcovDbk4Freq563Hz       = 5,
   DcovDbk4Freq281Hz       = 6,
   DcovDbk4Freq141Hz       = 7,

   /* Dbk4 set baseline for DcotSetBaseline option type */
   DcovDbk4BaselineNever   = 0,
   DcovDbk4BaselineOneShot = 1,

   /* Dbk7/55 debounce times for DcotDebounceTime option type */
   DcovDbk7DebounceNone    = 0,
   DcovDbk7Debounce600us   = 1,
   DcovDbk7Debounce2500us  = 2,
   DcovDbk7Debounce10ms    = 3,

   DcovDbk55DebounceNone    = DcovDbk7DebounceNone,
   DcovDbk55Debounce600us   = DcovDbk7Debounce600us,
   DcovDbk55Debounce2500us  = DcovDbk7Debounce2500us,
   DcovDbk55Debounce10ms    = DcovDbk7Debounce10ms,

   /* Wbk Option Value Definitions */

   /* Wbk 12 Filter Type Definitions for DcotWbk12FilterType */
   DcovWbk12FilterElliptic    = 0,
   DcovWbk12FilterLinear      = 1,

   /* Wbk 12 Filter Mode Definitions for DcotWbk12FilterMode */
   DcovWbk12FilterBypass      = 0,
   DcovWbk12FilterOn          = 1,

   /* Wbk 12 Anti-Aliasing Filter Mode Definitions for DcotWbk12PreFilterMode */
   DcovWbk12PreFilterDefault  = 0,
   DcovWbk12PreFilterOff      = 1,

   /* Wbk 12 Anti-Aliasing Filter CutOff Frequency Definitions for DcotWbk12PreFilterCutOff */
   DcovWbk12PreFilterCutOffDefault  = 0,

   /* Wbk 12A Clock Filter CutOff Frequency Definitions for DcotWbk12PostFilterCutOff */
   DcovWbk12PostFilterCutOffDefault = 0,

   /* Wbk 13 Filter Type Definitions for DcotWbk13FilterType */
   DcovWbk13FilterElliptic    = 0,
   DcovWbk13FilterLinear      = 1,

   /* Wbk 13 Filter Mode Definitions for DcotWbk13FilterMode */
   DcovWbk13FilterBypass      = 0,
   DcovWbk13FilterOn          = 1,

   /* Wbk 13 Anti-Aliasing Filter Mode Definitions for DcotWbk13PreFilterMode */
   DcovWbk13PreFilterDefault  = 0,
   DcovWbk13PreFilterOff      = 1,

   /* Wbk 13 Anti-Aliasing Filter CutOff Frequency Definitions for DcotWbk13PreFilterCutOff */
   DcovWbk13PreFilterCutOffDefault  = 0,

   /* Wbk 13A Clock Filter CutOff Frequency Definitions for DcotWbk13PostFilterCutOff */
   DcovWbk13PostFilterCutOffDefault = 0,

   /* Wbk 14 Current Source Definitions for DcotWbk14CurrentSrc */
   DcovWbk14CurrentSrcOff     = 0,
   DcovWbk14CurrentSrc2mA     = 1,
   DcovWbk14CurrentSrc4mA     = 2,

   /* Wbk 14 High Pass Filter Definitions for DcotWbk14HighPassCutOff */
   DcovWbk14HighPass0_1Hz     = 0,
   DcovWbk14HighPass10Hz      = 1,

   /* Wbk 14 Low Pass Filter Mode Definitions for DcotWbk14LowPassMode */
   DcovWbk14LowPassBypass     = 0,
   DcovWbk14LowPassOn         = 1,
   DcovWbk14LowPassExtClk     = 2,

   /* Wbk 14 Low Pass Filter Mode Definitions for DcotWbk14PreFilterMode */
   DcovWbk14PreFilterDefault  = 0,
   DcovWbk14PreFilterOff      = 1,

   /*Wbk14 External (Order Tracking) Filter Frequency Range Definitions*/
   DcovWbk14FilterRange_1K    = 0,
   DcovWbk14FilterRange_5K    = 1,
   DcovWbk14FilterRange_10K   = 2,
   DcovWbk14FilterRange_15K   = 3,
   DcovWbk14FilterRange_20K   = 4,
      
   /* Wbk 14 Excitation Source Waveform Definitions for WmotWbk14ExcSrcWaveform */
   DmovWbk14ExcSrcRandom      = 0,
   DmovWbk14ExcSrcSine        = 1,

   // Wbk16 Bridge Definitions
   DcovWbk16ApplyFull         = 0,
   DcovWbk16ApplyHalfQtrPos   = 1,
   DcovWbk16ApplyHalfQtrNeg   = 2,

   // Wbk16 Shunt resistors cal definitions
   DcovWbk16ApplyNone         = 0,
   DcovWbk16Apply120          = 1,
   DcovWbk16Apply350          = 2,
   DcovWbk16Apply1K           = 3,
   DcovWbk16AutoZero          = 4,

   // Wbk16 Input Diagnostics definitions
   DcovWbk16ReadNone          = 0,
   DcovWbk16ReadHalf          = 1,
   DcovWbk16ReadPosArm        = 2,

   // Wbk16 Output Source definitions
   DcovWbk16ReadSignal        = 0,
   DcovWbk16ReadExcVolts      = 1,
   DcovWbk16ReadExcCurr       = 2,

   // Wbk16 SSH definitions
   DcovWbk16Bypassed          = 0,
   DcovWbk16Ssh               = 1,

   // Wbk16 Inversion definitions
   DcovWbk16Normal            = 0,
   DcovWbk16Inverted          = 1,

   // WbK16 Filter Type Definitions
   DcovWbk16FltBypass         = 0,
   DcovWbk16Flt10Hz           = 1,
   DcovWbk16Flt1Khz           = 2,

   // Wbk16 Coupling Definitions
   DcovWbk16CoupleDC          = 0,
   DcovWbk16CoupleAC          = 1,

   // Wbk16 Instrumentation Amplifier w/Gain Definitions
   DcovWbk16X1                = 0,
   DcovWbk16X10               = 1,
   DcovWbk16X100              = 2,
   DcovWbk16X1000             = 3,

   // Wbk16 Programmable Gain Amplifier Definitions
   DcovWbk16X1_00             = 0,
   DcovWbk16X1_28             = 1,
   DcovWbk16X1_65             = 2,
   DcovWbk16X2_11             = 3,
   DcovWbk16X2_71             = 4,
   DcovWbk16X3_48             = 5,
   DcovWbk16X4_47             = 6,
   DcovWbk16X5_74             = 7,
   DcovWbk16X7_37             = 8,
   DcovWbk16X9_46             = 9,
   DcovWbk16X12_14            = 10,
   DcovWbk16X15_58            = 11,
   DcovWbk16X20_00            = 12,

   // Wbk16 Calibrated excitation DAC values.
   DcovWbk16Exc0_0            = 0,
   DcovWbk16Exc0_5            = 0x1000,
   DcovWbk16Exc1_0            = 0x2000,
   DcovWbk16Exc2_0            = 0x3000,
   DcovWbk16Exc5_0            = 0x4000,
   DcovWbk16Exc10_0           = 0x5000,

   // Wbk16 Immediate Function Definitions
   DmovWbk16ExcSrcApply       = 0,
   DmovWbk16ReadTemp          = 1,
   DmovWbk16ReadJumpers       = 2,
   DmovWbk16FanOn             = 3,
   DmovWbk16FanOff            = 4,

   // Wbk17 Input Coupling Definitions   
   DcovWbk17CoupleOff         = 0, // input off
   DcovWbk17CoupleAC          = 1,
   DcovWbk17CoupleDC          = 2,

   // WbK17 Filter Type Definitions
   DcovWbk17FltBypass         = 0,
   DcovWbk17Flt100KHz         = 1,
   DcovWbk17Flt20KHz          = 2,
   DcovWbk17Flt30Hz           = 4,

   // Wbk17 Debounce Times   
   DcovWbk17Debounce500ns     = 0,
   DcovWbk17Debounce1500ns    = 1,
   DcovWbk17Debounce3500ns    = 2,
   DcovWbk17Debounce7500ns    = 3,
   DcovWbk17Debounce15500ns   = 4,
   DcovWbk17Debounce31500ns   = 5,
   DcovWbk17Debounce63500ns   = 6,   
   DcovWbk17Debounce127500ns  = 7,   
   DcovWbk17Debounce100us     = 8,
   DcovWbk17Debounce300us     = 9,
   DcovWbk17Debounce700us     = 10,
   DcovWbk17Debounce1500us    = 11,
   DcovWbk17Debounce3100us    = 12,
   DcovWbk17Debounce6300us    = 13,
   DcovWbk17Debounce12700us   = 14,
   DcovWbk17Debounce25500us   = 15,
   DcovWbk17DebounceNone      = 16,

   // Wbk17 Debounce Trigger
   DcovWbk17TriggerAfterStable   = 0,
   DcovWbk17TriggerBeforeStable  = 1,

   // Wbk17 Edge Detection
   DcovWbk17RisingEdge        = 0,
   DcovWbk17FallingEdge       = 1,

   // Wbk17 Measurement Modes and mode specific flags
   // Usage: combine the Mode with any Mode specific settings required.
   // (DcovWbk17Mode_Counter | DcovWbk17Counter_ClearOnRead | DcovWbk17ModeMask_32Bit)

   // these are available for all modes
   // the rest are mode specific
   DcovWbk17ModeMask_16Bit      = 0x00,
   DcovWbk17ModeMask_32Bit      = 0x04,

   DcovWbk17ModeMaskGatingOff   = 0x00,
   DcovWbk17ModeMaskGatingOn    = 0x10,

   DcovWbk17Mode_OFF        = 0x0000,

   DcovWbk17Mode_Counter    = 0x0100,
   DcovWbk17Counter_Totalize    = 0x00,
   DcovWbk17Counter_ClearOnRead = 0x01,
	   
   DcovWbk17Counter_RollOver    = 0x00,
   DcovWbk17Counter_StopOnTop   = 0x02,
	   
   DcovWbk17Counter_LatchOnSOS  = 0x00,
   DcovWbk17Counter_LatchOnMap  = 0x08,
	   
   DcovWbk17Counter_DecrementOff= 0x00,
   DcovWbk17Counter_DecrementOn = 0x20,
	   
   DcovWbk17Counter_CountChan   = 0x00,
   DcovWbk17Counter_CountMap    = 0x40,

   DcovWbk17Mode_Period     = 0x0200,
   DcovWbk17Period_X1           = 0,
   DcovWbk17Period_X10          = 1,
   DcovWbk17Period_X100         = 2,
   DcovWbk17Period_X1000        = 3,

   DcovWbk17Period_MeasChan     = 0x00,
   DcovWbk17Period_MeasMap      = 0x40,

   DcovWbk17Mode_PulseWidth = 0x0300,
   DcovWbk17PulseWidth_MeasChan = 0x00,
   DcovWbk17PulseWidth_MeasMap  = 0x40,

   DcovWbk17Mode_Timing     = 0x0400,

   DcovWbk17Mode_Encoder    = 0x0500,
   DcovWbk17Encoder_X1          = 0,
   DcovWbk17Encoder_X2          = 1,
   DcovWbk17Encoder_X4          = 2,

   DcovWbk17Encoder_LatchOnSOS  = 0x00,
   DcovWbk17Encoder_LatchOnZ    = 0x08,

   DcovWbk17Encoder_ClearOnZ_Off= 0x00,
   DcovWbk17Encoder_ClearOnZ_On = 0x20,


   // Wbk17 Tick Size
   DcovWbk17Tick20ns          = 0,
   DcovWbk17Tick200ns         = 1,
   DcovWbk17Tick2000ns        = 2,
   DcovWbk17Tick20000ns       = 3,

   // Wbk17 Map Channel
   DcovWbk17Map_Channel_1     = 1,
   DcovWbk17Map_Channel_2     = 2,
   DcovWbk17Map_Channel_3     = 3,
   DcovWbk17Map_Channel_4     = 4,
   DcovWbk17Map_Channel_5     = 5,
   DcovWbk17Map_Channel_6     = 6,
   DcovWbk17Map_Channel_7     = 7,
   DcovWbk17Map_Channel_8     = 8,
   DcovWbk17Map_Detect_1      = 9,
   DcovWbk17Map_Detect_2      = 10,
   DcovWbk17Map_Detect_3      = 11,
   DcovWbk17Map_Detect_4      = 12,
   DcovWbk17Map_Detect_5      = 13,
   DcovWbk17Map_Detect_6      = 14,
   DcovWbk17Map_Detect_7      = 15,
   DcovWbk17Map_Detect_8      = 16,

   // Wbk17 Clear Detection Settings : DcotWbk17DetectClear
   DcovWbk17DetClr_Chan       = 0, // Clear all settings of specified channel (Low AND High)
   DcovWbk17DetClr_All        = 1, // Clear all settings of all channels (Low AND High)
      
   // Wbk17 Detection Control : DcotWbk17DetectControl
   // Settings can be combined (DcovWbk17DtctCtrl_Below_Low | DcovWbk17DtctCtrl_Update_Dig)
   // Note: Be sure to use DcofSubChannel... DaqChanOptionFlagType
   DcovWbk17DetCtrl_Off             = 0, // Detection Setting Off
   DcovWbk17DetCtrl_Below_Low       = 1, // Count < Low Limit
   DcovWbk17DetCtrl_Above_High      = 2, // High Limit < Count
   DcovWbk17DetCtrl_Outside_Range   = 3, // (Count < Low Limit) || (High Limit < Count)
   DcovWbk17DetCtrl_Inside_Range    = 4, // Low Limit > Count > High Limit
   DcovWbk17DetCtrl_Dig_Eq_Dig      = 8, // (DigComp & DigMask) == (DigOut & DigMask) (actual)
   DcovWbk17DetCtrl_Update_Dig      = 16,// Update DigOut On Detection (using DigMask & DigOut)


/* Wbk 18 Channel Definitions for DcotWbk18OverRangeEnable */
   DcovWbk18OverRangeOff     = 0,
   DcovWbk18OverRangeOn     = 1,
  
/* Wbk 18 Current Source Definitions for DcotWbk18CurrentSrc */
   DcovWbk18CurrentSrcOff     = 0,
   DcovWbk18CurrentSrc4mA     = 1,

   /* Wbk 18 High Pass Filter Definitions for DcotWbk18HighPassCutOff */
   DcovWbk18HighPass0_1Hz     = 0,/* Not valid for WgcGain0_2 */
   DcovWbk18HighPass10Hz      = 1,/* Not valid for WgcGain0_2 */
   DcovWbk18HighPassDC		  = 2,

   /* Wbk 18 Low Pass Filter Mode Definitions for DcotWbk18LowPassMode */
   DcovWbk18LowPassBypass     = 0,
   DcovWbk18LowPass_8_Pole    = 1,
   DcovWbk18LowPass_2_Pole    = 2,


 /* Wbk 18 Low Pass Filter Frequencies Definitions for DcotWbk18LowPassCutOff */
   DcovWbk18LPF_Cutoff_10Hz		= 0,
   DcovWbk18LPF_Cutoff_20Hz		= 1,
   DcovWbk18LPF_Cutoff_50Hz		= 2,
   DcovWbk18LPF_Cutoff_100Hz	= 3,
   DcovWbk18LPF_Cutoff_200Hz	= 4,
   DcovWbk18LPF_Cutoff_500Hz	= 5,
   DcovWbk18LPF_Cutoff_1000Hz	= 6,
   DcovWbk18LPF_Cutoff_2000Hz	= 7,
   DcovWbk18LPF_Cutoff_5000Hz	= 8,
   DcovWbk18LPF_Cutoff_10000Hz	= 9,
   DcovWbk18LPF_Cutoff_20000Hz	= 10,
   DcovWbk18LPF_Cutoff_50000Hz	= 11,

   /* Wbk 18 External Source level Definitions for DmotWbk18esAmplitude */ 
   DmovWbk18esAmplitude10pp			=0,	//10 volts peak-to-peak
   DmovWbk18esAmplitude5pp			=1,	//5 volts peak-to-peak
   DmovWbk18esAmplitude2pp			=2,	//2 volts peak-to-peak
   DmovWbk18esAmplitude1pp			=3,	//1 volts peak-to-peak
   DmovWbk18esAmplitude0_5pp		=4,	//0.5 volts peak-to-peak
   DmovWbk18esAmplitude0_2pp		=5,	//0.2 volts peak-to-peak
   DmovWbk18esAmplitude0_1pp		=6,	//0.1 volts peak-to-peak
   DmovWbk18esAmplitude0_0pp		=7, // 0.0 volts

   /* Wbk 18 Excitation Source Definitions for DmotWbk18esMode */
   DmovWbk18esSine        = 0,	// continuous sinewave mode
   DmovWbk18esSweptSine	  = 1,	// swept mode

   // Wbk 18 Excitation Source Definitions for DmotWbk18esRelay
   DmovWbk18esRelayOpen	  = 0,
   DmovWbk18esRelayClosed = 1,

   //Wbk18 Excitation Source definitions for DmotWbk18esImmediate
   DmovWbk18esStop				=0,	// stops excitation output
   DmovWbk18esStart				=1,	// starts excitation output using current amplitude/freq values
   DmovWbk18esWriteRAM			=3,	// writes the current amplitude/freq/relay values to RAM
   DmovWbk18esClearRAM			=4,	// erase stored segments
  

   // WaveBook/516 Filter
   DcovPga516LowPassBypass		= 0,
   DcovPga516LowPassOn			= 1,

   // DaqBook/2000 Ignore First Scan settings (On/Off) 
   DbovIgnoreFirstScanOff		= 0,
   DbovIgnoreFirstScanOn		= 1,

 } DaqChanOptionValue;


/* Bank Configuration Prototypes */
DAQAPI DaqError WINAPI  daqAdcExpSetBank(DaqHandleT handle, DWORD chan,
                           DaqAdcExpType bankType);
DAQAPI DaqError WINAPI  daqAdcExpSetBankEx(DaqHandleT handle, DWORD chan,
                           DaqAdcExpType bankType, DaqAdcExpSubType bankSubType);

/* Obsolete. Use daqSetOption */
DAQAPI DaqError WINAPI  daqAdcExpSetChanOption(DaqHandleT handle, DWORD chan,
                           DaqChanOptionType optionType, FLOAT optionValue);
DAQAPI DaqError WINAPI  daqAdcExpSetModuleOption(DaqHandleT handle, DWORD chan,
                           DaqChanOptionType optionType, FLOAT optionValue);


/* Universal function for setting channel and module options in WaveBooks and Daq* Devices */
/* flags defined in DaqChanOptionFlagType                                                  */
/* Some Option Types have enumerated Option Values (DaqChanOptionValue)                    */
DAQAPI DaqError WINAPI  daqSetOption(DaqHandleT handle, DWORD chan, DWORD flags,
                           DaqOptionType optionType, FLOAT optionValue);


/*****************************************************************/
/*                                                               */
/*                 ADC Definitions and Prototypes                */
/*                                                               */
/*****************************************************************/

/* ADC Gain Definitions */
typedef enum {
   /* Base Unit */
   DgainX1              = 0,
   DgainX2              = 1,
   DgainX4              = 2,
   DgainX8              = 3,
   DgainX16             = 4, // DaqBoard1000/2000 series only
   DgainX32             = 5, // DaqBoard1000/2000 series only
   DgainX64             = 6, // DaqBoard1000/2000 series only

   /* DaqBoard/3000 Base Unit */
   DgainDbd3kX1         = 0,
   DgainDbd3kX2         = 1,
   DgainDbd3kX5         = 2,
   DgainDbd3kX10        = 3,
   DgainDbd3kX20        = 4,
   DgainDbd3kX50        = 5,
   DgainDbd3kX100       = 6,

   /* Base Unit Gain on DBK Connected Channel */
   /* Reference Only : Use DBK Specific Codes in Applications */
   DgainX1DbkNone       = 0,
   DgainX2DbkNone       = 4,
   DgainX4DbkNone       = 8,
   DgainX8DbkNone       = 12,
   DgainX16DbkNone      = 16, // DaqBoard1000/2000 series only
   DgainX32DbkNone      = 20, // DaqBoard1000/2000 series only
   DgainX64DbkNone      = 24, // DaqBoard1000/2000 series only

   /* Dbk4 - Filter Mode (jumper selectable) */
   Dbk4FilterX1         = 0,
   Dbk4FilterX10        = 1,
   Dbk4FilterX100       = 2,
   Dbk4FilterX1000      = 3,
   Dbk4FilterX2         = 4,
   Dbk4FilterX20        = 5,
   Dbk4FilterX200       = 6,
   Dbk4FilterX2000      = 7,
   Dbk4FilterX4         = 8,
   Dbk4FilterX40        = 9,
   Dbk4FilterX400       = 10,
   Dbk4FilterX4000      = 11,
   Dbk4FilterX8         = 12,
   Dbk4FilterX80        = 13,
   Dbk4FilterX800       = 14,
   Dbk4FilterX8000      = 15,

   /* Dbk4 - Bypass Mode (jumper selectable) */
   Dbk4BypassX1_583     = 0,
   Dbk4BypassX15_83     = 1,
   Dbk4BypassX158_3     = 2,
   Dbk4BypassX1583      = 3,
   Dbk4BypassX3_166     = 4,
   Dbk4BypassX31_66     = 5,
   Dbk4BypassX316_6     = 6,
   Dbk4BypassX3166      = 7,
   Dbk4BypassX6_332     = 8,
   Dbk4BypassX63_32     = 9,
   Dbk4BypassX633_2     = 10,
   Dbk4BypassX6332      = 11,
   Dbk4BypassX12_664    = 12,
   Dbk4BypassX126_64    = 13,
   Dbk4BypassX1266_4    = 14,
   Dbk4BypassX12664     = 15,

   /* Dbk6 */
   Dbk6X1               = 0,
   Dbk6X4               = 1,
   Dbk6X16              = 2,
   Dbk6X64              = 3,
   Dbk6X2               = 4,
   Dbk6X8               = 5,
   Dbk6X32              = 6,
   Dbk6X128             = 7,
   Dbk6X256             = 11,
   Dbk6X512             = 15,
   Dbk6X1024            = 19, // DaqBoard2000 series only
   Dbk6X2048            = 23, // DaqBoard2000 series only
   Dbk6X4096            = 27, // DaqBoard2000 series only

   /* Dbk7/55 Bipolar */
   Dbk7X1               = 0,
   Dbk7X2               = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   Dbk55X1              = Dbk7X1,
   Dbk55X2              = Dbk7X2,  // use with PCCard & DaqBoard/2000

   /* Dbk8 */
   Dbk8X1               = 0,
   Dbk8X2               = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk9 */
   Dbk9VoltageA         = 0,
   Dbk9VoltageB         = 1,
   Dbk9VoltageC         = 2,
   Dbk9VoltageD         = 3,

   /* PCCard Dbk9 use with PCCard & DaqBoard/2000 */
   DbkPCC9VoltageA      = 4,
   DbkPCC9VoltageB      = 5,
   DbkPCC9VoltageC      = 6,
   DbkPCC9VoltageD      = 7,

   /* Dbk12 */
   Dbk12X1              = 0,
   Dbk12X2              = 1,
   Dbk12X4              = 2,
   Dbk12X8              = 3,
   Dbk12X16             = 7,
   Dbk12X32             = 11,
   Dbk12X64             = 15,
   Dbk12X128            = 19, // DaqBoard2000 series only
   Dbk12X256            = 23, // DaqBoard2000 series only
   Dbk12X512            = 27, // DaqBoard2000 series only

   /* Dbk13 */
   Dbk13X1              = 0,
   Dbk13X10             = 1,
   Dbk13X100            = 2,
   Dbk13X1000           = 3,
   Dbk13X2              = 4,
   Dbk13X20             = 5,
   Dbk13X200            = 6,
   Dbk13X2000           = 7,
   Dbk13X4              = 8,
   Dbk13X40             = 9,
   Dbk13X400            = 10,
   Dbk13X4000           = 11,
   Dbk13X8              = 12,
   Dbk13X80             = 13,
   Dbk13X800            = 14,
   Dbk13X8000           = 15,
   Dbk13X16             = 16, // DaqBoard2000 series only
   Dbk13X160            = 17, // DaqBoard2000 series only
   Dbk13X1600           = 18, // DaqBoard2000 series only
   Dbk13X16000          = 19, // DaqBoard2000 series only
   Dbk13X32             = 20, // DaqBoard2000 series only
   Dbk13X320            = 21, // DaqBoard2000 series only
   Dbk13X3200           = 22, // DaqBoard2000 series only
   Dbk13X32000          = 23, // DaqBoard2000 series only
   Dbk13X64             = 24, // DaqBoard2000 series only
   Dbk13X640            = 25, // DaqBoard2000 series only
   Dbk13X6400           = 26, // DaqBoard2000 series only
   Dbk13X64000          = 27, // DaqBoard2000 series only

   /* Dbk14 Bipolar */
   Dbk14BiCJC           = Dbk13X2,
   Dbk14BiTypeJ         = Dbk13X100,
   Dbk14BiTypeK         = Dbk13X100,
   Dbk14BiTypeT         = Dbk13X200,
   Dbk14BiTypeE         = Dbk13X40,
   Dbk14BiTypeN28       = Dbk13X400,
   Dbk14BiTypeN14       = Dbk13X100,
   Dbk14BiTypeS         = Dbk13X200,
   Dbk14BiTypeR         = Dbk13X200,
   Dbk14BiTypeB         = Dbk13X400,

   /* PCCard & DaqBoard/2000 Dbk14 Bipolar */
   /* place main unit in -5 to +5v range  (additional x2 gain) */
   /* bipolar gains only */
   DbkPCC14BiCJC        = Dbk13X4,
   DbkPCC14BiTypeJ      = Dbk13X200,
   DbkPCC14BiTypeK      = Dbk13X200,
   DbkPCC14BiTypeT      = Dbk13X400,
   DbkPCC14BiTypeE      = Dbk13X80,
   DbkPCC14BiTypeN28    = Dbk13X800,
   DbkPCC14BiTypeN14    = Dbk13X200,
   DbkPCC14BiTypeS      = Dbk13X400,
   DbkPCC14BiTypeR      = Dbk13X400,
   DbkPCC14BiTypeB      = Dbk13X800,

   /* Dbk14 Unipolar */
   Dbk14UniCJC          = Dbk13X4,
   Dbk14UniTypeJ        = Dbk13X200,
   Dbk14UniTypeK        = Dbk13X200,
   Dbk14UniTypeT        = Dbk13X400,
   Dbk14UniTypeE        = Dbk13X100,
   Dbk14UniTypeN28      = Dbk13X800,
   Dbk14UniTypeN14      = Dbk13X200,
   Dbk14UniTypeS        = Dbk13X400,
   Dbk14UniTypeR        = Dbk13X400,
   Dbk14UniTypeB        = Dbk13X800,

   /* Dbk15 Bipolar */
   Dbk15BiX1            = 0,
   Dbk15BiX2            = 1,

   /* Dbk15 Unipolar : Output of card offset to +/-5 V */
   Dbk15UniX1           = 2,
   Dbk15UniX2           = 3,

   /* Dbk16 */
   Dbk16ReadBridge      = 0,
   Dbk16SetOffset       = 1,
   Dbk16SetScalingGain  = 2,
   Dbk16SetInputGain    = 3,

   /* Dbk16 with X2 gain on main unit */
   DbkPCC16ReadBridge      = 4,  // use with PCCard & DaqBoard/2000
   DbkPCC16SetOffset       = 5,  // use with PCCard & DaqBoard/2000
   DbkPCC16SetScalingGain  = 6,  // use with PCCard & DaqBoard/2000
   DbkPCC16SetInputGain    = 7,  // use with PCCard & DaqBoard/2000

   /* Dbk17 */
   Dbk17X1              = 0,
   Dbk17X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk18 */
   Dbk18X1              = 0,
   Dbk18X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk19 Bipolar */
   Dbk19BiCJC           = 0,
   Dbk19BiTypeJ         = 1,
   Dbk19BiTypeK         = 1,
   Dbk19BiTypeT         = 2,
   Dbk19BiTypeE         = 0,
   Dbk19BiTypeN28       = 3,
   Dbk19BiTypeN14       = 1,
   Dbk19BiTypeS         = 3,
   Dbk19BiTypeR         = 2,
   Dbk19BiTypeB         = 3,

   /* PCCard & DaqBoard/2000 Dbk19 Bipolar */
   /* place main unit in -5 to +5v range  (additional x2 gain) */
   /* bipolar gains only */
   DbkPCC19BiCJC        = 4,
   DbkPCC19BiTypeJ      = 5,
   DbkPCC19BiTypeK      = 5,
   DbkPCC19BiTypeT      = 6,
   DbkPCC19BiTypeE      = 4,
   DbkPCC19BiTypeN28    = 7,
   DbkPCC19BiTypeN14    = 5,
   DbkPCC19BiTypeS      = 7,
   DbkPCC19BiTypeR      = 6,
   DbkPCC19BiTypeB      = 7,

   /* Dbk19 Unipolar */
   Dbk19UniCJC          = 1,
   Dbk19UniTypeJ        = 2,
   Dbk19UniTypeK        = 2,
   Dbk19UniTypeT        = 3,
   Dbk19UniTypeE        = 1,
   Dbk19UniTypeN28      = 3,
   Dbk19UniTypeN14      = 2,
   Dbk19UniTypeS        = 3,
   Dbk19UniTypeR        = 3,
   Dbk19UniTypeB        = 3,

   /* Dbk42 */
   Dbk42X1              = 0,
   Dbk42X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk43 */
   Dbk43ReadBridge      = 0,
   Dbk43SetOffset       = 1,
   Dbk43SetScalingGain  = 2,
   Dbk43SetInputGain    = 3,

   /* Dbk43 with X2 gain on main unit */
   DbkPCC43ReadBridge      = 4,  // use with PCCard & DaqBoard/2000
   DbkPCC43SetOffset       = 5,  // use with PCCard & DaqBoard/2000
   DbkPCC43SetScalingGain  = 6,  // use with PCCard & DaqBoard/2000
   DbkPCC43SetInputGain    = 7,  // use with PCCard & DaqBoard/2000

   /* Dbk44 */
   Dbk44X1              = 0,
   Dbk44X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk45 */
   Dbk45X1              = 0,
   Dbk45X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk48 */
   Dbk48X1              = 0,
   Dbk48X2              = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000

   /* Dbk50 */
   Dbk50Range0          = 0,
   Dbk50Range10         = 1,
   Dbk50Range100        = 2,
   Dbk50Range300        = 3,
   
   /* Dbk50 with X2 gain on main unit */
   DbkPCC50Range0       = 4,  // use with PCCard & DaqBoard/2000
   DbkPCC50Range10      = 5,  // use with PCCard & DaqBoard/2000
   DbkPCC50Range100     = 6,  // use with PCCard & DaqBoard/2000
   DbkPCC50Range300     = 7,  // use with PCCard & DaqBoard/2000

   /* Dbk51 */
   Dbk51Range0          = 0,
   Dbk51Range100mV      = 1,
   Dbk51Range1          = 2,
   Dbk51Range10         = 3,

   /* Dbk51 with X2 gain on main unit */
   DbkPCC51Range0       = 4,  // use with PCCard & DaqBoard/2000
   DbkPCC51Range100mV   = 5,  // use with PCCard & DaqBoard/2000
   DbkPCC51Range1       = 6,  // use with PCCard & DaqBoard/2000
   DbkPCC51Range10      = 7,  // use with PCCard & DaqBoard/2000

   /* Dbk52 Bipolar */
   Dbk52BiCJC           = Dbk19BiCJC,
   Dbk52BiTypeJ         = Dbk19BiTypeJ,
   Dbk52BiTypeK         = Dbk19BiTypeK,
   Dbk52BiTypeT         = Dbk19BiTypeT,
   Dbk52BiTypeE         = Dbk19BiTypeE,
   Dbk52BiTypeN28       = Dbk19BiTypeN28,
   Dbk52BiTypeN14       = Dbk19BiTypeN14,
   Dbk52BiTypeS         = Dbk19BiTypeS,
   Dbk52BiTypeR         = Dbk19BiTypeR,
   Dbk52BiTypeB         = Dbk19BiTypeB,

   /* PCCard & DaqBoard/2000 Dbk52 Bipolar */
   /* place main unit in -5 to +5v range  (additional x2 gain) */
   /* bipolar gains only */
   DbkPCC52BiCJC        = DbkPCC19BiCJC,
   DbkPCC52BiTypeJ      = DbkPCC19BiTypeJ,
   DbkPCC52BiTypeK      = DbkPCC19BiTypeK,
   DbkPCC52BiTypeT      = DbkPCC19BiTypeT,
   DbkPCC52BiTypeE      = DbkPCC19BiTypeE,
   DbkPCC52BiTypeN28    = DbkPCC19BiTypeN28,
   DbkPCC52BiTypeN14    = DbkPCC19BiTypeN14,
   DbkPCC52BiTypeS      = DbkPCC19BiTypeS,
   DbkPCC52BiTypeR      = DbkPCC19BiTypeR,
   DbkPCC52BiTypeB      = DbkPCC19BiTypeB,

   /* Dbk52 Unipolar */
   Dbk52UniCJC          = Dbk19UniCJC,
   Dbk52UniTypeJ        = Dbk19UniTypeJ,
   Dbk52UniTypeK        = Dbk19UniTypeK,
   Dbk52UniTypeT        = Dbk19UniTypeT,
   Dbk52UniTypeE        = Dbk19UniTypeE,
   Dbk52UniTypeN28      = Dbk19UniTypeN28,
   Dbk52UniTypeN14      = Dbk19UniTypeN14,
   Dbk52UniTypeS        = Dbk19UniTypeS,
   Dbk52UniTypeR        = Dbk19UniTypeR,
   Dbk52UniTypeB        = Dbk19UniTypeB,

   /* Dbk65 */
   Dbk65X1              = 0,
   Dbk65X2              = DgainX2DbkNone, // gain on DaqBook/Board itself
   Dbk65X4              = DgainX4DbkNone,
   Dbk65X8              = DgainX8DbkNone,
   Dbk65X16             = DgainX16DbkNone, // DaqBoard2000 series only
   Dbk65X32             = DgainX32DbkNone, // DaqBoard2000 series only
   Dbk65X64             = DgainX64DbkNone, // DaqBoard2000 series only

   /* Dbk70 */
   Dbk70X1              = 0,
   Dbk70X2              = DgainX2DbkNone,  // Use with PCCard or DaqBoard/2000 and DafBipolar (+/-5V)
                                           // all others with DafUnipolar (or jumper) (0-5V)
   Dbk70X4              = DgainX4DbkNone,  // Use with DaqBoard/2000 and DafUnipolar (0-5V)   

   /* Dbk80 */
   Dbk80X1              = 0,
   Dbk80X2              = DgainX2DbkNone, // gain on DaqBook/Board itself
   Dbk80X4              = DgainX4DbkNone,
   Dbk80X8              = DgainX8DbkNone,
   Dbk80X16             = DgainX16DbkNone, // DaqBoard2000 series only
   Dbk80X32             = DgainX32DbkNone, // DaqBoard2000 series only
   Dbk80X64             = DgainX64DbkNone, // DaqBoard2000 series only

   /* Dbk85 */
   Dbk85X1              = 0,
   Dbk85X2              = DgainX2DbkNone, // gain on DaqBook/Board itself
   Dbk85X4              = DgainX4DbkNone,
   Dbk85X8              = DgainX8DbkNone,
   Dbk85X16             = DgainX16DbkNone, // DaqBoard2000 series only
   Dbk85X32             = DgainX32DbkNone, // DaqBoard2000 series only
   Dbk85X64             = DgainX64DbkNone, // DaqBoard2000 series only

   /* DaqBook/Board 100,112,120,200,216,260 ONLY! */
   /* Dbk81 & Dbk82 - Bipolar Only */
   Dbk81CJC           = 0,
   Dbk81TypeJ         = 0,
   Dbk81TypeK         = 0,
   Dbk81TypeT         = DgainX2DbkNone,
   Dbk81TypeE         = 0,
   Dbk81TypeN28       = DgainX2DbkNone,
   Dbk81TypeN14       = 0,
   Dbk81TypeS         = DgainX2DbkNone,
   Dbk81TypeR         = DgainX2DbkNone,
   Dbk81TypeB         = DgainX2DbkNone,

   Dbk81x100          = 0, // Voltage Mode (+/- 50mV) Actual gain of 99.915 gives +/-50.043 mV

   /* PCCard & DaqBoard/2000 Dbk81 & Dbk82 - Bipolar Only */
   /* places main unit in -5 to +5v range  (additional x2 gain) */
   /* bipolar gains only */
   DbkPCC81CJC        = DgainX2DbkNone,
   DbkPCC81TypeJ      = DgainX2DbkNone,
   DbkPCC81TypeK      = DgainX2DbkNone,
   DbkPCC81TypeT      = DgainX4DbkNone,
   DbkPCC81TypeE      = DgainX2DbkNone,
   DbkPCC81TypeN28    = DgainX4DbkNone,
   DbkPCC81TypeN14    = DgainX2DbkNone,
   DbkPCC81TypeS      = DgainX4DbkNone,
   DbkPCC81TypeR      = DgainX4DbkNone,
   DbkPCC81TypeB      = DgainX4DbkNone,

   DbkPCC81x100       = DgainX1DbkNone, // Voltage Mode (+/- 100 mV) Actual gain of 99.915 gives +/- 100.085mV

   /* DaqBook/2000 Dbk90 - Bipolar Only */
   Dbk90CJC           = DgainX2DbkNone,
   Dbk90TypeJ         = DgainX2DbkNone,
   Dbk90TypeK         = DgainX2DbkNone,
   Dbk90TypeT         = DgainX4DbkNone,
   Dbk90TypeE         = DgainX2DbkNone,
   Dbk90TypeN28       = DgainX4DbkNone,
   Dbk90TypeN14       = DgainX2DbkNone,
   Dbk90TypeS         = DgainX4DbkNone,
   Dbk90TypeR         = DgainX4DbkNone,
   Dbk90TypeB         = DgainX4DbkNone,

   /* Dbk207 */
   Dbk207X1             = 0,
   Dbk207X2             = DgainX2DbkNone,  // use with PCCard & DaqBoard/2000


   /* Wavebook gain codes */	
   WgcX1                = 0,
   WgcX2                = 1,
   WgcX5                = 2,
   WgcX10               = 3,
   WgcX20               = 4, // Wbk11, 12, 13, & Wbk14 only
   WgcX50               = 5, // Wbk11, 12, 13, & Wbk14 only
   WgcX100              = 6, // Wbk11, 12, 13, & Wbk14 only
   WgcX200              = 7, // Wbk10A with Wbk11,12,or 13 installed, & Wbk14 only
   WgcX0_2				= 8, // Wbk18 Gain = 0.2 DC Couple only Range = -25V to 25V

   // WaveBook digital gain codes NOT currently used! : Use WgcX1 and set DaqAdcFlags as required
   WgcDigital8          = 8, // 8-Bit Digital              : proper DaqAdcFlags also required
   WgcDigital16         = 9, // 16-Bit Digital             : proper DaqAdcFlags also required
   WgcCtr16             = 10,// 16-Bit Countet/Timer       : proper DaqAdcFlags also required
   WgcCtr32Low          = 11,// 32-Bit Counter (High Byte) : proper DaqAdcFlags also required
   WgcCtr32High         = 12,// 32-Bit Counter (Low Byte)  : proper DaqAdcFlags also required
 
   /* TempBook/66 voltage gain codes */
   TgainX1                = 0,
   TgainX2                = 1,
   TgainX5                = 2,
   TgainX10               = 3,
   TgainX20               = 5,
   TgainX50               = 6,
   TgainX100              = 7,
   TgainX200              = 11,

   /* TempBook/66 Thermocouple Bipolar */
   TbkBiCJC               = TgainX50,
   TbkBiTypeJ             = TgainX100,
   TbkBiTypeK             = TgainX100,
   TbkBiTypeT             = TgainX200,
   TbkBiTypeE             = TgainX50,
   TbkBiTypeN28           = TgainX100,
   TbkBiTypeN14           = TgainX100,
   TbkBiTypeS             = TgainX200,
   TbkBiTypeR             = TgainX200,
   TbkBiTypeB             = TgainX200,
   
   /* TempBook/66 Thermocouple Unipolar */
   TbkUniCJC              = TgainX100,
   TbkUniTypeJ            = TgainX200,
   TbkUniTypeK            = TgainX200,
   TbkUniTypeT            = TgainX200,
   TbkUniTypeE            = TgainX100,
   TbkUniTypeN28          = TgainX200,
   TbkUniTypeN14          = TgainX200,
   TbkUniTypeS            = TgainX200,
   TbkUniTypeR            = TgainX200,
   TbkUniTypeB            = TgainX200,

   /* pDaq Gain Types */
   PgainDiv5              = 8,
   PgainX1                = 0,
   PgainX2                = 1,
   PgainX4                = 16,
   PgainX8                = 17,
   PgainX16               = 18,
   PgainX32               = 19,
   PgainX64               = 20,
   PgainX128              = 21,

} DaqAdcGain;


/* ADC Flag Definitions */
typedef enum {

   /* Unipolar/Bipolar Flag */
   DafUnipolar          = 0x00,
   DafBipolar           = 0x02,

   /* Unsigned/Signed ADC Data Flag */
   DafUnsigned          = 0x00,
   DafSigned            = 0x04,

   /* ADC Self Calibration Channel Flag */
   DafSelfCal           = 0x8000,

   /* Single Ended/Differential Flag */
   DafSingleEnded       = 0x00,
   DafDifferential      = 0x08,
   DafSingleEndedLow    = 0x0000,  // pDaq Type
   DafSingleEndedHigh   = 0x1000,  // pDaq Type

   /* SSH Hold/Sample Flag - For Internal Use Only */
   DafSSHSample         = 0x00,
   DafSSHHold           = 0x10,

   /* Analog/High Speed Digital Flag */
   DafAnalog            = 0x00,
   DafHighSpeedDigital  = 0x01,
   /* pDaq Digital or Counter Flag */
   DafScanDigital       = 0x01, 
   /* WaveBook Digital Channel Flags */   
   DafDigital8          = 0x001,
   DafDigital16         = 0x101,   
   /* Daq2000 P2/P3 Digital Channel Flags */
   DafP2Local8          = 0x2001,
   DafP2Exp8            = 0x4001,
   DafP3Local16         = 0x0001,
 
   /* WaveBook & Daq2000 Counter Channel Flags */   
   DafCtr16             = 0x201,
   DafCtr32Low          = 0x401,
   DafCtr32High         = 0x801,
   /* Daq2000 Counter Edge Flags */
   DafCtrRisingEdge     = 0x00000,
   DafCtrFallingEdge    = 0x10000,
   /* pDaq & Daq2000 Counter Types */
   DafCtrPulse          = 0x20000,
   DafCtrTotalize       = 0x40000,
   /* pDaq Digital and Counter Types */
   DafDioDirect         = 0x00000,
   DafCtrFreq           = 0x80000,
   DafCtrDutyLo         = 0x100000,
   DafCtrDutyHi         = 0x200000,

   /* pDaq Notch Frequencies */
   DafMeasDuration610   = 0x000000,
   DafMeasDuration370   = 0x100000,
   DafMeasDuration310   = 0x200000,
   DafMeasDuration130   = 0x300000,
   DafMeasDuration110   = 0x400000,
   DafMeasDuration40    = 0x500000,
   DafMeasDuration20    = 0x600000,
   DafMeasDuration12_5  = 0x700000,
   
   /* Daq Series Settling Time Control */
   DafSettle1us         = 0x1800000,
   DafSettle5us         = 0x0000000,
   DafSettle10us        = 0x0800000,
   DafSettle20us        = 0x1000000,
   DafSettle1ms			= 0x2000000,

   /* Clear or shift the least significant nibble - typically used with 12-bit ADCs */
   DafIgnoreLSNibble    = 0x00,
   DafClearLSNibble     = 0x20,
   DafShiftLSNibble     = 0x40,
   /*Shift the least significant Digital Byte - typically used with 8-bit WaveBook DIO port*/
   DafDigitalShiftLSByte =	0x40,

   /* pDaq, TempBook, & DBK19/52 Thermocouple Type codes */
   DafTcTypeNone        = 0x00,
   DafTcTypeJ           = 0x80,
   DafTcTypeK           = 0x100,
   DafTcTypeT           = 0x180,
   DafTcTypeE           = 0x200,
   DafTcTypeN28         = 0x280,
   DafTcTypeN14         = 0x300,
   DafTcTypeS           = 0x380,
   DafTcTypeR           = 0x400,
   DafTcTypeB           = 0x480,
   DafTcCJC             = 0x500,

   // 64 Channel expansion 
   Daf64ChannelExp      = 0x800,

   /* WaveBook Internal Channel Flags */  
   DafIgnoreType        = 0x1000000,

   // ETAS Scan list info
   DafETASInfo			= 0x04000000,
   DafETASMask			= 0x74000001,
   DafETASTimeStampLow	= 0x14000001,
   DafETASTimeStampHigh	= 0x24000001,
   DafETASSenderIDLow	= 0x34000001,
   DafETASSenderIDHigh	= 0x44000001,
   DafETASTopologyReg	= 0x54000001,

   } DaqAdcFlag;

typedef enum {
	DarUni0to10V      = 0, // Unipolar  0 to +10 Volt range 
	DarBiMinus5to5V   = 1, // Bipolar  -5 to  +5 Volt range 
	DarUniPolarDE     = 0, // Unipolar Differential
	DarBiPolarDE      = 1, // Bipolar Differential
	DarUniPolarSE     = 2, // Unipolar Single Ended
	DarBiPolarSE      = 3, // Bipolar Single Ended
} DaqAdcRangeT;

/* Custom ADC Acquisition Prototypes - Scan Sequence */
DAQAPI DaqError WINAPI  daqAdcSetMux(DaqHandleT handle, DWORD startChan, DWORD endChan, DaqAdcGain gain,
                           DWORD flags);
DAQAPI DaqError WINAPI  daqAdcSetScan(DaqHandleT handle, PDWORD channels, DaqAdcGain *gains, PDWORD flags,
                           DWORD chanCount);
DAQAPI DaqError WINAPI  daqAdcGetScan(DaqHandleT handle, PDWORD channels, DaqAdcGain *gains, PDWORD flags,
                           PDWORD chanCount);

/* Channel Type Definitions for Trigger Calculations*/
typedef enum {

	DaqTypeAnalogLocal   = 0,
	DaqTypeDigitalLocal  = 100000,
	DaqTypeDigitalExp    = 200000,
	DaqTypeCounterLocal  = 400000,

	DaqTypeDBK1		= 1,
	
	DaqTypeDBK4		= 4,
	
	DaqTypeDBK6		= 6,
	DaqTypeDBK7		= 7,
	DaqTypeDBK8		= 8,
	DaqTypeDBK9		= 9,
	
	DaqTypeDBK12	= 12,
	DaqTypeDBK13	= 13,
	DaqTypeDBK14	= 14,
	DaqTypeDBK15	= 15,
	DaqTypeDBK16	= 16,
	DaqTypeDBK17	= 17,
	DaqTypeDBK18	= 18,
	DaqTypeDBK19	= 19,
	
	DaqTypeDBK20	= 20,
	DaqTypeDBK21	= 21,
	DaqTypeDBK23	= 23,
	DaqTypeDBK24	= 24,
	DaqTypeDBK25	= 25,
	
	DaqTypeDBK42	= 42,
	DaqTypeDBK43	= 43,
	DaqTypeDBK44	= 44,
	DaqTypeDBK45	= 45,

	DaqTypeDBK48	= 48,

	DaqTypeDBK50	= 50,
	DaqTypeDBK51	= 51,
	DaqTypeDBK52	= 52,
	DaqTypeDBK53	= 53,
	DaqTypeDBK54	= 54,
	DaqTypeDBK55    = DaqTypeDBK7,
	DaqTypeDBK56	= 56,
    DaqTypeDBK70	= 70,

	DaqTypeDBK80	= 80,
	DaqTypeDBK81	= 81,
	DaqTypeDBK82	= 82,
	DaqTypeDBK85	= DaqTypeDBK80,
	DaqTypeDBK90	= 90,

	DaqTypeDBK65	= DaqTypeDBK80,

	DaqTypeDBK207	= 207,
	DaqTypeDBK208	= 208,
	DaqTypeDBK210	= 210,
	
} DaqChannelType;

/* Trigger Event Flags  */
typedef enum {
	DaqStartEvent	= 0,
	DaqStopEvent	= 1,
} DaqTriggerEvent;

/* ADC Trigger Source Definitions */
typedef enum {
   DatsImmediate        = 0,
   DatsSoftware         = 1,
   DatsAdcClock         = 2,
   DatsGatedAdcClock    = 3,
   DatsExternalTTL      = 4,
   DatsHardwareAnalog   = 5,
   DatsSoftwareAnalog   = 6,
   DatsEnhancedTrig     = 7,  // WaveBook series only
   DatsDigPattern       = 8,
   DatsPulse            = 9,  // WaveBook/516 only
   DatsScanCount        = 10, // Stop Event only
   DatsCounter          = 6,  // DaqBoard2000 series only

} DaqAdcTriggerSource;

/* Enhanced Trigger Sense Definitions */
typedef enum {
   DetsRisingEdge                 =   0,
   DetsFallingEdge                =   1,
   DetsAboveLevel                 =   2,
   DetsBelowLevel                 =   3,
   DetsAfterRisingEdge            =   4,
   DetsAfterFallingEdge           =   5,
   DetsAfterAboveLevel            =   6,
   DetsAfterBelowLevel            =   7,
   DetsEQLevel                    =   8,
   DetsNELevel                    =   9,  
   /* WaveBook/516 */
   DetsWindowAboveLevelBeforeTime =   10,
   DetsWindowAboveLevelAfterTime  =   11,
   DetsWindowBelowLevelBeforeTime =   12,
   DetsWindowBelowLevelAfterTime  =   13,

} DaqEnhTrigSensT;

/* Custom ADC Acquisition Prototypes - Trigger */

/* Daq* function to set acquisition start and stop events with built in level calculations */
/* Replaces the daqAdcCalcTrig, daqAdcSetTrig combination */
DAQAPI DaqError WINAPI  daqSetTriggerEvent(DaqHandleT handle, DaqAdcTriggerSource trigSource,
                           DaqEnhTrigSensT trigSensitivity, DWORD channel, DaqAdcGain gainCode, DWORD flags,
                           DaqChannelType channelType, FLOAT level, FLOAT variance, DaqTriggerEvent event);
/* WaveBook function for setting complex triggering */
DAQAPI DaqError WINAPI  daqAdcSetTrigEnhanced(DaqHandleT handle, DaqAdcTriggerSource *trigSources,
                           DaqAdcGain *gains, DaqAdcRangeT *adcRanges, DaqEnhTrigSensT *trigSensitivity, PFLOAT level,
                           PFLOAT hysteresis, PDWORD channels, DWORD chanCount, char *opStr);
/* Calculate trigger level in counts  for daqAdcSetTrig */
DAQAPI DaqError WINAPI  daqAdcCalcTrig(DaqHandleT handle, BOOL bipolar, FLOAT gainVal, FLOAT voltageLevel,
                           PWORD triggerLevel);
/* Recommended for Immediate or Software Trigger Only */
DAQAPI DaqError WINAPI  daqAdcSetTrig(DaqHandleT handle, DaqAdcTriggerSource triggerSource, BOOL rising,
                           WORD level, WORD hysteresis, DWORD channel);
/* Send Trigger Command */
DAQAPI DaqError WINAPI  daqAdcSoftTrig(DaqHandleT handle);



/* ADC Clock Source Definitions */
typedef enum {
   DacsAdcClock         = 0,
   DacsGatedAdcClock    = 1,
   DacsTriggerSource    = 2,
   DacsExternalTTL      = 3,
   DacsAdcClockDiv2     = 4,
   /* Daq2000 External Clock Detection Flags*/
   DacsRisingEdge       = 0,
   DacsFallingEdge      = 0x100,
   /* Daq2000 Internal Clock Output Control Flags*/
   DacsOutputDisable    = 0,
   DacsOutputEnable     = 0x1000,
   /* WaveBook/516A,512A,516E and DaqBook/2000A,X,E Master/Slave Control */
   DacsSyncMaster       = 0x2000,   // RJ-11 External Trigger and Clock Driven as Outputs
   DacsSyncSlave        = 0x4000,   // RJ-11 External Trigger and Clock Set to Inputs
} DaqAdcClockSource;

/* Setup and Retrieve Freq. or Period */
typedef enum {
   DarmPeriod           = 0,
   DarmFrequency        = 1,
   DarmExtClockPacer    = 2,
   DarmTTLPacer         = 3,
} DaqAdcRateMode;

typedef enum {
   DaasPreTrig          = 0,
   DaasPostTrig         = 1,
} DaqAdcAcqState;

/* Custom ADC Acquisition Prototypes - Set Clock Source and Scan Rate */
DAQAPI DaqError WINAPI  daqAdcSetClockSource(DaqHandleT handle, DaqAdcClockSource clockSource);
DAQAPI DaqError WINAPI  daqAdcSetRate(DaqHandleT handle, DaqAdcRateMode mode, DaqAdcAcqState state, FLOAT reqValue, PFLOAT actualValue);
/* Superceded by daqAdcSetRate */
DAQAPI DaqError WINAPI  daqAdcSetFreq(DaqHandleT handle, FLOAT freq);
DAQAPI DaqError WINAPI  daqAdcGetFreq(DaqHandleT handle, PFLOAT freq);


/* ADC Acquisition Mode Definitions */
typedef enum {
   DaamNShot            = 0,
   DaamNShotRearm       = 1, // WaveBook only
   DaamInfinitePost     = 2,
   DaamPrePost          = 3,
} DaqAdcAcqMode;

/* Custom ADC Acquisition Prototypes - Scan Count, Rate and Source */
DAQAPI DaqError WINAPI  daqAdcSetAcq(DaqHandleT handle, DaqAdcAcqMode mode, DWORD preTrigCount,
                           DWORD postTrigCount);

/* ADC File Open Mode Definitions */
typedef enum {
   DaomAppendFile       = 0,
   DaomWriteFile        = 1,
   DaomCreateFile       = 2,
} DaqAdcOpenMode;

/* Custom ADC Acquisition Prototypes - Direct-to-Disk */
DAQAPI DaqError WINAPI  daqAdcSetDiskFile(DaqHandleT handle, LPSTR filename, DaqAdcOpenMode openMode,
                           DWORD preWrite);

/* Custom ADC Acquisition Prototypes - Acquisition Control */
DAQAPI DaqError WINAPI  daqAdcArm(DaqHandleT handle);
DAQAPI DaqError WINAPI  daqAdcDisarm(DaqHandleT handle);

/* ADC Transfer Mask Definitions */
typedef enum {
   DatmCycleOff         = 0x00,
   DatmCycleOn          = 0x01,

   DatmUpdateBlock      = 0x00,
   DatmUpdateSingle     = 0x02,   
   DatmUpdateAny        = 0x04,

   DatmUserBuf          = 0x00,
   DatmDriverBuf        = 0x08,
   DatmIgnoreOverruns   = 0x10,

   // WaveBook Only : Enable user-buffer overflow protection. NOTE: This mode 
   // changes the usage of the active and retCount arguments of daqGetTransferStat
   DatmPacingMode       = 0x20,
   // Special forced update for DaqBoard1000/2000 
   DatmUpdateUser             = 0x40,

} DaqAdcTransferMask;

/* ADC Acquisition/Transfer Active Flag Definitions */
typedef enum {
   DaafAcqActive        = 0x01,  // DaafAcqArmed || DaafAcqDataPresent
   DaafAcqTriggered     = 0x02,
   DaafTransferActive   = 0x04,
   DaafAcqArmed         = 0x08,  // WaveBook Only
   DaafAcqDataPresent   = 0x10   // WaveBook Only : Data In Hardware FIFO

} DaqAdcActiveFlag;

/* Custom ADC Acquisition Prototypes - Data Transfer without Buffer Allocation */
DAQAPI DaqError WINAPI  daqAdcTransferSetBuffer(DaqHandleT handle, PWORD buf, DWORD scanCount,
                           DWORD transferMask);
DAQAPI DaqError WINAPI  daqAdcTransferStart(DaqHandleT handle);
DAQAPI DaqError WINAPI  daqAdcTransferGetStat(DaqHandleT handle,
                           PDWORD active, PDWORD retCount);
DAQAPI DaqError WINAPI  daqAdcTransferStop(DaqHandleT handle);

// Driver Buffer Retrieval Flags
typedef enum {
  DabtmOldest        = 0x00,
  DabtmNewest        = 0x01,
  DabtmWait          = 0x00,
  DabtmRetAvail      = 0x02,
  DabtmNoWait        = 0x04,
  DabtmRetNotDone    = 0x08,
} DaqAdcBufferXferMask;

// This function is used to request driver buffer data
DAQAPI DaqError WINAPI daqAdcTransferBufData(DaqHandleT handle, PWORD buf, DWORD scanCount, DaqAdcBufferXferMask bufMask, PDWORD retCount);


/* Custom ADC Acquisition Prototypes - Buffer Manipulation */
DAQAPI DaqError WINAPI  daqAdcBufferRotate(DaqHandleT handle, PWORD buf, DWORD scanCount, DWORD chanCount,
                           DWORD retCount);

/* One-Step ADC Acquisition Prototypes */
DAQAPI DaqError WINAPI  daqAdcRd(DaqHandleT handle, DWORD chan, PWORD sample, DaqAdcGain gain,
                           DWORD flags);
DAQAPI DaqError WINAPI  daqAdcRdScan(DaqHandleT handle, DWORD startChan, DWORD endChan,
                           PWORD buf, DaqAdcGain gain, DWORD flags);
DAQAPI DaqError WINAPI  daqAdcRdN(DaqHandleT handle, DWORD chan, PWORD buf, DWORD scanCount,
                           DaqAdcTriggerSource triggerSource, BOOL rising, WORD level, FLOAT freq,
                           DaqAdcGain gain, DWORD flags);
DAQAPI DaqError WINAPI  daqAdcRdScanN(DaqHandleT handle, DWORD startChan, DWORD endChan, PWORD buf,
                           DWORD scanCount, DaqAdcTriggerSource triggerSource, BOOL rising, WORD level,
                           FLOAT freq, DaqAdcGain gain, DWORD flags);

typedef enum {
   DardfNative     = 0,
   DardfPacked     = 1, /* WaveBook/512 & 512H Only */
   DardfFloat      = 2, /* Personal Daq Only */
} DaqAdcRawDataFormatT;

typedef enum {
   DappdfRaw        = 0,
   DappdfTenthsDegC = 1,   /* Used to read thermocouple data with the TempBook/66 via One-Step functions (daqAdcRd) */
} DaqAdcPostProcDataFormatT;

// Sets the data formats for raw and post-processed data : 12-bit WaveBook Data Packing
DAQAPI DaqError WINAPI  daqAdcSetDataFormat(DaqHandleT handle, DaqAdcRawDataFormatT rawFormat,
                           DaqAdcPostProcDataFormatT postProcFormat);

/* Type for raw data format conversion action */
typedef enum {
   DacaUnpack = 0,
   DacaPack =   1,
   DacaRotate = 2,
} DaqAdcCvtAction;

// converts data format base on the action variable
DAQAPI DaqError WINAPI daqCvtRawDataFormat(PWORD buf, DaqAdcCvtAction action, DWORD lastRetCount,
                          DWORD scanCount, DWORD chanCount);


/*****************************************************************/
/*                                                               */
/*                 DAC Definitions and Prototypes                */
/*                                                               */
/*****************************************************************/

/* DAC Device Type Definitions */
typedef enum {
   DddtLocal            = 0,	
   DddtDbk              = 1,
   /* Daq2000 Digital Streaming Control */   
   DddtLocalDigital     = 2,
} DaqDacDeviceType;

/* DAC Output Mode Definitions */
typedef enum {
   DdomVoltage          = 0,
   DdomStaticWave       = 1,
   DdomDynamicWave      = 2,   
   /* Daq2000 Digital Streaming Control */
   DdomDigitalDirect    = 0,
   /* Daq2000 Unsigned/Signed Data Format Flags */
   DdomUnsigned         = 0x0,
   DdomSigned           = 0x4,
   /* DaqBoard/500 Bipolar/Unipolar Data Format Flags */
   DdomUnipolar         = 0x0,
   DdomBipolar          = 0x8,
} DaqDacOutputMode;

/* DAC Global Configuration Prototypes */
DAQAPI DaqError WINAPI  daqDacSetOutputMode(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           DaqDacOutputMode outputMode);

/* DAC Voltage Output Mode Prototypes */
DAQAPI DaqError WINAPI  daqDacWt(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           WORD dataVal);
DAQAPI DaqError WINAPI  daqDacWtMany(DaqHandleT handle,
                           DaqDacDeviceType *deviceTypes, PDWORD chans,
                           PWORD dataVals, DWORD count);

/* DAC Trigger Source Definitions */
typedef enum {
   DdtsImmediate        = 0,
   DdtsSoftware         = 1,
   /* Daq2000 Clock Control */
   DdtsAdcClock         = 2,
} DaqDacTriggerSource;

/* DAC Clock Source Definitions */
typedef enum {
   DdcsDacClock         = 0,
   DdcsGatedDacClock    = 1,
   DdcsAdcClock         = 2,
   DdcsExternalTTL      = 3,
   Ddcs9513Ctr1         = 4,
   /* Daq2000 Clock Control Flags */
   DdcsRisingEdge       = 0x0,
   DdcsFallingEdge      = 0x100,
   /* Daq2000 Output Control Flags */
   DdcsOutputDisable    = 0x0,
   DdcsOutputEnable     = 0x1000,
} DaqDacClockSource;

/* DAC Waveform Mode Definitions */
typedef enum {
   DdwmNShot            = 0,
   DdwmNShotRearm       = 1, // not supported
   DdwmInfinite         = 2,
   DdwmNFileIterations  = 3, // DacWaveDiskFile Mode Only
} DaqDacWaveformMode;

/* DAC Waveform Prototypes - Trigger, Update Rate and Count */
DAQAPI DaqError WINAPI  daqDacWaveSetTrig(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           DaqDacTriggerSource triggerSource, BOOL rising);
DAQAPI DaqError WINAPI  daqDacWaveSoftTrig(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan);
DAQAPI DaqError WINAPI  daqDacWaveSetClockSource(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           DaqDacClockSource clockSource);
DAQAPI DaqError WINAPI  daqDacWaveSetFreq(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           FLOAT freq);
DAQAPI DaqError WINAPI  daqDacWaveGetFreq(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           PFLOAT freq);
DAQAPI DaqError WINAPI  daqDacWaveSetMode(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           DaqDacWaveformMode mode, DWORD updateCount);

/* DAC Predefined Waveform Type Definitions */
typedef enum {
   DdwtSine              = 0,
   DdwtSquare            = 1,
   DdwtTriangle          = 2
} DaqDacWaveType;

/* DAC Transfer Mask Definitions */
typedef enum {
   DdtmCycleOff         = 0x00,  // ignored: always cycle on
   DdtmCycleOn          = 0x01,  // ignored: always cycle on

   DdtmUpdateBlock      = 0x00,  // ignored: always variable
   DdtmUpdateSingle     = 0x02,  // ignored: always variable

   DdtmUserBuffer       = 0x00,
   DdtmDriverBuffer     = 0x04,  // DacWaveDiskFile Mode Only

} DaqDacTransferMask;

/* DAC Disk File Format Types */
typedef enum {
   DdwdfBinaryCounts    = 0x00,  // 16 bit unsigned word - native
   DdwdfBinaryCountsHL  = 0x01,  // high byte/low byte
   DdwdfBinaryFloat     = 0x02,  // float
   DdwdfBinaryDouble    = 0x03,  // double

   DdwdfAsciiCountsDec  = 0x04,  // 0 to 65535
   DdwdfAsciiCountsHex  = 0x05,  // 0x0000 to 0xFFFF
   DdwdfAsciiCountsBin  = 0x06,	// 0 to 1111111111111111
   DdwdfAsciiCountsOct  = 0x07,  // 0 to 177777

   DdwdfAsciiFloat      = 0x08,  // -10.0 to 10.0

} DaqDacWaveFileDataFormat;

/* DAC Waveform Prototypes - Buffer Management */
DAQAPI DaqError WINAPI  daqDacWaveSetBuffer(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           PWORD buf, DWORD scanCount, DWORD transferMask);

DAQAPI DaqError WINAPI  daqDacWaveSetUserWave(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan);

DAQAPI DaqError WINAPI  daqDacWaveSetPredefWave(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           DaqDacWaveType waveType, DWORD amplitude,
                           DWORD offset, DWORD dutyCycle, DWORD phaseShift);

DAQAPI DaqError WINAPI  daqDacWaveSetDiskFile(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan, LPSTR filename,
                           DWORD numUpdateCycles,		// Number of update cycles to read from file (0 = all)
                           DWORD offsetBytes,	      // offset for header in BYTES.
                           DWORD offsetUpdateCycles,	// Number of update cycles to skip at the start of the file
                           DaqDacWaveFileDataFormat dataFormat); // format of data file
                           /* File MUST contain exact number of enabled waveform channels */
                           /* 1 update cycle = 1 output per each channel simultaneously */

/* DAC Waveform Prototypes - Waveform Control */
DAQAPI DaqError WINAPI  daqDacWaveArm(DaqHandleT handle, DaqDacDeviceType deviceType);
DAQAPI DaqError WINAPI  daqDacWaveDisarm(DaqHandleT handle, DaqDacDeviceType deviceType);


/* DAC Waveform/Transfer Active Flag Definitions */
typedef enum {
   DdafWaveformActive   = 0x01,
   DdafWaveformTriggered= 0x02,
   DdafTransferActive   = 0x04,
   DdafUnderrun         = 0x08,
} DaqDacActiveFlag;

/* DAC Transfer Prototypes - Dynamic Waveform Data Transfer */
DAQAPI DaqError WINAPI  daqDacTransferStart(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan);
DAQAPI DaqError WINAPI  daqDacTransferGetStat(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan,
                           PDWORD active, PDWORD retCount);
DAQAPI DaqError WINAPI  daqDacTransferStop(DaqHandleT handle,
                           DaqDacDeviceType deviceType, DWORD chan);

/*****************************************************************/
/*                                                               */
/*          Data Conversion Definitions and Prototypes           */
/*                                                               */
/*****************************************************************/

/* Sets A/D min and max range for conversion routines */
DAQAPI DaqError WINAPI  daqCvtSetAdcRange(FLOAT Admin, FLOAT Admax);

/* Linear Conversion Prototypes : all require daqCvtSetAdcRange previously called */
DAQAPI DaqError WINAPI  daqCvtLinearSetup(DWORD nscan, DWORD readingsPos, DWORD nReadings,
                           FLOAT signal1, FLOAT voltage1,
                           FLOAT signal2, FLOAT voltage2, DWORD avg);
DAQAPI DaqError WINAPI  daqCvtLinearConvert(PWORD counts, DWORD scans, PFLOAT fValues, DWORD nValues);
DAQAPI DaqError WINAPI  daqCvtLinearSetupConvert(DWORD nscan, DWORD readingsPos, DWORD nReadings,
                           FLOAT signal1, FLOAT voltage1, FLOAT signal2, FLOAT voltage2,
                           DWORD avg, PWORD counts, DWORD scans, PFLOAT fValues, DWORD nValues);

/* Software Calibration Type Definitions */
typedef enum {
   DcalTypeDefault      = 0,
   DcalTypeCJC          = 1,  /* channel to be calibrated is a real CJC reading - not a CJC zero reading */
   DcalDbk4Bypass       = 2,  /* channel to be calibrated using the methods and structures               */
                              /* for a Dbk4 with the filters bypassed (set by jumper on the card).       */
   DcalDbk4Filter       = 3,  /* channel to be calibrated using the methods and sturctures               */
                              /* for a Dbk4 with the cutoff filters enabled.                             */
} DcalType;

typedef enum {
   DciNormal            = 0,  /* External signal from device input connector(s) */
   DciCalGround         = 1,  /* Internal calibration ground signal    */
   DciCal5V             = 2,  /* Internal 5V calibration signal        */
   DciCal500mV          = 3,  /* Internal 500mV calibration signal     */
} DaqCalInputT;

typedef enum {
   DcttFactory          = 0,  /* Factory calibration constants */
   DcttUser             = 1,  /* User-defined calibration constants */
   DcttBaseline			= 2,  /* Unity Gain - No Offset */
} DaqCalTableTypeT;

typedef enum {
	DcoGetConstants         = 0,
	DcoSetConstants         = 1,
	DcoSelectInput          = 2,
	DcoSelectTable          = 3,
	DcoSaveConstants        = 4,
	DcoGetCalPtr            = 5,
	DCoSetResponseDac       = 6,
	DcoSaveTable            = 7,
	DcoGetRefDacConstants   = 8,
	DcoSetRefDacConstants   = 9,
	DcoGetTrigDacConstants  = 10,
	DcoSetTrigDacConstants  = 11,
} DaqCalOperationT;

typedef enum {
   DcrcPosRefDac           = 0,  // positive reference DAC channel
   DcrcNegRefDac           = 1,  // negative reference DAC channel
} DaqCalRefDacChannelT;

typedef enum {
   CctdcTrigThresholdDac	= 0,  // Trigger Threshold DAC
   CctdcTrigHysteresisDac	= 1,  // Trigger Hysteresis DAC
   CctdcTrigBelowLevel		= 0,  // Trigger Value Below DAC Value
   CctdcTrigAboveLevel		= 2,  // Trigger Value Above DAC Value
} DaqCalTrigDacChannelT;

typedef enum {
   DcoptMainUnit           = 0,
   DcoptOptionUnit         = 1,
   DcoptUserCal            = 2,
   DcoptSensorCal          = 3,
   DcoptReserved1          = 4,
   DcoptReserved2          = 5,
   DcoptReserved3          = 6,
   DcoptReserved           = 7,
} DaqCalOptionT;


/* Software Calibration Prototypes */
DAQAPI DaqError WINAPI  daqCalSetup(DaqHandleT handle,DWORD nscan, DWORD readingsPos, DWORD nReadings,
                           DcalType chanType, DaqAdcGain chanGain, DWORD startChan, BOOL bipolar,
                           BOOL noOffset);
DAQAPI DaqError WINAPI  daqCalConvert(DaqHandleT handle, PWORD counts, DWORD scans);
DAQAPI DaqError WINAPI  daqCalSetupConvert(DaqHandleT handle, DWORD nscan, DWORD readingsPos,
                           DWORD nReadings, DcalType chanType, DaqAdcGain chanGain, DWORD startChan,
                           BOOL bipolar, BOOL noOffset, PWORD counts, DWORD scans);
DAQAPI DaqError WINAPI  daqReadCalFile(DaqHandleT handle,LPSTR calfile);

DAQAPI DaqError WINAPI  daqCalSelectInputSignal(DaqHandleT handle, DaqCalInputT input);
DAQAPI DaqError WINAPI  daqCalSelectCalTable(DaqHandleT handle, DaqCalTableTypeT tableType);
DAQAPI DaqError WINAPI  daqCalSetConstants(DaqHandleT handle, DWORD channel, DaqAdcGain gain,
                           DaqAdcRangeT range, WORD gainConstant, SHORT offsetConstant);
DAQAPI DaqError WINAPI  daqCalGetConstants(DaqHandleT handle, DWORD channel, DaqAdcGain gain,
                           DaqAdcRangeT range, PWORD gainConstant, PSHORT offsetConstant);
DAQAPI DaqError WINAPI  daqCalSaveConstants(DaqHandleT handle, DWORD channel);

DAQAPI DaqError WINAPI  daqConfCalConstants(DaqHandleT handle, DWORD chan, DaqAdcGain gain,
                                            DaqAdcRangeT adcRange, PWORD gainConst,
                                            PSHORT offsetConst, DaqCalOperationT operation,
                                            DaqCalTableTypeT tableType, DaqCalInputT input,
                                            DaqCalOptionT option);

/* Zero Compensation Definitions */
typedef enum {
   DazcNone          = 0,  /* Do not include auto-zero compensation in TC conversion. */
   DazcAutoZero      = 1,  /* Apply auto-zero compensation in TC conversion.          */
} DaqAutoZeroCompT;

/* Zero Compensation Prototypes */
DAQAPI DaqError WINAPI  daqAutoZeroCompensate(DaqAutoZeroCompT zero);

DAQAPI DaqError WINAPI  daqZeroSetup(DWORD nscan, DWORD zeroPos, DWORD readingsPos,
                           DWORD nReadings);
DAQAPI DaqError WINAPI  daqZeroConvert(PWORD counts, DWORD scans);
DAQAPI DaqError WINAPI  daqZeroSetupConvert(DWORD nscan, DWORD zeroPos,
                           DWORD readingsPos, DWORD nReadings, PWORD counts, DWORD scans);
DAQAPI DaqError WINAPI  daqZeroDbk19(BOOL zero);

/* RTD Type Definitions */
typedef enum {
   Dbk9RtdType100       = 0,    /* RTD 100 ohm  Platinum alpha = .00385 */
   Dbk9RtdType500       = 1,    /* RTD 500 ohm  Platinum alpha = .00385 */
   Dbk9RtdType1K        = 2,    /* RTD 1000 ohm Platinum alpha = .00385 */
}  RtdType;

/* Rtd Conversion Prototypes */
DAQAPI DaqError WINAPI  daqCvtRtdConvert(PWORD counts, DWORD scans, PSHORT temp, DWORD ntemp);
DAQAPI DaqError WINAPI  daqCvtRtdSetup(DWORD nScan, DWORD startPosition, DWORD nRtd,
                           RtdType rtdType, DWORD avg);
DAQAPI DaqError WINAPI  daqCvtRtdSetupConvert(DWORD nScan, DWORD startPosition, DWORD nRtd,
                           RtdType rtdType, DWORD avg, PWORD counts, DWORD scans, PSHORT temp, DWORD ntemp);

/* Thermocouple Type Definitions */
typedef enum {
   /* Dbk14 */
   Dbk14TCTypeJ         = 0,
   Dbk14TCTypeK         = 1,
   Dbk14TCTypeT         = 2,
   Dbk14TCTypeE         = 3,
   Dbk14TCTypeN28       = 4,
   Dbk14TCTypeN14       = 5,
   Dbk14TCTypeS         = 6,
   Dbk14TCTypeR         = 7,
   Dbk14TCTypeB         = 8,

   /* Dbk19 */
   Dbk19TCTypeJ         = 9,
   Dbk19TCTypeK         = 10,
   Dbk19TCTypeT         = 11,
   Dbk19TCTypeE         = 12,
   Dbk19TCTypeN28       = 13,
   Dbk19TCTypeN14       = 14,
   Dbk19TCTypeS         = 15,
   Dbk19TCTypeR         = 16,
   Dbk19TCTypeB         = 17,

   /* Dbk52 */
   Dbk52TCTypeJ         = Dbk19TCTypeJ,
   Dbk52TCTypeK         = Dbk19TCTypeK,
   Dbk52TCTypeT         = Dbk19TCTypeT,
   Dbk52TCTypeE         = Dbk19TCTypeE,
   Dbk52TCTypeN28       = Dbk19TCTypeN28,
   Dbk52TCTypeN14       = Dbk19TCTypeN14,
   Dbk52TCTypeS         = Dbk19TCTypeS,
   Dbk52TCTypeR         = Dbk19TCTypeR,
   Dbk52TCTypeB         = Dbk19TCTypeB,

   /* Dbk81 & Dbk82 */   
   Dbk81TCTypeJ         = 27,
   Dbk81TCTypeK         = 28,
   Dbk81TCTypeT         = 29,
   Dbk81TCTypeE         = 30,
   Dbk81TCTypeN28       = 31,
   Dbk81TCTypeN14       = 32,
   Dbk81TCTypeS         = 33,
   Dbk81TCTypeR         = 34,
   Dbk81TCTypeB         = 35,   
   Dbk81CJCType         = 36, // for CJC counts to DegC conversion

  /* Dbk90 */   
   Dbk90TCTypeJ         = Dbk81TCTypeJ,
   Dbk90TCTypeK         = Dbk81TCTypeK,
   Dbk90TCTypeT         = Dbk81TCTypeT,
   Dbk90TCTypeE         = Dbk81TCTypeE,
   Dbk90TCTypeN28       = Dbk81TCTypeN28,
   Dbk90TCTypeN14       = Dbk81TCTypeN14,
   Dbk90TCTypeS         = Dbk81TCTypeS,
   Dbk90TCTypeR         = Dbk81TCTypeR,
   Dbk90TCTypeB         = Dbk81TCTypeB,   
   Dbk90CJCType         = Dbk81CJCType, // for CJC counts to DegC conversion

   IntTempTCTypeJ         = 47,
   IntTempTCTypeK         = 48,
   IntTempTCTypeT         = 49,
   IntTempTCTypeE         = 50,
   IntTempTCTypeN28       = 51,
   IntTempTCTypeN14       = 52,
   IntTempTCTypeS         = 53,
   IntTempTCTypeR         = 54,
   IntTempTCTypeB         = 55,   
   IntTempCJCType         = 56, //

   /* TempBook/66 */
   TbkTCTypeJ           = 18,
   TbkTCTypeK           = 19,
   TbkTCTypeT           = 20,
   TbkTCTypeE           = 21,
   TbkTCTypeN28         = 22,
   TbkTCTypeN14         = 23,
   TbkTCTypeS           = 24,
   TbkTCTypeR           = 25,
   TbkTCTypeB           = 26,
}  TCType;

/* Thermocouple Conversion Prototypes */
DAQAPI DaqError WINAPI  daqCvtTCSetup(DWORD nscan, DWORD cjcPosition, DWORD ntc,
                           TCType tcType, BOOL bipolar, DWORD avg);
DAQAPI DaqError WINAPI  daqCvtTCConvert(PWORD counts, DWORD scans, PSHORT temp, DWORD ntemp);
DAQAPI DaqError WINAPI  daqCvtTCConvertF(PWORD counts, DWORD scans, PFLOAT temp, DWORD ntemp);
DAQAPI DaqError WINAPI  daqCvtTCSetupConvert(DWORD nscan, DWORD cjcPosition, DWORD ntc,
                           TCType tcType, BOOL bipolar, DWORD avg, PWORD counts, DWORD scans, PSHORT temp,
                           DWORD ntemp);
DAQAPI DaqError WINAPI  daqCvtTCSetupConvertF(DWORD nscan, DWORD cjcPosition, DWORD ntc,
                           TCType tcType, BOOL bipolar, DWORD avg, PWORD counts, DWORD scans, PFLOAT temp,
                           DWORD ntemp);


/*****************************************************************/
/*                                                               */
/*            General I/O Definitions and Prototypes             */
/*                                                               */
/*****************************************************************/

/* I/O Device Type Definitions */
typedef enum {
   DiodtLocalBitIO      = 0,
   DiodtLocal8255       = 1,
   DiodtP2Local8        = 12, /* P2 local addressing by byte */
   DiodtP3LocalDig16    = 1,  /* P3 local digital port by word */
   DiodtLocal9513       = 2,
   DiodtP3LocalCtr16    = 2,  /* P3 local counter addressing by word */
   DiodtP2Exp8          = 6,  /* P2 expansion addressing by byte */
   DiodtExp8255         = 3,  /* Dbk20, Dbk21 */
   DiodtDbk23           = 4,
   DiodtDbk24           = 5,
   DiodtDbk25           = 6,
   DiodtDbk208          = 6,
   DiodtDbk210			= 6,
   DiodtExp9513         = 7,  /* Not available */
   DiodtWbk512          = 8,
   DiodtWbk516          = 8,
   DiodtWbk17           = 13,
   DiodtLocal8254       = 9,  /* tempbook only */
   Diodt8254Dig         = 10, /* tempbook only */
   Diodt8254Ctr         = 11, /* tempbook only */
   
} DaqIODeviceType;

/* I/O Port Type Definitions */
typedef enum {
   /* Local Bit I/O */
   DiodpBitIO           = 0,

   /* Local 8254              */
   Diodp8254Trig        = 10,
   Diodp8254A           = 12,
   Diodp8254B           = 13,
   Diodp8254C           = 14,
   Diodp8254IR          = 15,

   /* P2 sequential 8-Bit addressing */
   DiodpP2Local8        = 0,  // I/O
   DiodpP2LocalIR       = 3,  // Config
   DiodpP2Exp8          = 0,  // I/O
   DiodpP2ExpIR         = 3,  // Config

   /* P3 digital port */
   DiodpP3LocalDig16    = 0,  // I/O
   DiodpP3LocalDigIR    = 1,  // Config
   
   /* Local 8255, Dbk20, Dbk21 */
   Diodp8255A           = 0,  // I/O
   Diodp8255B           = 1,  // I/O
   Diodp8255C           = 2,  // I/O
   Diodp8255IR          = 3,  // Config
   Diodp8255CHigh       = 4,  // I/O
   Diodp8255CLow        = 5,  // I/O

   /* Local 9513, Expansion 9513 */
   Diodp9513Command     = 0,
   Diodp9513Data        = 1,
   Diodp9513MasterMode  = 2,
   Diodp9513Alarm1      = 3,
   Diodp9513Alarm2      = 4,
   Diodp9513Status      = 5,
   Diodp9513Mode1       = 6,
   Diodp9513Mode2       = 7,
   Diodp9513Mode3       = 8,
   Diodp9513Mode4       = 9,
   Diodp9513Mode5       = 10,
   Diodp9513Load1       = 11,
   Diodp9513Load2       = 12,
   Diodp9513Load3       = 13,
   Diodp9513Load4       = 14,
   Diodp9513Load5       = 15,
   Diodp9513Hold1       = 16,
   Diodp9513Hold2       = 17,
   Diodp9513Hold3       = 18,
   Diodp9513Hold4       = 19,
   Diodp9513Hold5       = 20,
   Diodp9513Hold1HC     = 21, /* Hold register when in hold cycle mode */
   Diodp9513Hold2HC     = 22, /* Hold register when in hold cycle mode */
   Diodp9513Hold3HC     = 23, /* Hold register when in hold cycle mode */
   Diodp9513Hold4HC     = 24, /* Hold register when in hold cycle mode */
   Diodp9513Hold5HC     = 25, /* Hold register when in hold cycle mode */
   DiodpP3LocalCtr16    = 26,

   /* Dbk23 */
   DiodpDbk23A          = 0,
   DiodpDbk23B          = 1,
   DiodpDbk23C          = 2,
   DiodpDbk23Unused     = 3,

   /* Dbk24 */
   DiodpDbk24A          = 0,
   DiodpDbk24B          = 1,
   DiodpDbk24C          = 2,
   DiodpDbk24Unused     = 3,

   /* Dbk25 */
   DiodpDbk25           = 0,

   /* Dbk208 */
   DiodpDbk208          = 0,

   /* Dbk210 */
   DiodpDbk210			= 0,

   /* WaveBook/512 */
   DiodpWbk512Port0     = 0,
   DiodpWbk512Port1     = 1,
   DiodpWbk512Port2     = 2,
   DiodpWbk512Port3     = 3,
   DiodpWbk512Port4     = 4,
   DiodpWbk512Port5     = 5,
   DiodpWbk512Port6     = 6,
   DiodpWbk512Port7     = 7,
   DiodpWbk512Port8     = 8,
   DiodpWbk512Port9     = 9,
   DiodpWbk512Port10    = 10,
   DiodpWbk512Port11    = 11,
   DiodpWbk512Port12    = 12,
   DiodpWbk512Port13    = 13,
   DiodpWbk512Port14    = 14,
   DiodpWbk512Port15    = 15,
   DiodpWbk512Port16    = 16,
   DiodpWbk512Port17    = 17,
   DiodpWbk512Port18    = 18,
   DiodpWbk512Port19    = 19,
   DiodpWbk512Port20    = 20,
   DiodpWbk512Port21    = 21,
   DiodpWbk512Port22    = 22,
   DiodpWbk512Port23    = 23,
   DiodpWbk512Port24    = 24,
   DiodpWbk512Port25    = 25,
   DiodpWbk512Port26    = 26,
   DiodpWbk512Port27    = 27,
   DiodpWbk512Port28    = 28,
   DiodpWbk512Port29    = 29,
   DiodpWbk512Port30    = 30,
   DiodpWbk512Port31    = 31,

   /* WaveBook/516 */
   DiodpWbk516_8Bit     = 0,
   DiodpWbk516_16Bit    = 32,

   /* WBK17 Digital Output : Write Only */
   DiodpWbk17_8Bit      = 33,

} DaqIODevicePort;

/* I/O Expansion Port Code Definitions */
typedef enum {
   DioepP1              = 0,  /* DigiBook only */
   DioepP2              = 1,
   DioepP3              = 2,

} DaqIOExpansionPort;


// used for Tempbook66 cntr0 configurations
typedef enum {
  Dc0cHighTermCnt       = 0x30,
  Dc0cONeShot           = 0x32,
  Dc0cDivByNCtr         = 0x34,
  Dc0cSquareWave        = 0x36,
  Dc0csoftTrigStrobe    = 0x38,
  Dc0cHardTrigStrobe    = 0x3A,
} DaqCntr0Config;

/* General I/O Prototypes - Read/Write */
DAQAPI DaqError WINAPI  daqIOReadBit(DaqHandleT handle, DaqIODeviceType devType, DaqIODevicePort devPort,
                           DWORD whichDevice, DaqIOExpansionPort whichExpPort, DWORD bitNum, PBOOL bitValue);
DAQAPI DaqError WINAPI  daqIORead(DaqHandleT handle, DaqIODeviceType devType, DaqIODevicePort devPort,
                           DWORD whichDevice, DaqIOExpansionPort whichExpPort, PDWORD value);
DAQAPI DaqError WINAPI  daqIOWriteBit(DaqHandleT handle, DaqIODeviceType devType, DaqIODevicePort devPort,
                           DWORD whichDevice, DaqIOExpansionPort whichExpPort, DWORD bitNum, BOOL bitValue);
DAQAPI DaqError WINAPI  daqIOWrite(DaqHandleT handle, DaqIODeviceType devType, DaqIODevicePort devPort,
                           DWORD whichDevice, DaqIOExpansionPort whichExpPort, DWORD value);
DAQAPI DaqError WINAPI  daqIOGet8255Conf(DaqHandleT handle, BOOL portA, BOOL portB, BOOL portCHigh,
                           BOOL portCLow, PDWORD config);


/*****************************************************************/
/*                                                               */
/*         9513 Counter/Timer Definitions and Prototypes         */
/*                                                               */
/*****************************************************************/


/* 9513 Time-of-Day Definitions */
typedef enum {
   DtodDisabled         = 0,
   DtodDivideBy5        = 1,
   DtodDivideBy6        = 2,
   DtodDivideBy10       = 3
} Daq9513TimeOfDay;

/* 9513 Gating Control Definitions */
typedef enum {
   DgcNoGating          = 0,
   DgcHighTCNM1         = 1,
   DgcHighLevelGateNP1  = 2,
   DgcHighLevelGateNM1  = 3,
   DgcHighLevelGateN    = 4,
   DgcLowLevelGateN     = 5,
   DgcHighEdgeGateN     = 6,
   DgcLowEdgeGateN      = 7
} Daq9513GatingControl;

/* 9513 Count Source Definitions */
typedef enum {
   DcsTcnM1             = 0,  /* not valid with daq9513SetMasterMode or daqCtrRdFreq */
   DcsSrc1              = 1,
   DcsSrc2              = 2,
   DcsSrc3              = 3,
   DcsSrc4              = 4,
   DcsSrc5              = 5,
   DcsGate1             = 6,
   DcsGate2             = 7,
   DcsGate3             = 8,
   DcsGate4             = 9,
   DcsGate5             = 10,  /* not valid with daqCtrRdFreq */
   DcsF1                = 11,  /* not valid with daqCtrRdFreq */
   DcsF2                = 12,  /* not valid with daqCtrRdFreq */
   DcsF3                = 13,  /* not valid with daqCtrRdFreq */
   DcsF4                = 14,  /* not valid with daqCtrRdFreq */
   DcsF5                = 15   /* not valid with daqCtrRdFreq */
} Daq9513CountSource;

/* 9513 Output Control Definitions */
typedef enum {
   DocInactiveLow       = 0,
   DocHighTermCntPulse  = 1,
   DocTCToggled         = 2,
   DocInactiveHighImp   = 3,
   DocLowTermCntPulse   = 4
} Daq9513OutputControl;

/* 9513 Multiple Counter Command Definitions */
typedef enum {
   DmccArm              = 0,
   DmccLoad             = 1,
   DmccLoadArm          = 2,
   DmccDisarmSave       = 3,
   DmccSave             = 4,
   DmccDisarm           = 5
} Daq9513MultCtrCommand;

/* 9513 Counter/Timer Prototypes */
DAQAPI DaqError WINAPI  daq9513SetMasterMode(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD foutDiv, Daq9513CountSource cntSource,
                           BOOL comp1, BOOL comp2, Daq9513TimeOfDay tod);
DAQAPI DaqError WINAPI  daq9513SetAlarm(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD alarmNum, DWORD alarmVal);
DAQAPI DaqError WINAPI  daq9513SetCtrMode(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD ctrNum, Daq9513GatingControl gateCtrl,
                           BOOL cntEdge, Daq9513CountSource cntSource, BOOL specGate, BOOL reload,
                           BOOL cntRepeat, BOOL cntType, BOOL cntDir, Daq9513OutputControl outputCtrl);
DAQAPI DaqError WINAPI  daq9513MultCtrl(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, Daq9513MultCtrCommand ctrCmd, BOOL ctr1, BOOL ctr2,
                           BOOL ctr3, BOOL ctr4, BOOL ctr5);
DAQAPI DaqError WINAPI  daq9513GetHold(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD ctrNum, PWORD ctrVal);
DAQAPI DaqError WINAPI  daq9513SetHold(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD ctrNum, WORD ctrVal);
DAQAPI DaqError WINAPI  daq9513SetLoad(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD ctrNum, WORD ctrVal);
/* WARNING : Unexpected Results : Use standard configuration and measurement functions. */
DAQAPI DaqError WINAPI  daq9513RdFreq(DaqHandleT handle, DaqIODeviceType deviceType,
                           DWORD whichDevice, DWORD interval, Daq9513CountSource cntSource,
                           PDWORD count);

/*****************************************************************/
/*                                                               */
/*  Communications test/performance Definitions and Prototypes   */
/*    - Internally used                                          */
/*                                                               */
/*****************************************************************/

/* DaqTest Command Definitions */
typedef enum {
   DtstBaseAddressValid          = 0,
   DtstInterruptLevelValid       = 1,
   DtstDmaChannelValid           = 2,
   DtstAdcFifoInputSpeed         = 3,
   DtstDacFifoOutputSpeed        = 4,
   DtstIOInputSpeed              = 5,
   DtstIOOutputSpeed             = 6,
   DtstFifoAddrDataBusValid      = 7,
   DtstFifoMemCellValid          = 8,  // MegaFIFO memory test.
   DtstHardwareCompatibility     = 9,  // WaveBook Only
   DtstFirmwareCompatibility     = 10, // WaveBook Only
   DtstExpansionCompatibility    = 11, // WaveBook Only
   DtstExpUpgradeCompatibility   = 12, // WaveBook Only
   DtstScanListFifoValid         = 13,

   // Use these for range checking.  Always update when adding/removing enum's.
   Dtst_ParamRangeMin = DtstBaseAddressValid,         // Lowest valid value
   Dtst_ParamRangeMax = DtstScanListFifoValid,  // Highest valid value

} DaqTestCommand;

/* DaqTest Prototype */
DAQAPI DaqError WINAPI  daqTest(DaqHandleT handle, DaqTestCommand command,
                           DWORD count, PBOOL cmdAvailable, PDWORD result);


/*****************************************************************/
/*                                                               */
/*  Register level IO Definitions and Prototypes                 */
/*    - Internally Used - Not Supported                          */
/*                                                               */
/*  NOTE:  These functions are not intended to be used in user   */
/*         applications. Registers maps are hardware specific,   */
/*         and subject to change at any time.                    */
/*                                                               */
/*****************************************************************/

/* Register IO Defintions */
typedef enum {
   /* DaqBook/DaqBoard output registers */
   DregOutDAC0 =                  0x04,   // DaqBoard (16-bit), DaqBook (low byte)
   DregOutDAC0High =              0x05,   // DaqBook only (high byte)
   DregOutDAC1 =                  0x06,   // DaqBoard (16-bit), DaqBook (low byte)
   DregOutDAC1High =              0x07,   // DaqBook only (high byte)
   DregOuttrigSource =            0x0a,
   DregOutAuxControl =            0x0b,
   DregOut8254IR =                0x0f,
   DregOut8255IR =                0x13,
   DregOutDacFIFO =               0x16,   // DaqBoard only (16-bit)
   DregOutauxControl =            0x19,
   DregOutADCfifo =               0x1a,   // DaqBoard (16-bit), DaqBook (low byte)
   DregOutADCfifoHigh =           0x1b,   // DaqBook only (high byte)

   /* DaqBook/DaqBoard input registers */
   DregInADCfifo =                0x00,   // DaqBoard 16-bit, DaqBook (low byte)
   DregInADCfifoHigh =            0x01,   // DaqBook only (high byte)
   DregInstatReg =                0x08,
   DregInEventStatus =            0x0b,
   DregInEventStatusX =           0x15,   // DaqBoard only
   DregInIntIDX =                 0x18,   // DaqBoard only
   DregInIntID =                  0x1f,

   /* DaqBook/DaqBoard input/output registers */
   DregBitDIO =                   0x03,
   DregDACctrl =                  0x09,   // DaqBoard only
   Dreg8254C0 =                   0x0c,
   Dreg8254C1 =                   0x0d,
   Dreg8254C2 =                   0x0e,
   Dreg8255A =                    0x10,
   Dreg8255B =                    0x11,
   Dreg8255C =                    0x12,
   DregIntMaskX =                 0x14,   // DaqBoard only
   Dreg9513Data =                 0x1c,
   Dreg9513IR =                   0x1d,
   DregIntMask =                  0x1e,
   Dreg8255A0B0 =                 0x60,
   Dreg8255A0B1 =                 0x61,
   Dreg8255A0B2 =                 0x62,
   Dreg8255A0IR =                 0x63,
   Dreg8255A1B0 =                 0x64,
   Dreg8255A1B1 =                 0x65,
   Dreg8255A1B2 =                 0x66,
   Dreg8255A1IR =                 0x67,
   Dreg8255B0B0 =                 0x68,
   Dreg8255B0B1 =                 0x69,
   Dreg8255B0B2 =                 0x6a,
   Dreg8255B0IR =                 0x6b,
   Dreg8255B1B0 =                 0x6c,
   Dreg8255B1B1 =                 0x6d,
   Dreg8255B1B2 =                 0x6e,
   Dreg8255B1IR =                 0x6f,
   Dreg8255C0B0 =                 0x70,
   Dreg8255C0B1 =                 0x71,
   Dreg8255C0B2 =                 0x72,
   Dreg8255C0IR =                 0x73,
   Dreg8255C1B0 =                 0x74,
   Dreg8255C1B1 =                 0x75,
   Dreg8255C1B2 =                 0x76,
   Dreg8255C1IR =                 0x77,
   Dreg8255D0B0 =                 0x78,
   Dreg8255D0B1 =                 0x79,
   Dreg8255D0B2 =                 0x7a,
   Dreg8255D0IR =                 0x7b,
   Dreg8255D1B0 =                 0x7c,
   Dreg8255D1B1 =                 0x7d,
   Dreg8255D1B2 =                 0x7e,
   Dreg8255D1IR =                 0x7f,
} DaqIOReg;

typedef enum {
   RtfDeviceRegs  = 0x0,
   RtfLptPort     = 0x10000,
   RtfDspDM       = 0x20000,
   RtfDspPM       = 0x30000,
} RegTypeFlag;


DAQAPI DaqError WINAPI  daqRegWrite(DaqHandleT handle, DWORD reg, DWORD value);
DAQAPI DaqError WINAPI  daqRegRead(DaqHandleT handle, DWORD reg, PDWORD value);

/*****************************************************************/
/*                                                               */
/*  Obsolete, Internal Usage and Beta Definitions and Prototypes */
/*  - Internally Used - Not Supported                          */
/*                                                               */
/*  NOTE:  These functions are not intended to be used in user   */
/*         applications. Functions are hardware specific and,    */
/*         subject to change at any time.                        */
/*                                                               */
/*****************************************************************/

DAQAPI DaqError WINAPI
daqGetSwInfo(
			 DaqHandleT handle,
			 DWORD	nBufferLen,		// size of user level buffer in bytes
			 PCHAR	pBuffer, 		// user level buffer used to hold the contents of the sw info
			 DWORD	*pnBytesRead	// number of bytes that were or could be read into the user level buffer
			 );

typedef enum {
	DbgActionSetDebugMask	= 0x00000001,
	DbgActionReadBuffer		= 0x00000002,
	DbgActionFlushBuffer	= 0x00000004,
	DbgActionExecuteTest	= 0x00000008,
} DbgActionT;

DAQAPI DaqError WINAPI
daqDebugIO(
		   DaqHandleT handle,
		   DWORD	nAction,		// Action to perform (see DbgActionT)
		   DWORD	nMask,			// debug mask
		   DWORD	nBufferLen,		// size of user level buffer in bytes
		   PVOID	pBuffer, 		// user level buffer used to hold the contents of the debug buffer 
		   DWORD	*pnBytesRead,	// number of bytes read into the user level buffer
		   DWORD	*pnBytesAvail	// number of unread bytes in the driver level debug buffer
		   );

DAQAPI DaqError WINAPI  dspCmd(DaqHandleT handle, DWORD action, DWORD memType, DWORD addr, PDWORD data);
DAQAPI DaqError WINAPI  dspCmdA(DaqHandleT handle, DWORD action, DWORD memType, DWORD addr, PBYTE data);
DAQAPI DaqError WINAPI  daqBlockMemIO(DaqHandleT handle, DWORD action, DWORD blockType, DWORD whichBlock, DWORD addr, DWORD count, PVOID buf);
DAQAPI DaqError WINAPI  daqDumpDriverDebugBuffer( DaqHandleT handle, LPSTR buf, DWORD charCount );
DAQAPI DaqError WINAPI  daqAdcTransferSetBufferAllocMem(DaqHandleT handle, DWORD scanCount, DWORD transferMask);
DAQAPI DaqError WINAPI  daqAdcTransferStopAllocMem(DaqHandleT handle, PWORD buf, DWORD scanCount);
INT    daq_selftest(PCHAR device_name, PCHAR error_msg);

/* I/O Operation Code Definitions */
typedef enum {
   DioecP1IR            = 0,  /* DigiBook only */
   DioecP2IR            = 1,
   DioecP3IR            = 2,
} DaqIOEventCode;

/* I/O Operation Code Definitions */
typedef enum {
   DioocReadByte        = 0,
   DioocWriteByte       = 1,
   DioocReadWord        = 2,
   DioocWriteWord       = 3,
   DioocReadDWord       = 4,
   DioocWriteDWord      = 5,
} DaqIOOperationCode;

/* I/O Transfer Mask Definitions */
typedef enum {
   DiotmCycleOff         = 0,
   DiotmCycleOn          = 1,
} DaqIOTransferMask;

/* I/O Transfer Active Flag Definitions */
typedef enum {
   DioafDone             = 0,
   DioafArmed            = 1,
   DioafTriggered        = 2,
} DaqIOActiveFlag;

/* Note: The following block of functions are currently not supported. */
/* General I/O Prototypes - Transfer */
DAQAPI DaqError WINAPI  daqIOTransferReset(DaqHandleT handle);
DAQAPI DaqError WINAPI  daqIOTransferSetBuffer(DaqHandleT handle, DaqIOEventCode eventCode,
                           DaqIODeviceType devType, DaqIODevicePort devPort, DWORD whichDevice,
                           DaqIOExpansionPort whichExpPort, DaqIOOperationCode opCode, PVOID buf,
                           DWORD bufCount);
DAQAPI DaqError WINAPI  daqIOTransferStart(DaqHandleT handle);
DAQAPI DaqError WINAPI  daqIOTransferGetStat(DaqHandleT handle, DaqIODeviceType devType,
                           DaqIODevicePort devPort, DWORD whichDevice, DaqIOExpansionPort whichExpPort,
                           DaqIOActiveFlag *active, PDWORD retCount);
DAQAPI DaqError WINAPI  daqIOTransferStop(DaqHandleT handle);

typedef enum {
   DaqBook2001          = 24,
   DaqBook2002          = 25,
   DaqBook2003          = 26,
   DaqBook2004          = 27,
   DaqBook2005          = 28,
   DaqTemp7             = 0x100,
   DaqTemp7A            = 0x200,
   DaqTemp14            = 0x400,
   DaqTemp14A           = 0x800,
};


#ifdef   __cplusplus
};
#endif

#endif      /* #ifndef DAQ_H */

