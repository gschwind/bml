/*
 * exception.hxx
 *
 *  Created on: 30 avr. 2011
 *      Author: gschwind
 */

#ifndef EXCEPTION_HXX_
#define EXCEPTION_HXX_

#include <exception>

namespace bml {

class exception : public std::exception {
	char const * const _msg;
public:
	exception(char const * msg) : _msg(msg) { }
	char const * what() const throw() {
		return _msg;
	}
};

}

#endif /* EXCEPTION_HXX_ */
