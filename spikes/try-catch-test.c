#define with_context(name) switch (name->test_jmp_buf)
#define TRY case 0:
#define FAIL break; case 1:

int main(void) {
    dynamic_env_t env;
    dynamic_env_push(&env);

    with_context(local_dynamic_env) {
        TRY {
            assert_that("dummy text", the_int(5), is_the_int(6));
            dynamic_env_pop();
            fail("Assertion did not fail as it should have");
        } FAIL {
            dynamic_env_pop();
            assert_that("the_int failure text is correct",
                        the_string(captured_output),
                        matches_the_nv_string(
                            "Expected: the int 6\n"
                            "Actual: the int 5\n"));
        }
    }
}
