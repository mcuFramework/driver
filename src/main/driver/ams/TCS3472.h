/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_24F76255_C2F6_4840_9A2D_5EDEC9D147D3
#define DRIVER_24F76255_C2F6_4840_9A2D_5EDEC9D147D3

/* ****************************************************************************************
 * Include
 */  

//-----------------------------------------------------------------------------------------
#include "mcuf.h"

//-----------------------------------------------------------------------------------------

/* ****************************************************************************************
 * Namespace
 */  
namespace driver::ams{
  class TCS3472;
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::ams::TCS3472 extends mcuf::lang::Object implements
public hal::serial::SerialBusEvent,
public mcuf::util::Updater{

  /* **************************************************************************************
   * Enum Type
   */  
  public:
    enum struct Type : char{
      TCS34725,
      TCS34727
    };
    
  /* **************************************************************************************
   * Enum Address
   */    
  public:
    enum struct Address : char{
      ADDR_ENABLE  = 0x00,
      ADDR_ATIME   = 0x01,
      ADDR_WTIME   = 0x03,
      ADDR_AILTL   = 0x04,
      ADDR_AILTH   = 0x05,
      ADDR_AIHTL   = 0x06,
      ADDR_AIHTH   = 0x07,
      ADDR_PERS    = 0x0C,
      ADDR_CONFIG  = 0x0D,
      ADDR_CONTROL = 0x0F,
      ADDR_ID      = 0x12,
      ADDR_STATUS  = 0x13,
      ADDR_CDATAL  = 0x14,
      ADDR_CDATAH  = 0x15,
      ADDR_RDATAL  = 0x16,
      ADDR_RDATAH  = 0x17,
      ADDR_GDATAL  = 0x18,
      ADDR_GDATAH  = 0x19,
      ADDR_BDATAL  = 0x1A,
      ADDR_BDATAH  = 0x1B
    };

  /* **************************************************************************************
   * Struct Register
   */      
    
  public:
    struct Register{
      uint8_t ENABLE;       //0x00
      uint8_t ATIME;        //0x01
      uint8_t reserved0;    //0x02
      uint8_t WTIME;        //0x03
      uint16_t AILT;        //0x04-0x05
      uint16_t AIHT;        //0x06-0x07
      uint8_t reserved1[4]; //0x08-0x0B
      uint8_t PERS;         //0x0C
      uint8_t CONFIG;       //0x0D
      uint8_t reserved2;    //0x0E
      uint8_t CONTROL;      //0x0F
      uint8_t reserved3[2]; //0x10-0x11
      uint8_t ID;           //0x12
      uint8_t STATUS;       //0x13
      uint16_t CLEAR;       //0x14-0x15
      uint16_t RED;         //0x16-0x17
      uint16_t GREEN;       //0x18-0x19
      uint16_t BLUE;        //0x1A-0x1B
    };
    
  /* **************************************************************************************
   * Variable <Public>
   */
  public:
    Register mRegister;
    
  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    hal::serial::SerialBus& mSerailBus;
    mcuf::io::ByteBuffer mByteBufferTransfer;
    mcuf::io::ByteBuffer mByteBufferReceiver;
    uint8_t mWriteBuffer[4];
    uint8_t mAddress;
    bool mBusy;

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
     *
     */
    TCS3472(Type type, hal::serial::SerialBus& serailBus);

    /**
     *
     */
    virtual ~TCS3472(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - hal::serial::SerialBusEvent
   */
  public:
    /**
     * @brief 
     * 
     * @param status handle status
     * @param result 0 = successful, other = remaining byte count.
     * @param attachment user data
     */
    virtual void onSerialBusEvent(hal::serial::SerialBusStatus status, 
                                  int result,
                                  void* attachment) override;

  /* **************************************************************************************
   * Public Method <Override> - mcuf::util::Updater
   */
  public:
    /**
     * @brief 更新資源
     * 
     * @return true 開始嘗試更新成功
     * @return false 開始嘗試更新失敗
     */
    virtual bool update(void) override;

    /**
     * @brief 是否正在更新
     * 
     * @return true 正在更新中
     * @return false 等待更新
     */
    virtual bool isUpdating(void) override;

  /* **************************************************************************************
   * Public Method
   */
  public:
  
    /**
     * @brief TCS3472啟用
     * 
     * @return true 啟用成功
     * @return false 啟用失敗
     */
    bool enable(void);

    /**
     * @brief TCS3472停用
     * 
     * @return true 停用成功
     * @return false 停用失敗
     */
    bool disable(void);
  
    /**
     * @brief Set the Integration Cycle object
     * 
     * @param cycle 
     * @return true 
     * @return false 
     */
    bool setIntegrationCycle(uint8_t cycle);
  
    /**
     * @brief 設定TCS3472暫存器
     * 
     * @param addr 地址位置
     * @param data 資料
     * @return true 寫入成功
     * @return false 寫入失敗
     */
    bool writeRegister(Address addr, uint8_t data);

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
     * @brief Get the Command object
     * 
     * @param addr 
     * @return uint8_t 
     */
    uint8_t getCommand(Address addr);

};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_24F76255_C2F6_4840_9A2D_5EDEC9D147D3 */
