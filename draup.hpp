#ifndef DRAUP_DRAUP_HPP
#define DRAUP_DRAUP_HPP

#include <cstddef>
#include <type_traits>

namespace draup {

template <class T>
struct type_c
{
	using type = T;
};

namespace util {

template <class... Contents>
struct set
{
	template <class T>
	constexpr static bool contains();

	template <class NullaryFunction>
	static void for_each(NullaryFunction f) noexcept;

	template <class T>
	using add = set<T, Contents...>;
};

template <class Content, class... Rest>
struct set<Content, Rest...>
{
	template <class NullaryFunction>
	static void
	for_each(NullaryFunction f) noexcept
	{
		f(type_c<Content>{});
		set<Rest...>::for_each(f);
	}

	template <class T>
	constexpr static bool
	contains()
	{
		if constexpr (std::is_same_v<T, Content>) {
			return true;
		} else {
			return set<Rest...>::template contains<T>();
		}
	}

	template <class T>
	using add = set<T, Content, Rest...>;
};

template <>
struct set<>
{
	template <class NullaryFunction>
	static void
	for_each(NullaryFunction f) noexcept
	{
		(void)f;
	}

	template <class T>
	constexpr static bool
	contains()
	{
		return false;
	}

	template <class T>
	using add = set<T>;
};

template <class T>
constexpr auto
make_set()
{
	return set<T>{};
}

template <class T, class TheSet>
constexpr auto
set_insert()
{
	if constexpr (TheSet::template contains<T>()) {
		return TheSet{};
	} else {
		return typename TheSet::template add<T>{};
	}
}

} // namespace util

template <class>
struct sfinae_true : std::true_type
{
};

/* This template will be specialized by every plugin with its own integer.
 * The operator() of the respective struct should return a hana::set of all the
 * classes registered so far. */
template <unsigned int>
struct registry_hook;

/* Methods to test whether a registry_hook with a specified number is already
 * defined via SFINAE. */
template <class Dummy, unsigned int testN>
constexpr auto test_existence(int)
    -> sfinae_true<decltype(registry_hook<testN>{})>;
template <class Dummy, unsigned int testN>
constexpr auto test_existence(long) -> std::false_type;

/* Returns the smallest number N for which registry_hook<N> is not defined yet.
 * A per-plugin unique dummy class is necessary s.t. this struct is
 * re-instantiated for every plugin.
 */
template <class Dummy, unsigned int testN = 0>
constexpr unsigned int
get_free_N()
{
	/* The 'constexpr' is essential here. Otherwise, the template in the
	 * else-branch must be instantiated even if the condition evaluates to true,
	 * leading to infinite recursion. */
	if constexpr (std::is_same<decltype(test_existence<Dummy, testN>(0)),
	                           std::false_type>::value) {
		return testN;
	} else {
		return get_free_N<Dummy, testN + 1>();
	}
}

/* Helper struct / method to chain together the registered classes */
template <class ClassToRegister, unsigned int myN>
struct register_class
{
	/* General case: myN > 0. I.e., other classes have already been registered.
	 * We recursively get the set of these classes and append our own. */
	auto
	operator()()
	{
		return util::set_insert<ClassToRegister,
		                        typeof(registry_hook<myN - 1>{}())>();
	}
};
template <class ClassToRegister>
struct register_class<ClassToRegister, 0>
{
	/* Special case: myN == 0. No other classes have been registered. Create a new
	 * set. */
	auto
	operator()()
	{
		return util::make_set<ClassToRegister>();
	}
};

class GetterDummyClass {
};

template <class T>
struct plugin_getter
{
	auto
	operator()()
	{
		constexpr unsigned int next_N = get_free_N<T>();
		if constexpr (next_N == 0) {
			return util::set<>{};
		} else {
			// get the set of registered classes
			auto registered_classes = registry_hook<next_N - 1>{}();

			return registered_classes;
		}
	}
};

/*
 * A struct + variable template to provide a callable for_each symbol.
 */

template <class Dummy>
struct for_each
{
	template <class NullaryFunction>
	void
	operator()(NullaryFunction f)
	{
		using TheSet = typeof(plugin_getter<Dummy>{}());
		TheSet::for_each(f);
	}
};

template <std::size_t>
struct DraupDummy
{
};

} // namespace draup

/*
 * Macro to do the necessary incantation to register a new plugin
 */

#define DRAUP_REGISTER(classname)                                              \
	namespace draup {                                                            \
	template <>                                                                  \
	struct registry_hook<draup::get_free_N<classname>()>                         \
	{                                                                            \
		constexpr static unsigned int my_N = draup::get_free_N<classname>();       \
		auto                                                                       \
		operator()()                                                               \
		{                                                                          \
			return draup::register_class<classname, my_N>{}();                       \
		}                                                                          \
	};                                                                           \
	}

#define DRAUP_GET_REGISTERED()                                                 \
	::draup::plugin_getter<::draup::GetterDummyClass>{}()

#define DRAUP_FOR_EACH(f)                                                      \
	::draup::for_each<::draup::DraupDummy<__COUNTER__>>{}(f)

#endif // DRAUP_DRAUP_HPP
