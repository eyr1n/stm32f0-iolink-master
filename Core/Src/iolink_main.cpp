#include <cstdint>
#include <cstdio>

#include "iolink/iolink_port.hpp"
#include "iolink/ltc2874.hpp"

#include "device/lr_tb2000.hpp"

#include "main.h"

extern TIM_HandleTypeDef htim1;
extern CAN_HandleTypeDef hcan;
extern SPI_HandleTypeDef hspi2;

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;

constexpr uint32_t MASTER_CYCLE_TIME = 3000;

extern "C" void iolink_main() {
  // Initialize STM32 Peripherals
  HAL_TIM_Base_Start(&htim1);
  HAL_CAN_Start(&hcan);

  // Reset LTC2874 Registers
  ltc2874::LTC2874 ltc2874{&hspi2};
  ltc2874.reset();
  HAL_Delay(10);

  // Initialize LR-TB2000
  LR_TB2000 lr_tb2000{&ltc2874, &huart2, TXEN1_GPIO_Port, TXEN1_Pin};
  lr_tb2000.init();
  HAL_Delay(3000);
  lr_tb2000.pre_operate();
  lr_tb2000.operate();

  // CAN Header
  CAN_TxHeaderTypeDef tx_header;
  tx_header.StdId = 0x1;
  tx_header.IDE = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;
  tx_header.DLC = sizeof(uint16_t);
  tx_header.TransmitGlobalTime = DISABLE;
  uint32_t tx_mailbox;

  // Exchange Process Data
  while (true) {
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    lr_tb2000.pd_read();
    while (__HAL_TIM_GET_COUNTER(&htim1) < MASTER_CYCLE_TIME)
      ;
    lr_tb2000.pd_reply();
    uint16_t distance = lr_tb2000.distance();
    printf("%hu\r\n", distance);
    if (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) > 0) {
      HAL_CAN_AddTxMessage(&hcan, &tx_header, reinterpret_cast<const uint8_t *>(&distance), &tx_mailbox);
    }
  }
}
