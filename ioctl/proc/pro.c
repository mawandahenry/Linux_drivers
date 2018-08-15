#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/ioctl.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/types.h>
#include<asm/io.h>
#include<linux/proc_fs.h>

#define SIZE 1024
dev_t device_n = 0;
uint8_t *kern = "hello world";
struct cdev my_cdevn;
static int __init enter_function(void);
static void __exit close_down(void);
static int this_open(struct inode *inode, struct file *file);
static int this_close(struct inode *inode, struct file *file);
static ssize_t this_read(struct file *file, size_t len, const char __user *buf, loff_t *off);
static ssize_t this_write(struct file *file, size_t len, const char __user *buf, loff_t *off);

static int proc_open(struct inode *inode, struct file *file);
static int proc_close(struct inode *inode, struct file *file);
static ssize_t proc_read(struct file *file, size_t len, const char __user *buf, loff_t *off);
static ssize_t proc_write(struct file *file, size_t len, const char __user *buf, loff_t *off);

static int this_close(struct inode *inode, struct file *file){
	printk(KERN_INFO "file is closed");
	return 0;
}

static ssize_t this_write(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO "writing to file\n");
	copy_from_user(kern, buf,len);
	return len;
}

static ssize_t this_read(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO " reading data");
	copy_to_user(buf, kern, len);
	return len;
}

static int proc_open(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "file is open");
	if((kern = kmalloc(SIZE, GFP_KERNEL)) < 0){
		printk(KERN_ALERT "unable to allocate memory");
		return -1;
}
	return 0;
}

static int proc_close(struct inode *inode, struct file *file){
	printk(KERN_INFO "file is closed");
	return 0;
}

static ssize_t proc_write(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO "writing to file\n");
	copy_from_user(kern, buf,len);
	return len;
}

static ssize_t proc_read(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO " reading data");
	memcpy(buf, kern, len);
	return len;
}

static int this_open(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "file is open");
	kern = kmalloc(SIZE ,GFP_KERNEL);
	return 0;
}

struct file_operations proc_fops = {
	
	.read  = proc_read,
	.write = proc_write,
	.release = proc_close,
	.open = proc_open
};

struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read  = this_read,
	.write = this_write,
	.release = this_close,
	.open = this_open
};

static int __init enter_function(void){
	
	if((alloc_chrdev_region(&device_n,0,1,"naco")) < 0){
		printk(KERN_INFO "failed to register the device");
		return -1;
	}
	printk(KERN_INFO "created file with major number %d && minor %d\n", MAJOR(device_n),MINOR(device_n));
	cdev_init(&my_cdevn, &my_fops);
	my_cdevn.ops = &my_fops;
	my_cdevn.owner = THIS_MODULE;
	if((cdev_add(&my_cdevn,device_n,1)) < 0){
		printk(KERN_ALERT "dev structure not successfully initialized\n");
		return -1;
	}
	proc_create("naco",0666,NULL,&proc_fops);
	return 0;
}
static void __exit close_down(void){
	printk(KERN_ALERT "exiting module......\n");
	unregister_chrdev_region(device_n,"naco");
	cdev_del(&my_cdevn);
	remove_proc_entry("naco",NULL);
	
}
module_init(enter_function);
module_exit(close_down);

MODULE_AUTHOR("NACO SES <mawanda111@gmail.com>");
MODULE_VERSION("1.0.0");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("a simple character driver to implement procfs functions");
	

