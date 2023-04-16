
//  ContainerTest.cpp
//  vectorTest
//
//  Created by Chris Gregg on 4/4/2020
//

// To compile with g++ 4.4.7:
// g++ -std=c++0x -Os -Wall -Wextra ContainerTest.cpp -o ContainerTest
//
// Can also use a newer version of g++:
// g++ -std=c++11 -Os -Wall -Wextra ContainerTest.cpp -o ContainerTest
//
// Or with clang++:
// clang++ -Os -std=c++11 -Wall -Wextra ContainerTest.cpp -o ContainerTest

// Create 4 list-like data structures (and one final sorted vector)
// vector, sorted vector, linked-list (list), binary search tree (set), hash map
// (unordered_set) adds 100,000 elements (even ints from 0-200,000) searches for
// 50,000 elements (ints from 0-50,000) deletes 20,000 elements (ints from
// 0-20,000)

#include <algorithm>
#include <iostream>
#include <list>
#include <random>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unordered_set>
#include <vector>

using namespace std;

#define INSERTS 100000
#define SEARCHES 50000
#define DELETES 20000


/*
#define INSERTS 50000
#define SEARCHES 25000
#define DELETES 25000
*/

/*
#define INSERTS 1000000
#define SEARCHES 500000
#define DELETES 200000
*/

// Global variables so that the compiler doesn't optimize them out
vector<int>::iterator vector_iterator;
list<int>::iterator list_iterator;
set<int>::iterator set_iterator;
unordered_set<int>::iterator unordered_set_iterator;

struct TimeInfo {
    struct timeval insertStart, insertEnd;
    struct timeval searchStart, searchEnd;
    struct timeval deleteStart, deleteEnd;
    double overallTime, insertTime, searchTime, deleteTime;
};

void array_test(TimeInfo& timeInfo, vector<int>& random_vec, bool sorted) {
    vector<int> vec;

    // inserts (even ints)
    if (!sorted) {
        gettimeofday(&timeInfo.insertStart, NULL);
        for (int num : random_vec) {
            // even numbers between 0 and INSERTS * 2 (non-inclusive)
            vec.push_back(num);
        }
        gettimeofday(&timeInfo.insertEnd, NULL);
    } else {
        // we need to be a bit creative to test this. We want to insert into
        // an already sorted array, so let's first insert them by using
        // a binary search to find them and then insert at that location

        // now we can start timing
        gettimeofday(&timeInfo.insertStart, NULL);
        for (int num : random_vec) {
            // we are supposedly inserting in order, so we are forcing a
            // find-and-insert
            vector<int>::iterator v_iter =
                vector_iterator = lower_bound(vec.begin(), vec.end(), num);
            if (v_iter == vec.end()) {
                vec.push_back(num);
            } else if (*v_iter !=
                       num) { // not found, so we can insert at that location
                vec.insert(v_iter, num);
            }
        }
        gettimeofday(&timeInfo.insertEnd, NULL);
    }

    // searches
    gettimeofday(&timeInfo.searchStart, NULL);

    size_t vec_size = vec.size();
    for (int i = 0; i < SEARCHES; i++) {
        // searches for SEARCHES elements (ints from 0-SEARCHES)
        if (!sorted) { // linear search
            for (size_t j = 0; j < vec_size; j++) {
                if (vec[j] == i) {
                    vector_iterator = vec.begin() + j;
                    break;
                }
            }
        } else { // binary search
            vector_iterator = lower_bound(vec.begin(), vec.end(), i);
            // vector_binary_search(vec, i);
        }
    }
    gettimeofday(&timeInfo.searchEnd, NULL);

    // deletes
    gettimeofday(&timeInfo.deleteStart, NULL);
    for (int i = 0; i < DELETES; i++) {
        // deletes DELETES elements (ints from 0-DELETES)
        if (!sorted) { // use linear search
            size_t vec_size = vec.size();
            for (size_t j = 0; j < vec_size; j++) {
                if (vec[j] == i) {
                    vec.erase(vec.begin() + j);
                    break;
                }
            }
        } else { // use binary search
            vector<int>::iterator v_iter =
                lower_bound(vec.begin(), vec.end(), i);
            // size_t bsr = vector_binary_search(vec, i);
            if (v_iter != vec.end() && *v_iter == i) {
                vec.erase(v_iter);
            }
        }
    }
    gettimeofday(&timeInfo.deleteEnd, NULL);
}

void linked_list_test(TimeInfo& timeInfo, vector<int>& random_vec) {
    list<int> lst;
    gettimeofday(&timeInfo.insertStart, NULL);
    // inserts (even ints)
    for (int num : random_vec) {
        // even numbers between 0 and INSERTS * 2 (non-inclusive)
        lst.push_back(num);
    }
    gettimeofday(&timeInfo.insertEnd, NULL);

    // searches
    gettimeofday(&timeInfo.searchStart, NULL);

    for (int i = 0; i < SEARCHES; i++) {
        // searches for SEARCHES elements (ints from 0-SEARCHES)
        list_iterator = find(lst.begin(), lst.end(), i);
    }
    gettimeofday(&timeInfo.searchEnd, NULL);

    // deletes
    gettimeofday(&timeInfo.deleteStart, NULL);
    for (int i = 0; i < DELETES; i++) {
        // deletes DELETES elements (ints from 0-DELETES)

        list<int>::iterator itr = find(lst.begin(), lst.end(), i);
        if (itr != lst.end() && *itr == i) {
            lst.erase(itr);
        }
    }
    gettimeofday(&timeInfo.deleteEnd, NULL);
}

void bst_test(TimeInfo& timeInfo, vector<int> random_vec) {
    set<int> tree;
    gettimeofday(&timeInfo.insertStart, NULL);
    // inserts (even ints)
    for (int num : random_vec) {
        // even numbers between 0 and INSERTS * 2 (non-inclusive)
        tree.insert(num);
    }
    gettimeofday(&timeInfo.insertEnd, NULL);

    // searches/num
    gettimeofday(&timeInfo.searchStart, NULL);

    for (int i = 0; i < SEARCHES; i++) {
        // searches for SEARCHES elements (ints from 0-SEARCHES)
        set_iterator = tree.find(i);
    }

    gettimeofday(&timeInfo.searchEnd, NULL);

    // deletes
    gettimeofday(&timeInfo.deleteStart, NULL);
    for (int i = 0; i < DELETES; i++) {
        // deletes DELETES elements (ints from 0-DELETES)

        tree.erase(i);
    }
    gettimeofday(&timeInfo.deleteEnd, NULL);
}

void hash_test(TimeInfo& timeInfo, vector<int>& random_vec) {
    unordered_set<int> table;

    gettimeofday(&timeInfo.insertStart, NULL);
    // inserts (even ints)
    for (int num : random_vec) {
        // even numbers between 0 and INSERTS * 2 (non-inclusive)
        table.insert(num);
    }
    gettimeofday(&timeInfo.insertEnd, NULL);

    // searches
    gettimeofday(&timeInfo.searchStart, NULL);

    for (int i = 0; i < SEARCHES; i++) {
        // searches for SEARCHES elements (ints from 0-SEARCHES)
        unordered_set_iterator = table.find(i);
    }
    gettimeofday(&timeInfo.searchEnd, NULL);

    // deletes
    gettimeofday(&timeInfo.deleteStart, NULL);
    for (int i = 0; i < DELETES; i++) {
        // deletes DELETES elements (ints from 0-DELETES)
        table.erase(i);
    }
    gettimeofday(&timeInfo.deleteEnd, NULL);
}

void printTiming(TimeInfo timeInfo, string containerType) {
    timeInfo.overallTime =
        ((timeInfo.deleteEnd.tv_sec + timeInfo.deleteEnd.tv_usec / 1000000.0) -
         (timeInfo.insertStart.tv_sec +
          timeInfo.insertStart.tv_usec / 1000000.0));
    timeInfo.insertTime =
        ((timeInfo.insertEnd.tv_sec + timeInfo.insertEnd.tv_usec / 1000000.0) -
         (timeInfo.insertStart.tv_sec +
          timeInfo.insertStart.tv_usec / 1000000.0));
    timeInfo.searchTime =
        ((timeInfo.searchEnd.tv_sec + timeInfo.searchEnd.tv_usec / 1000000.0) -
         (timeInfo.searchStart.tv_sec +
          timeInfo.searchStart.tv_usec / 1000000.0));
    timeInfo.deleteTime =
        ((timeInfo.deleteEnd.tv_sec + timeInfo.deleteEnd.tv_usec / 1000000.0) -
         (timeInfo.deleteStart.tv_sec +
          timeInfo.deleteStart.tv_usec / 1000000.0));
    printf("%-*s", 18, (containerType + string(":")).c_str());
    printf("%-*.5f    ", 10, timeInfo.overallTime);
    printf("%-*.5f    ", 10, timeInfo.insertTime);
    printf("%-*.5f    ", 10, timeInfo.searchTime);
    printf("%-*.5f\n", 10, timeInfo.deleteTime);
    fflush(stdout);
}

int main() {
   // Let's create a random vector of the numbers we want to insert / search
   // for / delete
   vector<int> random_vec;
   for (int i = 0; i < INSERTS; i++) {
       random_vec.push_back(i * 2);
   }
   shuffle(random_vec.begin(), random_vec.end(), default_random_engine(0));

   // std::cout << "Structure\tOverall(s)\tInsert(s)\tSearch(s)\tDelete(s)\n";
   printf("%-*s%-*s    %-*s    %-*s    %-*s\n", 18, "Structure", 10, "Overall(s)",
          10, "Insert(s)", 10, "Search(s)", 10, "Delete(s)");
   TimeInfo testTime;

   array_test(testTime, random_vec, false); // non-binary search
   printTiming(testTime, "Unsorted Vector");

   linked_list_test(testTime, random_vec);
   printTiming(testTime, "Linked List");

   bst_test(testTime, random_vec);
   printTiming(testTime, "Binary Tree");

   hash_test(testTime, random_vec);
   printTiming(testTime, "Hash Table");

   array_test(testTime, random_vec, true); // with binary_search

   printTiming(testTime, "Sorted Vector");

   cout << "Done." << endl;
   return 0;
}
