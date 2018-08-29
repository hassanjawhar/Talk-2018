/*

Hassan Abedallah JAWHAR

*/

#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#include <fcntl.h>
#include <signal.h>


#define TRUE   1
#define FALSE  0

int main(int argc , char *argv[])
{
    int opt = TRUE;
    int master_socket , addrlen , new_socket , client_socket[30] , max_clients = 30 , activity, i , valread , sd;
    int max_sd;
    int PortServer;
    struct sockaddr_in address;

    char buffer[1025];  //data buffer of 1K
    char nameinapp[9];
    char name[100];
    char names[30][100];//[nbstring][lengthstring]

    printf("\nProgramme Serveur TCP, ce code est prepare par Hassan Abdellah JAWHAR\n");
    printf("L'adresse du serveur est: 127.0.0.1 \nVeuillez entrer le numero de port du serveur (doit etre > 1024): ");
    scanf("%d",&PortServer);
if(PortServer<1024){
PortServer=0;
printf("le numero de port doit etre > 1024 \n");
scanf("%d",&PortServer);
}

    //set of socket descriptors
    fd_set readfds;

    char *message;
    int m,z,sdm,x,nb=0,n, sdk;
    char lesmsg[40],exp[1025];
    char shut[100]="_shutdown";
    char *clavier;
    clavier = (char *)malloc(256);

    char connect1[100] = "",nom[100]="";
    char * p;
    char nul[100];
    int pid = 1;


    //initialise all client_socket[] to 0 so not checked
    for (i = 0; i < max_clients; i++)
    {
        client_socket[i] = 0;
    }

    //create a master socket
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PortServer );

    //bind the socket to localhost port 8888
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listener on port %d \n", PortServer);

    //try to specify maximum of 3 pending connections for the master socket
    if (listen(master_socket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

    int pipefd[2];
    char buf;

    pipe(pipefd); // create the pipe

    while(TRUE)
    {

    pid = fork();
        // start of shutdown
            if(pid > 0){
                while(TRUE){
                    scanf("%s", clavier);

                    if (strcmp(clavier,"_shutdown")==0){
                        snprintf(lesmsg, 40, "%s","The server is down." );
                        send(sdm , lesmsg , strlen(lesmsg) , 0 );
                        kill(0, SIGKILL);

                        exit(0);
                    } else if(strcmp(clavier,"_kill")==0){

                    }
                }
            }
        // end of shutdown
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }


        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);//to notify you when file descriptor 0 (standard input) is ready for reading.

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }


        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands

            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));



            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++)
            {
                //if position is empty
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

                    break;
                }
            }

        }

        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {

                //Check if it was for closing , and also read the incoming message
                if (((valread = read( sd , buffer, 1024)) == 0 ))
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }



                    //Echo back the message that came in
                else
                {

                    //set the string terminating NULL byte on the end of the data read

                    buffer[valread] = '\0';

                    for (z = 0 ; z < 9 ; z++ )
                        nameinapp[z]=buffer[z];

                    strcpy(exp,"NameInApp");

                    if(egal(exp,nameinapp))
                    {
                        for (z = 9 ; z < 100 ; z++ )
                            name[z-9]=buffer[z];

                        name[z-9]='\0';
                        strcpy(names[i], name);

                        message=names[i];
                        for (z = 0; z < max_clients; z++)
                            if(client_socket[z]!=0 && client_socket[z]!=sd)
                            {
                                sdm = client_socket[z];
                                snprintf(lesmsg, 40, "%s","SERVER: the user ' " );
                                send(sdm , lesmsg , strlen(lesmsg) , 0 );
                                snprintf(lesmsg, 40, "%s",message );
                                send(sdm , lesmsg , strlen(lesmsg) , 0 );
                                snprintf(lesmsg, 40, "%s"," ' is connected !\n" );
                                send(sdm, lesmsg , strlen(lesmsg) , 0 );

                            }
                    }

                    else{
                        strcpy (exp, " ");
                        strcpy (exp, "_quit");

                        if (egal(buffer,exp)==1)
                        {
                            //Somebody disconnected , get his details and print

                            getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                            printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                            //Close the socket and mark as 0 in list for reuse
                            message=names[i];
                            for (z = 0; z < max_clients; z++)
                                if(client_socket[z]!=0 && client_socket[z]!=sd)
                                {
                                    sdm = client_socket[z];
                                    snprintf(lesmsg, 40, "%s","SERVER: the user ' " );
                                    send(sdm , lesmsg , strlen(lesmsg) , 0 );
                                    snprintf(lesmsg, 40, "%s",message );
                                    send(sdm , lesmsg , strlen(lesmsg) , 0 );
                                    snprintf(lesmsg, 40, "%s"," ' is disconnected !\n" );
                                    send(sdm, lesmsg , strlen(lesmsg) , 0 );

                                }

                            close( sd );
                            client_socket[i] = 0;

                        }

//************************************************************************

                        else {
                            strcpy (exp, " ");
                            strcpy (exp, "_who");

                            if(egal(buffer,exp)==1)
                            {
                                snprintf(lesmsg, 40, "%s", "server reply : ");
                                send(sd , lesmsg , strlen(lesmsg) , 0 );
                                nb=0;
                                for (z = 0; z < max_clients; z++)
                                    if(client_socket[z]!=0 && client_socket[z]!=sd)
                                    {
                                        nb++;
                                    }

                                if(nb>0)
                                {
                                    snprintf(lesmsg, 40, "%s","Number of user connected is : ");
                                    send(sd , lesmsg , strlen(lesmsg) , 0 );
                                    snprintf(lesmsg, 40, "%d",nb);
                                    send(sd , lesmsg , strlen(lesmsg) , 0 );
                                    snprintf(lesmsg, 40, "%s"," \nUser(s) connected : ");
                                    send(sd , lesmsg , strlen(lesmsg) , 0 );
                                    m=0;
                                    for (z = 0; z < max_clients; z++)
                                        if(client_socket[z]!=0 && client_socket[z]!=sd)
                                        {


                                            sdm = client_socket[z];


                                            message=names[z];
                                            snprintf(lesmsg, 40, "%s", message);

                                            send(sd , lesmsg , strlen(lesmsg) , 0 );
                                            snprintf(lesmsg, 40, "%s","  " );
                                            send(sd , lesmsg , strlen(lesmsg) , 0 );

                                        }


                                }
                                else {snprintf(lesmsg, 40, "%s"," only you are online!  ");
                                    send(sd , lesmsg , strlen(lesmsg) , 0 );}

                            }// if buffer exp


//*****************************************************************************************

                            else {

                                for (z = 0; z < max_clients; z++)
                                    if(client_socket[z]!=0 && client_socket[z]!=sd)
                                    {
                                        sdm = client_socket[z];
                                        message=names[i];
                                        snprintf(lesmsg, 40, "%s", message);
                                        send(sdm , lesmsg , strlen(lesmsg) , 0 );
                                        snprintf(lesmsg, 40, "%s"," : " );
                                        send(sdm, lesmsg , strlen(lesmsg) , 0 );
                                        send(sdm , buffer , strlen(buffer) , 0 );
                                    }
                            }

                        }

//****************************************************************************************

                    }
                }

            }


        }


    }

    return 0;


} //main




int egal(char m1[],char m2[]){
    int i;


    for ( i = 0 ; i < strlen(m2) ; i++)
        if(m1[i] != m2[i])
            return 0;

    return 1;



}


