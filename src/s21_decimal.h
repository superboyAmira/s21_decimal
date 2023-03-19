#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <stdio.h>
#include <stdlib.h>

// typedef enum value_type_t{
//     0
//     1
//     2
//     3
// } value_type_t;

typedef struct s21_decimal{
    unsigned int bits[4];
} s21_decimal;

int s21_get_bit(unsigned int num, int place);
void s21_set_bit(s21_decimal num, int mantissa, int bit, int place);


#endif  // SRC_S21_DECIMAL_H_