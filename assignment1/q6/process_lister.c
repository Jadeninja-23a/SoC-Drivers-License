#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

MODULE_AUTHOR("Videep Reddy Jalapally");
MODULE_DESCRIPTION("Prints Hello Linux, my love >3");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init my_first_module_init(void)
{
	struct task_struct *hey;
	printk(KERN_INFO "Hello Linux, I am gonna print you processes");


	for_each_process(hey){
		char* state_str;
		char buffer1[32], buffer2[32], buffer3[32];
		switch(hey->state){
			case TASK_RUNNING: state_str = "RUNNING";break;
			case TASK_INTERRUPTIBLE: state_str = "SLEEPING";break;
			case TASK_UNINTERRUPTIBLE: state_str = "UNINTERRUPTIBLE";break;
			case __TASK_STOPPED: state_str = "STOPPED";break;
			case __TASK_TRACED: state_str = "TRACED";break;
			case EXIT_ZOMBIE: state_str = "ZOMBIE";break;
			case EXIT_DEAD: state_str = "DEAD";break;
			default: state_str = "OTHER";break;
		}
		sprintf(buffer1, "PID:%d", hey->pid);
		sprintf(buffer2, "STATE:%s", state_str);
	    	sprintf(buffer3, "NAME:%s", hey->comm);
		printk(KERN_INFO "%-30s %-30s %s\n",buffer1, buffer2, buffer3);
	}

	return 0;
}

static void  __exit my_first_module_exit(void)
{
	printk("Exiting The Module");
}

module_init(my_first_module_init);
module_exit(my_first_module_exit);
