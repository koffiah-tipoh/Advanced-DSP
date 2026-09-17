#include <stdio.h>
#include <stdint.h>

#include "stm32f4xx.h"
#include "signals.h"
#include "uart.h"
#include "arm_math.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sine_generator.h"

//extern float _5hz_signal[HZ_5_SIG_LEN];
//extern float32_t input_signal_f32_1kHz_15kHz[KHZ1_15_SIG_LEN];
//float g_in_sig_samples;
//
//static void plot_input_signal(void);
//static void serial_plot_input_sig(void);
static void pseudo_dly(int dly);
static void fpu_enable(void);

//#define BLOCK_SIZE 5
//
//uint32_t task1Profiler, task2Profiler;
//
//void Task1(void *pvParameter);
//void Task2(void *pvParameter);
//
//uint32_t src_sensor_data[BLOCK_SIZE] = { 2043, 1023, 990, 67, 3013 };
//float32_t f32_sensor_data[BLOCK_SIZE];
//q31_t q31_sensor_data[BLOCK_SIZE];
//float32_t f32_dest_sensor_data[BLOCK_SIZE];
//uint32_t u32_dest_sensor_data[BLOCK_SIZE];

sine_generator_f32_t sine_gen;

int main() {
	/* Enable FPU*/
	fpu_enable();

	/* Initialize the uart*/
	uart3_tx_init();

//	/* Scale ADC values to between -1 and +1 */
//	for(int i=0; i < BLOCK_SIZE; i++) {
//		f32_sensor_data[i] = ((float32_t) (src_sensor_data[i] & 0xFFF) / (0xFFF/2)) -1;
//	}
//
//	/*Convert float to q*/
//	arm_float_to_q31(f32_sensor_data, q31_sensor_data, BLOCK_SIZE);
//
//	/* Perform DSP*/
//	/* Do Something */
//
//	/* Convert q to float*/
//	arm_q31_to_float(q31_sensor_data, f32_dest_sensor_data, BLOCK_SIZE);
//
//	/* Convert float to uint32_t */
//	float32_t temp_data;
//
//	for(int i = 0; i < BLOCK_SIZE; i++) {
//		temp_data = (f32_dest_sensor_data[i] + 1) * (0xFFF/2);
//
//		/* Round to nearest integer */
//		u32_dest_sensor_data[i] = (uint32_t) (temp_data + 0.5);
//	}

	sine_gen_init_f32(&sine_gen, /*sine_freq=*/1000.0f, /*sampl_freq=*/
			16000.0f);

	for (;;) {

		// Using the Serial Plotter (Arduino) / Python + Qt5 plotter script
		//serial_plot_input_sig();
		float32_t sample = sine_calc_sample_f32(&sine_gen);
		printf("%f\r\n", sample);
		pseudo_dly(9000);
	}
}

//void Task1(void *pvParameter) {
//	for (;;) {
//		// Do something...
//		task1Profiler++;
//	}
//}
//
//void Task2(void *pvParameter) {
//	for (;;) {
//		// Do something...
//		task2Profiler++;
//	}
//}

//static void plot_input_signal(void) {
//	int i;
//	for (i = 0; i < KHZ1_15_SIG_LEN; i++) {
//		//g_in_sig_samples = _5hz_signal[i];
//		g_in_sig_samples = input_signal_f32_1kHz_15kHz[i];
//		pseudo_dly(9000);
//	}
//}
//
//static void serial_plot_input_sig(void) {
//	for (int i = 0; i < KHZ1_15_SIG_LEN; i++) {
//		printf("%f\r\n", input_signal_f32_1kHz_15kHz[i]);
//		pseudo_dly(9000);
//	}
//}

static void pseudo_dly(int dly) { /*False delay - dly is not time parameter*/
	for (int i = 0; i < dly; i++) {
	}
}

static void fpu_enable(void) {
	/* Enable the Floating Point Unit: Enable CP10 and CP11 full access */
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
}
