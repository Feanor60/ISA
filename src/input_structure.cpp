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

void add_timeout(input_structure *store_args, unsigned int timeout) {
  store_args->timeout = timeout;
}
