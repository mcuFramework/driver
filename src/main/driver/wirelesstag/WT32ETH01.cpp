
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
//#define WT32ETH01_DEBUG
/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::wirelesstag::WT32ETH01;
using driver::wirelesstag::internal::WT32ETH01Receiver;
using driver::wirelesstag::internal::WT32ETH01Transfer;
using hal::serial::SerialPort;
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
mReceiver(*this),
mTransfer(serialPort, *this),
mRemoteAddress(){
  
  this->mIp = 0;
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
  this->mSerialPort.init();
  this->mSerialPort.baudrate(115200);  
  
  this->reset();
  return true;
}

/**
 * @brief get hardware initialzed status.
 * 
 * @return true not init
 * @return false initd
 */
bool WT32ETH01::isInit(void){
  return (this->mStatus != Status::NOT_INIT);
}

/* **************************************************************************************
 * Public Method <Override> - mcuf::io::InputStreamBuffer
 */
/**
 * @brief Get the Output Buffer object
 * 
 * @return mcuf::io::OutputBuffer& 
 */
mcuf::io::OutputBuffer& WT32ETH01::getOutputBuffer(void){
  return *this;
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::OutputBuffer
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::isEmpty(void) const {
  return this->mReceiver.isEmpty();
}

/**
 * @brief 
 * 
 * @return int 
 */
int WT32ETH01::avariable(void) const{
  if(this->mStatus != Status::TRANSFER)
    return 0;
  
  return this->mReceiver.avariable();
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
int WT32ETH01::get(mcuf::io::InputBuffer& byteBuffer){
  return this->mReceiver.get(byteBuffer);
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @param length
 * @return int 
 */
int WT32ETH01::get(mcuf::io::InputBuffer& inputBuffer, int length){
  return this->mReceiver.get(inputBuffer, length);
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
 * @param value 
 * @return int 
 */
int WT32ETH01::skip(int value){
  return this->mReceiver.skip(value);
}

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
 * @brief 
 * 
 * @param inputBuffer 
 * @param timeout 
 * @return int 
 */
bool WT32ETH01::read(mcuf::io::InputBuffer& inputBuffer, int timeout){
  return this->mReceiver.read(inputBuffer, timeout);
}   

/**
 * @brief nonblocking
 * 
 * @param inputBuffer 
 * @param attachment 
 * @param handler 
 * @return true successful.
 * @return false fail.
 */
bool WT32ETH01::read(mcuf::io::InputBuffer& inputBuffer, void* attachment, CompletionHandler<int, void*>* handler){
  return this->mReceiver.read(inputBuffer, attachment, handler);
}

/**
 * @brief 
 * 
 * @param inputBuffer 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::read(mcuf::io::InputBuffer& inputBuffer, Future& future){
  return this->mReceiver.read(inputBuffer, future);
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
 * @param outputBuffer
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::write(mcuf::io::OutputBuffer& outputBuffer, int timeout){
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.write(outputBuffer, timeout);
}

/**
 * @brief 
 * 
 * @param outputBuffer 
 * @param attachment 
 * @param handler 
 * @return true successful.
 * @return false fail.
 */
bool WT32ETH01::write(mcuf::io::OutputBuffer& outputBuffer, void* attachment, CompletionHandler<int, void*>* handler){
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.write(outputBuffer, attachment, handler);
}

/**
 * @brief 
 * 
 * @param outputBuffer 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::write(mcuf::io::OutputBuffer& outputBuffer, Future& future){
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  return this->mTransfer.write(outputBuffer, future);
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::function::Consumer<internal::WT32ETH01Receiver::Event>
 */

/**
 * @brief 
 * 
 * @param t 
 */
void WT32ETH01::accept(WT32ETH01Receiver::Event t){
  switch(t){
    case WT32ETH01Receiver::Event::MODULE_OK:
      this->executeHandle(false);
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:MODULE_OK\n");
      #endif
      this->mStatus = Status::WAIT_INIT;
      this->mTransfer.setNonAck();
      break;
    
    case WT32ETH01Receiver::Event::OK:
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:OK\n");
      #endif
      this->eventOk();
      break;
    
    case WT32ETH01Receiver::Event::ERROR:
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:ERROR\n");
      #endif
      this->reset();
      break;
    
    case WT32ETH01Receiver::Event::NO_IP:
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:NO_IP\n");
      #endif
      this->mStatus = Status::INITD;
      this->executeHandle(false);
      break;
    
    case WT32ETH01Receiver::Event::ON_SEND:
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:ON_SEND\n");
      #endif
      this->mTransfer.setOnSendFlag();
      break;
    
    case WT32ETH01Receiver::Event::SEND_OK:
      #ifdef WT32ETH01_DEBUG
      System::out().print("r_event:SEND_OK\n");
      #endif
      this->mTransfer.setSendOkFlag();
      break;
  }

}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::function::Consumer<internal::WT32ETH01Transfer::Event>
 */

/**
 * @brief 
 * 
 * @param t 
 */
void WT32ETH01::accept(WT32ETH01Transfer::Event t){
  switch(t){
    case WT32ETH01Transfer::Event::WRITE_SUCCESSFUL:
      #ifdef WT32ETH01_DEBUG
      System::out().print("t_event:WRITE_SUCCESSFUL\n");
      #endif
      break;
    case WT32ETH01Transfer::Event::WRITE_FAIL:
      #ifdef WT32ETH01_DEBUG
      System::out().print("t_event:WRITE_FAIL\n");
      #endif
      this->reset();
      break;
  }
}

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief
 *
 */
void WT32ETH01::execute(void){
  this->mReceiver.execute(this->mSerialPort.getOutputBuffer());
}

/**
 * @brief 
 * 
 * @return mcuf::net::InternetProtocolAddress 
 */
InternetProtocolAddress WT32ETH01::getIP(void){
  return this->mReceiver.mInternetProtocolAddress;
}

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
  return this->mRemoteAddress;
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
  return (this->mStatus == Status::TRANSFER);
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::disconnect(void){
  if(this->mStatus != Status::TRANSFER)
    return false;
  
  this->reset();
  return true;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01::reset(void){
  this->mStatus = Status::WAIT_INIT;
  this->mEnablePin.setLow();
  this->mSerialPort.getOutputBuffer().skip(this->mSerialPort.getOutputBuffer().avariable());
  this->mReceiver.reset();
  this->mTransfer.start();
  this->delay(10);
  this->mEnablePin.setHigh();
  return true;
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
  
  this->mStatus = Status::HANDLE_CONNECTC_UPDATE_IP;
  if(this->setFuture(future) == false){
    this->mStatus = Status::INITD;
    return false;
  }
  
  this->mRemoteAddress.setAddress(remoteAddress.getAddress());
  this->mRemoteAddress.setPort(remoteAddress.getPort());
  
  if(type == ConnectType::TCP)
    this->mConnectType = WT32ETH01Transfer::ConnectType::TCP_CLIENT;
  
  else
    this->mConnectType = WT32ETH01Transfer::ConnectType::UDP_CLIENT;  
  
  bool result = this->mTransfer.getAddress();
  if(result == false)
    this->mStatus = Status::INITD;
  
  return result;
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
bool WT32ETH01::listen(ConnectType type, const uint16_t port, mcuf::io::Future& future){
  if(this->mStatus != Status::INITD)
    return false;
  
  this->mStatus = Status::HANDLE_CONNECTS_UPDATE_IP;
  if(this->setFuture(future) == false){
    this->mStatus = Status::INITD;
    return false;
  }
  
  this->mRemoteAddress.setPort(port);
  
  if(type == ConnectType::TCP)
    this->mConnectType = WT32ETH01Transfer::ConnectType::TCP_SERVER;
  
  else
    this->mConnectType = WT32ETH01Transfer::ConnectType::UDP_SERVER;
  
  bool result = this->mTransfer.getAddress();
  if(result == false)
    this->mStatus = Status::INITD;
  
  return result;
}

/**
 * @brief Set the Static IP Address object
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
  
  bool result = this->mTransfer.getAddress();
  if(result == false)
    this->mStatus = Status::INITD;
  
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

/**
 * @brief Set the Future object
 * 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01::setFuture(mcuf::io::Future& future){
  if(future.setWait() == false)
    return false;
  
  this->mFuture = &future;
  return true;
}

/**
 * @brief 
 * 
 * @param successful 
 */
void WT32ETH01::executeHandle(bool successful){
  Future* future = this->mFuture;
  this->mFuture = nullptr;

  if(future == nullptr)
    return;

  if(successful)
    future->completed(0, nullptr);
  
  else
    future->failed(nullptr, nullptr);

  return;
}

/**
 * @brief 
 * 
 */    
void WT32ETH01::eventOk(void){
  switch(this->mStatus){
    /* --------------------------------------------------------------------------------- */
    case Status::NOT_INIT:
    case Status::TRANSFER:
    case Status::INITD:
      break;  
    
    /* --------------------------------------------------------------------------------- */
    case Status::WAIT_INIT:
      if(this->mIp == 0){
        this->mStatus = Status::INITD;
        break;
      }
      
      
      if(this->mTransfer.setStaticAddress(this->mIp, this->mGateway, this->mMask) == false)
        this->reset();
      
      this->mStatus = Status::WAIT_INIT_SET_ADDRESS;
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::WAIT_INIT_SET_ADDRESS:
      this->mStatus = Status::INITD;
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::HANDLE:
      this->mStatus = Status::INITD;
      this->executeHandle(true);
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::HANDLE_CONNECTS_UPDATE_IP:
      this->mStatus = Status::HANDLE_CONNECT;
      this->mRemoteAddress.setAddress(this->mReceiver.mInternetProtocolAddress.getAddress());
      if(this->mTransfer.setConnect(this->mConnectType, this->mRemoteAddress) == false){
        this->mStatus = Status::INITD;
        this->executeHandle(false);
      }
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::HANDLE_CONNECTC_UPDATE_IP:
      this->mStatus = Status::HANDLE_CONNECT;
      if(this->mTransfer.setConnect(this->mConnectType, this->mRemoteAddress) == false){
        this->mStatus = Status::INITD;
        this->executeHandle(false);
      }
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::HANDLE_CONNECT:
      this->mStatus = Status::TRANSFER;
      this->executeHandle(true);
      break;
  }
}

/* ****************************************************************************************
 * End of file
 */
