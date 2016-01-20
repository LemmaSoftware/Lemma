/* This file is part of Lemma, a geophysical modelling and inversion API
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     05/03/2010
  @version  $Id: banner.h 87 2013-09-05 22:44:05Z tirons $
 **/

#ifndef __BANNER_H
#define __BANNER_H
#include <iostream>
#include <ostream>
void banner(const std::string &name, const std::string &version,
		const std::string &usage);
#endif

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
