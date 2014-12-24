/**
 * 	\file
 *
 *	\brief Interrupt handle
 *
 *	\page License
 *
 * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 * AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
 * INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
 * CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
 * INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * THIS SOFTWARE IS SPECIFICALLY DESIGNED FOR EXCLUSIVE USE WITH ST PARTS.
 *
 *
 *  Created on: 25.08.2011
 *      Author: Fabio Tota
 */

#ifndef LIS331DLH_H_
#define LIS331DLH_H_

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

//these could change accordingly with the architecture
typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef short int i8_t;
typedef u8_t Axis_t;
typedef u8_t IntConf_t;


typedef enum {
  MEMS_ENABLE			=		0x01,
  MEMS_DISABLE			=		0x00
} State_t;

typedef enum {
  ODR_50Hz		        =		0x00,
  ODR_100Hz		        =		0x01,
  ODR_400Hz		        =		0x02,
  ODR_1000Hz		    =		0x03
} ODR_t;

typedef enum {
  CONTINUOUS_MODE       =		0x00,
  SINGLE_MODE 			=		0x01,
  SLEEP_MODE			=		0x02
} Mode_M_t;

typedef enum {
  POWER_DOWN            =		0x00,
  NORMAL 				=		0x01,
  LOW_POWER_05			=		0x02,
  LOW_POWER_1 			=		0x03,
  LOW_POWER_2			=		0x04,
  LOW_POWER_5			=		0x05,
  LOW_POWER_10			=		0x06,
} Mode_t;

typedef enum {
  HPM_NORMAL_MODE_RES           =               0x00,
  HPM_REF_SIGNAL                =               0x01,
  HPM_NORMAL_MODE               =               0x02,
} HPFMode_t;

typedef enum {
  HPFCF_0                       =               0x00,
  HPFCF_1                       =               0x01,
  HPFCF_2                       = 				0x02,
  HPFCF_3                       =               0x03
} HPFCutOffFreq_t;

typedef enum {
  INT_SOURCE                    =               0x00,
  INT_1OR2_SOURCE               =               0x01,
  DATA_READY                    = 				0x02,
  BOOT_RUNNING                  =               0x03
} INT_Conf_t;

typedef enum {
  SLEEP_TO_WAKE_DIS             =               0x00,
  SLEEP_TO_WAKE_ENA             =               0x03,
} Sleeo_To_Wake_Conf_t;

typedef struct {
  i16_t AXIS_X;
  i16_t AXIS_Y;
  i16_t AXIS_Z;
} AccAxesRaw_t;


typedef enum {
  FULLSCALE_2                   =               0x00,
  FULLSCALE_4                   =               0x01,
  FULLSCALE_8                   =               0x03,
} Fullscale_t;

typedef enum {
  BLE_LSB						=				0x00,
  BLE_MSB						=				0x01
} Endianess_t;

typedef enum {
  SPI_4_WIRE                    =               0x00,
  SPI_3_WIRE                    =               0x01
} SPIMode_t;

typedef enum {
  X_ENABLE                      =               0x01,
  X_DISABLE                     =               0x00,
  Y_ENABLE                      =               0x02,
  Y_DISABLE                     =               0x00,
  Z_ENABLE                      =               0x04,
  Z_DISABLE                     =               0x00
} AXISenable_t;

typedef enum {
  UP_SX                         =               0x44,
  UP_DX                         =               0x42,
  DW_SX                         =               0x41,
  DW_DX                         =               0x48,
  TOP                           =               0x60,
  BOTTOM                        =               0x50
} POSITION_6D_t;

typedef enum {
  INT_MODE_OR                   =               0x00,
  INT_MODE_6D_MOVEMENT          =               0x01,
  INT_MODE_AND                  =               0x02,
  INT_MODE_6D_POSITION          =               0x03
} IntMode_t;


/* Exported constants --------------------------------------------------------*/
#define MEMS_SET				0x01
#define MEMS_RESET				0x00
#ifdef LIS331DLM
	#define MEMS_I2C_ADDRESS		0x12	/* LIS331DLM */
#elif LIS331DLH
	//#define MEMS_I2C_ADDRESS		0x32	/* LIS331DLH */
#endif

//Register and define
#define WHO_AM_I				0x0F  // device identification register

// CONTROL REGISTER 1
#define CTRL_REG1       		0x20
#define PM						5
#define DR				    	3
#define ZEN						2
#define YEN						1
#define XEN						0

//CONTROL REGISTER 2
#define CTRL_REG2				0x21
#define BOOT                    7
#define HPM     				5
#define FDS     				4
#define HPEN2					3
#define HPEN1					2
#define HPCF					0

//CONTROL REGISTER 3
#define CTRL_REG3				0x22
#define IHL                   	7
#define PP_OD					6
#define LIR2					5
#define I2_CFG  				3
#define LIR1    				2
#define I1_CFG  				0

//CONTROL REGISTER 4
#define CTRL_REG4				0x23
#define BDU						7
#define BLE						6
#define FS						4
#define ST_SIGN					3
#define ST       				1
#define SIM						0

//CONTROL REGISTER 5
#define CTRL_REG5       		0x24
#define TURN_ON                 0

#define HP_FILTER_RESET			0x25

//REFERENCE/DATA_CAPTURE
#define REFERENCE_REG		  	0x26
#define REF		           		0

//STATUS_REG_AXIES
#define STATUS_REG				0x27

//INTERRUPT 1 CONFIGURATION
#define INT1_CFG				0x30

//INTERRUPT 2 CONFIGURATION
#define INT2_CFG				0x34
#define ANDOR                  	7
#define INT_6D       			6

//INT REGISTERS
#define INT1_THS              	0x32
#define INT1_DURATION       	0x33
#define INT2_THS            	0x36
#define INT2_DURATION       	0x37

//INTERRUPT 1 SOURCE REGISTER
#define INT1_SRC					0x31
#define INT2_SRC					0x35

//INT_CFG  bit mask
#define INT_AND						0x80
#define INT_OR						0x00
#define INT_ZHIE_ENABLE				0x20
#define INT_ZHIE_DISABLE			0x00
#define INT_ZLIE_ENABLE				0x10
#define INT_ZLIE_DISABLE			0x00
#define INT_YHIE_ENABLE				0x08
#define INT_YHIE_DISABLE			0x00
#define INT_YLIE_ENABLE				0x04
#define INT_YLIE_DISABLE			0x00
#define INT_XHIE_ENABLE				0x02
#define INT_XHIE_DISABLE			0x00
#define INT_XLIE_ENABLE				0x01
#define INT_XLIE_DISABLE			0x00

//INT_SRC  bit mask
#define INT_SRC_IA					0x40
#define INT_SRC_ZH					0x20
#define INT_SRC_ZL					0x10
#define INT_SRC_YH					0x08
#define INT_SRC_YL					0x04
#define INT_SRC_XH					0x02
#define INT_SRC_XL					0x01

//OUTPUT REGISTER
#define OUT_X_L				        0x28
#define OUT_X_H				        0x29
#define OUT_Y_L			        	0x2A
#define OUT_Y_H			        	0x2B
#define OUT_Z_L			        	0x2C
#define OUT_Z_H			        	0x2D

//STATUS REGISTER bit mask
#define STATUS_REG_ZYXOR                        0x80    // 1	:	new data set has over written the previous one
							// 0	:	no overrun has occurred (default)
#define STATUS_REG_ZOR                          0x40    // 0	:	no overrun has occurred (default)
							// 1	:	new Z-axis data has over written the previous one
#define STATUS_REG_YOR                          0x20    // 0	:	no overrun has occurred (default)
							// 1	:	new Y-axis data has over written the previous one
#define STATUS_REG_XOR                          0x10    // 0	:	no overrun has occurred (default)
							// 1	:	new X-axis data has over written the previous one
#define STATUS_REG_ZYXDA                        0x08    // 0	:	a new set of data is not yet avvious one
                                                        // 1	:	a new set of data is available
#define STATUS_REG_ZDA                          0x04    // 0	:	a new data for the Z-Axis is not availvious one
                                                        // 1	:	a new data for the Z-Axis is available
#define STATUS_REG_YDA                          0x02    // 0	:	a new data for the Y-Axis is not available
                                                        // 1	:	a new data for the Y-Axis is available
#define STATUS_REG_XDA                          0x01    // 0	:	a new data for the X-Axis is not available

#define DATAREADY_BIT                         STATUS_REG_ZYXDA



/* Exported macro ------------------------------------------------------------*/
#define ValBit(VAR,Place)         (VAR & (1<<Place))

/* Exported functions --------------------------------------------------------*/
//Sensor Configuration Functions
static int GetWHO_AM_I(u8_t* val);
static int SetODR(ODR_t dr);
static int SetMode(Mode_t pm);
static int SetAxis(Axis_t axis);
static int SetFullScale(Fullscale_t fs);
static int SetBDU(State_t bdu);
static int SetBLE(Endianess_t ble);
static int SetSelfTest(State_t st);
static int SetSelfTestSign(State_t st_sign);
static int TurnONEnable(Sleeo_To_Wake_Conf_t stw);
static int SetBOOT(State_t boot);
static int SetFDS(State_t fds);
static int SetSPI34Wire(SPIMode_t sim);

//Filtering Functions
static int SetHPFMode(HPFMode_t hpm);
static int SetHPFCutOFF(HPFCutOffFreq_t hpf);
static int SetFilterDataSel(State_t state);
static int SetReference(i8_t ref);

//Interrupt Functions
static int SetIntHighLow(State_t hil);
static int SetIntPPOD(State_t pp_od);
static int SetInt1DataSign(INT_Conf_t i_cfg);
//int SetInt2DataSign(INT_Conf_t i_cfg);
static int SetInt1HPenable(State_t stat);
//int SetInt2HPenable(State_t stat);
static int Int1LatchEnable(State_t latch);
//int Int2LatchEnable(State_t latch);
int ResetInt1Latch(void);
//int ResetInt2Latch(void);
int SetInt1Configuration(IntConf_t ic);
//int SetInt2Configuration(IntConf_t ic);
static int SetInt1Threshold(u8_t ths);
//int SetInt2Threshold(u8_t ths);
static int SetInt1Duration(u8_t id);
//int SetInt2Duration(u8_t id);
static int SetInt1Mode(IntMode_t int_mode);
//int SetInt2Mode(IntMode_t int_mode);
static int GetInt1Src(u8_t* val);
//int GetInt2Src(u8_t* val);
static int GetInt1SrcBit(u8_t statusBIT);
//int GetInt2SrcBit(u8_t statusBIT);

//Other Reading Functions
static int GetSatusReg(u8_t* val);
static int GetSatusBit(u8_t statusBIT);
int GetAccAxesRaw(AccAxesRaw_t* buff);
static int Get6DPositionInt1(u8_t* val);
//int Get6DPositionInt2(u8_t* val);

void Accel_Configuration(void);

void setAccel(FunctionalState);


#endif /* LIS331DLH_H_ */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
