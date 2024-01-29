#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
using namespace std;
map<string, string> OPTAB = {
    {"STOP", "(IS,00)"},
    {"ADD", "(IS,01)"},
    {"SUB", "(IS,02)"},
    {"MUL", "(IS,03)"},
    {"MOVER", "(IS,04)"},
    {"MOVEM", "(IS,05)"},
    {"COMP", "(IS,06)"},
    {"BC", "(IS,07)"},
    {"DIV", "(IS,08)"},
    {"READ", "(IS,09)"},
    {"PRINT", "(IS,10)"}};

map<string, string> REG = {
    {"AREG", "(1)"},
    {"BREG", "(2)"},
    {"CREG", "(3)"},
    {"DREG", "(4)"}};

map<string, string> CC = {
    {"LT", "(1)"},
    {"LE", "(2)"},
    {"EQ", "(3)"},
    {"GT", "(4)"},
    {"GE", "(5)"},
    {"ANY", "(6)"}};

class AssemblerPass1
{
private:
    vector<vector<string>> symtab;
    vector<vector<string>> littab;
    string ic;
    vector<int> pooltab;
    int lc;
    vector<int> lctab;
    int litIndex;
    int poolIndex;

public:
    AssemblerPass1()
    {
        lc = 0;
        litIndex = 0;
        poolIndex = 0;
    }

    void UpdateSymtab(vector<string> pair)
    {
        for (auto &entry : symtab)
        {
            if (entry[0] == pair[0])
            {
                entry[1] = pair[1];
                return;
            }
        }
        symtab.push_back(pair);
    }

    int LCSymtab(string &word)
    {
        for (const auto &entry : symtab)
        {
            if (entry[0] == word)
            {
                return stoi(entry[1]);
            }
        }
        return -1;
    }

    int PosSymtab(string &word)
    {
        for (size_t pos = 0; pos < symtab.size(); ++pos)
        {
            if (symtab[pos][0] == word)
            {
                return static_cast<int>(pos);
            }
        }
        return -1;
    }

    void process()
    {
        ifstream file("TestCase.txt");
        if (!file.is_open())
        {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string word;
            vector<string> words;
            while (getline(ss, word, '\t'))
            {
                words.push_back(word);
            }

            if (!words.empty())
            {
                vector<string> pair = {words[0], to_string(lc)};
                UpdateSymtab(pair);
            }

            if (words[1] == "START")
            {
                lc = stoi(words[2]);
                lctab.push_back(lc);
                ic += "(AD,01)\t(C," + to_string(lc) + ")\n";
            }

            if (words[1] == "LTORG")
            {
                for (int i = poolIndex; i < litIndex; ++i)
                {
                    littab[i][1] = lc;
                    ++lc;
                    lctab.push_back(lc);

                    string value = littab[i][0];
                    value.erase(remove(value.begin(), value.end(), '='), value.end());
                    ic += "(DL,01)\t(C," + value + ")\n";
                }

                pooltab.push_back(poolIndex);
                poolIndex = litIndex;
            }
            else if (words[1] == "END")
            {
                for (int i = poolIndex; i < litIndex; ++i)
                {
                    littab[i][1] = lc;
                    ++lc;
                    lctab.push_back(lc);

                    string value = littab[i][0];
                    value.erase(remove(value.begin(), value.end(), '='), value.end());
                    ic += "(DL,01)\t(C," + value + ")\n";
                }

                pooltab.push_back(poolIndex);
                poolIndex = litIndex;

                ic += "(AD,02)\n";
            }
            else if (words[1] == "DC")
            {
                ic += "(DL,01)\t(C," + words[2] + ")\n";
                ++lc;
                lctab.push_back(lc);
            }
            else if (words[1] == "DS")
            {
                ic += "(DL,02)\t(C," + words[2] + ")\n";
                lc += stoi(words[2]);
                lctab.push_back(lc);
            }
            else if (words[1] == "ORIGIN")
            {
                string locationWord = words[2].substr(0, words[2].find('+'));
                int location = LCSymtab(locationWord);
                int addition = stoi(words[2].substr(words[2].find('+') + 1));
                ic += "(AD,03)\t(S," + to_string(PosSymtab(locationWord) + 1) + ")+" + to_string(addition) + "\n";
                lc = location + addition;
                lctab.push_back(lc);
            }
            else if (words[1] == "EQU")
            {
                string locationWord = words[2].substr(0, words[2].find('+'));
                int location = LCSymtab(locationWord);
                int addition = stoi(words[2].substr(words[2].find('+') + 1));
                ic += "(AD,04)\t(S," + to_string(PosSymtab(locationWord) + 1) + ")+" + to_string(addition) + "\n";
                UpdateSymtab({words[0], to_string(location + addition)});
            }
            else if (OPTAB.find(words[1]) != OPTAB.end())
            {
                ic += OPTAB[words[1]] + " ";

                for (size_t i = 2; i < words.size(); ++i)
                {
                    if (CC.find(words[i]) != CC.end())
                    {
                        ic += CC[words[i]] + " ";
                    }
                    else if (REG.find(words[i]) != REG.end())
                    {
                        ic += REG[words[i]] + " ";
                    }
                    else if (words[i].find('=') == string::npos)
                    {
                        int symbolLocation = LCSymtab(words[i]);
                        if (symbolLocation == -1)
                        {
                            UpdateSymtab({words[i], to_string(lc)});
                        }

                        int index = PosSymtab(words[i]);
                        ic += "(S," + to_string(index + 1) + ")\t";
                    }
                    else
                    {
                        littab.push_back({words[i], "-1"});
                        ++litIndex;
                        ic += "(L," + to_string(litIndex) + ")\t";
                    }
                }

                ++lc;
                lctab.push_back(lc);
                ic += "\n";
            }
            else
            {
                // continue;
            }
        }
        file.close();

        ofstream ICFile("Pass1_IC.txt");
        ICFile << ic;
        ICFile.close();

        ofstream SYMTABFile("Pass1_SYMTAB.txt");
        for (const auto &entry : symtab)
        {
            for (const auto &val : entry)
            {
                SYMTABFile << val << "\t";
            }
            SYMTABFile << "\n";
        }
        SYMTABFile.close();

        ofstream LITTABFile("Pass1_LITTAB.txt");
        for (const auto &entry : littab)
        {
            for (const auto &val : entry)
            {
                LITTABFile << val << " ";
            }
            LITTABFile << "\n";
        }
        LITTABFile.close();

        ofstream POOLTABFile("Pass1_POOLTAB.txt");
        for (const auto &val : pooltab)
        {
            POOLTABFile << val << "\n";
        }
        POOLTABFile.close();
    }
};

int main()
{
    AssemblerPass1 test;
    test.process();
    return 0;
}
