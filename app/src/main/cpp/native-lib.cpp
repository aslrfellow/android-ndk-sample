#include <jni.h>
#include <string>
#include <stdio.h>


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


extern "C" JNIEXPORT jstring JNICALL
Java_com_example_helloworldcpp1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Increment / Decrement number";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_helloworldcpp1_MainActivity_numberFromJNI(
        JNIEnv *env,
        jobject /* this */) {
        std::string hello = "99";
        return env->NewStringUTF(hello.c_str());
}

//https://medium.com/@jrejaud/modern-android-ndk-tutorial-630bc11829a2

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_helloworldcpp1_MainActivity_addTwoNumber(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "99";
    std::int32_t num = 100;

    int firstNumber, secondNumber, sumOfTwoNumbers;
    firstNumber = 10;
    secondNumber = 20;
    sumOfTwoNumbers = firstNumber + secondNumber;

    std::string result = std::to_string(sumOfTwoNumbers);

    return env->NewStringUTF(result.c_str());
}



extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_addTwoNumber2(
        JNIEnv *env,
        jobject /* this */,
        jint inputValue1) {

    std::string hello = "99";
    std::int32_t num = 100;

    int firstNumber, secondNumber, sumOfTwoNumbers;
    firstNumber = 10;
    secondNumber = 20;
    //sumOfTwoNumbers = firstNumber + secondNumber + inputValue1;

    sumOfTwoNumbers = inputValue1 + 1;

    std::string result = std::to_string(sumOfTwoNumbers);

    return env->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_decTwoNumber2(
        JNIEnv *env,
        jobject /* this */,
        jint inputValue1) {

    std::string hello = "99";
    std::int32_t num = 100;

    int firstNumber, secondNumber, sumOfTwoNumbers;
    firstNumber = 10;
    secondNumber = 20;
    //sumOfTwoNumbers = firstNumber + secondNumber + inputValue1;

    int *ptr_one;
    ptr_one = (int *)malloc(sizeof(int));
    if (ptr_one == 0)
    {
        printf("ERROR: Out of memory\n");
        return env->NewStringUTF("Error");
    }
    *ptr_one = 1;


    typedef struct rec
    {
        int i;
        float PI;
        char A;
    }RECORD;
    RECORD *ptr_two;
    ptr_two = (RECORD *) malloc (sizeof(RECORD));
    (*ptr_two).i = 1;
    (*ptr_two).PI = 3.14;
    (*ptr_two).A = 'a';


    struct rec3
    {
        int i;
        float PI;
        char A;
    };
    struct rec3 *ptr_three;
    ptr_three = (struct rec3 *) malloc (sizeof(struct rec3));
    ptr_three->i = 1;
    ptr_three->PI = 3.14;
    ptr_three->A = 'a';

    sumOfTwoNumbers = inputValue1 - 1 - *ptr_one - (*ptr_two).i - ptr_three->i;

    free(ptr_one);
    free(ptr_two);
    free(ptr_three);

    std::string result = std::to_string(sumOfTwoNumbers);

    return env->NewStringUTF(result.c_str());
}


extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_filltext(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    //https://electrofriends.com/interview-questions/jni-faq/convert-jstring-cstyle-string-vice-versa/
    const char *str = env->GetStringUTFChars(inputValue1,0);

    //https://www.tutorialspoint.com/c_standard_library/c_function_malloc.htm
    char *str2;
    str2 = (char *) malloc(5);
    strcpy(str2, str);

    //http://www.cplusplus.com/reference/cstdio/sprintf/
    //stack corruption detected (-fstack-protector)
    char buffer [50];
    int n, a=5, b=3;
    n=sprintf (buffer, "\\x%u %s \n\\x%u %s \n%d plus %d is %d", str2, str2, str, str, a, b, a+b);
    std::string result = buffer;

    free(str2);
    env->ReleaseStringUTFChars(inputValue1, str);

    return env->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_basicuaf1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    int *ptr_one = (int *)malloc(sizeof(int));
    if (ptr_one == 0)
    {
        printf("ERROR: Out of memory\n");
        return env->NewStringUTF("Error");
    }
    *ptr_one = 1;
    free(ptr_one); //use-after-free

    // https://sensepost.com/blog/2017/linux-heap-exploitation-intro-series-used-and-abused-use-after-free/

    int *ptr_two = (int *)malloc(0);
    *ptr_two = 7;
    std::string result = std::to_string(*ptr_one);

    //free(ptr_one);
    free(ptr_two);

    return env->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_socketclient1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    //https://stackoverflow.com/questions/6033581/using-socket-in-android-ndk
    //https://stackoverflow.com/questions/14161720/trouble-with-running-server-client-c-program-on-mac-os-x-mountain-lion

    int cliSock, numbytes;
    char buf[MAXDATASIZE], msg[MAXDATASIZE];

    struct hostent *he;

    //0.tcp.ngrok.io:12745

    if ((he = gethostbyname("0.tcp.ngrok.io")) == NULL)
    //if ((he = gethostbyname("192.168.0.210")) == NULL)
    {
        printf("Couldn't get hostname\n");
        exit(1);
    }

    struct sockaddr_in dest_addr;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(12745);
    //dest_addr.sin_port = htons(PORT);
    dest_addr.sin_addr = *((struct in_addr *)he->h_addr);

    if ((cliSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("Socket Error: %d\n", errno);
    else
        printf("Client Socket %d created\n", cliSock);

    if (connect(cliSock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == 1)
        printf("Connect Error: %d\n", errno);
    else
    {
        printf("Client Connection created\n");
    }

    numbytes = recv(cliSock, buf, MAXDATASIZE, 0);
    buf[numbytes] = '\0';
    printf("Received Message: %s\n", buf);

    sprintf(msg, "4 8 15 16 23 42");
    send(cliSock, msg, MAXDATASIZE, 0);
    printf("Client sent %s to %s\n", msg, inet_ntoa(dest_addr.sin_addr));

    close(cliSock);
    printf("Client Sockets closed\n");


    std::string result = "Sent!";

    return env->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_socketserver1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    //https://stackoverflow.com/questions/6033581/using-socket-in-android-ndk
    //https://stackoverflow.com/questions/14161720/trouble-with-running-server-client-c-program-on-mac-os-x-mountain-lion

    //https://stackoverflow.com/questions/4629308/any-simple-way-to-log-in-android-ndk-code
    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "The value of 1 + 1 is %d", 1+1);

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Start socketserver1 nc 192.168.0.26 1500");


    int servSock, cliProc;
    socklen_t sin_size;
    char buf[MAXDATASIZE], msg[MAXDATASIZE];
    struct sockaddr_in my_addr, income_addr;

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if ((servSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        printf("Socket Error: %d\n", errno);
    else
        printf("Server Socket %d created\n", servSock);

    if (bind(servSock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
        printf("Bind Error: %d\n", errno);
    else
        printf("Server Bind created\n");

    listen(servSock, BACKLOG);
    printf("Server is waitting for connection...\n");

    sin_size = sizeof(struct sockaddr_in);
    if ((cliProc = accept(servSock, (struct sockaddr *)&income_addr, &sin_size)) == -1)
        printf("Accept Error: %d\n", errno);
    else
    {
        printf("Server accepted connection from %s\n", inet_ntoa(income_addr.sin_addr));
    }

    sprintf(msg, "Welcome to Server, you addr is %s", inet_ntoa(income_addr.sin_addr));
    send(cliProc, msg, strlen(msg), 0);

    if (recv(cliProc, buf, MAXDATASIZE, 0) == -1)
    {
        printf("Recv Error: %d\n", errno);
    }
    else
    {
        printf("Server received %s from %s\n", buf, inet_ntoa(income_addr.sin_addr));
    }

    close(cliProc);
    close(servSock);
    printf("Server Sockets closed\n");

    std::string result = "Listening!";

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "Finished socketserver1");

    return env->NewStringUTF(result.c_str());

}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_fileoperation1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {


    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File listing start");

    DIR *d;
    struct dirent *dir;
    //d = opendir("/system/bin/");
    d = opendir("/sdcard/Pictures/");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "%s\n", dir->d_name);
        }
        closedir(d);
    }

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "File listing end");
    std::string result = "Listening!";
    return env->NewStringUTF(result.c_str());

}

extern "C"
JNIEXPORT jstring
JNICALL
Java_com_example_helloworldcpp1_MainActivity_socketclientmulti1(
        JNIEnv *env,
        jobject /* this */,
        jstring inputValue1) {

    //https://github.com/nikhilroxtomar/Multiple-Client-Server-Program-in-C-using-fork/blob/master/tcpClient.c

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket client multi start");

    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");


    struct hostent *he;

    //0.tcp.ngrok.io:12745

    if ((he = gethostbyname("0.tcp.ngrok.io")) == NULL)
    //if ((he = gethostbyname("192.168.0.210")) == NULL)
    {
        printf("Couldn't get hostname\n");
        exit(1);
    }

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    //serverAddr.sin_port = htons(4444);
    serverAddr.sin_port = htons(12443);
    serverAddr.sin_addr = *((struct in_addr *)he->h_addr);


    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == 1) {
        printf("Connect Error: %d\n", errno);
    } else
    {
        printf("Client Connection created\n");
    }

    printf("Client: \t");
    memset(buffer, 0, sizeof buffer);
    strncpy(buffer, "some payload", sizeof buffer - 1);

    send(clientSocket, buffer, strlen(buffer), 0);

    if(recv(clientSocket, buffer, 1024, 0) < 0){
        printf("[-]Error in receiving data.\n");
    }else{
        printf("Server: \t%s\n", buffer);
    }

    memset(buffer, 0, sizeof buffer);
    strncpy(buffer, ":exit", sizeof buffer - 1);

    send(clientSocket, buffer, strlen(buffer), 0);

    close(clientSocket);
    printf("[-]Disconnected from server.\n");

    __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "socket client multi end");
    std::string result = "Socket multi!";
    return env->NewStringUTF(result.c_str());

}