#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include<linux/moduleparam.h>
int major = 0;
int minor = 0;
int result;
dev_t dev = 0;
module_param(major,int,0);
module_param(minor,int,0);
struct file_operations fops={

};
static int __init hello_world_init(void){
	printk(KERN_ALERT "major = %d, & minor = %d ", major,minor);
	if((alloc_chrdev_region(&dev, 0, 1, "nacos")) <0){
                printk(KERN_INFO "Cannot allocate major number for device 1\n");
                return -1;
        }
        printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
        printk(KERN_INFO "Kernel Module Inserted Successfully dynamically...\n");
        return 0;

}
 
void __exit hello_world_exit(void)
{
        unregister_chrdev_region(dev, 1);
 printk(KERN_INFO "Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mawanda henry<mawandahenrynaco@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("1.1.2");

