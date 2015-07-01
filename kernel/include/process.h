#ifndef __PROCESS_H__
#define __PROCESS_H__

#define KSTACK_SIZE  1000
#define SIZE 1000
#define MSG_SIZE 20
#define ANY -1




int count;
int sum;
int in;
int out;
int k;
int lock_deepth;
pid_t TIMER;

struct PCB {
	//'0' is stand wait,'1'is stand for ready
	void *esp;
	struct PCB *next;
	struct PCB *pro;
	int pid;
	int wait;//wait for message
	struct Message *msg_addr;
	struct semaphore *s;
	int time_elapsed;
	uint_32 kstack[KSTACK_SIZE];
}pcb_pool[SIZE],*p;

struct semaphore {
	int value;
	struct PCB *list;
}*mutex,semph[SIZE+1];

struct Message{
	int src,dest;
	int type;
	int used;
	union{
		struct {
			int p1,p2,p3;
		}int_msg;
		struct{
			int pid;
			void *buf;
			int offset,length;
		}dev_io;
	};
}msg_pool[MSG_SIZE];

struct semaphore *empty,*full;
struct Message *m01,*m02,*m12;

// P,V operation
void P(struct semaphore *s);
void V(struct semaphore *s);

extern struct PCB *current_pcb;
extern boolean need_sched;

struct PCB *head1,*head2,*tail1,*tail2;

void send(int ,struct Message*);
void receive(int ,struct Message*);

void A();
void B();
void C();
void init_proc();

void create_kthread(void (*)(void));

void M();
void N();

void test_alarm();

void lock(void);
void unlock(void);

void timer_driver_thread();

void W(struct PCB*);
void R(struct PCB*);

#endif





















