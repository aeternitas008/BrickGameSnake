#include "tests.h"

int main() {
  int failed = 0;

  Suite *suite[] = {s21_test_1(), NULL};

  for (int i = 0; suite[i] != NULL; i++) {
    SRunner *sr = srunner_create(suite[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
