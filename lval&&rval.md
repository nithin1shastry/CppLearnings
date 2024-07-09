Ref - https://www.internalpointers.com/post/understanding-meaning-lvalues-and-rvalues-c

# Simple def of lvalues and rvalues
- lvalue is something that points to a specific memory location
- rvalue is something that doesn't point anywhere
- lvalues can be assumed as containers whereas rvalues are things contained in containers

```
int x = 100; Valid
int* y = &x;

Suppose int* y = &x; & require lvalue because only lvalue has an address that & can process

```
# Functions returning lvalue and rvalue

```

int setValue()
{
    return 6;
}

// ... somewhere in main() ...

setValue() = 3; // error!


int global = 100;

int& setGlobal()
{
    return global;    
}

// ... somewhere in main() ...

setGlobal() = 400; // OK

Watch out for & here: it's not the address-of operator, it defines the type of what's returned (a reference).
```

# Lvalue and rvalue conversion

```
int x = 1;
int y = 3;
int z = x + y;   // ok

Here x and y has undergone implicit lvalue to rvalue conversion
```
**QUESTION - Can an rvalue be converted to lvalue?**
```
int y = 10;
int& yref = y;
yref++;

How about int& yref = 10;
& points to lvalue, an object stored in memory, in this case 10 being numeric constant which isn't stored in memory
```

# Const lvalue reference

```
void fnc(int& x)
{
}

int main()
{
    fnc(10);  // Nope!
    // This works instead:
    // int x = 10;
    // fnc(x);
}


According to the language specifications, you are allowed to bind a const lvalue to an rvalue.

void fnc(const int& x)
{
}

int main()
{
    fnc(10);  // OK!
}
```

- The idea is simple
  -- literal constant 10 is volatile and referencing it is meaningless as it expires in no time
  -- Let's make the reference itself a constant instead, so that the value it points to can't be modified. Now the problem of modifying an rvalue is solved for good

```
Under the hood the compiler creates an hidden variable for you (i.e. an lvalue) where to store the original literal constant, and then bounds that hidden variable to your reference.

That's basically the same thing I did manually in a couple of snippets above. For example:

// the following...
const int& ref = 10;

// ... would translate to:
int __internal_unique_name = 10;
const int& ref = __internal_unique_name;

```
