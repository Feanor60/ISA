/**
 * @file: parse_input.hpp
 * @author: Vojtěch Bůbela
 * @login: xbubl08
 * @brief: header file with declaration of parse_input function
 */

#ifndef __PARSE_INPUT__
#define __PARSE_INPUT__

#include <assert.h>
#include <bits/c++config.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <string>

#include "input_structure.hpp"

/**
 * @name: parse_input
 * @param line: line of user input to be parsed
 * @param ErrMsg: buffer for error message in case of wrong input
 * @retval: false when the user input is incorrect, otherwise true
 * @store_args: structure to store arguments
 * @brief: check if user input is correct, if not send simple error
 * 				 message, if yes fill the input into structure
 */
bool parse_input(std::string *line, input_structure *store_args);

/**
 * @name: get_file_name
 * @param line: string to extract file name from
 * @param file_name: string to store file name
 * @param pos: first char of file name
 * @brief: extract file name from user input
 * @retval: true on succes or false on invalid arg
 */
bool get_file_name(std::string *line, std::string *file_name, std::size_t *pos);

/**
 *  @name: get_timeout
 *  @param line: string to extract timeout from
 *  @param pos: first char of timeout in line
 *  @brief: extract timeout from user input string and convert
 *          it into integer
 *  @retval: numerical value of timeout
 */
int get_timeout(std::string *line, std::size_t *pos);

/**
 *  @name: get_size
 *  @param line: string to extract size from
 *  @param pos: first char of size in line
 *  @brief: extract size from user input string and convert
 *          it into integer
 *  @retval: numerical value of size
 */
int get_size(std::string *line, std::size_t *pos);

/**
 *  name: get_data_mode
 *  @param line: string to extract data mode from
 *  @param pos: firct char of data mode in line
 *  @brief: read chars from pos in line until next empty char
 *          no data mode means binary
 *  @retval: return integer which signifies data mode:
 *           -1 = error
 *           1 = netascii
 *           2 = octet
 */
int get_data_mode(std::string *line, std::size_t *pos);

/**
 *  @name: get_ip_address
 *  @param line: string to extract ip address from
 *  @param ip_address: string to store ip address in
 *  @param pos: first char of ip address in line
 *  @brief: read ip address from line from pos
 *  @retval: return true on success and false on error
 */
bool get_ip_address(std::string *line, std::string *ip_address, std::size_t *pos);

/**
 *  @name: get_port_number
 *  @param line: string to extract port number from
 *  @param port_n: string to store port number in
 *  @param pos: first char of ip address in line
 *  @brief: portn number from line from pos
 *  @retval: return true on success and false on error
 */
bool get_port_number(std::string *line, std::size_t *port_n, std::size_t *pos);

/**
 *  @name: get_arg
 *  @param line: string to extract arg from
 *  @param arg: string to store arg in
 *  @param pos: position of first char of arg in string
 *  @brief: read chars from pos in line until next space char or end of string
 *  @retval: return true on succes or false on no arg
 */
bool get_arg(std::string *line, std::string *arg, std::size_t *pos);

/*
 * @name: test_input
 * @param input_strucutre: structurure with values to be tested
 * @brief: assert in user inputed needed information for app to work
 * @retval: return false if one of checks fails
 */
bool test_input(input_structure *store_args);

/*
 * @name: assert_app_mode
 * @param input_structure: structurure with values to be tested
 * @brief: test if app_mode was set
 * @retval: false if not set
 */
bool assert_app_mode(input_structure *store_args);

/*
 * @name: assert_file_name
 * @param input_structure: structurure with values to be tested
 * @brief: test if file name was set
 * @retval: false if not set
 */
bool assert_file_name(input_structure *store_args);

#endif
