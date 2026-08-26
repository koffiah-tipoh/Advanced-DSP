#include <stdio.h>

#include "stm32f4xx.h"
#include "signals.h"
#include "uart.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "task.h"


extern float _5hz_signal[HZ_5_SIG_LEN];
extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN];
float g_in_sig_samples;

static void plot_input_signal(void);
static void serial_plot_input_sig(void);
static void pseudo_dly(int dly);
static void fpu_enable(void);


int main(){
	/* Enable FPU*/
	fpu_enable();

	/* Initialize the uart*/
	uart3_tx_init();

    while(1) {
    	// Using the internal Logic Analyzer
    	//plot_input_signal();

    	// Using the Serial Plotter (Arduino) / Python + Qt5 plotter script
    	serial_plot_input_sig();

    	//printf("Hello from STM32...\n\r");

    }
}

static void plot_input_signal(void) {
	int i;
	for(i = 0; i < KHZ1_15_SIG_LEN; i++) {
		//g_in_sig_samples = _5hz_signal[i];
		g_in_sig_samples = input_signal_f32_1kHz_15kHz[i];
		pseudo_dly(9000);
	}
}

static void serial_plot_input_sig(void) {
	for(int i = 0; i < KHZ1_15_SIG_LEN; i++) {
	   printf("%f\r\n", input_signal_f32_1kHz_15kHz[i]);
	   pseudo_dly(9000);
	}
}

static void pseudo_dly(int dly) {	/*False delay - dly is not time parameter*/
	for(int i = 0; i < dly; i++) {}
}

static void fpu_enable(void) {
	/* Enable the Floating Point Unit: Enable CP10 and CP11 full access */
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));
}
