---
title: "Python try/except Can Be a Performant Way to Check for Type"
date: 2019-11-24T14:42:00-05:00
---

Say, you want to do some math with a variable. Math implies that the variable in the operation is a numerical value (we'll say in the typical case... don't mean tweet me...). Before doing math operations, you might want to sanity check the variavble so that your code will not blow up if the input is not numerical.

`isinstance` is a go-to for asking this _generally_. You just use it like `isinstance(some_variable, Type_Or_Class)` or to check a variable against multiple instance types you would just change the second argument to a tuple like `isinstance(some_variable, (Type_Or_Class1, Type_Or_Class2))`.

So, the usage would be as a _pre-check_ to validation:

```python
def validate(foobar):
    if not isinstance(foobar, (int, float)):
        raise ValueError('This input must be an int')
    
    return foobar + 10
```

As a fun-fact: `int` works here since it's in the method resolution order (mro) for the value.

```python
type(123).__mro__
> (<class 'int'>, <class 'object'>)
```

An opposite way to look at this is to ask for forgiveness rather than permission.

```python
def validate(foobar):
    try:
        return foobar + 10
    except:
        raise ValueError('This input must be a number!')
```

Whenever I stumble upon something like this, I like to break out `timeit` to compare.

```python
import timeit


N_ITERATIONS = 10000000

setup = """
# code that sets stuff up, that won't be included in profiling
from datetime import datetime
unix_epoch = datetime.now().timestamp()
"""

# good 'ol isinstance
test = """
isinstance(unix_epoch, (int, float))
"""
print(timeit.timeit(setup=setup, stmt=test, number=N_ITERATIONS))

# try to add to a number
test = """
try:
    unix_epoch += 1
except TypeError:
    pass
"""
print(timeit.timeit(setup=setup, stmt=test, number=N_ITERATIONS))

# just to be silly, also use divide
test = """
try:
    unix_epoch / 2
except TypeError:
    pass
"""
print(timeit.timeit(setup=setup, stmt=test, number=N_ITERATIONS))
```

On a random EC2 instance I SSh'ed into:

```shell
1.5472514729946852
0.43879494071006775
0.43832130171358585
```

My 2.7 GHz Quad-Core Intel Core i7 MacBook Pro:

```shell
1.748314122
0.5496298039999998
0.5205081649999999
```

Funny enough, the _silly_ thing i tried was actually the fastest. Since I was using this code in a REST API serializer that gets millions of hits a day, I went with _faster_.
