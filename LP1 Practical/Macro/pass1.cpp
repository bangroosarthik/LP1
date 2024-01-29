#include<bits/stdc++.h>
using namespace std;

int main(){

    ifstream fin;
    fin.open("source.txt");

    ofstream fout;
    fout.open("output.txt");

    ofstream mntout;
    mntout.open("mnt.txt");

    ofstream mdtout;
    mdtout.open("mdt.txt");

    ofstream pntout;
    pntout.open("pnt.txt"); 

    ofstream kpdout;
    kpdout.open("kpd.txt");  

    string line,word;
    int p=1;
    bool started=false;
    bool macrostart=false;

    vector<array<string,5>> mnttable;
    vector<pair<string,string>> kpdtable;
    map<string,int> pnttable;

    while(getline(fin,line)){

        stringstream st(line);
        st>>word;

            if(word=="MACRO"){

                macrostart=true;
                continue;
            }

        if(started){    //If 'started' is true, it means the program is processing lines of the source code.

            if(word!="END"){

                fout<<word<<" ";
                while(st>>word){

                    fout<<word<<" ";
                }

                fout<<endl;
            }

            continue;
        }

        if(macrostart){

            array<string,5> arr;

            mnttable.push_back(arr);
            int i=mnttable.size()-1;
            
            mnttable[i][0]=word;
            mnttable[i][3]=to_string(p);  //The line number is stored in the fourth column (index 3) of the 'mnttable'.

            int pp=0;  //Count of formal parameters
            int kp=0;  //Count of keyword parameters

            pntout<<"----------PNTAB FOR "<<word<<" MACRO------\n";
            while(st>>word){

                if(word[word.length()-1]==','){

                    word=word.substr(1,word.length()-2);

                }

                else{

                    word=word.substr(1,word.length()-1);
                }

                int eqlindex=-1;  //If 'eqlindex' is -1, it means the word is a formal parameter.

                for(int j=0;j<word.length();j++){  //Loop for assigning index where keyword parameter occurs

                    if(word[j]=='='){

                        eqlindex=j;
                        break;
                    }

                }

                if(eqlindex==-1){

                    pp++;
                    pnttable[word]=pnttable.size()+1;  //Calculating size required for the parameter 
                    pntout<<pnttable[word]<<". "<<word<<endl;

                }

                else{

                    kp++;
                    string beforeequal,afterequal;

                    beforeequal=word.substr(0,eqlindex);
                    afterequal=word.substr(eqlindex+1);

                    kpdtable.push_back({beforeequal,afterequal});
                    pnttable[beforeequal]=pnttable.size()+1;
                    pntout<<pnttable[beforeequal]<<". "<<beforeequal<<endl;

                }
            }

            mnttable[i][1]=to_string(pp);  //Adds count of formal parameters
            mnttable[i][2]=to_string(kp);  //Adds count of keyword parameters

            if(kp!=0){

                mnttable[i][4]=to_string(kpdtable.size()-kp+1);  //Adds starting index of the keyword parameters
            }

            else{

                mnttable[i][4]='-';  //If there is no keyword parameter it sets the starting index as '-'
            }

            macrostart=false;
            continue;

        }

        if(word=="MEND"){

            mdtout<<"MEND"<<endl;
            pnttable.clear();

        }

        else if(word=="START"){
            started=true;
        }
        //--------------------------------------------------------------------------------------------------

        else{

            mdtout<<word<<" ";  

            while(st>>word){

                if(word[0]=='&'){  //If the word begins with '&', it means it is a formal parameter

                    word=word.substr(1);
                    if(word[word.size()-1]==','){

                        word=word.substr(0,word.size()-1);
                    }
                    mdtout<<"(P,"<<pnttable[word]<<"), "; //pnttable[word]=index of parameter

                }

                else{   //If word does not begin with '&', it means it is a regular word

                    if(word[word.size()-1]==','){

                        word=word.substr(0,word.size()-1);
                    }

                    mdtout<<word<<", ";
                }
            }
            mdtout<<endl;
        }

        p++;
    }

    fin.close();

    for(int i=0;i<mnttable.size();i++){

        for(int j=0;j<5;j++){

            mntout<<mnttable[i][j]<<" ";

        }
        mntout<<endl;
    }

    for(auto i:kpdtable){

        kpdout<<i.first<<" "<<i.second<<endl;
    }

    cout<<"Program Executed";

    return 0;
}