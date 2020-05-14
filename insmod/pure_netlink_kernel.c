#include <net/sock.h>
#include <linux/init.h>
#include <net/netlink.h>
#include <linux/types.h>
#include <linux/module.h>
 
#define PURE_NETLINK_MAX_MSG_SIZE 1024

struct sock *pure_sk = NULL;

void pure_netlink_send(char *message, int pid)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    int len = NLMSG_SPACE(PURE_NETLINK_MAX_MSG_SIZE);
    int msgLen = 0;

    if(!message || !pure_sk) {
        return ;
    }

    skb = alloc_skb(len, GFP_KERNEL);
    if(!skb){
        printk(KERN_ERR "pure_netlink_send: alloc skb error\n");
    }

    msgLen = strlen(message);
    nlh = nlmsg_put(skb,0,0,0,PURE_NETLINK_MAX_MSG_SIZE,0);

    NETLINK_CB(skb).portid = 0;
    NETLINK_CB(skb).dst_group = 0;
    memcpy(NLMSG_DATA(nlh), message, msgLen + 1);

    printk("pure_netlink_send: %s\n",(char *)NLMSG_DATA(nlh));

    netlink_unicast(pure_sk, skb, pid, MSG_DONTWAIT);
}

static void pure_netlink_recv(struct sk_buff *__skb)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	char receive[128];
	char *send;

	skb = skb_get(__skb);
	if(skb->len >= NLMSG_SPACE(0)) {
		nlh = nlmsg_hdr(skb);
		memcpy(receive, NLMSG_DATA(nlh), sizeof(receive));
		printk("pure_netlink_recv:%s\n", receive) ;

		send = "I am from kernel!";
		pure_netlink_send(send, nlh->nlmsg_pid);
		kfree_skb(skb);
	}
}

static int __net_init pure_netlink_net_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.groups		= RTNLGRP_MAX,
		.input		= pure_netlink_recv,
		.cb_mutex	= NULL,
		.flags		= NL_CFG_F_NONROOT_RECV,
	};

	printk("pure_netlink_net_init: start.\n");

	pure_sk = netlink_kernel_create(&init_net, 25, &cfg);
	if (!pure_sk) {
		printk("pure_netlink_net_init: failed.\n");
		return -ENOMEM;
	}

	printk("pure_netlink_net_init: success.\n");
	return 0;
}

static void __net_exit pure_netlink_net_exit(void)
{
	netlink_kernel_release(pure_sk);
	pure_sk = NULL;
	printk("pure_netlink_net_exited\n");
}


module_init(pure_netlink_net_init);
module_exit(pure_netlink_net_exit);