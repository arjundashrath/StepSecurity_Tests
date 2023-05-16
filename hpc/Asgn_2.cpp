//Import header files
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Standard Bubble Sort
void standard_bubble_sort(vector<int>& arr) {
    int n = arr.size();

    //Iterate until second last element, to make passes
    for (int i = 0; i < n - 1; i++) {
        //for every pass, swap in terms of order
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort using Odd-Even Transposition
void bubble_sort_odd_even(vector<int>& arr) {
    bool isSorted = false;
    while (!isSorted) {
        isSorted = true;
        #pragma omp parallel for
        //Iterate even elements until second last element, to make passes in increments of 2
        for (int i = 0; i < arr.size() - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }
        #pragma omp parallel for
        //Iterate odd elements until second last element, to make passes in increments of 2
        for (int i = 1; i < arr.size() - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
                isSorted = false;
            }
        }
    }
}

// Merge two subarrays
void merge(vector<int>& arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);

    //Categorize into left and right sub array
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = j = 0;
    k = l;

    //Merge the sorted sub arrays
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    //Marge the remaining elements
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

// Sequential Merge Sort
void merge_sort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Parallel Merge Sort
void parallel_merge_sort(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort(arr, 0, arr.size() - 1);
    }
}

int main() {
    vector<int> arr(10000); // Create a vector with 10000 elements
    default_random_engine rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(1, 10000);

    // Fill the vector with random values
    for (int i = 0; i < 10000; i++) {
        arr[i] = dist(rng);
    }

    //Make 4 instances of the same unsorted array
    vector<int> arr2 = arr, arr3 = arr, arr4 = arr;

    //Compare time
    auto start = high_resolution_clock::now();
    standard_bubble_sort(arr);
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Standard bubble sort time: " << duration.count() << " milliseconds" << endl;


    // Measure performance of parallel bubble sort using odd-even transposition
    start = high_resolution_clock::now();
    bubble_sort_odd_even(arr2);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "Parallel bubble sort using odd-even transposition time: " << duration.count() << " milliseconds" << endl;

    // Measure performance of sequential merge sort
    start = high_resolution_clock::now();
    merge_sort(arr3, 0, arr.size() - 1);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "Sequential merge sort time: " << duration.count() << " milliseconds" << endl;

    // Measure performance of parallel merge sort
    arr.resize(10000); // Reset the array size
    for (int i = 0; i < 10000; i++) {
        arr[i] = dist(rng);
    }

    //Compare time
    start = high_resolution_clock::now();
    parallel_merge_sort(arr4);
    end = high_resolution_clock::now();
    duration = duration_cast<milliseconds>(end - start);

    cout << "Parallel merge sort time: " << duration.count() << " milliseconds" << endl;

    return 0;
}

