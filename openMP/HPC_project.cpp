#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <omp.h>
#include<math.h>
#include <cstdlib>
#include<time.h>
#include <chrono>

using namespace std;
void append_to_csv(const std::string& filename, const std::vector<std::vector<double>>& data) {
    std::ofstream outfile(filename, std::ios_base::app); // open the file in append mode
    if (outfile.is_open()) {
        for (const auto& row : data) {
            int i=0;
            for (const auto& val : row) {
                if(i!=40)
                    outfile << val << ",";
                else
                    outfile << val ;
                i++;
            }
            outfile << "\n";
        }
        outfile.close();
    } else {
        std::cout << "Error: unable to open file " << filename << " for writing." << std::endl;
    }
}

std::vector<std::vector<double>> read_csv(const std::string& filename) {
    std::vector<std::vector<double>> data;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::vector<double> row;
        double val;
        char delimiter;
        while (iss >> val) {
            row.push_back(val);
            if (iss.peek() == ',' || iss.peek() == ';') {
                iss >> delimiter;
            }
        }
        data.push_back(row);
    }
    return data;
}


void quicksort(std::vector<double>& data, std::vector<int>& indices, int left, int right) {
    if (left < right) {
        double pivot = data[left];
        int i = left, j = right;

        while (i <= j) {
            while (data[i] < pivot) i++;
            while (data[j] > pivot) j--;

            if (i <= j) {
                std::swap(data[i], data[j]);
                std::swap(indices[i], indices[j]);
                i++; j--;
            }
        }

        #pragma omp task
        quicksort(data, indices, left, j);
        #pragma omp task
        quicksort(data, indices, i, right);
    }
}

void parallel_quicksort(std::vector<double>& data, std::vector<int>& indices) {
    #pragma omp parallel
    {
        #pragma omp single
        quicksort(data, indices, 0, data.size() - 1);
    }
}

int main() {
    int percentIncrease=400;
    int p=percentIncrease/100;
    std::string filename = "output.csv";
    std::vector<std::vector<double>> data = read_csv(filename);
    vector<vector<double>> s;
    std::cout.precision(12);
    int N=data.size();
    int nn=4;
    vector<vector<double>> distance(N,vector<double>(N,0.0));
    auto start = std::chrono::high_resolution_clock::now();
     
    #pragma omp parallel num_threads(8)
    {
        #pragma omp for
        for ( int i = 0; i < N; i++)
        {
            #pragma omp parallel for num_threads(8)
            
                for(int j=i+1;j<N;j++){
                    double sum=0.0;
                    #pragma omp parallel for num_threads(8) reduction(+:sum) 
                        for(int k=0;k<41;k++){
                            // cout<<(data[i][k]-data[j][k])*(data[i][k]-data[j][k])<<endl;
                            sum+=(data[i][k]-data[j][k])*(data[i][k]-data[j][k]);
                            // cout<<sum<<endl;
                        }
                    // cout<<pow(sum,0.5)<<endl;
                    distance[i][j]=pow(sum,0.5);
                    distance[j][i]=pow(sum,0.5);
                }
            // cout<<distance[i][0]<<endl;
            vector<int> indices(distance[i].size());
            // cout<<"start\n";
            for (int x = 0; x < distance[i].size(); x++) {
                // cout<<distance[i][x]<<" ";
                indices[x] = x;
            }
            // cout<<endl;
            // cout<<"starti\n";
            parallel_quicksort(distance[i], indices);
            // random generator
            srand((unsigned)time(NULL)+omp_get_thread_num());
            #pragma omp parallel for num_threads(8)
            for(int t=0;t<p;t++){
                int j = rand() % (nn + 1);
                // cout<<j<<endl;
                if(indices[j]!=i){
                    vector<double> synthetic_instance(41,0);
                    #pragma omp parallel for num_threads(8)
                        for(int k=0;k<41;k++){// Initialize random seed
                            double random_double = (double)(rand()) /RAND_MAX;
                            
                            // cout<<random_double<<endl;
                            // // cout<<random_double<<endl;
                            // if(indices.size()<=j){
                            //     cout<<"h";
                            // }
                            double diff=data[indices[j]][k]-data[i][k];
                            synthetic_instance[k]=data[i][k]+random_double*diff;
                            
                        }
                    #pragma omp critical
                        s.push_back(synthetic_instance);
                        
                }
            }
        }
        
    }
     auto stop = std::chrono::high_resolution_clock::now();
    
    // calculate the duration in milliseconds
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;
    cout<<s.size();
    std::string filenam = "output.csv";
    append_to_csv(filenam,s);

    // for (auto row : data) {
    //     for (auto cell : row) {
    //         cout << cell << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}




