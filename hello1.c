#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Bohdan Bihdash <bogdan.bigdash@gmail.com>");
MODULE_DESCRIPTION("Hello1 module for managing list and timing");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_entry {
    struct list_head list;
    ktime_t start_time;
    ktime_t end_time;
};

static LIST_HEAD(hello_list);

void print_hello(int times)
{
    int i;
    struct hello_entry *entry;

    if (times == 0 || (times >= 5 && times <= 10)) {
        pr_warn("Warning: times = %d, but continuing.\n", times);
    } else if (times > 10) {
        pr_err("Error: times = %d is too large. Exiting.\n", times);
        return;
    }

    for (i = 0; i < times; i++) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) {
            pr_err("Memory allocation failed for list entry.\n");
            return;
        }

        entry->start_time = ktime_get();
        pr_info("Hello, world! %d\n", i + 1);
        entry->end_time = ktime_get();

        INIT_LIST_HEAD(&entry->list);
        list_add_tail(&entry->list, &hello_list);
    }
}
EXPORT_SYMBOL(print_hello);

static void clear_list(void)
{
    struct hello_entry *entry, *tmp;

    list_for_each_entry_safe(entry, tmp, &hello_list, list) {
        pr_info("Time taken: %llu ns\n",
                ktime_to_ns(entry->end_time) - ktime_to_ns(entry->start_time));
        list_del(&entry->list);
        kfree(entry);
    }
}

static int __init hello1_init(void)
{
    pr_info("Hello1 module loaded.\n");
    return 0;
}

static void __exit hello1_exit(void)
{
    clear_list();
    pr_info("Hello1 module unloaded.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

