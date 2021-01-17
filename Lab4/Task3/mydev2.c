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
static int __init mydev2_init(void);
static void __exit mydev2_exit(void);
static ssize_t mydev2_read(struct file *fp, char __user *buf,
                         size_t len, loff_t *pos);
static ssize_t mydev2_write(struct file *fp, const char __user *buf,
                           size_t len, loff_t *pos);
loff_t mydev2_llseek(struct file *filp, loff_t offset, int whence);

//驱动可用函数
struct file_operations mydev2_fops = {
    read: mydev2_read,      //读数据
    write: mydev2_write,    //写数据
    llseek: mydev2_llseek   //文件指针移动
};

const int mydev2_major = 62;    //设备号
const char *mydev2_name = "hhy's driver-2";
const loff_t mydev2_max_size = 64;  //驱动程序缓冲区最大值
char *mydev2_buf;   //驱动程序缓冲区
loff_t mydev2_size = 0;     //当前缓冲区数据大小

//安装驱动程序
static int __init mydev2_init(void) {
    int ret;
    //注册设备
    ret = register_chrdev(mydev2_major, mydev2_name, &mydev2_fops);
    if(ret < 0) {
        printk(KERN_EMERG"mydev2: cannot obtain major number %d\n", 
        mydev2_major);
        return ret;
    }
    //动态分配驱动程序缓冲区
    mydev2_buf = kmalloc(mydev2_max_size, GFP_KERNEL);
    if(!mydev2_buf) {
        ret = -ENOMEM;
        return ret;
    }
    //初始化缓冲区全0
    memset(mydev2_buf, 0, mydev2_max_size);
    printk("<1>mydev2: Inserting mydev2\n");
    return 0;
}

//删除驱动程序
static void __exit mydev2_exit(void) {
    //取消注册设备
    unregister_chrdev(mydev2_major, mydev2_name);
    //释放缓冲区内存
    if(mydev2_buf) kfree(mydev2_buf);
    printk("<1>mydev2: Removing mydev2\n");
}

//读数据：读取字符
static ssize_t mydev2_read(struct file *fp, char __user *buf,
                         size_t len, loff_t *pos) {
    //计算设备中数据还能读取的大小
    int left = mydev2_size - *pos;
    //若文件指针已经指到数据末尾则不能读取，返回
    if(left == 0) return 0;
    //若读取数据超过可读取上限，则更新读取字节数
    else if(len > left) len = left;
    //从设备缓冲区复制数据到用户缓冲区
    if(copy_to_user(buf, mydev2_buf+(*pos), len))
        return -EFAULT;
    printk("<1>mydev2: Read %ldB, left %ldB\n", len, left-len);
    //文件指针后移读取字节数个单位
    *pos += len;
    //返回实际读取字节数
    return len;
}

//写数据： 写入字符
static ssize_t mydev2_write(struct file *fp, const char __user *buf,
                           size_t len, loff_t *pos) {
    //计算设备缓冲区还能写入的数据大小
    int left = mydev2_max_size - *pos;
    //若文件指针已经指到缓冲区末尾则不能写入，返回
    if(left == 0) return 0;
    //若写入数据超过可写入上限，则更新写入字节数
    else if(len > left) len = left;
    //从用户缓冲区复制数据到设备缓冲区
    if(copy_from_user(mydev2_buf+(*pos), buf, len))
        return -EFAULT;
    printk("<1>mydev2: Write %ldB, left %ldB\n", len, left-len);
    //文件指针后移写入字节数个单位
    *pos += len;
    //更新当前设备缓冲区中数据字节数
    if(*pos > mydev2_size) mydev2_size = *pos;
    //返回实际写入字节数
    return len;
}

//文件指针移动
loff_t mydev2_llseek(struct file *fp, loff_t offset, int whence) {
    //新文件指针位置
    loff_t newpos = 0;
    //判断起点
    switch (whence) {
    case SEEK_SET:  //文件头
        newpos = offset;
        break;
    case SEEK_CUR:  //文件指针当前位置
        newpos = fp->f_pos + offset;
        break;
    case SEEK_END:  //文件尾
        newpos = mydev2_size + offset;
        break;
    default:
        return -EINVAL;
    }
    //若文件指针位置小于0或大于数据长度则报错
    if(newpos < 0 || newpos >= mydev2_size) return -EINVAL;
    //重置文件指针位置并返回
    fp->f_pos = newpos;
    printk("<1>mydev2: f_pos set to %lld\n", newpos);
    return newpos;
}

//注册函数
module_init(mydev2_init);
module_exit(mydev2_exit);