/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_9A5AEBFD_C794_4994_806D_3071898DD501
#define DRIVER_9A5AEBFD_C794_4994_806D_3071898DD501

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Namespace
 */  
namespace driver{
  namespace wirelesstag{
    namespace internal{
      class WT32ETH01Receiver;
    }
  }
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::wirelesstag::internal::WT32ETH01Receiver extends mcuf::io::RingBufferInputStream{
  
  /* **************************************************************************************
   * Enum State
   */
  private:
    enum struct State : int{
      WAIT_HEAD,
      WAIT_CAHR,
      WAIT_RECEIVER_DATA
    };
  
    
  /* **************************************************************************************
   * Enum Event
   */
  public:
    enum struct Event{
      MODULE_OK,
      OK,
      ERROR,
      NO_IP,
      ON_SEND,
      SEND_OK
    };
    
  /* **************************************************************************************
   * Variable <Public>
   */
  public:
    static const char* const TEXT_SEND_OK;
    static const char* const TEXT_OK;
    static const char* const TEXT_ERROR;
    static const char* const TEXT_NO_IP;
    static const char* const TEXT_ON_SEND;
    static const char* const TEXT_MODULE_OK;
    static const char* const TEXT_CIFSR_ETHIP;
    static const char* const TEXT_CIFSR_ETHIP_FORMAT;
    static const char* const TEXT_CIFSR_ETHMAC;
    static const char* const TEXT_CIFSR_ETHMAC_FORMAT;
    static const char* const TEXT_IPD;
    static const char* const TEXT_IPD_FORMAT;

    mcuf::net::InternetProtocolAddress mInternetProtocolAddress;
    mcuf::net::MediaAccessControlAddress mMediaAccessControlAddress;    

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    mcuf::io::ByteBuffer mByteBuffer;
    mcuf::function::Consumer<Event>& mEvent;
    uint8_t mRingBufferMemory[256];
    uint8_t mByteBufferMemory[64];
    int mWaitLength;
    State mState;
    char mWaitChar;
    
  
  /* **************************************************************************************
   * Abstract method <Public>
   */

  /* **************************************************************************************
   * Abstract method <Protected>
   */

  /* **************************************************************************************
   * Construct Method
   */
  public: 
    /**
     * @brief Construct a new WT32ETH01Receiver object
     * 
     */
    WT32ETH01Receiver(mcuf::function::Consumer<Event>& event);

    /**
     * @brief Destroy the WT32ETH01Receiver object
     * 
     */
    virtual ~WT32ETH01Receiver(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method
   */
  public:
    /**
     * @brief 
     * 
     */
    void reset(void);

    /**
     * @brief 
     * 
     * @param outputBuffer 
     */
    void execute(mcuf::io::OutputBuffer& outputBuffer);

  /* **************************************************************************************
   * Protected Method <Static>
   */

  /* **************************************************************************************
   * Protected Method <Override>
   */

  /* **************************************************************************************
   * Protected Method
   */

  /* **************************************************************************************
   * Private Method <Static>
   */

  /* **************************************************************************************
   * Private Method <Override>
   */

  /* **************************************************************************************
   * Private Method
   */
  private:
    /**
     * @brief 
     * 
     * @param outputBuffer 
     */
    void executeWaitHead(mcuf::io::OutputBuffer& outputBuffer);

    /**
     * @brief 
     * 
     * @param outputBuffer 
     */
    void executeWaitChar(mcuf::io::OutputBuffer& outputBuffer);

    /**
     * @brief 
     * 
     * @param outputBuffer 
     */
    void executeWaitReadLen(mcuf::io::OutputBuffer& outputBuffer);
  
    /**
     * @brief 
     * 
     * @param outputBuffer 
     */
    void executeWaitReceiverData(mcuf::io::OutputBuffer& outputBuffer);

    /**
     * @brief 
     * 
     */
    void eventConvertReturn(void);

    /**
     * @brief 
     * 
     */
    void setWaitReceiverData(void);
    
    /**
     * @brief Set the Wait Char object
     * 
     * @param ch 
     */
    void setWaitChar(char ch);
    
    /**
     * @brief 
     * 
     */
    void setWaitHead(void);
    
    /**
     * @brief
     *
     */
    void commandParse(void);
};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_9A5AEBFD_C794_4994_806D_3071898DD501 */
