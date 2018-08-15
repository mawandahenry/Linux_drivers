#include<linux/slab.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/cdev.h>
#include<linux/init.h>
#include<linux/kdev_t.h>
#include<linux/uaccess.h>
#include<linux/fs.h>

#define mem_size 1024
dev_t dev_str = 0;
uint8_t *KERNEL_BUFFER;
static struct cdev dev_hol;

static int __init first(void);
static void __exit exit_function(void);
static int naco_open(struct inode *inode, struct file *file);
static int naco_let(struct inode *inode, struct file *file);
static ssize_t naco_read(struct file *filp, char __user *buff, size_t len,loff_t *off);
static ssize_t naco_write(struct file *filp, const char *buff, size_t len, loff_t *off);
 
static int naco_open(struct inode *inode, struct file *file){
	if((KERNEL_BUFFER = kmalloc(mem_size, GFP_KERNEL))<0){
		printk(KERN_ALERT "no memory allocation took place");
		return -1;
	}
	printk(KERN_INFO "Device open and ready reading and writing");
	return 0;
}
static ssize_t naco_read(struct file *filp, char __user *buff, size_t len,loff_t *off){
	printk(KERN_INFO "reading operation");
	copy_to_user(buff,KERNEL_BUFFER,mem_size);
	return mem_size;
}
static ssize_t naco_write(struct file *filep,const char __user *buff, size_t len, loff_t *off){
	printk(KERN_INFO "writing operation going on");
	copy_from_user(buff,KERNEL_BUFFER,len);
	return len;
}

static int naco_let(struct inode *inode, struct file *file){
	printk(KERN_INFO "closing file operations");
	kfree(KERNEL_BUFFER);
	return 0;
}
static struct file_operations my_ops = {
        .owner = THIS_MODULE,
        .open  = naco_open,
        .read  = naco_read,
        .write = naco_write,
        .release = naco_let
};

static int __init first(void){
	if((alloc_chrdev_region(&dev_str,0,1,"mawanda"))<0){
		printk(KERN_ALERT "device allocation numbers failed");
		return -1;
	}
	else{
		printk(KERN_INFO "allocated %d-major and %d-minor", MAJOR(dev_str),MINOR(dev_str));
		cdev_init(&dev_hol,&my_ops);
		dev_hol.owner = THIS_MODULE;
		dev_hol.ops   = &my_ops;
		return 0;
	}
	if((cdev_add(&dev_hol,dev_str,1))<0){
		printk(KERN_ALERT "structure not registered");
		return -1;
	}
}

static void __exit exit_function(void){
	printk(KERN_INFO "signing out officially");
	unregister_chrdev_region(dev_str,1);
	cdev_del(&dev_hol);
}
module_init(first);
module_exit(exit_function);
MODULE_AUTHOR("mawanda henry <mawanda111@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("a simple character driver");
MODULE_VERSION("1.2.10");
