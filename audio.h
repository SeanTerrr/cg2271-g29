// Define Note Frequencies
// Note Frequencies (Hz)
#define note_C   1047
#define note_D   1175
#define note_E   1319
#define note_F   1397
#define note_G   1568
#define note_A   1760
#define note_B   1865
#define note_C1  2093
#define note_D1  2349
#define note_E1  2637
#define note_F1  2794
#define note_G1  3136
#define note_A1  3520
#define note_B1  3729

// Setup Clock, Set as Output
#define CLOCK (48000000 / 128) // 375000 Hz
#define PTD0 0 // Buzzer output (TPM0_CH0 - PWM)


uint16_t freqEnd[] = { 
    note_C, note_C, note_G, note_G,
    note_A, note_A, note_G, note_G,
    note_F, note_F, note_E, note_E,
    note_D, note_D, note_C
};

uint16_t freqRunning[] = { 
    note_C, note_C, note_G, note_G,
    note_A, note_A, note_G, note_G,
    note_F, note_F, note_E, note_E,
    note_D, note_D, note_C
};

void InitAudio(void){
  // Enable Clock Gating for PORTD
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

  // Configure PTD0 as PWM output
  PORTD->PCR[PTD0] &= ~PORT_PCR_MUX_MASK;
  PORTD->PCR[PTD0] |= PORT_PCR_MUX(4); // ALT4 for TPM

  // Enable Clock for TPM0 (Timer Module)
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;

  // Select Clock Source for TPM0
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1); // Internal clock

  // Configure TPM0 for PWM
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= TPM_SC_CMOD(1) | TPM_SC_PS(7); // Up counting, Prescaler 128
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK); // Edge-aligned PWM

  // Enable PWM on TPM0 Channel 0
  TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) |(TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
  TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1)); // High-true pulses
}
