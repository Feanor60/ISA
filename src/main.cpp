/**
 * @file: main.cpp #TODO keep this? or rename
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: mytftpclient is implementation of client side of tftp protocol
 * @description: mytftpclient is a application which implements client side of
 * 							 tftp protocol. It can either read or write a file from/to server
 * 							 which is specified, with number of other options (see >help),
 * 							 on the command line.
 */


#include <iostream>
#include <string>
#include <string.h>

#include "check_input.hpp"
#include "input_structure.hpp"

int main(int argc, char **argv) {
	
	std::string line;
	std::string ErrMsg;

	input_structure *store_args = new input_structure;

	while(1) {
	
		std::cout << "> ";
		std::cin >> line;
		
		if(!strcmp(line.data(), "help"))
			std::cout << "print help\n";

		check_input(&line, &ErrMsg, store_args);
	}

	return 0;
}
