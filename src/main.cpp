/**
 * @file: main.cpp #TODO keep this? or rename
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: mytftpclient is implementation of client side of tftp protocol
 * @description: mytftpclient is a application which implements client side of
 * 							 tftp protocol. It can either read or
 * write a file from/to server which is specified, with number of other options
 * (see >help), on the command line.
 */

#include <string.h>
#include <iostream>
#include <string>

#include "input_structure.hpp"
#include "parse_input.hpp"
#include "tftp_client.hpp"

void init_struct(input_structure *store_args) {
	store_args->app_mode = 0;
  store_args->timeout = -1;
  store_args->multicast = false;
  store_args->size = -1;
  store_args->data_mode = -1;
  store_args->ip_address = "127.0.0.1";
  store_args->port_number = 69;
}

int main(int argc, char **argv) {
  std::string line;

  input_structure *store_args;
  while (1) {
    store_args = new input_structure;
    init_struct(store_args);
    std::cout << "> ";
    std::getline(std::cin, line);

    if (!strcmp(line.data(), "help")) {
			std::cout << "print help\n";
    	continue;
		}

    if (!strcmp(line.data(), "exit")) break;

    if (!parse_input(&line, store_args)) {
      delete store_args;
      fprintf(stderr, "Error: wrong input\n");
      continue;
    }

    start_tftp_clinet(store_args);

    delete store_args;
  }

  if (store_args != NULL) {
    delete store_args;
  }
  return 0;
}
