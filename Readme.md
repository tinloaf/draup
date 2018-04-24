Draup
=====

This is Draup, a compile-time plugin registration system in C++17, using [Boost.Hana](https://boostorg.github.io/hana/). Draup is simple to use, and since everything is computed at compile time, Draup should add no runtime overhead to your program.

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

Now, after including `plugin.hpp` somewhere, you can do this:

```c++
auto plugins = DRAUP_GET_REGISTERED();

/* The class of your plugin is available as "plugin". */
DRAUP_FOR_EACH(plugins, {
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

Draup uses C++17 and Boost.Hana. Consequentially, you will need:
* a fairly recent compiler (GCC 7.0 or clang 5.0 should do)
* Boost >= 1.61
