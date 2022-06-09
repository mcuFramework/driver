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
#include <string.h>

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
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHIP_FORMAT = "+CIFSR:ETHIP,\"%[^\"]\0";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHMAC = "+CIFSR:ETHMAC,";
const char* const WT32ETH01Receiver::TEXT_CIFSR_ETHMAC_FORMAT = "+CIFSR:ETHMAC,\"%[^\"]\0";
const char* const WT32ETH01Receiver::TEXT_IPD = "+IPD,";
const char* const WT32ETH01Receiver::TEXT_IPD_FORMAT = "+IPD,%d:\0";

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new WT32ETH01Receiver object
 * 
 */
WT32ETH01Receiver::WT32ETH01Receiver(Consumer<Event>& event) :
RingBufferInputStream(Memory(this->mRingBufferMemory, sizeof(this->mRingBufferMemory))),
mInternetProtocolAddress(),
mMediaAccessControlAddress(),
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
 * Public Method
 */
/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::reset(void){
  this->mWaitLength = 0;
  this->mInternetProtocolAddress.setAddress(0, 0, 0, 0);
  
  const uint8_t empty[6] = {0,0,0,0,0,0};
  this->mMediaAccessControlAddress.setMediaAccessControlAddress(empty);
  
  this->mState = State::WAIT_HEAD;
  this->mByteBuffer.flush();
  this->flush();
  return;
}

/**
 * @brief 
 * 
 * @param outputBuffer 
 */
void WT32ETH01Receiver::execute(mcuf::io::OutputBuffer& outputBuffer){
  while(true){
    if(outputBuffer.isEmpty())
      break;

    switch(this->mState){
      case State::WAIT_HEAD:
        this->executeWaitHead(outputBuffer);
        break;
      
      case State::WAIT_CAHR:
        this->executeWaitChar(outputBuffer);
        break;
      
      case State::WAIT_RECEIVER_DATA:
        this->executeWaitReceiverData(outputBuffer);
        break;
    }

  }
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
 * @param outputBuffer 
 */
void WT32ETH01Receiver::executeWaitHead(mcuf::io::OutputBuffer& outputBuffer){
  char ch;
  
  while(true){
    if(outputBuffer.getByte(ch) == false)
      break;
    
    switch(ch){
      case '+':
        this->mByteBuffer.flush();
        this->mByteBuffer.putByte(ch);
        this->setWaitChar(',');
        return;
      
      case 'O':
      case 'E':
      case 'S':
      case 'm':
      case 'n':
      case 'R':
        this->mByteBuffer.flush();
        this->mByteBuffer.putByte(ch);
        this->setWaitChar('\n');
        return;
      
      case '>':
        this->mByteBuffer.flush();
        this->mByteBuffer.putByte(ch);
        this->setWaitChar(' ');
        return;
      
      default:
        break;
    }
  }
  
  return;
}

/**
 * @brief 
 * 
 * @param outputBuffer 
 */
void WT32ETH01Receiver::executeWaitChar(mcuf::io::OutputBuffer& outputBuffer){
  while(true){
    char ch;
    if(outputBuffer.getByte(ch) == false)
      break;
    
    this->mByteBuffer.putByte(ch);
    if(ch != this->mWaitChar)
      continue;
    
    switch(ch){
      case ':':
        this->setWaitReceiverData();
        return;    

      case ',':
        if(this->mByteBuffer.indexOfString(WT32ETH01Receiver::TEXT_IPD) != -1)
          this->setWaitChar(':');
        
        else
          this->setWaitChar('\n');
        break;
      
      case '\n':
        this->commandParse();
        this->setWaitHead();
        return;
      
      case ' ':
        this->mEvent.accept(Event::ON_SEND);
        this->setWaitHead();
        return;
      
      default:
        this->setWaitHead();
        return;
    }
  }
}

/**
 * @brief 
 * 
 * @param outputBuffer 
 */
void WT32ETH01Receiver::executeWaitReceiverData(mcuf::io::OutputBuffer& outputBuffer){
  char ch;
  while(true){
    if(this->mWaitLength <= 0){
      this->mWaitLength = 0;
      this->setWaitHead();
      break;
    }
    
    if(outputBuffer.getByte(ch) == false)
      break;
    
    this->putByte(ch);
    --this->mWaitLength;
  }
}

/**
 * @brief Set the Wait Char object
 * 
 * @param ch 
 */
void WT32ETH01Receiver::setWaitChar(char ch){
  this->mState = State::WAIT_CAHR;
  this->mWaitChar = ch;
  return;
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::setWaitHead(void){
  this->mState = State::WAIT_HEAD;
  this->mWaitChar = 0x00;
  this->mByteBuffer.flush();
  return;
}

/**
 * @brief
 *
 */
void WT32ETH01Receiver::commandParse(void){
  char ch = this->mByteBuffer[0];
  switch(ch){
    
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
void WT32ETH01Receiver::eventConvertReturn(void){
  char cache[20];
  int result = 0;
  this->mByteBuffer.putByte(0x00);
  const char* src = static_cast<const char*>(this->mByteBuffer.pointer());
  
  if(strcmp(src, "+CIFSR:ETHIP,\"0.0.0.0\"\r\n") == 0){
    this->mInternetProtocolAddress.setAddress("0.0.0.0\0");
    
  }else{
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
}

/**
 * @brief 
 * 
 */
void WT32ETH01Receiver::setWaitReceiverData(void){
  this->mByteBuffer.putByte(0x00);
  const char* src = static_cast<const char*>(this->mByteBuffer.pointer());
  
  int len = 0;
  int result = String::scanFormat(src, WT32ETH01Receiver::TEXT_IPD_FORMAT, &len);
  
  if(result == 1){
    this->mWaitLength = len;
    this->mState = State::WAIT_RECEIVER_DATA;
    return;
  }
  
  this->setWaitHead();
  return;
}

/* ****************************************************************************************
 * End of file
 */
