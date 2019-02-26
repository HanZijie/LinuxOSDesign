#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/uaccess.h>

/*处理版本问题CONFIG_MODVERSIONS */ 
#if CONFIG_MODVERSIONS==1 
#define MODVERSIONS 
#include "linux/version.h" 
#endif 

#define DEVICE_NUM 0

// 打开关闭 读写入口
static int mchar_open(struct inode *inode, struct file *filp);
static int mchar_release(struct inode *inode, struct file *filp);
static ssize_t mchar_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos);
static ssize_t mchar_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos);

static struct file_operations mchar_fops = {
    .read = mchar_read,
    .write = mchar_write,
    .open = mchar_open,
    .release = mchar_release,
};

// 必须要申明为静态变量和静态函数
static int device_num = 0;             //用来保存创建成功后的设备号
static char buffer[1024] = "Hello World!\n"; //数据缓冲区
static int open_process = 0;                //打开设备的进程数，用于内核的互斥

//打开设备
static int mchar_open(struct inode *inode, struct file *filp)
{
    // MAJOR和MINOR返回主次设备号
    printk("mchar: Main device number is %d  \n Slave device number is %d\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
    printk("mchar: Now device has opend! \n");
    // 未被占用
    if (open_process == 0)
    {
        open_process++; //标志占用
        try_module_get(THIS_MODULE);
        return 0;
    }else{
        printk(KERN_ALERT "Another process open the char device.\n");
        return -1;
    }
}

//释放设备
static int mchar_release(struct inode *inode, struct file *filp){
    open_process--;
    printk("mchar:Device is released\n");
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t mchar_read(struct file *file, char __user *buf, size_t count, loff_t *f_pos){
    if(copy_to_user(buf,buffer,sizeof(buffer))){
        return -1;
    }
    return sizeof(buffer);
}

static ssize_t mchar_write(struct file *file, const char __user *buf, size_t count, loff_t *f_pos){
    if(copy_from_user(buffer,buf,sizeof(buffer))){
        return -1;
    }
    return sizeof(buffer);
}

static int mchar_init(void){
    int result;
    printk(KERN_ALERT "mchar:Begin to init Char Device!\n"); //注册设备

    result = register_chrdev(DEVICE_NUM, "mchar_hzj", &mchar_fops);//向系统的字符登记表登记一个字符设备

    if(result < 0){
        printk(KERN_WARNING "mchar: register failure\n");
        return -1;
    }
    else{
        printk("mchar:register success\n");
        device_num = result;
        return 0;
    }

}

static void __exit mchar_exit(void)
{
    printk(KERN_ALERT "Unloading...\n");
    unregister_chrdev(device_num, "hello"); //字符设备卸载
    printk("mchar: unregister success!\n");
}

//模块宏定义
module_init(mchar_init);
module_exit(mchar_exit);
// 模块许可协议
MODULE_LICENSE("GPL");
