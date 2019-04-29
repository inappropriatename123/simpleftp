#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <err.h>

#include <netinet/in.h>

#define BUFSIZE 512
#define CMDSIZE 4
#define PARSIZE 100

#define MSG_220 "220 srvFtp version 1.0\r\n"
#define MSG_331 "331 Password required for %s\r\n"
#define MSG_230 "230 User %s logged in\r\n"
#define MSG_530 "530 Login incorrect\r\n"
#define MSG_221 "221 Goodbye\r\n"
#define MSG_550 "550 %s: no such file or directory\r\n"
#define MSG_299 "299 File %s size %ld bytes\r\n"
#define MSG_226 "226 Transfer complete\r\n"

/**
 * function: receive the commands from the client
 * sd: socket descriptor
 * operation: \0 if you want to know the operation received
 *            OP if you want to check an especific operation
 *            ex: recv_cmd(sd, "USER", param)
 * param: parameters for the operation involve
 * return: only usefull if you want to check an operation
 *         ex: for login you need the seq USER PASS
 *             you can check if you receive first USER
 *             and then check if you receive PASS
 **/
bool recv_cmd(int sd, char *operation, char *param) {
    char buffer[BUFSIZE], *token;
    int recv_s;

    // receive the command in the buffer and check for errors



    // expunge the terminator characters from the buffer
    buffer[strcspn(buffer, "\r\n")] = 0;

    // complex parsing of the buffer
    // extract command receive in operation if not set \0
    // extract parameters of the operation in param if it needed
    token = strtok(buffer, " ");
    if (token == NULL || strlen(token) < 4) {
        warn("not valid ftp command");
        return false;
    } else {
        if (operation[0] == '\0') strcpy(operation, token);
        if (strcmp(operation, token)) {
            warn("abnormal client flow: did not send %s command", operation);
            return false;
        }
        token = strtok(NULL, " ");
        if (token != NULL) strcpy(param, token);
    }
    return true;
}

/**
 * function: send answer to the client
 * sd: file descriptor
 * message: formatting string in printf format
 * ...: variable arguments for economics of formats
 * return: true if not problem arise or else
 * notes: the MSG_x have preformated for these use
 **/
bool send_ans(int sd, char *message, ...){
    char buffer[BUFSIZE];

    va_list args;
    va_start(args, message);

    vsprintf(buffer, message, args);
    va_end(args);
    // send answer preformated and check errors
    if(send(sd,message, BUFSIZE, 0)<0){
        printf("El mensaje no se a podido enviar\n");
        return false;
    }


    return true;
}

/**
 * function: RETR operation
 * sd: socket descriptor
 * file_path: name of the RETR file
 **/

void retr(int sd, char *file_path) {
    FILE *file;
    int bread;
    long fsize;
    char buffer[BUFSIZE];

    // check if file exists if not inform error to client

    // send a success message with the file length

    // important delay for avoid problems with buffer size
    sleep(1);

    // send the file
    // close the file

    // send a completed transfer message
}
/**

 * funcion: check valid credentials in ftpusers file
 * user: login user name
 * pass: user password
 * return: true if found or false if not
 **/
bool check_credentials(char *user, char *pass) {
    FILE *file;
    char *path = "./ftpusers", *line = NULL, cred[100];
    size_t len = 0;
    bool found = false;

    // make the credential string
    strcpy(cred,"");
    strcat(cred,user);
    strcat(cred,":");
    strcat(cred,pass);
    strcat(cred,"\n");
    len=sizeof(cred);

    // check if ftpusers file it's present
    if((file=fopen(path,"r"))==NULL){

    	printf("Archivo de credenciales no encontrado\n");
    	return false;
    }
    // search for credential string
    line=(char*)malloc(sizeof(char)*100);
    while(!feof(file)){
    	fgets(line, 100, file);
    	if(strncmp(line,cred,len)==0){
    		found=true;
    		break;
    	}
    }

    // close file and release any pointes if necessary
    fclose(file);
    free(line);
    // return search status
    if(found == false)
    	printf(MSG_530);
    else
    	printf(MSG_230,user);
    return found;
}

/**
 * function: login process management
 * sd: socket descriptor
 * return: true if login is succesfully, false if not
 **/
bool authenticate(int sd) {
    char user[PARSIZE], pass[PARSIZE];

    // wait to receive USER action

    // ask for password

    // wait to receive PASS action

    // if credentials don't check denied login

    // confirm login
}

/**
 *  function: execute all commands (RETR|QUIT)
 *  sd: socket descriptor
 **/

void operate(int sd) {
    char op[CMDSIZE], param[PARSIZE];

    while (true) {
        op[0] = param[0] = '\0';
        // check for commands send by the client if not inform and exit


        if (strcmp(op, "RETR") == 0) {
            retr(sd, param);
        } else if (strcmp(op, "QUIT") == 0) {
            // send goodbye and close connection




            break;
        } else {
            // invalid command
            // furute use
        }
    }
}

/**
 * Run with
 *         ./mysrv <SERVER_PORT>
 **/
int main (int argc, char *argv[]) {

    // arguments checking
    if(argc != 2){
        printf("Error, please include the port\n");
        return -1;
    }
    
    // reserve sockets and variables space
    typedef struct sockaddr *sad;
    int sockfd,sock_send;
    struct sockaddr_in sin1, peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr_in);
    int PORT = atoi(argv[1]);
    
    // create server socket and check errors
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    sin1.sin_family = AF_INET; // addres family
    sin1.sin_port=htons(PORT); // function that changes to little or big endian
    sin1.sin_addr.s_addr= INADDR_ANY; // found in inet.h
    
    if(sockfd < 0) {
        printf("Socket could not be reated\n");
        return -1;
    }
      printf("Socket created succesfully\n");

    // bind master socket and check errors
    if ((bind (sockfd, (sad) &sin1, sizeof sin1)) < 0) {
        printf("Socket could not be linked\n");
        return -1;
    }
    printf("Socket linked\n");
    
    // make it listen
    if(listen(sockfd,5) < 0) {
        printf("Socket not listening\n");
        return -1;
    }
    printf("Socket listening\n");
    
    // main loop
    while (true) {
        // accept connectiones sequentially and check errors
        if((sock_send = accept(sockfd, (sad) &peer_addr,&peer_addr_size)) < 0){
            printf("The socket did not accept the conection\n");
            continue;
        }
        printf("The socket accepted the conection\n");
        // send hello
        if(!send_ans(sock_send,MSG_220)){
            printf("The hello message could not be sent\n");
            continue;
        }
        printf("The hello was sent succesfully\n");

        // operate only if authenticate is true
        if(!authenticate(sock_send)) {
            send_ans(sock_send,MSG_530);
            continue;
        }
    }

    // close server socket
    close(sock_send);
    
    // end main function
    return 0;
}
