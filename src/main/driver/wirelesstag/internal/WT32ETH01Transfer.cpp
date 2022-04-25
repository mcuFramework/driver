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
#include "driver/wirelesstag/internal/WT32ETH01Transfer.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::wirelesstag::internal::WT32ETH01Transfer;
using mcuf::io::ByteBuffer;
using mcuf::io::CompletionHandler;
using mcuf::io::Future;
using mcuf::io::OutputStream;
using mcuf::net::InternetProtocolAddress;
using mcuf::function::Consumer;
using mcuf::lang::String;

/* ****************************************************************************************
 * Variable <Static>
 */
const char* const WT32ETH01Transfer::TEXT_COMMAND_ATE0 = "ATE0\r\n\0";
const char* const WT32ETH01Transfer::TEXT_COMMAND_GET_ADDRESS = "AT+CIFSR\r\n\0";
const char* const WT32ETH01Transfer::TEXT_COMMAND_STATIC_IP_FORMAT = "AT+CIPETH_DEF=\"%s\",\"%s\",\"%s\"\r\n\0";
const char* const WT32ETH01Transfer::TEXT_COMMAND_CONNECT_FORMAT = "AT+CIPSTART=\"%s\",\"%s\",%d,%d\r\n\0";
const char* const WT32ETH01Transfer::TEXT_COMMAND_DATA_SEND_FORMAT = "AT+CIPSEND=%d\r\n\0";
const char* const WT32ETH01Transfer::TEXT_IPADDRESS_FORMAT = "%d.%d.%d.%d\0";
const char* const WT32ETH01Transfer::TEXT_PARAM_TCPS = "TCPS\0";
const char* const WT32ETH01Transfer::TEXT_PARAM_TCPC = "TCPC\0";
const char* const WT32ETH01Transfer::TEXT_PARAM_UDPS = "UDPS\0";
const char* const WT32ETH01Transfer::TEXT_PARAM_UDPC = "UDPC\0";


/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new WT32ETH01Transfer object
 * 
 */
WT32ETH01Transfer::WT32ETH01Transfer(OutputStream& outputStream, Consumer<Event>& event) : 
ByteBuffer(Memory(this->mBuffer, sizeof(this->mBuffer))),
mOutputStream(outputStream),
mEvent(event){
  this->mStatus = Status::IDLE;
  this->mByteBuffer = nullptr;
  return;
}

/**
 * @brief Destroy the WT32ETH01Transfer object
 * 
 */
WT32ETH01Transfer::~WT32ETH01Transfer(void){
  this->mOutputStream.abortWrite();
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::OutputStream
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::abortWrite(void){
  return false;
}

/**
 * @brief 
 * 
 * @return true is busy.
 * @return false isn't busy.
 */
bool WT32ETH01Transfer::writeBusy(void){
  return (this->mByteBuffer != nullptr);
}

/**
 * @brief 
 * 
 * @param outputBuffer
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::write(mcuf::io::OutputBuffer& outputBuffer, int timeout){
  Future future = Future();
  if(this->write(outputBuffer, future) == false)
    return false;
  
  future.waitDone(timeout);
  if(future.isDone() == false)
    this->abortWrite();
  
  future.waitDone();
  return true;
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
bool WT32ETH01Transfer::write(mcuf::io::OutputBuffer& byteBuffer, void* attachment, CompletionHandler<int, void*>* handler){
  if(this->writeBusy())
    return false;
  
  if(byteBuffer.avariable() == 0)
    return false;
  
  this->mByteBuffer = &byteBuffer;
  this->mAttachment = attachment;
  this->mHandler = handler;
  
  if(this->mStatus == Status::IDLE){
    bool result = this->setSendLen();
    if(result == false){
      this->executeHandler(false);
      return false;
    }
  }
  
  return true;
}

/**
 * @brief 
 * 
 * @param byteBuffer 
 * @param future 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::write(mcuf::io::OutputBuffer& byteBuffer, Future& future){
  if(!future.classAvariable())
    return false;
  
  if(!future.isIdle())
    return false;
  
  return this->write(byteBuffer, nullptr, &future);
}

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::CompletionHandler<int ,void*>
 */

/**
 * @brief 
 * 
 * @param result 
 * @param attachment 
 */
void WT32ETH01Transfer::completed(int result, void* attachment){
  switch(this->mStatus){
    /* --------------------------------------------------------------------------------- */
    case Status::IDLE:
    case Status::WAIT_BEGIN_FLAG:
    case Status::WAIT_SEND_OK_FLAG:
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::WRITE_COMMAND:
      this->mStatus = Status::IDLE;
      this->mEvent.accept(Event::WRITE_SUCCESSFUL);
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::WRITE_USER_DATA:
      this->mStatus = Status::WAIT_SEND_OK_FLAG;
      break;
    
    /* --------------------------------------------------------------------------------- */
    case Status::WRITE_USER_DATALEN:
      this->mStatus = Status::WAIT_BEGIN_FLAG;
      break;
    
  }
}

/**
 * @brief 
 * 
 * @param exc 
 * @param attachment 
 */
void WT32ETH01Transfer::failed(void* exc, void* attachment){
  this->mStatus = Status::IDLE;
  this->mEvent.accept(Event::WRITE_FAIL);
  return;
}

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::isCommandBusy(void){
  return (this->mStatus != Status::IDLE);
}

/**
 * @brief 
 * 
 */
void WT32ETH01Transfer::start(void){
  this->stop();
  return;
}

/**
 * @brief 
 * 
 */
void WT32ETH01Transfer::stop(void){
  this->mOutputStream.abortWrite();
  this->flush();
  this->mStatus = Status::IDLE;
  this->mByteBuffer = nullptr;
}

/**
 * @brief Set the Non Ack object
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setNonAck(void){
  if(this->mStatus != Status::IDLE)
    return false;

  this->mStatus = Status::WRITE_COMMAND;
  this->flush();
  this->put(WT32ETH01Transfer::TEXT_COMMAND_ATE0);
  this->flip();
  return this->directWrite(this);
}

/**
 * @brief Get the Address object
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::getAddress(void){
  if(this->mStatus != Status::IDLE)
    return false;

  this->mStatus = Status::WRITE_COMMAND;
  this->flush();
  this->put(WT32ETH01Transfer::TEXT_COMMAND_GET_ADDRESS);
  this->flip();
  return this->directWrite(this);
}

/**
 * @brief Set the Static Address object
 * 
 * @param ip 
 * @param gateway 
 * @param mask 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setStaticAddress(const uint8_t* ip, const uint8_t* gateway, const uint8_t* mask){
  if(this->mStatus != Status::IDLE)
    return false;
  
  this->mStatus = Status::WRITE_COMMAND;  
  
  char ipa[16];
  char gatewaya[16];
  char maska[16];
  
  String::format(ipa, 16, WT32ETH01Transfer::TEXT_IPADDRESS_FORMAT, ip[0], ip[1], ip[2], ip[3]);
  String::format(gatewaya, 16, WT32ETH01Transfer::TEXT_IPADDRESS_FORMAT, gateway[0], gateway[1], gateway[2], gateway[3]);
  String::format(maska, 16, WT32ETH01Transfer::TEXT_IPADDRESS_FORMAT, mask[0], mask[1], mask[2], mask[3]);
  
  this->flush();
  this->putFormat(WT32ETH01Transfer::TEXT_COMMAND_STATIC_IP_FORMAT, ipa, gatewaya, maska);
  this->flip();
  return this->directWrite(this);
}

/**
 * @brief Set the Static Address object
 * 
 * @param ip 
 * @param gateway 
 * @param mask 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setStaticAddress(const uint32_t& ip, const uint32_t& gateway, const uint32_t& mask){
  const uint8_t* cip = reinterpret_cast<const uint8_t*>(&ip);
  const uint8_t* cgateway = reinterpret_cast<const uint8_t*>(&gateway);
  const uint8_t* cmask = reinterpret_cast<const uint8_t*>(&mask);
  return this->setStaticAddress(cip, cgateway, cmask);
}

/**
 * @brief Set the Commnad object
 * 
 * @param type 
 * @param ip 
 * @param destPort 
 * @param localPort 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setConnect(ConnectType type, const mcuf::net::SocketAddress& socketAddress){
  if(this->mStatus != Status::IDLE)
    return false;
  
  this->mStatus = Status::WRITE_COMMAND;
  uint8_t ip[4];
  uint16_t port = socketAddress.getPort();
  socketAddress.getAddress(ip);

  char ipa[16];
  String::format(ipa, 16, WT32ETH01Transfer::TEXT_IPADDRESS_FORMAT, ip[0], ip[1], ip[2], ip[3]);
  this->flush();
  
  const char* mode;
  switch(type){
    case ConnectType::TCP_CLIENT:
      mode = WT32ETH01Transfer::TEXT_PARAM_TCPC;
      break;
    
    case ConnectType::TCP_SERVER:
      mode = WT32ETH01Transfer::TEXT_PARAM_TCPS;
      break;
    
    case ConnectType::UDP_CLIENT:
      mode = WT32ETH01Transfer::TEXT_PARAM_UDPC;
      break;
    
    case ConnectType::UDP_SERVER:
      mode = WT32ETH01Transfer::TEXT_PARAM_UDPS;
      break;    
  }
  
  this->putFormat(WT32ETH01Transfer::TEXT_COMMAND_CONNECT_FORMAT, mode, ipa, port, port);
  this->flip();
  
  return this->directWrite(this);
}

/**
 * @brief Set the On Send Flag object
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setOnSendFlag(void){
  if(this->mStatus != Status::WAIT_BEGIN_FLAG)
    return false;
  
  this->mStatus = Status::WRITE_USER_DATA;
  bool result = this->directWrite(this->mByteBuffer);
  
  if(result == false)
    this->mStatus = Status::WAIT_BEGIN_FLAG;
  
  return result;
}

/**
 * @brief Set the Send Ok Flag object
 * 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::setSendOkFlag(void){
  if(this->mStatus != Status::WAIT_SEND_OK_FLAG)
    return false;
  
  this->mStatus = Status::IDLE;
  this->executeHandler(true);
  
  return true;
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
 */
void WT32ETH01Transfer::executeHandler(bool successful){
  void* attachment = this->mAttachment;
  mcuf::io::CompletionHandler<int, void*>* handler = this->mHandler;
  int result = this->mByteBuffer->avariable();
  this->mByteBuffer = nullptr;
  
  if(handler != nullptr){
    if(successful)
      handler->completed(result, attachment);
    
    else
      handler->failed(nullptr, attachment);
  }
  
  return;
}
  
/**
 * @brief 
 * 
 */
bool WT32ETH01Transfer::setSendLen(void){
  if(this->mStatus != Status::IDLE)
    return false;
  
  this->mStatus = Status::WRITE_USER_DATALEN;
  this->flush();
  this->putFormat(WT32ETH01Transfer::TEXT_COMMAND_DATA_SEND_FORMAT, this->mByteBuffer->avariable());
  this->flip();
  
  return this->directWrite(this);
}

  
/**
 * @brief 
 * 
 * @param byteBuffer 
 * @return true 
 * @return false 
 */
bool WT32ETH01Transfer::directWrite(mcuf::io::OutputBuffer* byteBuffer){
  bool result = this->mOutputStream.write(*byteBuffer, this, this);
  
  if(result == false){
    this->mStatus = Status::IDLE;
    this->mEvent.accept(Event::WRITE_FAIL);
  }
  
  return result;
}

/* ****************************************************************************************
 * End of file
 */
