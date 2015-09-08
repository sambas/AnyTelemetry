/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for AnyTelemetry board.
 */

/*
 * Board identifier.
 */
#define BOARD_STM32373C
#define BOARD_NAME                  "AnyTelemetry"

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768
#endif

#define STM32_LSEDRV                (3 << 3)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000
#endif

/*
 * MCU type as defined in the ST header.
 */
#define STM32F37X

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0 		          0
#define GPIOA_PIN1             	  1
#define GPIOA_USART2_TX            2
#define GPIOA_USART2_RX            3
#define GPIOA_PIN4                  4
#define GPIOA_PIN5                  5
#define GPIOA_PIN6                  6
#define GPIOA_PIN7             7
#define GPIOA_USB_DETECT              8
#define GPIOA_USART1_TX              9
#define GPIOA_USART1_RX              10
#define GPIOA_USB_DM                11
#define GPIOA_USB_DP                12
#define GPIOA_SWDIO                 13
#define GPIOA_SWCLK                 14
#define GPIOA_JTDI                  15

#define GPIOB_LED0                	0
#define GPIOB_LED1            		1
#define GPIOB_PIN2                  2
#define GPIOB_PIN3                   3
#define GPIOB_JTRST                 4
#define GPIOB_PIN5                  5
#define GPIOB_PIN6              6
#define GPIOB_PIN7              7
#define GPIOB_CAN_RX                8
#define GPIOB_CAN_TX                9
#define GPIOB_PIN10                 10
#define GPIOB_PIN11                 11
#define GPIOB_PIN12                 12
#define GPIOB_PIN13                 13
#define GPIOB_PIN14                 14
#define GPIOB_PIN15                 15

#define GPIOC_PIN0                  0
#define GPIOC_PIN1                  1
#define GPIOC_PIN2                  2
#define GPIOC_PIN3                  3
#define GPIOC_PIN4                  4
#define GPIOC_PIN5			        5
#define GPIOC_PIN6                  6
#define GPIOC_PIN7                  7
#define GPIOC_PIN8                  8
#define GPIOC_PIN9                  9
#define GPIOC_PIN10              10
#define GPIOC_PIN11             11
#define GPIOC_PIN12             12
#define GPIOC_PIN13                 13
#define GPIOC_OSC32_IN              14
#define GPIOC_OSC32_OUT             15

#define GPIOD_PIN0                0
#define GPIOD_PIN1                1
#define GPIOD_PIN2                2
#define GPIOD_PIN3            3
#define GPIOD_PIN4            4
#define GPIOD_PIN5             5
#define GPIOD_PIN6             6
#define GPIOD_PIN7                  7
#define GPIOD_USART3_TX             8
#define GPIOD_USART3_RX             9
#define GPIOD_PIN10                 10
#define GPIOD_PIN11             11
#define GPIOD_PIN12                 12
#define GPIOD_PIN13                 13
#define GPIOD_PIN14                 14
#define GPIOD_PIN15                 15

#define GPIOE_PIN0                  0
#define GPIOE_PIN1                  1
#define GPIOE_PIN2                 2
#define GPIOE_PIN3             3
#define GPIOE_PIN4                  4
#define GPIOE_PIN5                  5
#define GPIOE_PIN6               6
#define GPIOE_PIN7                7
#define GPIOE_PIN8             8
#define GPIOE_PIN9             9
#define GPIOE_PIN10                 10
#define GPIOE_PIN11                 11
#define GPIOE_PIN12                 12
#define GPIOE_PIN13                 13
#define GPIOE_PIN14         14
#define GPIOE_PIN15                 15

#define GPIOF_OSC_IN                0
#define GPIOF_OSC_OUT               1
#define GPIOF_PIN2              	2
#define GPIOF_PIN3                  3
#define GPIOF_PIN4              	4
#define GPIOF_PIN5                  5
#define GPIOF_PIN6                  6
#define GPIOF_USB_DISCONNECT        7
#define GPIOF_PIN8                  8
#define GPIOF_PIN9             		9
#define GPIOF_PIN10                	10
#define GPIOF_PIN11                 11
#define GPIOF_PIN12                 12
#define GPIOF_PIN13                 13
#define GPIOF_PIN14                 14
#define GPIOF_PIN15                 15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 * PA0  - WKUP_BUTTON               (input floating).
 * PA1  - LDR_OUT                   (analog).
 * PA2  - KEY_BUTTON                (input floating).
 * PA3  - PIN3                      (input pullup).
 * PA4  - PIN4                      (input pullup).
 * PA5  - PIN5                      (input floating).
 * PA6  - PIN6                      (input pullup).
 * PA7  - COMP2_OUT                 (output pushpull maximum).
 * PA8  - I2C2_SMB                  (input floating).
 * PA9  - I2C2_SCL                  (alternate 4).
 * PA10 - I2C2_SDA                  (alternate 4).
 * PA11 - USB_DM                    (alternate 14).
 * PA12 - USB_DP                    (alternate 14).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - JTDI                      (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_PIN0) |    \
                                     PIN_MODE_INPUT(GPIOA_PIN1) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_USART2_TX) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_USART2_RX) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN7) |     \
                                     PIN_MODE_INPUT(GPIOA_USB_DETECT) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_USART1_TX) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_USART1_RX) |   \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DM) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DP) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_JTDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PIN0) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USART2_TX) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USART2_RX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN7) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DETECT) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USART1_TX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USART1_RX) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DM) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DP) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIOA_PIN0) |     \
                                     PIN_OSPEED_2M(GPIOA_PIN1) |         \
                                     PIN_OSPEED_100M(GPIOA_USART2_TX) |      \
                                     PIN_OSPEED_100M(GPIOA_USART2_RX) |            \
                                     PIN_OSPEED_2M(GPIOA_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOA_PIN5) |          \
                                     PIN_OSPEED_2M(GPIOA_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOA_PIN7) |     \
                                     PIN_OSPEED_2M(GPIOA_USB_DETECT) |        \
                                     PIN_OSPEED_100M(GPIOA_USART1_TX) |      \
                                     PIN_OSPEED_100M(GPIOA_USART1_RX) |      \
                                     PIN_OSPEED_100M(GPIOA_USB_DM) |        \
                                     PIN_OSPEED_100M(GPIOA_USB_DP) |          \
                                     PIN_OSPEED_100M(GPIOA_SWDIO) |         \
                                     PIN_OSPEED_100M(GPIOA_SWCLK) |         \
                                     PIN_OSPEED_100M(GPIOA_JTDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_PIN0) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |    \
                                     PIN_PUPDR_FLOATING(GPIOA_USART2_TX) | \
                                     PIN_PUPDR_FLOATING(GPIOA_USART2_RX) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN6) |         \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN7) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DETECT) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_USART1_TX) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_USART1_RX) |   \
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DM) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DP) |     \
                                     PIN_PUPDR_PULLUP(GPIOA_SWDIO) |        \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_JTDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PIN0) |      \
                                     PIN_ODR_HIGH(GPIOA_PIN1) |          \
                                     PIN_ODR_HIGH(GPIOA_USART2_TX) |       \
                                     PIN_ODR_HIGH(GPIOA_USART2_RX) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOA_PIN7) |         \
                                     PIN_ODR_HIGH(GPIOA_USB_DETECT) |         \
                                     PIN_ODR_HIGH(GPIOA_USART1_TX) |         \
                                     PIN_ODR_HIGH(GPIOA_USART1_RX) |         \
                                     PIN_ODR_HIGH(GPIOA_USB_DM) |           \
                                     PIN_ODR_HIGH(GPIOA_USB_DP) |           \
                                     PIN_ODR_HIGH(GPIOA_SWDIO) |            \
                                     PIN_ODR_HIGH(GPIOA_SWCLK) |            \
                                     PIN_ODR_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PIN0, 0) |    \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0) |        \
                                     PIN_AFIO_AF(GPIOA_USART2_TX, 7) |     \
                                     PIN_AFIO_AF(GPIOA_USART2_RX, 7) |           \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOA_PIN7, 0))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_USB_DETECT, 0) |       \
                                     PIN_AFIO_AF(GPIOA_USART1_TX, 7) |       \
                                     PIN_AFIO_AF(GPIOA_USART1_RX, 7) |       \
                                     PIN_AFIO_AF(GPIOA_USB_DM, 14) |        \
                                     PIN_AFIO_AF(GPIOA_USB_DP, 14) |        \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0) |          \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0) |          \
                                     PIN_AFIO_AF(GPIOA_JTDI, 0))

/*
 * GPIOB setup:
 *
 * PB0  - MIC_IN                    (analog).
 * PB1  - ADC_POT_IN                (analog).
 * PB2  - PIN2                      (input pullup).
 * PB3  - SWO                       (alternate 0).
 * PB4  - JTRST                     (input floating).
 * PB5  - PIN5                      (input pullup).
 * PB6  - I2C1_SCL                  (alternate 4).
 * PB7  - I2C1_SDA                  (alternate 4).
 * PB8  - PIN8                      (input pullup).
 * PB9  - PIN9                      (input pullup).
 * PB10 - PIN10                     (input pullup).
 * PB11 - PIN11                     (input pullup).
 * PB12 - PIN12                     (input pullup).
 * PB13 - PIN13                     (input pullup).
 * PB14 - PIN14                     (input pullup).
 * PB15 - PIN15                     (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_OUTPUT(GPIOB_LED0) |        \
                                     PIN_MODE_OUTPUT(GPIOB_LED1) |    \
                                     PIN_MODE_INPUT(GPIOB_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN3) |        \
                                     PIN_MODE_INPUT(GPIOB_JTRST) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN6) |   \
                                     PIN_MODE_INPUT(GPIOB_PIN7) |   \
                                     PIN_MODE_ALTERNATE(GPIOB_CAN_RX) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_CAN_TX) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_LED0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED1) | \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN3) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTRST) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN_RX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_CAN_TX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_2M(GPIOB_LED0) |          \
                                     PIN_OSPEED_2M(GPIOB_LED1) |      \
                                     PIN_OSPEED_2M(GPIOB_PIN2) |            \
                                     PIN_OSPEED_2M(GPIOB_PIN3) |           \
                                     PIN_OSPEED_100M(GPIOB_JTRST) |         \
                                     PIN_OSPEED_2M(GPIOB_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOB_PIN6) |      \
                                     PIN_OSPEED_2M(GPIOB_PIN7) |      \
                                     PIN_OSPEED_100M(GPIOB_CAN_RX) |            \
                                     PIN_OSPEED_100M(GPIOB_CAN_TX) |            \
                                     PIN_OSPEED_2M(GPIOB_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOB_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOB_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOB_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOB_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_LED0) |     \
                                     PIN_PUPDR_FLOATING(GPIOB_LED1) | \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN3) |        \
                                     PIN_PUPDR_FLOATING(GPIOB_JTRST) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |   \
                                     PIN_PUPDR_FLOATING(GPIOB_CAN_RX) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_CAN_TX) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_LED0) |           \
                                     PIN_ODR_HIGH(GPIOB_LED1) |       \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN3) |              \
                                     PIN_ODR_HIGH(GPIOB_JTRST) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN6) |         \
                                     PIN_ODR_HIGH(GPIOB_PIN7) |         \
                                     PIN_ODR_HIGH(GPIOB_CAN_RX) |             \
                                     PIN_ODR_HIGH(GPIOB_CAN_TX) |             \
                                     PIN_ODR_HIGH(GPIOB_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_LED0, 0) |         \
                                     PIN_AFIO_AF(GPIOB_LED1, 0) |     \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN3, 0) |            \
                                     PIN_AFIO_AF(GPIOB_JTRST, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0) |       \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_CAN_RX, 9) |           \
                                     PIN_AFIO_AF(GPIOB_CAN_TX, 9) |           \
                                     PIN_AFIO_AF(GPIOB_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0))

/*
 * GPIOC setup:
 *
 * PC0  - LED1                      (output opendrain maximum).
 * PC1  - LED2                      (output opendrain maximum).
 * PC2  - LED3                      (output opendrain maximum).
 * PC3  - LED4                      (output opendrain maximum).
 * PC4  - PIN4                      (input pullup).
 * PC5  - USB_DISCONNECT            (output pushpull maximum).
 * PC6  - PIN6                      (input pullup).
 * PC7  - PIN7                      (input pullup).
 * PC8  - PIN8                      (input pullup).
 * PC9  - PIN9                      (input pullup).
 * PC10 - SPI3_SCK                  (alternate 6).
 * PC11 - SPI3_MISO                 (alternate 6).
 * PC12 - SPI3_MOSI                 (alternate 6).
 * PC13 - PIN13                     (input pullup).
 * PC14 - OSC32_IN                  (input floating).
 * PC15 - OSC32_OUT                 (input floating).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN1) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN2) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN3) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |\
                                     PIN_MODE_INPUT(GPIOC_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN10) |   \
                                     PIN_MODE_INPUT(GPIOC_PIN11) |  \
                                     PIN_MODE_INPUT(GPIOC_PIN12) |  \
                                     PIN_MODE_INPUT(GPIOC_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOC_OSC32_IN) |       \
                                     PIN_MODE_INPUT(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN10) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN11) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN12) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_2M(GPIOC_PIN0) |          \
                                     PIN_OSPEED_2M(GPIOC_PIN1) |          \
                                     PIN_OSPEED_2M(GPIOC_PIN2) |          \
                                     PIN_OSPEED_2M(GPIOC_PIN3) |          \
                                     PIN_OSPEED_2M(GPIOC_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN5) |\
                                     PIN_OSPEED_2M(GPIOC_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN7) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN8) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN9) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN10) |      \
                                     PIN_OSPEED_2M(GPIOC_PIN11) |     \
                                     PIN_OSPEED_2M(GPIOC_PIN12) |     \
                                     PIN_OSPEED_2M(GPIOC_PIN13) |           \
                                     PIN_OSPEED_100M(GPIOC_OSC32_IN) |      \
                                     PIN_OSPEED_100M(GPIOC_OSC32_OUT))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN4) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN5) |\
                                     PIN_PUPDR_FLOATING(GPIOC_PIN6) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN7) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN8) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN9) |         \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN10) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN11) |    \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN12) |  \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN13) |        \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN) |   \
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |   \
                                     PIN_ODR_HIGH(GPIOC_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN10) |         \
                                     PIN_ODR_HIGH(GPIOC_PIN11) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN12) |        \
                                     PIN_ODR_HIGH(GPIOC_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOC_OSC32_IN) |         \
                                     PIN_ODR_HIGH(GPIOC_OSC32_OUT))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0) | \
                                     PIN_AFIO_AF(GPIOC_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN7, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN10, 0) |       \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0) |      \
                                     PIN_AFIO_AF(GPIOC_PIN12, 0) |      \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOC_OSC32_IN, 0) |       \
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT, 0))

/*
 * GPIOD setup:
 *
 * PD0  - CAN_RX                    (alternate 7).
 * PD1  - CAN_TX                    (alternate 7).
 * PD2  - LCD_CS                    (output pushpull maximum).
 * PD3  - USART2_CTS                (alternate 7).
 * PD4  - USART2_RST                (alternate 7).
 * PD5  - USART2_TX                 (alternate 7).
 * PD6  - USART2_RX                 (alternate 7).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - AUDIO_RST                 (output pushpull maximum).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN0) |     \
                                     PIN_MODE_INPUT(GPIOD_PIN1) |     \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |        \
                                     PIN_MODE_INPUT(GPIOD_PIN3) | \
                                     PIN_MODE_INPUT(GPIOD_PIN4) | \
                                     PIN_MODE_INPUT(GPIOD_PIN5) |  \
                                     PIN_MODE_INPUT(GPIOD_PIN6) |  \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |           \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_TX) |           \
                                     PIN_MODE_ALTERNATE(GPIOD_USART3_RX) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN11) |     \
                                     PIN_MODE_INPUT(GPIOD_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) | \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_TX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_USART3_RX) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_2M(GPIOD_PIN0) |        \
                                     PIN_OSPEED_2M(GPIOD_PIN1) |        \
                                     PIN_OSPEED_2M(GPIOD_PIN2) |        \
                                     PIN_OSPEED_2M(GPIOD_PIN3) |    \
                                     PIN_OSPEED_2M(GPIOD_PIN4) |    \
                                     PIN_OSPEED_2M(GPIOD_PIN5) |     \
                                     PIN_OSPEED_2M(GPIOD_PIN6) |     \
                                     PIN_OSPEED_2M(GPIOD_PIN7) |            \
                                     PIN_OSPEED_100M(GPIOD_USART3_TX) |            \
                                     PIN_OSPEED_100M(GPIOD_USART3_RX) |            \
                                     PIN_OSPEED_2M(GPIOD_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOD_PIN11) |     \
                                     PIN_OSPEED_2M(GPIOD_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOD_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOD_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_PIN0) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN1) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN2) |     \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN3) | \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN4) | \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN6) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN7) |         \
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_TX) |         \
                                     PIN_PUPDR_FLOATING(GPIOD_USART3_RX) |         \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN10) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN11) |  \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN12) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN13) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PIN0) |           \
                                     PIN_ODR_HIGH(GPIOD_PIN1) |           \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |           \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |       \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |       \
                                     PIN_ODR_HIGH(GPIOD_PIN5) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN6) |        \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |             \
                                     PIN_ODR_LOW(GPIOD_USART3_TX) |             \
                                     PIN_ODR_LOW(GPIOD_USART3_RX) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN11) |         \
                                     PIN_ODR_HIGH(GPIOD_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 0) |         \
                                     PIN_AFIO_AF(GPIOD_PIN1, 0) |         \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0) |         \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0) |     \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0) |     \
                                     PIN_AFIO_AF(GPIOD_PIN5, 0) |      \
                                     PIN_AFIO_AF(GPIOD_PIN6, 0) |      \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_USART3_TX, 7) |           \
                                     PIN_AFIO_AF(GPIOD_USART3_RX, 7) |           \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0) |      \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (input pullup).
 * PE1  - PIN1                      (input pullup).
 * PE2  - SD_CS                     (output opendrain maximum).
 * PE3  - SD_DETECT                 (input pullup).
 * PE4  - PIN4                      (input pullup).
 * PE5  - PIN5                      (input pullup).
 * PE6  - JOY_SEL                   (input pulldown).
 * PE7  - RTD_IN                    (analog).
 * PE8  - PRESSUREP                 (analog).
 * PE9  - PRESSUREN                 (analog).
 * PE10 - PIN10                     (input pullup).
 * PE11 - PIN11                     (input pullup).
 * PE12 - PIN12                     (input pullup).
 * PE13 - PIN13                     (input pullup).
 * PE14 - PRESSURE_TEPM             (input floating).
 * PE15 - PIN15                     (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN2) |         \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |      \
                                     PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |        \
                                     PIN_MODE_INPUT(GPIOE_PIN7) |        \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |     \
                                     PIN_MODE_INPUT(GPIOE_PIN9) |     \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |  \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_2M(GPIOE_PIN0) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN1) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN2) |         \
                                     PIN_OSPEED_2M(GPIOE_PIN3) |     \
                                     PIN_OSPEED_2M(GPIOE_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN6) |       \
                                     PIN_OSPEED_2M(GPIOE_PIN7) |          \
                                     PIN_OSPEED_2M(GPIOE_PIN8) |     \
                                     PIN_OSPEED_2M(GPIOE_PIN9) |     \
                                     PIN_OSPEED_2M(GPIOE_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN14) |   \
                                     PIN_OSPEED_2M(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_FLOATING(GPIOE_PIN0) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN1) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN3) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN4) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN5) |         \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN6) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8) |  \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN9) |  \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN10) |        \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN11) |        \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN12) |        \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN13) |        \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN14) |\
                                     PIN_PUPDR_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |        \
                                     PIN_ODR_HIGH(GPIOE_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |           \
                                     PIN_ODR_HIGH(GPIOE_PIN8) |        \
                                     PIN_ODR_HIGH(GPIOE_PIN9) |        \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN13) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |     \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0) |      \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0) |        \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0) |      \
                                     PIN_AFIO_AF(GPIOE_PIN9, 0) |      \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0) |  \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 *
 * PF0  - OSC_IN                    (input floating).
 * PF1  - OSC_OUT                   (input floating).
 * PF2  - JOY_DOWN                  (input pulldown).
 * PF3  - PIN3                      (input pullup).
 * PF4  - JOY_LEFT                  (input pulldown).
 * PF5  - PIN5                      (input pullup).
 * PF6  - PIN6                      (input pullup).
 * PF7  - PIN7                      (input pullup).
 * PF8  - PIN8                      (input pullup).
 * PF9  - JOY_RIGHT                 (input pulldown).
 * PF10 - JOY_UP                    (input pulldown).
 * PF11 - PIN11                     (input pullup).
 * PF12 - PIN12                     (input pullup).
 * PF13 - PIN13                     (input pullup).
 * PF14 - PIN14                     (input pullup).
 * PF15 - PIN15                     (input pullup).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOF_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |       \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |       \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_OUTPUT(GPIOF_USB_DISCONNECT) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |      \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |         \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_USB_DISCONNECT) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_100M(GPIOF_OSC_IN) |        \
                                     PIN_OSPEED_100M(GPIOF_OSC_OUT) |       \
                                     PIN_OSPEED_2M(GPIOF_PIN2) |      \
                                     PIN_OSPEED_2M(GPIOF_PIN3) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN4) |      \
                                     PIN_OSPEED_2M(GPIOF_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOF_USB_DISCONNECT) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN8) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN9) |     \
                                     PIN_OSPEED_2M(GPIOF_PIN10) |        \
                                     PIN_OSPEED_2M(GPIOF_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |   \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |         \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |   \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |         \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |         \
                                     PIN_PUPDR_FLOATING(GPIOF_USB_DISCONNECT) |         \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |         \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |  \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |     \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |        \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |        \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |        \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |        \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOF_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |         \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |         \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_USB_DISCONNECT) |   \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |        \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |           \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOF_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |       \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |       \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_USB_DISCONNECT, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |      \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |         \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))


#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
