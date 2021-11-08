/**
 * @file: parse_input.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: functions for checking if the user input is correct
 */

#include "parse_input.hpp"

#include <bits/c++config.h>

#include <string>

bool parse_input(std::string *line, input_structure *store_args) {
  /* expected user input flags */
  std::string write("-W");
  std::string read("-R");
  std::string file("-d");
  std::string timeout("-t");
  std::string size("-s");
  std::string multicast("-m");
  std::string data_mode("-c");
  std::string address("-a");
  std::string file_name;

  std::size_t port_n;
  std::size_t pos;

  /* look for arguments in user input*/
  /* app mode */
  pos = line->find(write);
  if (pos != std::string::npos) {
    add_app_mode_write(store_args);
  }

  pos = line->find(read);
  if (pos != std::string::npos) {
    add_app_mode_read(store_args);
  }

  /* transfered file */
  pos = line->find(file);
  if (pos != std::string::npos) {
    get_file_name(line, &file_name, &pos);
    add_file_name(store_args, &file_name);
  } else {
    return false;
  }

  /* if one of these was not set print error and return false */
  if (!test_input(store_args)) return false;

  pos = line->find(timeout);
  if (pos != std::string::npos) {
    int seconds = get_timeout(line, &pos);
    if (seconds == -1) return false;

    add_timeout(store_args, seconds);
  }

  pos = line->find(multicast);
  if (pos != std::string::npos) {
    add_multicast(store_args);
  }

  pos = line->find(size);
  if (pos != std::string::npos) {
    int size = get_size(line, &pos);
    if (size == -1) return false;

    add_size(store_args, size);
  }

  pos = line->find(data_mode);
  if (pos != std::string::npos) {
    int mode = get_data_mode(line, &pos);
    if (mode == -1) return false;

    add_data_mode(store_args, mode);
  }

  pos = line->find(address);
  if (pos != std::string::npos) {
    std::string ip_address;

    if (!get_ip_address(line, &ip_address, &pos)) return false;

    add_ip_address(store_args, ip_address);
    if (!get_port_number(line, &port_n, &pos)) return false;

    add_port_number(store_args, port_n);
  }

  std::cout << "\n\n------ LOG ------\n";
  std::cout << "app mode: " << store_args->app_mode << "\n";
  std::cout << "file: " << store_args->file_name << "\n";
  std::cout << "timeout: " << store_args->timeout << "\n";
  std::cout << "multicast: " << store_args->multicast << "\n";
  std::cout << "data mode: " << store_args->data_mode << "\n";
  std::cout << "packet size: " << store_args->size << "\n";
  std::cout << "ip address and port: " << store_args->ip_address << "\n";
  std::cout << "ip address and port: " << store_args->port_number
            << "\n-----------------\n";

  return true;
}

bool get_file_name(std::string *line, std::string *file_name,
                   std::size_t *pos) {
  if (!get_arg(line, file_name, pos)) return false;

  return true;
}

int get_timeout(std::string *line, std::size_t *pos) {
  std::string timeout_str;
  if (!get_arg(line, &timeout_str, pos)) {
    return -1;
  }

  return stoi(timeout_str);
}

int get_size(std::string *line, std::size_t *pos) {
  std::string size_str;
  if (!get_arg(line, &size_str, pos)) {
    return -1;
  }

  return stoi(size_str);
}

int get_data_mode(std::string *line, std::size_t *pos) {
  std::string mode_str;
  std::string ascii("ascii");
  std::string netascii("netascii");
  std::string binary("binary");
  std::string octet("octet");

  if (!get_arg(line, &mode_str, pos)) {
    return -1;
  }

  if (!strcmp(mode_str.data(), "ascii")) return 1;

  if (!strcmp(mode_str.data(), "netascii")) return 2;

  if (!strcmp(mode_str.data(), "binary")) return 3;

  if (!strcmp(mode_str.data(), "octet")) return 4;

  return -1;
}

bool get_ip_address(std::string *line, std::string *ip_address,
                    std::size_t *pos) {
  if (!get_arg(line, ip_address, pos)) {
    fprintf(stderr, "Error: missing ip address\n");
    return false;
  }

  return true;
}

bool get_port_number(std::string *line, std::size_t *port_n, std::size_t *pos) {
  std::string temp;
  *pos = line->find(",");

  if (*pos != std::string::npos) {
    *pos = *pos - 2;
    if (!get_arg(line, &temp, pos)) {
      fprintf(stderr, "Error: missing port number\n");
      return false;
    }
  } else {
    fprintf(stderr, "Error: missing port number\n");
    return false;
  }

  *port_n = stoi(temp);

  return true;
}

bool get_arg(std::string *line, std::string *arg, std::size_t *pos) {
  /* initialize variables */
  char buffer[100];
  std::string space(" ");
  /* stard reading file name 3 chars after flag */
  std::size_t arg_pos = *pos + 3;
  std::size_t arg_end;

  /* clear out string in case of multiple calls */
  arg->erase();

  /* iterate until next argument in line is reached */
  for (std::size_t i = arg_pos; i < line->length(); i++) {
    if (line->at(i) != '\40') break;

    arg_pos++;
  }

  arg_end = line->find(space, arg_pos);

  if (arg_end == std::string::npos) {
    line->append(" ");
    arg_end = line->find(space, arg_pos);
  }

  if (arg_end == std::string::npos) {
    return false;
  }
  std::size_t name = arg_end - arg_pos;

  line->copy(buffer, name, arg_pos);

  for (std::size_t i = 0; i < name; i++) {
    arg->push_back(buffer[i]);
  }

  /* clear out buffer */
  for (int i = 0; i < 100; i++) {
    buffer[i] = 0;
  }

  std::cout << "arg is: \"" << arg->data() << "\"\n";

  *pos = arg_pos;
  fprintf(stderr, "pos at end of get_arg is: %zu\n", *pos);

  return true;
}

bool test_input(input_structure *store_args) {
  if (!assert_app_mode(store_args)) {
    fprintf(stderr, "Error: Read or Write parameter not set\n");
    return false;
  }

  if (!assert_file_name(store_args)) {
    fprintf(stderr, "Error: file name not set\n");
    return false;
  }

  return true;
}

bool assert_app_mode(input_structure *store_args) {
  if (store_args->app_mode == EMPTY) return false;

  return true;
}

bool assert_file_name(input_structure *store_args) {
  if (store_args->file_name.empty()) return false;

  return true;
}

