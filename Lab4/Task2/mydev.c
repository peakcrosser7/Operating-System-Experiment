#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <linux/uaccess.h> /* copy_from/to_user */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("HHY");
MODULE_DESCRIPTION("A Simple Linux Driver in OS Lab4-2");

//函数声明
static int __init mydev_init(void);
static void __exit mydev_exit(void);
static ssize_t mydev_read(struct file *fp, char __user *buf,
                         size_t len, loff_t *pos);
static ssize_t mydev_write(struct file *fp, const char __user *buf,
                           size_t len, loff_t *pos);

//驱动可用函数
struct file_operations mydev_fops = {
    read: mydev_read,   //读数据
    write: mydev_write  //写数据
};

const int mydev_major = 61;     //主设备号
const char *mydev_name = "hhy's driver";    //设备名
const int mydev_size = 2 * sizeof(int);     //驱动程序缓冲区大小
const int int_size = sizeof(int);   //int类型大小
char *mydev_buf;    //驱动程序缓冲区

//安装驱动
static int __init mydev_init(void) {
    int ret;
    //注册设备
    ret = register_chrdev(mydev_major, mydev_name, &mydev_fops);
    if(ret < 0) {
        printk(KERN_EMERG"mydev: cannot obtain major number %d\n", 
        mydev_major);
        return ret;
    }
    //动态分配设备缓冲区
    mydev_buf = kmalloc(mydev_size, GFP_KERNEL);
    if(!mydev_buf) {
        ret = -ENOMEM;
        return ret;
    }
    //初始化缓冲区全0
    memset(mydev_buf, 0, mydev_size);
    printk("<1>mydev: Inserting mydev\n");
    return 0;
}

//删除驱动
static void __exit mydev_exit(void) {
    //取消注册设备
    unregister_chrdev(mydev_major, mydev_name);
    //释放设备缓冲区
    if(mydev_buf) kfree(mydev_buf);
    printk("<1>mydev: Removing mydev\n");
}

//读取数据：设备中2个数据中的最大值
static ssize_t mydev_read(struct file *fp, char __user *buf,
                         size_t len, loff_t *pos) {
    //比较设备中2个数据的大小，并设置偏移量
    int a = (int)mydev_buf[0], b = (int)mydev_buf[int_size];
    int offset = (a >= b) ? 0 : int_size;
    //将数据从内核态复制到用户态缓冲区
    if(copy_to_user(buf, mydev_buf + offset, int_size))
        return -EFAULT;
    printk("<1>mydev: Read %d\n", (int)mydev_buf[offset]);
    //返回实际读取数据字节数
    return int_size;
}

//写入数据：向设备中写入最多2个整数
static ssize_t mydev_write(struct file *fp, const char __user *buf,
                           size_t len, loff_t *pos) {
    //最多向设备中写入2个整数大小
    if(len > mydev_size) len = mydev_size;
    //将数据从用户缓冲区复制到设备缓冲区
    if(copy_from_user(mydev_buf, buf, len)) return -EFAULT;
    printk("<1>mydev: write %d, %d\n", (int)mydev_buf[0], (int)mydev_buf[int_size]);
    //返回实际写入数据字节数
    return len;
}

//注册函数
module_init(mydev_init);
module_exit(mydev_exit);