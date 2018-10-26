//
//  SNSASBaseDefines.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#define AS_EXTERN FOUNDATION_EXTERN

#ifdef __GNUC__
# define ASDISPLAYNODE_GNUC(major, minor) \
(__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor)))
#else
# define ASDISPLAYNODE_GNUC(major, minor) 0
#endif

#ifndef ASDISPLAYNODE_INLINE
# if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#  define ASDISPLAYNODE_INLINE static inline
# elif defined (__MWERKS__) || defined (__cplusplus)
#  define ASDISPLAYNODE_INLINE static inline
# elif ASDISPLAYNODE_GNUC (3, 0)
#  define ASDISPLAYNODE_INLINE static __inline__ __attribute__ ((always_inline))
# else
#  define ASDISPLAYNODE_INLINE static
# endif
#endif

#if __has_attribute(warn_unused_result)
#define AS_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
#define AS_WARN_UNUSED_RESULT
#endif

#if defined(__cplusplus)
# define var auto
# define let const auto
#else
# define var __auto_type
# define let const __auto_type
#endif

#if __has_attribute(noescape)
#define AS_NOESCAPE __attribute__((noescape))
#else
#define AS_NOESCAPE
#endif

