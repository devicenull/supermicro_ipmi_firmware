
#ifndef _HEAPALLOC_H_
#define _HEAPALLOC_H_  

#ifdef _HEAPALLOC_C_

void *HeapAlloc(DWORD dw_size);

#else

extern void *HeapAlloc(DWORD dw_size);

#endif   

#endif  





