#include <windows.h>
#include <iostream>
#include <string>
using namespace std;

// Helpful functions ---------------------------

int enter_integer(string message, int a, int b) {
    string input;
    int number;

    while (true) {
        cout << message;
        cin >> input;
        try {
            number = stoi(input);
            if (number >= a && number <= b) break;
            else cout << "Entered value is not in the range [" << a << ", " << b << "]. Please try again!" << endl;
        } catch (...) {
            cout << "Error! Please try again" << endl;
        }
        cin.clear();
    }
    return number;
}

void clear_screen() {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

int main_menu() {
    cout << "\nOPTIONS:" << endl;
    cout << "0 - for EXIT" << endl;
    cout << "1 - for DRIVES" << endl;
    cout << "2 - for DIRECTORIES" << endl;
    cout << "3 - for FILES" << endl;
    return enter_integer("Choose option: ", 0, 3);
}

int drive_menu() {
    cout << "\nDRIVE OPTION:" << endl;
    cout << "0 - for MAIN MENU" << endl;
    cout << "1 - for OUTPUT DRIVES" << endl;
    cout << "2 - for OUTPUT DRIVE INFO" << endl;
    return enter_integer("Choose option: ", 0, 2);
}

int dir_menu() {
    cout << "\nDIRECTORIES OPTION:" << endl;
    cout << "0 - for MAIN MENU" << endl;
    cout << "1 - for CREATE DIRECTORY" << endl;
    cout << "2 - for REMOVE DIRECTORY" << endl;
    cout << "3 - for RENAME DIRECTORY" << endl;
    cout << "4 - for MOVE DIRECTORY" << endl;
    cout << "5 - for GET DIRECTORY ATTRIBUTES" << endl;
    cout << "6 - for SET DIRECTORY ATTRIBUTES" << endl;
    return enter_integer("Choose option: ", 0, 6);
}

int file_menu() {
    cout << "\nFILES OPTION:" << endl;
    cout << "0 - for MAIN MENU" << endl;
    cout << "1 - for CREATE FILE" << endl;
    cout << "2 - for OPEN FILE" << endl;
    cout << "3 - for REMOVE FILE" << endl;
    cout << "4 - for COPY FILE" << endl;
    cout << "5 - for RENAME FILE" << endl;
    cout << "6 - for MOVE FILE" << endl;
    cout << "7 - for GET FILE ATTRIBUTES" << endl;
    cout << "8 - for SET FILE ATTRIBUTES" << endl;
    cout << "9 - for GET FILE INFORMATION BY HANDLE" << endl;
    cout << "10 - for SET FILE TIME" << endl;
    return enter_integer("Choose option: ", 0, 10);
}

void wait_reaction(){
    cout << "\nEnter any key to continiue" << endl;
    cin.ignore();
    getchar();
}

string get_current_dir(){
    wchar_t buffer[MAX_PATH];
    DWORD len = GetCurrentDirectoryW(MAX_PATH, buffer);
    if (len==0) return "";
    else{
        wstring wide_str(buffer);
        string narrow_str(wide_str.begin(), wide_str.end());
        return narrow_str;
    }
}

wstring string_to_wstring(const string& str) {
    int size_needed = MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), NULL, 0);
    wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
    return wstr;
}

string wstring_to_string(const wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    string str(size_needed, 0);
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, NULL, NULL);
    return str;
}

wstring function_in_path(string msg_to_path="Function will be executed in that path: ", string msg_to_enter="Enter the name: "){
    string path = get_current_dir();
    string name;
    cout << "\n" << msg_to_path << path << "\\" << endl;
    cout << msg_to_enter;
    cin >> name;

    wstring wide_path = string_to_wstring(path) + L"\\" + string_to_wstring(name);
    return wide_path;
}

// Drives Functions ---------------------------

DWORD get_logical_drives(){
    DWORD drives_bm = GetLogicalDrives();
    int count=1;
    cout << "\nAVAILABLE DRIVES:\n";
    for (int i=0; i<sizeof(DWORD)*8; i++){
        if (drives_bm & (1<<i)){
            char drive = 'A'+i;
            cout << " " << count << " - " << drive << ":\\\n";
            count++;
        }
    }
    return drives_bm;
}

void output_drive_info(){
    // Drive select
    wchar_t disk;
    wchar_t path[4];
    int option, counter=0;
    DWORD options = get_logical_drives();
    for (int i=0; i<sizeof(DWORD)*8; i++){
        if (options & 1<<i) counter++;
    }
    option = enter_integer("Enter the number of drive: ", 1, counter);

    counter = 1;
    for (int i=0; i<sizeof(DWORD)*8; i++){
        if (options & 1<<i){
            if (counter==option){
                disk = 'A'+i;
                break;
            } else counter++;
        }
    }

    cout << "\nInfo about drive '" << (char)disk << ":\\\':" << endl;
    swprintf(path, 4, L"%c:\\", disk);
    // Drive info
    int drive_type = GetDriveTypeW(path);
    switch (drive_type){
    case 0: cout << "Unknown type of drive" << endl; break;
    case 1: cout << "Incorrect root" << endl; break;
    case 2: cout << "Drive is removable" << endl; break;
    case 3: cout << "Drive is fixed" << endl; break;
    case 4: cout << "Drive is remotable" << endl; break;
    case 5: cout << "Drive is CD-ROM" << endl; break;
    case 6: cout << "Drive is RAM disk" << endl; break;
    }
    
    wchar_t volume_name[MAX_PATH], file_system_name[MAX_PATH];
    DWORD serial_number, max_length;
    if (GetVolumeInformationW(path, volume_name,sizeof(volume_name), &serial_number, &max_length, NULL, file_system_name, sizeof(file_system_name))){
        wcout << "Volume Name: " << volume_name << endl;
        cout << "Serial Number: " << serial_number << endl;
        cout << "Max Component Length: " << max_length << endl;
        wcout << "File System Name: " << file_system_name << endl;
    } else cout << "Error! Please try again";

    DWORD sector_per_cluster, bytes_per_sector, numb_free_cluster, numb_total_cluster;
    GetDiskFreeSpaceW(path, &sector_per_cluster, &bytes_per_sector, &numb_free_cluster, &numb_total_cluster);
    ULONGLONG total = (ULONGLONG)numb_total_cluster*sector_per_cluster*bytes_per_sector;
    ULONGLONG free = (ULONGLONG)numb_free_cluster*sector_per_cluster*bytes_per_sector;
    cout << "Total space on drive: " << total/1e9 << " GB" << endl;
    cout << "Free space on drive: " << free/1e9 << " GB"<<endl;
}

// Directory Functions --------------------------

void create_dir(){
    wstring wide_path = function_in_path("Directory will be created in ", "Enter the name of directory: ");
    if (CreateDirectoryW(wide_path.c_str(), NULL)){
        cout << "Directory successfull created" << endl;
    } else cout << "Error! Directory is not created" << endl;
}

void remove_dir(){
    wstring wide_path = function_in_path("The directory to be deleted will be searched in ", "Enter the name of directory: ");
    if (RemoveDirectoryW(wide_path.c_str())){
        cout << "Directory is successfull removed" << endl;
    } else cout << "Error! Directory is not removed" << endl;
}

// File Funtcions --------------------------------

void create_file(int action_mode_choise){ // 0 - to create file, 1 - to open file
    int access_mode_choise, share_mode_choise;
    DWORD access_mode, share_mode, action_mode;

    wstring wide_path;
    if (action_mode_choise==0) wide_path = function_in_path("File will be created in ", "Enter the name of file (Do not forget about extension): ");
    else {
        wide_path = function_in_path("Fill will be opened in ", "Enter the name of file (Do not forget about extension: ");
    }

    cout << "\nACCESS MODE\n1 - only for READ\n2 - only for WRITE\n3 - for READ and WRITE\n";
    access_mode_choise = enter_integer("Choose access mode: ", 1, 3);
    switch(access_mode_choise){
        case 1: access_mode = GENERIC_READ; break;
        case 2: access_mode = GENERIC_WRITE; break;
        case 3: access_mode = GENERIC_READ | GENERIC_WRITE; break;
    }

    cout << "\nSHARE MODE\n1 - for NOT ALLOWED\n2 - for READ\n3 - for WRITE\n4 - for DELETE\n";
    share_mode_choise = enter_integer("Choose share mode: ", 1, 4);
    switch(share_mode_choise){
        case 1: share_mode= 0; break;
        case 2: share_mode= FILE_SHARE_READ; break;
        case 3: share_mode= FILE_SHARE_WRITE; break;
        case 4: share_mode= FILE_SHARE_DELETE; break;
    }
    
    if (action_mode_choise==0) action_mode = CREATE_NEW;
    else action_mode = OPEN_EXISTING;

    HANDLE new_file = CreateFileW(wide_path.c_str(), access_mode, share_mode, NULL, action_mode, FILE_ATTRIBUTE_NORMAL, NULL);
    if (new_file == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (error == ERROR_FILE_EXISTS) {
            cout << "\nFile with equal name exists. Do you want to rewrite?\n1 - YES\n2 - NO\nYour choice: ";
            int rewrite_choise;
            cin >> rewrite_choise;
            if (rewrite_choise == 1){
                if (action_mode_choise==0) action_mode = CREATE_ALWAYS;
                else action_mode = OPEN_ALWAYS;

                new_file = CreateFileW(wide_path.c_str(), access_mode, share_mode, NULL, action_mode, FILE_ATTRIBUTE_NORMAL, NULL);
                if (new_file == INVALID_HANDLE_VALUE) {
                    cout << "\nError! Failed to rewrite file" << endl;
                } else {
                    cout << "\nFile successfully rewritten" << endl;
                    CloseHandle(new_file);
                }
            } else cout << "File not rewritten." << endl;
        } else {
            if (error!=0){
                cout << "Error! Please try again";
            }
        }
    } else {
        if (action_mode_choise==0){
            cout << "\nFile successfully created!" << endl;
        } else cout << "\nFile successfully opened!" << endl;
        CloseHandle(new_file);
    }
}

void remove_file(){
    wstring wide_path = function_in_path("File will deleted in ", "Enter the name of file (Do not forget about extension): ");
    if(DeleteFileW(wide_path.c_str())){
        cout << "\nFile was successfully removed\n";
    } else cout << "Error! File was not removed";
}

void copy_file(){
    wstring wide_path = function_in_path("File will be copied from ", "Etmer the name of file to copy (Do not forget about extension): ");
    wstring wide_repath = function_in_path("File will be pasted in ", "Enter the new name of copied file: ");

    if(CopyFileW(wide_path.c_str(), wide_repath.c_str(), TRUE)){
        cout << "\nFile was successfully copied\n";
    } else {
        cout << "\nFile with equal name exists. Do you want to rewrite?\n1 - YES\n2 - NO\nYour choice: ";
        int rewrite_choise;
        cin >> rewrite_choise;
        if (rewrite_choise == 1){
            if(CopyFileW(wide_path.c_str(), wide_repath.c_str(), FALSE)){
                cout << "File was successfully copied";
            }
            else cout << "Error! Please try again";
        }
    }
}

void rename(int is_dir){
    wstring wide_path, wide_repath;
    if(is_dir==0){
        wide_path = function_in_path("File to rename fill be from ", "Enter the name of file to rename (Do not forget about extension): ");
        wide_repath = function_in_path("Renamed file will keeped in ", "Enter the new name of file: ");
    } else {
        wide_path = function_in_path("Directory to rename fill be from ", "Enter the name of directory to rename: ");
        wide_repath = function_in_path("Renamed directory will keeped in ", "Enter the new name of directory: ");
    }
    
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        if (is_dir==0) cout << "\nFile was successfullt renamed";
        else cout << "\nDirectory was successfullt renamed";
    } else {
        if (is_dir==0) cout << "\nFile was not renamed";
        else cout << "\nDirectory was not renamed";
    }
}

void move(int is_dir){
    string path = get_current_dir();
    string name, repath;
    if (is_dir==0) {
        cout << "\nFile will be moved according to this path: " << path << "\\" << endl;
        cout << "Enter the name of file that will be moved (Do not forget about extension): ";
    }
    else {
        cout << "\nDirectory will be moved according to this path: " << path << "\\" << endl;
        cout << "Enter the name of directory that will be moved: ";
    }

    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    cout << "Specify a relative path to move: ";
    cin >> repath;
    wstring wide_repath = wstring(path.begin(), path.end()) + L"\\"+ wstring(repath.begin(), repath.end()) + L"\\"+ wstring(name.begin(), name.end());

    
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        cout << "File was successfullt moved";
    } else {
        if (GetLastError() == ERROR_ALREADY_EXISTS){
            int replace = enter_integer("File with equal name exist. Are you want to replace it? (0 - for NO / 1 - for YES):",0, 1);
            if (replace==0) cout << "Operation of moving is canceled";
            else MoveFileExW(wide_path.c_str(), wide_repath.c_str(), 0x01);
        }
    }
}

void print_file_attr(DWORD attributes) {
    cout << "FILE ATTRIBUTES:" << endl;

    if (attributes & FILE_ATTRIBUTE_READONLY) cout << "- Only for reading" << endl;
    if (attributes & FILE_ATTRIBUTE_HIDDEN) cout << "- Hidden" << endl;
    if (attributes & FILE_ATTRIBUTE_SYSTEM) cout << "- System file" << endl;
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) cout << "- Directory" << endl;
    if (attributes & FILE_ATTRIBUTE_ARCHIVE) cout << "- Archive" << endl;
    if (attributes & FILE_ATTRIBUTE_NORMAL) cout << "- Default file" << endl;
    if (attributes & FILE_ATTRIBUTE_TEMPORARY) cout << "- Temporary file" << endl;
    if (attributes & FILE_ATTRIBUTE_COMPRESSED) cout << "- Compressed" << endl;
    if (attributes & FILE_ATTRIBUTE_ENCRYPTED) cout << "- Encrypted" << endl;
    if (attributes & FILE_ATTRIBUTE_VIRTUAL) cout << "- Virtual file" << endl;
}

void get_file_attr(int is_dir){
    wstring wide_path;
    if(is_dir==0){
        wide_path = function_in_path("File for get attributes will be taken from ", "Enter the name of file which attributes will be shown (Do not forget about extension): ");
    } else {
        wide_path = function_in_path("Directory for get attributes will be taken from ", "Enter the name of directory which attributes will be shown: ");
    }
    DWORD attributes = GetFileAttributesW(wide_path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) cout << "Error! Please try again";
    else print_file_attr(attributes);
}

void set_file_attr(int is_dir){
    wstring wide_path;
    if (is_dir==0){
        wide_path = function_in_path("File for set attributes will be taken from ", "Enter the name of file which attributes will be setted (Do not forget about extension): ");
    } else {
        wide_path = function_in_path("Directory for set attributes will be taken from ", "Enter the name of directory which attributes will be setted: ");
    }

    DWORD attributes = GetFileAttributesW(wide_path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        cout << "\nError! Please try again\n";
        return;
    }
    cout << "\nFor each attribute enter 0 (No) or 1 (Yes) to set it\n";
    int choice;
    cout << "\nOnly for reading? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_READONLY;
    else attributes &= ~FILE_ATTRIBUTE_READONLY;

    cout << "Hidden? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_HIDDEN;
    else attributes &= ~FILE_ATTRIBUTE_HIDDEN;

    cout << "System file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_SYSTEM;
    else attributes &= ~FILE_ATTRIBUTE_SYSTEM;

    cout << "Archived? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_ARCHIVE;
    else attributes &= ~FILE_ATTRIBUTE_ARCHIVE;

    cout << "Temporary file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_TEMPORARY;
    else attributes &= ~FILE_ATTRIBUTE_TEMPORARY;

    cout << "Compressed file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_COMPRESSED;
    else attributes &= ~FILE_ATTRIBUTE_COMPRESSED;

    cout << "Encrypted file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_ENCRYPTED;
    else attributes &= ~FILE_ATTRIBUTE_ENCRYPTED;

    if (SetFileAttributesW(wide_path.c_str(), attributes)) {
        cout << "\nAttributes were successfully set\n";
    } else cout << "Error! Attributes were not setted. Please try again";
}

void print_filetime(const FILETIME& ft) {
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    cout << st.wDay << "." << st.wMonth << "." << st.wYear << " ";
    cout << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
}

FILETIME systime_to_filetime(const SYSTEMTIME& st) {
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
    return ft;
}

void get_file_info_by_handle(){
    wstring wide_path = function_in_path("File for get file information by handle will be taken from ", "Enter the name of file which attributes will be shown: ");

    HANDLE handle_file = CreateFileW(wide_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle_file == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (error = ERROR_FILE_NOT_FOUND){
            cout << "Error! File to get info is not exist" << endl;
        }
        if (error == ERROR_ACCESS_DENIED) {
            cout << "Access denied. You do not have permission to open this file" << endl;
        } else if (error == ERROR_SHARING_VIOLATION) {
            cout << "File is being used by another process and cannot be opened" << endl;
        } else {
            cout << "Failed to open file" << endl;
        }
        return;
    }

    BY_HANDLE_FILE_INFORMATION file_info;
    FILETIME creation_time, laccess_time, lwrite_time;

    if (GetFileInformationByHandle(handle_file, &file_info)){
        print_file_attr(file_info.dwFileAttributes);

        if (GetFileTime(handle_file, &creation_time, &laccess_time, &lwrite_time)){
            cout << "Creation time: ";
            print_filetime(creation_time);
    
            cout << "Last access time: ";
            print_filetime(laccess_time);
    
            cout << "Last write time: ";
            print_filetime(lwrite_time);
        } else {
            cout << "Error! Cant get file time" << endl;
        }

        cout << "Serial number of volume that contains that file: " << file_info.dwVolumeSerialNumber << endl;

        cout << "Number of links on that file: " << file_info.nNumberOfLinks << endl;
    }
    CloseHandle(handle_file);
}

void set_file_time(){
    wstring wide_path = function_in_path("File for set file time will be taken from ", "Enter the name of file which file time will be setted (Do not forget about extension): ");

    HANDLE handle_file = CreateFileW(wide_path.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle_file == INVALID_HANDLE_VALUE) {
        cout << "Error! Please try again" << endl;
        return;
    }

    SYSTEMTIME st;
    FILETIME ftCreation, ftLastAccess, ftLastWrite;

    int choice = enter_integer("Set new creation time? (1 - yes, 0 - no): ",0, 1);
    if (choice) {
        cout << "Enter creation date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftCreation = systime_to_filetime(st);
    } else {
        ftCreation.dwLowDateTime = 0;
        ftCreation.dwHighDateTime = 0;
    }

    cout << "Set new last access time? (1 - yes, 0 - no): ";
    cin >> choice;
    if (choice) {
        cout << "Enter last access date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftLastAccess = systime_to_filetime(st);
    } else {
        ftLastAccess.dwLowDateTime = 0;
        ftLastAccess.dwHighDateTime = 0;
    }

    cout << "Set new last write time? (1 - yes, 0 - no): ";
    cin >> choice;
    if (choice) {
        cout << "Enter last write date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftLastWrite = systime_to_filetime(st);
    } else {
        ftLastWrite.dwLowDateTime = 0;
        ftLastWrite.dwHighDateTime = 0;
    }

    if (SetFileTime(handle_file, (ftCreation.dwLowDateTime == 0 && ftCreation.dwHighDateTime == 0) ? NULL : &ftCreation, (ftLastAccess.dwLowDateTime == 0 && ftLastAccess.dwHighDateTime == 0) ? NULL : &ftLastAccess, (ftLastWrite.dwLowDateTime == 0 && ftLastWrite.dwHighDateTime == 0) ? NULL : &ftLastWrite)) {
        cout << "File time successfully updated" << endl;
    } else cout << "Error! File time was not updated" << endl;

    CloseHandle(handle_file);
}

// main -----------------------------------------

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int option;
    do {
        option = main_menu();
        switch (option) {
            case 1: {
                int drive_option;
                do {
                    drive_option = drive_menu();
                    switch(drive_option){
                    case 0: break;
                    case 1: get_logical_drives(); wait_reaction(); break;
                    case 2: output_drive_info(); wait_reaction(); break;
                    }
                } while (drive_option != 0);
                break;
            }
            case 2: {
                int dir_option;
                do {
                    dir_option = dir_menu();
                    switch(dir_option){
                    case 0: break;
                    case 1: create_dir(); wait_reaction(); break;
                    case 2: remove_dir(); wait_reaction(); break;
                    case 3: rename(1); wait_reaction(); break;
                    case 4: move(1); wait_reaction(); break;
                    case 5: get_file_attr(1); wait_reaction(); break;
                    case 6: set_file_attr(1); wait_reaction(); break;
                    }
                } while (dir_option != 0);
                break;
            }
            case 3: {
                int file_option;
                do {
                    file_option = file_menu();
                    switch (file_option){
                    case 0: break;
                    case 1: create_file(0); wait_reaction(); break;
                    case 2: create_file(1); wait_reaction(); break;
                    case 3: remove_file(); wait_reaction(); break;
                    case 4: copy_file(); wait_reaction(); break;
                    case 5: rename(0); wait_reaction(); break;
                    case 6: move(0); wait_reaction(); break;
                    case 7: get_file_attr(0); wait_reaction(); break;
                    case 8: set_file_attr(0); wait_reaction(); break;
                    case 9: get_file_info_by_handle(); wait_reaction(); break;
                    case 10: set_file_time(); wait_reaction(); break;
                    }
                } while (file_option != 0);
                break;
            }
        }
    } while (option != 0);
    cout << "\nGoodbye!\n" << endl;
    return 0;
}
