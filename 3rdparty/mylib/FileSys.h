//
// Created by jmu on 18-3-13.
//

#ifndef INC_3_3_SIMPLE_LOAD_FILESYS_H
#define INC_3_3_SIMPLE_LOAD_FILESYS_H
#define ROOT "/home/jmu/workspaces/clion/Opengl-linux/"

#include <string>

using namespace std;

class FileSys {
public:
    FileSys(string path, bool useCurrentSys = true) {
        if (useCurrentSys) {
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

#define RES_ROOT ROOT"res/"

class Resource {
public:
    Resource(string path, bool useCurrentSys = true) {
        if (useCurrentSys) {
            pathName = string(RES_ROOT) + path;
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
