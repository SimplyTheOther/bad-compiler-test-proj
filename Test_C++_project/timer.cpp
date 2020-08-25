#include <iostream> 
#include <chrono> 
using namespace std::chrono; 

std::vector<std::vector<int>> subsetSums(int sumTarget, std::vector<int> values) {
    std::vector<std::vector<int>> subsetSums;
        
    int maxComparison = (int)Math.pow(2, values.length);
    // maxComparison looks like: 000000011111
        
    for (int i = 0; i < maxComparison; ++i) {
        int currentSet = i;
        int sum = 0;
	std::vector<int> subset;
	subset.reserve(values.size());

        for (int counter = 0; counter < values.length; ++counter) {
            if (((currentSet >> counter) & 1) == 1) {
                sum += values[counter];
                subset.push_back(counter);
            }
        }
        if (sum == sumTarget)
            subsetSums.push_back(std::move(subset));
    }
    return subsetSums;
}

int main() {
  using namespace std;
  auto start = high_resolution_clock::now();
  
  long sum = 0;
  long i;
  for (i = 0; i < 1000000; i++) {
    sum = sum + i;
  }

  auto stop = high_resolution_clock::now(); 
  auto duration = duration_cast<microseconds>(stop - start);
  cout << sum << endl; 
  cout << duration.count() << endl; 
  
  return 0;
} 