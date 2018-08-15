#include <linux/module.h>
#include<linux/moduleparam.h>
#include <linux/kernel.h>
#include<linux/sched.h>
#include<asm/current.h>
#include <linux/init.h>
#define USER "Mawanda henry <mawanda111@gmail.com>"
#define DESR "a simple driver to print hello world"
#define SUPPORT "AMD, intel, SPARC,ARM"

static int hello3_data __initdata = 3;
int count=1;
module_param(count,int,0);

static int __init hello_3_init(void){
int index=-1;
for(index=0; index<count; index++){
	printk(KERN_INFO "count is %d\n",index);
	
}
printk(KERN_INFO "Hello, world %d\n", hello3_data);
return 0;
}
static void __exit hello_3_exit(void)
{
printk(KERN_INFO "Goodbye, world 3\n");
}



module_init(hello_3_init);
module_exit(hello_3_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(USER);
MODULE_DESCRIPTION(DESR);
MODULE_SUPPORTED_DEVICE(SUPPORT);



