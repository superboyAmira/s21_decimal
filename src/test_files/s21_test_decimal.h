#ifndef TEST_FILES_S21_TEST_DECIMAL_H_
#define TEST_FILES_S21_TEST_DECIMAL_H_

#include <check.h>

#include "../s21_decimal.h"

#define is_fin(x) __builtin_isfinite(x)
#define is_nan(x) __builtin_isnan(x)
#define is_inf(x) __builtin_isinf(x)

// enum func_state {
//   OK = 0,
//   INF = 1,
//   MIN_INF = 2,
//   DIV_BY_ZERO = 3,
//   SIGN_MINUS = 4,
//   SIGN_PLUS = 5,
//   ERR = 6
// };

#endif  // TEST_FILES_S21_TEST_DECIMAL_H_