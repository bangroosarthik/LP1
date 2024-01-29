#include<bits/stdc++.h>
using namespace std;
map<string,string>ISTab={
    {"STOP","(IS,00)"},
    {"ADD","(IS,01)"},
    {"SUB","(IS,02)"},
    {"MUL","(IS,03)"},
    {"MOVER","(IS,04)"},
    {"MOVEM","(IS,05)"},
    {"COMP","(IS,06)"},
    {"BC","(IS,07)"},
    {"DIV","(IS,08)"},
    {"READ","(IS,09)"},
    {"PRINT","(IS,10)"}
};

map<string,string>RegTab={
    {"AREG","01"},
    {"BREG","02"},
    {"CREG","03"},
    {"DREG","04"}
};

map<string,string>CCTab={
    {"LT","01"},
    {"LE","02"},
    {"EQ","03"},
    {"GT","04"},
    {"GE","05"},
    {"ANY","06"}
};

class Pass1{
    vector<vector<string>>SymTab;
    vector<vector<string>>LitTab;
    vector<int>PoolTab;
    vector<int>LcTab;
    string ic;
    int lc;
    int litIndex;
    int poolIndex;

    public:
    Pass1(){
        lc=0;
        litIndex=0;
        poolIndex=0;
    }

    void updSymTab(vector<string>pair){
        for(auto entry:SymTab){
            if(entry[0]==pair[0]){
                entry[1]=pair[1];
                return;
            }
        }
        SymTab.push_back(pair);
    }

    int LcAdressTab(const string &word){
        for(const auto entry:SymTab){
            if(entry[0]==word){
                return stoi(entry[1]);
            }
        }
        return -1;
    }


    
    int IndexTab(const string &word){
        for(size_t pos=0;pos<SymTab.size();pos++){
            if(SymTab[pos][0]==word){
                return static_cast<int>(pos);
            }
        }
        return -1;
    }


    void process(){
        ifstream File("Testcase.txt");
        if(!File.is_open()){
            cerr<<"Error opening File"<<endl;
            return;
        }

        string line;
        while(getline(File,line)){
            string word;
            vector<string>words;
            stringstream ss(line);
            while(getline(ss,word,'\t')){
                words.push_back(word);
            }

            if(!words.empty()){
                vector<string>pair={words[0],to_string(lc)};
                updSymTab(pair);
            }

            if(words[1]=="START"){
                try{
                    lc=stoi(words[2]);
                    LcTab.push_back(lc);
                    ic+="(AD,01)\t(C,"+to_string(lc)+")\n";
                }
                catch(const std::invalid_argument& e){
                    cerr<<e.what();
                }
                
            }

            if(words[1]=="LTORG"){
                for(int i=poolIndex;i<litIndex;i++){
                    LitTab[i][1]=to_string(lc);
                    lc++;
                    LcTab.push_back(lc);
                }
                ic+="(AD,05)\n";
                ic+='\n';
                PoolTab.push_back(poolIndex);
                poolIndex=litIndex;
            }

            if(words[1]=="END"){
                for(int i=poolIndex;i<litIndex;i++){
                    LitTab[i][1]=to_string(lc);
                    lc++;
                    LcTab.push_back(lc);
                }
                ic+="(AD,02)\n";
                ic+='\n';
                PoolTab.push_back(poolIndex);
                poolIndex=litIndex;
            }

            if(words[1]=="DC"){
                ic+="(DL,01)\t(C,"+words[2]+")\n";
                lc++;
                LcTab.push_back(lc);
            }

            if(words[1]=="DS"){
                try{
                    ic+="(DL,02)\t(C,"+words[2]+")\n";
                    lc+=stoi(words[2]);
                }
                catch(const std::invalid_argument &e){
                    cerr<<e.what();
                }
                LcTab.push_back(lc);
            }

            if(words[1]=="ORIGIN"){
                
                try{
                    string word=words[2].substr(0,words[2].find('+'));
                    int loc=LcAdressTab(word);
                    int add=stoi(words[2].substr(words[2].find('+')+1));
                    ic+="(AD,03)\t(S,"+to_string(IndexTab(word))+")+"+to_string(add)+"\n";
                    lc=loc+add;
                    LcTab.push_back(lc);
                }
                catch(const std::invalid_argument &e){
                    cerr<<e.what();
                }

                
            }

            if(words[1]=="EQU"){
                try{
                    string word=words[2].substr(0,words[2].find('+'));
                    int loc=LcAdressTab(word);
                    int add=stoi(words[2].substr(words[2].find('+')+1));
                    ic+="(S,"+to_string(IndexTab(words[0]))+")\t"+"(AD,03)\t(S,"+to_string(IndexTab(word))+")+"+to_string(add)+"\n";
                    updSymTab({words[0],to_string(loc+add)});
                }
                catch(const std::invalid_argument &e){
                    cerr<<e.what();
                }
            }

            if(ISTab.find(words[1])!=ISTab.end()){
                ic+=ISTab[words[1]]+" ";
                for(size_t i=2;i<words.size();i++){
                    if(RegTab.find(words[i])!=RegTab.end()){
                        ic+=RegTab[words[i]]+" ";
                    }
                    else if(CCTab.find(words[i])!=CCTab.end()){
                        ic+=CCTab[words[i]]+" ";
                    }
                    else if(words[i].find('=')==string::npos){
                        int symloc=LcAdressTab(words[i]);
                        if(symloc==-1){
                            updSymTab({words[i],to_string(lc)});
                        }
                        int index=IndexTab(words[i]);
                        ic+="(S,"+to_string(index+1)+")\t";
                    }
                    else{
                        LitTab.push_back({words[i],"-1"});
                        litIndex++;
                        ic+="(L,"+to_string(litIndex)+")\t";
                    }
                }
                lc++;
                LcTab.push_back(lc);
                ic+='\n';
            }
            
        }
        File.close();
        ofstream ICFile("ICoutput.txt");
        ICFile<<ic;
        ICFile.close();

        ofstream SymFile("SymOutput.txt");
        for(auto &entry:SymTab){
            for(auto &it:entry ){
                SymFile<<it<<'\t';
            }
            SymFile<<"\n";
        }

        SymFile.close();

        ofstream LitFile("LitOutput.txt");
        for(auto &entry:LitTab){
            for(auto &it:entry){
                LitFile<<it<<"\t";
            }
            LitFile<<"\n";
        }

        LitFile.close();

        ofstream PoolFile("PoolOutput.txt");
        for(auto &entry:PoolTab){
            PoolFile<<entry<<"\n";
        }
        PoolFile.close();
    }

};
int main(){
    Pass1 obj;
    obj.process();
    return 0;
}