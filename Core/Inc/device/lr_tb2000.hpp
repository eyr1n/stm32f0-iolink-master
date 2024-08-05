#pragma once

#include <array>

#include "stm32f0xx_hal.h"

#include "iolink/iolink_port.hpp"

class LR_TB2000 : public iolink::IOLinkPort {
public:
  using iolink::IOLinkPort::IOLinkPort;

  void pre_operate() override {
    // wakeup request
    while (true) {
      constexpr auto mseq =
          iolink::create_mseq<0>(iolink::MSeqRW::READ, iolink::MSeqChannel::PAGE, 0x02, iolink::MSeqType::TYPE_0, {});
      std::array<uint8_t, 2> res;
      wakeup_request();
      mseq_transmit(mseq);
      HAL_Delay(10);
      if (mseq_receive(res)) {
        break;
      }
    }

    // set master ident
    while (true) {
      constexpr auto mseq = iolink::create_mseq<1>(iolink::MSeqRW::WRITE, iolink::MSeqChannel::PAGE, 0x00,
                                                   iolink::MSeqType::TYPE_0, {0x95});
      std::array<uint8_t, 1> res;
      mseq_transmit(mseq);
      HAL_Delay(10);
      if (mseq_receive(res)) {
        break;
      }
    }

    // pre operate mode
    while (true) {
      constexpr auto mseq = iolink::create_mseq<1>(iolink::MSeqRW::WRITE, iolink::MSeqChannel::PAGE, 0x00,
                                                   iolink::MSeqType::TYPE_0, {0x9A});
      std::array<uint8_t, 1> res;
      mseq_transmit(mseq);
      HAL_Delay(10);
      if (mseq_receive(res)) {
        break;
      }
    }
  }

  void operate() override {
    // set master cycle time
    while (true) {
      constexpr auto mseq = iolink::create_mseq<1>(iolink::MSeqRW::WRITE, iolink::MSeqChannel::PAGE, 0x01,
                                                   iolink::MSeqType::TYPE_0, {iolink::us_to_cycle_time(5000)});
      std::array<uint8_t, 1> res;
      mseq_transmit(mseq);
      HAL_Delay(10);
      if (mseq_receive(res)) {
        break;
      }
    }

    // operate mode
    while (true) {
      constexpr auto mseq = iolink::create_mseq<1>(iolink::MSeqRW::WRITE, iolink::MSeqChannel::PAGE, 0x00,
                                                   iolink::MSeqType::TYPE_0, {0x99});
      std::array<uint8_t, 1> res;
      mseq_transmit(mseq);
      HAL_Delay(10);
      if (mseq_receive(res)) {
        break;
      }
    }
  }

  void pd_read() override {
    constexpr auto mseq_pd =
        iolink::create_mseq<0>(iolink::MSeqRW::READ, iolink::MSeqChannel::PROCESS, 0x00, iolink::MSeqType::TYPE_2, {});
    mseq_transmit(mseq_pd);
  }

  void pd_reply() override {
    if (mseq_receive(process_data_)) {
      distance_ = (process_data_[1] << 8) | process_data_[2];
    }
  }

  const uint16_t &distance() { return distance_; }

private:
  std::array<uint8_t, 4> process_data_;
  uint16_t distance_;
};
