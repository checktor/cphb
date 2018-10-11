#include <iostream>
#include <vector>

// Adjacency list representation of tree.
static std::vector< std::vector<int> > tree;

// Store parent of each node as given by last dfs run.
static std::vector<int> parents;

// Store length of longest and second longest path
// to a leaf for every single node in the tree.
static std::vector< std::pair<int, int> > lengthLongestPathToLeaf;

// For every node in the tree, store child node through which
// the longest / the second longest path to a leaf leads.
static std::vector< std::pair<int, int> > nodeLongestPathToLeaf;

void process(int current, int child) {
        // Add current node as parent of child node (if necessary).
        if (parents[child] == -1) {
                parents[child] = current;
        }
        // Longest path to leaf from current node may be
        // the longest path from its child increased by one.
        int currentLongestPath = lengthLongestPathToLeaf[child].first + 1;
        // Record the longest and the second longest path to leaf.
        if (currentLongestPath > lengthLongestPathToLeaf[current].first) {
                // Current path is longer than previously stored
                // longest path, therefore add previously stored path
                // as second longest and current path as longest.
                lengthLongestPathToLeaf[current].second = lengthLongestPathToLeaf[current].first;
                nodeLongestPathToLeaf[current].second = nodeLongestPathToLeaf[current].first;
                lengthLongestPathToLeaf[current].first = currentLongestPath;
                nodeLongestPathToLeaf[current].first = child;
        } else if (currentLongestPath > lengthLongestPathToLeaf[current].second) {
                // Current path is longer than previously stored second
                // longest path, therefore update data accordingly.
                lengthLongestPathToLeaf[current].second = currentLongestPath;
                nodeLongestPathToLeaf[current].second = child;
        }
}

void dfs(int parent, int current) {
        for (int i = 0; i < tree[current].size(); i++) {
                // Recursive call on each neighbour of
                // current node excluding its parent.
                int child = tree[current][i];
                if (child == parent) {
                        continue;
                }
                dfs(current, child);
                // Update metadata after each recursive call.
                process(current, child);
        }
}

int getLengthDiameter() {
        // Compute diameter by adding up longest and
        // second longest path to a leaf concerning
        // every node and choosing the maximum.
        int maxDiameter = 0;
        for (int i = 0; i < lengthLongestPathToLeaf.size(); i++) {
                int diameter = lengthLongestPathToLeaf[i].first + lengthLongestPathToLeaf[i].second;
                if (diameter > maxDiameter) {
                        maxDiameter = diameter;
                }
        }
        return maxDiameter;
}

void getLengthAllLongestPaths(std::vector<int>* allLongestPaths) {
        // Move through every node in tree.
        for (int current = 0; current < tree.size(); current++) {
                // Initialize length of longest path starting at current
                // node with longest path through one of its children.
                int lengthLongestPath = lengthLongestPathToLeaf[current].first;
                // Move upwards through the tree searching
                // for an alternative path which is longer.
                int counter = 1;
                int lowerNode = current;
                int upperNode = parents[current];
                while (upperNode != -1) {
                        if (nodeLongestPathToLeaf[upperNode].first == lowerNode) {
                                // Current lower node is part of longest
                                // path from upper node, therefore
                                // consider its second longest path.
                                int secondLength = lengthLongestPathToLeaf[upperNode].second + counter;
                                if (secondLength > lengthLongestPath) {
                                        lengthLongestPath = secondLength;
                                }
                        } else {
                                // Lower node is not part of longest path,
                                // so its length can be taken into account.
                                int firstLength = lengthLongestPathToLeaf[upperNode].first + counter;
                                if (firstLength > lengthLongestPath) {
                                        lengthLongestPath = firstLength;
                                }
                        }
                        lowerNode = upperNode;
                        upperNode = parents[upperNode];
                        counter++;
                }
                (*allLongestPaths)[current] = lengthLongestPath;
        }
}

int main() {
        // Define tree and corresponding metadata.
        int numNodes = 6;
        for (int i = 0; i < numNodes; i++) {
                // Tree.
                std::vector<int> defaultVector;
                tree.push_back(defaultVector);
                // Parents.
                parents.push_back(-1);
                // Metadata.
                std::pair<int, int> firstDefaultPair(0, 0);
                lengthLongestPathToLeaf.push_back(firstDefaultPair);
                std::pair<int, int> secondDefaultPair(-1, -1);
                nodeLongestPathToLeaf.push_back(secondDefaultPair);
        }

        // Add edges.
        tree[0].push_back(1);
        tree[0].push_back(2);
        tree[0].push_back(3);
        tree[1].push_back(0);
        tree[1].push_back(4);
        tree[1].push_back(5);
        tree[2].push_back(0);
        tree[3].push_back(0);
        tree[3].push_back(6);
        tree[4].push_back(1);
        tree[5].push_back(1);
        tree[6].push_back(3);

        // Compute longest path from each node to leaf node
        // considering the tree as rooted on specified node.
        int rootNode = 0;
        dfs(-1, rootNode);

        // Use results to compute given graph's diameter.
        int length = getLengthDiameter();
        std::cout << length << std::endl;

        // Use results to compute longest paths starting at each node.
        std::vector<int>* allLongestPaths = new std::vector<int>(numNodes);
        getLengthAllLongestPaths(allLongestPaths);
        for (int i = 0; i < allLongestPaths->size(); i++) {
                std::cout << (i + 1) << ": " << (*allLongestPaths)[i] << "\n";
        }
        delete allLongestPaths;

        return 0;
}
