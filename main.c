//
// Created by YU on 2019/10/1.
//

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>


#define SERVER_IP       "www.yunshansou.com"
#define SERVER_PORT     8080
#define BUFF_SIZE       1024

int ret;

int main() {

    int client_socket;
    struct sockaddr_in server_addr;
    unsigned char send_buf[BUFF_SIZE];

    //创建套接字
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket == -1) {
        printf("create socket fail\n");
    }
    printf("success to create socket\n");

    //设置服务器地址
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    ret = connect(client_socket, (const struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    if (ret == -1) {
        printf("connect fail\n");
        return -1;
    }

    while (true) {
        ret = fgets(send_buf, sizeof(send_buf)-1, stdin);
        if (ret) {
            //判断是否结束
            if (strcmp(send_buf, "q")) {
                printf("stop client\n");
                break;
            }

            //发送数据
            ret = send(client_socket, send_buf, strlen(send_buf), 0);
            if (ret <= 0) {
                printf("send fail\n");
            }
        } else {
            printf("get input msg fail, please try again\n");
            continue;
        }
    }

    close(client_socket);
    return 0;

}