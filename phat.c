#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/cdev.h>
#include<linux/mutex.h>

#define SIZE 1024
int my_major;
static int times=0;
static short readPos=0;

uint8_t *kernel_buffer;
static int __init start_function(void);
static void __exit stop_function(void);
static int naco_open(struct inode *inode, struct file *file);
static int naco_close(struct inode *inode, struct file *file);
static ssize_t naco_read(struct file *filp, char __user *buff, size_t len,loff_t *off);
static ssize_t naco_write(struct file *filp, const char *buff, size_t len, loff_t *off);
static DEFINE_MUTEX(naco_mutex_lock);

static int naco_open(struct inode *inode, struct file *file){
	if(!mutex_trylock(&naco_mutex_lock)){
		printk(KERN_ALERT "\ndevice is in use by other processes");
		return -EBUSY;
	}
	else{
	times++;
	if((kernel_buffer = kmalloc(SIZE, GFP_KERNEL)) < 0){
		printk(KERN_ALERT "unable to allocate memory");
		return -1;
	}else{
	printk(KERN_INFO "\ndevice ready open for %d time", times);
	mutex_unlock(&naco_mutex_lock);
	return 0;
	
      }
   }
}

static int naco_close(struct inode *inode, struct file *file){
	times--;
	printk(KERN_INFO "\ndevice closing.......");
	kfree(kernel_buffer);
	
	return 0;
}

static ssize_t naco_read(struct file *filp, char __user *buff, size_t len,loff_t *off){
	
	int error_count;
	printk(KERN_INFO "\nreading from the file");
	error_count = copy_to_user(buff,kernel_buffer,len);
	if(error_count == 0){
		return len;
}
else{
	return -EFAULT;
}

}

static ssize_t naco_write(struct file *filp, const char *buff, size_t len, loff_t *off){
	
	printk(KERN_INFO "\nwriting to the file");
	copy_from_user(kernel_buffer,buff,len);
	
	return len;
}

struct file_operations fops = {
	.owner   = THIS_MODULE,
	.read    = naco_read,
	.write   = naco_write,
	.open    = naco_open,
	.release = naco_close,
};

 

static int __init start_function(void){
	mutex_init(&naco_mutex_lock);
	my_major = register_chrdev(0,"dev_naco", &fops);
	if(my_major < 0){
		printk(KERN_INFO "failed to register device");
		return -1;
}
	printk(KERN_INFO "\ndevice created with (%d)major", my_major);
	return 0;

}

static void __exit stop_function(void){
	printk(KERN_ALERT "\ngettint the hell out of here");
	unregister_chrdev(my_major,"dev_naco");
	mutex_destroy(&naco_mutex_lock);
}
module_init(start_function);
module_exit(stop_function);
MODULE_AUTHOR("naco ses");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Basically a simple linux driver");





