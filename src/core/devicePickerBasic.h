#pragma once
#include "YaveContext.h"

class	DevicePickerBasic: public DevicePicker
{
  public:
    DevicePickerBasic(){};

    int  rateDevice(gpuInfo_t &gpu);
};
