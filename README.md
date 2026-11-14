# Sweet Strings

Sweet Strings is a header-only string library aimed for ease of use and performance!

## Features

+ `Header-Only` so you can simply copy the header file and use it in your project without any setup!
+ `User First` philsophy used because why not!
+ `Performance` focused. 

## Installing

Since its a header-only library, you can simply install the `sweetstrings.h` file present at the root of the repository and place it inside your project.

## Usage

The main type is `sstring`.

Declare a string using:
```c
sstring s = sstring_new("Hello World!");
```
or if you want to intialize a string with a specific length:
```c
sstring s = sstring_create(100);
```

> [!NOTE]
> `sstring`, by default, allocates its buffer on the heap!

`sstring` is not null-terminated. To get a null-terminated libc compatible string. Use:
```c
const char* c = sstring_cstr(&s);
```

When done with using the string, delete it by using:
```c
sstring_del(&s);
```

## Supported Methods

+ new
+ create
+ del
+ reserve
+ resize
+ len
+ cap
+ cstr
+ get
+ set
+ clear
+ fill
+ isempty