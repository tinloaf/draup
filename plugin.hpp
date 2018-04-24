//
// Created by Lukas Barth on 24.04.18.
//

#ifndef DRAUP_PLUGIN_HPP
#define DRAUP_PLUGIN_HPP

#include "draup.hpp"

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

#endif //DRAUP_PLUGIN_HPP
