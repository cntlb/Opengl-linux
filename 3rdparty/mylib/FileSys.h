//
// Created by jmu on 18-3-13.
//

#ifndef INC_3_3_SIMPLE_LOAD_FILESYS_H
#define INC_3_3_SIMPLE_LOAD_FILESYS_H

#ifndef ROOT
#error 'ROOT'根目录未指定
#endif

#include <string>

using namespace std;

class FileSys {
public:
    FileSys(string path, bool useCurrentSys = true) {
        if (useCurrentSys) {
            string root(ROOT);
            if(root[root.length()-1] != '/')
                root += "/";
            pathName = string(ROOT) + path;
        } else {
            pathName = path;
        }
    }

    operator const char *() {
        return pathName.c_str();
    }

private:
    string pathName;
};

class Resource {
public:
    Resource(string path, bool useCurrentSys = true) {
        if (useCurrentSys) {
            string root(ROOT);
            if(root[root.length()-1] != '/')
                root += "/";
            pathName = root + "res/" + path;
        } else {
            pathName = path;
        }
    }

    operator const char *() {
        return pathName.c_str();
    }

private:
    string pathName;
};

#endif //INC_3_3_SIMPLE_LOAD_FILESYS_H
