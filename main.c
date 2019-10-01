#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SERVER_IP       "127.0.0.1"
#define SERVER_PORT     8000
#define BUFF_SIZE       1024


/**
 * 输入bye退出
 */

int ret;

int main() {

    int client_socket;
    struct sockaddr_in server_addr;
    unsigned char send_buf[BUFF_SIZE];
    unsigned char recv_buf[BUFF_SIZE];

    //创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        printf("create socket fail\n");
    }
    printf("success to create socket\n");

    //设置服务器地址
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    system("netstat -an | grep 8000");	// 查看连接状态
    ret = connect(client_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        printf("connect fail\n");
        close(client_socket);
        exit(-1);
    }
    printf("connect success\n");
    system("netstat -an | grep 8000");	// 查看连接状态

    while (true) {
        printf("input message: ");
        if (fgets(send_buf, sizeof(send_buf)-1, stdin)) {

            //去除换行符，fgets会读换行符
            send_buf[strlen(send_buf)-1] = '\0';

            //判断是否结束
            if (strcmp(send_buf, "bye") == 0) {
                close(client_socket);
                printf("stop client\n");
                exit(-1);
            }

            //发送数据
            ret = send(client_socket, send_buf, strlen(send_buf), 0);
            if (ret <= 0) {
                printf("send fail,  please try again\n");
                continue;
            }

            //接受服务端数据
            int len = recv(client_socket, recv_buf, BUFF_SIZE-1, 0);
            recv_buf[len] = '\0';
            printf("send success, received from server: %s\n", recv_buf);

        } else {
            printf("get input msg fail, please try again\n");
            continue;
        }
    }

}