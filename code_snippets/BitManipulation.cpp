#include <iostream>
#include <string>
#include <vector>


// Basic bit operations.
// =====================

bool checkKthBit(int num, int k) {
        // Return true if k-th bit is 1.
        return (num & (1 << k));
}

int setKthBitToZero(int num, int k) {
        // Return given number with k-th bit set to zero.
        return (num & ~(1 << k));
}

int setKthBitToOne(int num, int k) {
        // Return given number with k-th bit set to one.
        return (num | (1 << k));
}

int invertKthBit(int num, int k) {
        // Return given number with k-th bit inverted.
        return (num ^ (1 << k));
}

std::string getBitString(int num) {
        // Initialize strings for 0 and 1 bit.
        std::string zero("0");
        std::string one("1");
        // Initialize empty string for bit representation.
        std::string bitString("");
        // Iterate through number bit by bit starting
        // at the right and create corresponding string.
        for (int i = 0; i < 32; i++) {
                if (checkKthBit(num, i)) {
                        bitString.insert(0, one);
                } else {
                        bitString.insert(0, zero);
                }
        }
        return bitString;
}


// Represent sets using bit operations.
// ====================================

int convertToIntegerSet(std::vector<int> vectorSet) {
        // Create integer representation of empty subset.
        int integerSet = 0;
        // Add each element in vector to integer subset.
        for (int i = 0; i < vectorSet.size(); i++) {
                int element = vectorSet[i];
                if (element >= 0 && element <= 31) {
                        // Element is valid, therefore continue.
                        integerSet = setKthBitToOne(integerSet, element);
                } else {
                        // Element is invalid, therefore return error code -1.
                        return -1;
                }
        }
        return integerSet;
}

std::vector<int> convertToVectorSet(int integerSet) {
        // Create empty vector to store set elements.
        std::vector<int> vectorSet;
        // Iterate through given set element by
        // element and create corresponding vector.
        for (int i = 0; i < 32; i++) {
                if (checkKthBit(integerSet, i)) {
                        vectorSet.push_back(i);
                }
        }
        return vectorSet;
}

std::string getVectorString(std::vector<int> set) {
        // Initialize white space string.
        std::string whiteSpace(" ");
        // Initialize empty string for vector representation.
        std::string vectorString("");
        // Iterate through vector element by element
        // and create corresponding string.
        for (int i = 0; i < set.size(); i++) {
                vectorString += std::to_string(set[i]);
                vectorString += whiteSpace;
        }
        return vectorString;
}


// Basic set operations.
// =====================

int getIntersection(int firstSet, int secondSet) {
        return (firstSet & secondSet);
}

int getUnion(int firstSet, int secondSet) {
        return (firstSet | secondSet);
}

int getDifference(int firstSet, int secondSet) {
        return (firstSet & (~secondSet));
}

int getComplement(int set) {
        return (~set);
}

std::vector<int> getAllSubsets(int set) {
        // Create empty vector to store all subsets
        // in corresponding integer representation.
        std::vector<int> allSubsets;
        int currentSubset = 0;
        do {
                allSubsets.push_back(currentSubset);
        } while (currentSubset = (currentSubset - set) & set);
        return allSubsets;
}

int main() {
        // Input data.
        std::vector<int> firstVectorSet {0, 2};
        std::vector<int> secondVectorSet {1, 2, 5, 28};
        int firstIntSet = convertToIntegerSet(firstVectorSet);
        int secondIntSet = convertToIntegerSet(secondVectorSet);

        // Print all subsets.
        std::vector<int> allSubsets = getAllSubsets(firstIntSet);
        for (int i = 0; i < allSubsets.size(); i++) {
                std::cout << getVectorString(convertToVectorSet(allSubsets[i])) << std::endl;
        }

        // Set operations.
        std::cout << getBitString(firstIntSet) << std::endl;
        std::cout << getBitString(secondIntSet) << std::endl;
        std::cout << getBitString(getIntersection(firstIntSet, secondIntSet)) << std::endl;
        std::cout << getBitString(getUnion(firstIntSet, secondIntSet)) << std::endl;
        std::cout << getBitString(getDifference(firstIntSet, secondIntSet)) << std::endl;

        return 0;
}
