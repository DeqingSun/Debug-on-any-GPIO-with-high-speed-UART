import time
from machine import Pin
from rp2 import PIO, StateMachine, asm_pio

UART_BAUD = 5333333

#UART INPUT PIN
PIO_RX_PIN = Pin(17, Pin.IN, Pin.PULL_UP)
ONBOARD_LED_PIN = Pin(25, Pin.OUT)

@asm_pio(
    in_shiftdir=rp2.PIO.SHIFT_RIGHT,
)
def uart_rx():
    # fmt: off
    label("start")
    # Stall until start bit is asserted
    wait(0, pin, 0)
    # Preload bit counter, then delay until halfway through
    # the first data bit (12 cycles incl wait, set).
    set(x, 7)                 [10]
    label("bitloop")
    # Shift data bit into ISR
    in_(pins, 1)
    # Loop 8 times, each loop iteration is 8 cycles
    jmp(x_dec, "bitloop")     [6]
    # Check stop bit (should be high)
    jmp(pin, "good_stop")
    # Either a framing error or a break. Set a sticky flag
    # and wait for line to return to idle state.
    irq(block, 4)
    wait(1, pin, 0)
    # Don't push data if we didn't see good framing.
    jmp("start")
    # No delay before returning to start; a little slack is
    # important in case the TX clock is slightly too fast.
    label("good_stop")
    push(block)
    # fmt: on


# The handler for a UART break detected by the PIO.
def handler(sm):
    pass
    #print("break", time.ticks_ms(), end=" ")


# Set up the state machine we're going to use to receive the characters.
sm = StateMachine(0, uart_rx, freq=8 * UART_BAUD, in_base=PIO_RX_PIN, jmp_pin=PIO_RX_PIN)
sm.irq(handler)
sm.active(1)

ONBOARD_LED_PIN.value(1)
timestampInterval = 10
startTime = time.ticks_ms()
prevTime = startTime-timestampInterval
while True:
    recvChar = chr(sm.get() >> 24)
    recvTime = time.ticks_ms()
    if ((recvTime-prevTime)>=timestampInterval):
        print('\n['+str(recvTime-startTime)+']', end="")
        ONBOARD_LED_PIN.toggle()
    prevTime = recvTime    
    print(recvChar, end="")
