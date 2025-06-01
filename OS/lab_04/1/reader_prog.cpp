#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>
#include <iostream>

#define SHM_NAME L"SharedBuffer"
#define MUTEX_NAME L"WritersMutex"
#define SEM_NAME L"ReadersSemaphore"
// 331103 = 3+1+1+3
#define PAGE_COUNT 8
#define PAGE_SIZE 4096
#define LOG_NAME_FMT "logs_readers/reader_%02d.log"

#define READER_COUNTER_OFFSET 0
#define BUFFER_OFFSET 4

using namespace std;

volatile LONG* readers_count = nullptr;
BYTE* page_base = nullptr;

HANDLE h_writers_mutex = nullptr;
HANDLE h_readers_semaphore = nullptr;
HANDLE h_map = nullptr;
FILE* log_file = nullptr;

int reader_id = -1;

void endup() {
    if (log_file) fclose(log_file);
    if (page_base) UnmapViewOfFile(page_base - BUFFER_OFFSET);
    if (h_map) CloseHandle(h_map);
    if (h_writers_mutex) CloseHandle(h_writers_mutex);
    if (h_readers_semaphore) CloseHandle(h_readers_semaphore);
}

void shared_objects_init() {
    h_map = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, SHM_NAME);
    if (!h_map) {
        cout << "OpenFileMapping failed";
        exit(1);
    }

    BYTE* base = (BYTE*)MapViewOfFile(h_map, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!base) {
        cout << "MapViewOfFile failed";
        exit(1);
    }

    readers_count = (volatile LONG*)(base + READER_COUNTER_OFFSET);
    page_base = base + BUFFER_OFFSET;

    h_writers_mutex = OpenMutexW(SYNCHRONIZE, FALSE, MUTEX_NAME);
    if (!h_writers_mutex) {
        cout << "OpenMutex failed";
        exit(1);
    }

    h_readers_semaphore = OpenSemaphoreW(SYNCHRONIZE | SEMAPHORE_MODIFY_STATE, FALSE, SEM_NAME);
    if (!h_readers_semaphore) {
        cout << "OpenSemaphore failed";
        exit(1);
    }
}

void init_log(int id) {
    char filename[64];
    snprintf(filename, sizeof(filename), LOG_NAME_FMT, id);

    log_file = fopen(filename, "w");
    if (!log_file) {
        cout << "Failed to init log file";
        exit(1);
    }
}

void log_event(const char* state, int page_index, DWORD timestamp) {
    if (!log_file) return;

    if (page_index >= 0)
        fprintf(log_file, "READER_%02d %s PAGE_%d %lu\n", reader_id, state, page_index, timestamp);
    else
        fprintf(log_file, "READER_%02d %s %lu\n", reader_id, state, timestamp);

    fflush(log_file);
}

void read_operation() {
    int page_index = -1;
    char buffer[16] = {};

    for (int attempt = 0; attempt < PAGE_COUNT * 2; ++attempt) {
        int candidate = rand() % PAGE_COUNT;
        BYTE* page = page_base + candidate * PAGE_SIZE;
        memcpy(buffer, page, 9);

        if (buffer[0] == 1) {
            page_index = candidate;
            break;
        }
    }

    if (page_index == -1) {
        SleepEx(10, FALSE);
        return;
    }

    DWORD time_start = timeGetTime();
    log_event("WAIT_READ", -1, time_start);

    WaitForSingleObject(h_readers_semaphore, INFINITE);

    if (InterlockedIncrement(readers_count) == 1)
        WaitForSingleObject(h_writers_mutex, INFINITE);

    ReleaseSemaphore(h_readers_semaphore, 1, NULL);

    DWORD t_read = timeGetTime();
    log_event("READ", page_index, t_read);

    BYTE* page = page_base + page_index * PAGE_SIZE;
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, page, sizeof(buffer) - 1);

    fprintf(log_file, "READER_%02d READ_CONTENT PAGE_%d \"%s\"\n", reader_id, page_index, buffer);
    fflush(log_file);

    SleepEx(5 + rand() % 10, TRUE);

    DWORD t_release = timeGetTime();
    log_event("RELEASE", -1, t_release);

    WaitForSingleObject(h_readers_semaphore, INFINITE);

    if (InterlockedDecrement(readers_count) == 0)
        ReleaseMutex(h_writers_mutex);

    ReleaseSemaphore(h_readers_semaphore, 1, NULL);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "count of args is not correct";
        return 1;
    }

    reader_id = atoi(argv[1]);
    srand((unsigned int)(time(NULL) + reader_id));

    init_log(reader_id);
    shared_objects_init();

    for (int i = 0; i < 5; i++) {
        read_operation();
    }

    endup();
    printf("reader %d finished\n", reader_id);
    return 0;
}
