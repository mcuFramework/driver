/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_BCBBF717_2A77_4A94_90C7_4B489C0F36B3
#define DRIVER_BCBBF717_2A77_4A94_90C7_4B489C0F36B3

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"

//-----------------------------------------------------------------------------------------
#include "driver\wirelesstag\internal\WT32ETH01Receiver.h"
#include "driver\wirelesstag\internal\WT32ETH01Transfer.h"

/* ****************************************************************************************
 * Namespace
 */  
namespace driver::wirelesstag{
  class WT32ETH01;
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::wirelesstag::WT32ETH01 extends mcuf::lang::Object implements
  public mcuf::io::InputStream,
  public mcuf::io::OutputStream,
  public hal::Base,
  private mcuf::function::Consumer<internal::WT32ETH01Receiver::Event>,
  private mcuf::function::Consumer<internal::WT32ETH01Transfer::Event>{

  /* **************************************************************************************
   * Enum ConnectType
   */
  public:

    /**
     * @brief 
     * 
     */
    enum struct ConnectType : char{
      TCP,
      UDP
    };
    
  /* **************************************************************************************
   * Enum Status
   */
  private:
    /**
     * @brief 
     * 
     */
    enum struct Status : char{
      NOT_INIT,
      WAIT_INIT,
      WAIT_INIT_SET_ADDRESS,
      INITD,
      HANDLE,
      HANDLE_CONNECTS_UPDATE_IP,
      HANDLE_CONNECTC_UPDATE_IP,
      HANDLE_CONNECT,
      TRANSFER
    };
    
  /* **************************************************************************************
   * Enum ConnectStatus
   */
    
  /* **************************************************************************************
   * Variable <Public>
   */
  public:
  
  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    hal::serial::SerialPort& mSerialPort;
    hal::general::GeneralPin& mEnablePin;
    mcuf::io::SerialPortInputStream mSerialPortInputStream;
    mcuf::io::SerialPortOutputStream mSerialPortOutputStream;
    driver::wirelesstag::internal::WT32ETH01Receiver mReceiver;
    driver::wirelesstag::internal::WT32ETH01Transfer mTransfer;
    mcuf::net::SocketAddress mRemoteAddress;
    mcuf::io::Future* mFuture;
    uint32_t mIp;
    uint32_t mGateway;
    uint32_t mMask;
    Status mStatus;
    driver::wirelesstag::internal::WT32ETH01Transfer::ConnectType mConnectType;

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
     * @brief Construct a new WT32ETH01 object
     * 
     * @param serialPort 
     * @param enablePin 
     * @param bufferSize 
     */
    WT32ETH01(hal::serial::SerialPort& serialPort, hal::general::GeneralPin& enablePin);

    /**
     * @brief Destroy the WT32ETH01 object
     * 
     */
    virtual ~WT32ETH01(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - hal::Base
   */
  public:
    /**
     * @brief uninitialze hardware.
     * 
     * @return true 
     * @return false 
     */
    virtual bool deinit(void) override;

    /**
     * @brief initialze hardware;
     * 
     * @return true 
     * @return false 
     */
    virtual bool init(void) override;

    /**
     * @brief get hardware initialzed status.
     * 
     * @return true not init
     * @return false initd
     */
    virtual bool isInit(void) override;

  /* **************************************************************************************
   * Public Method <Override> - mcuf::io::InputStream
   */
  public:
    /**
     * @brief 
     * 
     * @return int 
     */
    virtual int avariable(void) override;
    
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool abortRead(void) override;  
    
    /**
     * @brief 
     * 
     * @return true is busy.
     * @return false isn't busy.
     */
    virtual bool readBusy(void) override;
    
    /**
     * @brief pop buffer byte non blocking.
     * 
     * @param result 
     * @return true has data in buffer.
     * @return false no data in buffer.
     */
    virtual bool getByte(char& result) override;

    /**
     * @brief 
     * 
     * @param byteBuffer 
     * @return int 
     */
    virtual int get(mcuf::io::ByteBuffer& byteBuffer) override;

    /**
     * @brief 
     * 
     * @param buffer 
     * @param bufferSize 
     * @return int 
     */
    virtual int get(void* buffer, int bufferSize) override;    
    
    /**
     * @brief 
     * 
     * @param byteBuffer 
     * @return int 
     */
    virtual bool read(mcuf::io::ByteBuffer& byteBuffer) override;    
    
    /**
     * @brief nonblocking
     * 
     * @param byteBuffer 
     * @param attachment 
     * @param handler 
     * @return true successful.
     * @return false fail.
     */
    virtual bool read(mcuf::io::ByteBuffer& byteBuffer, 
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
    virtual bool read(mcuf::io::ByteBuffer& byteBuffer, mcuf::io::Future& future) override;

    /**
     * @brief 
     * 
     * @param value 
     * @param attachment 
     * @param handler 
     * @return true 
     * @return false 
     */
    virtual bool skip(int value, 
                      void* attachment,
                      mcuf::io::CompletionHandler<int, void*>* handler) override;

    /**
     * @brief 
     * 
     * @param value 
     * @param future 
     * @return true 
     * @return false 
     */
    virtual bool skip(int value, mcuf::io::Future& future) override;

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
    virtual bool write(mcuf::io::ByteBuffer& byteBuffer, 
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
    virtual bool write(mcuf::io::ByteBuffer& byteBuffer, mcuf::io::Future& future) override;
                      
  /* **************************************************************************************
   * Public Method <Override> - mcuf::function::Consumer<internal::WT32ETH01Receiver::Event>
   */
  public:
    /**
     * @brief 
     * 
     * @param t 
     */
    virtual void accept(internal::WT32ETH01Receiver::Event t) override;
  
  /* **************************************************************************************
   * Public Method <Override> - mcuf::function::Consumer<internal::WT32ETH01Transfer::Event>
   */
  public:
    /**
     * @brief 
     * 
     * @param t 
     */
    virtual void accept(internal::WT32ETH01Transfer::Event t) override;
                      
  /* **************************************************************************************
   * Public Method
   */
  public:

    /**
     * @brief 
     * 
     * @return mcuf::net::InternetProtocolAddress 
     */
    mcuf::net::InternetProtocolAddress getIP(void);
  
    /**
     * @brief Get the Local Address object
     * 
     * @return mcuf::net::SocketAddress 
     */
    mcuf::net::SocketAddress getLocalAddress(void);

    /**
     * @brief Get the Remode Address object
     * 
     * @return mcuf::net::SocketAddress 
     */
    mcuf::net::SocketAddress getRemodeAddress(void);

    /**
     * @brief Get the Mac Address object
     * 
     * @return mcuf::net::MediaAccessControlAddress 
     */
    mcuf::net::MediaAccessControlAddress getMacAddress(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool isConnect(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool disconnect(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool reset(void);

    /**
     * @brief 
     * 
     * @param type 
     * @param remoteAddress 
     * @param future 
     * @return true 
     * @return false 
     */
    bool connect(ConnectType type, const mcuf::net::SocketAddress& remoteAddress, mcuf::io::Future& future);

    /**
     * @brief 
     * 
     * @param type 
     * @param port 
     * @param future 
     * @return true 
     * @return false 
     */
    bool listen(ConnectType type, const uint16_t port, mcuf::io::Future& future);
    
    /**
     * @brief Set the Static I P Address object
     * 
     * @param ip 
     * @param gateway 
     * @param mask 
     * @return true 
     * @return false 
     */
    bool setStaticIPAddress(const mcuf::net::InternetProtocolAddress& ip,
                            const mcuf::net::InternetProtocolAddress& gateway,
                            const mcuf::net::InternetProtocolAddress& mask);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool setDHCP(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    bool updateStatus(void);

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
     * @param successful 
     */
    void executeHandle(bool successful);
    
    /**
     * @brief 
     * 
     */    
    void eventOk(void);
  
    /**
     * @brief Set the Future object
     * 
     * @param future 
     * @return true 
     * @return false 
     */
    bool setFuture(mcuf::io::Future& future);
};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_BCBBF717_2A77_4A94_90C7_4B489C0F36B3 */
