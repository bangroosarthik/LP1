#include<bits/stdc++.h>
using namespace std;
class Job{
    int P_id;
    int AT;
    int BT;
    int CT;
    int TAT;
    int WT;
    public:
    Job(){
        P_id=0;
        AT=0;
        BT=0;
        CT=0;
        TAT=0;
        WT=0;
    }

    Job(int P_id,int AT,int BT){
        this->P_id=P_id;
        this->AT=AT;
        this->BT=BT;
        CT=0;
        TAT=0;
        WT=0;
    }

    void operator=(const Job &j1){
        this->P_id=j1.P_id;
        this->AT=j1.AT;
        this->BT=j1.BT;
        CT=0;
        TAT=0;
        WT=0;
    }

    friend class Scheduling;
};

class Scheduling{
    int n;
    Job *processes;
    vector<Job>readyqueue;
    public:
    Scheduling(){
        n=0;
        processes=NULL;
    }
    Scheduling(int n){
        this->n=n;
        processes=new Job[n];
    }

    void input(){
        int a,b;
        for(int i=0;i<n;i++){
            cout<<"Enter the arrival and burst time of P"<<(i+1)<<" process: "<<endl;
            cin>>a>>b;
            Job j1(i+1,a,b);
            processes[i]=j1;
        }
    }

    void display(){
        cout<<"P_id\t"<<"AT\t"<<"BT\t"<<"CT\t"<<"TAT\t"<<"WT"<<endl;
        for(int i=0;i<n;i++){
            cout<<processes[i].P_id<<"\t"<<processes[i].AT<<"\t"<<processes[i].BT<<"\t"<<processes[i].CT<<"\t"<<processes[i].TAT<<"\t"<<processes[i].WT<<endl;
        }
        float sumTat,sumWt,avgTat,avgWt=0;
        for(int i=0;i<n;i++){
            sumTat+=processes[i].TAT;
            sumWt+=processes[i].WT;
        }

        avgTat=sumTat/n;
        avgWt=sumWt/n;

        cout<<"Average TAT is: "<<avgTat<<endl;
        cout<<"Average Waiting Time: "<<avgWt<<endl;
    }

    void FCFS(){
        int currTime=0;
        int TotalTime=0;
        int completedProcess=0;

        //sort
        sort(processes,processes+n,[](const Job &j1,const Job &j2){
            return j1.AT<j2.AT;
        });

        for(int i=0;i<n;i++){
            TotalTime+=processes[i].BT;
        }

        while(currTime<=TotalTime){
            if(processes[completedProcess].AT<=currTime && completedProcess<n){
                readyqueue.push_back(processes[completedProcess]);
                ++completedProcess;
            }
            if(readyqueue.size()>0){
                readyqueue[0].BT--;
                if(readyqueue[0].BT==0 ){
                    for(int i=0;i<n;i++){
                        if(readyqueue[0].P_id==processes[i].P_id){
                            processes[i].CT=currTime+1;
                            processes[i].TAT=processes[i].CT-processes[i].AT;
                            processes[i].WT=processes[i].TAT-processes[i].BT;
                        }
                    }
                    readyqueue.erase(readyqueue.begin());
                }
            }
            currTime+=1;
        }
        this->display();
    }

    void SJF(){
        int currTime=0;
        int TotalTime=0;
        int completedProcess=0;

        //sort
        sort(processes,processes+n,[](const Job &j1,const Job &j2){
            return j1.AT<j2.AT;
        });

        for(int i=0;i<n;i++){
            TotalTime+=processes[i].BT;
        }

        while(currTime<=TotalTime){
            if(processes[completedProcess].AT<=currTime && completedProcess<n){
                readyqueue.push_back(processes[completedProcess]);
                ++completedProcess;
            }

            sort(readyqueue.begin(),readyqueue.end(),[](const Job &j1,const Job &j2){
                return j1.BT<j2.BT;
            });

            if(readyqueue.size()>0){
                readyqueue[0].BT--;
                if(readyqueue[0].BT==0 ){
                    for(int i=0;i<n;i++){
                        if(readyqueue[0].P_id==processes[i].P_id){
                            processes[i].CT=currTime+1;
                            processes[i].TAT=processes[i].CT-processes[i].AT;
                            processes[i].WT=processes[i].TAT-processes[i].BT;
                        }
                    }
                    readyqueue.erase(readyqueue.begin());
                }
            }
            currTime+=1;
        }
        this->display();
    }

    void priority(){
        int currTime=0;
        int TotalTime=0;
        int completedProcess=0;
        int priority[n];

        


        for(int i=0;i<n;i++){
            cout<<"Enter priority of P"<<(i+1)<<" process"<<endl;
            cin>>priority[i];
        }

        //sort priority array and then processes array 
        for(int i=0;i<n-1;i++){
            for(int j=0;j<n-i-1;j++){
                if(priority[j]>priority[j+1]){
                    swap(priority[j],priority[j+1]);
                    swap(priority[j],priority[j+1]);
                }
            }
        }

        

        for(int i=0;i<n;i++){
            TotalTime+=processes[i].BT;
        }

        while(currTime<=TotalTime){
            if(completedProcess<n){
                readyqueue.push_back(processes[completedProcess]);
                ++completedProcess;
            }

            

            if(readyqueue.size()>0){
                readyqueue[0].BT--;
                if(readyqueue[0].BT==0 ){
                    for(int i=0;i<n;i++){
                        if(readyqueue[0].P_id==processes[i].P_id){
                            processes[i].CT=currTime+1;
                            processes[i].TAT=(processes[i].CT-processes[i].AT)>0?processes[i].CT-processes[i].AT:0;
                            processes[i].WT=(processes[i].TAT-processes[i].BT)>0?processes[i].TAT-processes[i].WT:0;
                        }
                    }
                    readyqueue.erase(readyqueue.begin());
                }
            }
            currTime+=1;
        }
        this->display();
    }

    void rr(){
        int currTime=0;
        int TotalTime=0;
        int completedProcess=0;
        Job AddedJob;
        AddedJob.BT=0;
        int addedburst=0;
        
        int timequanta;
        cout<<"Enter time quanta"<<endl;
        cin>>timequanta;

        sort(processes,processes+n,[](const Job &j1,const Job &j2){
            return j1.AT<j2.AT;
        });

        for(int i=0;i<n;i++){
            TotalTime+=processes[i].BT;
        }

        while(currTime<TotalTime){
            while(completedProcess<n && processes[completedProcess].AT<=currTime){
                readyqueue.push_back(processes[completedProcess]);
                ++completedProcess;
            }
            

            if(AddedJob.BT>0){
                readyqueue.push_back(AddedJob);
            }
            
            if(readyqueue.size()>0){
                AddedJob=readyqueue[0];
                readyqueue.erase(readyqueue.begin());
                if(AddedJob.BT>timequanta){
                    AddedJob.BT-=timequanta;
                    addedburst=timequanta;
                }
                else{
                    addedburst=AddedJob.BT;
                    AddedJob.BT=0;
                }

                if(AddedJob.BT==0){
                    for(int i=0;i<n;i++){
                        if(AddedJob.P_id==processes[i].P_id){
                            processes[i].CT=currTime+addedburst;
                            processes[i].TAT=processes[i].CT-processes[i].AT;
                            processes[i].WT=processes[i].TAT-processes[i].BT;
                        }   
                    }
                }
            }
            currTime=currTime+addedburst;
        }
        this->display();
    }
};
int main(){
    int n;
    cout<<"Enter the number of processes: "<<endl;
    cin>>n;
    Scheduling s1(n);
    s1.input();
    while(true){
        cout<<"MENU--------------------"<<endl;
        cout<<"Enter 1 for FCFS"<<endl;
        cout<<"Enter 2 for SJF"<<endl;
        cout<<"Enter 3 for Priority"<<endl;
        cout<<"Enter 4 for Round robin"<<endl;
        int n;
        cout<<"Enter option"<<endl;
        cin>>n;
        if(n==1){
            s1.FCFS();
        }
        else if(n==2){
            s1.SJF();
        }
        else if(n==3)
        {
            s1.priority();

        }
        else if(n==4){
            s1.rr();
        }
    }
    return 0;
}