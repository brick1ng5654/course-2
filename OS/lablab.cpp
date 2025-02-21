#include <windows.h>
#include <iostream>
#include <string>
#include <locale>
#include <fcntl.h>
#include <io.h>

using namespace std;

// Helpful functions ---------------------------

int enter_integer(wstring message, int a, int b) {
    string input;
    int number;

    while (true) {
        wcout << message;
        cin >> input;
        try {
            number = stoi(input);
            if (number >= a && number <= b) break;
            else wcout << "Entered value is not in the range [" << a << ", " << b << "]. Please try again!" << endl;
        } catch (...) {
            wcout << "Error! Please try again" << endl;
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
    wcout << "\nOPTIONS:" << endl;
    wcout << "0 - for EXIT" << endl;
    wcout << "1 - for DRIVES" << endl;
    wcout << "2 - for DIRECTORIES" << endl;
    wcout << "3 - for FILES" << endl;
    return enter_integer(L"Choose option: ", 0, 3);
}

int drive_menu() {
    wcout << "\nDRIVE OPTION:" << endl;
    wcout << "0 - for MAIN MENU" << endl;
    wcout << "1 - for OUTPUT DRIVES" << endl;
    wcout << "2 - for OUTPUT DRIVE INFO" << endl;
    return enter_integer(L"Choose option: ", 0, 2);
}

int dir_menu() {
    wcout << "\nDIRECTORIES OPTION:" << endl;
    wcout << "0 - for MAIN MENU" << endl;
    wcout << "1 - for CREATE DIRECTORY" << endl;
    wcout << "2 - for REMOVE DIRECTORY" << endl;
    wcout << "3 - for RENAME DIRECTORY" << endl;
    wcout << "4 - for MOVE DIRECTORY" << endl;
    wcout << "5 - for GET DIRECTORY ATTRIBUTES" << endl;
    wcout << "6 - for SET DIRECTORY ATTRIBUTES" << endl;
    return enter_integer(L"Choose option: ", 0, 6);
}

int file_menu() {
    wcout << "\nFILES OPTION:" << endl;
    wcout << "0 - for MAIN MENU" << endl;
    wcout << "1 - for CREATE FILE" << endl;
    wcout << "2 - for OPEN FILE" << endl;
    wcout << "3 - for REMOVE FILE" << endl;
    wcout << "4 - for COPY FILE" << endl;
    wcout << "5 - for RENAME FILE" << endl;
    wcout << "6 - for MOVE FILE" << endl;
    wcout << "7 - for GET FILE ATTRIBUTES" << endl;
    wcout << "8 - for SET FILE ATTRIBUTES" << endl;
    wcout << "9 - for GET FILE INFORMATION BY HANDLE" << endl;
    wcout << "10 - for SET FILE TIME" << endl;
    return enter_integer(L"Choose option: ", 0, 10);
}

void wait_reaction(){
    wcout << "\nEnter any key to continiue" << endl;
    cin.ignore();
    getchar();
}

wstring get_current_dir(){
    wchar_t buffer[MAX_PATH];
    DWORD len = GetCurrentDirectoryW(MAX_PATH, buffer);
    if (len==0) return L"";
    else return wstring(buffer);
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

wstring function_in_path(wstring msg_to_path=L"Function will be executed in that path: ", wstring msg_to_enter=L"Enter the name: "){
    wstring path = get_current_dir();
    string name;
    wcout << L"\n" << msg_to_path << path << L"\\" << endl;
    wcout << msg_to_enter;
    cin >> name;
    wstring wide_path = path + L"\\" + string_to_wstring(name);
    wcout << "LOG " << wide_path;
    return wide_path;
}

// Drives Functions ---------------------------

DWORD get_logical_drives(){
    DWORD drives_bm = GetLogicalDrives();
    int count=1;
    wcout << "\nAVAILABLE DRIVES:\n";
    for (int i=0; i<sizeof(DWORD)*8; i++){
        if (drives_bm & (1<<i)){
            char drive = 'A'+i;
            wcout << " " << count << " - " << drive << ":\\\n";
            count++;
        }
    }
    return drives_bm;
}

void print_sys_flags(DWORD flags){
    wcout << "SYSTEM FLAGS:" << endl;
    if (flags & FILE_CASE_SENSITIVE_SEARCH) wcout << "- File sensetive tu uppercase and lowercase" << endl;
    if (flags & FILE_CASE_PRESERVED_NAMES) wcout << "- Retained registry of file names is supported" << endl;
    if (flags & FILE_UNICODE_ON_DISK) wcout << "- Unicode supported" << endl;
    if (flags & FILE_PERSISTENT_ACLS) wcout << "- ACL access supported" << endl;
    if (flags & FILE_FILE_COMPRESSION) wcout << "- File can be compressed" << endl;
    if (flags & FILE_VOLUME_QUOTAS) wcout << "- Disk quotas are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_SPARSE_FILES) wcout << "- Sparse files are supported on the volume" << endl;
    if (flags & FILE_SUPPORTS_REPARSE_POINTS) wcout << "- Reparse points are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_REMOTE_STORAGE) wcout << "- Remote storage is supported by the file system" << endl;
    if (flags & FILE_RETURNS_CLEANUP_RESULT_INFO) wcout << "- File system returns cleanup result info on successful cleanup" << endl;
    if (flags & FILE_SUPPORTS_POSIX_UNLINK_RENAME) wcout << "- POSIX-style unlink and rename operations are supported" << endl;
    if (flags & FILE_VOLUME_IS_COMPRESSED) wcout << "- The specified volume is a compressed volume" << endl;
    if (flags & FILE_SUPPORTS_OBJECT_IDS) wcout << "- Object identifiers are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_ENCRYPTION) wcout << "- Encrypted file system (EFS) is supported on the specified volume" << endl;
    if (flags & FILE_NAMED_STREAMS) wcout << "- Named streams are supported on the specified volume" << endl;
    if (flags & FILE_READ_ONLY_VOLUME) wcout << "- The specified volume is read-only" << endl;
    if (flags & FILE_SEQUENTIAL_WRITE_ONCE) wcout << "- Sequential write-once is supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_TRANSACTIONS) wcout << "- Transactions are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_HARD_LINKS) wcout << "- Hard links are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_EXTENDED_ATTRIBUTES) wcout << "- Extended attributes are supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_OPEN_BY_FILE_ID) wcout << "- Opening by FileID is supported by the file system" << endl;
    if (flags & FILE_SUPPORTS_USN_JOURNAL) wcout << "- Update Sequence Number (USN) journaling is supported on the specified volume" << endl;
    if (flags & FILE_SUPPORTS_INTEGRITY_STREAMS) wcout << "- Integrity streams are supported by the file system" << endl;
    if (flags & FILE_SUPPORTS_BLOCK_REFCOUNTING) wcout << "- Logical cluster sharing between files on the same volume is supported" << endl;
    if (flags & FILE_SUPPORTS_SPARSE_VDL) wcout << "- Sparse valid data length (VDL) tracking is supported by the file system" << endl;
    if (flags & FILE_DAX_VOLUME) wcout << "- The specified volume is a Direct Access (DAX) volume" << endl;
    if (flags & FILE_SUPPORTS_GHOSTING) wcout << "- Ghosting is supported by the file system" << endl;
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
    option = enter_integer(L"Enter the number of drive: ", 1, counter);

    counter = 1;
    for (int i=0; i<sizeof(DWORD)*8; i++){
        if (options & 1<<i){
            if (counter==option){
                disk = 'A'+i;
                break;
            } else counter++;
        }
    }

    wcout << "\nInfo about drive '" << (char)disk << ":\\\':" << endl;
    swprintf(path, 4, L"%c:\\", disk);
    // Drive info
    int drive_type = GetDriveTypeW(path);
    switch (drive_type){
    case 0: wcout << "Unknown type of drive" << endl; break;
    case 1: wcout << "Incorrect root" << endl; break;
    case 2: wcout << "Drive is removable" << endl; break;
    case 3: wcout << "Drive is fixed" << endl; break;
    case 4: wcout << "Drive is remotable" << endl; break;
    case 5: wcout << "Drive is CD-ROM" << endl; break;
    case 6: wcout << "Drive is RAM disk" << endl; break;
    }
    
    wchar_t volume_name[MAX_PATH], file_system_name[MAX_PATH];
    DWORD serial_number, max_length, sys_flags;
    if (GetVolumeInformationW(path, volume_name,sizeof(volume_name), &serial_number, &max_length, &sys_flags, file_system_name, sizeof(file_system_name))){
        wcout << "Volume Name: " << volume_name << endl;
        wcout << "Serial Number: " << serial_number << endl;
        wcout << "Max Component Length: " << max_length << endl;
        wcout << "File System Name: " << file_system_name << endl;
        print_sys_flags(sys_flags);
    } else wcout << "Error! Please try again";

    DWORD sector_per_cluster, bytes_per_sector, numb_free_cluster, numb_total_cluster;
    GetDiskFreeSpaceW(path, &sector_per_cluster, &bytes_per_sector, &numb_free_cluster, &numb_total_cluster);
    ULONGLONG total = (ULONGLONG)numb_total_cluster*sector_per_cluster*bytes_per_sector;
    ULONGLONG free = (ULONGLONG)numb_free_cluster*sector_per_cluster*bytes_per_sector;
    wcout << "Total space on drive: " << total/1e9 << " GB" << endl;
    wcout << "Free space on drive: " << free/1e9 << " GB"<<endl;
}

// Directory Functions --------------------------

void create_dir(){
    wstring wide_path = function_in_path(L"Directory will be created in ", L"Enter the name of directory: ");
    if (CreateDirectoryW(wide_path.c_str(), NULL)){
        wcout << "Directory successfull created" << endl;
    } else wcout << "Error! Directory is not created" << endl;
}

void remove_dir(){
    wstring wide_path = function_in_path(L"The directory to be deleted will be searched in ", L"Enter the name of directory: ");
    if (RemoveDirectoryW(wide_path.c_str())){
        wcout << "Directory is successfull removed" << endl;
    } else wcout << "Error! Directory is not removed" << endl;
}

// File Funtcions --------------------------------

void create_file(int action_mode_choise){ // 0 - to create file, 1 - to open file
    int access_mode_choise, share_mode_choise;
    DWORD access_mode, share_mode, action_mode;

    wstring wide_path;
    if (action_mode_choise==0) wide_path = function_in_path(L"File will be created in ", L"Enter the name of file (Do not forget about extension): ");
    else {
        wide_path = function_in_path(L"Fill will be opened in ", L"Enter the name of file (Do not forget about extension: ");
    }

    wcout << L"\nACCESS MODE\n1 - only for READ\n2 - only for WRITE\n3 - for READ and WRITE\n";
    access_mode_choise = enter_integer(L"Choose access mode: ", 1, 3);
    switch(access_mode_choise){
        case 1: access_mode = GENERIC_READ; break;
        case 2: access_mode = GENERIC_WRITE; break;
        case 3: access_mode = GENERIC_READ | GENERIC_WRITE; break;
    }

    wcout << L"\nSHARE MODE\n1 - for NOT ALLOWED\n2 - for READ\n3 - for WRITE\n4 - for DELETE\n";
    share_mode_choise = enter_integer(L"Choose share mode: ", 1, 4);
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
            wcout << "\nFile with equal name exists. Do you want to rewrite?\n1 - YES\n2 - NO\nYour choice: ";
            int rewrite_choise;
            cin >> rewrite_choise;
            if (rewrite_choise == 1){
                if (action_mode_choise==0) action_mode = CREATE_ALWAYS;
                else action_mode = OPEN_ALWAYS;

                new_file = CreateFileW(wide_path.c_str(), access_mode, share_mode, NULL, action_mode, FILE_ATTRIBUTE_NORMAL, NULL);
                if (new_file == INVALID_HANDLE_VALUE) {
                    wcout << "\nError! Failed to rewrite file" << endl;
                } else {
                    wcout << "\nFile successfully rewritten" << endl;
                    CloseHandle(new_file);
                }
            } else wcout << "File not rewritten." << endl;
        } else {
            if (error!=0){
                wcout << "Error! Please try again";
            }
        }
    } else {
        if (action_mode_choise==0){
            wcout << "\nFile successfully created!" << endl;
        } else wcout << "\nFile successfully opened!" << endl;
        CloseHandle(new_file);
    }
}

void remove_file(){
    wstring wide_path = function_in_path(L"File will deleted in ", L"Enter the name of file (Do not forget about extension): ");
    if(DeleteFileW(wide_path.c_str())){
        wcout << "\nFile was successfully removed\n";
    } else wcout << "Error! File was not removed";
}

void copy_file(){
    wstring wide_path = function_in_path(L"File will be copied from ", L"Etmer the name of file to copy (Do not forget about extension): ");
    wstring wide_repath = function_in_path(L"File will be pasted in ", L"Enter the new name of copied file: ");

    if(CopyFileW(wide_path.c_str(), wide_repath.c_str(), TRUE)){
        wcout << "\nFile was successfully copied\n";
    } else {
        wcout << "\nFile with equal name exists. Do you want to rewrite?\n1 - YES\n2 - NO\nYour choice: ";
        int rewrite_choise;
        cin >> rewrite_choise;
        if (rewrite_choise == 1){
            if(CopyFileW(wide_path.c_str(), wide_repath.c_str(), FALSE)){
                wcout << "File was successfully copied";
            }
            else wcout << "Error! Please try again";
        }
    }
}

void rename(int is_dir){
    wstring wide_path, wide_repath;
    if(is_dir==0){
        wide_path = function_in_path(L"File to rename fill be from ", L"Enter the name of file to rename (Do not forget about extension): ");
        wide_repath = function_in_path(L"Renamed file will keeped in ", L"Enter the new name of file: ");
    } else {
        wide_path = function_in_path(L"Directory to rename fill be from ", L"Enter the name of directory to rename: ");
        wide_repath = function_in_path(L"Renamed directory will keeped in ", L"Enter the new name of directory: ");
    }
    
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        if (is_dir==0) wcout << "\nFile was successfullt renamed";
        else wcout << "\nDirectory was successfullt renamed";
    } else {
        if (is_dir==0) wcout << "\nFile was not renamed";
        else wcout << "\nDirectory was not renamed";
    }
}

void move(int is_dir){
    wstring path = get_current_dir();
    string name, repath;
    if (is_dir==0) {
        wcout << L"\nFile will be moved according to this path: " << path << L"\\" << endl;
        wcout << L"Enter the name of file that will be moved (Do not forget about extension): ";
    }
    else {
        wcout << L"\nDirectory will be moved according to this path: " << path << L"\\" << endl;
        wcout << L"Enter the name of directory that will be moved: ";
    }

    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    wcout << "Specify a relative path to move: ";
    cin >> repath;
    wstring wide_repath = wstring(path.begin(), path.end()) + L"\\"+ wstring(repath.begin(), repath.end()) + L"\\"+ wstring(name.begin(), name.end());

    
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        wcout << L"File was successfullt moved";
    } else {
        if (GetLastError() == ERROR_ALREADY_EXISTS){
            int replace = enter_integer(L"File with equal name exist. Are you want to replace it? (0 - for NO / 1 - for YES):",0, 1);
            if (replace==0) wcout << L"Operation of moving is canceled";
            else MoveFileExW(wide_path.c_str(), wide_repath.c_str(), 0x01);
        }
    }
}

void print_file_attr(DWORD attributes) {
    wcout << "FILE ATTRIBUTES:" << endl;

    if (attributes & FILE_ATTRIBUTE_READONLY) wcout << "- Only for reading" << endl;
    if (attributes & FILE_ATTRIBUTE_HIDDEN) wcout << "- Hidden" << endl;
    if (attributes & FILE_ATTRIBUTE_SYSTEM) wcout << "- System file" << endl;
    if (attributes & FILE_ATTRIBUTE_DIRECTORY) wcout << "- Directory" << endl;
    if (attributes & FILE_ATTRIBUTE_ARCHIVE) wcout << "- Archive" << endl;
    if (attributes & FILE_ATTRIBUTE_NORMAL) wcout << "- Default file" << endl;
    if (attributes & FILE_ATTRIBUTE_TEMPORARY) wcout << "- Temporary file" << endl;
    if (attributes & FILE_ATTRIBUTE_COMPRESSED) wcout << "- Compressed" << endl;
    if (attributes & FILE_ATTRIBUTE_ENCRYPTED) wcout << "- Encrypted" << endl;
    if (attributes & FILE_ATTRIBUTE_VIRTUAL) wcout << "- Virtual file" << endl;
}

void get_file_attr(int is_dir){
    wstring wide_path;
    if(is_dir==0){
        wide_path = function_in_path(L"File for get attributes will be taken from ", L"Enter the name of file which attributes will be shown (Do not forget about extension): ");
    } else {
        wide_path = function_in_path(L"Directory for get attributes will be taken from ", L"Enter the name of directory which attributes will be shown: ");
    }
    DWORD attributes = GetFileAttributesW(wide_path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) wcout << "Error! Please try again";
    else print_file_attr(attributes);
}

void set_file_attr(int is_dir){
    wstring wide_path;
    if (is_dir==0){
        wide_path = function_in_path(L"File for set attributes will be taken from ", L"Enter the name of file which attributes will be setted (Do not forget about extension): ");
    } else {
        wide_path = function_in_path(L"Directory for set attributes will be taken from ", L"Enter the name of directory which attributes will be setted: ");
    }

    DWORD attributes = GetFileAttributesW(wide_path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        wcout << "\nError! Please try again\n";
        return;
    }
    wcout << "\nFor each attribute enter 0 (No) or 1 (Yes) to set it\n";
    int choice;
    wcout << "\nOnly for reading? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_READONLY;
    else attributes &= ~FILE_ATTRIBUTE_READONLY;

    wcout << "Hidden? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_HIDDEN;
    else attributes &= ~FILE_ATTRIBUTE_HIDDEN;

    wcout << "System file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_SYSTEM;
    else attributes &= ~FILE_ATTRIBUTE_SYSTEM;

    wcout << "Archived? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_ARCHIVE;
    else attributes &= ~FILE_ATTRIBUTE_ARCHIVE;

    wcout << "Temporary file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_TEMPORARY;
    else attributes &= ~FILE_ATTRIBUTE_TEMPORARY;

    wcout << "Compressed file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_COMPRESSED;
    else attributes &= ~FILE_ATTRIBUTE_COMPRESSED;

    wcout << "Encrypted file? (1 - Yes, 0 - No): ";
    cin >> choice;
    if (choice == 1) attributes |= FILE_ATTRIBUTE_ENCRYPTED;
    else attributes &= ~FILE_ATTRIBUTE_ENCRYPTED;

    if (SetFileAttributesW(wide_path.c_str(), attributes)) {
        wcout << "\nAttributes were successfully set\n";
    } else wcout << "Error! Attributes were not setted. Please try again";
}

void print_filetime(const FILETIME& ft) {
    SYSTEMTIME st;
    FileTimeToSystemTime(&ft, &st);
    wcout << st.wDay << "." << st.wMonth << "." << st.wYear << " ";
    wcout << st.wHour << ":" << st.wMinute << ":" << st.wSecond << endl;
}

FILETIME systime_to_filetime(const SYSTEMTIME& st) {
    FILETIME ft;
    SystemTimeToFileTime(&st, &ft);
    return ft;
}

void get_file_info_by_handle(){
    wstring wide_path = function_in_path(L"File for get file information by handle will be taken from ", L"Enter the name of file which attributes will be shown: ");

    HANDLE handle_file = CreateFileW(wide_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle_file == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (error = ERROR_FILE_NOT_FOUND){
            wcout << "Error! File to get info is not exist" << endl;
        }
        if (error == ERROR_ACCESS_DENIED) {
            wcout << "Access denied. You do not have permission to open this file" << endl;
        } else if (error == ERROR_SHARING_VIOLATION) {
            wcout << "File is being used by another process and cannot be opened" << endl;
        } else {
            wcout << "Failed to open file" << endl;
        }
        return;
    }

    BY_HANDLE_FILE_INFORMATION file_info;
    FILETIME creation_time, laccess_time, lwrite_time;

    if (GetFileInformationByHandle(handle_file, &file_info)){
        print_file_attr(file_info.dwFileAttributes);

        if (GetFileTime(handle_file, &creation_time, &laccess_time, &lwrite_time)){
            wcout << "Creation time: ";
            print_filetime(creation_time);
    
            wcout << "Last access time: ";
            print_filetime(laccess_time);
    
            wcout << "Last write time: ";
            print_filetime(lwrite_time);
        } else {
            wcout << "Error! Cant get file time" << endl;
        }

        wcout << "Serial number of volume that contains that file: " << file_info.dwVolumeSerialNumber << endl;

        wcout << "Number of links on that file: " << file_info.nNumberOfLinks << endl;
    }
    CloseHandle(handle_file);
}

void set_file_time(){
    wstring wide_path = function_in_path(L"File for set file time will be taken from ", L"Enter the name of file which file time will be setted (Do not forget about extension): ");

    HANDLE handle_file = CreateFileW(wide_path.c_str(), FILE_WRITE_ATTRIBUTES, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (handle_file == INVALID_HANDLE_VALUE) {
        wcout << "Error! Please try again" << endl;
        return;
    }

    SYSTEMTIME st;
    FILETIME ftCreation, ftLastAccess, ftLastWrite;

    int choice = enter_integer(L"Set new creation time? (1 - yes, 0 - no): ",0, 1);
    if (choice) {
        wcout << "Enter creation date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftCreation = systime_to_filetime(st);
    } else {
        ftCreation.dwLowDateTime = 0;
        ftCreation.dwHighDateTime = 0;
    }

    wcout << "Set new last access time? (1 - yes, 0 - no): ";
    cin >> choice;
    if (choice) {
        wcout << "Enter last access date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftLastAccess = systime_to_filetime(st);
    } else {
        ftLastAccess.dwLowDateTime = 0;
        ftLastAccess.dwHighDateTime = 0;
    }

    wcout << "Set new last write time? (1 - yes, 0 - no): ";
    cin >> choice;
    if (choice) {
        wcout << "Enter last write date (YYYY MM DD HH MM SS): ";
        cin >> st.wYear >> st.wMonth >> st.wDay >> st.wHour >> st.wMinute >> st.wSecond;
        ftLastWrite = systime_to_filetime(st);
    } else {
        ftLastWrite.dwLowDateTime = 0;
        ftLastWrite.dwHighDateTime = 0;
    }

    if (SetFileTime(handle_file, (ftCreation.dwLowDateTime == 0 && ftCreation.dwHighDateTime == 0) ? NULL : &ftCreation, (ftLastAccess.dwLowDateTime == 0 && ftLastAccess.dwHighDateTime == 0) ? NULL : &ftLastAccess, (ftLastWrite.dwLowDateTime == 0 && ftLastWrite.dwHighDateTime == 0) ? NULL : &ftLastWrite)) {
        wcout << "File time successfully updated" << endl;
    } else wcout << "Error! File time was not updated" << endl;

    CloseHandle(handle_file);
}

// main -----------------------------------------

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

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
    wcout << "\nGoodbye!\n" << endl;
    return 0;
}
