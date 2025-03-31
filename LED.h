#define MASK(x) (1 << (x))

// Define LEDs
#define GREEN_PTC4 4
#define GREEN_PTC5 5
#define GREEN_PTC6 6
#define GREEN_PTC10 10
#define GREEN_PTC11 11
#define GREEN_PTC12 12
#define GREEN_PTC13 13
#define GREEN_PTC16 16
#define RED_PTC7 7 

// Configure as GPIO, Set as output
void InitGPIO(void) {
  // Enable Clock to PORTC
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

  PORTC->PCR[GREEN_PTC4] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC5] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC6] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC10] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC11] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC12] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC13] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;
  PORTC->PCR[GREEN_PTC16] = PORT_PCR_MUX(1)|PORT_PCR_PE_MASK|PORT_PCR_PS_MASK;

  PORTC->PCR[RED_PTC7] = PORT_PCR_MUX(1);

  PTC->PDDR |= (MASK(GREEN_PTC4) | MASK(GREEN_PTC5) | MASK(GREEN_PTC6) | MASK(GREEN_PTC10) | MASK(GREEN_PTC11) | MASK(GREEN_PTC12) | MASK(GREEN_PTC13) | MASK(GREEN_PTC16) | MASK(RED_PTC7));
}

// Turn off LEDs
void GreenOff(void) {
  PTC->PSOR = (MASK(GREEN_PTC4) | MASK(GREEN_PTC5) | MASK(GREEN_PTC6) | MASK(GREEN_PTC10) | MASK(GREEN_PTC11) | MASK(GREEN_PTC12) | MASK(GREEN_PTC13) | MASK(GREEN_PTC16));
}

void RedOff(void) {
  PTC->PSOR = MASK(RED_PTC7);
}

// Robot Moving
void GreenMoving(void) {
  const uint32_t GREEN_LEDS[] = {
    MASK(GREEN_PTC4), MASK(GREEN_PTC5), MASK(GREEN_PTC6), MASK(GREEN_PTC10), MASK(GREEN_PTC11), MASK(GREEN_PTC12), MASK(GREEN_PTC13), MASK(GREEN_PTC16)
  };
  
  const int NUM_LEDs = sizeof(GREEN_LEDS) / sizeof(GREEN_LEDS[0]);

  for (int i = 0; i < NUM_LEDs; i++) {
    Green_Off(); // Turn off all LEDs before lighting the next
    PTC->PCOR = GREEN_LEDS[i]; // Turn on one LED at a time
    osDelay(100); // Wait for a short time
  }
}

void RedMoving(void) {
  PTC->PCOR = MASK(RED_PTC7); // Turn ON Red LED
  osDelay(500); // Keep ON for 500ms
  PTC->PSOR = MASK(RED_PTC7); // Turn OFF Red LED
  osDelay(500); // Keep OFF for 500ms
}

// Robot Stationary

void GreenStationary(void) {
  GreenOff(); // Ensure all LEDs are off first
  PTC->PCOR = (MASK(GREEN_PTC4) | MASK(GREEN_PTC5) | MASK(GREEN_PTC6) | MASK(GREEN_PTC10) | MASK(GREEN_PTC11) | MASK(GREEN_PTC12) | MASK(GREEN_PTC13) | MASK(GREEN_PTC16)); // Turn on all LEDs
}

void RedStationary(void) {
  PTC->PCOR = MASK(RED_PTC7); // Turn ON Red LED
  osDelay(250); // Keep ON for 250ms
  PTC->PSOR = MASK(RED_PTC7); // Turn OFF Red LED
  osDelay(250); // Keep OFF for 250ms
}

// RTOS Threads

void GreenThread (void *argument) {
  for (;;) {
    if (isMoving) {
      GreenMoving(); // Running Effect
    } else {
      GreenStationary(); // All LEDs ON
    }
  }
}

void RedThread (void *argument) {
  for (;;) {
    if (isMoving) {
      RedMoving(); // Blink at 500ms
    } else {
      RedStationary(); // Blink at 250ms
    }
  }
}

// Main
int main(void){
  SystemCoreClockUpdate();
  InitGPIO();

  osKernelInitialize();

  // Create LED threads
  osThreadNew(GreenThread, NULL, NULL);
  osThreadNew(RedThread, NULL, NULL);

  osKernelStart();
}