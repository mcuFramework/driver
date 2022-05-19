/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_B0500BD4_003A_4E4C_AAC4_8F82A827CECA
#define DRIVER_B0500BD4_003A_4E4C_AAC4_8F82A827CECA

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
  namespace microchip{
    class MCP3421;
  }
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::microchip::MCP3421 extends mcuf::lang::Object implements
public hal::analog::AnalogInputPin,
public hal::serial::SerialBusEvent{

  /* **************************************************************************************
   * Enum Resolution
   */
  public:
    enum struct Resolution : uint8_t{
      BIT_12 = 0,
      BIT_14 = 1,
      BIT_16 = 2,
      BIT_18 = 3
    };

  /* **************************************************************************************
   * Enum Gain
   */
  public:
    enum struct Gain : uint8_t{
      GAIN_1 = 0,
      GAIN_2 = 1,
      GAIN_4 = 2,
      GAIN_8 = 3
    };

  /* **************************************************************************************
   * Enum ConversionMode
   */
  public:
    enum struct ConversionMode : uint8_t{
      ONE_SHOT_MODE   = 0,
      CONTINUOUS_MODE = 1
    };

  /* **************************************************************************************
   * Enum Status
   */
  public:
    enum struct Status : uint8_t{
      READY   = 0,
      CONVERT = 1
    };

  /* **************************************************************************************
   * Variable <Public>
   */

  /* **************************************************************************************
   * Variable <Protected>
   */

  /* **************************************************************************************
   * Variable <Private>
   */
  private:
    hal::serial::SerialBus& mSerialBus;
    mcuf::io::ByteBuffer mByteBuffer;
    uint32_t mResult;
    uint8_t mBuffer[4];
    uint8_t mConfig;
    uint8_t mAddress;
    uint8_t mBusy;

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
     * @brief Construct a new MCP3421 object
     * 
     * @param serialBus 
     */
    MCP3421(hal::serial::SerialBus& serialBus, uint16_t address);

    /**
     * @brief Destroy the MCP3421 object
     * 
     */
    virtual ~MCP3421(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - hal::analog::AnalogInputPin
   */
  public:
    /**
     * @brief 
     * 
     * @return uint32_t 
     */
    virtual uint32_t convert(void) override;

    /**
     * @brief Get the adc convert level.
     * 
     * @return uint32_t 
     */
    virtual uint32_t getConvertLevel(void) override;

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
   * Public Method
   */
  public:
    /**
     * @brief 
     * 
     * @param value 
     * @return Resolution 
     */
    virtual Resolution resolution(Resolution value);
  
    /**
     * @brief 
     * 
     * @return Resolution 
     */
    virtual Resolution resolution(void);

    /**
     * @brief 
     * 
     * @param value 
     * @return Gain 
     */
    virtual Gain gain(Gain value);

    /**
     * @brief 
     * 
     * @return Gain 
     */
    virtual Gain gain(void);

    /**
     * @brief 
     * 
     * @param value 
     * @return ConversionMode 
     */
    virtual ConversionMode conversionMode(ConversionMode value);

    /**
     * @brief 
     * 
     * @return ConversionMode 
     */
    virtual ConversionMode conversionMode(void);

    /**
     * @brief 
     * 
     * @return Status 
     */
    virtual Status status(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool update(void);
    
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool startConvert(void);

    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool isBusy(void);
    
    /**
     * @brief 
     * 
     * @return true 
     * @return false 
     */
    virtual bool writeOption(void);

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

};

/* ****************************************************************************************
 * End of file
 */ 

#endif /* DRIVER_B0500BD4_003A_4E4C_AAC4_8F82A827CECA */
