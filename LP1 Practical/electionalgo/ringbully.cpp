#include<bits/stdc++.h>
#include<unistd.h>
using namespace std;
vector<bool>input(){
    int n;
    cout<<"Enter number of nodes"<<endl;
    cin>>n;
    vector<bool>nodeStates(n,true);
    return nodeStates;
}

void ring(){
    vector<bool>ring=input();

    //making last node as dead
    ring[ring.size()-1]=false;
    cout<<"Node "<<ring.size()<<" is dead node"<<endl;
    usleep(500000);
    int identifier=ring.size()-1;
    while(ring[identifier]==false){
        identifier--;
    }

    cout<<"Node "<<identifier+1<<" is the identifier node"<<endl;
    usleep(500000);

    int client=0;
    int sender=identifier;
    while(client<=identifier){
        if(ring[client]!=false){
            cout<<"Node "<<(sender+1)<<" elects "<<(client+1)<<" node"<<endl;
            usleep(500000);
            cout<<"Acknowledgement OK from node "<<(client+1)<<endl;
            usleep(500000);
            sender=client;
        }
        client++;    
    }

    cout<<"Node "<<(identifier+1)<<" is the new coordinator"<<endl;
    usleep(500000);
    
}


void bully(){
    vector<bool>bull=input();

    bull[bull.size()-1]=false;
    cout<<"Node "<<bull.size()<<" is dead"<<endl;
    usleep(500000);

    int detector=rand()%bull.size();
    while(bull[detector]==false){
        detector=rand()%bull.size();
    }

    cout<<"Node "<<(detector+1)<<" is the detecting node"<<endl;
    usleep(500000);

    for(int i=detector;i<bull.size();i++){
        int ackno=0;
        for(int j=i+1;j<bull.size();j++){
            cout<<"Node "<<(i+1)<<" elects to "<<(j+1)<<endl;
            usleep(500000);
            if(bull[j]==true){
                ackno++;    
                cout<<"Ok acknoledgement by "<<(j+1)<<" node"<<endl;
                usleep(500000);
            }
            else{
                cout<<"Acknowledgement Lost from"<<(j+1)<<" node"<<endl;
                usleep(500000);
            }
        }
        if(ackno==0){
            cout<<"Node "<<(i+1)<<"  is the new coordintor"<<endl;
            usleep(500000);
            break;
        }
    }

}
int main(){
    // input();
    // ring();
    bully();

    return 0;
}