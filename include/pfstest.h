#ifndef PFSTEST_H
#define PFSTEST_H

/** @mainpage
 
 PFSTest is a unit testing and mocking framework for C. It aims to
 come as close to the feature set and ease of use of Hamcrest and
 Mockito as possible, while still being small enough to be used on
 small embedded microcontrollers.
 
 @subpage quickstart
 */

/** @page quickstart Quickstart
 
 @tableofcontents
 
 @section intro Introduction
 
 While PFSTest is designed to be usable even on small embedded
 devices with limited or unusual IO capabilities, the quickest way
 to get started developing tests with PFSTest is natively on your
 development PC.
 
 PFSTest is designed to be modular and flexible, so you have some
 choices in what C files to include in your project. This guide will
 walk you through typical choices. The full documentation will
 explain which parts can be removed for a smaller footprint, and how
 to enable additional optional features.
 
 @section exe Building a command-line program that runs tests
 
 We will start by creating an executable meant to be run on the
 command-line which will execute our tests and report their results.
 
 -# Start a new C executable project in your favorite IDE or with a
    Makefile.
 
 -# From the PFSTest @c src directory, add the following files:
    - All C files in @ref src/core
    - All C files in @ref src/reporters
    - All C files in @ref src/cli
    - @c src/alloc/pfstest-alloc-malloc.c
    - @c src/main/main.c
 
 -# Add a new C file to your project:
   @code
   #include <pfstest.h>
 
   test(first_test)
   {
   }
 
   void register_tests(void)
   {
       register_test(first_test);
   }
   @endcode
 
   This test does nothing, and so will pass by default.
 
   @note register_tests() is called by the main() function defined in
         @c src/main/main.c before it invokes PFSTest's command line
         driver, pfstest_main().
 
   @note If you are compiling with GCC or Microsoft Visual C++, you
         can leave out the definition of register_tests(). PFSTest
         will detect these compilers and use language extensions to
         automatically run any test created with test().
 
 -# Compile and link your program.
 -# Run the program in a terminal. You should see output like this:
    @code{.txt}
    .
    Run complete. 1 passed, 0 failed, 0 ignored
    @endcode
 
    Congratulations! You have compiled and run your first test.
 
 -# To get more detail about what was done, run your program with
    the @c -v flag:
    @code{.txt}
    quickstart.c:first_test PASS
    Run complete. 1 passed, 0 failed, 0 ignored
    @endcode
 
    From this we can see that one test named "first_test"
    was run, and that it passed.
 
    @note If your terminal supports ANSI color escapes, and you want
    PASS/FAIL results to be highlighted with color, pass the @c -c
    flag to your program. @c -c can be combined with @c -v or used
    by itself.
 
 @section fail Causing a test to fail
 
 The most basic way to cause a test to fail is by calling fail().
 
 -# In the body of test(first_test), add these lines:
    @code
        fail("Intentional failure");
    @endcode
 
    @note In order to get an example up and running quickly, and to
          demonstrate PFSTest's core features, we will use fail()
          for the first few examples. In real world situations you
          will probably rarely call fail() directly, as PFSTest
          offers a collection of more expressive set of assertion
          and matching tools.
    
 -# Rebuild and run. Now your program should produce output like this:
    @code{.txt}
    quickstart.c:first_test FAIL
        Location: quickstart.c:5
        Intentional failure
    Run complete. 0 passed, 1 failed, 0 ignored
    @endcode
 
 @section cut Test-driving the development of a simple module
 
 We will use test-driven development to develop a simple code
 module.
 
 -# Delete the @c first_test block.
 
 -# Include a header file for the new module and write a short test to
    verify its functionality:
    @code
    #include "square.h"
 
    test(should_square_numbers)
    {
        if (square(5) != 25) {
            fail("square(5) didn't equal 25");
        }
    }
    @endcode
 
 -# Attempt to build this program. You should get an error like
    "square.h: No such file or directory". This is the first test
    error that we're going to use to drive development.

 -# Create a header file, square.h, empty except for a standard header
    guard:
    @code
    #ifndef SQUARE_H
    #define SQUARE H

    #endif
    @endcode

 -# Attempt to build again. Now you should get a link error about
    an undefined reference to `square'. You may also get a warning
    about an implicit declaration of function `square'.

 -# Let's fix those errors:

    In square.h, add @code int square(int x); @endcode between the
    guard macros. Then create a new C file in your project,
    square.c and add an empty square() definition:
    @code
    int square(int x)
    {
    }
    @endcode

 -# Try building now. Building should succeed, possibly with a
    warning about a missing return value. Now, run your test
    binary, and you should see this output:
    @code{.txt}
    quickstart.c:should_square_numbers FAIL
        Location: quickstart.c:8
        square(5) didn't equal 25
    @endcode

 -# Having seen that test fail, we can now write the code for it to
    pass:
    @code
    int square(int x)
    {
        return x * x;
    }
    @endcode

 -# Build and run again, and you should no longer see a test failure
    reported. Congratulations, you've just test-driven the
    development of your first bit of code with PFSTest!

 */

#include "pfstest-core.h"
#include "pfstest-alloc.h"
#include "pfstest-assert.h"

#include "pfstest-values.h"
#include "pfstest-matchers.h"
#include "pfstest-arg-handlers.h"

#include "pfstest-fp.h"
#include "pfstest-fp-printf.h"

#include "pfstest-mock.h"

#include "pfstest-reporters-standard.h"

#include "pfstest-shorthand.h"

#endif /* !PFSTEST_H */
