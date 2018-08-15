#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>

dev_t dev=0;
static struct cdev my_dev;
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
 
static struct file_operations fops =
{
.owner          = THIS_MODULE,
.read           = etx_read,
.write          = etx_write,
.open           = etx_open,
.release        = etx_release,
};
 
static int etx_open(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Driver Open Function Called...!!!\n");
        return 0;
}
 
static int etx_release(struct inode *inode, struct file *file)
{
        printk(KERN_INFO "Driver Release Function Called...!!!\n");
        return 0;
}
 
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Driver Read Function Called...!!!\n");
        return 0;
}
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        printk(KERN_INFO "Driver Write Function Called...!!!\n");
        return len;
}
 
static int __init init_func(void){
	if((alloc_chrdev_region(&dev,0,1,"my_devick"))<0){
		printk(KERN_ALERT "unable to create specified device");
		return -1;
	}
	printk(KERN_ALERT "created major= %d and minor=%d",MAJOR(dev),MINOR(dev));
	cdev_init(&my_dev,&fops);
	my_dev.owner = THIS_MODULE;
	my_dev.ops   = &fops;
	if((cdev_add(&my_dev,dev,1)) < 0){
		printk("structure not initialized");
		return -1;
	}
}
static void __exit exit_func(void){
	printk(KERN_INFO "bye bye cruel world");
	unregister_chrdev(dev,1);
	cdev_del(&my_dev);
}
module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("mawanda naco@bad guy ent");
MODULE_DESCRIPTION("A simple character driver to illustrate where i have reached");

