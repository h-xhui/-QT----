#ifndef CODEUTIL_H
#define CODEUTIL_H

#include <QByteArray>

class CodeUtil{
public:
    CodeUtil(){};
    ~CodeUtil(){};
    void getMousePressEventCode(QByteArray* array) {
        array->resize(3);
        array[0] = array[1] = array[2] = 0;
    }
};

#endif // CODEUTIL_H
