# Official Team Code Style Conventions
*A lot of this is borrowed from [Github's Ruby styleguide](https://github.com/styleguide/ruby)*

## General Style

- Use tab characters, with a 2 space indent. 
  - Choosing Edit > Code Formatting > Tabify Whole File in RobotC will convert indenting spaces to tab characters
  - Choosing Edit > Code Formatting > Format Whole File in Robotc will fix any indentation errors
- Keep lines fewer than 100 characters.
- Never leave trailing whitespace.
- End each file with a single blank newline.
- Use spaces around operators, after commas, colons and semicolons, around `{` and before `}`.
  - Exception: To show arithmatic precedence. ex: `4*2 + 16`
- No spaces after `(`, `[` or before `]`, `)`.
- No spaces after `!`.
- Format `switch` statements as follows
  ```c
  switch (i) {
  case 0:
    ...
    break;
  case 1:
    ...
    break;
  }
  ``` 
- Follow [K&R Style](http://en.wikipedia.org/wiki/Indent_style#K.26R_style) for Indentation Style
  ```c
  int main(int argc, char *argv[])
  {
    ...
    while (x == y) {
      something();
      somethingelse();
 
      if (some_error) {
        do_correct();
      } else {
        continue_as_usual();
      }
    }

    finalthing();
    ...
  }
  ```
- Use empty lines between function definitions

### Comments

- Line end comments should be formatted as such:
  ```c
  code(foo, bar); // Comment is here!
  ```
- File start comments: 50 dashes at the top. 4 spaces before file name, 6 before description
  ```c
  /*--------------------------------------------------
  *
  *	    Color v. 1
  *	      utility for interpreting color sensor
  *	      values.
  *
  *------------------------------------------------*/
  ```

## Syntax

- Avoid the ternary operator (`?:`) except in cases where all expressions are extremely trivial. However, do use the ternary operator(`?:`) over `if() {} else {}` constructs for single line conditionals.
- Use one expression per branch in a ternary operator. This also means that ternary operators must not be nested. Prefer `if/else` constructs in these cases.

  ```c
  // bad
  some_condition ? (nested_condition ? nested_something : nested_something_else) : something_else

  // good
  if (some_condition) {
    nested_condition ? nested_something : nested_something_else
  } else {
    something_else
  }
  ```
- Avoid multi-line `?:` (the ternary operator), use `if/unless` instead.
- Never put a space between a function name and the opening parenthesis.

## Naming

- Use `snake_case` for local variables and functions defined in implemetation files
- Use `headlessCamelCase` for global variables and functions defined in headers/by RobotC.
- Use `SCREAMING_SNAKE_CASE` for macros (`#define`), motor/sensor names, and constants.

**Use Meaningful Names!**
If you have a variable for the distance traveled consider naming it something obvious like “distance_traveled”.  Names like “dist” or “dstTvl” are ambiguous and unclear.

**Booleans should be named with a helper verb**
such as “is”, “has”, etc. which helps illustrate what the positive state of the boolean represents/indicates.  Adding in a verb allows you to mentally ask the boolean a question that its true/false state can answer.  If you have a boolean flag to indicate that your robot has reached a position, poor name choices would include “position”, “posFlag”, or “positionNotReached”.  A better name choices might be “is_position_reached”.  A reader can instantly understand what a true/false value in the variable means:  is the position reached?

## Specific Rules

- `wait10Msec()` is depricated. Use `wait1Msec()` instead. Why? It doesn't make sense to count in intervals of 10 milliseconds. If you use milliseconds, it's much more understandable. 

&copy; 2014 i²robotics. All rights reserved.
