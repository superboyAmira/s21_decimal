#include "../s21_decimal.h"

int format_div(s21_decimal *val_1, s21_decimal *val_2) {  // returned our scale
  int scale = 0;
  if (s21_get_scale(*val_1) == 0 && s21_get_scale(*val_2) > 0) {
    scale = s21_get_scale(*val_2);
    int sign_1 = s21_get_sign(*val_1);
    s21_decimal tmp_mul = {{(int)pow(10, scale), 0, 0, 0}};
    s21_decimal tmp_v_1 = {{0, 0, 0, 0}};

    s21_set_scale(val_1, 0);
    s21_mul(*val_1, tmp_mul, &tmp_v_1);
    s21_cpy_decimal(&tmp_v_1, val_1);
    if (sign_1) {
      s21_set_bit(val_1, 127);
    }
    s21_set_scale(val_2, 0);
  } else if (s21_get_scale(*val_1) > 0 && s21_get_scale(*val_2) == 0) {
    scale = s21_get_scale(*val_1);
    int sign_1 = s21_get_sign(*val_1);
    s21_decimal tmp_mul = {{(int)pow(10, scale), 0, 0, 0}};
    s21_decimal tmp_v_2 = {{0, 0, 0, 0}};

    s21_set_scale(val_2, 0);
    s21_mul(*val_2, tmp_mul, &tmp_v_2);
    s21_cpy_decimal(&tmp_v_2, val_2);
    if (sign_1) {
      s21_set_bit(val_2, 127);
    }
    s21_set_scale(val_1, 0);
  } else if (s21_get_scale(*val_1) > 0 && s21_get_scale(*val_2) > 0) {
    int sign_1 = s21_get_sign(*val_1);
    int sign_2 = s21_get_sign(*val_2);
    s21_decimal tmp_mul_v1 = {
        {(s21_get_scale(*val_2) > 0) ? (int)pow(10, s21_get_scale(*val_2)) : 1,
         0, 0, 0}};
    s21_decimal tmp_mul_v2 = {
        {(s21_get_scale(*val_1) > 0) ? (int)pow(10, s21_get_scale(*val_1)) : 1,
         0, 0, 0}};
    int first = (int)pow(10, s21_get_scale(*val_1));
    s21_decimal tmp_v_1 = {{0, 0, 0, 0}};
    s21_decimal tmp_v_2 = {{0, 0, 0, 0}};

    s21_set_scale(val_1, 0);
    s21_mul(*val_1, tmp_mul_v1, &tmp_v_1);
    s21_cpy_decimal(&tmp_v_1, val_1);
    if (sign_1) {
      s21_set_bit(val_1, 127);
    }
    tmp_mul_v2.bits[0] = first;
    s21_set_scale(val_2, 0);
    s21_mul(*val_2, tmp_mul_v2, &tmp_v_2);
    s21_cpy_decimal(&tmp_v_2, val_2);
    if (sign_2) {
      s21_set_bit(val_1, 127);
    }
  }
  return 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arith_result status = OK;

  if (result && !is_null_decimal(value_2) &&
      buffer_overflow(value_1, value_2, DIV) == 0) {
    int sign = 0;
    int scale = 0;
    if ((s21_get_bit(value_1, 127) && !s21_get_bit(value_2, 127)) ||
        (!s21_get_bit(value_1, 127) && s21_get_bit(value_2, 127))) {
      s21_set_bit(result, 127);
    }

    if ((s21_get_sign(value_1) == 0 && s21_get_sign(value_2) == 1) ||
        (s21_get_sign(value_1) == 1 && s21_get_sign(value_2) == 0)) {
      sign = 1;
    }
    s21_remove_bit(&value_1, 127);
    s21_remove_bit(&value_2, 127);
    if (s21_get_scale(value_1) + s21_get_scale(value_2) > 0) {
      scale = format_div(&value_1, &value_2);
    }

    if (s21_is_less(value_1, value_2)) {
      result->bits[LOW] = 0;
    } else if (s21_is_equal(value_1, value_2)) {
      result->bits[LOW] = 1;
    } else {
      int place_bit_result = 0;
      char val_1_string[255] = {'\0'};
      char ostatok_string[255] = {'\0'};
      char result_string[255] = {'\0'};
      s21_decimal ostatok = {{0, 0, 0, 0}};

      s21_decimal_to_string(val_1_string, value_1);

      int cnt_shift = 0;
      for (; s21_is_less(s21_string_to_decimal(ostatok_string), value_2);
           cnt_shift++) {
        ostatok_string[cnt_shift] = val_1_string[cnt_shift];
      }
      memmove(val_1_string, val_1_string + cnt_shift, strlen(val_1_string));

      do {
        s21_sub(s21_string_to_decimal(ostatok_string), value_2, &ostatok);
        result_string[place_bit_result++] = '1';
        memset(ostatok_string, '\0', sizeof(ostatok_string));
        if (!is_null_decimal(ostatok)) {
          s21_decimal_to_string(ostatok_string, ostatok);
        }

        cnt_shift = 1;
        while (s21_is_less(ostatok, value_2)) {
          if (strlen(val_1_string) == 0) {
            break;
          }
          cnt_shift++;

          strncat(ostatok_string, val_1_string, 1);
          memmove(val_1_string, val_1_string + 1, strlen(val_1_string));
          ostatok = s21_string_to_decimal(ostatok_string);

          if (cnt_shift > 2) {
            result_string[place_bit_result++] = '0';
          }

          int tmp = strlen(val_1_string);
          if (!tmp) {
            if ((s21_is_less(ostatok, value_2) && !is_null_decimal(ostatok))) {
              result_string[place_bit_result++] = '0';
            } else if ((is_null_decimal(s21_string_to_decimal(val_1_string)) &&
                        is_null_decimal(ostatok))) {
              for (size_t i = 0; i < strlen(val_1_string); i++) {
                result_string[place_bit_result++] = '0';
              }
              for (size_t i = 0; i < strlen(ostatok_string); i++) {
                result_string[place_bit_result++] = '0';
              }
            }
            break;
          }

          if (is_null_decimal(s21_string_to_decimal(val_1_string)) &&
              is_null_decimal(ostatok)) {
            for (size_t i = 0; i < strlen(val_1_string) + 1; i++) {
              result_string[place_bit_result++] = '0';
            }
            break;
          }
        }
      } while (s21_is_greater_or_equal(ostatok, value_2));
      *result = s21_string_to_decimal(result_string);

      s21_set_scale(result, scale);
      if (sign) {
        s21_set_bit(result, 127);
      }
      if (scale > 0) {
        make_lower_scale(result);
      }
    }
  } else {
    if (!result) {
      status = ERR;
    } else {
      status = buffer_overflow(value_1, value_2, DIV);
    }
  }

  return status;
}
