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
#include "driver/wirelesstag/internal/WT32ETH01Receiver.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::wirelesstag::internal::WT32ETH01Receiver;
using mcuf::function::Consumer;
using mcuf::io::InputStream;
using mcuf::io::CompletionHandler;
using mcuf::io::Future;
using mcuf::io::ByteBuffer;
using mcuf::lang::String;

/* ****************************************************************************************
 * Variable <Static>
 */
const char* const WT32ETH01Receiver::TEXT_SEND_OK = "SEND OK";
const char* const WT32ETH01Receiver::TEXT_OK = "OK";
const char* const WT32ETH01Receiver::TEXT_ERROR = "ERROR";
const char* const WT32ETH01Receiver::TEXT_NO_IP = "no ip";
const char* const WT32ETH01Receiver::TEXT_ON_SEND = "> ";
const char* const WT32ETH01Receiver::TEXT_MODULE_OK = "module ok";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHIP = "+CIFSR:ETHIP,";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHIP_FORMAT = "+CIFSR:ETHIP,\"%[^\"]\r\n\0";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHMAC = "+CIFSR:ETHMAC,";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHMAC_FORMAT = "+CIFSR:ETHMAC,\"%[^\"]\r\n\0";
const char* const WT32ETH01Receiver::TEXT_IPD = "+IPD,";
const char* const WT32ETH01Receiver::TEXT_IPD_FORMAT = "+IPD,%d:\0";

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new WT32ETH01Receiver object
 * 
 */
WT32ETH01Receiver::WT32ETH01Receiver(InputStream& inputStream, Consumer<Event>& event) :
RingBufferInputStream(Memory(this->mRingBufferMemory, sizeof(this->mRingBufferMemory))),
mInternetProtocolAddress(),
mMediaAccessControlAddress(),
mInputStream(inputStream),
mByteBuffer(Memory(this->mByteBufferMemory, sizeof(this->mByteBufferMemory))),
mEvent(event){
  this->mWaitLength = 0;
  return;
}
/**
 * @brief Destroy the WT32ETH01Receiver object
 * 
 */
WT32ETH01Receiver::~WT32ETH01Receiver(void){
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override> - mcuf::io::CompletionHandler<int ,void*>
 */

/**
 * @brief 
 * 
 * @param result 
 * @param attachment 
 */
void WT32ETH01Receiver::completed(int result, void* attachment){
  switch(this->mStatus){
    case Status::WAIT_CHAR:
      this->eventWaitChar();
      break;
    
    case Status::WAIT_HEAD:
      this->eventWaitHead();
      break;
    
    case Status::WAIT_READ_LEN:
      this->eventWaitLen();
      break;
  }
  return;
}

/**
 * @brief 
 * 
 * @param exc 
 * @param attachment 
 */
void WT32ETH01Receiver::failed(void* exc, void* attachment){
  this->mEvent.accept(Event::ERROR);
  return;
}

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::start(void){
  this->beginReadHead();
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::stop(void){
  this->abortRead();
  this->flush();
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
 * @return true 
 * @return false 
 */
void WT32ETH01Receiver::beginReadHead(void){ 
  this->mStatus = Status::WAIT_HEAD;
  this->mByteBuffer.clear();
  this->mByteBuffer.limit(1);
  this->mInputStream.read(this->mByteBuffer, this, this);
  return;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
void WT32ETH01Receiver::beginReadNext(void){
  this->mByteBuffer.limit(this->mByteBuffer.position()+1);
  this->mInputStream.read(this->mByteBuffer, this, this);
  return;
}

/**
 * @brief 
 * 
 * @param ch 
 * @return true 
 * @return false 
 */
void WT32ETH01Receiver::beginReadAtChar(char ch){
  this->mWaitChar = ch;
  this->mStatus = Status::WAIT_CHAR;
  this->beginReadNext();
  return;
}

/**
 * @brief 
 * 
 * @param len 
 * @return true 
 * @return false 
 */
void WT32ETH01Receiver::beginReadAtLength(uint16_t len){
  this->mStatus = Status::WAIT_READ_LEN;
  uint16_t max = static_cast<uint16_t>(this->mByteBuffer.capacity() - this->mByteBuffer.position());
  if(len > max)
    this->mWaitLength = len - max;
  
  len = max;
  this->mByteBuffer.limit(this->mByteBuffer.position() + len);
  this->mInputStream.read(this->mByteBuffer, this, this);
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventWaitHead(void){
  char ch = this->mByteBuffer[0];
  switch(ch){
    case '+':
      this->beginReadAtChar(',');
      break;
    
    case 'O':
    case 'E':
    case 'S':
    case 'm':
    case 'n':
    case 'R':
      this->beginReadAtChar('\n');
      break;
    
    case '>':
      this->beginReadAtChar(' ');
      break;
    
    default:
      this->beginReadHead();
  }
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventWaitChar(void){
  char ch = this->mByteBuffer[this->mByteBuffer.position()];
  
  if(ch != this->mWaitChar){
    this->beginReadNext();
    return;
  }
  
  switch(ch){
    case ',':
      this->eventCommand();
      break;
    
    case '\n':
      this->eventResult();
      this->beginReadHead();
      break;
    
    case ' ':
      this->mEvent.accept(Event::ON_SEND);
      this->beginReadHead();
      break;
    
    default:
      this->beginReadHead();
      break;
  }
  return;
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventWaitLen(void){
  this->mByteBuffer.flip();
  this->insertMult(this->mByteBuffer.pointer(), this->mByteBuffer.limit());
  
  if(this->mWaitLength == 0){
    this->beginReadHead();
  }else{
    this->mByteBuffer.clear();
    this->beginReadAtLength(this->mWaitLength);
  }
  
  return;
}

/**
 * @brief 
 * 
 */
void  WT32ETH01Receiver::eventResult(void){
  char ch = this->mByteBuffer[0];
  switch(ch){
    case ':':
      this->eventStreamLength();
      break;
    
    case '+':
      this->eventConvertReturn();
      break;
    
    case 'O':
      if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_OK) != -1)
        this->mEvent.accept(Event::OK);
      
      break;
      
    case 'E':
      if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_ERROR) != -1)
        this->mEvent.accept(Event::ERROR);
      
      break;
      
    case 'S':
      if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_SEND_OK) != -1)
        this->mEvent.accept(Event::SEND_OK);
      
      break;
      
    case 'm':
      if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_MODULE_OK) != -1)
        this->mEvent.accept(Event::MODULE_OK);
      
      break;
      
    case 'n':
      if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_NO_IP) != -1)
        this->mEvent.accept(Event::NO_IP);
      
      break;
      
    case 'R':
      break;
  }
  
  return;
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventCommand(void){
  if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_IPD) != -1)
    this->beginReadAtChar(':');

  else
    this->beginReadAtChar('\n');
  
  return;
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventConvertReturn(void){
  char cache[20];
  int result = 0;
  this->mByteBuffer.limit(this->mByteBuffer.limit() + 1);
  this->mByteBuffer.putByte(0x00);
  const char* src = static_cast<const char*>(this->mByteBuffer.pointer());
  
  result = String::scanFormat(src, WT32ETH01Receiver::TEXT_CIFSR_ETHIP_FORMAT, &cache);
  if(result == 1){
    this->mInternetProtocolAddress.setAddress(cache);
    return;
  }
  
  result = String::scanFormat(src, WT32ETH01Receiver::TEXT_CIFSR_ETHMAC_FORMAT, &cache);
  if(result == 1){
    this->mMediaAccessControlAddress.setMediaAccessControlAddress(cache);
    return;
  }
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::eventStreamLength(void){
  this->mByteBuffer.limit(this->mByteBuffer.limit() + 1);
  this->mByteBuffer.putByte(0x00);
  const char* src = static_cast<const char*>(this->mByteBuffer.pointer());
  uint16_t len = 0;
  int result = String::scanFormat(src, WT32ETH01Receiver::TEXT_IPD_FORMAT, &len);
  
  if(result == 1){
    this->mByteBuffer.clear();
    this->beginReadAtLength(len);
    return;
  }
  
  this->beginReadHead();
  return;
}

/* ****************************************************************************************
 * End of file
 */
