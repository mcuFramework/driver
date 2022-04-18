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
class driver::wirelesstag::internal::WT32ETH01Receiver extends mcuf::io::RingBufferInputStream implements
  public mcuf::io::CompletionHandler<int, void*>{
    
    
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
   * Enum Status
   */    
  private:
    enum struct Status{
      WAIT_HEAD,
      WAIT_CHAR,
      WAIT_READ_LEN
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
    mcuf::io::InputStream& mInputStream;
    mcuf::io::ByteBuffer mByteBuffer;
    mcuf::function::Consumer<Event>& mEvent;
    
  
    uint8_t mRingBufferMemory[256];
    uint8_t mByteBufferMemory[64];
    uint16_t mWaitLength;
    char mWaitChar;
    Status mStatus;
  
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
    WT32ETH01Receiver(mcuf::io::InputStream& inputStream, mcuf::function::Consumer<Event>& event);

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
   * Public Method <Override> - mcuf::io::CompletionHandler<int ,void*>
   */
  public:
  
    /**
     * @brief 
     * 
     * @param result 
     * @param attachment 
     */
    virtual void completed(int result, void* attachment) override;
    
    /**
     * @brief 
     * 
     * @param exc 
     * @param attachment 
     */
    virtual void failed(void* exc, void* attachment) override;

  /* **************************************************************************************
   * Public Method
   */
  public:
    /**
     * @brief 
     * 
     */
    void start(void);

    /**
     * @brief 
     * 
     */
    void stop(void);

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
     * @return true 
     * @return false 
     */
    void beginReadHead(void);
  
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    void beginReadNext(void);

    /**
     * @brief 
     * 
     * @param ch 
     * @return true 
     * @return false 
     */
    void beginReadAtChar(char ch);

    /**
     * @brief 
     * 
     * @param len 
     * @return true 
     * @return false 
     */
    void beginReadAtLength(uint16_t len);

    /**
     * @brief 
     * 
     */
    void eventWaitHead(void);
  
    /**
     * @brief 
     * 
     */
    void eventWaitChar(void);
  
    /**
     * @brief 
     * 
     */
    void eventWaitLen(void);

    /**
     * @brief 
     * 
     */
    void eventResult(void);

    /**
     * @brief 
     * 
     */
    void eventCommand(void);

    /**
     * @brief 
     * 
     */
    void eventConvertReturn(void);

    /**
     * @brief 
     * 
     */
    void eventStreamLength(void);
};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_9A5AEBFD_C794_4994_806D_3071898DD501 */
