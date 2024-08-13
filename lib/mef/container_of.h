#ifndef container_of
#include <stddef.h>
#define container_of(ptr,tipo,miembro) ((tipo*)((char*)(1?(ptr):&((tipo*)0)->miembro)-offsetof(tipo,miembro))) 
#endif

