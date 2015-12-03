#ifndef __WILLCONTROLLER_H_
#define __WILLCONTROLLER_H_

#include <iostream>
#include "wiic/wiicpp.h"

class WiiController{
public:
	WiiController();
	~WiiController();
	void initialize();

private:
	std::vector<CWiimote> _wiimotes;
	CWii _wii;
};


#endif