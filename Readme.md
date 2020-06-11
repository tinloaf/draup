Draup
=====

This is Draup, a compile-time plugin registration system in C++17. Draup is simple to use, and since everything is computed at compile time, Draup should add no runtime overhead to your program.

Usage
-----

You just need to include `draup.hpp` from all files where you use Draup.
Then, you can register classes in the header files in which you declare them (or anywhere else, really). You can later get a list of all registered classes and perform some action on them (instatiate an object, call a static method, …)

Consider this `plugin.hpp` file that declares, defines and registers two very simple plugin classes:

```c++
class MyPlugin {
public:
	static const char * get_name() {
		return "MyPlugin";
	}
};

DRAUP_REGISTER(MyPlugin);

class MyOtherPlugin {
public:
	static const char * get_name() {
		return "MyOtherPlugin";
	}
};

DRAUP_REGISTER(MyOtherPlugin);
```

Note that the calls to `DRAUP_REGISTER` must be in the outermost namespace scope.

Now, after including `plugin.hpp` somewhere, you can do this:

```c++

DRAUP_FOR_EACH([&](auto type_container) {
	using plugin = typename decltype(type_container)::type;
	std::cout << "Plugin registered: " << plugin::get_name() << std::endl;
});
```

This should output:
```
Plugin registered: MyPlugin
Plugin registered: MyOtherPlugin
```

Everything is built at compile time, there are no pointers being juggled, no objects being created, nothing.

Requirements
------------

Draup uses C++17 plus the `__COUNTER__` macro. Thus, you will need a fairly recent compiler (GCC 7.0
or clang 5.0 should do).
