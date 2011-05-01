/*
 * document.cxx
 *
 *  Created on: 1 mai 2011
 *      Author: gschwind
 */

#include "exception.hxx"
#include "document.hxx"
#include <iostream>
#include <fstream>

namespace bml {

document::document(char const * filename) : node((uint64_t)-1) {
	std::fstream f;
	char head[4];
	f.open(filename, std::fstream::in | std::fstream::binary);
	f.read(head, 4);
	if (head[0] != 'B' || head[1] != 'M' || head[2] != 'L' || head[3] != 1) {
		throw exception("bml signature not found");
	}
	std::cout << "toto" << std::endl;
	new (this) node(f);
	std::cout << "titi" << std::endl;
	f.close();
	if(id != -1)
		throw exception("bad bml format");
}

document::document() : node((uint64_t)-1) {
	std::cout << "toto" << data_size << std::endl;
}

void document::write(char const * filename) const {
	std::fstream f;
	f.open(filename, std::fstream::out | std::fstream::binary);
	char const head[] = { 'B', 'M', 'L', 1 };
	f.write(head, sizeof(head));
	prepare_write();
	this->node::write(f);
	f.close();
}

}
