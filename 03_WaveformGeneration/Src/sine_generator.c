#include "sine_generator.h"

void sine_gen_init_f32(sine_generator_f32_t *sine_desc, float32_t sine_freq, float32_t sampl_freq) {

	float32_t y[4];

	y[0] = 0;

	y[1] = arm_sin_f32(2 * PI * sine_freq / sampl_freq);
	y[2] = arm_sin_f32(4 * PI * sine_freq / sampl_freq);
	y[3] = arm_sin_f32(6 * PI * sine_freq / sampl_freq);

	/* b0, b1, b2 = 0 — no input driving this, it's a self-oscillator */
		sine_desc->coeff[0] = 0;
		sine_desc->coeff[1] = 0;
		sine_desc->coeff[2] = 0;

	/* Calculate coeffs for sine wave generator */
	sine_desc->coeff[3] = (y[2] / y[1]);
	sine_desc->coeff[4] = (y[1] * y[3] - y[2] * y[2]) / (y[1] * y[1]);

	/* No conversion needed — everything stays float32_t */

	/* f32 init has no postShift argument (fixed-point-only parameter) */
	arm_biquad_cascade_df1_init_f32(&(sine_desc->iir_sine_generator_instance),
			1, sine_desc->coeff, sine_desc->state);

	/* Seed state: y[n-1] = sin(w), y[n-2] = 0 — correct slots (index 2 and 3) */
	sine_desc->state[2] = y[1];
	sine_desc->state[3] = 0;
}

float32_t sine_calc_sample_f32(sine_generator_f32_t *sine_desc) {

	float32_t input = 0.0f;
	float32_t output;

	arm_biquad_cascade_df1_f32(&(sine_desc->iir_sine_generator_instance), &input, &output, 1);

	return output;
}
