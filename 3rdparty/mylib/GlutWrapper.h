//
// Created by jmu on 18-3-2.
//

#ifndef INC_1_7_TEXTURE2PP_GLUTWRAPER_H
#define INC_1_7_TEXTURE2PP_GLUTWRAPER_H

#include <cstdlib>

struct GlutWrapper {
public:
    void (*onPreDraw)();
    void (*onDraw)();
    void (*onIdle)();
    void (*onKeyboard)(unsigned char key, int x, int y);

public:
    GlutWrapper():onPreDraw(0),onDraw(0),onIdle(0),onKeyboard(0){}
    
    void init(int *argc, char * *argv);

    /* 定义对键盘的响应函数 */
    static void keyboard(unsigned char key, int x, int y) {
        /*按Esc键退出*/
        if(27 == key){
            exit(0);
        }
    }
};


#endif //INC_1_7_TEXTURE2PP_GLUTWRAPER_H
