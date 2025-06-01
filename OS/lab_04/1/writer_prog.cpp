#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

#define SHM_NAME L"SharedBuffer"
#define MUTEX_NAME L"WritersMutex"
#define SEM_NAME L"ReadersSemaphore"
// 331103 = 3+1+1+3
#define PAGE_COUNT 8
#define PAGE_SIZE 4096
#define TOTAL_SIZE (4 + PAGE_COUNT * PAGE_SIZE)
#define LOG_FMT "logs_writers/writer_%02d.log"

#define COUNTER_OFFSET 0
#define BUFFER_OFFSET 4

using namespace std;


BYTE* page_base = nullptr;
FILE* log_file = nullptr;

HANDLE h_map = nullptr;
HANDLE h_mutex = nullptr;
HANDLE h_reader_semaphore = nullptr;

int writer_id = -1;

void endup() {
    if (log_file) fclose(log_file);
    if (page_base) UnmapViewOfFile(page_base - BUFFER_OFFSET);
    if (h_map) CloseHandle(h_map);
    if (h_mutex) CloseHandle(h_mutex);
    if (h_reader_semaphore) CloseHandle(h_reader_semaphore);
}

void sync_objects_init() {
    h_mutex = CreateMutexW(NULL, FALSE, MUTEX_NAME);
    if (!h_mutex) {
        cout << "CreateMutex failed";
        exit(1);
    }

    h_reader_semaphore = CreateSemaphoreW(NULL, 1, 1, SEM_NAME);

    if (!h_reader_semaphore) {
        cout << "CreateSemaphore failed";
        exit(1);
    }
}

void shared_memory_init() {
    h_map = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TOTAL_SIZE, SHM_NAME);

    if (!h_map) {
        cout << "CreateFileMapping failed";
        exit(1);
    }

    BYTE* base = (BYTE*)MapViewOfFile(h_map, FILE_MAP_ALL_ACCESS, 0, 0, TOTAL_SIZE);
    if (!base) {
        cout << "MapViewOfFile failed";
        exit(1);
    }

    if (GetLastError() != ERROR_ALREADY_EXISTS) {
        memset(base, 0, TOTAL_SIZE);
    }

    page_base = base + BUFFER_OFFSET;
}

void init_log(int id) {
    char filename[64];
    snprintf(filename, sizeof(filename), LOG_FMT, id);

    log_file = fopen(filename, "w");
    if (!log_file) {
        cout << "Failed to init log file";
        exit(1);
    }
}

void log_event(const char* state, int page_index, DWORD timestamp) {
    if (!log_file) return;

    if (page_index >= 0)
        fprintf(log_file, "WRITER_%02d %s PAGE_%d %lu\n", writer_id, state, page_index, timestamp);
    else
        fprintf(log_file, "WRITER_%02d %s %lu\n", writer_id, state, timestamp);

    fflush(log_file);
}

void write_operation() {
    int page_index = -1;

    for (int attempt = 0; attempt < PAGE_COUNT * 2; ++attempt) {
        int candidate = rand() % PAGE_COUNT;
        BYTE* page = page_base + candidate * PAGE_SIZE;

        if (page[0] == 0) {
            page_index = candidate;
            break;
        }
    }

    if (page_index == -1) {
        SleepEx(10, FALSE);
        return;
    }

    DWORD t_wait = GetTickCount();
    log_event("WAIT_WRITE", -1, t_wait);

    WaitForSingleObject(h_mutex, INFINITE);

    VirtualLock(page_base + page_index * PAGE_SIZE, PAGE_SIZE);

    DWORD t_write = GetTickCount();
    log_event("WRITE", page_index, t_write);

    BYTE* target = page_base + page_index * PAGE_SIZE;
    target[0] = 1;

    char content[16] = {0};
    sprintf(content, "hello %02d", writer_id);
    memcpy(target + 1, content, strlen(content) + 1);

    VirtualUnlock(h_map, PAGE_SIZE * page_index);
    SleepEx(5 + rand() % 10, TRUE);

    DWORD t_release = GetTickCount();
    log_event("RELEASE", -1, t_release);

    ReleaseMutex(h_mutex);
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "count of args is not correct";
        return 1;
    }

    writer_id = atoi(argv[1]);
    srand((unsigned int)(time(NULL) + writer_id));

    init_log(writer_id);
    shared_memory_init();
    sync_objects_init();

    for (int i = 0; i < 5; ++i) {
        write_operation();
    }

    endup();
    printf("writer %d finished\n", writer_id);
    return 0;
}
