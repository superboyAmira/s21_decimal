#include "../s21_decimal.h"

int s21_equal_compare(
    s21_decimal dec1,
    s21_decimal dec2) {  // для нахождения равенства и неравенства
  int res = -1;
  if (s21_get_scale(dec1) > 0 || s21_get_scale(dec2) > 0) {
    eq_scale(&dec1, &dec2);
  }
  if (dec1.bits[3] == dec2.bits[3]) {
    if (dec1.bits[2] == dec2.bits[2]) {
      if (dec1.bits[1] == dec2.bits[1]) {
        if (dec1.bits[0] == dec2.bits[0]) {
          res = 1;  // True
        } else {
          res = 0;  // False
        }
      } else {
        res = 0;
      }
    } else {
      res = 0;
    }
  } else {
    res = 0;
  }
  return res;
}

int s21_easy_compare(s21_decimal dec1,
                     s21_decimal dec2) {  // для сравнения положительных чисел
  int res = -1;
  int bit1 = 0;
  int bit2 = 0;
  if (s21_get_scale(dec1) > 0 || s21_get_scale(dec2) > 0) {
    eq_scale(&dec1, &dec2);
  }
  for (int i = 95; i >= 0 && (res != 0 || res != 1); i--) {
    bit1 = s21_get_bit(dec1, i);
    bit2 = s21_get_bit(dec2, i);
    if (bit1 > bit2) {
      res = 0;  // сравниваем, что меньше, поэтому в данном случае 0 - false
      break;
    } else if (bit1 < bit2) {
      res = 1;
      break;
    } else {
      res = 0;
    }
  }
  return res;
}

void s21_decimal_null_sign(s21_decimal *tmp) { tmp->bits[3] = 0; }

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {  // меньше
  int res = -1;
  int sign1 = s21_get_bit(dec1, 127);
  int sign2 = s21_get_bit(dec2, 127);
  if (s21_is_equal(dec1, dec2)) {  // если равны друг другу, то false
    res = 0;
  } else if (sign1 > sign2) {  // 1 - отрицательное, 0 - положительное
    res = 1;  // положительное больше отрицательного anyway
  } else if (sign2 > sign1) {
    res = 0;
  } else if (sign1 == 1 &&
             sign2 == 1) {  // если оба числа отрицательные, то больше - то,
                            // которое меньше по значению))
    if (s21_easy_compare(dec1, dec2) == 0) {
      res = 1;
    } else {
      res = 0;
    }       // работаем со степенью
  } else {  // если оба положительные или один равен нулю, то просто сравниваем
    res = s21_easy_compare(dec1, dec2);
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal dec1,
                         s21_decimal dec2) {  // меньше или равно
  return s21_is_less(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {  // больше
  return !s21_is_less_or_equal(dec1, dec2);
}

int s21_is_greater_or_equal(s21_decimal dec1,
                            s21_decimal dec2) {  // больше или равно
  return s21_is_greater(dec1, dec2) || s21_is_equal(dec1, dec2);
}

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {  // равно
  int res = -1;
  int sign1 = s21_get_sign(dec1);
  int sign2 = s21_get_sign(dec2);
  if (sign1 == sign2) {
    res = s21_equal_compare(dec1, dec2);
  } else {
    res = 0;
  }
  return res;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {  // не равно
  return !s21_is_equal(dec1, dec2);
}
