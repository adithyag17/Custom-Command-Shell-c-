#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <cstdlib>

namespace filesys = std::filesystem; 
class utils {
public:
// This function just takes the initial input and passes it to the execute function
    void executeCommand(const std::string& input) {
        std::vector<std::string> words = split(input);
        execute(words);
    }
    //Add all the commands to the help function. acts as an initial mannual
    void help() {
        std::cout << "Available commands:\n";
        std::cout << "  help : Display available commands.\n";
        std::cout << "  exit : Exit the shell.\n";
        std::cout << "  ls   : List files and directories in the current directory.\n";
        std::cout << "  pwd : Print Working Directory.\n";
        std::cout << "  cd : Change Current Working Directory.\n";
        std::cout << "  rm : Remove file or directories.\n";
        std::cout << "  cp : Copy files or directories from one location to another.\n";
    }
    void show_all_commands(){
        std::string commands[21] = {
        "ls: List files and directories in the current working directory.",
        "pwd: Print the current working directory (the path of the directory you are currently in).",
        "cd: Change the current working directory to the specified directory.",
        "cp: Copy files or directories from one location to another.",
        "rm: Remove (delete) files or directories.",
        "man: Display the manual pages (documentation) for a specific command.",
        "mv: Move or rename files or directories.",
        "mkdir: Create a new directory.",
        "touch: Create an empty file or update the access/modified timestamps of an existing file.",
        "chmod: Change the permissions of files or directories.",
        "exit: Exit the current shell or terminal session.",
        "sudo: Execute a command with superuser (administrative) privileges.",
        "echo: Print the given arguments to the standard output.",
        "cat: Concatenate and display the contents of files.",
        ">>: Redirect and append output to a file.",
        "<<: Redirect input from a here document or a string.",
        "history: Display a list of recently executed commands.",
        "grep: Search for a pattern in files or output.",
        "wget: Download files from the internet using the HTTP, HTTPS, or FTP protocols.",
        "curl: Transfer data from or to a server using various protocols (URLs).",
        "unzip: Extract files from a ZIP archive."
    };
    std::cout << "Command Manual:\n";
        for (const std::string& command : commands) {
            std::cout << command << std::endl;
        }
    }
    void manualCommand(const std::string flags) {
    std::string commands[21] = {
        "ls: List files and directories in the current working directory.",
        "pwd: Print the current working directory (the path of the directory you are currently in).",
        "cd: Change the current working directory to the specified directory.",
        "cp: Copy files or directories from one location to another.",
        "rm: Remove (delete) files or directories.",
        "man: Display the manual pages (documentation) for a specific command.",
        "mv: Move or rename files or directories.",
        "mkdir: Create a new directory.",
        "touch: Create an empty file or update the access/modified timestamps of an existing file.",
        "chmod: Change the permissions of files or directories.",
        "exit: Exit the current shell or terminal session.",
        "sudo: Execute a command with superuser (administrative) privileges.",
        "echo: Print the given arguments to the standard output.",
        "cat: Concatenate and display the contents of files.",
        ">>: Redirect and append output to a file.",
        "<<: Redirect input from a here document or a string.",
        "history: Display a list of recently executed commands.",
        "grep: Search for a pattern in files or output.",
        "wget: Download files from the internet using the HTTP, HTTPS, or FTP protocols.",
        "curl: Transfer data from or to a server using various protocols (URLs).",
        "unzip: Extract files from a ZIP archive."
    };

    if (flags == "" || flags==" ") {
        std::cout << "Command Manual:\n";
        for (const std::string& command : commands) 
            std::cout << command << std::endl;
        
    } else {
        bool foundcmd = false;
        for (const std::string& command : commands) {
            if (command.find(flags + ":") == 0) {
                std::cout << "Command Manual for " << flags << ":\n" << command << std::endl;
                foundcmd = true;
                break;
            }
        }
        if (!foundcmd) 
            std::cout << "Command not found in the manual." << std::endl;
        
    }
}
    void makedir(std::string path){
        if(filesys::create_directory(path))
        return ;
        else
        std::cout << "couldnt create directory " << path << std::endl;
     
    }
void rm(const std::vector<std::string>& input) {
        bool forceRemoveflag = false;
        std::string file_path;
        for (size_t i = 1; i < input.size(); i++) { // to check -f flag
            std::string flags_of_command = input[i];
            if (flags_of_command == "-f") {
                forceRemoveflag = true;
            } else {
                file_path = flags_of_command;
            }
        }

        if (file_path.empty()) {
            std::cout << "Usage: rm [-f] <file_path>\n";
            return;
        }

        try {
            if (forceRemoveflag == true) {
                filesys::remove_all(file_path); //all directories
            } else {
                filesys::remove(file_path);  //remove files
            }
            std::cout << "File removed successfully.\n";
        } catch (const std::filesystem::filesystem_error& ex) {
            std::cerr << "Error removing file/directory: " << ex.what() << std::endl;
        }
    }
void cd(const std::string& directory) {
    try {
        if(directory.empty()==true){
            char* home_directory = std::getenv("HOME");
            if (home_directory != nullptr) {
                filesys::current_path(home_directory);
                std::cout << "Current working directory changed to: " << home_directory << std::endl;
            } else {
                std::cerr << "Error: HOME environment variable not set." << std::endl;
            }

        }
        else
        {
        filesys::path current_path = filesys::current_path();
        filesys::path new_path = current_path / directory;

        filesys::current_path(new_path);
        std::cout << "Current working directory changed to: " << new_path << std::endl;
        } 
        }
    catch (const std::exception& e) {
        std::cerr << "Error changing the current directory: " << e.what() << std::endl;
    }
}
    void makedirwithpermissions(std::string path,int permissions){
         try {
        filesys::create_directory(path);
        filesys::permissions(path, filesys::perms::all | static_cast<filesys::perms>(permissions));
        return ;
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error creating directory: " << ex.what() << std::endl;
        return ;
    }
    }
    void opennotepad(std::string filename){
    std::string notepadparameter;notepadparameter = "notepad \""+filename + "\"";
    system(notepadparameter.c_str()); 
    return;
    }
void clearfunc(){
    std::cout << "\033[2J\033[1;1H";
    }
    //copy and delete contents from source to destination file and delete the file as well
    void copyfunction(const std::string& source, const std::string& destination) {
        std::ifstream sourceFile(source, std::ios::binary);
        std::ofstream destinationFile(destination, std::ios::binary);

        // Check if both files are open before proceeding
        if (sourceFile.is_open() && destinationFile.is_open()) {
            // Copy the contents from source to destination
            destinationFile << sourceFile.rdbuf();

            // Close the files after copying
            sourceFile.close();
            destinationFile.close();

            std::cout << "File copied successfully to your dest file." << std::endl;
        } else {
            std::cerr << "Error: Could not open files." << std::endl;
      }
    
}
    // All the functions for the commands are written as a else if condition in the execute function
    void execute(std::vector<std::string> input) {
        if (input.empty()) {
            return;
        }
       
        if (input[0] == "help") {
            help();
        } else if (input[0] == "ls") {
            bool showHidden = false;

            for (size_t i = 1; i < input.size(); i++) {
                std::string word = input[i];
                if (word == "-a") {
                    showHidden = true; // setting bool variables first to check which flags have been activated.
                }
            }

            listDirectory(".", showHidden);
        }else if (input[0] == "run") {
            if (input.size() < 2) {
                std::cout << "Usage: execute <path_to_sh_file>\n";
                return;
            }
            else run(input[1]);
        } 
        else if(input[0] == "mkdir"){
            if (input[1] =="-m"){
                int permissions = std::stoi(input[2]);
                makedirwithpermissions(input[3],permissions);
            }
            else
            makedir(input[1]);
        }

        else if(input[0] == "pwd"){
            std::string current_directory = pwd();
            if (!current_directory.empty()) {
       
            std::cout << "Present working directory: " << current_directory << std::endl;
    }        }
          
      else if (input[0] == "rm") {
            rm(input);
        }
          else if (input[0] == "cd") {
        if (input.size() > 1) {
            cd(input[1]);
        } else {

            cd(""); //home directory
        }
    }
        else if(input[0] == "touch"){
            touchFile(input[1]);
            //tried updating time stamp was not possible unless we use external libraries
        }
         else if(input[0] == "clear"){
            clearfunc();
        }
        else if(input[0] == "nano"){
            opennotepad(input[1]);
        }
        else if(input[0] == "rename"){
            renameFile(input[1],input[2]);
        }
        else if(input[0] == "cp"){
            copyfunction(input[1],input[2]);
        }

        else if (input[0] == "man") {
        if (input.size() > 1) {
            for (size_t i = 1; i < input.size(); i++) {
                std::string word = input[i];
                manualCommand(word);}}
         else {
            show_all_commands();
        }}
          else if (input[0] == "exec") {
            if (input.size() < 2) {
                std::cout << "Usage: exec <path_to_file>\n";
                return;
            }
            else exec(input[1]);
        } 
          
     else {
        std::cout << "Unknown command: " << input[0] << std::endl;
    }
    }
    void touchFile(const std::string& filename) {
    auto current_time = std::chrono::system_clock::now();
  
    filesys::path path{ "." };
    path /= filename; 
    filesys::create_directories(path.parent_path()); 
    std::ofstream ofs(path);
    ofs << "File created by touch command that you have put\n"; 
    ofs.close();
    }
    
void renameFile(const std::string& source, const std::string& destination) {
        try {
        if (filesys::exists(source)) {
            // Using  filesys::rename() method to to rename the file
            filesys::rename(source, destination);
            return ;
        } else {
            std::cout << "Source file does not exist." << std::endl;
            return ;
        }
    } catch (const std::filesystem::filesystem_error& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return ;
    }
}
    //From here we start defining all the functions required to execute the commands.

    void listDirectory(const std::string& directory, bool showHidden) {
        for (const auto& entry : filesys::directory_iterator(directory)) {
            std::string filename = entry.path().filename().string();
            if (!showHidden && filename[0] == '.') { //when show hidden(a flag) is true the continue is not executed hence all files are given
                continue;
            }
            std::cout << filename << " ";
        }
        std::cout << std::endl;
    }
    std::vector<std::string> split(std::string line) {
        std::vector<std::string> words;
        std::string word;
        for (char c : line) {
            if (c == ' ') {
                if (!word.empty()) {
                    words.push_back(word);
                    word="";
                }
            } else {
                word += c;
            }
        }
        if (!word.empty()) {
            words.push_back(word);
        }
        return words;
    }
    //define the run fucntion  which takes the path of a .sh file and executes all the lines
   void run(std::string p) {
        std::string path = p;
        std::ifstream file(path); //This opens and reads the file
        std::string lin;
        while (std::getline(file, lin)) {
            executeCommand(lin);
        }
    } 
    std::string pwd() {
    try {    
        //Use std::filesystem::current_path() to get the current working directory 
        std::filesystem::path current_path = std::filesystem::current_path();
        return current_path.string();
    } catch (const std::exception& e) {
        std::cerr << "Error getting the current directory: " << e.what() << std::endl;
        return "";
    }
}
  void exec(const std::string& path) {
        if (filesys::exists(path)) {
            std::string fileExtension = path.substr(path.find_last_of(".") + 1);
            if (fileExtension == "cpp") {
                executeCppFile(path);
            } else if (fileExtension == "py") {
                executePythonFile(path);}
            else if (fileExtension == "c") {
                executeCFile(path);
            } else {
                std::cout << "Unsupported file type. Cannot execute." << std::endl;
            }
        } else {
            std::cout << "File not found: " << path << std::endl;
        }
    }
void executeCppFile(const std::string& cppFilePath) {
        std::filesystem::path cppPath(cppFilePath);
        std::string executablePath = cppPath.stem().string(); // Get the filename stem (without extension)
        std::string compileCommand = "g++ -o " + executablePath + " " + cppFilePath;

        int compileResult = std::system(compileCommand.c_str());

        if (compileResult == 0) {
            std::string runCommand =  executablePath;
            std::system(runCommand.c_str());
        } else {
            std::cout << "Compilation failed." << std::endl;
        }
    }
    void executeCFile(const std::string& cppFilePath) {
    std::filesystem::path cppPath(cppFilePath);
    std::string executablePath = cppPath.stem().string(); // Get the filename stem (without extension)
    std::string compileCommand = "gcc -o " + executablePath + " " + cppFilePath;

    int compileResult = std::system(compileCommand.c_str());

    if (compileResult == 0) {
        std::string runCommand = executablePath; // Removed the './' prefix
        std::system(runCommand.c_str());
    } else {
        std::cout << "Compilation failed." << std::endl;
    }
}

     void executePythonFile(const std::string& pyFilePath) {
        std::string command = "python " + pyFilePath;
        int executionResult = std::system(command.c_str());

        if (executionResult != 0) {
            std::cout << "Execution failed." << std::endl;
        }
    }
};

int main() {
    std::string input;
    utils obj;
    std::cout << "Simple Shell Interface\n";
    std::cout << "Type 'help' to see available commands.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Exiting shell";
            return 0;
        }
        obj.executeCommand(input);
    }
}
