#include "tests.h"

START_TEST(TEST_1) {
  static UserAction_t signals[] = {
      Start,  Start,  Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Left,   Left,   Left,   Left,   Left,     Left,   Down,
      Down,   Left,   Left,   Left,   Left,   Left,   Left,     Action, Right,
      Action, Right,  Right,  Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Left,   Left,     Left,   Left,
      Left,   Left,   Down,   Down,   Down,   Down,   Down,     Right,  Right,
      Right,  Right,  Right,  Right,  Right,  Right,  Right,    Right,  Right,
      Action, Action, Action, Action, Left,   Left,   Left,     Left,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Right,  Right,  Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Left,   Left,   Down,   Right,  Right,  Right,    Right,  Right,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Action, Action, Action, Action,   Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Action, Action, Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Down,     Down,   Down,
      Down,   Down,   Down,   Down,   Down,   Down,   Terminate};

  int i = 0;
  while (signals[i] != Terminate) {
    user_input(signals[i]);
    i++;
  }
  GameInfo_t info = update_current_state();
  ck_assert_int_eq(info.high_score, 100);
  ck_assert_int_eq(info.score, 100);
  ck_assert_int_eq(info.level, 1);
  ck_assert_int_eq(info.speed, 1);
  ck_assert_int_eq(info.pause, 0);
}
END_TEST

Suite *s21_test_1(void) {
  Suite *s = suite_create("\033[45m-=s21_test_1=-\033[0m");
  TCase *tc = tcase_create("s21_test_1");

  if (s != NULL && tc != NULL) {
    tcase_add_test(tc, TEST_1);
    suite_add_tcase(s, tc);
  }

  return s;
}
