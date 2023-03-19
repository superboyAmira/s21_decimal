#include "../s21_decimal.h"

int s21_get_bit(unsigned int num, int place) { // полуает бит
    int result = 1 << place;
    if (num & result == 1) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void s21_set_bit(s21_decimal num, int mantissa, int bit, int place) { // replace bit on chosen bit
    int result = 1 << place;
    num.bits[mantissa] & (~result);
}

