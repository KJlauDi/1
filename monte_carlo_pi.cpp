#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;
using namespace chrono;

double estimate_pi_monte_carlo(long long n_points, long long& inside_circle) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_real_distribution<double> dis(-1.0, 1.0);
    
    for (long long i = 0; i < n_points; i++) {
        double x = dis(gen);
        double y = dis(gen);
        
        if (x*x + y*y <= 1.0) {
            inside_circle++;
        }
    }
    
    return 4.0 * inside_circle / n_points;
}

int main() {
    vector<long long> checkpoints = {
        1000000,
        10000000,
        100000000,
        1000000000,
        10000000000,
        100000000000
    };
    
    long long total_iterations = 100000000;
    const double true_pi = 3.141592653589793;
    
    system("mkdir results 2>nul");
    
    ofstream csv_file("results/pi_monte_carlo_results.csv");
    csv_file << "N,Pi_Estimate,Accuracy,Execution_Time_Sec,Time_Per_Point\n";
    csv_file.close();
    
    cout << "Початок обчислення π методом Монте-Карло" << endl;
    cout << string(70, '=') << endl;
    
    long long inside_circle = 0;
    size_t checkpoint_index = 0;
    
    auto start_time = high_resolution_clock::now();
    auto checkpoint_start_time = start_time;
    
    for (long long i = 1; i <= total_iterations; i++) {
        random_device rd;
        mt19937_64 gen(rd());
        uniform_real_distribution<double> dis(-1.0, 1.0);
        
        double x = dis(gen);
        double y = dis(gen);
        
        if (x*x + y*y <= 1.0) {
            inside_circle++;
        }
        
        if (checkpoint_index < checkpoints.size() && i == checkpoints[checkpoint_index]) {
            auto current_time = high_resolution_clock::now();
            duration<double> execution_time = current_time - checkpoint_start_time;
            
            double pi_estimate = 4.0 * inside_circle / i;
            double accuracy = abs(pi_estimate - true_pi);
            double time_per_point = execution_time.count() / checkpoints[checkpoint_index];
            
            csv_file.open("results/pi_monte_carlo_results.csv", ios::app);
            csv_file << checkpoints[checkpoint_index] << ","
                     << fixed << setprecision(10) << pi_estimate << ","
                     << accuracy << ","
                     << execution_time.count() << ","
                     << scientific << time_per_point << "\n";
            csv_file.close();
            
            cout << "\nN = " << checkpoints[checkpoint_index] << endl;
            cout << "  π оцінка: " << fixed << setprecision(10) << pi_estimate << endl;
            cout << "  Точність: " << accuracy << endl;
            cout << "  Час виконання: " << setprecision(2) << execution_time.count() << " сек" << endl;
            cout << "  Час на точку: " << scientific << time_per_point << " сек" << endl;
            cout << string(70, '-') << endl;
            
            checkpoint_start_time = current_time;
            checkpoint_index++;
        }
        
        if (i % 10000000 == 0) {
            cout << "Прогрес: " << i << " / " << total_iterations 
                 << " точок (" << fixed << setprecision(2) 
                 << (100.0 * i / total_iterations) << "%)" << endl;
        }
    }
    
    auto end_time = high_resolution_clock::now();
    duration<double> total_time = end_time - start_time;
    
    cout << "\n" << string(70, '=') << endl;
    cout << "Обчислення завершено!" << endl;
    cout << "Загальний час: " << fixed << setprecision(2) << total_time.count() << " секунд" << endl;
    cout << "Результати збережено у: results/pi_monte_carlo_results.csv" << endl;
    
    return 0;
}
