/*==================================================
 * Name: Maxwell Ding
 * Student ID: 251279310
 * Description: a)	A Disjoint-Set data structure should be implemented, 
 * with the most efficient algorithm (union by rank and path compression), 
 * as an abstract data type (a class in C++ or java) with the following operations.
 *==================================================*/
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <vector>

using namespace std;

class DisjointSet
{
private:
    vector<int> parent;
    vector<int> rank;

public:
    // DisjointSet() constructor
    DisjointSet(int n)
    {
        uandf(n);
    }
    // uandf (n): constructs an disjoint-set data type with n elements, 1, 2, . . . , n.
    void uandf(int n)
    {
        parent.resize(n + 1);
        rank.resize(n + 1, 0);
        for (int i = 1; i <= n; i++)
        {
            parent[i] = i;
        }
    }
    // makeSet(): creates a new set whose only member (and thus representative) is i.
    void makeSet(int i)
    {
        parent[i] = i;
        rank[i] = 0;
    }
    // findSet(i): returns the representative of the set containing i.
    int findSet(int i)
    {
        if (parent[i] == i)
        {
            return i;
        }
        parent[i] = findSet(parent[i]); // path compression
        return parent[i];
    }
    // unionSets(i, j): unites the dynamic sets that contains i and j, respectively, into a new set that is the union of these two sets.
    void unionSets(int i, int j)
    {
        int root_i = findSet(i);
        int root_j = findSet(j);
        if (root_i == root_j)
        {
            return;
        }
        if (rank[root_i] < rank[root_j])
        {
            parent[root_i] = root_j;
        }
        else if (rank[root_i] > rank[root_j])
        {
            parent[root_j] = root_i;
        }
        else
        {
            parent[root_j] = root_i;
            rank[root_i]++;
        }
    }
    // finalSets(): returns the number of disjoint sets.
    int finalSets()
    {
        int num = 0;
        for (int i = 1; i < parent.size(); i++)
        {
            if (parent[i] == i)
            {
                num++;
            }
        }
        return num;
    }
};

// findConnectedComponents(): finds connected components in a binary image
void findConnectedComponents(const vector<vector<bool>> &image)
{
    const int dx[] = {-1, 0, 1, 0}; // x-directions of neighbors
    const int dy[] = {0, 1, 0, -1}; // y-directions of neighbors
    int i, rows = image.size(), cols = image[0].size();
    DisjointSet ds(rows * cols);
    // Make sets for foreground pixels
    for (int i = 0; i < rows * cols; i++)
    {
        if (image[i / cols][i % cols])
        {
            ds.makeSet(i + 1); // 1-based indexing
        }
    }
    // Union sets for neighboring foreground pixels
    for (int i = 0; i < rows * cols; i++)
    {
        if (image[i / cols][i % cols])
        {
            int index = i + 1; // 1-based indexing
            for (int k = 0; k < 4; k++)
            {
                // check neighbors
                int ni = i / cols + dx[k];
                int nj = i % cols + dy[k];
                if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && image[ni][nj])
                {
                    int neighbor = ni * cols + nj + 1; // 1-based indexing
                    ds.unionSets(index, neighbor);
                }
            }
        }
    }
    // Finalize sets and count connected components
    ds.finalSets();        // ignore return value, just finalize sets
    map<int, int> sizes; // map from set representative to size
    for (int i = 1; i <= rows * cols; i++)
    {
        if (image[(i - 1) / cols][(i - 1) % cols])
        { // only consider foreground pixels
            int rep = ds.findSet(i);
            sizes[rep]++;
        }
    }
    // Output original image
    cout << "The input binary image:" << endl;
    for (int i = 0; i < rows * cols; i++)
    {
        cout << (image[i / cols][i % cols] ? '+' : ' ') << ' ' << (i % cols == cols - 1 ? '\n' : '\0');
    }

    vector<char> labels(rows * cols); // label for each component
    char next_label = 'a';
    cout << "Connected component image:\n";
    for (i = 0; i < rows * cols; i++)
    {
        if (image[i / cols][i % cols])
        {
            int rep = ds.findSet(i + 1);
            if (labels[rep - 1] == 0)
            { // not labeled yet
                labels[rep - 1] = next_label++;
            }
            cout << labels[rep - 1] << ' ' << (i % cols == cols - 1 ? '\n' : '\0');
        }
        else
        {
            cout << "  " << (i % cols == cols - 1 ? '\n' : '\0');
        }
    }
    vector<pair<int, char>> sorted_sizes; // list of (size, label) pairs
    for (auto it = sizes.begin(); it != sizes.end(); ++it)
    {
        sorted_sizes.push_back(make_pair(it->second, labels[it->first - 1]));
    }
    sort(sorted_sizes.begin(), sorted_sizes.end(), greater<pair<int, char>>()); // sort by size in descending order
    cout << "Connected component sizes:\n";
    for (auto it = sorted_sizes.begin(); it != sorted_sizes.end(); ++it)
    {
        cout << it->first << ' ' << it->second << '\n';
    }
    cout << "Connected components with size > 1:\n";
    for (i = 0; i < rows * cols; i++)
    {
        if (image[i / cols][i % cols])
        {
            int rep = ds.findSet(i + 1);
            cout << (sizes[rep] > 1 ? labels[rep - 1] : ' ');
        }
        else
        {
            cout << ' ';
        }
        cout << ' ' << (i % cols == cols - 1 ? '\n' : '\0');
    }
    cout << "Connected components with size > 11:\n";
    for (i = 0; i < rows * cols; i++)
    {
        if (image[i / cols][i % cols])
        {
            int rep = ds.findSet(i + 1);
            cout << (sizes[rep] > 11 ? labels[rep - 1] : ' ');
        }
        else
        {
            cout << ' ';
        }
        cout << ' ' << (i % cols == cols - 1 ? '\n' : '\0');
    }
}

int main(int argc, char *argv[])
{
   int rows = 0, cols = 0;
    string line;

    // get the number of rows and columns
    getline(cin, line);
    if(argc != 1 || line.empty())
    {
        cout << "Usage:" << argv[0] << "< input.txt" << endl;
        return 0;
    }
    cols = line.length();
    cin.seekg(0, ios::beg);
    rows = count(istreambuf_iterator<char>(cin), istreambuf_iterator<char>(), '\n');
    cin.seekg(0, ios::beg);
    vector<vector<bool>> image(rows, vector<bool>(cols));

    // read the image
    for (int i = 0; i < rows; i++)
    {
        getline(cin, line);
        for (int j = 0; j < cols; j++)
        {
            // check if the character is a plus sign
            image[i][j] = (line[j] == 43);
        }
    }
    findConnectedComponents(image);
    cout << "Resize the window to see better output." << endl;
    return 0;
}