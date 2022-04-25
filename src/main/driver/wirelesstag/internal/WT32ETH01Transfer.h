/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_14327736_8FCD_450E_ADA9_443F7D50F6D7
#define DRIVER_14327736_8FCD_450E_ADA9_443F7D50F6D7

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
      class WT32ETH01Transfer;
    }
  }
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::wirelesstag::internal::WT32ETH01Transfer extends mcuf::io::ByteBuffer implements
  public mcuf::io::OutputStream,
  public mcuf::io::CompletionHandler<int, void*>{

  /* **************************************************************************************
   * Enum Status
   */
  private:
    enum struct Status{
      IDLE,
      WRITE_COMMAND,
      WRITE_USER_DATALEN,
      WAIT_BEGIN_FLAG,
      WAIT_SEND_OK_FLAG,
      WRITE_USER_DATA
    };
    
  /* **************************************************************************************
   * Enum Event
   */
  public:
    enum struct Event{
      WRITE_SUCCESSFUL,
      WRITE_FAIL
    };

  /* **************************************************************************************
   * Enum Event
   */
  public:
    enum struct ConnectType{
      TCP_CLIENT,
      TCP_SERVER,
      UDP_CLIENT,
      UDP_SERVER
    };

  /* **************************************************************************************
   * Variable <Public>
   */
  public:
    static const char* const TEXT_COMMAND_ATE0;
    static const char* const TEXT_COMMAND_GET_ADDRESS;
    static const char* const TEXT_COMMAND_STATIC_IP_FORMAT;
    static const char* const TEXT_COMMAND_CONNECT_FORMAT;
    static const char* const TEXT_COMMAND_DATA_SEND_FORMAT;
    static const char* const TEXT_IPADDRESS_FORMAT;
    static const char* const TEXT_PARAM_TCPS;
    static const char* const TEXT_PARAM_TCPC;
    static const char* const TEXT_PARAM_UDPS;
    static const char* const TEXT_PARAM_UDPC;

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    mcuf::io::OutputStream& mOutputStream;
    mcuf::function::Consumer<Event>& mEvent;
    void* mAttachment;
    mcuf::io::CompletionHandler<int, void*>* mHandler;
    mcuf::io::OutputBuffer* mByteBuffer;
    uint8_t mBuffer[128];
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
     * @brief Construct a new WT32ETH01Transfer object
     * 
     */
    WT32ETH01Transfer(mcuf::io::OutputStream& outputStream, mcuf::function::Consumer<Event>& event);

    /**
     * @brief Destroy the WT32ETH01Transfer object
     * 
     */
    virtual ~WT32ETH01Transfer(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - mcuf::io::OutputStream
   */
  public:
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool abortWrite(void) override;
    
    /**
     * @brief 
     * 
     * @return true is busy.
     * @return false isn't busy.
     */
    virtual bool writeBusy(void) override;

    /**
     * @brief 
     * 
     * @param byteBuffer 
     * @param attachment 
     * @param handler 
     * @return true successful.
     * @return false fail.
     */
    virtual bool write(mcuf::io::OutputBuffer& byteBuffer, 
                       void* attachment,
                       mcuf::io::CompletionHandler<int, void*>* handler) override;

    /**
     * @brief 
     * 
     * @param byteBuffer 
     * @param future 
     * @return true 
     * @return false 
     */
    virtual bool write(mcuf::io::OutputBuffer& byteBuffer, mcuf::io::Future& future) override;

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
     * @return true 
     * @return false 
     */
    bool isCommandBusy(void);
  
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
  
    /**
     * @brief Set the Non Ack object
     * 
     * @return true 
     * @return false 
     */
    bool setNonAck(void);

    /**
     * @brief Get the Address object
     * 
     * @return true 
     * @return false 
     */
    bool getAddress(void);

    /**
     * @brief Set the Static Address object
     * 
     * @param ip 
     * @param gateway 
     * @param mask 
     * @return true 
     * @return false 
     */
    bool setStaticAddress(const uint8_t* ip, const uint8_t* gateway, const uint8_t* mask);
    
    /**
     * @brief Set the Static Address object
     * 
     * @param ip 
     * @param gateway 
     * @param mask 
     * @return true 
     * @return false 
     */
    bool setStaticAddress(const uint32_t& ip, const uint32_t& gateway, const uint32_t& mask);    

    /**
     * @brief Set the Connect object
     * 
     * @param type 
     * @param ip 
     * @param destPort 
     * @param localPort 
     * @return true 
     * @return false 
     */
    bool setConnect(ConnectType type, const mcuf::net::SocketAddress& socketAddress);

    /**
     * @brief Set the On Send Flag object
     * 
     * @return true 
     * @return false 
     */
    bool setOnSendFlag(void);
    
    /**
     * @brief Set the Send Ok Flag object
     * 
     * @return true 
     * @return false 
     */
    bool setSendOkFlag(void);
    

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
     */
    void executeHandler(bool successful);
  
    /**
     * @brief 
     * 
     */
    bool setSendLen(void);
  
    /**
     * @brief 
     * 
     * @param byteBuffer 
     * @return true 
     * @return false 
     */
    bool directWrite(mcuf::io::OutputBuffer* byteBuffer);
   
};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_14327736_8FCD_450E_ADA9_443F7D50F6D7 */
