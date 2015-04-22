#include <linux/ipv6.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv6.h>
#include <net/ipv6.h>

#define NF_DUMMY_MOD_VER "1.0.0"

/* Input device */
static struct net_device *indev;

static int debug_level = 2;
module_param(debug_level, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug_level, " enable debug traces (0 to 4, 4 is most verbose)");

static char *iniface = "lo";
module_param(iniface, charp, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(iniface, " input interface name - default 'lo'");

static int ttl = 0;
module_param(ttl, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(ttl, " IPv6 hop limit value for which to drop packets");

#define dummy_debug_printk(level, fmt, arg...)			\
	do {							\
		if (level <= debug_level)			\
			pr_debug("%s: " fmt, __func__, ##arg);	\
	} while (0)

#define debugf0( ... ) dummy_debug_printk(0, __VA_ARGS__ )
#define debugf1( ... ) dummy_debug_printk(1, __VA_ARGS__ )
#define debugf2( ... ) dummy_debug_printk(2, __VA_ARGS__ )
#define debugf3( ... ) dummy_debug_printk(3, __VA_ARGS__ )
#define debugf4( ... ) dummy_debug_printk(4, __VA_ARGS__ )

static unsigned int dummy_filter(const struct nf_hook_ops *ops,
				 struct sk_buff *skb,
				 const struct net_device *in,
				 const struct net_device *out,
				 int (*okfn)(struct sk_buff *))
{
	struct ipv6hdr *ipv6h;

	/* Packet ingress device match ? */
	if (in == indev) {
		ipv6h = ipv6_hdr(skb);
		/* Packet hop limit value match ? */
		if (ipv6h->hop_limit == ttl) {
			debugf0("pkt received on %s with hop limit %d - drop\n",
				in->name, ipv6h->hop_limit);
			return NF_DROP;
		}

		debugf4("pkt received on %s with hop limit %d\n",
			in->name, ipv6h->hop_limit);

	}

	return NF_ACCEPT;
}

static struct nf_hook_ops dummy_ops __read_mostly = {
	.hook		= dummy_filter,
	.owner		= THIS_MODULE,
	.pf		= NFPROTO_IPV6,
	.hooknum	= NF_INET_PRE_ROUTING,
	.priority	= NF_IP6_PRI_FIRST + 1,

};

static int __init nf_dummy_init(void)
{
	int ret;

	indev = __dev_get_by_name(&init_net, iniface);

	if (!indev) {
		printk(KERN_ERR "%s can't find device by name %s\n",
		       __func__, iniface);
		return -1;
	}

	ret = nf_register_hook(&dummy_ops);
	if (ret < 0) {
		printk(KERN_ERR "%s can't register NF hook\n", __func__);
		return -1;
	}
	
	return 0;
}

static void __exit nf_dummy_fini(void)
{
	nf_unregister_hook(&dummy_ops);
}

module_init(nf_dummy_init);
module_exit(nf_dummy_fini);

MODULE_DESCRIPTION("Example IPv6 Netfilter hook");
MODULE_LICENSE("GPL");
MODULE_VERSION(NF_DUMMY_MOD_VER);
