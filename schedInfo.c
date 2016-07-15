#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/printk.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uidgid.h>

void print_processes(struct task_struct *task);
void print_siblings(int *pid);

/**
 * The following defines and initializes a list_head object named files_list
 */

static int processID;
static int processSPolicy;
static int processPrio;

module_param(processID, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(processID, "processID :))");

module_param(processSPolicy, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(processSPolicy, "processSPolicy :))");

module_param(processPrio, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(processPrio, "processPrio :))");

int myfileinfo_init(void)
{
  //print_siblings(1121);
print_processes(&init_task);
struct sched_param param = { .sched_priority = 0 };
param.sched_priority=(100-(processPrio+1));
struct task_struct *lebron;
// SCHED_OTHER SCHED_FIFO SCHED_RR SCHED_BATCH SCHED_IDLE

lebron = pid_task(find_vpid(processID), PIDTYPE_PID);

char description[25];

if(processSPolicy == 1){
  strcpy(description,"(SCHED_FIFO)");
  int a =sched_setscheduler(lebron, SCHED_FIFO, &param);
  if(a == 0)
  printk("DONE\n");
}
if(processSPolicy == 2){
  strcpy(description,"(SCHED_RR)");
  int a = sched_setscheduler(lebron, SCHED_RR, &param);
  if(a == 0)
  printk("DONE\n");
}

printk("Process after the change pid: %d,comm: %s prio: %d static: %d parent is: %s user id is: %u schedule: %d policy: %u %s time slice: %u \n",
lebron->pid,lebron->comm,lebron->prio,lebron->static_prio,lebron->parent->comm, lebron->cred->uid,lebron->normal_prio,
lebron->policy,description,lebron->rt.time_slice);
print_siblings(processID);

/*
	struct task_struct *p;

	p = list_entry(list,struct task_struct,sibling);

	pr_info("yepp: %s %d \n",p->comm,p->pid);
*/

       return 0;
}


void print_processes(struct task_struct *task) {
struct list_head *list;
struct task_struct *another_task;
char description[25];
// task->real_cred->uid

if(task->policy == 0)
strcpy(description,"(SCHED_NORMAL)");

if(task->policy == 1)
  strcpy(description,"(SCHED_FIFO)");

if(task->policy == 2)
strcpy(description,"(SCHED_RR)");

if(task->policy == 3)
strcpy(description,"(SCHED_BATCH)");


printk("pid: %d,comm: %s prio: %d static: %d parent is: %s user id is: %u schedule: %d policy: %u %s time slice: %u \n",
task->pid,task->comm,task->prio,task->static_prio,task->parent->comm, task->cred->uid,task->normal_prio,
task->policy,description,task->rt.time_slice);

list_for_each(list, &task->children) {
  another_task = list_entry(list, struct task_struct, sibling);
  print_processes(another_task);
}}

void print_siblings(int *pid) {
  struct list_head *list;
  struct task_struct *lebron,*yep,*parent;

  lebron = pid_task(find_vpid(pid), PIDTYPE_PID);
  parent = lebron->parent;

  list_for_each(list, &parent->children) {
  yep = list_entry(list, struct task_struct, sibling);
  if(yep->pid != lebron->pid)
    printk("This is the SIBLING my pid is : %d comm is : %s I AM THE SIBLING OF pid: %d\n",yep->pid,yep->comm,lebron->pid);
  }
}

void myfileinfo_exit(void) {

}

module_init( myfileinfo_init);
module_exit( myfileinfo_exit);

MODULE_LICENSE( "GPL");
MODULE_DESCRIPTION( "Exercise for COMP304");
MODULE_AUTHOR("Abdullah Emin Ak");
