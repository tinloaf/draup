#include <iostream>

#include "draup.hpp"

#include "plugin.hpp"

/* Needed for hana::for_each */
#include <boost/hana.hpp>
namespace hana = boost::hana;

int
main()
{
	auto plugins = DRAUP_GET_REGISTERED();

	/* The class of your plugin is available as "plugin". */
	DRAUP_FOR_EACH(plugins, {
		std::cout << "Plugin registered: " << plugin::get_name() << std::endl;
	});

	/* This is equivalent, not using the macro:

	hana::for_each(plugins, [](auto plugin_cls_obj) {
	  using plugin = typename decltype(plugin_cls_obj)::type;

		std::cout << "Plugin registered: " << plugin::get_name() << std::endl;
	});
	*/
}