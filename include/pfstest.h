/* SPDX-License-Identifier: LGPL-2.1-or-later */

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
   @include quickstart/1/quickstart.c
 
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
    @include output/quickstart1.txt

    Congratulations! You have compiled and run your first test.
 
 -# To get more detail about what was done, run your program with
    the @c -v flag:
    @include output/quickstart1v.txt
 
    From this we can see that one test named "first_test"
    was run, and that it passed.
 
    @note If your terminal supports ANSI color escapes, and you want
    PASS/FAIL results to be highlighted with color, pass the @c -c
    flag to your program. @c -c can be combined with @c -v or used
    by itself.
 
 @section fail Causing a test to fail
 
 The most basic way to cause a test to fail is by calling fail().
 
 -# In the body of test(first_test), add this line:
    @dontinclude quickstart/2/quickstart.c
    @skipline fail
 
    @note In order to get an example up and running quickly, and to
          demonstrate PFSTest's core features, we will use fail() for
          the first few examples. In real world situations you will
          probably rarely call fail() directly, as PFSTest offers a
          collection of more expressive assertion and matching tools.
    
 -# Rebuild and run. Now your program should produce output like this:
    @include output/quickstart2.txt
 
 @section cut Test-driving the development of a simple module
 
 We will use test-driven development to develop a simple code
 module.
 
 -# Delete the @c first_test block and the @c
    register_test(first_test) call from register_tests().
 
 -# Include a header file for the new module and write a short test to
    verify its functionality:
    @include quickstart/3/quickstart.c
 
 -# Attempt to build this program. You should get an error like
    "square.h: No such file or directory". This is the first test
    error that we're going to use to drive development.

 -# Create a header file, @c square.h, empty except for a standard header
    guard:
    @dontinclude quickstart/3/square.h
    @line ifndef
    @line define
    @skipline endif

 -# Attempt to build again. Now you should get a link error about
    an undefined reference to @c `square'. You may also get a warning
    about an implicit declaration of function @c `square'.

 -# Let's fix those errors:

    In @c square.h, add
    @dontinclude quickstart/3/square.h
    @skipline int square
    between the guard macros. Then create a new C file in your
    project, @c square.c and add an empty square() definition:
    @include quickstart/3f/square.c

 -# Try building now. Building should succeed, possibly with a
    warning about a missing return value. Now, run your test
    binary, and you should see this output:
    @include output/quickstart3f.txt

 -# Having seen that test fail, we can now write the code for it to
    pass:
    @include quickstart/3/square.c

 -# Build and run again, and you should no longer see a test failure
    reported.
    @include output/quickstart3.txt
    Congratulations, you've just test-driven the development of your
    first bit of code with PFSTest!

    @note This example was run with @c -v to make changed test result
    more obvious.

 @section assert Using Matchers and Assertions

 fail() provided a quick way to get started, but most real tests in
 PFSTest will be written using the Hamcrest-inspired assert_that and
 matchers. We'll start with a simple example and then explain the new
 operations it uses.

 -# Add a test for a function that cubes its argument:
    @include quickstart/4/quickstart.c

    @c should_cube_numbers contains three new PFSTest
    features:

    - assert_that(description, actual_value, matcher)

      Most PFSTest tests will use assert_that(). It simplifies and
      makes more regular the process of writing assertions and
      printing information about failures. The first argument is a
      description, which can be an empty string. If provided, it will
      be printed in failure messages along with the details of the
      test expectation and the actual value. The second argument is
      the value returned by the code under test, boxed so that it can
      be handled generically.  The third argument is a matcher that
      describes the expected value. In this example, equal_to() is
      used to create a matcher.

    - the_int()

      In the object-oriented languages where frameworks like xUnit and
      Hamcrest were initially developed, all types either inherit from
      a common @c Object class or can be encapsulated into an object
      which does. Since C lacks such a common ancestor type, all C
      primitive values must be passed to a value boxer like the_int()
      before being passed to PFSTest calls like assert_that(),
      equal_to(), or when(). Value boxers return a @c pfstest_value*
      which keeps track of the size of the data and knows how to print
      it in messages. For more examples of value boxers, see @ref
      include/pfstest-values.h and @ref include/pfstest-fp.h.

    - equal_to()

      Matchers are a flexible, composable means of describing what
      values produced by the code under test are
      acceptable. equal_to() returns a matcher which compares boxed
      data using memcmp() and boxed pointers with @c ==. For more
      examples of matchers, see @ref include/pfstest-matchers.h.

 -# Create a @c cube.c / @c cube.h pair, and add an incorrect
    implementation of @c `cube', to get past compilation and linking
    errors.
    @include quickstart/4f/cube.c

 -# Run the test runner and see the result of the output of the
    assertion failure:
    @include output/quickstart4f.txt

    Notice that in addition to the file and line number of the failing
    assertion, PFSTest prints the descriptive text that was passed to
    it, and a helpful description both of what was expected and what
    value was actually produced.

 -# Add working code to @c `cube':
    @include quickstart/4/cube.c

 -# Watch the tests pass:
    @include output/quickstart4.txt

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
