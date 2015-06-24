#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/netlink.h>
#include <net/sock.h>

#define NETLINK_TEST 17

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fkm");

struct sock* nl_sk = 0;
static void nl_data_ready(struct sock* sk, int len){
  struct sk_buff* skb;
  struct nlmsghdr* nlh = 0;
  while((skb = skb_dequeue(&sk->sk_receive_queue))){
    nlh = (struct nlmsghdr*)skb->data;
    printk("%s: received netlink msg payload: %s\n", __FUNCTION__, (char*)NLMSG_DATA(nlh));
    kfree_skb(skb);
  }
  printk("received finished\n");
}

static int __init nl_init(void){
  struct netlink_kernel_cfg cfg = {.input = (void*)nl_data_ready,};
  printk("nelink in\n");
  nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);
  if(!nl_sk) printk(KERN_ERR"failed to create netlink socket\n");
  return 0;
}

static void __exit nl_exit(void){
  printk("netlink out\n");
  sock_release(nl_sk->sk_socket);
}

module_init(nl_init);
module_exit(nl_exit);
