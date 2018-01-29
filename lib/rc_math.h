#ifndef __RC_MATH_H__
#define __RC_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef RC_ABS
#define RC_ABS(data) (((data) < 0) ? -(data) : (data) )                             
#endif

#ifdef __cplusplus
}
#endif

#endif