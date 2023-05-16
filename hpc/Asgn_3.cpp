#include <bits/stdc++.h>
#include <vector>
#include <chrono>  // for measuring time

using namespace std;
using namespace chrono;

// Find the minimum value in the vector
int min_reduction(const vector<int>& arr) {
    int min_value = INT_MAX;
    // Use OpenMP to perform reduction
    #pragma omp parallel for reduction(min: min_value)
    //Iterate through array and keep updating minimum value
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
    }
    return min_value;
}

// Find the maximum value in the vector
int max_reduction(const vector<int>& arr) {
    int max_value = INT_MIN;
    // Use OpenMP to perform reduction
    #pragma omp parallel for reduction(max: max_value)
    //Iterate through array and keep updating maximum value
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    return max_value;
}

// Calculate the sum of all elements in the vector
int sum_reduction(const vector<int>& arr) {
    int sum = 0;
    // Use OpenMP to perform reduction
    #pragma omp parallel for reduction(+: sum)
    //Iterate through array to add values
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return sum;
}

// Calculate the average value of all elements in the vector
double average_reduction(const vector<int>& arr) {
    int sum = 0;
    // Use OpenMP to perform reduction
    #pragma omp parallel for reduction(+: sum)
    //Iterate through array to add values
    for (size_t i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return (double)sum / arr.size();
}

int min_non_parallel(const vector<int>& arr) {
    int min_value = INT_MAX;
    //Iterate through array and update minimum values
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] < min_value) {
            min_value = arr[i];
        }
    }
    return min_value;
}

int max_non_parallel(const vector<int>& arr) {
    int max_value = INT_MIN;
    //Iterate through array and update maximum values
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] > max_value) {
            max_value = arr[i];
        }
    }
    return max_value;
}

int sum_non_parallel(const vector<int>& arr) {
    int sum = 0;
    //Iterate through array to add values
    for (int i = 0; i < arr.size(); i++) {
        sum += arr[i];
    }
    return sum;
}

double average_non_parallel(const vector<int>& arr) {
    //Call sum function instead
    int sum = sum_non_parallel(arr);
    return static_cast<double>(sum) / arr.size();
}


int main() {
    vector<int> arr;

    int range = 100000;

    for(int i = 0; i < range; i++){
        arr.push_back(1+rand());
    }

    // Parallel reduction functions
    // Compare time
    auto start = high_resolution_clock::now();
    int min_parallel = min_reduction(arr);
    int max_parallel = max_reduction(arr);
    int sum_parallel = sum_reduction(arr);
    double average_parallel = average_reduction(arr);
    auto end = high_resolution_clock::now();
    auto duration_parallel = duration_cast<microseconds>(end - start);

    // Non-parallel functions
    // Compare time
    start = high_resolution_clock::now();
    int min_non_parallel_result = min_non_parallel(arr);
    int max_non_parallel_result = max_non_parallel(arr);
    int sum_non_parallel_result = sum_non_parallel(arr);
    double average_non_parallel_result = average_non_parallel(arr);
    end = high_resolution_clock::now();
    auto duration_non_parallel = duration_cast<microseconds>(end - start);

    // Print the results
    cout << "Parallel Reduction Functions:" << endl;
    cout << "Minimum value: " << min_parallel << endl;
    cout << "Maximum value: " << max_parallel << endl;
    cout << "Sum: " << sum_parallel << endl;
    cout << "Average: " << average_parallel << endl;
    cout << "Time taken: " << duration_parallel.count() << " microseconds" << endl;

    cout << "\nNon-Parallel Functions:" << endl;
    cout << "Minimum value: " << min_non_parallel_result << endl;
    cout << "Maximum value: " << max_non_parallel_result << endl;
    cout << "Sum: " << sum_non_parallel_result << endl;
    cout << "Average: " << average_non_parallel_result << endl;
    cout << "Time taken: " << duration_non_parallel.count() << " microseconds" << endl;

    return 0;
}
