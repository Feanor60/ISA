/**
 * @file: tftp_client.hpp 
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: implements client side of tftp protocol 
 */

#ifndef __TFTP_CLIENT__
#define __TFTP_CLIENT__ 

#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "input_structure.hpp"

/* default port for tftp server */
#define PORT      69

/* tftp header OP codes */
#define OP_READ   1
#define OP_WRITE  2
#define OP_DATA   3
#define OP_ACK    4
#define OP_ERR    5

/**
 *  @name: start_tftp_client
 *  @param store_args: struct with information needed by client
 *  @brief: main function where tftp client runs
 *  @retval: returns true on succesful transfer or false on failure
 */
bool start_tftp_clinet(input_structure *store_args);

/**
 * @name: fill_sockaddr_in
 * @param store_args: structure with ip address and port
 * @param server: structure which we will be filled out
 * @brief: fill out a socket with information from user input
 * @retval: true on succes false on failure
 */
bool fill_sockaddr_in(input_structure *store_args, struct sockaddr_in *server);

/**
 * @name: fill_sockaddr_in
 * @param store_args: structure with ip address and port
 * @param server6: structure which we will be filled out
 * @brief: fill out a socket with information from user input
 * @retval: true on succes false on failure
 */
bool fill_sockaddr_in6(input_structure *store_args, struct sockaddr_in6 *server6);

/**
 *  @name: request_read
 *  @brief: send read request to server
 *  @retval: returns true on server akcnowledging read request
 *           returns false on timeout #TODO edit as needed
 */
bool request_read(input_structure *store_args);

/**
 *  @name: request_write
 *  @brief: send write request to server
 *  @revatl: returns true on server akcnowledging write request
 *           returns false on timeout #TODO edit as needed
 */
bool request_write(input_structure *store_args);

/**
 * @name: build_request_read_header
 * @param buffer: pointer to buffer with tftp header and data
 * @param p: pointer to char in buffer
 * @retval: pointer that points at end of tftp header in buffer 
 */
char *build_tftp_request_header(input_structure *store_args, char *buffer);

/**
 * @name: handle_error
 * @param buffer: char array with error op code, and description of error
 * @brief: parse buffer, print error message and decide if transfer needs to
 *         be terminated
 * @retval: true if transfer should continue, false if it should be terminated
 */
bool handle_error(char *buffer);

/**
 * @name: convert_from_netascii
 * @param buffer: char array that needs to be converted
 * @param count: lenght of buffer
 * @brief: convert from netascii to normal
 */
void convert_from_netascii(char *buffer, int count);

/**
 * @name: remove_file
 * @param file_name: name of file to be removed
 * @param fp: file descriptor of read file
 * @brief: if reading fails close file descriptor and delete file
 */
void remove_file(const char* file_name, FILE *fp, bool RRQ);
#endif
