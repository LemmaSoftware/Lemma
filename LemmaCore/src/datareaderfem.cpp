/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/03/2013
  @version  $Id$
 **/

#include "datareaderfem.h"

namespace Lemma {

	DataReaderFem::DataReaderFem(const std::string &name) : DataReader(name) {
	}

	DataReaderFem::~DataReaderFem() {
	}

	DataReaderFem* DataReaderFem::New() {
		DataReaderFem* Obj = new DataReaderFem("DataReaderFem");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void DataReaderFem::Delete() {
		this->DetachFrom(this);
	}

	void DataReaderFem::Release() {
		delete this;
	}

	Data* DataReaderFem::GetData() {
		return 0;
		// I fucking hate virtual functions and they should die
	}

} // end of namespace Lemma

