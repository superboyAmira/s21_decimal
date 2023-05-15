#include "../s21_decimal.h"

int s21_get_bit(s21_decimal num, int place) {
  unsigned int mask = 1u << (place % 32);
  int a = ((num.bits[place / 32] & mask) >> (place % 32));
  return a;
}

int s21_set_bit(s21_decimal *num, int place) {
  unsigned int mask = 1u << (place % 32);
  return num->bits[place / 32] = num->bits[place / 32] | mask;
}

int s21_remove_bit(s21_decimal *num, int place) {
  unsigned int mask = 1u << (place % 32);
  return num->bits[place / 32] = num->bits[place / 32] & ~mask;
}

int s21_get_scale(s21_decimal num) {
  int mask = 127 << 16;
  int scale = (mask & num.bits[3]) >> 16;
  return scale;
}

void s21_set_scale(s21_decimal *num, int exp) {
  int mask = exp << 16;
  num->bits[3] = mask;
}

int s21_is_negative(int src) { return (src < 0); }

void init_decimal(s21_decimal *decimla_num) {
  memset(&(decimla_num->bits), 0, sizeof(decimla_num->bits));
}

void add_char_to_beginning(char *str, char ch) {
  char temp[2] = {ch, '\0'};
  strcat(temp, str);
  strcpy(str, temp);
}

bool is_null_decimal(s21_decimal num) {
  bool status = false;
  if (num.bits[0] == 0 && num.bits[1] == 0 && num.bits[2] == 0) {
    status = true;
  }
  return status;
}

void eq_scale(s21_decimal *dst1, s21_decimal *dst2) {
  unsigned int scale1 = s21_get_scale(*dst1);
  unsigned int scale2 = s21_get_scale(*dst2);
  s21_decimal ten = {{0, 0, 0, 0}};
  s21_decimal tmp = {{0, 0, 0, 0}};
  if (scale1 > scale2) {
    s21_set_scale(dst2, 0);
    for (int i = scale1 - scale2; i > 0; i--) {
      s21_from_int_to_decimal(10, &ten);
      s21_mul(*dst2, ten, &tmp);
      *dst2 = tmp;
    }
    s21_set_scale(dst2, scale1);
  }
  if (scale1 < scale2) {
    s21_set_scale(dst1, 0);
    for (int i = scale2 - scale1; i > 0; i--) {
      s21_from_int_to_decimal(10, &ten);
      s21_mul(*dst1, ten, &tmp);
      *dst1 = tmp;
    }
    s21_set_scale(dst1, scale2);
  }
}

void s21_cpy_decimal(s21_decimal *src, s21_decimal *dst) {
  for (int place = LOW; place <= SCALE; place++) {
    dst->bits[place] = src->bits[place];
  }
}

int s21_get_sign(s21_decimal number) {  // узнать знак
  int sign = s21_get_bit(number, 127);
  return sign;
}

int buffer_overflow(s21_decimal val_1, s21_decimal val_2, operator oper) {
  arith_result status = OK;

  if (oper == PLUS) {
    long long int res =
        (long long int)val_1.bits[HIGH] + (long long int)val_2.bits[HIGH];
    if (res > MAX_BITS_VAL) {
      status = INF;
      if (s21_get_bit(val_1, 127) == 1 && s21_get_bit(val_2, 127) == 1) {
        status = MIN_INF;
      }
    }
  } else if (oper == MIN) {
    long long int res = 0;
    if ((s21_get_sign(val_1) && !s21_get_sign(val_2)) ||
        (!s21_get_sign(val_1) && s21_get_sign(val_2))) {
      res = (long long int)val_1.bits[HIGH] + (long long int)val_2.bits[HIGH];
    } else {
      res = (long long int)val_1.bits[HIGH] - (long long int)val_2.bits[HIGH];
    }
    if (res > MAX_BITS_VAL) {
      status = INF;
      if (s21_get_bit(val_1, 127) == 1 && s21_get_bit(val_2, 127) == 1) {
        status = MIN_INF;
      }
    }
    // нужно дописать как в муле
  } else if (oper == MUL) {
    int i = 95;
    int j = 95;
    while (s21_get_bit(val_1, i) != 1 && i != 0) {
      i--;
    }
    while (s21_get_bit(val_2, j) == 0 && j != 0) {
      j--;
    }
    if (i + j >= 96) {
      if ((s21_get_sign(val_1) == 1 && s21_get_sign(val_2) == 0) ||
          (s21_get_sign(val_1) == 0 && s21_get_sign(val_2) == 1)) {
        status = MIN_INF;
      } else {
        status = INF;
      }
    }
  } else if (oper == DIV) {
    if (is_null_decimal(val_1)) {
      status = INF;
    } else if (is_null_decimal(val_2)) {
      status = DIV_BY_ZERO;
    } else {
      int i = 95;
      int j = 95;
      while (s21_get_bit(val_1, i) != 1 && i != 0) {
        i--;
      }
      while (s21_get_bit(val_2, j) == 0 && j != 0) {
        j--;
      }
      if (i + j >= 96 && (s21_get_scale(val_2) > 0)) {
        if ((s21_get_sign(val_1) == 1 && s21_get_sign(val_2) == 0) ||
            (s21_get_sign(val_1) == 0 && s21_get_sign(val_2) == 1)) {
          status = MIN_INF;
        } else {
          status = INF;
        }
      }
    }
  }

  return status;
}
