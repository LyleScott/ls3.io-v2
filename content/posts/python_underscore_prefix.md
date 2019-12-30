---
title: "Python and the Underscore Prefix"
description: ""
date: 2015-02-08T00:42:06-04:00
tags:
  - "programming"
  - "python"
meta_kw:
  - "python"
  - "development"
  - "underscore"
  - "hidden"
  - "protected"
---

Underscore prefixes in Python provide a way to protect functions, methods, and variables...kinda. In Python, any notion of private variables simply does not exist. There are, though, some pythonic ways to declare that a variable, function, or method shouldn't be consumed outside of where it is being directly used.

## Single Underscore

When you prefix something with a single underscore, it **politely** asks developers that interact with that code that the thing being prefixed should not be used in any direct manner other than calling it within the scope for which it was defined in. If you see it in third-party code, it means that you should not use or depend on it in any way.

Note, though, that the thing having a single underscore prefix can still be used as if it didn't have an underscore in the name, so having the prefix underscore is only symbolic and only represents some advice that hopefully people adhere to.

For example, 

```python
class FooBar(object):
    foo = 'abc123'
    _bar = 'qwerty'

    def foofunc(self):
        print('foofunc!')

    def _barfunc(self):
         print('barfunc!')
```

The internal representation of the class looks like you think it would, listing the names of the defined variables and methods exactly as they were defined.

```python
print(dir(FooBar))
[
 ...,
 '_bar',
 '_barfunc',
 ...,
 'foo',
 'foofunc'
 ...,
]
```

Abuse is easy, though. You can still use the single underscore prefixed things with their name like you would any other variable or method. Get why I mentioned it was based off the honor system?

```python
foobar = FooBar()
foobar.foofunc()           # foofunc!
foobar._barfunc()          # barfunc!
print(foobar._bar)         # qwerty
foobar._bar = 'hello'
print(foobar._bar)         # hello
```

This example was with class methods and variables. The same rules apply to a variable or function definition in the module's scope.

## Double Underscore

When you prefix something with a double underscore, it **sternly** implies to developers that interactict with that code that the thing being prefixed should absolutely and positively not be used in any direct manner other than calling it within the scope for which it was defined in.

The thing having a double underscore prefix becomes mangled, meaning that the class's variable or method gets renamed internally to protect the variable from being used directly. Like a single underscore prefix, this protection is only symbolic and is still based on the honor system, thought it is harder to use the variable or method.

Abuse is still possible, especially given that the result of manging always has the same pattern: `_TheClassName` is internally prefixed to the internal attribute.

For example,

```python
class FooBar(object):
    foo = 'abc123'
    __bar = 'qwerty'

    def foofunc(self):
        print('foofunc!')

    def __barfunc(self):
         print('barfunc!')
```

The variable `__bar` and `__barfunc` are both mangled internally.

```python
print(dir(FooBar))
[
 ...,
 '_FooBar__bar',
 '_FooBar__barfunc',
 ...
 'foo',
 'foofunc'
 ...,
]
```

As you can see, `__bar` and `__barfunc` are mangled using the FooBar classname. This makes direct access more difficult and deliberate.

```python
foobar = FooBar() 
foobar.__barfunc()         # AttributeError: 'FooBar' object has no attribute '__barfunc
foobar._FooBar__barfunc()  # barfunc!
```

Though access is possible, it is far from good practice!
