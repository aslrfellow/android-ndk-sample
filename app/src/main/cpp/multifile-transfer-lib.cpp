#include <jni.h>
#include <string>
#include <stdio.h>
#include <string.h>


#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define MAXDATASIZE 100
#define PORT 1500
#define MYPORT 1500
#define BACKLOG 5

// android logging
#include <android/log.h>
#define APPNAME "HelloWorldCPP1"

//file listing
#include <dirent.h>

//socket multi

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <sys/socket.h>
//#include <sys/types.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#define PORT 4444

#include "transfer.h"

//https://github.com/omair18/Socket-Programming-in-C/blob/master/receive_file.c

void writefile(int sockfd, FILE *fp);
void sendfullfile(FILE *fp, int sockfd);
void listsdcarddir();

ssize_t total=0;

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_socketclientmultifile1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    const char *str = env->GetStringUTFChars(inputValue1,0);

    //https://www.sendatext.co  free text

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket client multifile start");

    char input_filename[200] = {0};
    sprintf(input_filename,"%s/%s", "/sdcard/Pictures/CameraDemo", str);

    //char input_filename[200] = "/sdcard/Pictures/CameraDemo/2_pic.jpg";
    char input_hostname[200] = "192.168.0.210";

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Can't allocate sockfd");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVERPORT);
    if (inet_pton(AF_INET, input_hostname, &serveraddr.sin_addr) < 0)
    {
        perror("IPaddress Convert Error");
        exit(1);
    }

    if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("Connect Error");
        exit(1);
    }

    char *filename = basename(input_filename);
    if (filename == NULL)
    {
        perror("Can't get filename");
        exit(1);
    }

    char buff[BUFFSIZE] = {0};
    strncpy(buff, filename, strlen(filename));
    if (send(sockfd, buff, BUFFSIZE, 0) == -1)
    {
        perror("Can't send filename");
        exit(1);
    }

    FILE *fp = fopen(input_filename, "rb");
    if (fp == NULL)
    {
        perror("Can't open file");
        exit(1);
    }

    sendfullfile(fp, sockfd);
    //puts("Send Success");
    printf("Send Success, NumBytes = %ld\n", total);
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Send Success, NumBytes = %ld\n", total);
    fclose(fp);
    close(sockfd);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket client multifile end");

    std::string result = "Socket multifile send!";
    env->ReleaseStringUTFChars(inputValue1, str);
    return env->NewStringUTF(result.c_str());

}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_socketservermultifile1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket server multifile start");

    int connfd;
    pid_t childpid;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("Can't allocate sockfd");
        exit(1);
    }

    struct sockaddr_in clientaddr, serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    if (bind(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1)
    {
        perror("Bind Error");
        exit(1);
    }

    // if (listen(sockfd, LINSTENPORT) == -1)
    // {
    //     perror("Listen Error");
    //     exit(1);
    // }

    if(listen(sockfd, 10) == 0){
        printf("[+]Listening....\n");
    }else{
        printf("[-]Error in listening.\n");
    }

    socklen_t addr_size;

    while(1){

        //socklen_t addrlen = sizeof(clientaddr);
        //connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addrlen);
        connfd = accept(sockfd, (struct sockaddr *) &clientaddr, &addr_size);
        if (connfd == -1)
        {
            perror("Connect Error");
            exit(1);
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

        if((childpid = fork()) == 0){

            close(sockfd);

            char filename[BUFFSIZE] = {0};
            if (recv(connfd, filename, BUFFSIZE, 0) == -1)
            {
                perror("Can't receive filename");
                exit(1);
            }

            //https://www.onlinegdb.com/online_c_compiler

            //FILE *fp = fopen(filename, "wb");

            char full_filename[BUFFSIZE] = {0};

            sprintf(full_filename,"%s/%s", "/sdcard", filename);

            FILE *fp = fopen(full_filename, "wb");
            //FILE *fp = fopen(out, "wb");
            if (fp == NULL)
            {
                perror("Can't open file");
                exit(1);
            }

            char addr[INET_ADDRSTRLEN];
            printf("Start receive file: %s from %s\n", filename, inet_ntop(AF_INET, &clientaddr.sin_addr, addr, INET_ADDRSTRLEN));
            writefile(connfd, fp);
            printf("Receive Success, NumBytes = %ld\n", total);

            fclose(fp);

            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Receive Success, NumBytes = %ld\n", total);

            listsdcarddir();

            printf("fp closed");

        }

//        else {
//            break;
//        }

    }

    close(connfd);
    printf("connfd closed");

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket server multifile end");
    std::string result = "Socket multifile!";
    return env->NewStringUTF(result.c_str());

}

void listsdcarddir() {

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File listing start");

    DIR *d;
    struct dirent *dir;
    //d = opendir("/system/bin/");
    d = opendir("/sdcard/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s\n", dir->d_name);
        }
        closedir(d);
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File listing end");

}

void writefile(int sockfd, FILE *fp)
{
    ssize_t n;
    char buff[MAX_LINE] = {0};
    while ((n = recv(sockfd, buff, MAX_LINE, 0)) > 0)
    {
        total+=n;
        if (n == -1)
        {
            perror("Receive File Error");
            exit(1);
        }

        if (fwrite(buff, sizeof(char), n, fp) != n)
        {
            perror("Write File Error");
            exit(1);
        }
        memset(buff, 0, MAX_LINE);
    }
}

void sendfullfile(FILE *fp, int sockfd)
{
    int n;
    char sendline[MAX_LINE] = {0};
    while ((n = fread(sendline, sizeof(char), MAX_LINE, fp)) > 0)
    {
        total+=n;
        if (n != MAX_LINE && ferror(fp))
        {
            perror("Read File Error");
            exit(1);
        }

        if (send(sockfd, sendline, n, 0) == -1)
        {
            perror("Can't send file");
            exit(1);
        }
        memset(sendline, 0, MAX_LINE);
    }
}