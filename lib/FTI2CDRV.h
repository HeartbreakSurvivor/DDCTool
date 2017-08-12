// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the FTI2CDRV_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
//  functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef FTI2CDRV_H
#define FTI2CDRV_H


// The following ifdef block is the standard way of creating macros
// which make exporting from a DLL simpler.  All files within this DLL
// are compiled with the FTCI2C_EXPORTS symbol defined on the command line.
// This symbol should not be defined on any project that uses this DLL.
// This way any other project whose source files include this file see
//  functions as being imported from a DLL, whereas this DLL
// sees symbols defined with this macro as being exported.

#ifdef FTI2CDRV_EXPORTS
#define FTI2CDRV_API __declspec(dllexport)
#else
#define FTI2CDRV_API __declspec(dllimport)
#endif

typedef DWORD FTC_HANDLE;
typedef ULONG FTC_STATUS;


typedef DWORD FTC_HANDLE;
typedef ULONG FTC_STATUS;

#define NO_WRITE_TYPE 0
#define BYTE_WRITE_TYPE 1
#define PAGE_WRITE_TYPE 2

#define BYTE_READ_TYPE 1
#define BLOCK_READ_TYPE 2

#define STANDARD_MODE 1
#define FAST_MODE 2
#define STRETCH_DATA_MODE 4

#define FTC_SUCCESS 0 // FT_OK
#define FTC_INVALID_HANDLE 1 // FT_INVALID_HANDLE
#define FTC_DEVICE_NOT_FOUND 2 //FT_DEVICE_NOT_FOUND
#define FTC_DEVICE_NOT_OPENED 3 //FT_DEVICE_NOT_OPENED
#define FTC_IO_ERROR 4 //FT_IO_ERROR
#define FTC_INSUFFICIENT_RESOURCES 5 // FT_INSUFFICIENT_RESOURCES
#define FTC_INVALID_PARAMETER	6

//extended definition, by Joevin@cvte.cn 2014/04/09
#define FTC_UNKNOWN_FLASH_TYPE	10
#define FTC_LOAD_FILE_ERROR		11
#define FTC_WRITE_VERIFY_FAILED	12
#define FTC_UNKNOWN_CHIPSET_TYPE 13
#define FTC_FLASH_WRITE_FAILED	14
#define FTC_FLASH_READ_FAILED	15
#define FTC_DATA_VERIFY_FAILED  16

#define FTC_FAILED_TO_COMPLETE_COMMAND 20          // cannot change, error code mapped from FT2232c classes
#define FTC_FAILED_TO_SYNCHRONIZE_DEVICE_MPSSE 21  // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_DEVICE_NAME_INDEX 22           // cannot change, error code mapped from FT2232c classes
#define FTC_NULL_DEVICE_NAME_BUFFER_POINTER 23     // cannot change, error code mapped from FT2232c classes 
#define FTC_DEVICE_NAME_BUFFER_TOO_SMALL 24        // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_DEVICE_NAME 25                 // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_LOCATION_ID 26                 // cannot change, error code mapped from FT2232c classes
#define FTC_DEVICE_IN_USE 27                       // cannot change, error code mapped from FT2232c classes
#define FTC_TOO_MANY_DEVICES 28                    // cannot change, error code mapped from FT2232c classes
#define FTC_INVALID_SERIAL_NUMBER 29			   // cannot change, error code mapped from FT2232c classes

#define FTC_NULL_CHANNEL_BUFFER_POINTER 30         // cannot change, error code mapped from FT2232h classes 
#define FTC_CHANNEL_BUFFER_TOO_SMALL 31            // cannot change, error code mapped from FT2232h classes
#define FTC_INVALID_CHANNEL 32                     // cannot change, error code mapped from FT2232h classes
#define FTC_INVALID_TIMER_VALUE 33                 // cannot change, error code mapped from FT2232h classes

#define FTC_EXTERNAL_DEVICE_NOT_FOUND				40
#define FTC_INVALID_CLOCK_DIVISOR					41
#define FTC_NULL_INPUT_BUFFER_POINTER				42
#define FTC_NULL_INPUT_OUTPUT_BUFFER_POINTER		43
#define FTC_NULL_CONTROL_DATA_BUFFER_POINTER		44
#define FTC_INVALID_NUMBER_CONTROL_BYTES			45
#define FTC_CONTROL_ACKNOWLEDGE_TIMEOUT				46
#define FTC_NULL_WRITE_DATA_BUFFER_POINTER			47
#define FTC_INVALID_NUMBER_DATA_BYTES_WRITE			48
#define FTC_DATA_ACKNOWLEDGE_TIMEOUT				49
#define FTC_INVALID_WRITE_TYPE						50
#define FTC_NUMBER_BYTES_TOO_SMALL_PAGE_WRITE		51
#define FTC_NULL_PAGE_WRITE_BUFFER_POINTER			52
#define FTC_NULL_READ_DATA_BUFFER_POINTER			53
#define FTC_INVALID_NUMBER_DATA_BYTES_READ			54
#define FTC_INVALID_READ_TYPE						55
#define FTC_INVALID_COMMS_MODE						56
#define FTC_NULL_CLOSE_FINAL_STATE_BUFFER_POINTER	57
#define FTC_NULL_DLL_VERSION_BUFFER_POINTER			58
#define FTC_DLL_VERSION_BUFFER_TOO_SMALL			59
#define FTC_NULL_LANGUAGE_CODE_BUFFER_POINTER		60
#define FTC_NULL_ERROR_MESSAGE_BUFFER_POINTER		61
#define FTC_ERROR_MESSAGE_BUFFER_TOO_SMALL			62
#define FTC_INVALID_LANGUAGE_CODE					63
#define FTC_INVALID_STATUS_CODE						64

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_DEVICE_NAME_CHARS 64
#define MAX_NUM_SERIAL_NUMBER_CHARS 16

typedef struct Ft_Device_Info{
	DWORD dwIndex;
	DWORD dwDeviceType;
	DWORD dwProductVendorID;
	DWORD dwLocationID;
	char szSerialNumber[MAX_NUM_SERIAL_NUMBER_CHARS];
	char  szDeviceName[MAX_NUM_DEVICE_NAME_CHARS];
}FTC_DEVICE_INFO, *PFTC_DEVICE_INFO;

FTI2CDRV_API FTC_STATUS WINAPI I2C_GetNumDevices(LPDWORD lpdwNumDevices);


FTI2CDRV_API FTC_STATUS WINAPI I2C_Open(FTC_HANDLE *pftHandle);


FTI2CDRV_API FTC_STATUS WINAPI I2C_OpenEx(LPSTR lpDeviceName, DWORD dwLocationID, FTC_HANDLE *pftHandle);


FTI2CDRV_API FTC_STATUS WINAPI I2C_Close(FTC_HANDLE ftHandle);

typedef struct Ft_Close_Final_State_Pins{
  BOOL  bTCKPinState;
  BOOL  bTCKPinActiveState;
  BOOL  bTDIPinState;
  BOOL  bTDIPinActiveState;
  BOOL  bTMSPinState;
  BOOL  bTMSPinActiveState;
}FTC_CLOSE_FINAL_STATE_PINS, *PFTC_CLOSE_FINAL_STATE_PINS;


FTI2CDRV_API FTC_STATUS WINAPI I2C_InitDevice(FTC_HANDLE ftHandle, DWORD dwClockDivisor);


FTI2CDRV_API FTC_STATUS WINAPI I2C_GetClock(DWORD dwClockDivisor, LPDWORD lpdwClockFrequencyHz);


FTI2CDRV_API FTC_STATUS WINAPI I2C_SetMode(FTC_HANDLE ftHandle, DWORD dwCommsMode);

typedef struct Ft_Input_Output_Pins{
  BOOL  bPin1InputOutputState;
  BOOL  bPin1LowHighState;
  BOOL  bPin2InputOutputState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3InputOutputState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4InputOutputState;
  BOOL  bPin4LowHighState;
}FTC_INPUT_OUTPUT_PINS, *PFTC_INPUT_OUTPUT_PINS;

typedef struct FTH_Input_Output_Pins{
  BOOL  bPin1InputOutputState;
  BOOL  bPin1LowHighState;
  BOOL  bPin2InputOutputState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3InputOutputState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4InputOutputState;
  BOOL  bPin4LowHighState;
  BOOL  bPin5InputOutputState;
  BOOL  bPin5LowHighState;
  BOOL  bPin6InputOutputState;
  BOOL  bPin6LowHighState;
  BOOL  bPin7InputOutputState;
  BOOL  bPin7LowHighState;
  BOOL  bPin8InputOutputState;
  BOOL  bPin8LowHighState;
}FTH_INPUT_OUTPUT_PINS, *PFTH_INPUT_OUTPUT_PINS;

typedef struct Ft_Low_High_Pins{
  BOOL  bPin1LowHighState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4LowHighState;
}FTC_LOW_HIGH_PINS, *PFTC_LOW_HIGH_PINS;


FTI2CDRV_API FTC_STATUS WINAPI I2C_GetGPIOs(FTC_HANDLE ftHandle, PFTC_LOW_HIGH_PINS pHighPinsInputData);

typedef struct FTH_Low_High_Pins{
  BOOL  bPin1LowHighState;
  BOOL  bPin2LowHighState;
  BOOL  bPin3LowHighState;
  BOOL  bPin4LowHighState;
  BOOL  bPin5LowHighState;
  BOOL  bPin6LowHighState;
  BOOL  bPin7LowHighState;
  BOOL  bPin8LowHighState;
}FTH_LOW_HIGH_PINS, *PFTH_LOW_HIGH_PINS;

#define MAX_WRITE_CONTROL_BYTES_BUFFER_SIZE 256    // 256 bytes

typedef BYTE WriteControlByteBuffer[MAX_WRITE_CONTROL_BYTES_BUFFER_SIZE];
typedef WriteControlByteBuffer *PWriteControlByteBuffer;

typedef struct FTC_Page_Write_Data{
  DWORD  dwNumPages;
  DWORD  dwNumBytesPerPage;
}FTC_PAGE_WRITE_DATA, *PFTC_PAGE_WRITE_DATA;

#define MAX_WRITE_DATA_BYTES_BUFFER_SIZE 65536    // 64k bytes

typedef BYTE WriteDataByteBuffer[MAX_WRITE_DATA_BYTES_BUFFER_SIZE];
typedef WriteDataByteBuffer *PWriteDataByteBuffer;


// FTI2CDRV_API FTC_STATUS WINAPI I2C_Write_Nt_Isp_Mode(FTC_HANDLE ftHandle, PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumDataBytesToWrite,
//	BOOL bDataAcknowledge, DWORD dwDataAckTimeoutmSecs);
//
// FTI2CDRV_API FTC_STATUS WINAPI I2C_Write_Nt_Isp_Cmd(FTC_HANDLE ftHandle, PWriteControlByteBuffer pWriteControlBuffer,
//	DWORD dwNumControlBytesToWrite, BOOL bControlAcknowledge,
//	DWORD dwControlAckTimeoutmSecs, BOOL bStopCondition, 
//	PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumDataBytesToWrite,
//	DWORD dwDataAckTimeoutmSecs);

#define MAX_READ_DATA_BYTES_BUFFER_SIZE 65536    // 64k bytes

typedef BYTE ReadDataByteBuffer[MAX_READ_DATA_BYTES_BUFFER_SIZE];
typedef ReadDataByteBuffer *PReadDataByteBuffer;


FTI2CDRV_API FTC_STATUS WINAPI I2C_GetDllVersion(LPSTR lpDllVersionBuffer, DWORD dwBufferSize);


FTI2CDRV_API FTC_STATUS WINAPI I2C_GetErrorCodeString(LPSTR lpLanguage, FTC_STATUS StatusCode,
                                        LPSTR lpErrorMessageBuffer, DWORD dwBufferSize);

FTI2CDRV_API FTC_STATUS WINAPI I2C_OpenByDeviceNameIndex(DWORD dwDeviceNameIndex, FTC_HANDLE *pftHandle);
FTI2CDRV_API FTC_STATUS WINAPI I2C_GetDeviceInfo(DWORD dwDeviceNameIndex, FTC_DEVICE_INFO *pstDevInfo);
FTI2CDRV_API FTC_STATUS WINAPI I2C_EnumDevices(FT_DEVICE_LIST_INFO_NODE* pDevicesInfo, int& len);
FTI2CDRV_API FTC_STATUS WINAPI I2C_Read(FTC_HANDLE ftHandle, PWriteControlByteBuffer pWriteControlBuffer,
                        DWORD dwNumControlBytesToWrite, BOOL bControlAcknowledge, DWORD dwControlAckTimeoutmSecs,
                        DWORD dwDataReadTypes, PReadDataByteBuffer pReadDataBuffer, DWORD dwNumDataBytesToRead);
FTI2CDRV_API FTC_STATUS WINAPI I2C_Isp_Read(FTC_HANDLE ftHandle, BYTE SlaveAddr, BYTE * pReadBuffer, DWORD dwBytesToRead);
FTI2CDRV_API FTC_STATUS WINAPI I2C_Isp_Write(FTC_HANDLE ftHandle, BYTE SlaveAddr, BYTE * pWriteBuffer, DWORD dwBytesToWrite);

FTI2CDRV_API FTC_STATUS WINAPI I2C_Write_Nt_Isp_Mode(FTC_HANDLE ftHandle, PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumDataBytesToWrite,
	BOOL bDataAcknowledge, DWORD dwDataAckTimeoutmSecs);

FTI2CDRV_API FTC_STATUS WINAPI I2C_Write_Nt_Isp_Cmd(FTC_HANDLE ftHandle, PWriteControlByteBuffer pWriteControlBuffer,
	DWORD dwNumControlBytesToWrite, BOOL bControlAcknowledge,
	DWORD dwControlAckTimeoutmSecs, BOOL bStopCondition, 
	PWriteDataByteBuffer pWriteDataBuffer, DWORD dwNumDataBytesToWrite,
	DWORD dwDataAckTimeoutmSecs);

#ifdef __cplusplus
}
#endif



//// This class is exported from the FTI2CDRV.dll
//class  CFTI2CDRV {
//public:
//	CFTI2CDRV(void);
//	// TODO: add your methods here.
//};
//
//extern  int nFTI2CDRV;
//
// int fnFTI2CDRV(void);

#endif  /* FTI2CDRV_H */