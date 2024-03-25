/**
  ******************************************************************************
  * @file    rf_driver_ll_radio_subghz.c
  * @author  RF Application Team
  * @brief   SPIRIT3 MRSubG module APIs
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
#include "rf_driver_ll_radio_subghz.h"
#include "rf_driver_ll_utils.h"

#ifdef  USE_FULL_ASSERT
#include "rf_driver_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

#define MAX_DBM		0x51
#define GAIN_RX_CHAIN   64

#define S_ABS(a) ((a)>0?(a):-(a))

#define IS_FREQUENCY_BAND(FREQUENCY) (IS_FREQUENCY_BAND_HIGH(FREQUENCY) || \
                                      IS_FREQUENCY_BAND_LOW(FREQUENCY))

#define IS_FREQUENCY_BAND_HIGH(FREQUENCY) ((FREQUENCY)>=HIGH_BAND_LOWER_LIMIT && \
                                           (FREQUENCY)<=HIGH_BAND_UPPER_LIMIT)


#define IS_FREQUENCY_BAND_LOW(FREQUENCY) ((FREQUENCY)>=LOW_BAND_LOWER_LIMIT && \
                                             (FREQUENCY)<=LOW_BAND_UPPER_LIMIT)

#define IS_MODULATION(MOD) (((MOD) == MOD_NO_MOD) || \
					((MOD) == MOD_2FSK) || \
					((MOD) == MOD_4FSK) || \
					((MOD) == MOD_2GFSK05) || \
					((MOD) == MOD_2GFSK1)  || \
					((MOD) == MOD_4GFSK05) || \
					((MOD) == MOD_4GFSK1) || \
					((MOD) == MOD_ASK) || \
					((MOD) == MOD_OOK) || \
					((MOD) == MOD_POLAR) || \
					((MOD) == MOD_CW) )


#define IS_DATARATE(DATARATE)      (DATARATE>=MINIMUM_DATARATE && DATARATE<=((uint64_t)MAXIMUM_DATARATE)

static const uint32_t s_Channel_Filter_Bandwidth[99]=
{
  1600000,1510000,1422000,1332000,1244000,1154000,1066000, \
  976000,888000,800000,755000,711000,666000,622000,577000, \
  533000,488000,444000,400000,377000,355000,333000,311000, \
  288000,266000,244000,222000,200000,188000,178000,166000, \
  155000,144000,133000,122000,111000,100000,94400,88900,83300, \
  77800,72200,66700,61100,55600,50000,47200,44400,41600,38900, \
  36100,33300,30500,27800,25000,23600,22200,20800,19400,18100, \
  16600,15300,13900,12500,11800,11100,10400,9700,9000,8300,7600, \
  6900,6125,5910,5550,5200,4870,4500,4100,3800,3500,3125,2940, \
  2780,2600,2400,2200,2100,1900,1700
};

static uint32_t MRSubG_ComputeDatarate(uint16_t cM, uint8_t cE);
static void MRSubG_SearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE);
static void MRSubG_SearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE, uint8_t bs);
static uint32_t MRSubG_ComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs);
static void MRSubG_SearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE);
static void MRSubG_ComputeSynthWord(uint32_t frequency, uint8_t* synth_int, uint32_t* synth_frac, uint8_t* band);
static int32_t MRSubG_ConvertRssiToDbm(uint16_t rssi_level_from_register);
static uint8_t MRSubG_GetAllowedMaxOutputPower(MRSubG_PA_DRVMode paMode);
static void MRSUBG_EvaluateDSSS(MRSubGModSelect xModulation, uint8_t dsssExponent);

// ---------------> Internal (private) MR_SUBG radio functions

/**
* @brief  Computes the synth word from a given frequency.
* @param  frequency Target frequency value expressed in Hz.
* @param  synth_int pointer to the int part of the synth word
* @param  synth_frac pointer to the fract part of the synth word
* @param  band pointer to the high/low band selector
* @retval None.
*/
static void MRSubG_ComputeSynthWord(uint32_t frequency, uint8_t* synth_int, uint32_t* synth_frac, uint8_t* band)
{
  if(IS_FREQUENCY_BAND_HIGH(frequency)) {
    *band = HIGH_BAND_FACTOR;
  }
  else {
    *band = LOW_BAND_FACTOR;
  }

  *synth_int = (uint32_t)(*band * frequency/LL_GetXTALFreq());

  *synth_frac = (uint32_t)(((*band * (uint64_t)frequency * (1<<20))/LL_GetXTALFreq()) - (*synth_int * (1<<20)));
}

/**
* @brief  Evaluate the data rate.
* @param  cM the mantissa value.
* @param  cE the exponent value.
* @retval The datarate.
*/
static uint32_t MRSubG_ComputeDatarate(uint16_t cM, uint8_t cE)
{
  uint32_t f_sys=LL_GetXTALFreq()/3; //16 MHz nominal
  uint64_t dr;

  if(cE==0){
    dr = ((uint64_t)f_sys*cM);
    return (uint32_t)(dr>>32);
  }
  else if(cE==15){
	return ((uint64_t)f_sys*(8*cM));
  }
  else{
    dr = ((uint64_t)f_sys)*((uint64_t)cM+65536);
    return (uint32_t)(dr>>(33-cE));
  }
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the datarate formula
*         will give the datarate value closer to the given datarate.
* @param  lDatarate datarate expressed in sps.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
static void MRSubG_SearchDatarateME(uint32_t lDatarate, uint16_t* pcM, uint8_t* pcE)
{
  uint32_t lDatarateTmp, f_sys=LL_GetXTALFreq()/3;
  uint8_t uDrE;
  uint64_t tgt1,tgt2,tgt;

  /* Search the exponent value */
  for(uDrE = 0; uDrE<16; uDrE++) {
    lDatarateTmp = MRSubG_ComputeDatarate(0xFFFF, uDrE);
    if(lDatarate<=lDatarateTmp)
      break;
  }
  (*pcE) = (uint8_t)uDrE;

  if(uDrE==0) {
    tgt=((uint64_t)lDatarate)<<32;
    (*pcM) = (uint16_t)(tgt/f_sys);
    tgt1=(uint64_t)f_sys*(*pcM);
    tgt2=(uint64_t)f_sys*((*pcM)+1);
  }
  else {
    tgt=((uint64_t)lDatarate)<<(33-uDrE);
    (*pcM) = (uint16_t)((tgt/f_sys)-65536);
    tgt1=(uint64_t)f_sys*((*pcM)+65536);
    tgt2=(uint64_t)f_sys*((*pcM)+1+65536);
  }

  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the Frequency Deviation formula
*         will give the value closer to the given one.
* @param  cM the mantissa value.
* @param  cE the exponent value.
* @param  bs the band value.
* @retval The frequency deviation.
*/
static uint32_t MRSubG_ComputeFreqDeviation(uint8_t cM, uint8_t cE, uint8_t bs){
  uint32_t f_xo = LL_GetXTALFreq();

  if(cE==0) {
    return (uint32_t)((uint64_t)f_xo*(cM*bs/8)/(bs*(1<<19)));
  }

  return (uint32_t)((uint64_t)f_xo*((256+cM)*(1<<(cE-1))*bs/8)/(bs*(1<<19)));;
}

/**
* @brief  Returns the mantissa and exponent, whose value used in the Frequency Deviation formula
*         will give the value closer to the given one.
* @param  lFDev frequency deviation expressed in Hz.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @param  bs the high/low band selector
* @retval None.
*/
static void MRSubG_SearchFreqDevME(uint32_t lFDev, uint8_t* pcM, uint8_t* pcE, uint8_t bs){
  uint8_t uFDevE;
  uint32_t lFDevTmp;
  uint64_t tgt1,tgt2,tgt;

  /* Search the exponent of the frequency deviation value */
  for(uFDevE = 0; uFDevE != 12; uFDevE++) {
    lFDevTmp = MRSubG_ComputeFreqDeviation(255, uFDevE, bs);
    if(lFDev<lFDevTmp)
      break;
  }
  (*pcE) = (uint8_t)uFDevE;

  if(uFDevE==0)
  {
    tgt=((uint64_t)lFDev)<<22;
    (*pcM)=(uint32_t)(tgt/LL_GetXTALFreq());
    tgt1=(uint64_t)LL_GetXTALFreq()*(*pcM);
    tgt2=(uint64_t)LL_GetXTALFreq()*((*pcM)+1);
  }
  else
  {
    tgt=((uint64_t)lFDev)<<(23-uFDevE);
    (*pcM)=(uint32_t)(tgt/LL_GetXTALFreq())-256;
    tgt1=(uint64_t)LL_GetXTALFreq()*((*pcM)+256);
    tgt2=(uint64_t)LL_GetXTALFreq()*((*pcM)+1+256);
  }

  (*pcM)=((tgt2-tgt)<(tgt-tgt1))?((*pcM)+1):(*pcM);
}

/**
* @brief  Returns the mantissa and exponent for a given bandwidth.
*         The API will search the closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet, returning the corresponding mantissa
*         and exponent value.
* @param  lBandwidth bandwidth expressed in Hz. This parameter ranging between 1700 and 1600000.
* @param  pcM pointer to the returned mantissa value.
* @param  pcE pointer to the returned exponent value.
* @retval None.
*/
static void MRSubG_SearchChannelBwME(uint32_t lBandwidth, uint8_t* pcM, uint8_t* pcE)
{
  int8_t i, i_tmp;
  uint32_t f_dig=LL_GetXTALFreq()/3;
  int32_t chfltCalculation[3];

  assert_param(IS_CH_BW(lBandwidth, f_dig));

  /* Search the channel filter bandwidth table index */
  for(i=0;i<99 && (lBandwidth<(uint32_t)(((uint64_t)s_Channel_Filter_Bandwidth[i]*f_dig)/16000000));i++);

  if(i!=0) {
    /* Finds the index value with best approximation in i-1, i and i+1 elements */
    i_tmp = i;

    for(uint8_t j=0;j<3;j++) {
      if(((i_tmp+j-1)>=0) && ((i_tmp+j-1)<=98)) {
        chfltCalculation[j] = (int32_t)lBandwidth - (int32_t)(((uint64_t)s_Channel_Filter_Bandwidth[i_tmp+j-1]*f_dig)/16000000);
      }
      else {
        chfltCalculation[j] = 0x7FFFFFFF;
      }
    }
    uint32_t chfltDelta = 0xFFFFFFFF;

    for(uint8_t j=0;j<3;j++) {
      if(S_ABS(chfltCalculation[j])<chfltDelta) {
        chfltDelta = S_ABS(chfltCalculation[j]);
        i=i_tmp+j-1;
      }
    }
  }
  (*pcE) = (uint8_t)(i/9);
  (*pcM) = (uint8_t)(i%9);    
}

/*
 * @brief Convert RSSI_LEVEL_ON_SYNC to dBm
 * @param rssi_level_from_register the value to convert
 * @retval The converted RSSI level in dBm
*/
static int32_t MRSubG_ConvertRssiToDbm(uint16_t rssi_level_from_register){
  return (rssi_level_from_register/2)-(96+GAIN_RX_CHAIN);
}

/**
* @brief  Returns the maximum allowed output power supported by the specific configuration
* @param  paDrvMode the configuration type.
* @retval The maximum output power.
*/
static uint8_t MRSubG_GetAllowedMaxOutputPower(MRSubG_PA_DRVMode paDrvMode){
  uint8_t retPwr = 20;
    
  switch(paDrvMode){
  case PA_DRV_TX:
    retPwr = 10;
    break;
  case PA_DRV_TX_HP:
    retPwr = 14;
    break;
  case PA_DRV_TX_TX_HP:
    retPwr = 18; /* Max allowed power without PA_DEGEN_ON */
    break;
  }
  
  return retPwr;
}

static void MRSUBG_EvaluateDSSS(MRSubGModSelect xModulation, uint8_t dsssExponent){
  //Disable DSSS for modulations different from 2(G)FSK
  if (xModulation != MOD_2GFSK05 && xModulation != MOD_2GFSK1){
    dsssExponent = 0;
  }

  uint8_t dsss_en = (dsssExponent > 0);
  uint8_t dsss_sf = (1 << dsssExponent); //Spread Factor = 2^dssExponent
  uint8_t dsss_acq_thr = (dsss_sf - 1);

  if (dsssExponent > 3) {
    dsss_acq_thr = dsss_sf/2;
  }

  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->DSSS_CTRL,  MR_SUBG_GLOB_STATIC_DSSS_CTRL_DSSS_EN, dsss_en);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->DSSS_CTRL,  MR_SUBG_GLOB_STATIC_DSSS_CTRL_SPREADING_EXP, dsssExponent);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->DSSS_CTRL,  MR_SUBG_GLOB_STATIC_DSSS_CTRL_ACQ_THR, dsss_acq_thr);
}

// End Of Internal (private) MR_SUBG radio functions <---------------

/**
* @brief  Get the IP version of the MRSubG.
* @param  None.
* @retval IP version.
*/
SMRSubGVersion MRSubGGetVersion(void)
{
  SMRSubGVersion MRSubGVersion;

  MRSubGVersion.product =  READ_REG_FIELD(MR_SUBG_GLOB_MISC->RFIP_VERSION, MR_SUBG_GLOB_MISC_RFIP_VERSION_PRODUCT);
  MRSubGVersion.version =  READ_REG_FIELD(MR_SUBG_GLOB_MISC->RFIP_VERSION, MR_SUBG_GLOB_MISC_RFIP_VERSION_VERSION);
  MRSubGVersion.revision = READ_REG_FIELD(MR_SUBG_GLOB_MISC->RFIP_VERSION, MR_SUBG_GLOB_MISC_RFIP_VERSION_REVISION);

  return MRSubGVersion;
}

/**
* @brief  Initializes the MR_SUBG radio interface according to the specified
*         parameters in the pxSRadioInitStruct.
* @param  pxSRadioInitStruct pointer to a SMRSubGConfig structure that
*         contains the configuration information for the MR_SUBG radio part of SPIRIT3.
* @retval Error code: 0=no error, 1=error during calibration of VCO.
*/
uint8_t MRSubG_Init(SMRSubGConfig* pxSRadioInitStruct){

  assert_param(IS_FREQUENCY_BAND(pxSRadioInitStruct->lFrequencyBase));
  assert_param(IS_MODULATION(pxSRadioInitStruct->xModulationSelect));
  assert_param(IS_DATARATE(pxSRadioInitStruct->lDatarate));

  //Setup design values for default registers
  MODIFY_REG_FIELD(MR_SUBG_RADIO->AFC1_CONFIG, MR_SUBG_RADIO_AFC1_CONFIG_AFC_FAST_PERIOD, 0x00);
  
  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL0, MR_SUBG_RADIO_CLKREC_CTRL0_PSTFLT_LEN, 0x01);
  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL0, MR_SUBG_RADIO_CLKREC_CTRL0_CLKREC_P_GAIN_FAST, 0x03);
  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL0, MR_SUBG_RADIO_CLKREC_CTRL0_CLKREC_I_GAIN_FAST, 0x08);

  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL1, MR_SUBG_RADIO_CLKREC_CTRL1_CLKREC_ALGO_SEL, 0x00);
  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL1, MR_SUBG_RADIO_CLKREC_CTRL1_CLKREC_P_GAIN_SLOW, 0x05);
  MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL1, MR_SUBG_RADIO_CLKREC_CTRL1_CLKREC_I_GAIN_SLOW, 0x0C);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->DSSS_CTRL, MR_SUBG_GLOB_STATIC_DSSS_CTRL_ACQ_HITS, 0x03);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->DSSS_CTRL, MR_SUBG_GLOB_STATIC_DSSS_CTRL_ACQ_WINDOW, 0x04);          

  //Enable calibration
  SET_BIT(MR_SUBG_GLOB_DYNAMIC->VCO_CAL_CONFIG, MR_SUBG_GLOB_DYNAMIC_VCO_CAL_CONFIG_VCO_CALIB_REQ);

  //Avoid AGC glitches
  WRITE_REG(MR_SUBG_RADIO->RF_FSM7_TIMEOUT, 0x0F);

  //Set synth registers
  MRSubG_SetFrequencyBase(pxSRadioInitStruct->lFrequencyBase);

  //Configure modulation
  MRSubG_SetModulation(pxSRadioInitStruct->xModulationSelect, pxSRadioInitStruct->dsssExp);

  //Configure datarate
  MRSubG_SetDatarate(pxSRadioInitStruct->lDatarate);

  //Configure frequency deviation
  MRSubG_SetFrequencyDev(pxSRadioInitStruct->lFreqDev);

  //Configure RX channel bandwidth
  MRSubG_SetChannelBW(pxSRadioInitStruct->lBandwidth);

  return 0;
}

/**
* @brief  Set the PA Mode.
* @param  paMode the PA Mode.
* @retval None.
*/
void MRSubG_SetPAMode(MRSubG_PAMode paMode){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_MODE, paMode);
}

/**
* @brief  Set the PA Drive Mode.
* @param  paMode the PA Drive Mode.
* @retval None.
*/
void MRSubG_SetPADriveMode(MRSubG_PA_DRVMode paDrvMode){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_DRV_MODE, paDrvMode);
}

/**
* @brief  Set the Synth word and the Band Select register according to desired base carrier frequency.
* @param  lFBase the base carrier frequency expressed in Hz as unsigned word.
* @retval None.
*/
void MRSubG_SetFrequencyBase(uint32_t lFBase){
  uint8_t band;
  uint8_t synth_int;
  uint32_t synth_frac;

  assert_param(IS_FREQUENCY_BAND(lFBase));

  MRSubG_ComputeSynthWord(lFBase, &synth_int, &synth_frac, &band);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_SYNTH_INT, synth_int);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_SYNTH_FRAC, synth_frac);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_BS, (band/4)-1);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->ADDITIONAL_CTRL, MR_SUBG_GLOB_DYNAMIC_ADDITIONAL_CTRL_CH_NUM, 0x00);
}

/**
* @brief  Set the modulation type.
* @param  xModulation modulation to set.
*         This parameter shall be of type @ref MRSubGModSelect.
* @param  dsssExponent the DSSS spreading exponent. 0 means DSSS disabled.
* @retval None.
*/
void MRSubG_SetModulation(MRSubGModSelect xModulation, uint8_t dsssExponent){
  assert_param(IS_MODULATION(xModulation));  

  //Internal equalizer
  switch(xModulation){
  case MOD_2GFSK05:
  case MOD_4GFSK05: 
  case MOD_2GFSK1: 
  case MOD_4GFSK1:
    //In case of gaussian filter, in order to reduce intersymbol interference (ISI),
    //we have to set the internal equalizer to 2 symbols
    MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->AS_QI_CTRL, MR_SUBG_GLOB_STATIC_AS_QI_CTRL_AS_EQU_CTRL, 0x02);
    break;
  default:
    //For non gaussian modulation set internal equalizer to 0 symbols
    MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->AS_QI_CTRL, MR_SUBG_GLOB_STATIC_AS_QI_CTRL_AS_EQU_CTRL, 0x00);   
  }  

  //Post filter
  switch(xModulation){
  case MOD_4GFSK05: 
  case MOD_4GFSK1:
  case MOD_4FSK:    
    //In case of 4 level FSK modulation, in order to reduce intersymbol interference (ISI),
    //we have to set the post filter len equal to 8 (register value 0).
    MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL0, MR_SUBG_RADIO_CLKREC_CTRL0_PSTFLT_LEN, 0x00);        
    break;
  default:
    MODIFY_REG_FIELD(MR_SUBG_RADIO->CLKREC_CTRL0, MR_SUBG_RADIO_CLKREC_CTRL0_PSTFLT_LEN, 0x01);
  }
  
  //Modulation
  switch(xModulation){
  case MOD_2GFSK05:
  case MOD_4GFSK05:
    MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_BT_SEL, 1);
    xModulation &= 0x0F;
    break;
  default:
    MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_BT_SEL, 0);
  }
  
  //Evaluate DSSS settings
  MRSUBG_EvaluateDSSS(xModulation, dsssExponent);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_MOD_TYPE, xModulation);
}

/**
* @brief  Force the SYNC word to be formatted as 2-(G)FSK bit mapping instead of 4-(G)FSK.
* @param  forceSync DISABLE: the SYNC word is treated in 4-(G)FSK mapping as the rest of the frame.
*                   ENABLE: the SYNC word of the frame is mapped as follows
*                       '1' is mapped to +Fdev; '0' is mapped to -Fdev for constellation mapping = 0 and 1
*                       '1' is mapped to -Fdev; '0' is mapped to +Fdev for constellation mapping = 2 and 3
*         This parameter shall be of type @ref FunctionalState.
* @retval None.
*/
void MRSubG_Force2FSKSync(FunctionalState forceSync){
  if(forceSync == ENABLE)
    SET_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_FORCE_2FSK_SYNC_MODE);
  else
    CLEAR_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_FORCE_2FSK_SYNC_MODE);
}

/**
* @brief  Return the modulation type used.
* @param  None.
* @retval MRSubGModSelect Settled modulation type.
*/
MRSubGModSelect MRSubG_GetModulation(void)
{
  MRSubGModSelect retMod;
  retMod = (MRSubGModSelect)(READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_MOD_TYPE));

  if(retMod == MOD_2GFSK1 || retMod == MOD_4GFSK1)
  {
    /* Check the BT_SEL bit to evaluate if xGFSK1 or 05 */
    uint8_t bt = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_BT_SEL);

    if (bt) { retMod |= 0x10; }
  }

  return retMod;
}

/**
* @brief  Set the datarate.
* @param  lDatarate datarate expressed in sps.
* @retval None.
*/
void MRSubG_SetDatarate(uint32_t lDatarate){
  uint8_t dr_e;
  uint16_t dr_m;

  assert_param(IS_DATARATE(lDatarate));

  /* Calculates the datarate mantissa and exponent */
  MRSubG_SearchDatarateME(lDatarate, &dr_m, &dr_e);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_DATARATE_M, dr_m);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_DATARATE_E, dr_e);
}

/**
* @brief  Set the frequency deviation.
* @param  lFDev frequency deviation expressed in Hz.
* @retval None.
*/
void MRSubG_SetFrequencyDev(uint32_t lFDev){
  uint8_t uFDevM, uFDevE;
  uint8_t bs = ((READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_BS)+1)*4);

  /* Calculates the frequency deviation mantissa and exponent */
  MRSubG_SearchFreqDevME(lFDev, &uFDevM, &uFDevE, bs);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_FDEV_M, uFDevM);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_FDEV_E, uFDevE);
}

/**
* @brief  Set the channel filter bandwidth.
* @param  lBandwidth channel filter bandwidth expressed in Hz.
*         The API will search the most closer value according to a fixed table of channel
*         bandwidth values (@ref s_vectnBandwidth), as defined in the datasheet. To verify the settled channel bandwidth
*         it is possible to use the MRSubG_GetChannelBW API.
* @retval None.
*/
void MRSubG_SetChannelBW(uint32_t lBandwidth)
{
  uint8_t uBwM = 0;
  uint8_t uBwE = 0;  
  uint32_t f_if = 0;
  uint32_t if_offset = 0;
  uint32_t chf_threshold = 400000;
  uint32_t f_dig=LL_GetXTALFreq()/3;

  assert_param(IS_CH_BW(lBandwidth,(LL_GetXTALFreq())));

  //Calculates the channel bandwidth mantissa and exponent
  MRSubG_SearchChannelBwME(lBandwidth, &uBwM, &uBwE);

  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_CHFLT_M, uBwM);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_CHFLT_E, uBwE);

  //Set IF to 600 kHz, if channel filter requested is greater then CHF threshold
  if (lBandwidth > chf_threshold){    
    SET_BIT(MR_SUBG_GLOB_STATIC->IF_CTRL, MR_SUBG_GLOB_STATIC_IF_CTRL_IF_MODE);

    //Define f_if
    f_if = 600;
  }
  else{
    //Set antialiasing filter to 684kHz
    CLEAR_BIT(MR_SUBG_GLOB_STATIC->IF_CTRL, MR_SUBG_GLOB_STATIC_IF_CTRL_IF_MODE);    

    //Define f_if
    f_if = 300;
  }

  if_offset = (((f_if*100)*65536)/f_dig)*10;
  
  //Set IF
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->IF_CTRL, MR_SUBG_GLOB_STATIC_IF_CTRL_IF_OFFSET_ANA, if_offset);
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->IF_CTRL, MR_SUBG_GLOB_STATIC_IF_CTRL_IF_OFFSET_DIG, if_offset);
}

/**
* @brief  Return the channel filter bandwidth.
* @param  None.
* @retval uint32_t Channel filter bandwidth expressed in Hz.
*/
uint32_t MRSubG_GetChannelBW(void)
{
  uint8_t bs = ((READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_BS)+1)*4);
  uint8_t cm, ce;

  uint32_t fclk = (LL_GetXTALFreq()/3);
  uint32_t correction_factor = fclk/16000000;

  cm = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_CHFLT_M);
  ce = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_CHFLT_E);

  uint8_t index = ce*9 + cm;
  return correction_factor*s_Channel_Filter_Bandwidth[index];
}

/**
* @brief  Return the base carrier frequency.
* @param  None.
* @retval uint32_t Base carrier frequency expressed in Hz as unsigned word.
*/
uint32_t MRSubG_GetFrequencyBase(void)
{
  uint8_t  bs = ((READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_BS)+1)*4);
  uint8_t  synth_int = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_SYNTH_INT);
  uint32_t synth_frac = READ_REG(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ) & MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_SYNTH_FRAC;
  uint32_t fbase;

  fbase = LL_GetXTALFreq()*((uint64_t)(synth_int*(1<<20) + synth_frac))/(bs*(1<<20));

  return fbase;
}

/**
* @brief  Return the datarate.
* @param  None.
* @retval uint32_t Datarate expressed in sps.
*/
uint32_t MRSubG_GetDatarate(void)
{
  uint32_t dr, datarateM, datarateE;
  uint32_t f_sys;

  datarateM = READ_REG(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG) & MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_DATARATE_M;
  datarateE = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_DATARATE_E);

  f_sys = (uint32_t)(LL_GetXTALFreq()/3);

  if (datarateE == 0)
	dr = f_sys*datarateM/(uint64_t)1<<32;
  else if (datarateE == 15) // Jitter free mode
    dr = f_sys/(8*datarateM);
  else
    dr = f_sys*((uint64_t)(1<<16) + datarateM)*(1<<datarateE)/((uint64_t)1<<33);

  return dr;
}

/**
* @brief  Return the frequency deviation.
* @param  None.
* @retval uint32_t Frequency deviation value expressed in Hz.
*/
uint32_t MRSubG_GetFrequencyDev(void)
{
  uint8_t fdev_m;
  uint8_t fdev_e;
  uint32_t f_dev;
  uint16_t factor1;
  uint32_t factor2;
  uint8_t bs = ((READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->SYNTH_FREQ, MR_SUBG_GLOB_DYNAMIC_SYNTH_FREQ_BS)+1)*4);

  fdev_m = READ_REG(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG) & MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_FDEV_M;
  fdev_e = READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD1_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD1_CONFIG_FDEV_E);

  factor1 = fdev_e==0 ? 0 : 256;
  factor2 = fdev_e==0 ? 0 : (fdev_e-1);

  f_dev = (LL_GetXTALFreq()*(uint64_t)((factor1+fdev_m)*(1<<(factor2))*bs/8)/(bs*(1<<19)));

  return f_dev;
}

/**
 * @brief  Set the CONST_MAP configuration for
 * 2-(G)FSK or 4-(G)FSK
 * @param  nConMap the configuration.
 * @retval None.
 */
void MRSubG_SetConstellationMapping(uint8_t nConMap){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_CONST_MAP, nConMap);
}

/**
 * @brief Invert bit to symbol mapping for 4(G)FSK
 * @param  xNewState new state for FOUR_FSK_SYM_SWAP bit
 * @retval None.
 */
void MRSubG_SwapSymbolMapping(FunctionalState xNewState){
  assert_param(IS_FUNCTIONAL_STATE(xNewState));

  if(xNewState == ENABLE)
    SET_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_FOUR_FSK_SYM_SWAP);
  else
    CLEAR_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_FOUR_FSK_SYM_SWAP);
}

/**
 * @brief Select BT value for GFSK
 * @param  bSel BT value
 * 	This value is type of @ref MRSubG_BTSelect.
 * @retval None.
 */
void MRSubG_SetBTSel(MRSubG_BTSelect bSel){
  if(bSel == BT_05)
    SET_BIT(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_BT_SEL);
  else
    CLEAR_BIT(MR_SUBG_GLOB_DYNAMIC->MOD0_CONFIG, MR_SUBG_GLOB_DYNAMIC_MOD0_CONFIG_BT_SEL);
}

/**
 * @brief Select BT value for GFSK
 * @param  bSel BT value
 * 	This value is type of @ref MRSubG_BTSelect.
 * @retval None.
 */
void MRSubG_SetFrequencyInterpolator(FunctionalState xNewState){
  assert_param(IS_FUNCTIONAL_STATE(xNewState));

  if(xNewState == ENABLE)
    SET_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_MOD_INTERP_EN);
  else
    CLEAR_BIT(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_MOD_INTERP_EN);
}

/**
 * @brief Set the ISI cancellation equalizer
 * @param  isiEq the Equalizer value
 * 	This value is type of @ref MRSubG_ISIEQMode.
 * @retval None.
 */
void MRSubG_SetISIEqualizer(MRSubG_ISIEQMode isiEq){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->AS_QI_CTRL, MR_SUBG_GLOB_STATIC_AS_QI_CTRL_AS_EQU_CTRL, isiEq);
}

/**
 * @brief  Returns the RSSI value.
 * @param  None.
 * @retval int32_t RSSI value.
 */
int32_t MRSubG_GetRssidBm(void)
{
  // Return RSSI value in dBm from content of RSSI_LEVEL_ON_SYNC field
  uint16_t rssiReg = READ_REG(MR_SUBG_GLOB_STATUS->RX_INDICATOR) & MR_SUBG_GLOB_STATUS_RX_INDICATOR_RSSI_LEVEL_ON_SYNC;
  return MRSubG_ConvertRssiToDbm(rssiReg);
}

/**
 * @brief  Set the value for RSSI threshold according to the following formula:
 * RSSIdBm = (rssi_level_xx/2)-(96+GAIN_RX_CHAIN) - See equation 7 in UM_MR_SubG_IP
 * @param  The desired RSSI threshold in dBm.
 * @retval None.
 */
void MRSubG_SetRSSIThreshold(int16_t rssiTh){
  uint16_t rssiValReg = 2*(rssiTh+(96+GAIN_RX_CHAIN));
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->AS_QI_CTRL, MR_SUBG_GLOB_STATIC_AS_QI_CTRL_RSSI_THR, rssiValReg);
}

/**
 * @brief  Get the value for RSSI threshold in dBm
 * @param  None.
 * @retval int32_t the RSSI Threshold in dBm.
 */
int32_t MRSubG_GetRSSIThreshold(void){
  // Return RSSI Threshold
  uint16_t rssiReg = READ_REG(MR_SUBG_GLOB_STATIC->AS_QI_CTRL) & MR_SUBG_GLOB_STATIC_AS_QI_CTRL_RSSI_THR;
  return MRSubG_ConvertRssiToDbm(rssiReg);
}

/**
 * @brief  Send a specific command to the SPIRIT3.
 * @param  xCommandCode code of the command to send.
           This parameter can be any value of @ref MRSubGCmd.
 * @retval None.
 */
void MRSubG_StrobeCommand(MRSubGCmd xCommandCode) {
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->COMMAND, MR_SUBG_GLOB_DYNAMIC_COMMAND_COMMAND_ID, xCommandCode);
}

/**
 * @brief  Set the TX_MODE field of the PCKT_CTRL register.
 * @param  The TX Mode.
 *	    This parameter can be any value of @ref MRSubGTXMode.
 * @retval None.
 */
void MRSubG_SetTXMode(MRSubGTXMode txMode){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_TX_MODE, txMode);
}

/**
 * @brief  Sets the RX_MODE field of PCKT_CTRL register.
 * @param  rxMode the rx mode.
 *	    This parameter can be any value of @ref MRSubGRXMode.
 * @retval None.
 */
void MRSubG_SetRXMode(MRSubGRXMode rxMode){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PCKT_CTRL, MR_SUBG_GLOB_STATIC_PCKT_CTRL_RX_MODE, rxMode);
}

/**
 * @brief  Get the main radio info for the current configuration.
 * @param  pxSRadioInitStruct pointer to a structure of the type of @ref SMRSubGConfig
 * @retval None.
 */
void MRSubG_GetInfo(SMRSubGConfig* pxSRadioInitStruct){
  pxSRadioInitStruct->lFrequencyBase = MRSubG_GetFrequencyBase();
  pxSRadioInitStruct->xModulationSelect = MRSubG_GetModulation();
  pxSRadioInitStruct->lDatarate = MRSubG_GetDatarate();
  pxSRadioInitStruct->lFreqDev = MRSubG_GetFrequencyDev();
  pxSRadioInitStruct->lBandwidth = MRSubG_GetChannelBW();
}

/**
* @brief  Sets the channel number.
* @param  cChannel the channel number.
* @retval None.
*/
void MRSubG_SetChannel(uint8_t cChannel){
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->ADDITIONAL_CTRL, MR_SUBG_GLOB_DYNAMIC_ADDITIONAL_CTRL_CH_NUM, cChannel);
}

/**
* @brief  Returns the actual channel number.
* @param  None.
* @retval uint8_t Actual channel number.
*/
uint8_t MRSubG_GetChannel(void){
  return READ_REG(MR_SUBG_GLOB_DYNAMIC->ADDITIONAL_CTRL) & MR_SUBG_GLOB_DYNAMIC_ADDITIONAL_CTRL_CH_NUM;
}

/**
* @brief  Set the channel spacing factor in channel space register.
*         The channel spacing is From ~732 Hz to ~187 kHz in 732 Hz steps
* @param  lChannelSpace the channel spacing expressed in Hz.
* @retval None.
*/
void MRSubG_SetChannelSpace(uint8_t lChannelSpace)
{
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->ADDITIONAL_CTRL, MR_SUBG_GLOB_DYNAMIC_ADDITIONAL_CTRL_CH_SPACING, lChannelSpace);
}

/**
* @brief  Return the channel space register.
* @param  None.
* @retval uint8_t Channel spacing.
*/
uint8_t MRSubG_GetChannelSpace(void)
{
  return READ_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->ADDITIONAL_CTRL, MR_SUBG_GLOB_DYNAMIC_ADDITIONAL_CTRL_CH_SPACING);
}

/**
* @brief  Set the PA_DEGEN_ON bit. This will Enable/Disable a degeneration mode, which introduces a pre-distortion to linearize the power control curve.
* @param  xNewState enable (ENABLE) to enable or disable (DISABLE) the degeneration.
* @retval None.
*/
void MRSubG_SetPADegen(FunctionalState xNewState) {
  if(xNewState == ENABLE) {
    SET_BIT(MR_SUBG_RADIO->PA_REG, MR_SUBG_RADIO_PA_REG_PA_DEGEN_ON);
  } else {
    CLEAR_BIT(MR_SUBG_RADIO->PA_REG, MR_SUBG_RADIO_PA_REG_PA_DEGEN_ON);
  }
}

/**
* @brief  Sets a specific PA_LEVEL register, with a value given in dBm.
* @param  cIndex PA_LEVEL to set. This parameter shall be in the range [0:7].
* @param  lPowerdBm PA value to write expressed in dBm.
* @param  mode PA drive modes.
*	    This parameter can be any value of @ref MRSubG_PA_DRVMode.
* @retval None.
*/
void MRSubG_SetPALeveldBm(uint8_t cIndex, int8_t lPowerdBm, MRSubG_PA_DRVMode drvMode)
{
  int32_t pa03 = 0;
  int32_t pa07 = 0;
  uint8_t maxAllowedValue;

  SET_BIT(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_RAMP_ENABLE);

  MRSubG_SetPAMode(PA_LEGACY);
  MRSubG_SetPADriveMode(drvMode);
  
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_LEVEL_MAX_INDEX, cIndex);
  
  maxAllowedValue = MRSubG_GetAllowedMaxOutputPower(drvMode);

  if(lPowerdBm > maxAllowedValue){
    lPowerdBm = 0x51;
    MRSubG_SetPADegen(ENABLE);
  }
  else{
    int8_t tmpPow;
    tmpPow = MAX_DBM-((maxAllowedValue-lPowerdBm)*2);
    
    lPowerdBm = tmpPow>0?tmpPow:0;
  }
  
  for (int i=cIndex; i>=0; i--){
    if(i<4)
      pa03 |= (int32_t)(lPowerdBm)<<(i*8);
    else
      pa07 |= (int32_t)(lPowerdBm)<<((i%4)*8);
    
    lPowerdBm = (lPowerdBm - 10) > 0 ? lPowerdBm - 10 : 0;
  }

  WRITE_REG(MR_SUBG_GLOB_STATIC->PA_LEVEL_3_0, pa03);
  WRITE_REG(MR_SUBG_GLOB_STATIC->PA_LEVEL_7_4, pa07);
}

/**
* @brief  Returns a value in dBm.
* @param  None
* @retval int32_t Settled power level expressed in dBm.
*/
int8_t MRSubG_GetPALeveldBm(void)
{
  int32_t retDbm;

  /* Get the max PA Index */
  uint8_t maxIdx = MRSubG_GetPALevelMaxIndex();

  if(maxIdx < 4){
    retDbm = READ_REG(MR_SUBG_GLOB_STATIC->PA_LEVEL_3_0) & MR_SUBG_GLOB_STATIC_PA_LEVEL_3_0_PA_LEVEL0;
    return retDbm << maxIdx * 8;
  }
  else {
    retDbm = READ_REG(MR_SUBG_GLOB_STATIC->PA_LEVEL_7_4) & MR_SUBG_GLOB_STATIC_PA_LEVEL_7_4_PA_LEVEL4;
    return retDbm << (maxIdx%4) * 8;
  }
}

/**
* @brief  Returns the actual PA_LEVEL_MAX_INDEX.
* @param  None.
* @retval uint8_t Actual PA_LEVEL_MAX_INDEX. This parameter will be in the range [0:7].
*/
uint8_t MRSubG_GetPALevelMaxIndex(void)
{
  return READ_REG_FIELD(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_LEVEL_MAX_INDEX);
}

/**
* @brief  Sets a specific PA_LEVEL_MAX_INDEX.
* @param  cIndex PA_LEVEL_MAX_INDEX to set.
* @retval None
*/
void MRSubG_SetPALevelMaxIndex(uint8_t cIndex)
{
  MODIFY_REG_FIELD(MR_SUBG_GLOB_STATIC->PA_CONFIG, MR_SUBG_GLOB_STATIC_PA_CONFIG_PA_LEVEL_MAX_INDEX, cIndex);
}

/**
 * @brief  Set the Almost Empty threshold for the Databuffer. When the number of elements in Databuffer reaches this value an interrupt can can be generated.
 * @param  cThrTx: almost empty threshold.
 *         This parameter is an uint8_t.
 * @retval None.
 */
void MRSubG_SetAlmostEmptyThresholdTx(uint8_t cThrTx){  
  /* Writes the Almost Empty threshold for Tx in the corresponding register */
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->DATABUFFER_THR, MR_SUBG_GLOB_DYNAMIC_DATABUFFER_THR_TX_ALMOST_EMPTY_THR, cThrTx);
}

/**
 * @brief  Set the almost full threshold for the Databuffer. When the number of elements in Databuffer reaches this value an interrupt can be generated.
 * @param  cThrRx almost full threshold.
 * @retval None.
 */
void MRSubG_SetAlmostFullThresholdRx(uint8_t cThrRx){  
  /* Writes the Almost Full threshold for Tx in the corresponding register */
  MODIFY_REG_FIELD(MR_SUBG_GLOB_DYNAMIC->DATABUFFER_THR, MR_SUBG_GLOB_DYNAMIC_DATABUFFER_THR_RX_ALMOST_FULL_THR, cThrRx);
}

/**
 * @brief  Returns the number of bytes after each TX/RX transaction.
 * @param  None
 * @retval The number of bytes after each TX/RX transaction.
 */
uint32_t MRSubG_GetBytesOfTransaction(void){
  uint16_t used = READ_REG_FIELD(MR_SUBG_GLOB_STATUS->DATABUFFER_INFO, MR_SUBG_GLOB_STATUS_DATABUFFER_INFO_NB_DATABUFFER_USED);
  uint16_t size = READ_REG(MR_SUBG_GLOB_STATIC->DATABUFFER_SIZE) & MR_SUBG_GLOB_STATIC_DATABUFFER_SIZE_DATABUFFER_SIZE;
  uint16_t count = READ_REG_FIELD(MR_SUBG_GLOB_STATUS->DATABUFFER_INFO, MR_SUBG_GLOB_STATUS_DATABUFFER_INFO_CURRENT_DATABUFFER_COUNT);

  return (used*size)+count;
}

/**
 * @brief  Return the RADIO FSM State.
 * @param  None
 * @retval Value of RADIO_FSM_STATE register
 */
MRSubGFSMState MRSubG_GetRadioFSMState(void){
  return (MRSubGFSMState)(READ_REG_FIELD(MR_SUBG_GLOB_STATUS->RADIO_FSM_INFO, MR_SUBG_GLOB_STATUS_RADIO_FSM_INFO_RADIO_FSM_STATE));
}

/**
 * @brief  Evaluate a sleep allowance signal.
 * The goal is to inform the power controller of the SoC if the MR_SubG IP 
 * agrees to have a low power mode activated on the SoC or not.
 * @param  None
 * @retval The allowance signal.
 * 	This value is type of @ref PowerSaveLevels.
 */
PowerSaveLevels RADIO_STACK_SleepCheck(void){
  PowerSaveLevels retLevel = POWER_SAVE_LEVEL_RUNNING;

  if (MRSubG_GetRadioFSMState() == STATE_IDLE)
    retLevel = POWER_SAVE_LEVEL_STOP_NOTIMER;
    
  return retLevel;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
