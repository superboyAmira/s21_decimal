#include "../s21_decimal.h"

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arith_result status = OK;

  if (result && !is_null_decimal(value_2)) {
    int sign = 0;
    if (s21_get_scale(value_1) + s21_get_scale(value_2) > 0) {
      format_div(&value_1, &value_2);
    }
    if ((s21_get_sign(value_1) == 1 || s21_get_sign(value_2) == 1)) {
      sign = 1;
    }
    s21_remove_bit(&value_1, 127);
    s21_remove_bit(&value_2, 127);

    eq_scale(&value_1, &value_2);

    if (s21_is_less(value_1, value_2)) {
      s21_cpy_decimal(&value_1, result);
    } else if (s21_is_equal(value_1, value_2)) {
      result->bits[LOW] = 0;
    } else if (value_2.bits[LOW] == 1 && value_2.bits[MID] == 0 &&
               value_2.bits[HIGH] == 0) {
      result->bits[LOW] = 0;
    } else {
      char val_1_string[255] = {'\0'};
      char ostatok_string[255] = {'\0'};
      s21_decimal ostatok;
      init_decimal(&ostatok);

      s21_decimal_to_string(val_1_string, value_1);

      int cnt_shift = 0;
      for (; s21_is_less(s21_string_to_decimal(ostatok_string), value_2);
           cnt_shift++) {
        ostatok_string[cnt_shift] = val_1_string[cnt_shift];
      }
      memmove(val_1_string, val_1_string + cnt_shift, strlen(val_1_string));

      do {
        s21_sub(s21_string_to_decimal(ostatok_string), value_2, &ostatok);

        memset(ostatok_string, '\0', sizeof(ostatok_string));
        if (!is_null_decimal(ostatok)) {
          s21_decimal_to_string(ostatok_string, ostatok);
        }

        while (s21_is_less(ostatok, value_2)) {
          strncat(ostatok_string, val_1_string, 1);
          memmove(val_1_string, val_1_string + 1, strlen(val_1_string));
          ostatok = s21_string_to_decimal(ostatok_string);

          if ((is_null_decimal(s21_string_to_decimal(val_1_string)) &&
               is_null_decimal(ostatok)) ||
              !strlen(val_1_string)) {
            break;
          }
        }
      } while (s21_is_greater_or_equal(ostatok, value_2));
      s21_cpy_decimal(&ostatok, result);
      if (sign) {
        s21_set_bit(result, 127);
      }
    }
  } else {
    status = ERR;
  }

  return status;
}
