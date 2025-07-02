#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define PAGE_SIZE 4096
#define NUM_PAGES 9

const wchar_t *mutexName = L"SharedMemoryMutex";
const wchar_t *mapName = L"SharedMemoryMap";

typedef struct
{
    int page_numbers[NUM_PAGES];
    char data[NUM_PAGES][PAGE_SIZE];
    int readers_count;
} SharedMemory;

void log_event(HANDLE log_file, const char *state, int page)
{
    DWORD time = timeGetTime();
    char buffer[256];
    int len = sprintf(buffer, "[%lu] [%d] [WRITER] [%s]", time, GetCurrentProcessId(), state);
    if (page >= 0)
    {
        sprintf(buffer + len, " [PAGE %d]", page);
    }
    strcat(buffer, "\n");
    DWORD bytesWritten;
    WriteFile(log_file, buffer, strlen(buffer), &bytesWritten, NULL);
}

int main()
{
    srand((unsigned int)time(NULL));

    // Инициализация
    HANDLE hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
                                       sizeof(SharedMemory), mapName);
    if (hMapFile == NULL)
    {
        printf("CreateFileMapping error: %d\n", GetLastError());
        return 1;
    }
    
    // Открытие объектов синхронизации
    HANDLE IOMutex = OpenMutexW(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, mutexName);
    if (IOMutex == NULL)
    {
        printf("OpenMutex error: %d\n", GetLastError());
        return 1;
    }

    // Создание журнального файла
    char log_filename[50];
    sprintf(log_filename, "writer_%d.log", GetCurrentProcessId());
    HANDLE log_file = CreateFileA(log_filename, GENERIC_WRITE, FILE_SHARE_READ, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE writer_semaphor[NUM_PAGES];
    HANDLE reader_semaphor[NUM_PAGES];
    
    for (int i = 0; i < NUM_PAGES; i++)
    {
        wchar_t semaphoreName[50];
        swprintf(semaphoreName, 50, L"writeSemaphore #%d", i);
        writer_semaphor[i] = OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, semaphoreName);
        
        swprintf(semaphoreName, 50, L"readSemaphore #%d", i);
        reader_semaphor[i] = OpenSemaphoreW(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, FALSE, semaphoreName);
    }
    
    DWORD page = 0;
    // Ожидание
    log_event(log_file, "WAITING", -1);
    page = WaitForMultipleObjects(NUM_PAGES, writer_semaphor, FALSE, INFINITE);

    WaitForSingleObject(IOMutex, INFINITE);

    // Запись
    Sleep(500 + rand() % 1000);
    log_event(log_file, "WRITING", (int)page);

    // Освобождение
    ReleaseMutex(IOMutex);
    ReleaseSemaphore(reader_semaphor[page], 1, NULL);
    log_event(log_file, "RELEASING", -1);

    for (int i = 0; i < NUM_PAGES; i++)
    {
        CloseHandle(writer_semaphor[i]);
        CloseHandle(reader_semaphor[i]);
    }
    CloseHandle(IOMutex);
    CloseHandle(hMapFile);
    CloseHandle(log_file);
    return 0;
}
