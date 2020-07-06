#include <cppfs/fs.h>
#include <cppfs/FilePath.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>
#include <iostream>

using namespace cppfs;
using namespace std;

void openFile(const std::string & filename)
{
    FileHandle fh = fs::open(filename);

    if (fh.isFile()){
            cout << "file: ";
    }
    else if (fh.isDirectory()){
            cout << "dir: ";
    }
    else if (!fh.exists()){
            cout << "unexistent: ";
    }
    cout << filename << endl;
}
void lstDir(const std::string & path)
{
    FilePath fp(path);
    FileHandle dir = fs::open(path);
    if (dir.isDirectory()){
        dir.traverse([](FileHandle& fh)->bool{
            openFile(fh.path());
            return true;
        },[](FileHandle& fh)->bool{
            return true;
        });
        /*for (FileIterator it = dir.begin(); it != dir.end(); ++it){
            openFile(fp.resolve(*it).fullPath());
        }*/
    }
}
int main(int argc, char** argv){
    if(argc != 2){
        cout << "Give path as only argument" << endl;
        return 0;
    }
    lstDir(argv[1]);
    return 0;
}
