/*
 * Maths.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Tran_Cau
 */

#ifndef MATHS_MATHS_H_
#define MATHS_MATHS_H_
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "stm32_config.h"

#define SIZE_OF_ARRAY(array)  (sizeof(array)/sizeof(array[0]))

#define ONE_DIV_3	1/3.0f
#define ONE_DIV_SQRT_3	0.5773
#define SQRT_3		1.7321
#define ONE_DIV_2	0.5
#define RADPS_FROM_RPM			(0.104720f)

extern const float foc_sin_tbl[];
extern const float foc_saturation_tbl[];
extern const float sqrt_tbl[];

#define TRUE  1
#define FALSE 0

static inline uint8_t maths_is_almost_equal(float a, float b, float epsilon);
static inline float maths_saturation(float input, float up_output, float low_output);
static inline float maths_map_type_1(float input, float gain, float up_output, float low_output);
static inline float maths_map_type_2(float up_output, float low_output,
						float up_input, float low_input,
						float input);

static OPTIMIZE_ATTR float clarke_transfor_alpha(const float a, const float b,
		const float c) {
	float result, temp_1, temp_2, temp_3;
	temp_1 = 2*a;
	temp_2 = b + c;
	temp_3 = temp_1 - temp_2;
	result = temp_3*ONE_DIV_3;
	return result;
}

static OPTIMIZE_ATTR float clarke_transfor_beta(const float b,const float c) {

	float temp_1, result;
	temp_1 = b - c;					// q14
	result = temp_1*ONE_DIV_SQRT_3;	//q16
	return result;
}

static OPTIMIZE_ATTR float inverse_transfor_a(const float alpha,__attribute((unused))const float beta) {

	return alpha;
}

static OPTIMIZE_ATTR float inverse_transfor_b(const float alpha,
		const float beta) {

	float result;
	float temp_1, temp_2;
	temp_1 = SQRT_3 * beta;  	// q14
	temp_2 = temp_1 - alpha;				//q14
	result = temp_2 * ONE_DIV_2;	//q14
	return result;
}

static OPTIMIZE_ATTR float inverse_transfor_c(const float alpha,
		const float beta) {

	float result;
	float temp_1, temp_2;
	temp_1 = SQRT_3 * beta;
	temp_2 = temp_1 + alpha;
	result = (-temp_2) * ONE_DIV_2;
	return result;

}

static OPTIMIZE_ATTR float park_transfor_d(const float alpha,
		const float beta, const float f_cosin, const float f_sin) {
	float temp_1, temp_2, result;
	temp_1 = alpha * f_cosin;
	temp_2 = beta * f_sin;
	result = temp_1 + temp_2;

	return result;
}

static OPTIMIZE_ATTR float park_transfor_q(const float alpha, const float beta,
		const float f_cosin, const float f_sin) {
	float temp_1, temp_2, result;

	temp_1 = beta * f_cosin;
	temp_2 = alpha * f_sin;
	result = temp_1 - temp_2;
	return result;

}

static OPTIMIZE_ATTR float inverse_park_transfor_alpha(const float I_d,
		const float I_q, const float f_cosin, const float f_sin) {
	float temp_1, temp_2, result;
	temp_1 = I_d * f_cosin;
	temp_2 = I_q * f_sin;
	result = temp_1 - temp_2;
	return result;

}

static OPTIMIZE_ATTR float inverse_park_transfor_beta(const float I_d,
		const float I_q,const float f_cosin, const float f_sin) {
	float temp_1, temp_2, result;
	temp_1 = I_q * f_cosin;
	temp_2 = I_d * f_sin;
	result = temp_1 + temp_2;
	return result;

}
static inline uint32_t min(const uint32_t temp_1, const uint32_t temp_2, const uint32_t temp_3) {
	uint32_t min_value = 0;
	min_value = (temp_1 <= temp_2) ? temp_1 : temp_2;
	min_value = (min_value <= temp_3) ? min_value : temp_3;
	return min_value;
}

/* Square root of sum of squares */
float maths_sqrt_of_susq_lut(const float a, const float b, const float max);
float maths_sqrt_of_susq_binary(const float a, const float b);

static inline uint8_t maths_is_almost_equal(float a, float b, float epsilon)
{
	return ((((a-b) < epsilon) && ((a-b) > 0)) || (((b-a) < epsilon) && ((b-a) > 0)));
}

static inline float maths_saturation(float input, float up_output, float low_output)
{
	if(input > up_output){
		return up_output;
	}else if(input < low_output){
		return low_output;
	}
	return input;
}
static inline float maths_map_type_1(float input, float gain, float up_output, float low_output)
{
	float temp = input * gain;
	return maths_saturation(temp, up_output, low_output);
}
static inline float maths_map_type_2(float up_output, float low_output,
						float up_input, float low_input,
						float input)
{
	float temp = up_input - low_input;
	if (temp < 10)
	{
		temp = 10;
	}
	float gain = (up_output - low_output)/temp;
	temp = gain * (input - low_input) + low_output;
	temp = maths_saturation(temp, up_output, low_output);
	return temp;
}
static inline float maths_sqrt_binary(const float input)
{
	int32_t temp = *(int32_t*)&input;
	temp -= 127<<23;
	temp >>= 1;
	temp += 127<<23;
	return *(float*)&temp;
}
#endif /* MATHS_MATHS_H_ */
