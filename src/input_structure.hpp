/**
 * @file: argument_structure.hpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: header file for structure that is used to store user input from
 *         mytftpclient
 */

#define EMPTY 0

#ifndef __ARGUMENT_STRUCTURE__
#define __ARGUMENT_STRUCTURE__

#include <string>

/*
 * input_structure:
 * @brief: structure to store command line input for mytftpclient
 * @file_name: name of the read/write file
 * @app_mode: specifies if client read or writes from/to server
 * @timeout: specifies time before server resends package after no response
 *           client suggests this value, server does not have to accept
 * @size: maximum size of blocks in octets, this value is sugested by client
 * @multicast: client asks for mulcticast transfer
 * @data_mode: data encoding in packets - netasci, octet
 * @ip_addres: address of the server, in ipv4 or ipv6
 * @port_number: port where server is listening
 */
struct input_structure {
  public:
   std::string file_name;
   int app_mode; /* 0 - no user input, 1 - read mode, 2 - write mode */
   int timeout;
   int size;
   bool multicast;
   int data_mode;
   std::string ip_address;
   std::size_t port_number;
};

/**
 * @name; add_app_mode_read
 * @param input_structure: structure to write user input into
 * @brief: sets app_mode argument to 1, which means read mode
 */
void add_app_mode_read(input_structure *store_args);

/**
 * @name; add_app_mode_write
 * @param input_structure: structure to write user input into
 * @brief: sets app_mode argument to 2, which means write mode
 */
void add_app_mode_write(input_structure *store_args);

/**
 * @name; add_file_name
 */
void add_file_name(input_structure *store_args, std::string *file);

/**
 * @name; add_timeout
 */
void add_timeout(input_structure *store_args, int seconds);

/**
 * @name; add_size
 */
void add_size(input_structure *store_args, int size);

/**
 * @name: add_multicast
 */
void add_multicast(input_structure *store_args);

/**
 * @name: add_data_mode
 */
void add_data_mode(input_structure *store_args, int data_mode);

/**
 * @name: add_ip_address
 */
void add_ip_address(input_structure *store_args, std::string ip_address);

/**
 * @name: add_port_number
 */
void add_port_number(input_structure *store_args, std::size_t port_n);

#endif
