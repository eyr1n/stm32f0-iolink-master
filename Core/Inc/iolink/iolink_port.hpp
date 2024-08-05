#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "stm32f0xx_hal.h"

#include "iolink_util.hpp"
#include "ltc2874.hpp"

namespace iolink {

class IOLinkPort {
public:
  IOLinkPort(ltc2874::LTC2874 *ltc2874, UART_HandleTypeDef *huart, GPIO_TypeDef *txen_port, uint16_t txen_pin)
      : ltc2874_(ltc2874), huart_(huart), txen_port_(txen_port), txen_pin_(txen_pin) {}

  void init() {
    if (huart_->Instance == USART2) {
      ltc2874_->write_value(ltc2874::Register::CTRL2, ltc2874::CTRL2_Bit::ENLP1, 1);
      if (huart_->Init.BaudRate == COM1_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW1, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF1, 1);
      } else if (huart_->Init.BaudRate == COM2_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW1, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF1, 2);
      } else if (huart_->Init.BaudRate == COM3_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF1, 3);
      }
    } else if (huart_->Instance == USART3) {
      ltc2874_->write_value(ltc2874::Register::CTRL2, ltc2874::CTRL2_Bit::ENLP2, 1);
      if (huart_->Init.BaudRate == COM1_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW2, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF2, 1);
      } else if (huart_->Init.BaudRate == COM2_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW2, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF2, 2);
      } else if (huart_->Init.BaudRate == COM3_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF2, 3);
      }
    } else if (huart_->Instance == USART4) {
      ltc2874_->write_value(ltc2874::Register::CTRL2, ltc2874::CTRL2_Bit::ENLP3, 1);
      if (huart_->Init.BaudRate == COM1_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW3, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF3, 1);
      } else if (huart_->Init.BaudRate == COM2_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW3, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF3, 2);
      } else if (huart_->Init.BaudRate == COM3_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF3, 3);
      }
    } else if (huart_->Instance == USART6) {
      ltc2874_->write_value(ltc2874::Register::CTRL2, ltc2874::CTRL2_Bit::ENLP4, 1);
      if (huart_->Init.BaudRate == COM1_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW4, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF4, 1);
      } else if (huart_->Init.BaudRate == COM2_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::MODE2, ltc2874::MODE2_Bit::SLEW4, 0);
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF4, 2);
      } else if (huart_->Init.BaudRate == COM3_BAUD_RATE) {
        ltc2874_->write_value(ltc2874::Register::NSF, ltc2874::NSF_Bit::NSF4, 3);
      }
    }
    ltc2874_->update();
  }

  void wakeup_request() {
    HAL_UART_DeInit(huart_);
    HAL_GPIO_WritePin(txen_port_, txen_pin_, GPIO_PIN_RESET);
    if (huart_->Instance == USART2) {
      ltc2874_->write_value(ltc2874::Register::EVENT3, ltc2874::EVENT3_Bit::TOC_CQ1, 0, true);
      ltc2874_->write_value(ltc2874::Register::CTRL1, ltc2874::CTRL1_Bit::WKUP1, 1, true);
    } else if (huart_->Instance == USART3) {
      ltc2874_->write_value(ltc2874::Register::EVENT3, ltc2874::EVENT3_Bit::TOC_CQ2, 0, true);
      ltc2874_->write_value(ltc2874::Register::CTRL1, ltc2874::CTRL1_Bit::WKUP2, 1, true);
    } else if (huart_->Instance == USART4) {
      ltc2874_->write_value(ltc2874::Register::EVENT3, ltc2874::EVENT3_Bit::TOC_CQ3, 0, true);
      ltc2874_->write_value(ltc2874::Register::CTRL1, ltc2874::CTRL1_Bit::WKUP3, 1, true);
    } else if (huart_->Instance == USART6) {
      ltc2874_->write_value(ltc2874::Register::EVENT3, ltc2874::EVENT3_Bit::TOC_CQ4, 0, true);
      ltc2874_->write_value(ltc2874::Register::CTRL1, ltc2874::CTRL1_Bit::WKUP4, 1, true);
    }
    HAL_Delay(1);
    HAL_UART_Init(huart_);
    HAL_UART_Receive_DMA(huart_, uart_buf_.data(), uart_buf_.size());
  }

  template <size_t N> void mseq_transmit(const std::array<uint8_t, N> &mseq) {
    uart_advance(uart_available());
    HAL_GPIO_WritePin(txen_port_, txen_pin_, GPIO_PIN_SET);
    HAL_UART_Transmit_DMA(huart_, mseq.data(), N);
    uart_advance(N);
  }

  template <size_t N> bool mseq_receive(std::array<uint8_t, N> &res) {
    if (uart_available() != N) {
      return false;
    }
    for (size_t i = 0; i < N; ++i) {
      res[i] = uart_buf_[uart_read_idx_];
      uart_advance(1);
    }
    uint8_t checksum = res[N - 1];
    res[N - 1] = 0;
    return mseq_checksum(res) == checksum;
  }

  virtual void pre_operate() = 0;

  virtual void operate() = 0;

  virtual void pd_read() = 0;

  virtual void pd_reply() = 0;

private:
  static constexpr uint32_t COM1_BAUD_RATE = 4800;
  static constexpr uint32_t COM2_BAUD_RATE = 38400;
  static constexpr uint32_t COM3_BAUD_RATE = 230400;

  ltc2874::LTC2874 *ltc2874_;
  UART_HandleTypeDef *huart_;
  GPIO_TypeDef *txen_port_;
  uint16_t txen_pin_;

  std::array<uint8_t, 64> uart_buf_;
  size_t uart_read_idx_ = 0;

  size_t uart_available() {
    size_t write_idx = uart_buf_.size() - __HAL_DMA_GET_COUNTER(huart_->hdmarx);
    return (uart_buf_.size() + write_idx - uart_read_idx_) % uart_buf_.size();
  }

  void uart_advance(size_t len) { uart_read_idx_ = (uart_read_idx_ + len) % uart_buf_.size(); }
};

} // namespace iolink
