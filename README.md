# Archive-Program-
This is a simple command-line program written in C that allows you to create, list, and unpack archives. The program provides functionality to bundle multiple files into a single binary archive file, list the contents of an archive, and extract the files from an archive.

# Usage
Clone or download the source code from the repository and save it in a file, say, archive.c.
Change to the working directory where you have saved the archive.c on your terminal. 
Compile the program using a C compiler. For example, you can use GCC:
  gcc archive.c -o archive
Execute the program with the desired operation and arguments:
  ./archive <operation> <archive_file> [file1 file2 ...]
  Replace <operation> with one of the following options:
    -c: Create an archive
    -l: List the contents of an archive
    -u: Unpack an archive
  Replace <archive_file> with the name of the archive file you want to create, list, or unpack. If the file does not exist, it will be        created during the archive creation process.
  For the -c operation, you need to specify one or more files to be included in the archive. Provide the file paths as additional             arguments, separated by spaces.

# Examples
Create an archive: archive -c myarchive.bin file1.txt file2.txt file3.txt
List the contents of an archive: archive -l myarchive.bin
Unpack an archive: archive -u myarchive.bin

# Additional Information
The maximum length of a filename is defined as MAX_FILENAME_LENGTH and set to 256 characters. If a filename exceeds this limit, it will be truncated.
When creating the archive with the files it includes, it is assumed that in the current directory, you have all the files for testing(file1.txt, file2.txt and file3.txt). Else create the files using the commands :
echo "This is file 1 content" > file1.txt
echo "This is file 2 content" > file2.txt
echo "This is file 3 content" > file3.txt

The program uses binary mode when reading and writing files and archives.
When creating an archive, the program stores the file entries (filename and size) first and then writes the file contents.
The archive format consists of the number of files (stored as an integer) followed by an array of file entries.
When unpacking an archive, the program recreates the files in the current working directory with the original filenames and file contents.
The program handles basic error checking for file operations and memory allocation. However, it may not cover all possible error scenarios.
It is recommended to provide absolute or relative file paths when specifying files for archiving or unpacking.
This program assumes a Unix-like shell environment. If you are using a Windows Command Prompt, adjust the commands accordingly.
Feel free to modify and enhance the program as per your requirements. Happy archiving!
