#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <errno.h>


#define PURE_NETLINK_MAX_MSG_SIZE 1024


int main(int argc, char* argv[])
{
    int recv_state;
    int send_state = 0;

    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh = NULL;

    struct iovec iov;
    struct msghdr msg;

    int sock_fd, bind_status;

    char *sendMessage = "Hello you!";
    

    // Create socket
    sock_fd = socket(AF_NETLINK, SOCK_RAW, 25);
    if(sock_fd == -1){
        printf("error getting socket: %s", strerror(errno));
        return -1;
    }

    // To prepare binding
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = 100; //A：设置源端端口号
    src_addr.nl_groups = 0;

    // Bind
    bind_status = bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    if(bind_status < 0){
        printf("bind sock_fd failed: %s", strerror(errno));
        close(sock_fd);
        return -1;
    }

    // To prepare create message
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(PURE_NETLINK_MAX_MSG_SIZE));
    if(!nlh){
        printf("malloc struct nlmsghdr error!\n");
        close(sock_fd);
        return -1;
    }


    memset(&dest_addr,0,sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; //B：设置目的端口号
    dest_addr.nl_groups = 0;
    nlh->nlmsg_len = NLMSG_SPACE(PURE_NETLINK_MAX_MSG_SIZE);
    nlh->nlmsg_pid = 100; //C：设置源端口
    nlh->nlmsg_flags = 0;
    strcpy(NLMSG_DATA(nlh), sendMessage); //设置消息体
    iov.iov_base = (void *)nlh;
    iov.iov_len = NLMSG_SPACE(PURE_NETLINK_MAX_MSG_SIZE);

    // Create message
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Send message
    send_state = sendmsg(sock_fd,&msg,0);
    printf("Sent state: %d, message: %s\n", send_state, (char *)NLMSG_DATA(nlh));

    memset(nlh,0,NLMSG_SPACE(PURE_NETLINK_MAX_MSG_SIZE));

    // Receive message
    while(1){
    printf("Waiting for receive...\n");
    recv_state = recvmsg(sock_fd, &msg, 0);
    printf("Received state: %d, message: %s\n", recv_state, (char *)NLMSG_DATA(nlh));
}

    close(sock_fd);
    return 0;
}