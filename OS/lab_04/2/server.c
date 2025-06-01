#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIPE_NAME "\\\\.\\pipe\\PipeOS"
#define BUFFER_SIZE 4096

static HANDLE hPipe = INVALID_HANDLE_VALUE;
static HANDLE hConnectEvent = NULL;
static OVERLAPPED ovWrite = {0};

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

void create_pipe() {
    if (hPipe != INVALID_HANDLE_VALUE) {
        printf("Pipe already created\n");
        return;
    }
    hPipe = CreateNamedPipeA(PIPE_NAME, PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, BUFFER_SIZE, BUFFER_SIZE, 0, NULL);
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("CreateNamedPipe error: %lu\n", GetLastError());
    } else {
        printf("Pipe created\n");
    }
}

void wait_for_client() {
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Pipe not created\n");
        return;
    }
    hConnectEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    OVERLAPPED ov = {0};
    ov.hEvent = hConnectEvent;

    if (!ConnectNamedPipe(hPipe, &ov)) {
        DWORD err = GetLastError();
        if (err == ERROR_IO_PENDING) {
            printf("Waiting for client...\n");
            WaitForSingleObject(hConnectEvent, INFINITE);
            printf("Client connected\n");
        } else if (err == ERROR_PIPE_CONNECTED) {
            printf("Client already connected\n");
        } else {
            printf("ConnectNamedPipe error: %lu\n", err);
        }
    }
}

void write_message() {
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("No pipe available\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    printf("Enter message to send: ");
    fgets(buffer, sizeof buffer, stdin);

    if (ovWrite.hEvent == NULL)
        ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    DWORD bytesWritten = 0;
    if (!WriteFile(hPipe, buffer, (DWORD)strlen(buffer), NULL, &ovWrite)) {
        if (GetLastError() != ERROR_IO_PENDING) {
            printf("WriteFile error: %lu\n", GetLastError());
            return;
        }
    }

    printf("Writing asynchronously, waiting for completion...\n");
    WaitForSingleObject(ovWrite.hEvent, INFINITE);
    GetOverlappedResult(hPipe, &ovWrite, &bytesWritten, FALSE);
    printf("Written %lu bytes\n", bytesWritten);
    ResetEvent(ovWrite.hEvent);
}

void disconnect_client() {
    if (DisconnectNamedPipe(hPipe)) {
        printf("Client disconnected\n");
    } else {
        printf("DisconnectNamedPipe error: %lu\n", GetLastError());
    }
}

void cleanup() {
    if (hPipe != INVALID_HANDLE_VALUE) CloseHandle(hPipe);
    if (hConnectEvent) CloseHandle(hConnectEvent);
    if (ovWrite.hEvent) CloseHandle(ovWrite.hEvent);
}

int main() {
    int choice;
    do {
        printf("\nSERVER MENU\n");
        printf("1 - CREATE PIPE\n");
        printf("2 - WAIT FOR CLIENT\n");
        printf("3 - WRITE MESSAGE\n");
        printf("4 - DISCONNECT CLIENT\n");
        printf("0 - EXIT\n");
        choice = enter_integer("Choose option: ", 0, 4);

        switch (choice) {
            case 1: create_pipe(); break;
            case 2: wait_for_client(); break;
            case 3: write_message(); break;
            case 4: disconnect_client(); break;
            case 0: break;
            default: printf("Invalid option\n"); break;
        }
    } while (choice != 0);

    cleanup();
    return 0;
}
