---
title: "Python Class and Instance Variables"
description: ""
date: 2014-12-02T00:42:06-04:00
tags:
  - "programming"
  - "python"
meta_kw:
  - "python"
  - "development"
  - "instance"
  - "class"
  - "difference"
  - "scoping"
  - "side effect"
---

There are a million topics written on this, so I'm not going to delve into gory details. Instead, just checkout the snippet below. I think it says it all. 

A class variable is a variable that is shared between all instances of a class. You access it by using the class's name in the dotted reference, rather than self (unless you are in a class method, where the self or cls argument could be used instead).  For example, if Car is a class with a variable number_tires and Honda, Jaguar, and VolksWagon were all instances, if number_tires was changed in any of the instances, then the new value would be reflected when I assessed it from Honda, Jaguar, or VolksWagon instances.

An instance variable is scoped to a single instance of a class. Meaning, if Car is a class with a variable number_tires and Honda, Jaguar, and VolksWagon were all instances, if number_tired was changed in the Honda instance, then the new value would ONLY be reflected in the Honda instance and the Jaguar and VolksWagon would be left to what ever value they were.

Accessing a class variable with the self (instance) reference copies that variable into the instance's scope.

```python
class FooBar(object):
    foo = 0
    bar = 0

    def __init__(self):
       FooBar.foo += 1
       self.bar += 1
       self.foo += 1

    def __str__(self):
        return '\n'.join((
            '--------',
            'FooBar.foo \t {} \t (class variable)'.format(FooBar.foo),
            'self.bar \t {} \t (instance variable)'.format(self.bar),
            'self.foo \t {} \t (instance variable)'.format(self.foo),
        ))
```

As you can see,

* the class variable was incremented accross all instances
* the instance variable was only incremented for the single instance that it was being used in
* (bonus) a class variable used as an instance variable gets copied to the instance's scope and does not effect the class version if you were to alter it

```python
--------
FooBar.foo       1       (class variable)
self.bar         1       (instance variable)
self.foo         2       (instance variable)
--------
FooBar.foo       2       (class variable)
self.bar         1       (instance variable)
self.foo         3       (instance variable)
--------
FooBar.foo       3       (class variable)
self.bar         1       (instance variable)
self.foo         4       (instance variable)
```
