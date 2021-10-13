void tx_on_PB5(char c) {
  //this approach takes 3 clks to send a bit, that is 5333333bit/s for a 16M Arduino
  //use __tmp_reg__ to store SREG to recover interrupt status
  //use __zero_reg__ as temp register, clear at the end of code

  asm volatile("IN __tmp_reg__,__SREG__          \n\t" //backup SREG
               "CLI                              \n\t"

               "IN __zero_reg__,%[ADDR_PORTB]    \n\t"

               "CLT                              \n\t" //1CLK start bit
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK

               "BST %[OUT_CHAR],0                \n\t" //1CLK bit 0
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],1                \n\t" //1CLK bit 1
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],2                \n\t" //1CLK bit 2
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],3                \n\t" //1CLK bit 3
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],4                \n\t" //1CLK bit 4
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],5                \n\t" //1CLK bit 5
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],6                \n\t" //1CLK bit 6
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK
               "BST %[OUT_CHAR],7                \n\t" //1CLK bit 7
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK

               "SET                              \n\t" //1CLK stop bit
               "BLD __zero_reg__,5               \n\t" //1CLK
               "OUT %[ADDR_PORTB],__zero_reg__   \n\t" //1CLK

               "CLR __zero_reg__                 \n\t"
               "OUT __SREG__,__tmp_reg__         \n\t" //restore SREG

               ::[OUT_CHAR]"r" (c), [ADDR_PORTB]"I" (_SFR_IO_ADDR(PORTB)));
}

// Function that printf and related will use to print
int serial_IO_tx_putchar(char c, FILE* f) {
  //suppose we are using D13 on Uno
  tx_on_PB5(c);
  return 1;
}

FILE serial_IO_tx_stdout;

void setup() {
  pinMode(13, OUTPUT);//pin 13 is PB5
  digitalWrite(13, HIGH);

  // Set up stdout, ref: https://forum.arduino.cc/t/using-stdin-stdout-printf-scanf-etc/117914
  fdev_setup_stream(&serial_IO_tx_stdout, serial_IO_tx_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &serial_IO_tx_stdout;
}

void loop() {
  printf("test\n");
  delayMicroseconds(2);
  //if higher speed is desired, just call tx_on_PB5 directly
  tx_on_PB5('t');
  tx_on_PB5('e');
  tx_on_PB5('s');
  tx_on_PB5('t');
  tx_on_PB5('\n');
  delayMicroseconds(2);
}
