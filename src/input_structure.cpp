/**
 * @file: input_structure.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubl08
 * @brief: implementation of functions which fill input_structure
 */

#include "input_structure.hpp"

void add_app_mode_read(input_structure *store_args) {
  store_args->app_mode = 1;
}

void add_app_mode_write(input_structure *store_args) {
  store_args->app_mode = 2;
}

void add_file_name(input_structure *store_args, std::string *file) {
  store_args->file_name = *file; 
}

void add_timeout(input_structure *store_args, int timeout) {
  store_args->timeout = timeout;
}

void add_size(input_structure *store_args, int size) {
  store_args->size = size;
}

void add_multicast(input_structure *store_args) {
  store_args->multicast = true;
}

void add_data_mode(input_structure *store_args, int data_mode) {
  store_args->data_mode = data_mode;
}

void add_ip_address(input_structure *store_args, std::string ip_address) {
  store_args->ip_address = ip_address;
}

void add_port_number(input_structure *store_args, std::size_t port_n) {
  store_args->port_number = port_n;
}
