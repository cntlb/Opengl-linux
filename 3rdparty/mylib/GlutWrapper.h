//
// Created by jmu on 18-3-2.
//

#ifndef INC_1_7_TEXTURE2PP_GLUTWRAPER_H
#define INC_1_7_TEXTURE2PP_GLUTWRAPER_H

#include <cstdlib>

struct GlutWrapper;
/*
 * 不接受参数,返回void
 */
typedef void (*FUNC_VV)();

/*
 * 接受unsigned char key, int x, int y, 返回void
 * @param key 按键码
 */
typedef void (*OnKeyboard)(unsigned char key, int x, int y);

struct GlutWrapper {
public:
    FUNC_VV onPreDraw;
    FUNC_VV onDraw;
    FUNC_VV onIdle;
    OnKeyboard onKeyboard;

public:
    GlutWrapper();

    GlutWrapper(FUNC_VV onPreDraw, FUNC_VV onDraw, FUNC_VV onIdle = 0);

    virtual uint getWidth() const;

    virtual uint getHigh() const;

    void init(int *argc, char **argv);
};


#endif //INC_1_7_TEXTURE2PP_GLUTWRAPER_H
