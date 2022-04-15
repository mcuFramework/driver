
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
#include "driver/wirelesstag/WT32ETH01.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::wirelesstag::WT32ETH01;
using hal::serial::SerialPort;
using hal::serial::SerialPortStatus;
using hal::general::GeneralPin;
using mcuf::io::ByteBuffer;
using mcuf::io::CompletionHandler;
using mcuf::io::Future;
using mcuf::io::RingBuffer;
using mcuf::net::MediaAccessControlAddress;
using mcuf::net::SocketAddress;



/* ****************************************************************************************
 * Variable <Static>
 */
const char* const WT32ETH01::TEXT_MODULE_OK = "module ok\r\n\0";
const char* const WT32ETH01::TEXT_RETURN_OK = "\r\nOK\r\n\0";
const char* const WT32ETH01::TEXT_RETURN_ERROR = "\r\nERROR\r\n\0";
const char* const WT32ETH01::TEXT_RETURN_ATE0 = "ATE0\r\n\r\nOK\r\n\0";
const char* const WT32ETH01::TEXT_RETURN_ADDRESS_FORMAT = "ATE0\r\n\r\nOK\r\n\0";
const char* const WT32ETH01::TEXT_CMD_ATE_DISABLE = "ATE0\r\n\0";
const char* const WT32ETH01::TEXT_CMD_GET_ADDRESS = "AT+CIFSR\r\n\0";


/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new WT32ETH01 object
 * 
 */
WT32ETH01::WT32ETH01(SerialPort& serialPort, GeneralPin& enablePin, uint32_t bufferSize) : RingBuffer(bufferSize){
  return;
}

/**
 * @brief Destroy the WT32ETH01 object
 * 
 */
WT32ETH01::~WT32ETH01(void){
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::InputStream
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::abortRead(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01::readBusy(void){
  return false;
}

/**
 * @brief nonblocking
 * 
 * @param byteBuffer 
 * @param attachment 
 * @param handler 
 * @return true successful.
 * @return false fail.
 */
bool WT32ETH01::read(ByteBuffer& byteBuffer, void* attachment, CompletionHandler<int, void*>* handler){
  return false;
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::read(ByteBuffer& byteBuffer, Future& future){
  if(!future.isIdle())
    return false;
  
  future.setWait();
  bool result = this->read(byteBuffer, nullptr, &future);
  
  if(!result)
    future.clear();
  
  return result; 
}
/**
 * @brief 
 * 
 * @param value 
 * @param attachment 
 * @param handler 
 * @return true 
 * @return false 
 */
bool WT32ETH01::skip(int value, void* attachment, CompletionHandler<int, void*>* handler){
  return false;
}
/**
 * @brief 
 * 
 * @param value 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::skip(int value, Future& future){
  if(!future.isIdle())
    return false;
  
  future.setWait();
  bool result = this->skip(value, nullptr, &future);
  
  if(!result)
    future.clear();
  
  return result; 
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::OutputStream
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::abortWrite(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01::writeBusy(void){
  return false;
}
/**
 * @brief 
 * 
 * @param byteBuffer 
 * @param attachment 
 * @param handler 
 * @return true successful.
 * @return false fail.
 */
bool WT32ETH01::write(ByteBuffer& byteBuffer, void* attachment, CompletionHandler<int, void*>* handler){
  return false;
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::write(ByteBuffer& byteBuffer, Future& future){
  if(!future.isIdle())
    return false;
  
  future.setWait();
  bool result = this->write(byteBuffer, nullptr, &future);
  
  if(!result)
    future.clear();
  
  return result; 
}

/* ****************************************************************************************
 * Public Method <Override> - hal::serial::SerialPortEvent
 */
/**
 * @brief 
 * 
 * @param status handle status
 * @param result 0 = successful, other = remaining byte count.
 * @param attachment user data
 */
void WT32ETH01::onSerialPortEvent(SerialPortStatus status, int result, void* attachment){

}

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief Get the Local Address object
 * 
 * @return mcuf::net::SocketAddress 
 */
SocketAddress WT32ETH01::getLocalAddress(void){
  uint8_t cache[4] = {0,0,0,0};
  return SocketAddress(cache, 0);
}

/**
 * @brief Get the Remode Address object
 * 
 * @return mcuf::net::SocketAddress 
 */
SocketAddress WT32ETH01::getRemodeAddress(void){
  uint8_t cache[4] = {0,0,0,0};
  return SocketAddress(cache, 0);
}

/**
 * @brief Get the Mac Address object
 * 
 * @return mcuf::net::MediaAccessControlAddress 
 */
MediaAccessControlAddress WT32ETH01::getMacAddress(void){
  uint8_t cache[6] = {0,0,0,0,0,0};
  return MediaAccessControlAddress(cache);
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::isConnect(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::disconnect(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::init(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::reset(void){
  return false;
}

/**
 * @brief 
 * 
 * @param type 
 * @param remoteAddress 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::connect(ConnectType type, SocketAddress remoteAddress, Future* future){
  return false;
}

/**
 * @brief 
 * 
 * @param type 
 * @param remoteAddress 
 * @param destPort 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::listen(ConnectType type, SocketAddress remoteAddress, uint16_t destPort, Future* future){
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

/* ****************************************************************************************
 * End of file
 */
