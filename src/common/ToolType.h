#ifndef TOOLTYPE_H
#define TOOLTYPE_H

typedef struct _TOOL_TYPE{
    enum Type{
        EMPTY = 0,
        SELECT = 1,
        PENCIL,
        RECTANGLE,
        ELLIPSE,
        TRIANGLE,
        LINE,
        ARC,
        ERASER,
        UNDO,
        REDO,
        MOVE,
        ZOOMIN,
        ZOOMOUT,
        RESET,
        CLEAR,
        OPENFILE
    };
}TOOL_TYPE;

#endif // TOOLTYPE_H
