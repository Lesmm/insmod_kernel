#include <linux/init.h>
#include <linux/module.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/netlink.h>

#define NETLINK_TEST 25
#define MAX_MSGSIZE 1024

int stringlength(char *s);
int err;
struct sock *nl_sk = NULL;
int flag = 0;
//向用户态进程回发消息
void sendnlmsg(char *message, int pid)
{
    struct sk_buff *skb_1;
    struct nlmsghdr *nlh;
    int len = NLMSG_SPACE(MAX_MSGSIZE);
    int slen = 0;
    char symlink[128];
    const char *mCache = "/data/local/rom/cache";

    if(!message || !nl_sk)
    {
        return ;
    }
    printk(KERN_ERR "prot_id:%d\n",pid);

    printk(KERN_ERR "pid:%d\n",current->pid);
    printk(KERN_ERR "loginuid.val:%d\n",(current->loginuid).val);
    printk(KERN_ERR "current_uid():%d\n",current_uid().val);
    printk(KERN_ERR "current->cred->uid:%d\n",(current->cred->uid).val);
    printk(KERN_ERR "comm:%s\n",current->comm);

    printk(KERN_ERR "argv:%s\n",(char *)(current->mm->arg_start));

    memset(symlink, '\0', sizeof(symlink));
    sprintf(symlink, "%s/%u",  mCache, current_uid().val); // TODO ... in kernel no getuid() function, use current_uid().val instead
    printk(KERN_ERR "symlink:%s\n",symlink);
    

    skb_1 = alloc_skb(len,GFP_KERNEL);
    if(!skb_1)
    {
        printk(KERN_ERR "my_net_link:alloc_skb error\n");
    }
    slen = stringlength(message);

    nlh = nlmsg_put(skb_1,0,0,0,MAX_MSGSIZE,0);

    NETLINK_CB(skb_1).portid = 0;
    NETLINK_CB(skb_1).dst_group = 0;
    
    // message[slen]= '\0';
    memcpy(NLMSG_DATA(nlh),message,slen+1);

    printk("my_net_link:send message '%s'.\n",(char *)NLMSG_DATA(nlh));
    netlink_unicast(nl_sk,skb_1,pid,MSG_DONTWAIT);
}
int stringlength(char *s)
{
    int slen = 0;
    for(; *s; s++)
    {
        slen++;
    }
    return slen;
}
//接收用户态发来的消息
void nl_data_ready(struct sk_buff *__skb)
 {
     struct sk_buff *skb;
     struct nlmsghdr *nlh;
     char str[100];
     struct completion cmpl;
     int i=10;
     int pid;
     printk("begin data_ready\n");
     skb = skb_get (__skb);
     if(skb->len >= NLMSG_SPACE(0))
     {
         nlh = nlmsg_hdr(skb);
         memcpy(str, NLMSG_DATA(nlh), sizeof(str));
         printk("Message received:%s\n",str) ;
         pid = nlh->nlmsg_pid;
         while(i--)
        {//我们使用completion做延时，每3秒钟向用户态回发一个消息
            init_completion(&cmpl);
            wait_for_completion_timeout(&cmpl,3 * HZ);
            sendnlmsg("I am from kernel!",pid);
        }
        printk("Message send done.~~~~~ \n");
         flag = 1;
         kfree_skb(skb);
    }
 }
// Initialize netlink
int netlink_init(void)
{
    struct netlink_kernel_cfg cfg = {
        .groups     = RTNLGRP_MAX,
        .input      = nl_data_ready,
        .cb_mutex   = NULL,
        .flags      = NL_CFG_F_NONROOT_RECV,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
    if(!nl_sk){
        printk(KERN_ERR "my_net_link: create netlink socket error.\n");
        return 1;
    }
    printk("my_net_link_7: create netlink socket ok.\n");
    return 0;
}
static void netlink_exit(void)
{
    if(nl_sk != NULL){
        sock_release(nl_sk->sk_socket);
    }
    printk("my_net_link: self module exited\n");
}
module_init(netlink_init);
module_exit(netlink_exit);
MODULE_AUTHOR("zhao_h");
MODULE_LICENSE("GPL");