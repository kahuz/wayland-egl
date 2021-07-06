#ifndef __LOGGER_H__
#define __LOGGER_H__

#if DEBUG
#define Log( fmt, args...) printf("[%s : %d] " fmt "\n", __func__, __LINE__, ## args) 
#define Err( fmt, args...) printf("Error ! " fmt "\n", ##args)
#else
#define Log( fmt, args...)
#define Err( fmt, args...)
#endif	// #if DEBUG

#endif // #ifndef __LOGGER_H__
