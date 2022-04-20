
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
using driver::wirelesstag::internal::WT32ETH01Transfer;
using hal::serial::SerialPort;
using hal::serial::SerialPortStatus;
using hal::general::GeneralPin;
using mcuf::io::ByteBuffer;
using mcuf::io::CompletionHandler;
using mcuf::io::Future;
using mcuf::io::RingBuffer;
using mcuf::net::MediaAccessControlAddress;
using mcuf::net::SocketAddress;
using mcuf::net::InternetProtocolAddress;



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
mReceiver(mSerialPortInputStream, *this),
mTransfer(mSerialPortOutputStream, *this),
mRemoteAddress(){
  
  this->mIp = 0;
  this->mConnectStatus = ConnectStatus::NO_CONNECT;
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
  if(this->mStatus != Status::TRANSFER)
    return 0;
  
  return this->mReceiver.avariable();
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::abortRead(void){
  if(this->mStatus != Status::TRANSFER)
    return false;  
  
  return this->mReceiver.abortRead();
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01::readBusy(void){
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mReceiver.readBusy();
}

/**
 * @brief pop buffer byte non blocking.
 * 
 * @param result 
 * @return true has data in buffer.
 * @return false no data in buffer.
 */
bool WT32ETH01::getByte(char& result){
  return this->mReceiver.getByte(result);
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @return int 
 */
int WT32ETH01::get(mcuf::io::ByteBuffer& byteBuffer){
  return this->mReceiver.get(byteBuffer);
}

/**
 * @brief 
 * 
 * @param buffer 
 * @param bufferSize 
 * @return int 
 */
int WT32ETH01::get(void* buffer, int bufferSize){
  return this->mReceiver.get(buffer, bufferSize);
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
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.abortWrite();
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01::writeBusy(void){
  if(this->mStatus != Status::TRANSFER)
    return true;
  
  return this->mTransfer.writeBusy();
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
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.write(byteBuffer, attachment, handler);
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
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.write(byteBuffer, future);
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
 * Public Method <Override> - hal::serial::SerialPortEvent
 */

/**
 * @brief 
 * 
 * @param t 
 */
void WT32ETH01::accept(WT32ETH01Transfer::Event t){
  
  switch(t){
    case WT32ETH01Transfer::Event::WRITE_SUCCESSFUL:
      mcuf::lang::System::out().print("WRITE_SUCCESSFUL\n");
      break;
    case WT32ETH01Transfer::Event::WRITE_FAIL:
      mcuf::lang::System::out().print("WRITE_FAIL\n");
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
  this->mTransfer.start();
  this->mEnablePin.setHigh();
  
  this->delay(1000);
  
  this->mTransfer.setNonAck();
  this->delay(1000);
  uint8_t ip[4] = {192, 168, 11, 169};
  uint8_t gateway[4] = {192, 168, 11, 254};
  uint8_t mask[4] = {255, 255, 255, 0};
  this->mTransfer.setStaticAddress(ip, gateway, mask);
  this->delay(1000);
  this->mTransfer.getAddress();
  this->delay(1000);
  this->mTransfer.setConnect(WT32ETH01Transfer::ConnectType::TCP_SERVER, ip, 1234, 6789);
  this->delay(1000);
  
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
  if(this->mStatus != Status::INITD)
    return false;
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
  if(this->mStatus != Status::INITD)
    return false;
  return false;
}

/**
 * @brief Set the Static I P Address object
 * 
 * @param ip 
 * @param gateway 
 * @param mask 
 * @return true 
 * @return false 
 */
bool WT32ETH01::setStaticIPAddress(const InternetProtocolAddress& ip,
                                   const InternetProtocolAddress& gateway,
                                   const InternetProtocolAddress& mask){
                                     
  this->mIp = ip.getAddress();
  this->mGateway = gateway.getAddress();
  this->mMask = mask.getAddress();
  return true;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::setDHCP(void){
  this->mIp = 0;
  this->mGateway = 0;
  this->mMask = 0;
  return true;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::updateStatus(void){
  if(this->mStatus != Status::INITD)
    return false;
  
  return this->mTransfer.getAddress();
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
