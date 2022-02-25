+++
date = "2019-05-09T13:13:56-04:00"
title = "Do Not Use Mutable types a Python Default Arguments"
description = "Why you should (usually) avoid using mutable types as default arguments."
tags = [
  "Python",
  "programming"
]
meta_kw = [
  "Python mutable",
  "default arguments",
  "antipattern",
  "tutorial",
  "warning",
  "mutability",
  "immutable"
]
+++

A popular mistake writing Python is using mutable types as default variables in places
that have unintentional consequences. The following outlines a few of those scenarios.

## Mutability

To make a long story short, mutable data is data that can be modified (or "mutated")
in other parts of a program, and the changes are reflected on all other references to
that variable.

Python's mutable data-types are:

- `list` ie, `["foo", "bar"]`
- `dict` ie, `{"foo": "bar"}`
- `set` ie, `{"foo", bar}`

A very basic example:

```python
# This modifies `people`, a dict, a mutable data type without returning anything.
def pour_beer(people={}):  # <--- !!! UH OH
    people["alice"] = 1
    
# Start with a dict with just "bob" in it.
people_beers = {"bob": 2}
# Call a function that ends up mutating `people_beers`.
pour_beer(people_beers)
# Notice we have a new dict item.
print(people_beers)
# {'bob': 2, 'alice': 1}
```

As you can see, the function `pour_beer` modifies `people`, doesn't return anything, yet
the changes (aka mutations) are reflected in the original variable definition.

The same situation applies to a `list` and a `set` if you were to add/remove/replace an
item. On the other hand, if the variable was a `tuple`, this issue would not happen
since tuples are _not_ mutable (immutable). If you want to do something like "add" a new
value to an existing tuple, you are forced to essentially create an entirely new value
which does not have references to an existing value container.

What this often leads to is run-time errors that can be very difficult to debug. So much
so, entire languages and development paradigms avoid them all together. They are the
archenemy of reliably and predictable distributed systems.

## Mutable Types for Default Function Values :(

It is popular and tempting to use mutable datastructures as default variables for
function arguments since they can make sense as default values. For example:

```python
# If a `people` list is not supplied, default to an empty list.
def pour_beer(name, people=[]):  # <--- !!! UH OH
    """Append `name` to a list of `people`."""
    people.append(name)
    return people

# party1 contains alice and bob
party1 = pour_beer("alice")
party1 = pour_beer("bob", people=party1)

# party contains rose and mary
party2 = pour_beer("rose")
party2 = pour_beer("mary", people=party2)

# Observe each party has all people added.
print("party1", party1)
# party1 ['alice', 'bob', 'rose', 'mary']
print("party2", party2)
# party2 ['alice', 'bob', 'rose', 'mary']
```
    
What's really going on here is that `people=[]` is defining an empty list, but in
*module* scope rather than *function* scope. As in, the value `[]` is not initialized
to a new list every function call, but rather the value is initialized when the program
starts up and mutations are all done to a _single_ value for the lifetime of that
program execution.

## Mutable Types for Default Class Variables :(

In much the same way that functions with default values that are mutable can be
problematic, a very similar thing happens with class variables.

```python
class Person:

    # !!! UH OH
    names = []
    
    def add_name(self, name: str):
        self.names.append(name)
        
    def get_names(self):
        return self.names


alice = Person()
alice.add_name("alli")

bob = Person()
bob.add_name("bobby")
bob.add_name("bobo")

print(alice.get_names())
# ['alli', 'bobby', 'bobo']
print(bob.get_names())
# ['alli', 'bobby', 'bobo']
```

Similar to the preceding example, `names = []` is defining a _class_ level variable,
not an _instance_ variable. This means that value mutations live across and are
shared between all instances of the class.

## Mutable Types for Module Variables

This is the "uh oh" situation I see the most of since it makes a lot of sense to set
default values at the top of a module (Python file) so that they may be used in various
places across a file or program. The problem is, just like in preceding examples, that
the value is defined singularly and all mutations are to a single value reference that
lives for the lifetime of the program.

```python
# !!! UH OH
DEFAULT_EMAILS = ["me@ls3.io", "someone@ls3.io"]

def send_email(email):
    emails = DEFAULT_EMAILS
    emails.append(email)
    print(f"Sending emails to: {emails}")

send_email("yerboi@ls3.io")
# Sending emails to: ['me@ls3.io', 'someone@ls3.io', 'yerboi@ls3.io']
send_email("again@ls3.io")
# Sending emails to: ['me@ls3.io', 'someone@ls3.io', 'yerboi@ls3.io', 'again@ls3.io']
```

Since `DEFAULT_EMAILS` is mutable, `emails` is merely a _reference_ to the
variable, though it might look like `emails` is just a copy of the list.

This means that when we mutate the value within `send_email`, that we're actually
mutating the module level variable vkye and it will be forever mutated for the lifetime
of the program.

## Ways Around It

Since the crux if the problem is that mutable types basically pass a reference around,
the idea is that you create a completely _new_ value to avoid this. This in itself can
be equally as implicit and booby-trapped.

As we've seen, reassignment can actually just stash a _reference_ to a variable instead
of establishing a new value with no previous attachment. Then you end up with multiple
values that will all mutate each other upon values changing... what a mess!

```python
DEFAULT_EMAILS = ["me@ls3.io", "someone@ls3.io"]
email = DEFAULT_EMAILS
other_emails = email

print(id(DEFAULT_EMAILS))
# 4475096832
print(id(email))
#4475096832
print(id(other_emails))
#4475096832
```

As you can see above, all variables have the same "id", meaning they are pointing to the
same underlying data-structure.

A non-fussy way to do this is using `deepcopy`. This copies all data to a new value,
absent any reference to the original variable. This is a sure way to make sure the
overall data-structure is copied over, but also any nested mutable datastructures in
the case you have mutable types containing other mutable types as values (ie, a `dict`
of `dict`s).

```python
from copy import deepcopy

DEFAULT_EMAILS = ["me@ls3.io", "someone@ls3.io"]
emails = deepcopy(DEFAULT_EMAILS)
```

You may also just create a new version of the variable:

With a `list`:

```python
DEFAULT_EMAILS = ["me@ls3.io", "someone@ls3.io"]
emails = DEFAULT_EMAILS[:]

print(id(DEFAULT_EMAILS))
print(id(emails))
```

With a `dict`:

```python
CONTACTS = {
    "email": "me@ls3.io",
}
contacts = dict(CONTACTS.items())

print(id(CONTACTS))
print(id(contacts))
```

For a `dict`, `frozendict` is a great alternative to just not worry about the problem.
It is a drop-in replacement for `dict`.

```python
# assumes you've installed it with `pip install frozendict`
from frozendict import frozendict

def add_person(people, name):
    # this will blow up since you are trying to change it
    people[name] = "here!"

all_people = frozendict(foo="bar")
print(all_people)
# frozendict({'foo': 'bar'})
print(all_people["foo"])
# "bar"
print(all_people.items())
# dict_items([('foo', 'bar')])

# This will blow up since you are trying to change `all_people` directly.
add_person(all_people, "alice")
```

## Mutability Can Be Helpful

As mentioned, passing around mutable data-structures is analogous to passing around a
reference that, no matter where it gets updated, ends up reflecting value changes
amongst any holder of its reference, no matter where in the program it might live.

> Don't take pass-by-reference literally. In Python, it's all pass-by-object so get used
> to people correcting you on that.

You may do in-place updates on large mutable datastructures:

```python
my_huge_list = [...]
def increment_first(a_huge_list):
    a_huge_list[0] += 1
```

If this was a `tuple`, you'd need to initialize and allocate new resources to copy over
the data-structure entirely.

Sharing values over a larger scope can also be helpful for locks and caching. They may
also be used to avoid `return`ing data from a deep call stack... though this is usually
begging for bugs of people misunderstanding important context on how the code works.


