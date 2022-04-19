
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
using driver::wirelesstag::internal::WT32ETH01Receiver;
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

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new WT32ETH01 object
 * 
 */
WT32ETH01::WT32ETH01(SerialPort& serialPort, GeneralPin& enablePin) : 
mSerialPort(serialPort),
mEnablePin(enablePin),
mSerialPortInputStream(serialPort),
mSerialPortOutputStream(serialPort),
mReceiver(mSerialPortInputStream, *this){
  
  this->mConnectStatus = ConnectStatus::NO_CONNECT;
  this->mReceiverStatus = ReceiverStatus::WAIT_EMABLE;
  this->mStatus = Status::NOT_INIT;
  return;
}

/**
 * @brief Destroy the WT32ETH01 object
 * 
 */
WT32ETH01::~WT32ETH01(void){
  this->deinit();
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
 * @return int 
 */
int WT32ETH01::avariable(void){
  return this->mReceiver.avariable();
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::abortRead(void){
  return this->mReceiver.abortRead();
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01::readBusy(void){
  return this->mReceiver.readBusy();
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @return int 
 */
bool WT32ETH01::read(mcuf::io::ByteBuffer& byteBuffer){
  return this->mReceiver.read(byteBuffer);
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
  return this->mReceiver.read(byteBuffer, attachment, handler);
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
  return this->mReceiver.read(byteBuffer, future);
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
  return this->mReceiver.skip(value, attachment, handler);
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
  return this->mReceiver.skip(value, future);
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
 * @param t 
 */
void WT32ETH01::accept(WT32ETH01Receiver::Event t){
  
  switch(t){
    case WT32ETH01Receiver::Event::MODULE_OK:
      mcuf::lang::System::out().print("MODULE_OK\n");
      break;
    case WT32ETH01Receiver::Event::OK:
      mcuf::lang::System::out().print("OK\n");
      break;
    case WT32ETH01Receiver::Event::ERROR:
      mcuf::lang::System::out().print("ERROR\n");
      break;
    case WT32ETH01Receiver::Event::NO_IP:
      mcuf::lang::System::out().print("NO_IP\n");
      break;
    case WT32ETH01Receiver::Event::ON_SEND:
      mcuf::lang::System::out().print("ON_SEND\n");
      break;
    case WT32ETH01Receiver::Event::SEND_OK:
      mcuf::lang::System::out().print("SEND_OK\n");
      break;
  }

}

/* ****************************************************************************************
 * Public Method <Override> - hal::Base
 */

/**
 * @brief uninitialze hardware.
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::deinit(void){
  if(!this->isInit())
    return false;
  
  this->mSerialPort.deinit();
  this->mStatus = Status::NOT_INIT;
  
  return true;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::init(void){
  if(this->isInit())
    return false;
  
  this->mEnablePin.setOutput();
  this->mEnablePin.setLow();
  
  this->mSerialPort.init();
  
  this->mSerialPort.baudrate(115200);
  
  this->mSerialPort.clear();
  this->mReceiver.start();
  this->mEnablePin.setHigh();
  
  return true;
}

/**
 * @brief get hardware initialzed status.
 * 
 * @return true not init
 * @return false initd
 */
bool WT32ETH01::isInit(void){
  return (this->mStatus == Status::INITD);
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
  return SocketAddress(this->mReceiver.mInternetProtocolAddress, 0);
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
  return this->mReceiver.mMediaAccessControlAddress;
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
bool WT32ETH01::connect(ConnectType type, const SocketAddress& remoteAddress, Future& future){
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
bool WT32ETH01::listen(ConnectType type, const SocketAddress& remoteAddress, uint16_t destPort, Future& future){
  return false;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::updateStatus(void){
  Future future = Future();
  bool result;
  result = this->updateStatus(future);
  future.waitDone();
  return result;
}

/**
 * @brief 
 * 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::updateStatus(mcuf::io::Future& future){
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
