#include<bits/stdc++.h>
using namespace std;
int framesize;
int checkpagefault(int page,int frames[]){
    for(int i=0;i<framesize;i++){
        if(frames[i]==page){
            return 0;
        }
    }
    return 1;
}

void printFrames(int frames[],int size,char hm){
    for(int i=0;i<size;i++){
        cout<<frames[i]<<" ";
    }
    cout<<hm<<endl;
}

int fifo(int seqarr[],int seqsize){
    int size=0;
    int pagefaults=0;
    int index=0;
    int frames[framesize];
    for(int i=0;i<seqsize;i++){
        if(checkpagefault(seqarr[i],frames)){
            if(size<framesize){
                pagefaults++;
                frames[size]=seqarr[i];
                size++;
                printFrames(frames,size,'M');
            }
            else{
                pagefaults++;
                frames[index]=seqarr[i];
                index=(index+1)%size;
                printFrames(frames,size,'M');
            }
        }
        else{
            printFrames(frames,size,'H');
        }
    }
    return pagefaults;
}

int getleastRecentlyused(int seqarr[],int start,int frames[],int seqsize){
    int index;
    int least=INT_MAX;
    for(int i=0;i<framesize;i++){
        for(int j=start;j>=0;j--){
            if(seqarr[j]==frames[i]){
                if(least>j){
                    least=j;
                    index=i;
                }
                break;
            }
        }
    }
    return index;
}
int LRU(int seqarr[],int seqsize){
    int size=0;
    int pagefaults=0;
    int index=0;
    int frames[framesize];
    for(int i=0;i<seqsize;i++){
        if(checkpagefault(seqarr[i],frames)){
            if(size<framesize){
                pagefaults++;
                frames[size]=seqarr[i];
                size++;
                printFrames(frames,size,'M');
            }
            else{
                index=getleastRecentlyused(seqarr,i-1,frames,seqsize);
                pagefaults++;
                frames[index]=seqarr[i];
                printFrames(frames,size,'M');
            }
        }
        else{
            printFrames(frames,size,'H');
        }
    }
    return pagefaults;
}

int getFarthest(int seqarr[],int start,int frames[],int seqsize){
    int far=-1;
    int index;
    for(int i=0;i<framesize;i++){
        int nooccurence=0;
        for(int j=start;j<seqsize;j++){
            if(seqarr[j]==frames[i]){
                if(far<j){
                    index=i;
                    far=j;
                }
                nooccurence=1;
                break;
            }
        }
        if(nooccurence==0){
            return i;
        }
    }
    return index;
}
int optimal(int seqarr[],int seqsize){
    int frames[framesize];
    int size=0;
    int index=0;
    int pagefaults=0;
    for(int i=0;i<seqsize;i++){
        if(checkpagefault(seqarr[i],frames)){
            if(size<framesize){
                pagefaults++;
                frames[size]=seqarr[i];
                size++;
                printFrames(frames,size,'M');
            }
            else{
                index=getFarthest(seqarr,i+1,frames,seqsize);
                frames[index]=seqarr[i];
                pagefaults++;
                printFrames(frames,size,'M');
            }
        }
        else{
            printFrames(frames,size,'H');
        }
    }
    return pagefaults;
}
int main(){
    int seqsize;
    cout<<"Enter the sequence size: "<<endl;
    cin>>seqsize;
    int seqarr[seqsize];
    for(int i=0;i<seqsize;i++){
        cout<<"Enter 1st seq:"<<" ";
        cin>>seqarr[i];
        cout<<endl;
    }
    cout<<"Enter frame size: "<<endl;
    cin>>framesize;

    int ans;

    // ans=fifo(seqarr,seqsize);
    // cout<<"Number of page faults are: "<<ans<<endl;

    // int anss=LRU(seqarr,seqsize);
    // cout<<"Number of page faults using LRU are: "<<anss<<endl;

    int ansss=optimal(seqarr,seqsize);
    cout<<"Number of page faults using LRU are: "<<ansss<<endl;
    return 0;
}