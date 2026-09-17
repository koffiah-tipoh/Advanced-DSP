#ifndef __SINE_GENERATOR_H__
#define __SINE_GENERATOR_H__

#include "arm_math.h"

//typedef struct {
//	arm_biquad_casd_df1_inst_q15 iir_sine_generator_instance;
//	q15_t coeff[6];
//	q15_t state[4];
//} sine_generator_q15_t;
//
//void sine_gen_init_q15(sine_generator_q15_t *sine_desc, q15_t sine_freq, q15_t sampl_freq);
//q15_t sine_calc_sample_q15(sine_generator_q15_t *sine_desc);

typedef struct {
	arm_biquad_casd_df1_inst_f32 iir_sine_generator_instance;
	float32_t coeff[5];
	float32_t state[4];
} sine_generator_f32_t;

void sine_gen_init_f32(sine_generator_f32_t *sine_desc, float32_t sine_freq, float32_t sampl_freq);
float32_t sine_calc_sample_f32(sine_generator_f32_t *sine_desc);

#endif
