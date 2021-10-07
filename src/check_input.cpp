/**
 * @file: check_input.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: functions for checking if the user input is correct
 */

#include "check_input.hpp"

bool check_input(std::string *line, std::string *ErrMsg,
                 input_structure *store_args) {
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

  std::size_t pos;

  pos = line->find(write);
  if (pos != std::string::npos) {
    add_app_mode_write(store_args);
  }

  pos = line->find(read);
  if (pos != std::string::npos) {
    add_app_mode_read(store_args);
  }

  pos = line->find(file);
  if (pos != std::string::npos) {
    // get_file_name(line, &file_name, pos);
    add_file_name(store_args, &file_name);
  } else {
    ErrMsg->assign("Error: specify file to be transfered");
  }

  return true;
}
