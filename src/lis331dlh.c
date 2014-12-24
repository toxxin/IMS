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


#include "ims.h"
#include "lis331dlh.h"

/**
 *  \brief	Generic Reading function.
 *
 *  This function must be fulfilled with either I2C or SPI reading functions.
 *
 *  \param addr		- device i2c address
 *  \param reg		- device internal register
 *  \param data		- pointer to buf for reading data
 *
 *  \retval			- _error_t
 */
static int ReadReg(uint8_t addr, uint8_t reg, uint8_t* data)
{
	int ret = E_OK;

	ret = I2C_read(I2C_MEMS_BUS, addr, reg, data);

	return ret;
}


/**
 *  \brief	Generic Writing function.
 *
 *  This function must be fulfilled with either I2C or SPI writing functions.
 *
 *  \param addr		- device i2c address
 *  \param reg		- device internal register
 *  \param data		- pointer to buf for writing data
 *
 *  \retval			- _error_t
 */
static int WriteReg(uint8_t addr, uint8_t cmd, uint8_t data)
{
	int ret = E_OK;

	ret = I2C_write(I2C_MEMS_BUS, addr, cmd, data);

	return ret;
}

/*******************************************************************************
* Function Name  : GetWHO_AM_I
* Description    : Read identification code by WHO_AM_I register
* Input          : char to empity by Device identification Value
* Output         : None
* Return         : Status [value of FSS]
*******************************************************************************/
static int GetWHO_AM_I(uint8_t* val)
{

	if (ReadReg(MEMS_I2C_ADDRESS, WHO_AM_I, val))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetODR
* Description    : Sets LIS331DLH Output Data Rate Accelerometer
* Input          : Output Data Rate
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_ERR_MEMS_ERROR]
*******************************************************************************/
static int SetODR(ODR_t dr)
{
	uint8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG1, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xE7;
  value |= dr<<DR;

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG1, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetMode
* Description    : Sets LIS331DLH Operating Mode Accelrometer
* Input          : Modality (LOW_POWER, NORMAL, POWER_DOWN...)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_ERR_MEMS_ERROR]
*******************************************************************************/
static int SetMode(Mode_t pm)
{
	uint8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG1, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x1F;
  value |= (pm<<PM);

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG1, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetAxis
* Description    : Enable/Disable LIS331DLH Axis
* Input          : X_ENABLE/X_DISABLE | Y_ENABLE/Y_DISABLE | Z_ENABLE/Z_DISABLE
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetAxis(Axis_t axis)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG1, &value))
		return ERR_MEMS_ERROR;

	value &= 0xF8;
	value |= (0x07 & axis);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG1, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetFullScale
* Description    : Sets the LIS331DLH FullScale
* Input          : FULLSCALE_2/FULLSCALE_4/FULLSCALE_8/FULLSCALE_16
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetFullScale(Fullscale_t fs)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value))
		return ERR_MEMS_ERROR;

	value &= 0xCF;
	value |= (fs << FS);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetBDU
* Description    : Enable/Disable Block Data Update Functionality
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetBDU(State_t bdu)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value))
		return ERR_MEMS_ERROR;

	value &= 0x7F;
	value |= (bdu << BDU);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetBLE
* Description    : Set Endianess (MSB/LSB)
* Input          : BLE_LSB / BLE_MSB
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetBLE(Endianess_t ble)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value))
		return ERR_MEMS_ERROR;

	value &= 0xBF;
	value |= (ble << BLE);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetFDS
* Description    : Set Filter Data Selection
* Input          : MEMS_ENABLE/MEMS_DISABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetFDS(State_t fds)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value))
		return ERR_MEMS_ERROR;

	value &= 0xEF;
	value |= (fds << FDS);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}

/*******************************************************************************
 * Function Name  : SetBOOT
 * Description    : Rebot memory content
 * Input          : MEMS_ENABLE/MEMS_DISABLE
 * Output         : None
 * Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
 *******************************************************************************/
static int SetBOOT(State_t boot)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value))
		return ERR_MEMS_ERROR;

	value &= 0x7F;
	value |= (boot << BOOT);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}

/*******************************************************************************
 * Function Name  : SetSelfTest
 * Description    : Set Self Test Modality
 * Input          : MEMS_DISABLE/MEMS_ENABLE
 * Output         : None
 * Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
 *******************************************************************************/
static int SetSelfTest(State_t st)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value))
		return ERR_MEMS_ERROR;

	value &= 0xFD;
	value |= (st << ST);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetSelfTestSign
* Description    : Set Self Test Sign (Disable = st_plus, Enable = st_minus)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetSelfTestSign(State_t st_sign)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value))
		return ERR_MEMS_ERROR;

	value &= 0xF7;
	value |= (st_sign << ST_SIGN);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetIntHighLow
* Description    : Set Interrupt active state (Disable = active high, Enable = active low)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetIntHighLow(State_t ihl)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value))
		return ERR_MEMS_ERROR;

	value &= 0x7F;
	value |= (ihl << IHL);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetIntPPOD
* Description    : Set Interrupt Push-Pull/OpenDrain Pad (Disable = Push-Pull, Enable = OpenDrain)
* Input          : MEMS_DISABLE/MEMS_ENABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetIntPPOD(State_t pp_od)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value))
		return ERR_MEMS_ERROR;

	value &= 0xBF;
	value |= (pp_od << PP_OD);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}


/*******************************************************************************
* Function Name  : SetInt1DataSign
* Description    : Set Data signal Interrupt 1 pad
* Input          : Modality by INT_Conf_t Typedef (INT_SOURCE, INT_1OR2_SOURCE, DATA_READY, BOOT_RUNNING)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt1DataSign(INT_Conf_t i_cfg)
{
	u8_t value;

	if (ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value))
		return ERR_MEMS_ERROR;

	value &= 0xFC;
	value |= (i_cfg << I1_CFG);

	if (WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value))
		return ERR_MEMS_ERROR;

	return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : SetInt2DataSign
* Description    : Set Data signal Interrupt 2 pad
* Input          : Modality by INT_Conf_t Typedef (INT_SOURCE, INT_1OR2_SOURCE, DATA_READY, BOOT_RUNNING)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2DataSign(INT_Conf_t i_cfg)
{
	u8_t value;

	if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value) )
	return ERR_MEMS_ERROR;

	value &= 0xE7;
	value |= (i_cfg<<I2_CFG);

	if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value) )
	return ERR_MEMS_ERROR;

	return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : SetSPI34Wire
* Description    : Set SPI mode
* Input          : Modality by SPIMode_t Typedef (SPI_4_WIRE, SPI_3_WIRE)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetSPI34Wire(SPIMode_t sim) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG4, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xFE;
  value |= (sim<<SIM);

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG4, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : TurnONEnable
* Description    : TurnON Mode selection for sleep to wake function
* Input          : SLEEP_TO_WAKE_DIS/SLEEP_TO_WAKE_ENA
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int TurnONEnable(Sleeo_To_Wake_Conf_t stw) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG5, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x00;
  value |= (stw<<TURN_ON);

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG5, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : HPFilterReset
* Description    : Reading register for reset the content of internal HP filter
* Input          : None
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int HPFilterReset(void) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, HP_FILTER_RESET, &value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetReference
* Description    : Sets Reference register acceleration value as a reference for HP filter
* Input          : Value of reference acceleration value (0-255)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetReference(i8_t ref) {

  if( WriteReg(MEMS_I2C_ADDRESS, REFERENCE_REG, ref) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetHPFMode
* Description    : Set High Pass Filter Modality
* Input          : HPM_NORMAL_MODE_RES/HPM_REF_SIGNAL/HPM_NORMAL_MODE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetHPFMode(HPFMode_t hpm) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x9F;
  value |= (hpm<<HPM);

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetHPFCutOFF
* Description    : Set High Pass CUT OFF Freq
* Input          : HPFCF [0,3]
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetHPFCutOFF(HPFCutOffFreq_t hpf) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xFC;
  value |= (hpf<<HPCF);

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value) )
    return ERR_MEMS_ERROR;

  return E_OK;

}

#if 0
/*******************************************************************************
* Function Name  : SetInt2HPenable
* Description    : Set Interrupt2 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : SetInt2HPenable(MEMS_ENABLE)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2HPenable(State_t stat) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xF7;
  value |= stat<<HPEN2 ;

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : SetInt1HPenable
* Description    : Set Interrupt1 hp filter enable/disable
* Input          : MEMS_ENABLE/MEMS_DISABLE
* example        : SetInt1HPenable(MEMS_ENABLE)
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt1HPenable(State_t stat) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG2, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xFB;
  value |= stat<<HPEN1 ;

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG2, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : Int1LatchEnable
* Description    : Enable Interrupt 1 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int Int1LatchEnable(State_t latch) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xFB;
  value |= latch<<LIR1;

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : Int2LatchEnable
* Description    : Enable Interrupt 2 Latching function
* Input          : ENABLE/DISABLE
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int Int2LatchEnable(State_t latch) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, CTRL_REG3, &value) )
    return ERR_MEMS_ERROR;

  value &= 0xDF;
  value |= latch<<LIR2;

  if( WriteReg(MEMS_I2C_ADDRESS, CTRL_REG3, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : ResetInt1Latch
* Description    : Reset Interrupt 1 Latching function
* Input          : None
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
int ResetInt1Latch(void) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_SRC, &value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : ResetInt2Latch
* Description    : Reset Interrupt 2 Latching function
* Input          : None
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int ResetInt2Latch(void) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_SRC, &value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : SetInt1Configuration
* Description    : Interrupt 1 Configuration (whitout 6D_INT)
* Input          : INT_AND/OR | INT_ZHIE_ENABLE/DISABLE | INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
int SetInt1Configuration(IntConf_t ic) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_CFG, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x40;
  value |= ic;

  if( WriteReg(MEMS_I2C_ADDRESS, INT1_CFG, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : SetInt2Configuration
* Description    : Interrupt 1 Configuration (whitout 6D_INT)
* Input          : INT_AND/OR | INT_ZHIE_ENABLE/DISABLE | INT_ZLIE_ENABLE/DISABLE...
* Output         : None
* Note           : You MUST use all input variable in the argument, as example
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2Configuration(IntConf_t ic) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_CFG, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x40;
  value |= ic;

  if( WriteReg(MEMS_I2C_ADDRESS, INT2_CFG, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : SetInt1Mode
* Description    : Interrupt 1 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : INT_MODE_OR, INT_MODE_6D_MOVEMENT, INT_MODE_AND, INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt1Mode(IntMode_t int_mode) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_CFG, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x3F;
  value |= (int_mode<<INT_6D);

  if( WriteReg(MEMS_I2C_ADDRESS, INT1_CFG, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : SetInt2Mode
* Description    : Interrupt 2 Configuration mode (OR, 6D Movement, AND, 6D Position)
* Input          : INT_MODE_OR, INT_MODE_6D_MOVEMENT, INT_MODE_AND, INT_MODE_6D_POSITION
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2Mode(IntMode_t int_mode) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_CFG, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x3F;
  value |= (int_mode<<INT_6D);

  if( WriteReg(MEMS_I2C_ADDRESS, INT2_CFG, value) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : Get6DPositionInt1
* Description    : 6D Interrupt 1 Position Detect
* Input          : Byte to empity by POSITION_6D_t Typedef
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int Get6DPositionInt1(u8_t* val){
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_SRC, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x7F;

  switch (value){
  case UP_SX:   *val = UP_SX;    break;
  case UP_DX:   *val = UP_DX;    break;
  case DW_SX:   *val = DW_SX;    break;
  case DW_DX:   *val = DW_DX;    break;
  case TOP:     *val = TOP;      break;
  case BOTTOM:  *val = BOTTOM;   break;
  }

return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : Get6DPositionInt2
* Description    : 6D Interrupt 2 Position Detect
* Input          : Byte to empity by POSITION_6D_t Typedef
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int Get6DPositionInt2(u8_t* val){
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_SRC, &value) )
    return ERR_MEMS_ERROR;

  value &= 0x7F;

  switch (value){
  case UP_SX:   *val = UP_SX;    break;
  case UP_DX:   *val = UP_DX;    break;
  case DW_SX:   *val = DW_SX;    break;
  case DW_DX:   *val = DW_DX;    break;
  case TOP:     *val = TOP;      break;
  case BOTTOM:  *val = BOTTOM;   break;
  }

return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : SetInt1Threshold
* Description    : Sets Interrupt 1 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt1Threshold(u8_t ths) {
  if (ths > 127)
    return ERR_MEMS_ERROR;

      if( WriteReg(MEMS_I2C_ADDRESS, INT1_THS, ths) )
        return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetInt1Duration
* Description    : Sets Interrupt 1 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt1Duration(u8_t id) {

  if (id > 127)
    return ERR_MEMS_ERROR;

  if( WriteReg(MEMS_I2C_ADDRESS, INT1_DURATION, id) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : SetInt2Threshold
* Description    : Sets Interrupt 2 Threshold
* Input          : Threshold = [0,31]
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2Threshold(u8_t ths) {
  if (ths > 127)
    return ERR_MEMS_ERROR;

      if( WriteReg(MEMS_I2C_ADDRESS, INT2_THS, ths) )
        return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : SetInt2Duration
* Description    : Sets Interrupt 2 Duration
* Input          : Duration value
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int SetInt2Duration(u8_t id) {

  if (id > 127)
    return ERR_MEMS_ERROR;

  if( WriteReg(MEMS_I2C_ADDRESS, INT2_DURATION, id) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : GetSatusReg
* Description    : Read the status register
* Input          : char to empty by Status Reg Value
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetSatusReg(u8_t* val) {

  if( ReadReg(MEMS_I2C_ADDRESS, STATUS_REG, val) )
      return ERR_MEMS_ERROR;

  return E_OK;
}


/*******************************************************************************
* Function Name  : GetSatusBIT
* Description    : Read the status register BIT
* Input          : STATUS_REG_ZYXOR, STATUS_REG_ZOR, STATUS_REG_YOR, STATUS_REG_XOR,
                   STATUS_REG_ZYXDA, STATUS_REG_ZDA, STATUS_REG_YDA, STATUS_REG_XDA, DATAREADY_BIT
* Output         : status register BIT
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetSatusBit(u8_t statusBIT) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, STATUS_REG, &value) )
      return ERR_MEMS_ERROR;

  switch (statusBIT){
    case STATUS_REG_ZYXOR:     if(value &= STATUS_REG_ZYXOR) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_ZOR:       if(value &= STATUS_REG_ZOR) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_YOR:       if(value &= STATUS_REG_YOR) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_XOR:       if(value &= STATUS_REG_XOR) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_ZYXDA:     if(value &= STATUS_REG_ZYXDA) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_ZDA:       if(value &= STATUS_REG_ZDA) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_YDA:       if(value &= STATUS_REG_YDA) return E_OK;
                               else  return ERR_MEMS_ERROR;
    case STATUS_REG_XDA:       if(value &= STATUS_REG_XDA) return E_OK;
                               else  return ERR_MEMS_ERROR;

  }
return ERR_MEMS_ERROR;
}


/*******************************************************************************
* Function Name  : GetAccAxesRaw
* Description    : Read the Acceleration Values Output Registers
* Input          : buffer to empity by AccAxesRaw_t Typedef
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
int GetAccAxesRaw(AccAxesRaw_t* buff) {
  u8_t valueL;
  u8_t valueH;

  if( ReadReg(MEMS_I2C_ADDRESS, OUT_X_L, &valueL) )
      return ERR_MEMS_ERROR;

  if( ReadReg(MEMS_I2C_ADDRESS, OUT_X_H, &valueH) )
      return ERR_MEMS_ERROR;

#ifdef LIS331DLM
  buff->AXIS_X = (int8_t)(valueH);
#elif LIS331DLH
  buff->AXIS_X = (i16_t)( (valueH << 8) | valueL )/16;
#endif

  if( ReadReg(MEMS_I2C_ADDRESS, OUT_Y_L, &valueL) )
      return ERR_MEMS_ERROR;

  if( ReadReg(MEMS_I2C_ADDRESS, OUT_Y_H, &valueH) )
      return ERR_MEMS_ERROR;

#ifdef LIS331DLM
  buff->AXIS_Y = (int8_t)(valueH);
#elif LIS331DLH
  buff->AXIS_Y = (i16_t)( (valueH << 8) | valueL )/16;
#endif

   if( ReadReg(MEMS_I2C_ADDRESS, OUT_Z_L, &valueL) )
      return ERR_MEMS_ERROR;

  if( ReadReg(MEMS_I2C_ADDRESS, OUT_Z_H, &valueH) )
      return ERR_MEMS_ERROR;

#ifdef LIS331DLM
  buff->AXIS_Z = (int8_t)(valueH);
#elif LIS331DLH
  buff->AXIS_Z = (i16_t)( (valueH << 8) | valueL )/16;
#endif

  return E_OK;
}


/*******************************************************************************
* Function Name  : GetInt1Src
* Description    : Reset Interrupt 1 Latching function
* Input          : buffer to empty by Int1 Source Value
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetInt1Src(u8_t* val) {

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_SRC, val) )
    return ERR_MEMS_ERROR;

  return E_OK;
}

#if 0
/*******************************************************************************
* Function Name  : GetInt2Src
* Description    : Reset Interrupt 2 Latching function
* Input          : buffer to empty by Int2 Source Value
* Output         : None
* Return         : Status [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetInt2Src(u8_t* val) {

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_SRC, val) )
    return ERR_MEMS_ERROR;

  return E_OK;
}
#endif

/*******************************************************************************
* Function Name  : GetInt1SrcBit
* Description    : Reset Interrupt 1 Latching function
* Input          : INT1_SRC_IA, INT1_SRC_ZH, INT1_SRC_ZL, INT1_SRC_YH .....
* Output         : None
* Return         : Status of BIT [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetInt1SrcBit(u8_t statusBIT) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT1_SRC, &value) )
      return ERR_MEMS_ERROR;

  if(statusBIT == INT_SRC_IA){
    if(value &= INT_SRC_IA)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_ZH){
    if(value &= INT_SRC_ZH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_ZL){
    if(value &= INT_SRC_ZL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_YH){
    if(value &= INT_SRC_YH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_YL){
    if(value &= INT_SRC_YL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_XH){
    if(value &= INT_SRC_XH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_XL){
    if(value &= INT_SRC_XL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }
return ERR_MEMS_ERROR;
}

#if 0
/*******************************************************************************
* Function Name  : GetInt2SrcBit
* Description    : Reset Interrupt 2 Latching function
* Input          : INT_SRC_IA, INT_SRC_ZH, INT_SRC_ZL, INT_SRC_YH .....
* Output         : None
* Return         : Status of BIT [ERR_MEMS_ERROR, ERR_NO_ERROR]
*******************************************************************************/
static int GetInt2SrcBit(u8_t statusBIT) {
  u8_t value;

  if( ReadReg(MEMS_I2C_ADDRESS, INT2_SRC, &value) )
      return ERR_MEMS_ERROR;

  if(statusBIT == INT_SRC_IA){
    if(value &= INT_SRC_IA)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_ZH){
    if(value &= INT_SRC_ZH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_ZL){
    if(value &= INT_SRC_ZL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_YH){
    if(value &= INT_SRC_YH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_YL){
    if(value &= INT_SRC_YL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_XH){
    if(value &= INT_SRC_XH)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }

  if(statusBIT == INT_SRC_XL){
    if(value &= INT_SRC_XL)    return E_OK;
    else  return ERR_MEMS_ERROR;
  }
return ERR_MEMS_ERROR;
}
#endif


#ifdef LIS331DLH
void Accel_Configuration(void)
{
	uint8_t val;

	_DEBUG("Accel configuration...");

	if (GetWHO_AM_I(&val))
		_DEBUG("Can't connect to accelerometer", NULL);
	if (val != 0x32)
		_DEBUG("Can't get product ID", NULL);

	_DEBUG("Done accel.");

	SetODR(ODR_1000Hz);

	SetMode(NORMAL);

	SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);

	SetFullScale(FULLSCALE_2);

	SetBDU(MEMS_ENABLE);

	SetBLE(BLE_LSB);

	/* have no idea how to use it */
	// if (SetSelfTest(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	// if (SetSelfTestSign(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	/* boot configuration - questions yet! */
	// if (SetBOOT(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

/* irq configuration */

	/* active irq state - high(default) */
	SetIntHighLow(MEMS_DISABLE);

	/* setup push-pull */
	SetIntPPOD(MEMS_DISABLE);

	/* irq source - int1 */
	SetInt1DataSign(INT_SOURCE);

	/* filter enable */
	// if (SetInt1HPenable(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	/* int_src not latched */
	Int1LatchEnable(MEMS_ENABLE);

	/* use this function to reset int_src */
	// if (ResetInt1Latch())
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	/* irq enable any axis */
	SetInt1Configuration(INT_XHIE_ENABLE | INT_YHIE_ENABLE |INT_ZHIE_ENABLE | INT_OR);

	/* ~2g */
	SetInt1Threshold(30);

	/* duration */
//	SetInt1Duration(0);

	/* movement configuration */
	SetInt1Mode(INT_MODE_6D_MOVEMENT);

	/* disable to wakeup for testing!!! */
	TurnONEnable(SLEEP_TO_WAKE_DIS);

/* filters configuration */

	/* code here */
}
#elif LIS331DLM
void Accel_Configuration(void)
{
	uint8_t val;

	_DEBUG("Accel configuration...");

	if (GetWHO_AM_I(&val))
		_DEBUG("Can't connect to accelerometer", NULL);
	if (val != 0x12)
		_DEBUG("Can't get product ID");

	SetODR(ODR_400Hz);

	SetMode(NORMAL);

	/* Default mode - all axis are enabled */
	SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);

	SetFullScale(FULLSCALE_8);

	/* have no idea how to use it */
	// if (SetSelfTest(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	// if (SetSelfTestSign(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

	/* boot configuration - questions yet! */
	// if (SetBOOT(MEMS_ENABLE))
	// 	return ERR_CANT_CONFIGURE_ACCEL;

/* irq configuration */

	/* active irq state - high(default) */
	SetIntHighLow(MEMS_DISABLE);

	/* setup push-pull */
	SetIntPPOD(MEMS_DISABLE);

	/* irq source - int1 */
	SetInt1DataSign(INT_SOURCE);

	/* filter enable */
//	SetInt1HPenable(MEMS_ENABLE);

	/* int_src not latched */
	Int1LatchEnable(MEMS_ENABLE);

	/* irq enable any axis */
	SetInt1Configuration(INT_XHIE_DISABLE | INT_XLIE_DISABLE |
						INT_YHIE_DISABLE | INT_YLIE_DISABLE |
						INT_ZHIE_DISABLE | INT_ZLIE_DISABLE | INT_OR);

	/* ~Xg */
	SetInt1Threshold(20);

	/* movement configuration */
	SetInt1Mode(INT_MODE_OR);

	/* wake up */
	TurnONEnable(SLEEP_TO_WAKE_ENA);

	ResetInt1Latch();

/* filters configuration */

	/* code here */

	_DEBUG("Done accel.");
}
#endif


void setAccel(FunctionalState state)
{
	(state == ENABLE) ? SetMode(NORMAL) : SetMode(POWER_DOWN);
}


