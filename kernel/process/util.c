#include "kernel.h"

void
create_kthread(void (*function)(void)){
	pcb_pool[sum].pid=sum;
	pcb_pool[sum].wait=0;
	pcb_pool[sum].s=&semph[sum];
	pcb_pool[sum].s->value=0;
	//printk("%d",pcb_pool[sum].s);
	//panic("create");
	if(sum==0){
		pcb_pool[sum].next=NULL;//&pcb_pool[sum];
		pcb_pool[sum].pro=NULL;//NULL replace &pcb_pool[sum];
	}
	else if(sum==1){
		pcb_pool[sum].next=&pcb_pool[sum];
		pcb_pool[sum].pro=&pcb_pool[sum];
		pcb_pool[0].next=&pcb_pool[sum];
	}
	else{
		pcb_pool[sum].next=&pcb_pool[1];
		pcb_pool[1].pro=&pcb_pool[sum];
		pcb_pool[sum-1].next=&pcb_pool[sum];
		pcb_pool[sum].pro=&pcb_pool[sum-1];
	}
	int *f = (int *)(&pcb_pool[sum])+KSTACK_SIZE;
	f--;
	*f=0x202;//EFLAGS
	f--;
	*f=0x8;//CS
	f--;
	*f=(int)function;//EIP
	f--;
	*f=1000;//irq
	f=f-8;
	pcb_pool[sum].esp=f;
	sum++;
}

/*void A(void){while(1) printk("a");}
void B(void){while(1) printk("b");}
void C(void){while(1) printk("c");}*/

void produce(){
	printk("%s%d","in:",in);
	console_printc('\n');
}

void consumer(){
	printk("%s%d","out",out);
	console_printc('\n');
}
void producer_i(){
	while(1){
		int i,temp=0;
		for(i=0;i<3000000;i++)
			temp=(231+i)%432;
		printk("%d",temp);
		P(empty);
		P(mutex);
		
		in=(in+1)%k;
		produce();
		V(mutex);
		V(full);
	}
}

void consumer_j(){
	while(1){
		int i,temp=0;
		for(i=0;i<3000000;i++)
			temp=(231+i)%432;
		printk("%d",temp);
		P(full);
		P(mutex);
		out=(out+1)%k;
		consumer();
		V(mutex);
		V(empty);
		
	}
}

void A(void){
	
	while(1){
		P(mutex);
		int i,temp=0;
		for (i=0;i<3000000;i++)
		temp=(temp%234+1)%423;
		printk("%d ",temp);
		printk("%s%d","A:",count);
		count++;
		
		console_printc('\n');
		V(mutex);
	}
}
void B(void){
	//num=num+1;
	
	while(1){
		P(mutex);
		int i,temp=0;
		for (i=0;i<3000000;i++)
		temp=(temp%234+1)%423;
		printk("%d ",temp);
		printk("%s%d","B:",count);
		count++;
		
		console_printc('\n');
		V(mutex);
		//enable_interrupt();
	}
	
}
void C(void){
	
	while(1){
		printk("C");
		console_printc('\n');
		//panic("pause");
	}
	
}

void 
Pi1(void){
	static double pi=0.0;
	static double i=1.0;
	static int state=1;
	while(1){
		if(state==1){
			pi+=4.0/i;
			state=0;
		}
		else{
			pi-=4.0/i;
			state=1;
		}
		i=i+2.0;
		printk("Pi1:%f",pi);
		console_printc('\n');
	}
}
void 
Pi2(void){
	static double pi=0.0;
	static double i=1.0;
	static int state=1;
	while(1){
		if(state==1){
			pi+=4.0/i;
			state=0;
		}
		else{
			pi-=4.0/i;
			state=1;
		}
		i=i+2.0;
		printk("Pi2:%f",pi);
		console_printc('\n');
	}
}

void
M(void){
	struct Message m1,m2;
	m1.int_msg.p1=56;
	m1.int_msg.p2=0;
	m1.int_msg.p3=99;
	while(1){
		int temp=0,i;
		for(i=0;i<30000000;i++)
			temp=(i+231)%432;
		printk("%d",temp);
		receive(2,&m2);
		printk("%d%s%d",1," receive message from",2);
		console_printc('\n');
		printk("%s%d%s%d%s%d","p1=",m2.int_msg.p1,"p2=",m2.int_msg.p2,"p3=",m2.int_msg.p3);
		console_printc('\n');
		send(2,&m1);
		printk("%d%s%d",1,"send message to",2);
		console_printc('\n');
	}
}

void
N(void){
	struct Message m1,m2;
	m1.int_msg.p1=23;
	m1.int_msg.p2=36;
	m1.int_msg.p3=78;
	send(1,&m1);
	while(1){
		int temp=0,i;
		for(i=0;i<30000000;i++)
			temp=(i+231)%432;
		printk("%d",temp);
		receive(1,&m2);
		printk("%d%s%d",2,"receive message from",1);
		console_printc('\n');
		printk("%s%d%s%d%s%d","p1=",m2.int_msg.p1,"p2=",m2.int_msg.p2,"p3=",m2.int_msg.p3);
		console_printc('\n');
		send(1,&m1);
		printk("%d%s%d",2,"send message to",1);
		console_printc('\n');
	}
}

void
test_alarm(void){

	struct Message m1,m2;
	m1.int_msg.p1=2;
	m1.int_msg.p2=1;
	m1.type=TIMER_SET_ALRM;
	while(1){
		send(TIMER,&m1);
		receive(ANY,&m2);
		if(m2.type == TIMER_ALRM_FIRE)
			printk("ABC");
	}
}

void
init_proc() {
	//mutex=semph[SIZE];
	mutex->value=1;
	//empty->value=10;
	//full->value=0;
	//k=3;
	//in=0;
	//out=0;
	sum=0;
	count=0;
	lock_deepth=0;
	m01->int_msg.p1=31;
	m01->int_msg.p2=34;
	m01->int_msg.p3=45;
	TIMER=1;
	//head2=tail2=NULL;
	//create_kthread(Pi1);
	//create_kthread(Pi2);
	create_kthread(C);
	//create_kthread(timer_driver_thread);
	//create_kthread(test_alarm);
	//create_kthread(M);
	//create_kthread(N);
	
	
	
	//create_kthread(producer_i);
	//create_kthread(consumer_j);
	create_kthread(A);
	create_kthread(B);
	head1=&pcb_pool[0];
	tail1=&pcb_pool[sum-1];
}


/*
void 
deque(struct PCB *list){
	list=head2;
	if(head2->next==head2){
		head2=tail2=NULL;
	}
	else{
		list->pro->next=list->next;
		list->next->pro=list->pro;
		head2=list->next;
	}
	if(head1==NULL){
		head1=list;
		list->pro=NULL;
		list->next=NULL;
	}
	else{
		list->next=tail1->next;
		list->pro=tail1;
		tail1->next->pro=list;
		tail1->next=list;
	}
	tail1=list;
	
}
*/
void 
W(struct PCB* list){
	struct PCB *temp;
	temp=current_pcb;
	if(temp->next==temp){
	head1=NULL;
	tail1=NULL;
	}
	else{
		temp->pro->next=temp->next;
		temp->next->pro=temp->pro;
	}
	if(list==NULL){
		list=temp;
		temp->next=temp;
		temp->pro=temp;
		//printk("head2 is not NULL now");
	}
	else{
		list->pro->next=temp;
		temp->pro=list->pro;
		list->pro=temp;
		temp->next=list;
		//printk("add a node to wait");
	}
	//panic("guaqi");
	asm("int $0x80");
};

void
R(struct PCB* list){	
	struct PCB *temp;
	temp=list;
	
	if(temp->next==temp){
		list=NULL;
	}
	else{
		temp->pro->next=temp->next;
		temp->next->pro=temp->pro;
		list=list->next;
	}
	if(head1==NULL){
		head1=temp;
		temp->next=temp;
		temp->pro=temp;
	}
	else{
		temp->next=tail1->next;
		temp->pro=tail1;
		tail1->next->pro=temp;
		tail1->next=temp;
	}
	tail1=temp;
	
};

void P(struct semaphore *s){
	disable_interrupt();
	s->value--;
	//printk("%d",s->value);
	//panic("fuck");
	if(s->value<0)
		W(s->list);
	enable_interrupt();
};

void V(struct semaphore *s){
	disable_interrupt();
	s->value++;
	if(s->value<=0)
		R(s->list);
	enable_interrupt();
};

void 
send(int pid,struct Message *m){
	//panic
	P(mutex);
	m->src=current_pcb->pid;
	m->dest=pid;
	if(pcb_pool[pid].wait==1){//There is a process wait for message.
		printk("send message directly\n");
		pcb_pool[pid].wait=0;
		*pcb_pool[pid].msg_addr=*m;
		V(pcb_pool[pid].s);
	}
	else{
		int i;
		for(i=0;i<MSG_SIZE;i++){
			if(msg_pool[i].used==0){
				printk("enque the message\n");
				panic("the message queue");
				msg_pool[i].used=1;
				msg_pool[i]=*m;
				break;
			}
		}
		if(i>=MSG_SIZE)
			panic("the message queue is full");
	}
	V(mutex);
	//panic("pause");
}

void
receive(int pid,struct Message *m){
	P(mutex);
	printk("%d",current_pcb->s->value);
	panic("receive");
	current_pcb->wait=1;
	if(pid==-1){
		int i;
		for(i=0;i<MSG_SIZE;i++){
			if(msg_pool[i].used==1){
				msg_pool[i].used=0;
				current_pcb->wait=0;
				*m=msg_pool[i];
				break;
			}
		}
		if(i>=MSG_SIZE){
			//printk("%d",current_pcb->s->value);
			current_pcb->msg_addr=m;
			V(mutex);
			
			P(current_pcb->s);// need something
			P(mutex);
		}
		printk("%d",current_pcb->pid);
		
	}
	else{
		int i;
		for(i=0;i<MSG_SIZE;i++){
			if(msg_pool[i].used==1&&msg_pool[i].src==pid){
				printk("receive message from queue\n");
				m=&msg_pool[i];
				current_pcb->wait=0;
				msg_pool[i].used=0;
				//current_pcb->msg_addr=m;
				break;
			}
		}
		if(i>=MSG_SIZE){
			printk("wait for message\n");
			current_pcb->msg_addr=m;
			P(current_pcb->s);
		}
	}
	V(mutex);
	//panic("ffc\n");
	
}

void lock(void){
	disable_interrupt();
	lock_deepth--;
}

void unlock(void){
	lock_deepth++;
	if(lock_deepth==0)
		enable_interrupt();
}





