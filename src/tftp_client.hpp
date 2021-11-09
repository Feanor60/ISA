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

#include "input_structure.hpp"

#define PORT      69
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
 *  @name: request_read
 *  @brief: send read request to server
 *  @retval: returns true on server akcnowledging read request
 *           returns false on timeout #TODO edit as needed
 */
bool request_read(input_structure *store_args, struct sockaddr_in *server);

/**
 *  @name: request_write
 *  @brief: send write request to server
 *  @revatl: returns true on server akcnowledging write request
 *           returns false on timeout #TODO edit as needed
 */
bool request_write(input_structure *store_args, struct sockaddr_in *server);

#endif
