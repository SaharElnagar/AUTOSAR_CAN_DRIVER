/*
 * NVICInterruptHandler.h
 *
 *  Created on: Aug 13, 2017
 *      Author: Muhammad Ali
 */


#ifndef DRIVERS_SL_NVICINTERRUPTHANDLER_H_
#define DRIVERS_SL_NVICINTERRUPTHANDLER_H_


/** \brief Enumerate the interrupt array index */
typedef enum interruptIndx{
GPIOA_IRQ,                      // GPIO Port A
GPIOB_IRQ,                      // GPIO Port B
GPIOC_IRQ,                      // GPIO Port C
GPIOD_IRQ,                      // GPIO Port D
GPIOE_IRQ,                      // GPIO Port E
UART0_IRQ,                      // UART0 Rx and Tx
UART1,                      // UART1 Rx and Tx
SSI0,                      // SSI0 Rx and Tx
I2C0,                      // I2C0 Master and Slave
PWM_FAULT,                      // PWM Fault
PWM_GENERATOR0,                      // PWM Generator 0
PWM_GENERATOR1,                      // PWM Generator 1
PWM_GENERATOR2,                      // PWM Generator 2
QUADRATURE_ENCODER0,                      // Quadrature Encoder 0
ADC_SEQUENCE0,                      // ADC Sequence 0
ADC_SEQUENCE1,                      // ADC Sequence 1
ADC_SEQUENCE2,                      // ADC Sequence 2
ADC_SEQUENCE3,                      // ADC Sequence 3
WATCHDOG_TIMER,                      // Watchdog timer
TIMER0_SUBTIMER_A,                      // Timer 0 subtimer A
TIMER0_SUBTIMER_B,                      // Timer 0 subtimer B
TIMER1_SUBTIMER_A,                      // Timer 1 subtimer A
TIMER1_SUBTIMER_B,                      // Timer 1 subtimer B
TIMER2_SUBTIMER_A,                      // Timer 2 subtimer A
TIMER2_SUBTIMER_B,                      // Timer 2 subtimer B
ANALOG_COMPARATOR_0,                      // Analog Comparator 0
ANALOG_COMPARATOR_1,                      // Analog Comparator 1
ANALOG_COMPARATOR_2,                      // Analog Comparator 2
SYSTEM_CONTROL_PLL_OSC_BO,                      // System Control (PLL, OSC, BO)
FLASH_CONROL,                      // FLASH Control
GPIOF_IRQ,                      // GPIO Port F
GPIOG,                      // GPIO Port G
GPIOH,                      // GPIO Port H
UART2,                      // UART2 Rx and Tx
SSI1,                      // SSI1 Rx and Tx
TIMER3_SUBTIMER_A,                      // Timer 3 subtimer A
TIMER3_SUBTIMER_B,                      // Timer 3 subtimer B
I2C1,                      // I2C1 Master and Slave
QUADRATURE_ENCODER1,                      // Quadrature Encoder 1
CAN0,                      // CAN0
CAN1,                      // CAN1
HIBERNATE,                      // Hibernate
USB0,                      // USB0
PWM_GENERATOR3,                      // PWM Generator 3
UDMA_SW_TRANSFER,                      // uDMA Software Transfer
UDMA_ERROR,                      // uDMA Error
ADC1_SEQUENCE0,                      // ADC1 Sequence 0
ADC1_SEQUENCE1,                      // ADC1 Sequence 1
ADC1_SEQUENCE2,                      // ADC1 Sequence 2
ADC1_SEQUENCE3,                      // ADC1 Sequence 3
GPIOJ,                      // GPIO Port J
GPIOK,                      // GPIO Port K
GPIOL,                      // GPIO Port L
SSI2,                      // SSI2 Rx and Tx
SSI3,                      // SSI3 Rx and Tx
UART3,                      // UART3 Rx and Tx
UART4,                      // UART4 Rx and Tx
UART5,                      // UART5 Rx and Tx
UART6,                      // UART6 Rx and Tx
UART7,                      // UART7 Rx and Tx
I2C2,                      // I2C2 Master and Slave
I2C3,                      // I2C3 Master and Slave
TIMER4_SUBTIMER_A,                      // Timer 4 subtimer A
TIMER4_SUBTIMER_B,                      // Timer 4 subtimer B
TIMER5_SUBTIMER_A,                      // Timer 5 subtimer A
TIMER5_SUBTIMER_B,                      // Timer 5 subtimer B
WIDE_TIMER0_SUBTIMER_A,                      // Wide Timer 0 subtimer A
WIDE_TIMER0_SUBTIMER_B,                      // Wide Timer 0 subtimer B
WIDE_TIMER1_SUBTIMER_A,                      // Wide Timer 1 subtimer A
WIDE_TIMER1_SUBTIMER_B,                      // Wide Timer 1 subtimer B
WIDE_TIMER2_SUBTIMER_A,                      // Wide Timer 2 subtimer A
WIDE_TIMER2_SUBTIMER_B,                      // Wide Timer 2 subtimer B
WIDE_TIMER3_SUBTIMER_A,                      // Wide Timer 3 subtimer A
WIDE_TIMER3_SUBTIMER_B,                      // Wide Timer 3 subtimer B
WIDE_TIMER4_SUBTIMER_A,                      // Wide Timer 4 subtimer A
WIDE_TIMER4_SUBTIMER_B,                      // Wide Timer 4 subtimer B
WIDE_TIMER5_SUBTIMER_A,                      // Wide Timer 5 subtimer A
WIDE_TIMER5_SUBTIMER_B,                      // Wide Timer 5 subtimer B
FPU,                      // FPU
I2C4,                      // I2C4 Master and Slave
I2C5,                      // I2C5 Master and Slave
GPIOM,                      // GPIO Port M
GPION,                      // GPIO Port N
QUADRATURE_ENCODER2,                      // Quadrature Encoder 2
GPIOP,                      // GPIO Port P (Summary or P0)
GPIOP1,                      // GPIO Port P1
GPIOP2,                      // GPIO Port P2
GPIOP3,                      // GPIO Port P3
GPIOP4,                      // GPIO Port P4
GPIOP5,                      // GPIO Port P5
GPIOP6,                      // GPIO Port P6
GPIOP7,                      // GPIO Port P7
GPIOQ,                      // GPIO Port Q (Summary or Q0)
GPIOQ1,                      // GPIO Port Q1
GPIOQ2,                      // GPIO Port Q2
GPIOQ3,                      // GPIO Port Q3
GPIOQ4,                      // GPIO Port Q4
GPIOQ5,                      // GPIO Port Q5
GPIOQ6,                      // GPIO Port Q6
GPIOQ7,                      // GPIO Port Q7
GPIOR,                      // GPIO Port R
GPIOS,                      // GPIO Port S
PWM1_GENERATOR_0,                      // PWM 1 Generator 0
PWM1_GENERATOR_1,                      // PWM 1 Generator 1
PWM1_GENERATOR_2,                      // PWM 1 Generator 2
PWM1_GENERATOR_3,                      // PWM 1 Generator 3
PWM1_FAULT                      // PWM 1 Fault
}g_ISRVirIdx_t;



void installNIVCISRFunction(g_ISRVirIdx_t idx, void (*isr)(void));
void deInstallNIVCISRFunction(g_ISRVirIdx_t idx);
void IntVirDefaultFun(void);

#endif /* DRIVERS_SL_NVICINTERRUPTHANDLER_H_ */
