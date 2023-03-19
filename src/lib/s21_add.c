#include "../s21_decimal.h"
#include <string.h>
/*Функции возвращают код ошибки:

0 - OK
1 - число слишком велико или равно бесконечности
2 - число слишком мало или равно отрицательной бесконечности
3 - деление на 0*/

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) { // +
    memset(&(result->bits), 0, sizeof(result->bits));

    int cnt_mantissa = 0;
    int tmp = 0;
    for (int cnt = 0; cnt < 95; cnt++) {
        int summa = s21_get_bit(value_1.bits[cnt_mantissa], cnt) + s21_get_bit(value_2.bits[cnt_mantissa], cnt);
        if (summa == 2) {
            s21_set_bit(*result, cnt_mantissa, 0 + tmp, cnt);
            tmp = 1;
            
        } else if (summa == 0) {
            s21_set_bit(*result, cnt_mantissa, 0 + tmp, cnt);
            tmp = 0;
        } else {
            s21_set_bit(*result, cnt_mantissa, 1 + tmp, cnt);
            tmp = (tmp == 0) ? 0 : 1;
        }
    }
}

// int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {

// }

int s21_get_bit(unsigned int num, int place) { // полуает бит
    int result = (1 << place);
    if (num & result == 1) {
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

void s21_set_bit(s21_decimal num, int mantissa, int bit, int place) { // replace bit on chosen bit
    int result = 1 << place;
    num.bits[mantissa] = num.bits[mantissa] & (~result);
}


int main() {
    s21_decimal value_1 = {0};
    value_1.bits[0] = 0b0100;
    s21_decimal value_2 = {0};
    value_2.bits[0] = 0b0101;
    s21_decimal result;
    s21_add(value_1, value_2, &result);
    printf("%u", result.bits);

    return 0;

}