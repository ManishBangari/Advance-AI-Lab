#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// Using your existing Node structure
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

// Global structures for the backtracking logic
map<int, vector<int>> adj;    // Adjacency list: Dependency -> Task
map<int, int> in_degree;      // Task -> Number of dependencies
vector<int> all_node_ids;
vector<int> current_path;
bool visited[1000] = {false}; // Simplified visit tracking
  
map<int, int> idprmtmap;

vector<vector<int>> allperms;

void findAllSequences(int totalNodes) {
    bool finished = true;

    for (int id : all_node_ids) {
        // If the node has no dependencies left and hasn't been used in this path
        if (in_degree[id] == 0 && !visited[id]) {
            
            // 1. Mark and push to path
            visited[id] = true;
            current_path.push_back(id);

            // 2. Reduce in-degree of all nodes that depended on this one
            for (int neighbor : adj[id]) {
                in_degree[neighbor]--;
            }

            // 3. Recurse to find the next node in the sequence
            findAllSequences(totalNodes);

            // 4. Backtrack: Undo everything to try a different node at this position
            for (int neighbor : adj[id]) {
                in_degree[neighbor]++;
            }
            current_path.pop_back();
            visited[id] = false;

            finished = false;
        }
    }

    // If the path is full, we found a valid sequence
    if (finished && current_path.size() == totalNodes) {
        for (int i = 0; i < current_path.size(); i++) {
            cout << "A" << current_path[i] << (i == current_path.size() - 1 ? "" : " -> ");
        }
        allperms.push_back(current_path);
        cout << endl;
    }
}

int main() {
    int n, k, m;

    cout << "Number of students in a group: "; cin >> n;
    cout << "Number of prompts allowed per day: "; cin >> k;

    cout << "Assignment details (A <id> <count> <deps> 0). Type 'END' to finish:\n";
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
        }
    }

    // --- PREPARE FOR TOPOLOGICAL SORT ---
    for (const auto& node : graph) {
        all_node_ids.push_back(node.id);
        
        // Ensure the node exists in the in_degree map
        if (in_degree.find(node.id) == in_degree.end()) in_degree[node.id] = 0;

        for (int d : node.dependencies) {
            // Your input: A2 depends on A1. 
            // Logical flow: A1 -> A2 (A1 must finish to unlock A2)
            adj[d].push_back(node.id);
            in_degree[node.id]++;
        }
    }

    // Sorting IDs ensures the sequences are generated in a predictable order
    sort(all_node_ids.begin(), all_node_ids.end());

    cout << "\n--- All Valid Topological Sequences ---\n";
    findAllSequences(all_node_ids.size());

    cout<<"Perms are \n";
    
    for(auto &vec : allperms) {
        for(auto &i : vec) {
            cout << i << ' ';
        }
        cout << endl;
    }

    cout << "\nNumber of days allowed (m): ";
    cin >> m;

    return 0;
}