#include<linux/vmalloc.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#include<linux/fs.h>
#include<linux/proc_fs.h>
#include<linux/cdev.h>
#include<linux/kdev_t.h>


#define SIZE 1024
dev_t div = 0;
char *kernel_buffer;
int write_index = 0;
int read_index = 0;
struct cdev dev;
static int __init opecn(void);
static void __exit stop_function(void);
static int naco_open(struct inode *inode, struct file *file);
static int naco_close(struct inode *inode, struct file *file);
static ssize_t naco_read(struct file *file, size_t len, const char __user *buf, loff_t *off);
static ssize_t naco_write(struct file *file, size_t len, const char __user *buf, loff_t *off);

static int proc_open(struct inode *inode, struct file *file);
static int proc_close(struct inode *inode, struct file *file);
static ssize_t procw(struct file *filp, const char __user *buf, unsigned long len, void *data);
static int procr(char *page, char **start, int *eof, void *data,loff_t off);

static int naco_open(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "opening device for operations......\n");
	
	return 0;
}

static int naco_close(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "\ndevice closing.......");
	return 0;
}

static ssize_t naco_read(struct file *file, size_t len, const char __user *buf, loff_t *off){
	
	int error_count;
	printk(KERN_INFO "reading from the file..........");
	error_count = copy_to_user(buf,kernel_buffer,len);
	if(error_count == 0){
		return len;
}
else{
	return -EFAULT;
}

}

static ssize_t naco_write(struct file *file, size_t len, const char __user *buf, loff_t *off){
	
	printk(KERN_INFO "writing to the file");
	copy_from_user(kernel_buffer,buf,len);
	
	return len;
}

static int proc_close(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "device closing in proc fs.......");
	return 0;
}

static int procr(char *page, char **start, int *eof, void *data,loff_t off){
	int len;
	if(off > 0){
		*eof = 1;
		return 0;
	}
if(read_index >= write_index){
	read_index = 0;
	len = sprintf(page, "%s\n",&kernel_buffer[read_index]);
	read_index += len;
}
return len; 
}

static ssize_t procw(struct file *filp, const char __user *buf, unsigned long len, void *data){
	int capacity;
	capacity = (SIZE -write_index)+1;
	if(len > capacity){
		printk(KERN_INFO "ran out of space to write");
		return -1;
	}
	if(copy_from_user(&kernel_buffer[write_index], buf, len)){
		printk(KERN_INFO "data transmitted successfully");
	}
	write_index += len;
	kernel_buffer[write_index-1] = 0;
	return len; 
}	

struct file_operations tops = {
	.owner   = THIS_MODULE,
	.read    = naco_read,
	.write   = naco_write,
	.open    = naco_open,
	.release = naco_close,
};

struct file_operations procx = {
	.read    = procr,
	.write   = procw,
	.open    = proc_open,
	.release = proc_close,
};

static int proc_open(struct inode *inode, struct file *file){
	
	printk(KERN_INFO "opening device for operations in proc mode......\n");
	
	return 0;
}

static int __init opecn(void){
	
	if((alloc_chrdev_region(&div,0,1,"dev_naco")) < 0){
		printk(KERN_ALERT "unable to allocate numbers");
		return -1;
	}
	
	printk(KERN_INFO "\ndevice created with (%d)major && (%d)minor", MAJOR(div),MINOR(div));
	if((kernel_buffer = (char*)vmalloc(SIZE)) < 0 ){
		printk(KERN_INFO "no space allocated");
		return -1;
}
if((proc_create("dev_naco",0666,NULL,&procx)) < 0){
	printk(KERN_ALERT "no proc entry has been created");
	return -1; 
}
cdev_init(&dev, &tops);
dev.owner = THIS_MODULE;
dev.ops   = &tops;
if((cdev_add(&dev,div,1)) < 0 ){
	printk(KERN_INFO "could not create structure");
	return -1;
}
	return 0;

}

static void __exit stop_function(void){
	printk(KERN_ALERT "module exited with a 0 status.....");
	unregister_chrdev_region(div,1);
	remove_proc_entry("dev_naco",NULL);
	
}
module_init(opecn);
module_exit(stop_function);
MODULE_AUTHOR("naco ses");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("implementing a proc entry file system into the kernel");
MODULE_VERSION("2.0.14");



