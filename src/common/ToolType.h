#ifndef TOOLTYPE_H
#define TOOLTYPE_H

typedef struct _TOOL_TYPE{
    enum Type{
        SELECT = 1,
        PENCIL,
        RECTANGLE,
        ELLIPSE,
        TRIANGLE,
        LINE,
        ERASER,
        UNDO,
        REDO,
        MOVE,
        ZOOMIN,
        ZOOMOUT,
        RESET,
        CLEAR
    };
}TOOL_TYPE;

#endif // TOOLTYPE_H
