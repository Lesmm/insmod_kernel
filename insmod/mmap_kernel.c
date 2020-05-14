#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");
int dev_major = 256;
int dev_minor = 0;
char* shmem;

#define SHM_SIZE 1 //1 PAGE
struct page*shm_page;
int symboler_open(struct inode*, struct file*);
int symboler_release(struct inode*,struct file*);
ssize_t symboler_read(struct file*,char *,size_t, loff_t *);
ssize_t symboler_write(struct file*,const char*,size_t, loff_t *);
int symboler_mmap (struct file*, struct vm_area_struct *);
long symboler_ioctl(struct file*,unsigned int, unsigned long);

struct file_operations symboler_fops ={
     owner:THIS_MODULE,
     open: symboler_open,
     release:symboler_release,
     read:  symboler_read,
     write:symboler_write,
     unlocked_ioctl:symboler_ioctl,
     mmap:  symboler_mmap,
};


struct symboler_dev{
	int     sym_var;
	struct   cdev    cdev;
};


struct symboler_dev     *symboler_dev;



int symboler_open(struct inode*inode, struct file*filp)
{
 printk("%s()is called.\n", __func__);
 return 0;
 }

int symboler_release(struct inode*inode, struct file*filp)
{
 printk("%s()is called.\n", __func__);
 return 0;
 }

ssize_t symboler_read(struct file*filp, char *buf, size_t len, loff_t *off)
{
 printk("%s()is called.\n", __func__);
 
 return 0;
 }

ssize_t symboler_write(struct file*filp, const char*buf, size_t len, loff_t *off)
{
 printk("%s()is called.\n", __func__);
 
 return 0;
 }

long symboler_ioctl(struct file*filp,unsigned int intVal, unsigned long longVal)
{
 printk("%s()is called.\n", __func__);
 
 return 0l;
 }

 void symboler_vma_open(struct vm_area_struct *vma)
 {
 printk("%s()is called.\n", __func__);
 }

void symboler_vma_close(struct vm_area_struct *vma)
{
 printk("%s()is called.\n", __func__);
 }

static struct vm_operations_struct symboler_remap_vm_ops = {
 .open =symboler_vma_open,
 .close =symboler_vma_close,
 };


 int symboler_mmap (struct file*filp, struct vm_area_struct *vma)
{
 printk("%s()is called.\n", __func__);
 if(remap_pfn_range(vma, vma->vm_start, page_to_pfn(shm_page),vma->vm_end -vma->vm_start, vma->vm_page_prot))
         return -EAGAIN;

 vma->vm_ops =&symboler_remap_vm_ops;
 symboler_vma_open(vma);

 return 0;
 }




 int symboler_init(void)
 {
         int ret,err;

         dev_t devno =MKDEV(dev_major,dev_minor);

         ret= register_chrdev_region(devno,1, "symboler");

         if(ret < 0)
         {
                 printk("symboler register failure.\n");
                 return ret;
         }
         else
                 printk("symboler register successfully.\n");


         symboler_dev = kmalloc(sizeof(struct symboler_dev), GFP_KERNEL);

         if(!symboler_dev)
         {
                 ret = -ENOMEM;
                 printk("create device failed.\n");
         }
         else
         {
                 symboler_dev->sym_var =0;
                 cdev_init(&symboler_dev->cdev, &symboler_fops);
                 symboler_dev->cdev.owner= THIS_MODULE;
                 err = cdev_add(&symboler_dev->cdev,devno, 1);

                 if(err <0)
                         printk("Add device failure\n");
         }

         shm_page = alloc_pages(___GFP_WRITE, SHM_SIZE);
         shmem= page_address(shm_page);
         strcpy(shmem, "hello,mmap\n");

         return ret;
 }

 static void symboler_exit(void)
{
	int status;
	status = unregister_chrdev(dev_major, "symboler");
	if (status < 0 ) {
		printk("failed to unregister_chrdev char device\n");
	}
    printk("symboler_exit\n");
}

module_init(symboler_init);
module_exit(symboler_exit);