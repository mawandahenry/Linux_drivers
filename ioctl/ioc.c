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
#include<linux/interrupt.h>
#include<asm/io.h>
#include<linux/mutex.h>


#define NACO_RD _IOR('a','a',int32_t*)
#define NACO_WRT _IOW('a','b',int32_t*)
#define IRQ_NO 11


static DEFINE_MUTEX(naco_mutex_lock);
int major;
dev_t devn = 0;
struct cdev my_dev;
int32_t value = 0;
static int __init initializer(void);
static void __exit shut_down(void);
static int my_open(struct inode *inode, struct file *file);
static int my_close(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, size_t len, const char __user *buf, loff_t *off);
static ssize_t my_write(struct file *file, size_t len, const char __user *buf, loff_t *off);
static long my_iotcl(struct file *file, unsigned int cmd, unsigned long arg);



static irqreturn_t irq_handler(int irq,void *dev_id) {
  printk(KERN_ALERT "Interrupt Occurred");
  return IRQ_HANDLED;
}

static int my_open(struct inode *inode, struct file *file){
	mutex_lock(&naco_mutex_lock);
	printk(KERN_INFO "file is open");
	mutex_unlock(&naco_mutex_lock);
	return 0;
}


static int my_close(struct inode *inode, struct file *file){
	printk(KERN_INFO "file is closed");
	return 0;
}

static ssize_t my_write(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO "writing to file");
	return 0;
}

static ssize_t my_read(struct file *file, size_t len, const char __user *buf, loff_t *off){
	printk(KERN_INFO " reading data");
	return 0;
}


static long my_iotcl(struct file *file, unsigned int cmd, unsigned long arg){
	printk(KERN_INFO "executing ioctl operations");
	switch(cmd){
		case NACO_RD:
			copy_to_user((int32_t *)arg, &value, sizeof(value));
			printk(KERN_ALERT "reading from the device file");
			asm("int $0x3B"); 
		break;
		case NACO_WRT:
			copy_from_user(&value, (int32_t *)arg, sizeof(value));
			printk(KERN_ALERT "writing to the device file");
		break;
		default:
			printk(KERN_ALERT "unknown command used");
		}
		return 0;
}


static int __init initializer(void){
	mutex_init(&naco_mutex_lock);
	if((alloc_chrdev_region(&devn,0,1,"mawanda")) < 0){
		printk(KERN_INFO "failed to register the device");
		return -1;
	}
	printk(KERN_INFO "created file with major number %d && minor %d\n", MAJOR(devn),MINOR(devn));
	if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "mawanda", (void *)(irq_handler))) {
            printk(KERN_INFO "my_device: cannot register IRQ ");
                    return -1;
        }
	return 0;
}



static void __exit shut_down(void){
	printk(KERN_ALERT "officially shuting down......\n");
	unregister_chrdev_region(devn,"mawanda");
	free_irq(IRQ_NO, (void *)(irq_handler));
	mutex_destroy(&naco_mutex_lock);
}
module_init(initializer);
module_exit(shut_down);

MODULE_AUTHOR("NACO SES <mawanda111@gmail.com>");
MODULE_VERSION("1.0.0");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("a simple character driver to implement major ioctl commands");
	










