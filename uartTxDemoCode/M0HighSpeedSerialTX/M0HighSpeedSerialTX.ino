//tested on M0, should also work for Zero

//need to run in SRAM, or flash wait will occur
__attribute__ ((section (".data")))
void tx_on_PA17(char c) {
  //use IOBUS instead of APB for faster access
  //avoid branching to reduce uncertainty in timing

  //the instruction counts 7 clks per bit. Without ".data" it will be around 9 clks, not accurate.
  //that is 6857142bit/s for a 48M M0 Arduino

  asm volatile("mrs r2, PRIMASK                         \n\t" //save previous interrupt status
               "cpsid  i                                \n\t" //noInterrupts

               "ldr r0, [%[OUT_REG_ADDR], #0]           \n\t"
               "bic r0, %[BIT_MASK]                     \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //start bit
               //bit 0
               "bic r0,%[BIT_MASK]                      \n\t" //1clk, clear bit 17
               "movs r1, #0                             \n\t" //1clk
               "lsr %[OUT_CHAR], #1                     \n\t" //1clk, right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //1clk, move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //1clk, move the bit to position 17
               "orr r0, r1                              \n\t" //1clk,
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //1clk, send calculated OUT to register *2 if to AHB interface or SCS, 1 if to single-cycle I/O port
               //bit 1
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 2
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 3
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 4
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 5
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 6
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //bit 7
               "bic r0,%[BIT_MASK]                      \n\t" //clear bit 17
               "movs r1, #0                             \n\t"
               "lsr %[OUT_CHAR], #1                     \n\t" //right shift OUT_CHAR
               "adc r1, %[ZERO_REG]                     \n\t" //move the shifted out bit into r1
               "lsl r1, #17                             \n\t" //move the bit to position 17
               "orr r0, r1                              \n\t"
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t" //send calculated OUT to register
               //stop bit
               "bic r0,%[BIT_MASK]                      \n\t" //doing some useless work to keep the right timing
               "movs r1, #0                             \n\t" //doing some useless work to keep the right timing
               "lsr %[OUT_CHAR], #1                     \n\t" //doing some useless work to keep the right timing
               "adc r1, %[ZERO_REG]                     \n\t" //doing some useless work to keep the right timing
               "lsl r1, #17                             \n\t" //doing some useless work to keep the right timing
               "orr r0, %[BIT_MASK]                     \n\t" //always set to 1 for stop bit
               "str r0, [%[OUT_REG_ADDR], #0]           \n\t"

               "msr PRIMASK,r2                          \n\t" //restore interrupt status

               :
               :[OUT_REG_ADDR]"r"((int)&(PORT_IOBUS->Group[0].OUT.reg)), [BIT_MASK]"r"(1<<17), [ZERO_REG]"r"(0), [OUT_CHAR]"r"(c)
               :"r0", "r1", "r2", "cc", "memory"
              );
}

void setup() {
  pinMode(13, OUTPUT);//pin 13 is PA17
  digitalWrite(13, HIGH);
}

void loop() {
  tx_on_PA17('t');
  tx_on_PA17('e');
  tx_on_PA17('s');
  tx_on_PA17('t');
  tx_on_PA17('\n');
  delayMicroseconds(2);
}
