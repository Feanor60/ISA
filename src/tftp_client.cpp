/**
 * @file: tftp_client.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: implements client side of tftp protocol
 */

#include "tftp_client.hpp"

bool start_tftp_clinet(input_structure *store_args) {
  struct sockaddr_in server;
  
  if(!fill_sockaddr_in(store_args, &server)) return false;

  if(store_args->app_mode == 1) {
    if(!request_read(store_args, &server)) {
      return false;
    }
  } else {
    if(!request_write(store_args, &server)) {
      return false;
    }
  }

  return true;
}

bool fill_sockaddr_in(input_structure *store_args, struct sockaddr_in *server) {
  struct hostent *host;       // host info


  host = gethostbyname(store_args->ip_address.data());
  if (host == NULL) {
    fprintf(stderr, "Error: uknown address\n");
    return false;
  }

  server->sin_family = AF_INET;
  memcpy(&server->sin_addr.s_addr, host->h_addr, host->h_length);
  server->sin_port = htons(store_args->port_number);

  return true;
}

bool request_read(input_structure *store_args, struct sockaddr_in *server) {
  /* socket desriptor */
  int sock;
  /* fixed size buffer, blocksize option not implemented yet #TODO revise this */
  char buffer[600];
  char *p;
  int count;
  unsigned int server_len;

  /* ipv4  */
  sock = socket(AF_INET, SOCK_DGRAM, 0);

  /* build tftp header  */
  /* The op-code   */
  *(short *)buffer = htons(OP_READ); 
  /* OP code takes 2 bytes in buffer */
  p = buffer + 2;
  /* copy file name to buffer */
  strcpy(p, store_args->file_name.data());
  /* one byte left empty */
  p += strlen(store_args->file_name.data()) + 1;
  /* copy mode */
  strcpy(p, "octet");
  /* last byte left empty */
  p += strlen("octet") + 1;

  /* send read request to tftp server */
  count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server,
                 sizeof *server);

  /* Loop, collecting data packets from the server, until a short
     packet arrives. This indicates the end of the file.
  */
  do {
    server_len = sizeof(*server);
    count = recvfrom(sock, buffer, 600, 0, (struct sockaddr *)&server, &server_len);
    if (ntohs(*(short *)buffer) == OP_ERR) {
      fprintf(stderr, "rcat: %s\n", buffer + 4);
    } else {
      write(1, buffer + 4, count - 4);
      /* Send an ack packet. The block number we want to ack is
         already in the buffer so we just need to change the
         opcode. Note that the ACK is sent to the port number
         which the server just sent the data from, NOT to port
         69
      */
      *(short *)buffer = htons(OP_ACK);
      sendto(sock, buffer, 4, 0, (struct sockaddr *)&server, sizeof(*server));
    }
  } while (count == 516);

  return true;
}

bool request_write(input_structure *store_args, struct sockaddr_in *server) {
  return true;
}
