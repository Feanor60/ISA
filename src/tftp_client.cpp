/**
 * @file: tftp_client.cpp
 * @author: Vojtěch Bůbela
 * @login: xbubel08
 * @brief: implements client side of tftp protocol
 */

#include "tftp_client.hpp"

bool start_tftp_clinet(input_structure *store_args) {
  FILE *fp;
  bool RRQ;
  bool ipv6;
  char *p;
  /* fixed size buffer, blocksize option not implemented yet */
  char buffer[600];
  int count;
  /* socket desriptor */
  int sock;
  int write_count;
  int recieved_data;
  std::size_t pos;
  std::size_t result;
  struct sockaddr_in server;
  struct sockaddr_in6 server6;
  struct timeval timeout;
  time_t t;
  unsigned int server_len;

  /* check if unsupported options had been set */
  if(store_args->size != -1) {
    fprintf(stderr, "Error: option -s is not supported\n");
    return false;
  }

  if(store_args->timeout != -1) {
    fprintf(stderr, "Error: option -t is not supported\n");
    return false;
  }
  
  if(store_args->multicast) {
    fprintf(stderr, "Error: option -m is not supported\n");
    return false;
  }

  /* do some initial setup */
  /* --------------------------------------------- */
  if (store_args->app_mode == 1) {
    RRQ = true;
    recieved_data = 0;
  } else {
    RRQ = false;
  }

  // buffer_count = 0;
  /* set up socket */
  pos = store_args->ip_address.find(":");
  if (pos != std::string::npos) {
    ipv6 = true;
    sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (!fill_sockaddr_in6(store_args, &server6)) return false;
  } else {
    ipv6 = false;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (!fill_sockaddr_in(store_args, &server)) return false;
  }

  time(&t);

  timeout.tv_sec = 7;
  timeout.tv_usec = 0;

  /* add timeout to the socket */
  if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) <
      0) {
    fprintf(stderr, "[%.24s] Error: failed to set timeout\n", ctime(&t));
  }

  /* create or open file that is transfered */

  if (RRQ) {
    fp = fopen(store_args->file_name.data(), "w+");
  } else {
    fp = fopen(store_args->file_name.data(), "r+");
  }

  if (fp == NULL) {
    time(&t);
    fprintf(stderr,
            "[%.24s] Error: could not open file for reading or writting, errno "
            "is: %d\n",
            ctime(&t), errno);
    return false;
  }
  /* --------------------------------------------- */


  /* build tftp header for the first time */
  p = build_tftp_request_header(store_args, buffer);


  /* print update data */
  if (RRQ) {
    time(&t);
    fprintf(stdout, "[%.24s] Requesting READ from server %s:%zu\n", ctime(&t),
            store_args->ip_address.data(), store_args->port_number);
  } else {
    time(&t);
    write_count = 1;
    fprintf(stdout, "[%.24s] Requesting WRITE from server %s:%zu\n", ctime(&t),
            store_args->ip_address.data(), store_args->port_number);
  }

  /* send read request to tftp server */
  if (ipv6) {
    count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server6,
                   sizeof(server6));
  } else {
    count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server,
                   sizeof(server));
  }

  /* collect data into file until short packet arrives (signaling end of
   * transfer) or either terminating error packet is recieved or error occurs*/
  do {
    if (ipv6)
      server_len = sizeof(server6);
    else
      server_len = sizeof(server);

    /* actively wait for response form server */
    if (ipv6) {
      count = recvfrom(sock, buffer, 600, 0, (struct sockaddr *)&server6,
                       &server_len);
    } else {
      count = recvfrom(sock, buffer, 600, 0, (struct sockaddr *)&server,
                       &server_len);
    }

    /* print update if packet with ACK was recieved */
    if (ntohs(*(short *)buffer) == OP_ACK) {
      time(&t);
      fprintf(stdout, "[%.24s] Recieved ack packet from server %s:%lu\n",
              ctime(&t), store_args->ip_address.data(),
              (unsigned long)ntohs(server.sin_port));
    }

    /* timeout is set to 3 seconds, after timeout happens, packet is resent once
     * if it happens again, transfer is reminated */
    if (errno == EWOULDBLOCK) {
      fprintf(stderr, "[%.24s] Error: timed out, trying again ...\n",
              ctime(&t));
      if (ipv6) {
        count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server6,
                       sizeof(server6));
      } else {
        count = sendto(sock, buffer, p - buffer, 0, (struct sockaddr *)&server,
                       sizeof(server));
      }

      server_len = sizeof(server);

      errno = 0;
      if (ipv6) {
        count = recvfrom(sock, buffer, 600, 0, (struct sockaddr *)&server6,
                         &server_len);
      } else {
        /* actively wait for response form server #TODO implement timeout */
        count = recvfrom(sock, buffer, 600, 0, (struct sockaddr *)&server,
                         &server_len);
      }

      /* if it times out again print error and abort transfer */
      if (errno == EWOULDBLOCK) {
        time(&t);
        fprintf(stderr, "[%.24s] Error: timed out\n", ctime(&t));
        fprintf(stderr, "[%.24s] Aborting transfer\n", ctime(&t));
        remove_file(store_args->file_name.data(), fp, RRQ);
        return false;
      }
    }

    /* if error packet is recieved, parse its error code, if its 1 - 4, terminate transfer */
    if (ntohs(*(short *)buffer) == OP_ERR) {
      if (!handle_error(buffer)) {
        fprintf(stderr, "Aborting transfer\n");
        remove_file(store_args->file_name.data(), fp, RRQ);
        return false;
      }
    } else {
      recieved_data = count;
      /* handle reading from server */
      if (RRQ) {
        time(&t);
        fprintf(stdout, "[%.24s] Recieving data from server %s:%lu\n",
                ctime(&t), store_args->ip_address.data(),
                (unsigned long)ntohs(server.sin_port));

        /* write recieved data into file */
        result = fwrite(buffer + 4, sizeof(char), count - 4, fp);

        if (result != (unsigned long int)(count - 4)) {
          time(&t);
          fprintf(stderr, "[%.24s]Error while writing into file\n", ctime(&t));
          remove_file(store_args->file_name.data(), fp, RRQ);
          return false;
        }

        /* take the recieved buffer and replace OP code with OP_ACK
         * because it already contains block nubmer that needs to be acked*/
        *(short *)buffer = htons(OP_ACK);

        if (ipv6) {
          count = sendto(sock, buffer, 4, 0, (struct sockaddr *)&server6,
                         sizeof(server6));
        } else {
          count = sendto(sock, buffer, 4, 0, (struct sockaddr *)&server,
                         sizeof(server));
        }

      } else {
        /* handle writing to server */

        /* print update */
        time(&t);
        fprintf(stdout, "[%.24s] Writing data to server %s:%lu\n", ctime(&t),
                store_args->ip_address.data(),
                (unsigned long)ntohs(server.sin_port));

        /* prepare DATA packet */
        *(short *)buffer = htons(OP_DATA);
        p = buffer + 2;
        *(short *)p = htons(write_count);
        write_count++;

        /* write from file to buffer */
        char c;
        int counter = 0;

        do {
          c = fgetc(fp);

          if (feof(fp)) {
            result = counter;
            break;
          }
          
          /* if netascii mode is set, convert read data */
          if(store_args->data_mode == 1) {
            if(c == '\n') {
              buffer[counter + 4] = '\r';
              counter ++;
              if(counter == 512) {
                break;
              }
            } else if( c == '\r') {
              buffer[counter + 4] = '\0';
              counter++;
              if(counter == 512) {
                break;
              }
            }
          }

          buffer[counter + 4] = c;

          counter++;

        } while (counter != 512);

        /* set number of bytes written */
        result = counter;

        /* sent written packet */
        if (ipv6) {
          count = sendto(sock, buffer, counter + 4, 0,
                         (struct sockaddr *)&server6, sizeof(server6));
        } else {
          count = sendto(sock, buffer, counter + 4, 0,
                         (struct sockaddr *)&server, sizeof(server));
        }
      }
    }
    
  } while ((recieved_data == 516 && RRQ) || (!RRQ && result == 512));

  fclose(fp);

  /* if reading netascii file, convert it to binary */
  if(store_args->data_mode == 1 && RRQ) {
    if(!convert_from_netascii(store_args)) {
      fprintf(stderr, "Error while reading or writing from file, aborting transfer ...\n");
    }
  }

  time(&t);
  fprintf(stdout, "[%.24s] Transfer completed without error\n", ctime(&t));

  return true;
}

bool fill_sockaddr_in(input_structure *store_args, struct sockaddr_in *server) {
  struct hostent *host;  // host info

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

bool fill_sockaddr_in6(input_structure *store_args,
                       struct sockaddr_in6 *server) {
  struct hostent *host;  // host info

  host = gethostbyname2(store_args->ip_address.data(), AF_INET6);
  if (host == NULL) {
    fprintf(stderr, "Error: uknown address\n");
    return false;
  }

  server->sin6_family = AF_INET6;

  memcpy(&server->sin6_addr, host->h_addr, host->h_length);
  server->sin6_port = htons(store_args->port_number);

  return true;
}

char *build_tftp_request_header(input_structure *store_args, char *buffer) {
  char *p;
  /* build tftp header  */
  /* The op-code   */
  if (store_args->app_mode == 1) {
    *(short *)buffer = htons(OP_READ);
  } else {
    *(short *)buffer = htons(OP_WRITE);
  }

  /* OP code takes 2 bytes in buffer */
  p = buffer + 2;
  /* copy file name to buffer */
  strcpy(p, store_args->file_name.data());
  /* one byte left empty */
  p += strlen(store_args->file_name.data()) + 1;
  /* copy mode */
  if (store_args->data_mode == 1) {
    strcpy(p, "netascii");
    p += strlen("netascii") + 1;
  } else {
    strcpy(p, "octet");
    p += strlen("octet") + 1;
  }

  return p;
}

bool handle_error(char *buffer) {
  uint32_t err_code;
  /* get the error code */
  char *p = buffer;
  p = p + 2;
  err_code = ntohs(*(short *)p);

  fprintf(stderr, "Error: %s\n", buffer + 4);

  if (err_code == 0 || err_code == 1 || err_code == 2 || err_code == 3 ||
      err_code == 4) {
    return false;
  } else if (err_code == 5) {
    return true;
  } else {
    fprintf(stderr,
            "Error: recived error message with unknown error number: %d\n",
            err_code);
    return false;
  }

  return true;
}

bool convert_from_netascii(input_structure *store_args) {
  
  FILE *fp_orig;
  FILE *fp_new;
  int result;
  char c;

  if((result = rename(store_args->file_name.data(), "temp")) != 0) {
    return false;
  } 
  
  fp_orig = fopen("temp", "r"); 
  if(fp_orig == NULL) {
    remove_file("temp", fp_orig, true);
    return false;
  }

  fp_new = fopen(store_args->file_name.data(), "w");
  if(fp_new == NULL) {
    remove_file(store_args->file_name.data(), fp_new, false);
    return false;
  }

  /* go through recieved buffer char by char and look for '\r''\n'
   * and '\r''\0' and replace them with '\n' and '\r' respectively
   * then write them into file, write everything else into file */
 
  while((c = fgetc(fp_orig)) != EOF){

    if (c == '\r') {
      c = fgetc(fp_orig);

      if(feof(fp_orig)) {
          fputc(c, fp_new);
          remove_file("temp", fp_orig, true);
          return true;
      }

      if (c == '\n') {
        fputc('\n', fp_new);
      } else if (c == '\0') {
        fputc('\r', fp_new);
      }

    } else {
      fputc(c, fp_new);
    }
  }

  remove_file("temp", fp_orig, true);
  fclose(fp_new);
  return true;
}

void remove_file(const char *file_name, FILE *fp, bool RRQ) {
  fclose(fp);
  if (RRQ) remove(file_name);
}
