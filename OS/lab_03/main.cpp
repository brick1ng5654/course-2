#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int N = 10000000;
const int BLOCK_SIZE = 3311030; // 331103 numb of stud bilet
const int TOTAL_BLOCKS = (N + BLOCK_SIZE - 1) / BLOCK_SIZE;

struct thread_data {
    int thread_id;
};

double pi = 0.0;
int current_block = 0;
CRITICAL_SECTION pi_cs;
CRITICAL_SECTION block_cs;

HANDLE* threads;
thread_data* thread_info;
int num_threads;

DWORD WINAPI compute_pi(LPVOID param) {
    thread_data* data = static_cast<thread_data*>(param);

    while (true) {
        SuspendThread(GetCurrentThread());

        int block_id;

        EnterCriticalSection(&block_cs);
        block_id = current_block++;
        LeaveCriticalSection(&block_cs);

        if (block_id >= TOTAL_BLOCKS) break;

        int start = block_id * BLOCK_SIZE;
        int end = min(start + BLOCK_SIZE, N);
        double local_sum = 0.0;

        for (int i = start; i < end; ++i) {
            double x = (i + 0.5) / N;
            local_sum += 4.0 / (1.0 + x * x);
        }

        EnterCriticalSection(&pi_cs);
        pi += local_sum;
        LeaveCriticalSection(&pi_cs);
    }
    return 0;
}

int main() {
    InitializeCriticalSection(&pi_cs);
    InitializeCriticalSection(&block_cs);

    vector<int> thread_counts = {1, 2, 4, 8, 12, 16};

    for (int count : thread_counts) {
        pi = 0.0;
        current_block = 0;
        num_threads = count;

        threads = new HANDLE[num_threads];
        thread_info = new thread_data[num_threads];

        for (int i = 0; i < num_threads; ++i) {
            thread_info[i].thread_id = i;
            threads[i] = CreateThread(NULL, 0, compute_pi, &thread_info[i], CREATE_SUSPENDED, NULL);
        }

        LARGE_INTEGER frequency, start, end;
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&start);

        for (int i = 0; i < num_threads; ++i) {
            ResumeThread(threads[i]);
        }

        bool all_done = false;
        while (!all_done) {
            all_done = true;
            for (int i = 0; i < num_threads; ++i) {
                DWORD code;
                GetExitCodeThread(threads[i], &code);
                if (code == STILL_ACTIVE) {
                    ResumeThread(threads[i]);
                    all_done = false;
                }
            }
        }

        WaitForMultipleObjects(num_threads, threads, TRUE, INFINITE);
        QueryPerformanceCounter(&end);

        double elapsed_time = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
        pi /= N;

        cout.precision(15);
        cout << "Threads: " << num_threads << " | PI equal " << pi << " | Time: " << fixed << elapsed_time << " sec" << endl;

        for (int i = 0; i < num_threads; ++i)
            CloseHandle(threads[i]);

        delete[] threads;
        delete[] thread_info;
    }

    DeleteCriticalSection(&pi_cs);
    DeleteCriticalSection(&block_cs);

    return 0;
}
