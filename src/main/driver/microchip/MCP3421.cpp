/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */

/* ****************************************************************************************
 * Include
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
#include "driver/microchip/MCP3421.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::microchip::MCP3421;
using hal::serial::SerialBus;
using mcuf::lang::Memory;
using hal::serial::SerialBusStatus;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new MCP3421 object
 * 
 * @param serialBus 
 */
MCP3421::MCP3421(hal::serial::SerialBus& serialBus, uint16_t address) : 
mSerialBus(serialBus),
mByteBuffer(Memory(&this->mBuffer, sizeof(this->mBuffer))){
  
  this->mAddress = (0x68 | (address & 0x0007));
  this->mResult = 0;
  this->mConfig = 0;
  this->resolution(Resolution::BIT_18);
  this->conversionMode(ConversionMode::CONTINUOUS_MODE);
  this->gain(Gain::GAIN_1);
  
  return;
}

/**
 * @brief Destroy the MCP3421 object
 * 
 */
MCP3421::~MCP3421(void){
  return;
}
/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - hal::analog::AnalogInputPin
 */
/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t MCP3421::convert(void){
  return this->mResult;
}
/**
 * @brief Get the adc convert level.
 * 
 * @return uint32_t 
 */
uint32_t MCP3421::getConvertLevel(void){
  Resolution r = this->resolution();
  switch(r){
    case Resolution::BIT_12:
      return 4096;
    
    case Resolution::BIT_14:
      return 16384;
    
    case Resolution::BIT_16:
      return 65536;
    
    case Resolution::BIT_18:
      return 262144;
  }
}
/* ****************************************************************************************
 * Public Method <Override> - hal::serial::SerialBusEvent
 */

/**
 * @brief 
 * 
 * @param status handle status
 * @param result 0 = successful, other = remaining byte count.
 * @param attachment user data
 */
void MCP3421::onSerialBusEvent(hal::serial::SerialBusStatus status, int result, void* attachment){
  if(this->mBusy == 2){
    this->mConfig = this->mBuffer[3];
    Resolution r = this->resolution();
    uint32_t cache = 0;
   
    switch(r){
      //-----------------------------------------------------------------------------------
      case Resolution::BIT_12:
        cache |= static_cast<uint32_t>(this->mBuffer[1]);
        cache |= (static_cast<uint32_t>(this->mBuffer[0] & 0x07) << 8);
        if(this->mBuffer[0] & 0x08)
          cache = 0x000007FF - cache;
        
        else
          cache += 0x000007FF;
        
        break;
      
      //-----------------------------------------------------------------------------------
      case Resolution::BIT_14:
        cache |= static_cast<uint32_t>(this->mBuffer[1]);
        cache |= (static_cast<uint32_t>(this->mBuffer[0] & 0x1F) << 8);
        if(this->mBuffer[0] & 0x20)
          cache = 0x00001FFF - cache;
        
        else
          cache += 0x00001FFF;
        
        break;
      
      //-----------------------------------------------------------------------------------
      case Resolution::BIT_16:
        cache |= static_cast<uint32_t>(this->mBuffer[1]);
        cache |= (static_cast<uint32_t>(this->mBuffer[0] & 0x7F) << 8);
        if(this->mBuffer[0] & 0x80)
          cache = 0x00007FFF - cache;
        
        else
          cache += 0x00007FFF;
        
        break;
      
      //-----------------------------------------------------------------------------------
      case Resolution::BIT_18:
        cache |= static_cast<uint32_t>(this->mBuffer[2]);
        cache |= (static_cast<uint32_t>(this->mBuffer[1]) << 8);
        cache |= (static_cast<uint32_t>((this->mBuffer[0] & 0x01)) << 16);
      
        if(this->mBuffer[0] & 0x02)
          cache = 0x0001FFFF - cache;
        
        else
          cache += 0x0001FFFF;
        
        break;
    }
    
    this->mResult = cache;
  }
  
  this->mBusy = 0;
  return;
}

/* ****************************************************************************************
 * Public Method
 */
/**
 * @brief 
 * 
 * @param value 
 * @return Resolution 
 */
MCP3421::Resolution MCP3421::resolution(Resolution value){
  this->mConfig &= ~(0x03 << 2);
  this->mConfig |= (static_cast<uint8_t>(value) << 2);
  return this->resolution();
}

/**
 * @brief 
 * 
 * @return Resolution 
 */
MCP3421::Resolution MCP3421::resolution(void){
  uint8_t value = ((this->mConfig >> 2) & 0x03);
  return static_cast<Resolution>(value);
}

/**
 * @brief 
 * 
 * @param value 
 * @return Gain 
 */
MCP3421::Gain MCP3421::gain(Gain value){
  this->mConfig &= ~(0x03 << 0);
  this->mConfig |= (static_cast<uint8_t>(value) << 0);
  return this->gain();
}

/**
 * @brief 
 * 
 * @return Gain 
 */
MCP3421::Gain MCP3421::gain(void){
  uint8_t value = ((this->mConfig >> 0) & 0x03);
  return static_cast<Gain>(value);
}

/**
 * @brief 
 * 
 * @param value 
 * @return ConversionMode 
 */
MCP3421::ConversionMode MCP3421::conversionMode(ConversionMode value){
  if(value == ConversionMode::ONE_SHOT_MODE)
    this->mConfig &= (1 << 4);
  
  else
    this->mConfig |= (1 << 4);
  
  return conversionMode();
}

/**
 * @brief 
 * 
 * @return ConversionMode 
 */
MCP3421::ConversionMode MCP3421::conversionMode(void){
  if(this->mConfig &= (1 << 4)) // is bit 1
    return ConversionMode::CONTINUOUS_MODE;
  
  else //is bit 0
    return ConversionMode::ONE_SHOT_MODE;
}

/**
 * @brief 
 * 
 * @return Status 
 */
MCP3421::Status MCP3421::status(void){
  if(this->mConfig &= (1 << 7)) //is bit 1
    return Status::CONVERT;
  
  else //is bit 0
    return Status::READY;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool MCP3421::update(void){
  if(this->isBusy())
    return false;
  
  this->mBusy = 2;
  this->mByteBuffer.flush();
  
  bool result = this->mSerialBus.read(this->mAddress, this->mByteBuffer, this, this);
  if(result == false)
    this->mBusy = false;
  
  return result;  
}

    
/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool MCP3421::startConvert(void){
  if(this->isBusy())
    return false;
  
  this->mBusy = 1;
  
  this->mByteBuffer.flush();
  this->mByteBuffer.putByte((this->mConfig | 0x80));
  this->mByteBuffer.flip();
  
  bool result = this->mSerialBus.write(this->mAddress, this->mByteBuffer, this, this);
  if(result == false)
    this->mBusy = false;
  
  return result;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool MCP3421::isBusy(void){
  return this->mBusy;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool MCP3421::writeOption(void){
  if(this->isBusy())
    return false;
  
  this->mBusy = 1;
  
  this->mByteBuffer.flush();
  this->mByteBuffer.putByte((this->mConfig & 0x7F));
  this->mByteBuffer.flip();
  
  bool result = this->mSerialBus.write(this->mAddress, this->mByteBuffer, this, this);
  if(result == false)
    this->mBusy = false;
  
  return result;
}

/* ****************************************************************************************
 * Protected Method <Static>
 */

/* ****************************************************************************************
 * Protected Method <Override>
 */

/* ****************************************************************************************
 * Protected Method
 */

/* ****************************************************************************************
 * Private Method
 */

/* ****************************************************************************************
 * End of file
 */
