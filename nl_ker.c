#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/init.h>
#include <linux/ip.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <net/netlink.h>
#include <net/sock.h>

#define NETLINK_TEST 17
#define MAX_MSGSIZE 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fkm");

// pointer to kernel sock
struct sock* nl_sk = 0;

// send msg to user space
void sendnlmsg(char* msg, int pid){
  struct sk_buff* skb;
  struct nlmsghdr* nlh;
  int len = NLMSG_SPACE(MAX_MSGSIZE);
  int slen = 0;
  if(!msg||!nl_sk) return;

  // skb = alloc_skb(len, GFP_KERNEL);
  skb = nlmsg_new(MAX_MSGSIZE, GFP_ATOMIC);
  if(!skb){
    printk(KERN_ERR"error allocating skb\n");
    return;
  }
  slen = strlen(msg)+1;
  nlh = nlmsg_put(skb, 0, 0, 0, MAX_MSGSIZE, 0);
  NETLINK_CB(skb).portid = 0;
  NETLINK_CB(skb).dst_group = 0;

  msg[slen] = 0;
  memcpy(NLMSG_DATA(nlh), msg, slen+1);

  if(netlink_unicast(nl_sk, skb, pid, 0)<0){
    printk(KERN_ERR"unicast error\n");
    return;
  }
  printk("send ok\n");

  return;
}

static void nl_data_ready(struct sock* sk, int len){
  struct sk_buff* skb;
  struct nlmsghdr* nlh = 0;
  while((skb = skb_dequeue(&sk->sk_receive_queue))!=0){
    nlh = (struct nlmsghdr*)skb->data;
    printk("%s: received netlink msg payload: %s\n", __FUNCTION__, (char*)NLMSG_DATA(nlh));
    // kfree_skb(skb);
    sendnlmsg("i see you", nlh->nlmsg_pid);
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
