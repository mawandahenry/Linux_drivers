#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/moduleparam.h>
#include<asm/uaccess.h>
#include<linux/types.h>


#define USER "Mawanda henry <mawanda111@gmail.com>"
#define DESR "a simple driver to print hello world"
#define SUPPORT "AMD, intel, SPARC, ARM"
#define DEV_NAME "nacoz_driver"
#define ARRAY_SIZE 1024


char kernel_buffer[ARRAY_SIZE];    //my kernel side buffer. it receives data from the device hardware
char user_buffer[ARRAY_SIZE];      // oriented user space buffer where data read from the kernel buffer is picked



ssize_t naco_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
	printk(KERN_INFO "reading started with offset %i", (int)*offset);
	if(*offset >= kernel_buffer)
		return 0;
	copy_to_user(user_buffer,kernel_buffer,length);
	return length;
}
ssize_t naco_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
	printk(KERN_INFO "writimg initiated at offset %i",(int)*offset);
	if(*offset >= user_buffer)
		return 0;
	copy_from_user(kernel_buffer,user_buffer,length);
		return length;
}
int naco_open(struct inode *pinode, struct file *pfile) {
	printk(KERN_INFO "inside function %s\n", __FUNCTION__);
	return 0;
}
int naco_release(struct inode *pinode, struct file *pfile) {
	printk(KERN_ALERT "closing operations");
	return 0;
}

struct file_operations operations_done={
	.owner = THIS_MODULE,
	.open = naco_open,
	.write = naco_write,
	.read = naco_read,
	.release = naco_release
};


int __init initial(void){
	
		
	register_chrdev(112,"mawanda", &operations_done);
	return 0;
}

void __exit signout(void){
	printk(KERN_INFO "goodbye cruel world");
	unregister_chrdev(112,"mawanda");
}

module_init(initial);
module_exit(signout);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(USER);
MODULE_DESCRIPTION(DESR);
MODULE_SUPPORTED_DEVICE(SUPPORT);


