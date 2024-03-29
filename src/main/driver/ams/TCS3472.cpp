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
#include "mcuf.h"

//-----------------------------------------------------------------------------------------
#include "driver/ams/TCS3472.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

using driver::ams::TCS3472;

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using hal::serial::SerialBus;
using hal::serial::SerialBusStatus;
using mcuf::lang::Memory;


/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

/**
 *
 */
TCS3472::TCS3472(Type type, SerialBus& serailBus) : mSerailBus(serailBus),
  mByteBufferTransfer(Memory(this->mWriteBuffer, sizeof(this->mWriteBuffer))),
  mByteBufferReceiver(Memory(&this->mRegister, sizeof(mRegister))){
  this->mAddress = 0x29;
  this->mBusy = false;
}

/**
 *
 */
TCS3472::~TCS3472(void){
  this->disable();
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override>
 */

void TCS3472::onSerialBusEvent(SerialBusStatus status, int result, void* attachment){
  this->mBusy = false;
}

/* **************************************************************************************
 * Public Method <Override> - mcuf::util::Updater
 */
/**
 * @brief 更新資源
 * 
 * @return true 開始嘗試更新成功
 * @return false 開始嘗試更新失敗
 */
bool TCS3472::update(void){
  if(this->mBusy)
    return false;
  
  this->mBusy = true;
  
  this->mByteBufferTransfer.flush();
  this->mByteBufferTransfer.putByte(this->getCommand(Address::ADDR_ENABLE));
  this->mByteBufferTransfer.flip();
  this->mByteBufferReceiver.flush();
  
  if(this->mSerailBus.transfer(this->mAddress, this->mByteBufferTransfer, this->mByteBufferReceiver, this, this))
    return true;
  
  this->mBusy = false;
  return false;
}

/**
 * @brief 是否正在更新
 * 
 * @return true 正在更新中
 * @return false 等待更新
 */
bool TCS3472::isUpdating(void){
  return this->mBusy;
}

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief TCS3472啟用
 * 
 * @return true 啟用成功
 * @return false 啟用失敗
 */
bool TCS3472::enable(void){
  return this->writeRegister(Address::ADDR_ENABLE, (0x01 | 0x02));
}

/**
 * @brief TCS3472停用
 * 
 * @return true 停用成功
 * @return false 停用失敗
 */
bool TCS3472::disable(void){
  return this->writeRegister(Address::ADDR_ENABLE, 0);
}

/**
 * @brief 
 * 
 * @param cycle 
 * @return true 
 * @return false 
 */
bool TCS3472::setIntegrationCycle(uint8_t cycle){
  if(cycle)
    cycle -= 1;
  
  cycle = 0xFF - cycle;
  return this->writeRegister(Address::ADDR_ENABLE, cycle);
}

/**
 * @brief 
 * 
 * @param addr 
 * @param data 
 * @return true 
 * @return false 
 */
bool TCS3472::writeRegister(Address addr, uint8_t data){
  if(this->mBusy)
    return false;
  
  this->mBusy = true;

  this->mByteBufferTransfer.flush();
  this->mByteBufferTransfer.putByte(this->getCommand(addr));
  this->mByteBufferTransfer.putByte(data);
  this->mByteBufferTransfer.flip();
  
  if(this->mSerailBus.write(this->mAddress, this->mByteBufferTransfer, this, this))
    return true;
  
  this->mBusy = false;
  return false;  
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

/**
 * @brief 
 * 
 * @param addr 
 * @return uint8_t 
 */
uint8_t TCS3472::getCommand(Address addr){
  return (static_cast<uint8_t>(addr) | 0x80);
}

/* ****************************************************************************************
 * End of file
 */
