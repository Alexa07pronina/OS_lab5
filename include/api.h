#ifndef LAB5_API_H
#define LAB5_API_H

#ifdef _WIN32
#define LAB5_CALL __cdecl

#if defined(LAB5_EXPORTS)
#define LAB5_API __declspec(dllexport)

#elif defined(LAB5_IMPORTS)
#define LAB5_API __declspec(dllimport)

#else
#define LAB5_API
#endif

#else
#define LAB5_CALL
#define LAB5_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

LAB5_API float LAB5_CALL Square(float A, float B);

LAB5_API char* LAB5_CALL translation(long x);

#ifdef __cplusplus
}
#endif

typedef float (LAB5_CALL *SquareFunction)(float, float);
typedef char* (LAB5_CALL *TranslationFunction)(long);

#endif
