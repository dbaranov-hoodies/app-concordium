#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cmocka.h>

static void dummy_test(void **state)
{
    (void) state;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(dummy_test),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
