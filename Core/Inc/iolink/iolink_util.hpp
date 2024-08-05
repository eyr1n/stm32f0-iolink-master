#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace iolink {

enum class MSeqRW : uint8_t {
  WRITE = 0,
  READ = 1,
};

enum class MSeqChannel : uint8_t {
  PROCESS = 0,
  PAGE = 1,
  DIAGNOSTIC = 2,
  ISDU = 3,
};

enum class MSeqType : uint8_t {
  TYPE_0 = 0,
  TYPE_1 = 1,
  TYPE_2 = 2,
};

template <class T> constexpr std::underlying_type_t<T> to_underlying(T value) noexcept {
  return static_cast<std::underlying_type_t<T>>(value);
}

template <size_t N> constexpr uint8_t mseq_checksum(const std::array<uint8_t, N> &mseq) {
  uint8_t checksum = 0x52;
  for (uint8_t e : mseq) {
    checksum ^= e;
  }
  checksum = ((((checksum >> 7) ^ (checksum >> 5) ^ (checksum >> 3) ^ (checksum >> 1)) & 1) << 5) |
             ((((checksum >> 6) ^ (checksum >> 4) ^ (checksum >> 2) ^ checksum) & 1) << 4) |
             ((((checksum >> 7) ^ (checksum >> 6)) & 1) << 3) | ((((checksum >> 5) ^ (checksum >> 4)) & 1) << 2) |
             ((((checksum >> 3) ^ (checksum >> 2)) & 1) << 1) | (((checksum >> 1) ^ checksum) & 1);
  return checksum;
}

template <size_t N>
constexpr std::array<uint8_t, 2 + N> create_mseq(MSeqRW rw, MSeqChannel channel, uint8_t address, MSeqType type,
                                                 const std::array<uint8_t, N> &data) {
  uint8_t mc = (to_underlying(rw) << 7) | (to_underlying(channel) << 5) | (address);
  uint8_t ckt = (to_underlying(type) << 6);
  std::array<uint8_t, 2 + N> mseq = {mc, ckt};
  for (size_t i = 0; i < N; ++i) {
    mseq[2 + i] = data[i];
  }
  uint8_t checksum = mseq_checksum(mseq);
  mseq[1] |= checksum;
  return mseq;
}

inline constexpr uint32_t cycle_time_to_us(uint8_t cycle_time) {
  uint8_t encoding = (cycle_time >> 6) & 0x3;
  uint8_t multiplier = cycle_time & 0x3f;
  uint32_t offset = 0;
  uint32_t time_base = 0;

  switch (encoding) {
  case 0x0:
    offset = 0;
    time_base = 100;
    break;
  case 0x1:
    offset = 6400;
    time_base = 400;
    break;
  case 0x2:
    offset = 32000;
    time_base = 1600;
    break;
  }

  return offset + multiplier * time_base;
}

inline constexpr uint8_t us_to_cycle_time(uint32_t us) {
  uint8_t encoding = 0;
  uint32_t offset = 0;
  uint32_t time_base = 0;

  if (us < 6400) {
    encoding = 0x0;
    offset = 0;
    time_base = 100;
  } else if (us < 32000) {
    encoding = 0x1;
    offset = 6400;
    time_base = 400;
  } else {
    encoding = 0x2;
    offset = 32000;
    time_base = 1600;
  }

  uint8_t multiplier = (us - offset) / time_base;
  return (encoding << 6) | multiplier;
}

} // namespace iolink
