#ifndef DEFINES_H
#define DEFINES_H



#define DEBUG_LEVEL 1

// debug level 0 = only one try catch at Main()
// debug level 1 = various try catch throught the code
// debug level 2 =

#ifndef repeat
    #define repeat(i, t) for(unsigned end___ = t, i = 0; i < end___; ++i)
    #define repeatI(i, t) for(unsigned i = 0; i < t; ++i)
#endif
#ifndef repeatInt
    #define repeatInt(i, t) for(int i = 0; i < t; ++i)
#endif
#ifndef iterate
    #define iterate(iteratorType, iterator, iterable) for(iteratorType iterator = (iterable).begin(), end___ = (iterable).end(); iterator!=end___; ++iterator)
#endif
#define range(varName, start, end) for(int varName=start, end___ = end;varName<end___;++varName)
#define rangeInt(varName, start, end) for(int varName=start, end___ = end;varName<end___;++varName)
#define SQUARED(x) ((x)*(x))
#define SWAP(a, b) {int aux = a;a = b;b = aux;}

#endif
