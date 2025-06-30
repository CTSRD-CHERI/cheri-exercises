#include <stdint.h>
#include <microkit.h>
#include "printf.h"

// This variable will have the address of the UART device
uintptr_t uart_base_vaddr;

/* QEMU RISC-V virt emulates a 16550 compatible UART. */
#define BIT(n) (1ul<<(n))

#define UART_IER_ERBFI   BIT(0)   /* Enable Received Data Available Interrupt */
#define UART_IER_ETBEI   BIT(1)   /* Enable Transmitter Holding Register Empty Interrupt */
#define UART_IER_ELSI    BIT(2)   /* Enable Receiver Line Status Interrupt */
#define UART_IER_EDSSI   BIT(3)   /* Enable MODEM Status Interrupt */

#define UART_FCR_ENABLE_FIFOS   BIT(0)
#define UART_FCR_RESET_RX_FIFO  BIT(1)
#define UART_FCR_RESET_TX_FIFO  BIT(2)
#define UART_FCR_TRIGGER_1      (0u << 6)
#define UART_FCR_TRIGGER_4      (1u << 6)
#define UART_FCR_TRIGGER_8      (2u << 6)
#define UART_FCR_TRIGGER_14     (3u << 6)

#define UART_LCR_DLAB    BIT(7)   /* Divisor Latch Access */

#define UART_LSR_DR      BIT(0)   /* Data Ready */
#define UART_LSR_THRE    BIT(5)   /* Transmitter Holding Register Empty */

typedef volatile struct {
    //uint8_t unused_offset[0x1000];

    uint8_t rbr_dll_thr; /* 0x00 Receiver Buffer Register (Read Only)
                           *   Divisor Latch (LSB)
                           *   Transmitter Holding Register (Write Only)
                           */
    uint8_t dlm_ier;     /* 0x04 Divisor Latch (MSB)
                           *   Interrupt Enable Register
                           */
    uint8_t iir_fcr;     /* 0x08 Interrupt Identification Register (Read Only)
                           *    FIFO Control Register (Write Only)
                           */
    uint8_t lcr;         /* 0xC Line Control Register */
    uint8_t mcr;         /* 0x10 MODEM Control Register */
    uint8_t lsr;         /* 0x14 Line Status Register */
    uint8_t msr;         /* 0x18 MODEM Status Register */
} uart_regs_t;

#define REG_PTR(base, offset) ((volatile uint32_t *)((base) + (offset)))
/*
 *******************************************************************************
 * UART access primitives
 *******************************************************************************
 */

static int internal_uart_is_tx_empty(uart_regs_t *regs)
{
    /* The THRE bit is set when the FIFO is fully empty. On real hardware, there
     * seems no way to detect if the FIFO is partially empty only, so we can't
     * implement a "tx_ready" check. Since QEMU does not emulate a FIFO, this
     * does not really matter.
     */
    return (0 != (regs->lsr & UART_LSR_THRE));
}

static void internal_uart_tx_byte(uart_regs_t *regs, uint8_t byte)
{
    /* Caller has to ensure TX FIFO is ready */
    regs->rbr_dll_thr = byte;
}

static int internal_uart_is_rx_empty(uart_regs_t *regs)
{
    return (0 == (regs->lsr & UART_LSR_DR));
}


static int internal_uart_rx_byte(uart_regs_t *regs)
{
    /* Caller has to ensure RX FIFO has data */
    return regs->rbr_dll_thr;
}

void uart_init() {
    uart_regs_t *regs = (uart_regs_t *) uart_base_vaddr;
    regs->dlm_ier = 0; // disable interrupts

    /* Baudrates and serial line parameters are not emulated by QEMU, so the
     * divisor is just a dummy.
     */
    uint16_t clk_divisor = 1; /* dummy, would be for 115200 baud */
    regs->lcr = UART_LCR_DLAB; /* baud rate divisor setup */
    regs->dlm_ier = (clk_divisor >> 8) & 0xFF;
    regs->rbr_dll_thr = clk_divisor & 0xFF;
    regs->lcr = 0x03; /* set 8N1, clear DLAB to end baud rate divisor setup */

    /* enable and reset FIFOs, interrupt for each byte */
    regs->iir_fcr = UART_FCR_ENABLE_FIFOS
                    | UART_FCR_RESET_RX_FIFO
                    | UART_FCR_RESET_TX_FIFO
                    | UART_FCR_TRIGGER_1;

    /* enable RX interrupts */
    regs->dlm_ier = UART_IER_ERBFI;
}

void uart_put_char(int c) {
    uart_regs_t *regs = (uart_regs_t *) uart_base_vaddr;

    /* There is no way to check for "TX ready", the only thing we have is a
     * check for "TX FIFO empty". This is not optimal, as we might wait here
     * even if there is space in the FIFO. Seems the 16550 was built based on
     * the idea that software keeps track of the FIFO usage. A driver would
     * know how much space is left in the FIFO, so it can write new data
     * either immediately or buffer it. If the FIFO empty interrupt arrives,
     * data can be written from the buffer to fill the FIFO.
     * However, since QEMU does not emulate a FIFO, we can just implement a
     * simple model here and block - expecting to never block practically.
     */
    while (!internal_uart_is_tx_empty(regs)) {
        /* busy waiting loop */
    }

    /* Extract the byte to send, drop any flags. */
    uint8_t byte = (uint8_t)c;

    internal_uart_tx_byte(regs, byte);

    //return byte;
}

void uart_handle_irq() {
}

void uart_put_str(char *str) {
    while (*str) {
        uart_put_char(*str);
        str++;
    }
}

int uart_get_char() {
    uart_regs_t *regs = (uart_regs_t *) uart_base_vaddr;

    /* if UART is empty return an error */
    while(internal_uart_is_rx_empty(regs));

    return internal_uart_rx_byte(regs) & 0xFF;
}

void init(void) {
    // First we initialise the UART device, which will write to the
    // device's hardware registers. Which means we need access to
    // the UART device.
    uart_init();
    // After initialising the UART, print a message to the terminal
    // saying that the serial server has started.
    uart_put_str("SERIAL SERVER: starting\n");
}

#define UART_IRQ_CH 0
#define CLIENT_CH 2

uintptr_t serial_to_client_vaddr;
uintptr_t client_to_serial_vaddr;

microkit_msginfo protected(microkit_channel channel, microkit_msginfo msginfo)
{
    switch (channel) {
        case CLIENT_CH: {
            ((char *)serial_to_client_vaddr)[0] = (char) uart_get_char();
            return microkit_msginfo_new(0, 1);
            break;
        }
    }
    return microkit_msginfo_new(0, 0);
}

void notified(microkit_channel channel) {
    switch (channel) {
        /*
        case UART_IRQ_CH:
            ((char *)serial_to_client_vaddr)[0] = uart_get_char();
            uart_handle_irq();
            microkit_irq_ack(channel);
            microkit_notify(CLIENT_CH);
            break;
         */
        case CLIENT_CH:
            uart_put_str((char *)client_to_serial_vaddr);
            break;
    }
}
