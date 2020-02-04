#include <iostream>
#include <vector>

int getLargestDividingPowerOfTwo(int num) {
    // Compute the largest power of
    // two which divides given number.
    return (num)&(-num);
}

int getSum(std::vector<int>* tree, int index) {
    // A tree entry at index k contains the
    // sum of a specific input array subrange
    // that ends at k and whose length is
    // defined by the largest power of two
    // which divides k. In order to compute
    // the sum of range [1, k], you start at
    // index k and move backwards through the
    // array looking at chosen entries and
    // summing up its values. At each step, the
    // distance to the next entry is proportional
    // to the length of the input subrange
    // represented at this particular index.
    int sum = 0;
    while (index >= 1) {
        sum += (*tree)[index];
        index -= getLargestDividingPowerOfTwo(index);
    }
    return sum;
}

int getSum(std::vector<int>* tree, int leftIndex, int rightIndex) {
    // Compute range [a, b] by splitting it into
    // two overlapping subranges, both starting at
    // the first index , i.e. [1, a] and [1, b].
    // Therefore, [a, b] = [1, b] - [1, a - 1].
    return getSum(tree, rightIndex) - getSum(tree, leftIndex - 1);
}

void updateTree(std::vector<int>* tree, int index, int difference) {
    // Increase or decrease specific input
    // array entry by given difference amount
    // and propagate the change through the tree.
    // You start with an update of the entry at
    // index k before looking at all entries
    // whose corresponding subranges include k.
    // These entries can be found at indices
    // higher than k. Analogous to the subrange
    // sum computation, the distance to the next
    // entry can be computed as the largest power
    // of two which divides the current index.
    while (index <= tree->size()) {
        (*tree)[index] += difference;
        index += getLargestDividingPowerOfTwo(index);
    }
}

std::vector<int>* createTree(std::vector<int> data) {
    // Create array with capacity of the input size + 1
    // initialized with zeros. This allows the tree array
    // to start with index 1, leaving the entry at index
    // 0 unused. This procedure simplifies index computation.
    std::vector<int>* tree = new std::vector<int>(data.size() + 1, 0);
    // Iterate through input data array entry by entry
    // updating the tree at each step. The update procedure
    // guarantees to update all relevant subranges.
    for (int index = 1; index < tree->size(); index++) {
        updateTree(tree, index, data[index - 1]);
    }
    return tree;
}

int main() {
    // Input data array.
    std::vector<int> data {1, 3, 4, 8, 6, 1, 4, 2};
    // Create binary indexed tree represented as an array.
    std::vector<int>* tree = createTree(data);
    // Process tree.
    std::cout << getSum(tree, 4, 6) << std::endl;
    // Change a single entry.
    updateTree(tree, 4, -1);
    // Process tree again.
    std::cout << getSum(tree, 4, 6) << std::endl;
    // Delete tree.
    delete tree;

    return 0;
}