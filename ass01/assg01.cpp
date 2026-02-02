#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

class Node {
public:
    int id;
    int promptCount;
    vector<int> dependencies;
    Node(int nodeId, int count, const vector<int>& deps) {
        id = nodeId;
        promptCount = count;
        dependencies = deps;
    }
};

// Some global variables
map<int, vector<int>> adj;   
map<int, int> indeg;   
map<int, int> idprmtmap;

vector<int> nodes;
vector<int> currpath;
vector<vector<int>> allperms;

bool visited[1000] = {false}; 
int totalperms = 0;

void findallseq(int totalNodes) {
    bool finished = true;
    
    for (int id : nodes) {
        if (indeg[id] == 0 && !visited[id]) {
            visited[id] = true;
            currpath.push_back(id);
            
            for (int neighbor : adj[id]) {
                indeg[neighbor]--;
            }
            
            findallseq(totalNodes);
            
            for (int neighbor : adj[id]) {
                indeg[neighbor]++;
            }
            
            currpath.pop_back();
            visited[id] = false;
            finished = false;
        }
    }
    
    // Only print when we actually have a complete path
    if (finished && currpath.size() == totalNodes) {
        ++totalperms;
        allperms.push_back(currpath);
    }
}

void init_evryd_prmtcnt(int n,int k, map<int,int> &evryd_prmtcnts){
    for(int i=1;i<=n;i++)    evryd_prmtcnts[i]=k;
    //  cout<<"init_evryd_prmtcnt \n";
    return;
}

int main(){

    int n,k,m;

    cout<<"Number of students in a group\n";
    cin>>n;

    cout<<"Number of prompts allowed per day\n";
    cin>>k;

    cout<<"A <id> <prompt-count> <dependencies> <terminating-symbol>\n";

    vector<Node> graph;
    string line;
    cin.ignore(); 

    while (getline(cin, line)) {
        if (line == "END" || line.empty()) break; 
        
        stringstream ss(line);
        char type;
        ss >> type;
        
        if (type == 'A') {
            int id, count;
            ss >> id >> count; 
            
            vector<int> deps;
            int dep;
            while (ss >> dep && dep != 0) {
                deps.push_back(dep);
            }
            
            graph.push_back(Node(id, count, deps));
            idprmtmap[id] = count;
        }
    }

    /*
    for(auto &i:graph){
        cout<<i.id<<' '<<i.promptCount;
        cout<<"\n";
        for(auto &j:i.dependencies) cout<<j<<' ';
        cout<<"\n";
    }
    */

    for (const auto& node : graph) {
        nodes.push_back(node.id);
        if (indeg.find(node.id) == indeg.end()) {
            indeg[node.id] = 0;
        }
        
        for (int d : node.dependencies) {
            adj[d].push_back(node.id);
            indeg[node.id]++;
        }
    }

    sort(nodes.begin(), nodes.end());

    findallseq(nodes.size());

    cout << "\nNumber of days allowed (m): ";
    cin >> m;

    // cout<<"\nThe total number of sequences are : "<<totalperms<<"\n";
    // cout<<"Perms are \n";
    
    // for(auto &vec : allperms) {
    //     for(auto &i : vec) {
    //         cout << i << ' ';
    //     }
    //     cout << endl;
    // }

    // print id to prompt count mapping

    /*
    for(auto &i: idprmtmap){
        cout<<i.first<<' '<<i.second<<"\n";
    }
    */
    

    //  for every possible sequence of the topological sort, we need to see if the minimum days constraint is valid.
    //  also we need to store such sequences in a data structure(vec of vecs) and print the number of such sequences.

    vector<map<int,vector<int>>> validseqs;
    int validseqcnt=0,currdays=0;

    map<int,int> evryd_prmtcnts;
    init_evryd_prmtcnt(n,k, evryd_prmtcnts);

    /*
    for(auto i: evryd_prmtcnts){
        cout<<i.first<<' '<<i.second<<"\n";
    }
    */

    //  cout<<allperms.size()<<"\n";

    // main logic

    for(auto &vec: allperms){
        // find the minimum value that can satisfy the prompt count constraint and reduce it from the student daily prompts
        // untill we cant satisfy and more constaint and then increment the currdays and initialize the evryd_prmtcnts to original k values
        // idprmtmap
        
        bool isvalid=false;
        int mindays=0,mnind=-1;
        map<int,vector<int>> currseqs;

        // cout<<a<<" inside for - \n";
        while(mindays<m){
            // cout<<a<<" inside while - \n";
            // we can't apply binary sort on this as after reducing the prompt count the values will not be in a sorted order
            // find the minimum possible needed from students prompt
            for(auto &i:vec){
                // cout<<a<<" inside for - \n";
                bool found=false;
                int currprmtcnt=idprmtmap[i];
                int mincnt=100000, ind=0;

               for(auto i:evryd_prmtcnts){
                // cout<<a<<" inside 2nd for - \n";
                if(currprmtcnt<=i.second){
                    found=true;
                    if(mincnt>i.second){    mincnt=i.second;    ind=i.first;    }
                }
                } 
                if(found)   idprmtmap[ind]-=mincnt; 
                else{
                    init_evryd_prmtcnt(n,k, evryd_prmtcnts);
                    // reduce this from student prompt
                    ++mindays;
                }

                for(auto &b:evryd_prmtcnts){
                    // cout<<a<<" inside last for - \n";
                    cout<<b.first<<' '<<b.second<<"\n";
                }
            }
            
        }
        if(mindays<m) validseqs.push_back(currseqs);
    }

/*
    for(auto &vec: allperms){
    bool isvalid=false;
    int mindays=0;
    map<int,vector<int>> currseqs;
    map<int,int> remaining_prompts = idprmtmap; // Copy of assignment prompts
    init_evryd_prmtcnt(n,k, evryd_prmtcnts);
    
    cout<<a<<" inside for - \n";
    
    for(auto &i:vec){
        cout<<a<<" inside assignment loop - \n";
        
        int currprmtcnt = remaining_prompts[i];
        
        // Find student with minimum available prompts that can handle this
        int mincnt = 100000, best_student = -1;
        bool found = false;
        
        for(auto &student : evryd_prmtcnts){
            if(currprmtcnt <= student.second){
                found = true;
                if(student.second < mincnt){
                    mincnt = student.second;
                    best_student = student.first;
                }
            }
        }
        
        if(found){
            // Assign to student
            evryd_prmtcnts[best_student] -= currprmtcnt;
            currseqs[mindays].push_back(i); // Track assignment in current day
        }
        else{
            // Need a new day
            ++mindays;
            if(mindays >= m){
                // Exceeds day limit, this sequence is invalid
                break;
            }
            init_evryd_prmtcnt(n,k, evryd_prmtcnts);
            // Now assign to student
            mincnt = 100000;
            best_student = -1;
            for(auto &student : evryd_prmtcnts){
                if(currprmtcnt <= student.second && student.second < mincnt){
                    mincnt = student.second;
                    best_student = student.first;
                }
            }
            if(best_student != -1){
                evryd_prmtcnts[best_student] -= currprmtcnt;
                currseqs[mindays].push_back(i);
            }
        }
    }
    
    // Check if we completed all assignments within m days
    if(mindays < m){
        validseqs.push_back(currseqs);
        ++validseqcnt;
    }
    
    ++a;
    }
*/
    for(auto &i:validseqs){
        for(auto &seqs: i){
            cout<<seqs.first<<' ';
            for(auto &j:seqs.second){   cout<<j<<' ';   }
            cout<<"\n";
        }
        cout<<"\n";
    }

    cout<<"Total possible sequences : "<<validseqs.size()<<"\n";

    return 0;
}