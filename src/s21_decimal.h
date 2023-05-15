#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIT 8
#define SIZE_BITS 4
#define NUM_255 255
#define TRUE 1
#define FALSE 0

#define S21_MAX_UINT 4294967295  // for big tests

#define MAX_BITS_VAL 4294967295U
#define is_fin(x) __builtin_isfinite(x)
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)

typedef enum operator{ PLUS = 0, MIN = 1, MUL = 2, DIV = 3 } operator;

enum { LOW, MID, HIGH, SCALE };

typedef enum { CONVERTATION_OK, CONVERTATION_ERROR } conv_result;

typedef enum {
  OK = 0,
  INF = 1,
  MIN_INF = 2,
  DIV_BY_ZERO = 3,
  ERR = 4
} arith_result;

typedef enum { OTHER_OK, OTHER_ERROR } other_result;

typedef struct s21_decimal {
  unsigned int bits[4];
} s21_decimal;

// arithmetic
int format_div(s21_decimal *val_1, s21_decimal *val_2);
void s21_add_positive(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

void s21_sub_positive(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// converters
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_float_format(int integer, float fract, s21_decimal *value);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
void add_char_to_beginning(char *str, char ch);

s21_decimal s21_string_to_decimal(char *string);
void s21_decimal_to_string(char *dst, s21_decimal src);

// comparison

int s21_equal_compare(s21_decimal dec1, s21_decimal dec2);
int s21_easy_compare(s21_decimal dec1, s21_decimal dec2);
void s21_decimal_null_sign(s21_decimal *tmp);
int s21_is_less(s21_decimal dec1, s21_decimal dec2);              // <
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2);     // <=
int s21_is_greater(s21_decimal dec1, s21_decimal dec2);           // >
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);  // >=
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);             // ==
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);         // !=

// other
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);     // -
int s21_truncate(s21_decimal value, s21_decimal *result);  // -

// help
void eq_scale(s21_decimal *dst1, s21_decimal *dst2);
void make_lower_scale(s21_decimal *val);
int buffer_overflow(s21_decimal val_1, s21_decimal val_2, operator oper);
int s21_get_sign(s21_decimal number);
int s21_get_bit(s21_decimal num, int place);
int s21_remove_bit(s21_decimal *num, int place);
int s21_get_scale(s21_decimal num);
void s21_set_scale(s21_decimal *num, int exp);
int s21_is_negative(int src);
int s21_set_bit(s21_decimal *num, int place);
void init_decimal(s21_decimal *decimla_num);
void s21_cpy_decimal(s21_decimal *src, s21_decimal *dst);
bool is_null_decimal(s21_decimal num);

#endif  // SRC_S21_DECIMAL_H_