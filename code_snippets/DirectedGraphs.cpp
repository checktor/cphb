#include <cmath>
#include <iostream>
#include <vector>

// Successor graph.
static std::vector<int> successorGraph;

// Precomputed successor paths.
static std::vector< std::vector<int> > successorPaths;

int getExponent(int base, int result) {
        return std::floor(std::log(result)/std::log(base));
}

void computeSuccessorPaths(int numNodes) {
        // Successor node after first step
        // can be found in original graph.
        for (int i = 0; i < numNodes; i++) {
                successorPaths[i].push_back(successorGraph[i]);
        }
        // Use number of nodes as maximum number
        // of steps, therefore only consider powers
        // of 2 less or equal to given number of nodes.
        int exponent = getExponent(2, numNodes);
        for (int i = 1; i <= exponent; i++) {
                for (int j = 0; j < numNodes; j++) {
                        // Use values of previous row to
                        // compute current successor node.
                        successorPaths[j].push_back(successorPaths[successorPaths[j][i - 1]][i - 1]);
                }
        }
}

int getSuccessor(int startNode, int numSteps) {
        // Start at given node.
        int currentNode = startNode;
        // Break down the specified number of steps into
        // several substeps corresponding to powers of two.
        // Thereby, make sure to only use powers of two for which
        // a successor is precomputed in successor paths matrix.
        int maxExponent = successorPaths[0].size() - 1;
        while(numSteps > 0) {
                // Get greatest possible power of two precomputed
                // in matrix and move pointer correspondingly.
                int exponent = getExponent(2, numSteps);
                if (exponent > maxExponent) {
                        exponent = maxExponent;
                }
                currentNode = successorPaths[currentNode][exponent];
                numSteps -= std::pow(2, exponent);
        }
        return currentNode;
}

int getFirstNodeCycle(int startNode) {
        // Create two pointers starting at the same node. The first
        // is moving one step at a time and the other two steps.
        // The first node where both pointers meet again must be a
        // node in the cycle, therefore compute this node.
        int firstPointer = getSuccessor(startNode, 1);
        int secondPointer = getSuccessor(startNode, 2);
        while (firstPointer != secondPointer) {
                firstPointer = getSuccessor(firstPointer, 1);
                secondPointer = getSuccessor(secondPointer, 2);
        }
        // Current node is part of the cycle but not necessarily
        // the first node seen from starting point. So reset first
        // pointer to the starting node before moving through the
        // graph again but this time with the same speed of one
        // node per step for both pointers. The first time the
        // pointers meet again indicates the first node of the cycle.
        firstPointer = startNode;
        while (firstPointer != secondPointer) {
                firstPointer = getSuccessor(firstPointer, 1);
                secondPointer = getSuccessor(secondPointer, 1);
        }
        return firstPointer;
}

int getLengthCycle(int firstNodeCycle) {
        // Create two pointers, the first is pointing to beginning
        // of cycle and the other to its direct successor. Then
        // move second pointer step by step through graph while
        // increasing a counter until both pointers meet again.
        int secondNodeCycle = getSuccessor(firstNodeCycle, 1);
        int length = 1;
        while (firstNodeCycle != secondNodeCycle) {
                secondNodeCycle = getSuccessor(secondNodeCycle, 1);
                length++;
        }
        return length;
}

int main() {
        // Define successor graph.
        int numNodes = 6;
        successorGraph.resize(numNodes);
        // Add edges.
        successorGraph[0] = 1;
        successorGraph[1] = 2;
        successorGraph[2] = 3;
        successorGraph[3] = 4;
        successorGraph[4] = 5;
        successorGraph[5] = 3;

        // Initialize successor paths.
        for (int i = 0; i < numNodes; i++) {
                std::vector<int> defaultVector;
                successorPaths.push_back(defaultVector);
        }

        // Precompute values of successor paths.
        computeSuccessorPaths(numNodes);

        // Define start node.
        int startNode = 0;

        // Get beginning and length of cycle.
        int beginningCycle = getFirstNodeCycle(startNode);
        int length = getLengthCycle(beginningCycle);
        std::cout << beginningCycle + 1 << "\n";
        std::cout << length << "\n";

        return 0;
}
