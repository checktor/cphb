#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

int getParentIndex(int index) {
        // Get index of parent.
        return std::floor(index / 2);
}

int getLeftChildIndex(int index) {
        // Get index of left child.
        return 2 * index;
}

int getRightChildIndex(int index) {
        // Get index of right child.
        return getLeftChildIndex(index) + 1;
}

int process(int a, int b) {
        // Specify how two neighboring entries should be processed.
        return a + b;
        // return std::min(a, b);
        // return std::max(a, b);
}

int getRangeBottomUp(std::vector<int>* tree,
                        int leftIndex,
                        int rightIndex) {
        // Convert data indices to tree indices.
        int dataSize = tree->size() / 2;
        leftIndex += dataSize;
        rightIndex += dataSize;
        // Initialize result variable.
        int result;
        // Create a boolean flag indicating if the
        // result variable is used for the first time.
        bool isFirstResult = true;
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
                        if (isFirstResult) {
                                result = (*tree)[leftIndex];
                                isFirstResult = false;
                        } else {
                                result = process(result, (*tree)[leftIndex]);
                        }
                        leftIndex++;
                }
                // Even indices indicate a left child in the
                // tree. If the right border is a right child,
                // there is also nothing to do.
                if ((rightIndex % 2) == 0) {
                        // Otherwise, add the right border to the
                        // result and move to its left neighbour.
                        if (isFirstResult) {
                                result = (*tree)[rightIndex];
                                isFirstResult = false;
                        } else {
                                result = process(result, (*tree)[rightIndex]);
                        }
                        rightIndex--;
                }
                // Continue processing with parent nodes.
                leftIndex = getParentIndex(leftIndex);
                rightIndex = getParentIndex(rightIndex);
        }
        return result;
}

int getRangeTopDown(std::vector<int>* tree,
                       int leftIndex,
                       int rightIndex,
                       int currentNode,
                       int nodeRangeLeftIndex,
                       int nodeRangeRightIndex) {
        // Desired array range is completely outside the
        // range covered by current node. In this case,
        // the corresponding precomputed value is irrelevant.
        if ((rightIndex < nodeRangeLeftIndex) || (leftIndex > nodeRangeRightIndex)) {
                throw std::invalid_argument("Desired range is invalid.");
        }
        // Range covered by current node completely
        // contains desired array range, therefore its
        // precomputed value needs to be taken into account.
        if ((leftIndex <= nodeRangeLeftIndex) && (nodeRangeRightIndex <= rightIndex)) {
                return (*tree)[currentNode];
        }
        // Continue processing with child nodes.
        int leftChildIndex = getLeftChildIndex(currentNode);
        int rightChildIndex = getRightChildIndex(currentNode);
        // Compute right border of range covered by left child node.
        int border = (nodeRangeLeftIndex + nodeRangeRightIndex) / 2;
        // Recursive call for left child node.
        int leftResult;
        bool leftSuccessful = true;
        try {
                leftResult = getRangeTopDown(tree, leftIndex, rightIndex, leftChildIndex, nodeRangeLeftIndex, border);
        } catch(const std::invalid_argument& e) {
                leftSuccessful = false;
        }
        // Recursive call for right child node.
        int rightResult;
        bool rightSuccessful = true;
        try {
                rightResult = getRangeTopDown(tree, leftIndex, rightIndex, rightChildIndex, (border + 1), nodeRangeRightIndex);
        } catch(const std::invalid_argument& e) {
                rightSuccessful = false;
        }
        // Return result depending on successfully processed child nodes.
        if (leftSuccessful && rightSuccessful) {
                // Both child nodes could be processed successfully.
                return process(leftResult, rightResult);
        } else if (!leftSuccessful && rightSuccessful) {
                // Only right child could be processed successfully.
                return rightResult;
        } else if (leftSuccessful && !rightSuccessful) {
                // Only left child could be processed successfully.
                return leftResult;
        } else {
                // No child node could be processed successfully.
                throw std::invalid_argument("Desired range invalid.");
        }
}

void updateTree(std::vector<int>* tree, int index, int difference) {
        // Convert data index to tree index.
        int dataSize = tree->size() / 2;
        index += dataSize;
        // Increase or decrease specific leaf
        // node by given difference amount.
        (*tree)[index] += difference;
        // Propagate change through
        // tree from bottom to top.
        index = getParentIndex(index);
        while(index >= 1) {
                (*tree)[index] = process((*tree)[getLeftChildIndex(index)], (*tree)[getRightChildIndex(index)]);
                index = getParentIndex(index);
        }
}

std::vector<int>* createTree(std::vector<int> data) {
        // Resize input data to the next power of 2.
        int dataSize = std::pow(2, std::ceil(std::log(data.size())/std::log(2)));
        data.resize(dataSize);
        // Create array of twice the input size initialized with zeros.
        int treeSize = 2 * dataSize;
        std::vector<int>* tree = new std::vector<int>(treeSize, 0);
        // Copy values of input array to leafs,
        // i.e. to the right half of the tree array.
        for (int i = 0; i < dataSize; i++) {
                (*tree)[dataSize + i] = data[i];
        }
        // Compute values of inner nodes, i.e.
        // the left half of the tree array. Note
        // that the entry at index 0 stays unused,
        // so the array starts with index 1. This
        // procedure simplifies index computation.
        for (int i = dataSize - 1; i > 0; i--) {
                int leftChildIndex = getLeftChildIndex(i);
                int rightChildIndex = getRightChildIndex(i);
                (*tree)[i] = process((*tree)[leftChildIndex], (*tree)[rightChildIndex]);
        }
        // Return tree.
        return tree;
}

int main() {
        // Input data array.
        std::vector<int> data {5, 8, 6, 3, 2, 7, 2, 6};
        // Create segment tree represented as an array.
        std::vector<int>* tree = createTree(data);
        // Process tree.
        std::cout << getRangeBottomUp(tree, 2, 7) << std::endl;
        std::cout << getRangeTopDown(tree, 2, 7, 1, 0, (tree->size() / 2) - 1) << std::endl;
        // Change a single entry.
        updateTree(tree, 4, -1);
        // Process tree again.
        std::cout << getRangeBottomUp(tree, 2, 7) << std::endl;
        std::cout << getRangeTopDown(tree, 2, 7, 1, 0, (tree->size() / 2) - 1) << std::endl;
        // Delete tree.
        delete tree;

        return 0;
}
