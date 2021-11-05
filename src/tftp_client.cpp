/**
 * @file: tftp_client.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: implements client side of tftp protocol
 */

#include "tftp_client.hpp"

bool start_tftp_clinet(input_structure *store_args) {
  int sock;                   // socket descriptor
  struct sockaddr_in server;  // server address
  struct hostent *host;       // host info
  char buffer[600];
  char *p;
  int count;
  unsigned int server_len;

  sock = socket(AF_INET, SOCK_DGRAM, 0);

  host = gethostbyname("127.0.0.1");
  if (host == NULL) {
    fprintf(stderr, "Error: uknown address\n");
    return false;
  }

  server.sin_family = AF_INET;
  memcpy(&server.sin_addr.s_addr, host->h_addr, host->h_length);
  server.sin_port = htons(PORT);

  *(short *)buffer = htons(OP_READ); /* The op-code   */
  p = buffer + 2;
  strcpy(p, store_args->file_name.data());       /* The file name */
  p += strlen(store_args->file_name.data()) + 1; /* Keep the nul  */
  strcpy(p, "octet");                            /* The Mode      */
  p += strlen("octet") + 1;

  /* Send Read Request to tftp server */
  count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server,
                 sizeof server);

  /* Loop, collecting data packets from the server, until a short
     packet arrives. This indicates the end of the file.
  */
  do {
    server_len = sizeof(server);
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
      sendto(sock, buffer, 4, 0, (struct sockaddr *)&server, sizeof server);
    }
  } while (count == 516);

  return true;
}

bool request_read(input_structure *store_args, char err_buff[]) { return true; }

bool request_write(input_structure *store_args, char err_buff[]) {
  return true;
}
