/* 

First column in machine code indicates location counter of that 
particular line, second column is for machine code of particular 
statement (Ex. READ is a imperative statement(IS) Machine code for 
IS is 09), third column deals with machine code for registers if registers
are used for particular statement(Ex. 01 is machine code for register A),
fourth column deals with address of literal inside the literal table/symbol 
inside the symbol table/ 

*/

#include<bits/stdc++.h>
using namespace std;

int main(){

    ifstream sin;
    sin.open("symbol_table.txt");

    ifstream lin;
    lin.open("literal_table.txt");

    ifstream fin;
    fin.open("intermidate_code.txt");

    ofstream fout;
    fout.open("machine.txt");

    string mytext,word;

    vector<pair<string,int>> symtable;

    while(getline(sin,mytext)){

        stringstream st(mytext);
        st>>word;

        string label=word;
        st>>word;
        symtable.push_back({label,stoi(word)});

    }

    sin.close();

    vector<pair<string,int>> litable;

    while(getline(lin,mytext)){

        stringstream st(mytext);
        st>>word;

        string label=word;
        st>>word;
        litable.push_back({label,stoi(word)});

    }

    lin.close();    

    int lc=-1;

    while(getline(fin,mytext)){

        stringstream st(mytext);
        st>>word;

        string cls,memonic;
        cls=word.substr(1,2);
        memonic=word.substr(4,2);

        if(cls=="AD"){   //This indicates that current line of the input file is an assembler directive.

            fout<<"  No Machine Code  "<<endl;    //For declarative statement no machine code

            if(memonic=="01"){

                st>>word;
                word=word.substr(3,word.length()-4);
                lc=stoi(word);

            }

            else if(memonic=="03"){

                st>>word;
                word=word.substr(4,1);
                int ind=stoi(word);
                ind--;
                lc=symtable[ind].second;  //it assigns the value found in the symtable at the index indicated by ind to the variable lc.

            }

        }

        else if(cls=="IS"){   //IS= instruction statement

            fout<<lc<<" "<<memonic<<" ";
            lc++;

            if(memonic=="00"){

                fout<<"0 000"<<endl;
                continue;
            }

            st>>word;

            if(word[1]!='S' && word[1]!='L'){  //It checks if the second character of word is not 'S' and not 'L'

                word=word.substr(1,1);
                fout<<word<<" ";
                st>>word;

            }

            else{

                fout<<"0 ";

            }

            string temp=word.substr(3,2);
            int num=stoi(temp);
            num--;

            if(word[1]=='S'){

                fout<<symtable[num].second<<endl;

            }

            else if(word[1]=='L'){

                fout<<litable[num].second<<endl;
                
            }
        }

        else if(cls=="DL"){  //DL=declaration statement

            fout<<lc<<" ";
            lc++;

            if(memonic=="01"){

                fout<<"00 0 ";
                st>>word;
                word=word.substr(3,1);
                fout<<"00"<<word<<endl;

            }

            else if(memonic=="02"){

                fout<<"No Machine Code"<<endl;

            }
        }

    }

    fin.close();
    fout.close();

    cout<<"Program Executed";

    return 0;
}