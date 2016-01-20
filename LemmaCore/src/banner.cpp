/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     05/03/2010
  @version  $Id: banner.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "banner.h"

///Prints banner information to the screen
void banner(const std::string &name, const std::string &version,
		const std::string &usage) {

	std::cout << std::endl;
	std::cout << "  " << name.c_str() << version.c_str() << std::endl;
	std::cout << std::endl;
	std::cout << "  A program built from Lemma v 1.0b" << std::endl;
	std::cout << "  Usage: " << usage.c_str() << std::endl;
	std::cout << std::endl;
	std::cout << "  Developed jointly by the Center for Gravity, " <<
		"Electrical, and Magnetic" <<std::endl;
	std::cout << "   Studies (CGEM), the United States Geologic " <<
		"Survey (USGS), and Broken" << std::endl;
	std::cout << "   Spoke Development, LLC." <<std::endl;
	std::cout << std::endl;
	std::cout << "  (C) Copyright 2010, Trevor Irons and M. Andy Kass.  " <<
		"Some rights reserved." << std::endl;
	std::cout << "  Please see license for details." << std::endl;
	std::cout << std::endl;
}
