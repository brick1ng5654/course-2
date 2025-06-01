#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_NAME "\\\\.\\pipe\\PipeOS"
#define BUFFER_SIZE 4096

static HANDLE hPipe = INVALID_HANDLE_VALUE;
static char buffer[BUFFER_SIZE];
static OVERLAPPED ovRead = {0};

int enter_integer(const char *message, int a, int b) {
    int number;
    char input[64];
    while (1) {
        printf("%s", message);
        if (!fgets(input, sizeof(input), stdin)) {
            printf("Input interrupted. Exiting...\n");
            exit(1);
        }
        if (sscanf(input, "%d", &number) == 1 && number >= a && number <= b) {
            return number;
        } else {
            printf("Invalid input. Enter a number in range [%d, %d]\n", a, b);
        }
    }
}

VOID CALLBACK read_completed(DWORD err, DWORD bytes, LPOVERLAPPED lpOv) {
    if (err == 0) {
        printf("\nRead %lu bytes: %.*s\n", bytes, bytes, buffer);
    } else {
        printf("ReadFileEx error: %lu\n", err);
    }
}

void connect_pipe() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        printf("Already connected\n");
        return;
    }
    hPipe = CreateFileA(PIPE_NAME, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("CreateFile error: %lu\n", GetLastError());
    } else {
        printf("Connected to server\n");
    }
}

void async_read() {
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Not connected\n");
        return;
    }
    if (ovRead.hEvent == NULL)
        ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    memset(buffer, 0, sizeof buffer);
    if (!ReadFileEx(hPipe, buffer, sizeof buffer, &ovRead, read_completed)) {
        printf("ReadFileEx error: %lu\n", GetLastError());
        return;
    }

    printf("Reading asynchronously. Press any key after receiving data\n");
    SleepEx(INFINITE, TRUE);
}

void cleanup() {
    if (hPipe != INVALID_HANDLE_VALUE) CloseHandle(hPipe);
    if (ovRead.hEvent) CloseHandle(ovRead.hEvent);
}

int main() {
    int choice;
    do {
        printf("\nCLIENT MENU\n");
        printf("1 - CONNECT TO PIPE\n");
        printf("2 - READ MESSAGE\n");
        printf("0 - EXIT\n");
        choice = enter_integer("Choose option: ", 0, 2);

        switch (choice) {
            case 1: connect_pipe(); break;
            case 2: async_read(); break;
            case 0: break;
            default: printf("Invalid option\n"); break;
        }
    } while (choice != 0);

    cleanup();
    return 0;
}
