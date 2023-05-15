#include "../s21_decimal.h"

void s21_sub_positive(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result) {
  int scale_1 = s21_get_scale(value_1);
  int scale_2 = s21_get_scale(value_2);

  if (scale_1 > 0 || scale_2 > 0) {
    s21_set_scale(result, (scale_1 > scale_2) ? scale_1 : scale_2);
    s21_set_scale((scale_1 > scale_2) ? &value_2 : &value_1, 0);
    s21_decimal shift = {
        {(int)pow(10.0, (double)fabs((double)scale_1 - (double)scale_2)), 0, 0,
         0}};
    s21_decimal new_val_1 = {{0, 0, 0, 0}};
    s21_decimal new_val_2 = {{0, 0, 0, 0}};
    s21_mul((scale_1 > scale_2) ? value_2 : value_1, shift,
            (scale_1 > scale_2) ? &new_val_2 : &new_val_1);
    s21_cpy_decimal((scale_1 > scale_2) ? &new_val_2 : &new_val_1,
                    (scale_1 > scale_2) ? &value_2 : &value_1);
  }

  int tmp = 0;
  for (int cnt = 0; cnt < 96; cnt++) {
    int sub = s21_get_bit(value_1, cnt) - s21_get_bit(value_2, cnt);
    if (sub > 0) {  // 1 - 0 - tmp
      (tmp == 0) ? s21_set_bit(result, cnt) : s21_remove_bit(result, cnt);
      tmp = 0;
    } else if (sub < 0) {  // 0 - 1
      (tmp == 0) ? s21_set_bit(result, cnt) : s21_remove_bit(result, cnt);
      tmp = 1;
    } else {  // 0 - 0 or 1 - 1
      (tmp == 0) ? s21_remove_bit(result, cnt) : s21_set_bit(result, cnt);
      tmp = (tmp == 0) ? 0 : 1;
    }
  }
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arith_result status = OK;

  if (result && buffer_overflow(value_1, value_2, MIN) == 0) {
    init_decimal(result);
    if (s21_get_bit(value_1, 127) == 0 && s21_get_bit(value_2, 127) == 0) {
      s21_sub_positive(value_1, value_2, result);
    } else if (s21_get_bit(value_1, 127) && s21_get_bit(value_2, 127)) {
      s21_remove_bit(&value_1, 127);
      s21_remove_bit(&value_2, 127);
      s21_sub_positive(value_1, value_2, result);
      s21_set_bit(result, 127);
    } else {
      if (s21_get_bit(value_1, 127)) {
        s21_add_positive(value_1, value_2, result);
        s21_set_bit(result, 127);
      } else {
        s21_add_positive(value_1, value_2, result);
      }
    }
  } else {
    status = buffer_overflow(value_1, value_2, MIN);
    if (result) {
      init_decimal(result);
    }
  }

  return status;
}
