#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");  //模块许可证声明
MODULE_AUTHOR("HHY");   //模块作者（可选）
MODULE_DESCRIPTION("A Simple Linux Module in OS Lab4-1");   //模块描述

int test;
module_param(test, int, 0644);  //模块参数绑定

//安装模块调用函数
static int __init myModuleInit(void) {
    printk("Hello World! This's hhy's module.\n");
    printk("test = %d\n", test);
    return 0;
}

//删除模块调用函数
static void __exit myModuleExit(void) {
    printk("Goodbye World! This's hhy's module.\n");
}

//模块函数注册
module_init(myModuleInit);
module_exit(myModuleExit);