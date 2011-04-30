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
	bml::node * n = new bml::tnode<uint32_t>(0, 999);
	doc->append_child(n);
	doc->write("/tmp/test_bml.bml");
	delete doc;

	doc = new bml::document("/tmp/test_bml.bml");
	std::cout << (*((doc->get_nodes_by_id(0)).front())).get<uint32_t>() << std::endl;

}
