// The as/400 doesn't have the C9X standard include stdint,
// so we're pretending it does. (Used in pyport.h)

#include <qleawi.h>
#define HAVE_UINTPTR_T
typedef _OPENPTR uintptr_t;
typedef _OPENPTR intptr_t;
