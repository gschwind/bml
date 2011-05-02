/*
 * Copyright 2010 Benoit Gschwind <gschwind@gnu-log.net>
 *
 * This file is part of libbml.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the Lesser GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Lesser GNU General Public License for more details.
 *
 * You should have received a copy of the Lesser GNU General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef DOCUMENT_HXX_
#define DOCUMENT_HXX_

#include "node.hxx"

namespace bml {

class document : public node {

public:
	/* create document from file name */
	document(char const * filename);
	/* create an empty document */
	document();

	virtual ~document() { };
	/* write the document to a new file */
	void write(char const * filename) const;

};

}

#endif /* DOCUMENT_HXX_ */
