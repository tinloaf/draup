#include "draup.hpp"
#include "plugin.hpp"

#include <iostream>

int
main()
{
	DRAUP_FOR_EACH([&](auto type_container) {
		               using plugin = typename decltype(type_container)::type;
		               std::cout << "Plugin registered: " << plugin::get_name() << std::endl;
	               });
}
