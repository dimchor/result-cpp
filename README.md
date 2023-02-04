# Result

## Small summary

A small header-only library that provides Rust-style error handling in C++. It's
similar to `std::expected` (C++23).

`dlight::Result` is a class that contains a C-style `union` and an `enum class`.
The union may have either a return object or an error object. 

The `ok()` method returns `true` if it's not an error object. 

The `unpack()` method checks if the union has an error object (using `ok()`) and
if it does, it prints the error and immediately exits the program. Otherwise, it
returns the non-error object. It's like `unwrap()` in Rust. You can overload 
`operator<<` in your error class to make it print a custom message in 
`std::cerr`.

The `then()` method checks if the union is `ok()` and if it is, it runs a 
user-provided function (or lambda expression) that has the return object as 
argument. It's a nice way of turning something like this:
```c++
auto r = some_function();
if (!r.ok()) {
    // ...
}
auto obj = r.get_value();

r = do_this(obj);
if (!r.ok()) {
    //...
}
obj = r.get_value();
// and so on...
```
into this:
```c++
auto r = some_function()
    .then(do_this)
    .then(do_that);
// and so on... 
```
which looks much cleaner.

## Examples

There's an example in the examples folder which demonstrates some of the 
features of this library. More examples might be added in the future.

## Notes

Add `-std=c++20`, if needed.
