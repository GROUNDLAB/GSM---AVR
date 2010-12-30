#include "ioHelper.h"


void blink(uint16_t LED, void (*delay_ms)(uint16_t)){
//output_high(PORTB,LED);
toggleOutput(PORTB,LED);
delay_ms(1000);
toggleOutput(PORTB,LED);
//output_low(PORTB,LED);
delay_ms(1000);
}
