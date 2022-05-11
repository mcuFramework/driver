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
#include "driver/thermometer/Thermistor.h"

/* ****************************************************************************************
 * Macro
 */

/* ****************************************************************************************
 * Using
 */

//-----------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------
using driver::thermometer::Thermistor;
using mcuf::lang::Math;

/* ****************************************************************************************
 * Variable <Static>
 */

/* ****************************************************************************************
 * Construct Method
 */

/**
 * @brief Construct a new Thermistor:: Thermistor object
 * 
 * @param voltmeter 
 * @param voltagePower 
 */
Thermistor::Thermistor(mcuf::util::Voltmeter& voltmeter, float voltagePower) : 
mVoltmeter(voltmeter){

  this->mVoltagePower = voltagePower;
  this->mBeta = 3950;
  this->mResistanceSensor = 10;
  this->mResistanceThermistor = 10;
  return;
}

/**
 * @brief Destroy the Thermistor object
 * 
 */
Thermistor::~Thermistor(void){
  return;
}

/* ****************************************************************************************
 * Operator Method
 */

/* ****************************************************************************************
 * Public Method <Static>
 */

/* ****************************************************************************************
 * Public Method <Override>
 */

/* ****************************************************************************************
 * Public Method
 */

/**
 * @brief Get the Celsius object
 * 
 * @return float 
 */
float Thermistor::getCelsius(void){
  double voltage = static_cast<double>(this->mVoltmeter.voltage());
  double rt = ((static_cast<double>(this->mVoltagePower) - voltage) * static_cast<double>(this->mResistanceSensor)) / voltage;
  double logValue = Math::log(static_cast<double>(rt / static_cast<double>(this->mResistanceThermistor)));
  double result = (((298.15 * this->mBeta) / (this->mBeta + 298.15 * logValue)) - 273.15);
  
  return static_cast<float>(result);
}

/**
 * @brief 
 * 
 * @return int 
 */
int Thermistor::beta(void){
  return this->mBeta;
}

/**
 * @brief 
 * 
 * @param value 
 */
void Thermistor::beta(int value){
  this->mBeta = value;
  return;
}

/**
 * @brief 
 * 
 * @return float 
 */
float Thermistor::resistanceSensor(void){
  return this->mResistanceSensor;
}

/**
 * @brief 
 * 
 * @param value 
 */
void Thermistor::resistanceSensor(float value){
  this->mResistanceSensor = value;
  return;
}

/**
 * @brief 
 * 
 * @return float 
 */
float Thermistor::resistanceThermistor(void){
  return this->mResistanceThermistor;
}

/**
 * @brief 
 * 
 * @param value 
 */
void Thermistor::resistanceThermistor(float value){
  this->mResistanceThermistor = value;
  return;
}

/**
 * @brief 
 * 
 * @return float 
 */
float Thermistor::voltagePower(void){
  return this->mVoltagePower;
}

/**
 * @brief 
 * 
 * @param value 
 */
void Thermistor::voltagePower(float value){
  this->mVoltagePower = value;
  return;
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

/* ****************************************************************************************
 * End of file
 */
