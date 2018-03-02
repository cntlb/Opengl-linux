//
// Created by jmu on 18-2-28.
//

#ifndef HELLO_TRIANGLE_LOG_H
#define HELLO_TRIANGLE_LOG_H

#include <stdio.h>

#define LOGE(fmt, ...) printf("[%s:%d]" fmt "\n",  LOG_TAG, __LINE__, ##__VA_ARGS__);
#endif //HELLO_TRIANGLE_LOG_H
