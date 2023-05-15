#include "../s21_decimal.h"

void s21_add_positive(s21_decimal value_1, s21_decimal value_2,
                      s21_decimal *result) {
  int tmp = 0;
  int scale_1 = s21_get_scale(value_1);
  int scale_2 = s21_get_scale(value_2);

  // scale
  if (scale_1 > 0 || scale_2 > 0) {  // преобразование числа с меньшим скейлом
                                     // для нормального вычитания или сложения
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
  // ------------

  for (int cnt = 0; cnt < 96; cnt++) {
    int summa = s21_get_bit(value_1, cnt) + s21_get_bit(value_2, cnt);
    if (summa == 2) {
      if (tmp == 1) {
        s21_set_bit(result, cnt);  // 2 + 1
      } else {
        s21_remove_bit(result, cnt);  // 2 + 0
      }
      tmp = 1;
    } else if (summa == 0) {
      (tmp == 0) ? s21_remove_bit(result, cnt) : s21_set_bit(result, cnt);
      tmp = 0;
    } else {
      (tmp == 0) ? s21_set_bit(result, cnt) : s21_remove_bit(result, cnt);
      tmp = (tmp == 0) ? 0 : 1;
    }
  }
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arith_result status = OK;

  if (result && buffer_overflow(value_1, value_2, PLUS) == 0) {
    init_decimal(result);
    if (!s21_get_bit(value_1, 127) && !s21_get_bit(value_2, 127)) {  // + + +
      s21_add_positive(value_1, value_2, result);
    } else if (s21_get_bit(value_1, 127) == 1 &&
               s21_get_bit(value_2, 127) == 1) {  // - + -
      s21_remove_bit(&value_1, 127);
      s21_remove_bit(&value_2, 127);
      s21_add_positive(value_1, value_2, result);
      s21_set_bit(result, 127);
    } else {                            // - + + or + + -
      if (s21_get_bit(value_1, 127)) {  // - + +
        s21_decimal tmp_v_1;
        init_decimal(&tmp_v_1);
        s21_negate(value_1, &tmp_v_1);
        s21_set_scale(&tmp_v_1, s21_get_scale(value_1));
        if (s21_is_greater(tmp_v_1, value_2)) {  // v_1 > v_2
          s21_sub_positive(tmp_v_1, value_2, result);
          s21_set_bit(result, 127);
        } else if (s21_is_less(tmp_v_1, value_2)) {  // v_1 < v_2
          s21_sub_positive(value_2, tmp_v_1, result);
        } else {  // ==
          init_decimal(result);
        }
      } else {                                   // + + -
        if (s21_is_greater(value_1, value_2)) {  // v_1 > v_2
          s21_sub_positive(value_1, value_2, result);
        } else if (s21_is_less(value_1, value_2)) {  // v_1 < v_2
          s21_sub_positive(value_2, value_1, result);
          s21_set_bit(result, 127);
        } else {  // ==
          init_decimal(result);
        }
      }
    }
  } else {
    status = buffer_overflow(value_1, value_2, PLUS);
    if (result) {
      init_decimal(result);
    }
  }

  return status;
}
