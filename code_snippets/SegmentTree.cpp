#include <cmath>
#include <iostream>
#include <vector>


int getParentIndex(int index) {
        // Get index of parent.
        return std::floor(index / 2);
}

int getLeftChildIndex(int index) {
        // Get index of left child.
        return 2 * index;
}

int process(int a, int b) {
        // Specify how entries should be processed.
        return std::max(a, b);
}

void createTree(std::vector<int> data, std::vector<int>* tree) {
        // Create segment tree as an array.
        int dataSize = data.size();
        int treeSize = 2 * dataSize;
        tree->resize(treeSize);
        // Fill right part of segment tree with input array.
        for (int i = 0; i < dataSize; i++) {
                (*tree)[dataSize + i] = data[i];
        }
        // Fill left part of segment tree.
        for (int i = dataSize - 1; i > 0; i--) {
                int leftChildIndex = getLeftChildIndex(i);
                (*tree)[i] = process((*tree)[leftChildIndex], (*tree)[leftChildIndex + 1]);
        }
}

int processTree(std::vector<int>* tree, int leftIndex, int rightIndex) {
        // Convert data indices to tree indices.
        int dataSize = tree->size() / 2;
        leftIndex += dataSize;
        rightIndex += dataSize;
        // Initialize result variable.
        int result = 0;
        // Move through segment tree from bottom to top.
        while (leftIndex <= rightIndex) {
                // Odd indices indicate a right child in the tree.
                // If the left border is also left child, there is
                // nothing to do because this part of the array is
                // already processed in the parent node.
                if ((leftIndex % 2) == 1) {
                        // Otherwise, the left border is a right child,
                        // therefore add it to the result and move to
                        // its right neighbour in the tree.
                        result = process(result, (*tree)[leftIndex]);
                        leftIndex++;
                }
                // Even indices indicate a left child in the
                // tree. If the right border is a right child,
                // there is also nothing to do.
                if ((rightIndex % 2) == 0) {
                        // Otherwise, add the right border to the
                        // result and move to its left neighbour.
                        result = process(result, (*tree)[rightIndex]);
                        rightIndex--;
                }
                // Continue with parent nodes.
                leftIndex = getParentIndex(leftIndex);
                rightIndex = getParentIndex(rightIndex);
        }
        return result;
}

void changeTree(std::vector<int>* tree, int index, int difference) {
        // Convert data index to tree index.
        int dataSize = tree->size() / 2;
        index += dataSize;
        // Increase specified leaf node by given difference.
        (*tree)[index] += difference;
        // Propagate change through tree from bottom to top.
        index = getParentIndex(index);
        while(index >= 1) {
                (*tree)[index] = process((*tree)[getLeftChildIndex(index)], (*tree)[getLeftChildIndex(index) + 1]);
                index = getParentIndex(index);
        }
}

int main() {
        // Input data.
        std::vector<int> data {5, 8, 6, 3, 2, 7, 2, 6};
        // Resize input data to the next power of 2.
        int dataSize = std::pow(2, std::ceil(std::log(data.size())/std::log(2)));
        data.resize(dataSize);
        // Create segment tree.
        std::vector<int>* tree = new std::vector<int>();
        createTree(data, tree);
        // Process segment tree.
        std::cout << processTree(tree, 2, 7) << std::endl;
        // Change a single entry in tree.
        changeTree(tree, 5, -2);
        // Process segment tree again.
        std::cout << processTree(tree, 2, 7) << std::endl;
        // Delete tree.
        delete tree;
        return 0;
}
