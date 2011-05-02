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

#include "document.hxx"
#include <iostream>

int main(int argc, char ** argv) {
	bml::document * doc = new bml::document();
	bml::node * n;
	doc->append_child(new bml::tnode<uint32_t>(1, 999));
	doc->append_child(new bml::tnode<uint64_t>(2, 999));
	doc->append_child(new bml::tnode<uint64_t>(3, 300));
	(*doc)[3].append_child(new bml::tnode<std::string>(3, "ho my good"));
	(*doc)[3].append_child(new bml::tnode<char>(3, 32));
	doc->write("/tmp/test_bml.bml");
	delete doc;
	doc = new bml::document("/tmp/test_bml.bml");
	std::cout << (*doc)[1].get<uint32_t>() << std::endl;
	std::cout << *((*doc)[3][3]) << std::endl;
}
