#include "../s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  conv_result status = CONVERTATION_OK;

  if (dst) {
    memset(&(dst->bits), 0, sizeof(dst->bits));
    if (s21_is_negative(src)) {
      s21_set_bit(dst, 127);
      src *= -1;
    }
    if (src <= INT32_MAX) {
      dst->bits[0] = src;
    } else {
      init_decimal(dst);
      status = CONVERTATION_ERROR;
    }

  } else {
    status = CONVERTATION_ERROR;
  }

  return status;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  conv_result status = CONVERTATION_OK;

  if (dst) {
    uint64_t buffer = 0;
    *dst = 0;
    int scale = s21_get_scale(src);
    int sign = s21_get_bit(src, 127);

    buffer = src.bits[0] + src.bits[1] * 4294967295 + src.bits[2] * 4294967295;
    if (scale != 0) {
      buffer /= (int)pow(10, scale);
    }

    if (buffer <= INT32_MAX) {
      buffer = (sign != 0) ? -buffer : buffer;
      *dst = (int32_t)buffer;

    } else {
      *dst = 0;
      status = CONVERTATION_ERROR;
    }
  } else {
    status = CONVERTATION_ERROR;
  }
  return status;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  conv_result status = CONVERTATION_OK;
  bool flag_minus = false;

  if (dst && !is_inf(src) && !is_nan(src)) {
    init_decimal(dst);

    if (src < 0) {
      src *= -1.0f;
      flag_minus = true;
    }

    float fl_int = 0.0;
    float fl_fract = modff(src, &fl_int);
    // printf("%f\n", fl_fract);
    src = s21_float_format((int)fl_int, fl_fract, dst);
    int scale_tmp = s21_get_scale(*dst);  // нужен буффер для установленого
                                          // скейла, после инт->дец пропадает

    // if (exp != 0) {
    //     s21_set_scale(dst, exp);
    // }
    s21_from_int_to_decimal(src, dst);
    s21_set_scale(dst, scale_tmp);
    if (flag_minus) {
      s21_set_bit(dst, 127);
    }
  } else {
    if (dst) init_decimal(dst);
    status = CONVERTATION_ERROR;
  }

  return status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  conv_result status = CONVERTATION_OK;

  if (dst) {
    uint64_t buffer = 0;
    *dst = 0;
    int scale = s21_get_scale(src);
    int sign = s21_get_bit(src, 127);

    buffer = src.bits[0] + src.bits[1] * 4294967295 + src.bits[2] * 4294967295;
    *dst = (float)buffer / powf(10.0, scale);

    if (buffer <= __FLT_MAX__) {
      *dst = (sign != 0) ? -*dst : *dst;
    } else {
      status = CONVERTATION_ERROR;
    }
  } else {
    if (dst) *dst = 0.0;
    status = CONVERTATION_ERROR;
  }

  return status;
}

int s21_float_format(int integer, float fract, s21_decimal *value) {
  char str_fract[255] = {0};
  char str_int[255] = {0};
  int cnt = 0;
  int exp = 0;

  if (fract != 0.0) {
    sprintf(str_fract, "%.9f", fract);
  }
  sprintf(str_int, "%d", integer);

  memmove(str_fract, str_fract + 2, strlen(str_fract));
  exp = strlen(str_int);
  strcat(str_int, str_fract);

  size_t cnt_sign_number = strlen(str_int);
  cnt = cnt_sign_number;

  if (cnt_sign_number > 7 && strlen(str_fract) > 0) {
    while (cnt != 7) {
      str_int[cnt--] = '\0';
    }
    if ((int)str_int[cnt] - '0' > 4) {  // okryglenie
      cnt--;
      if (str_int[cnt] == '9') {
        while (str_int[cnt] == '9') {
          str_int[cnt--] = '0';
        }
        if (cnt != -1) {
          str_int[cnt]++;
        } else {
          add_char_to_beginning(str_int, '1');
        }
      } else {
        str_int[cnt]++;
      }
      str_int[7] = '\0';
      cnt = strlen(str_int) - 1;
      while (str_int[cnt] == '0') {
        str_int[cnt--] = '\0';
      }
    } else {
      while (str_int[cnt] == '0') {
        str_int[cnt--] = '\0';
      }
    }
  }
  exp = (strlen(str_int) - exp);
  s21_set_scale(value, exp);

  return atoi(str_int);
}

void s21_decimal_to_string(char *dst, s21_decimal src) {
  if (dst) {
    bool flag = false;
    char tmp[255] = {'\0'};
    int lenght = 0;
    for (int cnt = 95; cnt >= 0; cnt--) {  // 95
      if (s21_get_bit(src, cnt)) {
        tmp[cnt] = '1';
        flag = true;
        lenght++;
      } else {
        if (flag) {
          tmp[cnt] = '0';
          lenght++;
        }
      }
    }

    if (lenght == 0) {
      dst[0] = '0';
    }

    for (size_t i = 0, j = strlen(tmp) - 1; i < strlen(tmp);
         i++, j--) {  // разворачиваем строку
      dst[i] = tmp[j];
    }
  }
}

s21_decimal s21_string_to_decimal(char *string) {
  s21_decimal result;
  init_decimal(&result);

  char tmp[255] = {'\0'};
  for (size_t i = 0, j = strlen(string) - 1; i < strlen(string);
       i++, j--) {  // разворачиваем строку
    tmp[i] = string[j];
  }

  for (int cnt = 0; cnt < 96; cnt++) {
    if (tmp[cnt] == '1') {
      s21_set_bit(&result, cnt);
    } else {
      s21_remove_bit(&result, cnt);
    }
  }

  return result;
}
