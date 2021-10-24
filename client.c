#include"headers.h"

pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex1=PTHREAD_MUTEX_INITIALIZER;
float uti=0;
pthread_t add_thread,checker,memory;
struct Process{
    int pid;
	float duration;
	float size;
    time_t seconds;
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
	printf("Enter physical memory size:\n");
	scanf("%d",&p);
	printf("Enter reserved memory size:\n");
	scanf("%d",&q);
	printf("Enter process arrival rate:\n");
	scanf("%d",&n);
	printf("Size of the process in multiples of 10 MB:\n");
	scanf("%d",&m);
	printf("Enter duration of process in multiples of 5 seconds:\n");
	scanf("%d",&t);
	printf("Enter total time of execution in multiples of 100 seconds:\n");
	scanf("%d",&T);
	printf("Enter the algorithm to use: \n1 for first fit\n2 for best fit\n3 for next fit respectively\n");
	scanf("%d",&a);
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

int main(){
	time_t present,g_time;
	time(&g_time);
	take_input(p,q,m,n,t,T);
	len=p-q;
	printf("Length of the available space %d MB\n",len);
	arr=(float *)malloc(sizeof(float)*len);
	for(int i=0;i<len;i++){
		arr[i]=0;
	}
	int pid=0;
	int nxt=0;
	while(1){
		time(&present);
		if(present-g_time> T*100){
		printf("Time finished\n");
		 exit(0);
		}	
		srand(time(0));
		float r=(rand()%12) +1;
		r/=10;
		r*=n;
		printf("Process rate %f\n",r);
		sleep(r);
		float s=(rand()%26) +5;
		s/=10;
		s*=m;
		printf("Process size %f\n",s);
		float d=(rand()%56) +5;
		d/=10;
		d*=t;
		d*=5;
		printf("Process duration %f\n",d);
		struct Process *process=(struct Process *) malloc(sizeof (struct Process));
		process->size=s;
		pid++;
		process->duration=d;
		process->pid=pid;
		time(&(process->seconds));
		add_to_queue(process);
		struct Node* temp=head;
		

		if(temp==NULL){
			continue;
		}
		struct Process *p_temp=temp->process;
		float size=p_temp->size * 10;
		float duration=p_temp->duration;
		if(a==1){
			for(int i=0;i<len;i++){
					if(arr[i]==0){
						int t_size=1;
						int j=i;
						while((j<len) && (arr[j]==0) && (t_size<=size)){
							t_size++;
							j++;
						}
						t_size--;
						if(size<=t_size){
							for(int k=i;k<j;k++) arr[k]=duration;
							remove_from_queue();
							time(&present);
							printf("Average turnaround time for process %d is: %ld seconds\n",p_temp->pid,(present-p_temp->seconds));
							break;
						}
						i=j;
				}
			}
		}
		else if(a==2){
			float size=p_temp->size*10;
			int s_l=0,s_r=INT_MAX;
			for(int i=0;i<len;i++){
					int t_size=1;
					int j=i;
					if(arr[i]==0){
						while(arr[j]==0 && j<len){
							t_size++;
							j++;
						}
						t_size--;
					
						if(size<=t_size && t_size<(s_r-s_l)){
							s_r=j;
							s_l=i;
						}
						j=i-1;
					}
			}
			if(s_r!=INT_MAX){
				int t_size=0;
				for(int i=s_l;i<s_r;i++){
					t_size++;
					arr[i]=p_temp->duration;
					if(t_size==size) break;
				}
				remove_from_queue();
				time(&present);
				printf("Average turnaround time for process %d is: %ld seconds\n",p_temp->pid,(present-p_temp->seconds));

			} 

		}
		if(nxt>=len) nxt=0;
		else if(a==3){
			int i=nxt;
			int flag=0;
			for(i;i<len;i++){
				if(arr[i]==0){
					int t_size=1;
					int j=i;
					while((j<len) && (arr[j]==0) && (t_size<=size)){
						t_size++;
						j++;
					}
					t_size--;
					if(size<=t_size){
						for(int k=i;k<j;k++) arr[k]=duration;
						remove_from_queue();
						time(&present);
						flag=1;
						printf("Average turnaround time for process %d is: %ld seconds\n",p_temp->pid,(present-p_temp->seconds));
						nxt=i+1;
						break;
					}
					i=j;
				}
			}
			if(!flag){
				for(int i=0;i<nxt;i++){
					if(arr[i]==0){
					int t_size=1;
					int j=i;
					while((j<len) && (arr[j]==0) && (t_size<=size)){
						t_size++;
						j++;
					}
					t_size--;
					if(size<=t_size){
						for(int k=i;k<j;k++) arr[k]=duration;
						remove_from_queue();
						time(&present);
						flag=1;
						printf("Average turnaround time for process %d is: %ld seconds\n",p_temp->pid,(present-p_temp->seconds));
						nxt=i+1;
						break;
					}
					i=j;
				}
				}

			}
		}
		uti=0;
		for(int i=0;i<len;i++){
			if(arr[i]-r>0){
				arr[i]-=r;
				uti+=1;
			}else{
				arr[i]=0;
			}
		}
		printf("Memory utilised is %f\n",(uti/len)*100);



	}

    return 0;
}


