/**
 * Copyright (c) 2020 ZxyKira
 * All rights reserved.
 * 
 * SPDX-License-Identifier: MIT
 */
#ifndef DRIVER_CBAE3B23_3019_4BB0_931B_474789932CB8
#define DRIVER_CBAE3B23_3019_4BB0_931B_474789932CB8

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
  namespace thermometer{
    class Thermistor;
  }
}


/* ****************************************************************************************
 * Class/Interface/Struct/Enum
 */  
class driver::thermometer::Thermistor extends mcuf::lang::Object implements
public mcuf::util::TemperatureSensor{

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
    mcuf::util::Voltmeter& mVoltmeter;
    int mBeta;
    float mResistanceSensor;
    float mResistanceThermistor;
    float mVoltagePower;

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
     * @brief Construct a new Thermistor object
     * 
     * @param voltmeter 
     * @param voltagePower 
     */
    Thermistor(mcuf::util::Voltmeter& voltmeter, float voltagePower);

    /**
     * @brief Destroy the Thermistor object
     * 
     */
    virtual ~Thermistor(void) override;

  /* **************************************************************************************
   * Operator Method
   */

  /* **************************************************************************************
   * Public Method <Static>
   */

  /* **************************************************************************************
   * Public Method <Override> - mcuf::util::TemperatureSensor
   */
  public:
    /**
     * @brief Get the Celsius object
     * 
     * @return float 
     */
    virtual float getCelsius(void) override;
  
  /* **************************************************************************************
   * Public Method
   */
  public:

    /**
     * @brief 
     * 
     * @return int 
     */
    int beta(void);

    /**
     * @brief 
     * 
     * @param value 
     */
    void beta(int value);

    /**
     * @brief 
     * 
     * @return float 
     */
    float resistanceSensor(void);

    /**
     * @brief 
     * 
     * @param value 
     */
    void resistanceSensor(float value);

    /**
     * @brief 
     * 
     * @return float 
     */
    float resistanceThermistor(void);

    /**
     * @brief 
     * 
     * @param value 
     */
    void resistanceThermistor(float value);

    /**
     * @brief 
     * 
     * @return float 
     */
    float voltagePower(void);

    /**
     * @brief 
     * 
     * @param value 
     */
    void voltagePower(float value);

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

#endif /* DRIVER_CBAE3B23_3019_4BB0_931B_474789932CB8 */
