#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct client {
    int id;                 // client's id;
    char msg[100000]        // client's message
}           t_client;


t_client clients[1024];     // array of clients

int max = 0; 
int next_id = 0;

fd_set active;      // fd_set is a struct that contains socket in a 'set'
fd_set ready_read;
fd_set ready_write;

char buff_read[424242];
char buff_write[424242];


void error_mess(char *str)  // writes the message and exite with status code 1
{
    if (str) // protection
        write(2, str, strlen(str));
    exit(1);
}

void send_to_all(int client)
{
    for (int i = 0; i <= max; i++)
    {
        if (FD_ISSET(i, &ready_write) && i != client)
            send(i, buff_write, strlen(buff_write), 0);
    }
}

int main(int argc, char **argv)
{
    if (argc != 2) // protection
        error_mess("Wrong number of arguments\n");
    int port = atoi(argv[1])

    // AF_INET = internet domain, SOCK_STREAM = socket type for TCP/IP protocol, 0 = default protocol
    int serverSock = socket(AF_INET, SOCK_STREAM, 0) 
    if (serverSock < 0)                                 // the socket creation failed
        error_mess("Fatal error\n");
    
    bzero(&clients, sizeof(clients));                   // initialize the array of clients (no residual data)
    FD_ZERO(&active);                                   // initializes the file descriptor set to contain no file descriptors.                  

    max = serverSock;
    FD_SET(serverSock, &active)                         // adds file descriptor to a file descriptor set

    struct sockaddr_in addr;                            // struct containing info about environment to which it binds
    socklen_t addr_len = sizeof(addr);                  // get the size of struct addr
    addr.sin_family = AF_INET;                          // which protocol the socket will use
    addr.sin_addr.s_addr = htons(port);                 // on which port htons translate the port in big endian 

    if (bind(serverSock, (const struct (sockaddr *)&addr, sizeof(addr))) < 0)       // bind the server
        error_mess("Fatal error\n");
    if (listen(serverSock, 128) < 0)                    // server socket in passive mode (listen for event)
        error_mess("Fatal error\n");

    while (1)
    {
        ready_read = ready_write = active;
        if (select(max + 1, &ready_read, &ready_write, NULL, NULL) < 0) // wait and 'select' a file descriptor when it becomes 'ready' and adds it to ready
            continue;                                   // force next iteration of the loop
        for (int fd = 0; fd <= max; fd++)
        {
            for (FD_ISSET(fd, &ready_read) && fd == serverSock)     // if fd is in ready write and it corresponds to the server one = new connection
            {
                int cientSock = accept(serverSock, (struct sockaddr *)&addr, &addr_len);
                if(clientSocket < 0)
                    continue;
                clients[clientSocket].id = next_id++;
                FD_SET(clientSocket, &active);               // add the socket to the active set
                sprintf(buff_write, "server: client %d just arrived\n", clients[clientSocket].id)           //prints in buffer
                send_to_all(clientSocket);
                break;
            }
            if (FD_ISSET(fd, &ready_read) && fd != serverSock)  // it not a new incoming connection
            {
                int read = recv(fd, buff_read, 424242, 0);
                if (read <= 0)
                {
                    sprintf(buff_write, "server: client %d jsut left\n", clients[fd].id);
                    send_to_all(fd);
                    FD_CLR(fd, &active);
                    close(fd);
                    break;
                }
                else
                {
                    for (int i = 0; j = strlen(client[fd].msg); i < read; i++, j++)
                    {
                        clients[fd].msg[j] = buff_read[i];
                        if (client[fd].msg[j] == '\n')
                        {
                            client[fd].msg[j] == '\0';
                            sprintf(buff_write, "client %d: %s\n", client[fd].id, clients[fd].msg);
                            send_to_all(fd);
                            bzero(&clients[fd].msg, strlen(clients[fd].msg));
                            j = -1;
                        }
                    }
                    break;
                }
            }
        }
    }

}

