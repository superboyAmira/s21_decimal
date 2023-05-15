#include "../s21_decimal.h"
#define MAX_DECIMAL 4294967295

int s21_floor(s21_decimal value, s21_decimal *result) {
  other_result status = OTHER_OK;

  if (result) {
    init_decimal(result);

    int sign = s21_get_sign(value);
    s21_decimal one_val = {{1, 0, 0, 0}};
    s21_decimal tmp_val = {{0, 0, 0, 0}};

    s21_truncate(value, &tmp_val);
    if (sign) {
      s21_sub(tmp_val, one_val, result);
    } else {
      *result = tmp_val;
    }
  } else {
    status = OTHER_ERROR;
  }
  return status;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  other_result status = OTHER_OK;

  if (result) {
    init_decimal(result);
    s21_set_scale(result, s21_get_scale(value));
    (s21_get_bit(value, 127) == 0) ? s21_set_bit(result, 127)
                                   : s21_remove_bit(result, 127);
    for (size_t place = 0; place < 96; place++) {
      if (s21_get_bit(value, place) == 0) {
        s21_remove_bit(result, place);
      } else {
        s21_set_bit(result, place);
      }
    }
  } else {
    status = OTHER_ERROR;
  }

  return status;
}

int round_up(s21_decimal fract, s21_decimal integer) {
  int status = 0;
  s21_decimal fract_first_symbol = {{0, 0, 0, 0}};
  s21_decimal five = {{5, 0, 0, 0}};
  s21_decimal two = {{2, 0, 0, 0}};
  s21_decimal tmp = {{0, 0, 0, 0}};
  s21_decimal tmp2 = {{0, 0, 0, 0}};
  s21_decimal ten = {{10, 0, 0, 0}};

  s21_truncate(fract, &fract_first_symbol);
  s21_mod(fract_first_symbol, ten, &tmp);

  if (s21_is_greater_or_equal(tmp, five)) {
    s21_mod(integer, two, &tmp2);
    if (is_null_decimal(tmp2) && s21_is_equal(tmp, five)) {
      status = 0;
    } else {
      status = 1;
    }
  }

  return status;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  other_result status = OTHER_OK;

  if (result) {
    init_decimal(result);

    if (s21_get_scale(value) == 0) {
      *result = value;
    } else {
      int sign = s21_get_sign(value);
      s21_remove_bit(&value, 127);
      s21_decimal tmp_val_int = {{0, 0, 0, 0}};
      s21_decimal tmp_val_fract = {{0, 0, 0, 0}};
      s21_decimal one_val = {{1, 0, 0, 0}};

      s21_truncate(value, &tmp_val_int);
      s21_sub(value, tmp_val_int, &tmp_val_fract);
      s21_set_scale(&tmp_val_fract, s21_get_scale(tmp_val_fract) - 1);

      if (round_up(tmp_val_fract, tmp_val_int)) {
        s21_truncate(value, &tmp_val_int);
        s21_add(tmp_val_int, one_val, result);
      } else {
        s21_truncate(value, result);
      }
      if (sign) {
        s21_set_bit(result, 127);
      }
    }
  } else {
    status = OTHER_ERROR;
  }

  return status;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  other_result status = OTHER_OK;

  if (result) {
    init_decimal(result);
    int val_scale = s21_get_scale(value);
    int sign = s21_get_sign(value);
    s21_set_scale(&value, 0);
    s21_decimal ten = {{10, 0, 0, 0}};

    for (int i = 0; i < val_scale; i++) {
      s21_div(value, ten, result);
      value = *result;
    }

    *result = value;

    if (sign) {
      s21_set_bit(result, 127);
    }
  } else {
    status = OTHER_ERROR;
  }
  return status;
}
