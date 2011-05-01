/*
 * test_bml.cxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
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
