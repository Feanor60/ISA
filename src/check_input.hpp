/**
 * @file: check_input.hpp
 * @author: Vojtěch Bůbela
 * @login: xbubl08
 * @brief: header file with declaration of check_input function
 */

#ifndef __CHECK_INPUT__
#define __CHECK_INPUT__

#include <iostream>
#include <string>

#include "input_structure.hpp"

/**
 * @name: parse_args
 * @param line: line of user input to be checked
 * @param ErrMsg: buffer for error message in case of wrong input
 * @retval: false when the user input is incorrect, otherwise true
 * @store_args: structure to store arguments
 * @brief: check if user input is correct, if not send simple error
 * 				 message, if yes fill the input into structure
 */
bool check_input(std::string *line, std::string *ErrMsg,
		 input_structure *store_args);

/**
 * @name: get_file_name
 * @param line: string to extract file name from
 * @param file_name: string to store file name
 * @param pos: first char of file name
 * @brief: extract file name from user input
 */
bool get_file_name(std::string *line, std::string *file_name, std::size_t pos);

#endif
