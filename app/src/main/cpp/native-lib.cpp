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

    int cliSock, numbytes;
    char buf[MAXDATASIZE], msg[MAXDATASIZE];

    struct hostent *he;

    if ((he = gethostbyname("192.168.0.210")) == NULL)
    {
        printf("Couldn't get hostname\n");
        exit(1);
    }

    struct sockaddr_in dest_addr;

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
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