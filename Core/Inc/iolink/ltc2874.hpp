#pragma once

#include <cstdint>
#include <type_traits>

#include "stm32f0xx_hal.h"

#include "iolink_util.hpp"

namespace ltc2874 {

enum class Command : uint8_t {
  READ = 0x00,
  WRITE = 0x20,
  UPDATE = 0x40,
  WRTUPD = 0x60,
  RESET = 0xE0,
};

enum class Register : uint8_t {
  IRQREG = 0x00,
  IRQMASK = 0x01,
  EVENT1 = 0x02,
  EVENT2 = 0x03,
  EVENT3 = 0x04,
  EVENT4 = 0x05,
  STATUS1 = 0x06,
  STATUS2 = 0x07,
  MODE1 = 0x08,
  MODE2 = 0x09,
  NSF = 0x0A,
  ILLM = 0x0B,
  TMRCTRL = 0x0C,
  CTRL1 = 0x0D,
  CTRL2 = 0x0E,
};

enum class IRQREG_Bit : uint8_t {
  OT = (0x1 << 7),
  SUPPLY = (0x1 << 6),
  WU = (0x1 << 5),
  TOC_LP = (0x1 << 4),
  PWRCHNG = (0x1 << 3),
  TOC_CQ = (0x1 << 2),
  CSENSE = (0x1 << 1),
};

enum class IRQMASK_Bit : uint8_t {
  OT = (0x1 << 7),
  SUPPLY = (0x1 << 6),
  WU = (0x1 << 5),
  TOC_LP = (0x1 << 4),
  PWRCHNG = (0x1 << 3),
  TOC_CQ = (0x1 << 2),
  CSENSE = (0x1 << 1),
};

enum class EVENT1_Bit : uint8_t {
  OT_SD = (0x1 << 7),
  OT_WARN = (0x1 << 6),
  UVLO_VL = (0x1 << 4),
  UVLO_VDD = (0x1 << 3),
  UV_VDD = (0x1 << 2),
  OV_VDD = (0x1 << 1),
};

enum class EVENT2_Bit : uint8_t {
  WU4 = (0x1 << 7),
  WU3 = (0x1 << 6),
  WU2 = (0x1 << 5),
  WU1 = (0x1 << 4),
  TOC_LP4 = (0x1 << 3),
  TOC_LP3 = (0x1 << 2),
  TOC_LP2 = (0x1 << 1),
  TOC_LP1 = 0x1,
};

enum class EVENT3_Bit : uint8_t {
  PWRCHNG4 = (0x1 << 7),
  PWRCHNG3 = (0x1 << 6),
  PWRCHNG2 = (0x1 << 5),
  PWRCHNG1 = (0x1 << 4),
  TOC_CQ4 = (0x1 << 3),
  TOC_CQ3 = (0x1 << 2),
  TOC_CQ2 = (0x1 << 1),
  TOC_CQ1 = 0x1,
};

enum class EVENT4_Bit : uint8_t {
  CQ_SNS4 = (0x1 << 7),
  CQ_SNS3 = (0x1 << 6),
  CQ_SNS2 = (0x1 << 5),
  CQ_SNS1 = (0x1 << 4),
  CSENSE4 = (0x1 << 3),
  CSENSE3 = (0x1 << 2),
  CSENSE2 = (0x1 << 1),
  CSENSE1 = 0x1,
};

enum class STATUS1_Bit : uint8_t {
  OT = (0x1 << 7),
  WU_COOL = (0x1 << 6),
  UVLO_VDD = (0x1 << 5),
  OV_VDD = (0x1 << 4),
  OC_LP4 = (0x1 << 3),
  OC_LP3 = (0x1 << 2),
  OC_LP2 = (0x1 << 1),
  OC_LP1 = 0x1,
};

enum class STATUS2_Bit : uint8_t {
  PWRGD4 = (0x1 << 7),
  PWRGD3 = (0x1 << 6),
  PWRGD2 = (0x1 << 5),
  PWRGD1 = (0x1 << 4),
  OC_CQ4 = (0x1 << 3),
  OC_CQ3 = (0x1 << 2),
  OC_CQ2 = (0x1 << 1),
  OC_OQ1 = 0x1,
};

enum class MODE1_Bit : uint8_t {
  _24VMODE = (0x1 << 7),
  CSENSE_MODE = (0x1 << 6),
  _2XPTC = (0x3 << 4),
  FLDBK_MODE = (0x1 << 3),
  RETRY_OV = (0x1 << 2),
  RETRY_LP = (0x1 << 1),
  RETRY_CQ = 0x1,
};

enum class MODE2_Bit : uint8_t {
  SLEW4 = (0x1 << 7),
  SLEW3 = (0x1 << 6),
  SLEW2 = (0x1 << 5),
  SLEW1 = (0x1 << 4),
  OV_TH = (0x3 << 2),
  OV_ALLOW = (0x1 << 1),
  CQ_SNS_MODE = 0x1,
};

enum class NSF_Bit : uint8_t {
  NSF4 = (0x3 << 6),
  NSF3 = (0x3 << 4),
  NSF2 = (0x3 << 2),
  NSF1 = 0x3,
};

enum class ILLM_Bit : uint8_t {
  ILLM4 = (0x3 << 6),
  ILLM3 = (0x3 << 4),
  ILLM2 = (0x3 << 2),
  ILLM1 = 0x3,
};

enum class TMRCTRL_Bit : uint8_t {
  LPTC = (0xF << 4),
  RETRYTC = 0x07,
};

enum class CTRL1_Bit : uint8_t {
  WKUP4 = (0x1 << 7),
  WKUP3 = (0x1 << 6),
  WKUP2 = (0x1 << 5),
  WKUP1 = (0x1 << 4),
  DRVEN4 = (0x1 << 3),
  DRVEN3 = (0x1 << 2),
  DRVEN2 = (0x1 << 1),
  DRVEN1 = 0x1,
};

enum class CTRL2_Bit : uint8_t {
  ENLP4 = (0x1 << 7),
  ENLP3 = (0x1 << 6),
  ENLP2 = (0x1 << 5),
  ENLP1 = (0x1 << 4),
  SIO_MODE4 = (0x1 << 3),
  SIO_MODE3 = (0x1 << 2),
  SIO_MODE2 = (0x1 << 1),
  SIO_MODE1 = 0x1,
};

class LTC2874 {
public:
  LTC2874(SPI_HandleTypeDef *hspi) : hspi_(hspi) {}

  uint8_t read(Register reg) {
    uint8_t command = iolink::to_underlying(Command::READ) | (iolink::to_underlying(reg) << 1);
    uint8_t tx[2] = {0, command};
    uint8_t rx[2] = {};
    HAL_SPI_TransmitReceive(hspi_, tx, rx, 1, HAL_MAX_DELAY);
    return rx[0];
  }

  void write(Register reg, uint8_t data, bool update = false) {
    uint8_t command =
        iolink::to_underlying(update ? Command::WRTUPD : Command::WRITE) | (iolink::to_underlying(reg) << 1);
    uint8_t tx[] = {data, command};
    HAL_SPI_Transmit(hspi_, tx, 1, HAL_MAX_DELAY);
  }

  void update() {
    uint8_t tx[] = {0x0, iolink::to_underlying(Command::UPDATE)};
    HAL_SPI_Transmit(hspi_, tx, 1, HAL_MAX_DELAY);
  }

  void reset() {
    uint8_t tx[] = {0x0, iolink::to_underlying(Command::RESET)};
    HAL_SPI_Transmit(hspi_, tx, 1, HAL_MAX_DELAY);
  }

  template <class Bit> void write_value(Register reg, Bit bit, uint8_t value, bool update = false) {
    uint8_t data = read(reg);
    uint8_t mask = iolink::to_underlying(bit);
    data &= ~mask;
    for (uint8_t i = 0; i < 8; ++i) {
      if (mask & (1 << i)) {
        data |= value << i;
        break;
      }
    }
    write(reg, data, update);
  }

private:
  SPI_HandleTypeDef *hspi_;
};

} // namespace ltc2874
