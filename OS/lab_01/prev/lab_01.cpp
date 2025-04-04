#include <windows.h>
#include <iostream>
#include <string>
#include <limits>
#include <vector>


using namespace std;

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

int enter_integer(string message, int a, int b){
    string input;
    int number;

    while (true) {
        std::cout << message;
        std::cin >> input;

        try {
            number = stoi(input);
            if (number >= a && number <= b) {
                break; 
            } else {
                cout << "Entered value is not in the range [" << a << ", " << b << "]. Please try again!" << endl;
            }
        } catch (...) {
            cout << "Error! Please try again" << endl;
        }

        cin.clear();
    }

    return number;
}

int menu(){
    int option;
    cout << "OPTIONS:\n";
    cout << "0 - for EXIT\n";
    cout << "1 - for OUTPUT DRIVES\n";
    cout << "2 - for OUTPUT DRIVE INFO\n";
    cout << "3 - for CREATE DIRECTORY\n";
    cout << "4 - for REMOVE DIRECTORY\n";
    cout << "5 - for CREATE FILE\n";
    cout << "6 - for OPEN FILE\n";
    cout << "7 - for REMOVE FILE\n";
    cout << "8 - for COPY FILE\n";
    cout << "9 - for RENAME FILE/DIRECTORY\n";
    cout << "10 - for MOVE FILE\n";
    cout << "11 - for GET FILE ATTRIBUTES\n";
    cout << "12 - for SET FILE ATTRIBUTES\n";
    cout << "13 - for GET FILE INFORMATION BY HANDLE\n";
    cout << "14 - for SET FILE TIME\n";
    option = enter_integer("Choose option: ", 0, 14);
    return option;
}

void clear_screen(){
    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #else
        system("clear");
    #endif
}

string get_current_dir(){
    wchar_t buffer[MAX_PATH];
    DWORD len = GetCurrentDirectoryW(MAX_PATH, buffer);
    if (len==0){
        return "";
    } else{
        wstring wide_str(buffer);
        string narrow_str(wide_str.begin(), wide_str.end());
        return narrow_str;
    }
}

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
    wchar_t disk = L'C';
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
    case 0:
        cout << "Unknown type of drive" << endl;
        break;
    case 1:
        cout << "Incorrect root" << endl;
        break;
    case 2:
        cout << "Drive is removable" << endl;
        break;
    case 3:
        cout << "Drive is fixed" << endl;
        break;
    case 4:
        cout << "Drive is remotable" << endl;
        break;
    case 5:
        cout << "Drive is CD-ROM" << endl;
        break;
    case 6:
        cout << "Drive is RAM disk" << endl;
        break;
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

void create_dir(){
    string path = get_current_dir();
    string name;
    cout << "\nDirectory will be created in " << path << "\\" << endl;
    cout << "Enter the name of directory: ";
    cin >> name;
    // wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    wstring wide_path = string_to_wstring(path) + L"\\" + string_to_wstring(name);
    if (CreateDirectoryW(wide_path.c_str(), NULL)){
        cout << "Directory successfull created" << endl;
    } else cout << "Error! Directory is not created" << endl;
}

void remove_dir(){
    string path = get_current_dir();
    string name;
    cout << "\nThe directory to be deleted will be searched in " << path << "\\" << endl;
    cout << "Enter the name of dircetory: ";
    cin >> name;
    wstring wide_path = string_to_wstring(path) + L"\\" + string_to_wstring(name);
    if (RemoveDirectoryW(wide_path.c_str())){
        cout << "Directory is successfull removed" << endl;
    } else cout << "Error! Directory is not removed" << endl;
}

void create_file(int action_mode_choise){ // 0 - to create file, 1 - to open file
    string path = get_current_dir();
    string name;
    int access_mode_choise, share_mode_choise;
    DWORD access_mode, share_mode, action_mode;
    if (action_mode_choise==0) cout << "\nFile will be created in " << path << "\\" << endl;
    else cout << "\nFile will be opened in " << path << "\\" << endl;
    cout << "Enter the name of file (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = string_to_wstring(path) + L"\\" + string_to_wstring(name);

    cout << "\nACCESS MODE\n1 - only for READ\n2 - only for WRITE\n3 - for READ and WRITE\n";
    access_mode_choise = enter_integer("Choose access mode: ", 1, 3);
    switch(access_mode_choise){
        case 1: access_mode = GENERIC_READ; break;
        case 2: access_mode = GENERIC_WRITE; break;
        case 3: access_mode = GENERIC_READ | GENERIC_WRITE; break;
        default:{
            cout << "Error! File is not created" << endl;
            return;
        }
    }

    cout << "\nSHARE MODE\n1 - for NOT ALLOWED\n2 - for READ\n3 - for WRITE\n4 - for DELETE\n";
    share_mode_choise = enter_integer("Choose share mode: ", 1, 4);
    switch(share_mode_choise){
        case 1: share_mode= 0; break;
        case 2: share_mode= FILE_SHARE_READ; break;
        case 3: share_mode= FILE_SHARE_WRITE; break;
        case 4: share_mode= FILE_SHARE_DELETE; break;
        default:{
            cout << "Error! File is not created" << endl;
            return;
        }
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
        }
    } else {
        if (action_mode_choise==0){
            cout << "\nFile successfully created!" << endl;
        } else cout << "\nFile successfully opened!" << endl;
        CloseHandle(new_file);
    }
}


void wait_reaction(){
    cout << "\nEnter any key to continiue" << endl;
    cin.ignore();
    getchar();
}

void remove_file(){
    string path = get_current_dir();
    string name;
    cout << "\nFile will be removed from " << path << "\\" << endl;
    cout << "Enter the name of file to copy (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    if(DeleteFileW(wide_path.c_str())){
        cout << "\nFile was successfully removed\n";
    } else cout << "Error! File was not removed";
}

void copy_file(){
    string path = get_current_dir();
    string name, rename;
    DWORD access_mode, share_mode;
    cout << "\nFile will be copied from " << path << "\\ and pasted here" << endl;
    cout << "Enter the name of file to copy (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    cout << "Enter the new name of copied file: ";
    cin >> rename;
    wstring wide_repath = wstring(path.begin(), path.end()) + L"\\"+wstring(rename.begin(), rename.end());
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

void rename(){
    string path = get_current_dir();
    string name, rename;
    cout << "\nFile/Directory to rename will be from " << path << "\\" << endl;
    cout << "Enter the name of File/Directory to rename (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    cout << "Enter the new file/directory name: ";
    cin >> rename;
    wstring wide_repath = wstring(path.begin(), path.end()) + L"\\"+wstring(rename.begin(), rename.end());
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        cout << "\nFile/Directory was successfully renamed\n";
    } else cout << "Error! File/Directory was not renamed";
}

void move(){
    string path = get_current_dir();
    string name, repath;
    cout << "\nFile/Directory will be moved according to this path: " << path << "\\" << endl;
    cout << "Enter the name of File/Directory that will be moved (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    cout << "Specify a relative path to move: ";
    cin >> repath;
    wstring wide_repath = wstring(path.begin(), path.end()) + L"\\"+ wstring(repath.begin(), repath.end()) + L"\\"+ wstring(name.begin(), name.end());
    if(MoveFileW(wide_path.c_str(), wide_repath.c_str())){
        cout << "OK";
    } else {
        if (GetLastError() == 183){
            int replace = enter_integer("File with equal name exist. Are you want to replace it? (0 - for NO / 1 - for YES):",0, 1);
            if (replace==0) cout << "File was not moved";
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

void get_file_attr(){
    string path = get_current_dir();
    string name;
    cout << "\nFile for get attributes will be taken from this path: " << path << "\\" << endl;
    cout << "Enter the name of file which attributes will be shown (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    DWORD attributes = GetFileAttributesW(wide_path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        cout << "Error! Please try again";
    } else {
        print_file_attr(attributes);
    }
}

void set_file_attr(){
    string path = get_current_dir();
    string name;
    cout << "\nFile for set attributes will be taken from this path: " << path << "\\" << endl;
    cout << "Enter the name of file which attributes will be setted (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());

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
    string path = get_current_dir();
    string name;
    cout << "\nFile for set attributes will be taken from this path: " << path << "\\" << endl;
    cout << "Enter the name of file which attributes will be setted (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());
    HANDLE handle_file = CreateFileW(wide_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (handle_file == INVALID_HANDLE_VALUE) {
        DWORD error = GetLastError();
        if (error = ERROR_FILE_NOT_FOUND){
            cout << "Error! File to get info is not exist";
        }
        if (error == ERROR_ACCESS_DENIED) {
            cout << "Access denied. You don't have permission to open this file." << endl;
        } else if (error == ERROR_SHARING_VIOLATION) {
            cout << "File is being used by another process and cannot be opened." << endl;
        } else {
            cout << "Failed to open file. Error code: " << error << endl;
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
    string path = get_current_dir();
    string name;
    cout << "\nFile for set filetime will be taken from this path: " << path << "\\" << endl;
    cout << "Enter the name of file which filetime will be setted (Do not forget about extension): ";
    cin >> name;
    wstring wide_path = wstring(path.begin(), path.end()) + L"\\" + wstring(name.begin(), name.end());

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

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    int option;
    do{
        option=menu();
        switch(option){
            case 0:{
                clear_screen();
                cout << "Your selection is EXIT\n" << endl; 
                break;
            }
            case 1:{
                DWORD drives_mask = get_logical_drives();
                wait_reaction();
                break;
            }
            case 2:{
                output_drive_info();
                wait_reaction();
                break;
            }
            case 3:{
                create_dir();
                wait_reaction();
                break;
            }
            case 4:{
                remove_dir();
                wait_reaction();
                break;
            }
            case 5:{
                create_file(0);
                wait_reaction();
                break;
            }
            case 6:{
                create_file(1);
                wait_reaction();
                break;
            }
            case 7:{
                remove_file();
                wait_reaction();
                break;
            }
            case 8:{
                copy_file();
                wait_reaction();
                break;
            }
            case 9:{
                rename();
                wait_reaction();
                break;
            }
            case 10:{
                move();
                wait_reaction();
                break;
            }
            case 11:{
                get_file_attr();
                wait_reaction();
                break;
            }
            case 12:{
                set_file_attr();
                wait_reaction();
                break;
            }
            case 13:{
                get_file_info_by_handle();
                wait_reaction();
                break;
            }
            default:
                cout << "\nEntered value is not correct. Please try again!\n\n";
        }
    } while(option!=0);
    return 0;
}