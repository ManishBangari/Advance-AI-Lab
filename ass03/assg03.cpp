#include <bits/stdc++.h>
using namespace std;

struct Assgn {
    int id;
    int prompts;
    vector<int> deps;
};

struct State {
    vector<int> done;  
    int day;
    int gCost;           
};

int N; 
int totAssgns;

vector<Assgn> tasks;
unordered_map<int,int> idIndex;

int C1, C2; 

long long nodesDFS = 0;
long long nodesDFBB = 0;
long long nodesAstar = 0;

bool caseA = true;
bool sharingNextDay = true;

bool allDone(const vector<int>& done) {
    for(int x : done) if(!x) return false;
    return true;
}

bool depsSatisfied(int idx, const vector<int>& done) {
    for(int d : tasks[idx].deps) {
        if(!done[idIndex[d]]) return false;
    }
    return true;
}

vector<int> avaTasks(const vector<int>& done) {
    vector<int> avail;
    for(int i=0;i<totAssgns;i++) {
        if(!done[i] && depsSatisfied(i, done)) {
            avail.push_back(i);
        }
    }
    return avail;
}

int heurRemDays(const vector<int>& done, int cgpt, int gem) {
    int needCGPT = 0, needGEM = 0;

    for(int i=0;i<totAssgns;i++) {
        if(!done[i]) {
            if(tasks[i].id % 2 == 0)
                needCGPT += tasks[i].prompts;
            else
                needGEM += tasks[i].prompts;
        }
    }

    int d1 = (cgpt==0)?1e9: (needCGPT + cgpt - 1) / cgpt;
    int d2 = (gem==0)?1e9: (needGEM + gem - 1) / gem;

    return max(d1, d2);
}

vector<State> genNexDays(const State& s, int cgptLimit,int gemLimit){

    vector<State> nexstates;
    vector<int> avail = avaTasks(s.done);

    int maxAssign = caseA ? N : totAssgns;

    int n = avail.size();
    int subsets = 1<<n;

    for(int mask=1; mask<subsets; mask++) {

        if(__builtin_popcount(mask) > maxAssign) continue;

        int usedCGPT=0, usedGEM=0;
        vector<int> newDone = s.done;
        bool valid = true;

        for(int i=0;i<n;i++) {
            if(mask&(1<<i)) {
                int idx = avail[i];
                if(tasks[idx].id % 2 == 0)
                    usedCGPT += tasks[idx].prompts;
                else
                    usedGEM += tasks[idx].prompts;
            }
        }

        if(usedCGPT > cgptLimit || usedGEM > gemLimit)
            continue;

        for(int i=0;i<n;i++) {
            if(mask&(1<<i)) {
                newDone[avail[i]] = 1;
            }
        }

        State ns;
        ns.done = newDone;
        ns.day = s.day + 1;
        ns.gCost = s.gCost + 1;

        nexstates.push_back(ns);
    }

    return nexstates;
}

//  DFS

int bestDFS = 1e9;

void dfs(State s, int cgpt, int gem) {
    nodesDFS++;

    if(allDone(s.done)) {
        bestDFS = min(bestDFS, s.day);
        return;
    }

    if(s.day >= bestDFS) return;

    auto nexstates = genNexDays(s, cgpt, gem);

    for(auto &ns : nexstates) {
        dfs(ns, cgpt, gem);
    }
}

// DFBB

int bestDFBB = 1e9;

void dfbb(State s, int cgpt, int gem) {
    nodesDFBB++;

    int h = heurRemDays(s.done, cgpt, gem);
    if(s.day + h >= bestDFBB) return;

    if(allDone(s.done)) {
        bestDFBB = s.day;
        return;
    }

    auto nexstates = genNexDays(s, cgpt, gem);

    for(auto &ns : nexstates)   dfbb(ns, cgpt, gem);
    
}

// A*

struct CompareAstar {
    bool operator()(const State& a, const State& b) const {
        return a.gCost > b.gCost;
    }
};

// logic for A*
int Astar(int cgpt, int gem) {

    priority_queue<State, vector<State>, CompareAstar> pq;

    State start;
    start.done.assign(totAssgns,0);
    start.day = 0;
    start.gCost = 0;

    pq.push(start);

    while(!pq.empty()) {

        State s = pq.top();
        pq.pop();
        nodesAstar++;

        if(allDone(s.done))
            return s.day;

        auto nexstates = genNexDays(s, cgpt, gem);

        for(auto &ns : nexstates) {
            int h = heurRemDays(ns.done, cgpt, gem);
            ns.gCost = ns.day + h;
            pq.push(ns);
        }
    }

    return -1;
}

void parseInput(string filename) {

    ifstream fin(filename);
    string line;

    while(getline(fin,line)) {

        if(line.size()==0 || line[0]=='%') continue;

        stringstream ss(line);
        char ch;
        ss>>ch;

        if(ch=='A') {
            Assgn a;
            ss>>a.id>>a.prompts;

            int dep;
            while(ss>>dep) {
                if(dep==0) break;
                a.deps.push_back(dep);
            }
            tasks.push_back(a);
        }
    }

    totAssgns = tasks.size();

    for(int i=0;i<totAssgns;i++)
        idIndex[tasks[i].id] = i;
}


int main(int argc, char* argv[]) {

    string filename = argv[1];
    string mode = argv[2];
    string caseType = argv[3];

    caseA = (caseType=="A");

    parseInput(filename);

    if(mode=="EARLIEST") {

        N = stoi(argv[4]);
        int cgpt = stoi(argv[5]);
        int gem  = stoi(argv[6]);

        State start;
        start.done.assign(totAssgns,0);
        start.day = 0;

        // main-code
        dfs(start, cgpt, gem);
        dfbb(start, cgpt, gem);
        int resAstar = Astar(cgpt, gem);

        cout<<"DFS days: "<<bestDFS<<"\n";
        cout<<"DFBB days: "<<bestDFBB<<"\n";
        cout<<"A* days: "<<resAstar<<"\n";

        cout<<"Nodes DFS: "<<nodesDFS<<"\n";
        cout<<"Nodes DFBB: "<<nodesDFBB<<"\n";
        cout<<"Nodes A*: "<<nodesAstar<<"\n";
    }

    else if(mode=="MINCOST") {

        N = stoi(argv[4]);
        int M = stoi(argv[5]);
        C1 = stoi(argv[6]);
        C2 = stoi(argv[7]);

        int bestCost = 1e9;
        int bestCGPT=0, bestGEM=0;

        for(int cgpt=1; cgpt<=20; cgpt++) {
            for(int gem=1; gem<=20; gem++) {

                nodesAstar=0;

                int days = Astar(cgpt, gem);

                if(days!=-1 && days<=M) {
                    int cost = cgpt*C1 + gem*C2;
                    if(cost < bestCost) {
                        bestCost = cost;
                        bestCGPT = cgpt;
                        bestGEM = gem;
                    }
                }
            }
        }

        if(bestCost==1e9)
            cout<<"Infeasible\n";
        else {
            cout<<"Minimum cost: "<<bestCost<<"\n";
            cout<<"ChatGPT prompts/day: "<<bestCGPT<<"\n";
            cout<<"Gemini prompts/day: "<<bestGEM<<"\n";
        }
    }

    return 0;
}
