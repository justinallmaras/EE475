#include <msp430.h>
#include "msprf24.h"
#include "nrf_userconfig.h"
#include "stdint.h"

volatile unsigned int user;

int main()
{
    uint8_t addr[5];
//    __delay_cycles(800000);
    WDTCTL = WDTHOLD | WDTPW;
    DCOCTL = CALDCO_16MHZ;
    BCSCTL1 = CALBC1_16MHZ;
    BCSCTL2 = DIVS_1;  // SMCLK = DCOCLK/2
    // SPI (USCI) uses SMCLK, prefer SMCLK < 10MHz (SPI speed limit for nRF24 = 10MHz)

    // Red, Green LED used for status
    P1DIR |= 0x01;



//    user = 0xFE;

    /* Initial values for nRF24L01+ library config variables */
    rf_crc = RF24_EN_CRC | RF24_CRCO; // CRC enabled, 16-bit
    rf_addr_width      = 5;
    rf_speed_power     = RF24_SPEED_1MBPS | RF24_POWER_MAX;
    rf_channel         = 0x76;


    msprf24_init();  // All RX pipes closed by default
    msprf24_set_pipe_packetsize(0, 0);
    msprf24_open_pipe(0, 0);  // Open pipe#0 with Enhanced ShockBurst enabled for receiving Auto-ACKs
        // Note: Pipe#0 is hardcoded in the transceiver hardware as the designated "pipe" for a TX node to receive
        // auto-ACKs.  This does not have to match the pipe# used on the RX side.

    // Transmit to 'rad01' (0x72 0x61 0x64 0x30 0x31)
    msprf24_standby();
    user = msprf24_current_state();
    addr[0] = 0xF0; addr[1] = 0xF0; addr[2] = 0xF0; addr[3] = 0xF0; addr[4] = 0xE1;
    w_tx_addr(addr);
    w_rx_addr(0, addr);  // Pipe 0 receives auto-ack's, autoacks are sent back to the TX addr so the PTX node
                         // needs to listen to the TX addr on pipe#0 to receive them.

    if(msprf24_is_alive()) {
        P1OUT |= 0x01;
    } else {
        while(1);
    }
    uint8_t rxBuf[32] = {0};
    uint8_t mode = 0;
    while(1){
        if(mode == 0) {
            char buf[10] = "hello";
            buf[6] = '\0';
            w_tx_payload(6 , buf);
            msprf24_activate_tx();
            P1OUT |= 0x01;
            mode = 1;
        } else {
            if(msprf24_rx_pending()) {
                r_rx_payload(32 , rxBuf);
                if(rxBuf[0] == 'W') {
                    P1OUT &= ~0x01;
                }

                mode = 0;
            }

        }

        __delay_cycles(80000000);
    }
    return 0;
}