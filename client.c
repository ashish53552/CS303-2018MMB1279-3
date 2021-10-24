#include"headers.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
float uti=0;
pthread_t add_thread,checker,memory;
struct Process{
	float duration;
	float size;
};
struct Node{
	struct Process * process;
	struct Node *next;
};
int p,q,n,m,t,T,a,len;
float * arr;
struct Node *head=NULL;
struct Node *tail=NULL;
struct Node* create_node(struct Process *process){
	struct Node *temp=(struct Node*) malloc(sizeof(struct Node));
	temp->process=process;
	temp->next=NULL;
	return temp;
}

void take_input(){
	printf("Enter physicalmem, reserved, rate,size,duration,totalTime, algo (1:first 2:best 3:next):\n");
	scanf("%d%d%d%d%d%d%d",&p,&q,&n,&m,&t,&T,&a);
	return;
}
void add_to_queue(struct Process *process){
	struct Node *temp=create_node(process);
	if(tail!=NULL){
		tail->next=temp;
	}
	tail=temp;
	if(head==NULL){
		head=temp;
	}
}
struct Node* remove_from_queue(){
	if(head==NULL){
		return head;
	}
	struct Node*temp=head;
	head=head->next;
	return temp;
}
void *summon_process_and_add_in_queue_thread_function(void *arg){
	while(1){
		srand(time(0));
		float r=(rand()%12) +1;
		r/=10;
		r*=n;
		sleep(r);
		float s=(rand()%26) +5;
		s/=10;
		s*=m;
		float d=(rand()%56) +5;
		d/=10;
		d*=m;
		struct Process *process=(struct Process *) malloc(sizeof (struct Process));
		process->size=s;
		process->duration=d;
		pthread_mutex_lock(&mutex);
		add_to_queue(process);
		pthread_mutex_unlock(&mutex);

	}
}
void *check_if_process_finished_function(void *arg){
	while(1){
		sleep(0.50);
		pthread_mutex_lock(&mutex1);
		for(int i=0;i<len;i++){
			if(arr[i]>0) arr[i]-= (float)1/(float)2;
			if(arr[i]>0) uti++;
			else uti--; 
		}
		pthread_mutex_unlock(&mutex1);
		printf("%f\n",uti*100/len);
	}
	return;
}
void *from_queue_add_to_memory_function(void *arg){
	int nxt=0;
	while(1){
		if(head==NULL){	
			continue;
		}
		pthread_mutex_lock(&mutex1);
		struct Process * process=head->process;
		float size=process->size;
		float duration=process->duration;
		if(a==1){// first-fit;
			for(int i=0;i<len;i++){
				if(arr[i]==0){
					int t_size=0;
					int j;
					for(j=i;j<len;++j){
						if(arr[j]==0){
							t_size++;
						}else{
							break;
						}
					}
					if(size<=t_size){
						for(int k=i;k<j;k++) arr[k]=duration;
						pthread_mutex_lock(&mutex);
						remove_from_queue();
						pthread_mutex_unlock(&mutex);
					}
					i=j;
				}
			}
		}
		if(a==2){
			int s_l=0,s_r=0;
			for(int i=0;i<len;i++){
				if(arr[i]==0){
					int t_size=0;
					int j;
					for(j=i;j<len;++j){
						if(arr[j]==0){
							t_size++;
						}else{
							break;
						}
					}
					if(size<=t_size && (t_size<=(s_r-s_l))){
						s_l=i;
						s_r=j;
					}	
					i=j;
				}
			}
			if(size<=(s_r-s_l) ){
				for(int k=s_l;k<s_r;k++) arr[k]=duration;
				pthread_mutex_lock(&mutex);
				remove_from_queue();
				pthread_mutex_unlock(&mutex);

			}
		}
		if(a==3){
			for(nxt;nxt<len;nxt++){
				if(arr[nxt]==0){
					int t_size=0;
					int j;
					for(j=nxt;j<len;++j){
						if(arr[j]==0){
							t_size++;
						}else{
							break;
						}
					}
					if(size<=t_size){
						for(int k=nxt;k<j;k++) arr[k]=duration;
						nxt=j;
						pthread_mutex_lock(&mutex);
						remove_from_queue();
						pthread_mutex_unlock(&mutex);
						break;
					}
					
				}
			}
			if(nxt==len) nxt=0;
		}
		pthread_mutex_unlock(&mutex1);
	}
}
void summon_process_and_add_in_queue(){
	int n=pthread_create(add_thread,NULL,summon_process_and_add_in_queue_thread_function,NULL);
	if(n<0) printf("Error Creating thread\n");
	return;
};

void check_if_process_finished(){
	int n=pthread_create(checker,NULL,check_if_process_finished_function,NULL);
	if(n<0) printf("Error Creating thread\n");
	return;
};
void from_queue_add_to_memory(){
	int n=pthread_create(memory,NULL,from_queue_add_to_memory_function,NULL);
	if(n<0) printf("Error Creating thread\n");
	return;
};

int main(){

	take_input(p,q,m,n,t,T);
	printf("%d%d%d%d%d%d",p,q,m,n,t,T);
	len=p-q;
	arr=(float *)malloc(sizeof(float)*len);
	for(int i=0;i<len;i++){
		arr[i]=0;
	}
	summon_process_and_add_in_queue();

	from_queue_add_to_memory();

	check_if_process_finished();

    return 0;
}


