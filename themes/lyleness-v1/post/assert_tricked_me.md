---
title: "Python assert oops"
description: "derp."
date: "2016-10-19"
tags:
  - "programming"
  - "python"
meta_kw:
  - "assert"
  - "advice"
---

Today, I derped. I was refactoring a piece of code that used an overly complicated regex for
password validation. Specifically, refactoring a single `if/then` with a super generic
validation error message to a few specific assertions that will inform the User of a specific
validation failure.

## The Code to be Refactored

We want to make this bit of code a bit more robust and User friendly.

```python
if not re.match(LONG_CRAZY_REGEX, password):
    raise ValidationError('Invalid password.')
```

## The Fix: use `assert`

The fix is to be more explicit about what is being validated and reporting on the cause of the
failed assertion. This helps both Users and Developers.

Yup, it's more LoC and not as elegant as a regex that requires a PhD in discrete mathematics to
understand... BUT, it's far more explicit to the Developer what is going on and has the added
bonus of a nicer error message that can bubble up to the User.

The code change here is refactoring the `if` check and changing it to a `try/except` that wraps
a number of assertions.

```python
try:
    assert len(password) < 10, 'Password needs to be atleast 10 characters long.'
    # more assertions here.
except AssertionError as ex:
    raise ValidationError(str(ex))
```

Supposing the assertion failed (condition is falsy), this would `raise` a `ValidationError`
containing the error message for the failed assertion.

## The Introduction of the Bug

So, I happily began writing all my assertions. I noticed a few of the assertion failure essages
exceeded my standard of 100 character line length (believe it or not, they make monitors that can
handle >80 characters these days).

Clearly some cleanup was in order before I commit. I thought to myself, line break time.

```python
try:
    assert (
        len(password) < 10,
        'Password needs to be atleast 10 characters long.'
    )
    # more assertions here.
except AssertionError as ex:
    raise ValidationError(str(ex))
```

Awesome, right? No, not at all. In hindsight, this was a stupid mistake, honestly. 

You see, `assert` is a statement with a
[a specific grammar](https://docs.python.org/3/reference/simple_stmts.html#assert). My minds eye
was going back to how `return` can return a tuple like thing _or_ you can remove the parenthesis
and it will work just fine.

```python
# Same things.
return ('foo', 'bar')
return 'foo', 'bar'
```

That being said, `assert ('foo', 'bar')` will
*always* evaluate to `True`. The assertion is testing if a `tuple` containing 2 items is truthy,
which is always `True`. It's not just for grouping...

## Fix assertion Line Breakage

The fix is easy. Simply use a backslash (`\`) for line breaks.

```python
try:
    assert len(password) < 10, \
        'Password needs to be atleast 10 characters long.'
    # more assertions here.
except AssertionError as ex:
    raise ValidationError(str(ex))
```

To be fair, it *was* a Monday when it happened. Lesson learned!
