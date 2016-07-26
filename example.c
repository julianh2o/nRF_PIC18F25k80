//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "config.h"
#include "nRF2401.h"
#include <delays.h>

//Configure these to point to something useful
#define STATUS_TRIS TRISBbits.TRISB4
#define STATUS_LED PORTBbits.RB4

#define MODE_SELECT_TRIS TRISBbits.TRISB0
#define MODE_SELECT PORTBbits.RB0
#define MODE_SEND 1

unsigned char tx_buf[MAX_PAYLOAD];
unsigned char rx_buf[MAX_PAYLOAD];

void setup(void);

////                            Sender Code                                 ////
void senderMain(void);

////                          Receiver Code                                 ////
void receiverMain(void);

////                            System Code                                 ////
void run(void);
void main(void);

void setup(void) {
    //NRF port configure (todo: move me)
    TRIS_CE = OUTPUT;
    TRIS_CSN = OUTPUT;
    TRIS_IRQ = INPUT;
    TRIS_SCK = OUTPUT;
    TRIS_MISO = INPUT;
    TRIS_MOSI = OUTPUT;
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Sender Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
void senderMain() {
    nrf_init();
    delay();

    nrf_txmode();
    delay();

    tx_buf[0] = 0;

    while(1) {
        tx_buf[0]++;
        STATUS_LED = nrf_send(&tx_buf, &rx_buf);
        delay();
    }
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Receiver Code                               ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void receiverMain() {
    nrf_init();
    delay();

    nrf_rxmode();
    delay();

    Delay10KTCYx(100);

    while(1) {
        nrf_receive(&rx_buf);
        
        
        //do something to indicate rx_buf[0]?
    }
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Shared Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void delay(void) {
    Delay10KTCYx(254);
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            System Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void run(void) {
    while(1) {
        if (MODE_SELECT == MODE_SEND) {
            senderMain();
        } else {
            receiverMain();
        }
    }
}

void main(void) {
    setup();

    run();

    while(1);
}
