#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Bohdan Bihdash <bogdan.bigdash@gmail.com>");
MODULE_DESCRIPTION("Hello2 module using Hello1's print_hello");
MODULE_LICENSE("Dual BSD/GPL");

static uint times = 1;
module_param(times, uint, 0644);
MODULE_PARM_DESC(times, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void)
{
    pr_info("Hello2 module loaded.\n");
    print_hello(times);
    return 0;
}

static void __exit hello2_exit(void)
{
    pr_info("Hello2 module unloaded.\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
