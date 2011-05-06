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

#include "exception.hxx"
#include "document.hxx"
#include <iostream>
#include <fstream>

namespace bml {

document::document(char const * filename) : node((uint64_t)0) {
	std::fstream f;
	char head[4];
	f.open(filename, std::fstream::in | std::fstream::binary);
	f.read(head, 4);
	if (head[0] != 'B' || head[1] != 'M' || head[2] != 'L' || head[3] != 0) {
		throw exception("bml signature not found");
	}
	new (this) node(f);
	f.close();
	if(id != 0)
		throw exception("bad bml format");
}

document::document() : node((uint64_t)0) {
}

void document::write(char const * filename) const {
	std::fstream f;
	f.open(filename, std::fstream::out | std::fstream::binary);
	char const head[] = { 'B', 'M', 'L', 0 };
	f.write(head, sizeof(head));
	this->node::write(f);
	f.close();
}

}
