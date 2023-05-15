#include "../s21_decimal.h"

void make_lower_scale(s21_decimal *val) {
  if (s21_get_scale(*val) > 0) {
    int scale_retruned = s21_get_scale(*val);
    int sign = s21_get_sign(*val);
    s21_set_scale(val, 0);
    s21_remove_bit(val, 127);
    s21_decimal tmp_mult = {{10, 0, 0, 0}};
    s21_decimal tmp_res = {{0, 0, 0, 0}};
    s21_decimal tmp_val = {{0, 0, 0, 0}};
    s21_mod(*val, tmp_mult, &tmp_res);

    while (is_null_decimal(tmp_res) && scale_retruned > 0) {
      s21_div(*val, tmp_mult, &tmp_val);
      scale_retruned--;
      s21_cpy_decimal(&tmp_val, val);
    }

    s21_set_scale(val, scale_retruned);
    if (sign) {
      s21_set_bit(val, 127);
    }
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arith_result status = OK;

  if (result && buffer_overflow(value_1, value_2, MUL) == 0) {
    int tmp_cnt = 0;
    s21_decimal tmp_sum;
    init_decimal(&tmp_sum);
    init_decimal(result);

    int res_scale = s21_get_scale(value_1) + s21_get_scale(value_2);
    res_scale = (res_scale > 28) ? 28 : res_scale;

    s21_decimal ten = {{10, 0, 0, 0}};
    if (s21_is_equal(value_2, ten) && (res_scale > 0)) {
      *result = value_1;
      s21_set_scale(result, s21_get_scale(value_1) - 1);
    } else {
      for (int cnt_v_2 = 0; cnt_v_2 < 96; cnt_v_2++) {  // 96
        init_decimal(result);
        for (int cnt_v_1 = 0; cnt_v_1 < 96; cnt_v_1++) {  // 96
          int mul =
              s21_get_bit(value_1, cnt_v_1) * s21_get_bit(value_2, cnt_v_2);
          if (mul) {  // mul == 1
            s21_set_bit(result, cnt_v_1 + tmp_cnt);
          } else {  // mul == 0
            s21_remove_bit(result, cnt_v_1 + tmp_cnt);
          }
        }

        s21_add(*result, tmp_sum, result);
        s21_cpy_decimal(result, &tmp_sum);
        tmp_cnt++;
      }
      s21_set_scale(result, res_scale);
      if ((!s21_get_sign(value_1) && s21_get_sign(value_2)) ||
          (s21_get_sign(value_1) && !s21_get_sign(value_2))) {
        s21_set_bit(result, 127);
      }
      if (s21_get_scale(*result) > 0) {
        make_lower_scale(result);
      }
    }
  } else {
    status = buffer_overflow(value_1, value_2, MUL);
    if (result) {
      init_decimal(result);
    }
  }
  return status;
}
