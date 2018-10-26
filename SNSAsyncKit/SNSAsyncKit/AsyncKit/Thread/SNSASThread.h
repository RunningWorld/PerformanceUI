//
//  SNSAsyncTread.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SNSASBaseDefines.h"
#import "SNSASAssert.h"

#import <assert.h>
#import <os/lock.h>
#import <pthread.h>
#import <stdbool.h>
#import <stdlib.h>


ASDISPLAYNODE_INLINE AS_WARN_UNUSED_RESULT BOOL ASDisplayNodeThreadIsMain()
{
    return 0 != pthread_main_np();
}

/**
 * Adds the lock to the current scope.
 *
 * A C version of the C++ lockers. Pass in any id<NSLocking>.
 * One benefit this has over C++ lockers is that the lock is retained. We
 * had bugs in the past where an object would be deallocated while someone
 * had locked its instanceLock, and we'd get a crash. This macro
 * retains the locked object until it can be unlocked, which is nice.
 */
#define ASLockScope(nsLocking) \
id<NSLocking> __lockToken __attribute__((cleanup(_ASLockScopeCleanup))) NS_VALID_UNTIL_END_OF_SCOPE = nsLocking; \
[__lockToken lock];

/// Same as ASLockScope(1) but lock isn't retained (be careful).
#define ASLockScopeUnowned(nsLocking) \
__unsafe_unretained id<NSLocking> __lockToken __attribute__((cleanup(_ASLockScopeUnownedCleanup))) = nsLocking; \
[__lockToken lock];

ASDISPLAYNODE_INLINE void _ASLockScopeCleanup(id<NSLocking> __strong * const lockPtr) {
    [*lockPtr unlock];
}

ASDISPLAYNODE_INLINE void _ASLockScopeUnownedCleanup(id<NSLocking> __unsafe_unretained * const lockPtr) {
    [*lockPtr unlock];
}

/**
 * Same as ASLockScope(1) but it uses self, so we can skip retain/release.
 */
#define ASLockScopeSelf() ASLockScopeUnowned(self)

/// One-liner while holding the lock.
#define ASLocked(nsLocking, expr) ({ ASLockScope(nsLocking); expr; })

/// Faster self-version.
#define ASLockedSelf(expr) ({ ASLockScopeSelf(); expr; })

#define ASLockedSelfCompareAssign(lvalue, newValue) \
ASLockedSelf(ASCompareAssign(lvalue, newValue))

#define ASLockedSelfCompareAssignObjects(lvalue, newValue) \
ASLockedSelf(ASCompareAssignObjects(lvalue, newValue))

#define ASLockedSelfCompareAssignCustom(lvalue, newValue, isequal) \
ASLockedSelf(ASCompareAssignCustom(lvalue, newValue, isequal))

#define ASLockedSelfCompareAssignCopy(lvalue, obj) \
ASLockedSelf(ASCompareAssignCopy(lvalue, obj))

#define ASUnlockScope(nsLocking) \
id<NSLocking> __lockToken __attribute__((cleanup(_ASUnlockScopeCleanup))) NS_VALID_UNTIL_END_OF_SCOPE = nsLocking; \
[__lockToken unlock];

#define ASSynthesizeLockingMethodsWithMutex(mutex) \
- (void)lock { mutex.lock(); } \
- (void)unlock { mutex.unlock(); } \
- (BOOL)tryLock { return mutex.tryLock(); }

#define ASSynthesizeLockingMethodsWithObject(object) \
- (void)lock { [object lock]; } \
- (void)unlock { [object unlock]; } \
- (BOOL)tryLock { return [object tryLock]; }

#define CHECK_LOCKING_SAFETY 1

#ifdef __cplusplus
#include <memory>

// This MUST always execute, even when assertions are disabled. Otherwise all lock operations become no-ops!
// (To be explicit, do not turn this into an NSAssert, assert(), or any other kind of statement where the
// evaluation of x_ can be compiled out.)
#define AS_POSIX_ASSERT_NOERR(x_) ({ \
__unused int res = (x_); \
ASDisplayNodeCAssert(res == 0, @"Expected %s to return 0, got %d instead. Error: %s", #x_, res, strerror(res)); \
})

#define ASAssertUnlocked(lock) ASDisplayNodeAssertFalse(lock.locked())
#define ASAssertLocked(lock) ASDisplayNodeAssert(lock.locked(), @"Lock must be held by current thread")

namespace ASDN {
    
    template<class T>
    class Locker
    {
        T &_l;
        
    public:
        Locker (T &l) noexcept : _l (l) {
            _l.lock ();
        }
        
        ~Locker () {
            _l.unlock ();
        }
        
        // non-copyable.
        Locker(const Locker<T>&) = delete;
        Locker &operator=(const Locker<T>&) = delete;
    };
    
    template<class T>
    class SharedLocker
    {
        std::shared_ptr<T> _l;
        
    public:
        SharedLocker (std::shared_ptr<T> const& l) noexcept : _l (l) {
            ASDisplayNodeCAssertTrue(_l != nullptr);
            _l->lock ();
        }
        
        ~SharedLocker () {
            _l->unlock ();
        }
        
        // non-copyable.
        SharedLocker(const SharedLocker<T>&) = delete;
        SharedLocker &operator=(const SharedLocker<T>&) = delete;
    };
    
    template<class T>
    class Unlocker
    {
        T &_l;
    public:
        Unlocker (T &l) noexcept : _l (l) { _l.unlock (); }
        ~Unlocker () {_l.lock ();}
        Unlocker(Unlocker<T>&) = delete;
        Unlocker &operator=(Unlocker<T>&) = delete;
    };
    
    // Silence unguarded availability warnings in here, because
    // perf is critical and we will check availability once
    // and not again.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunguarded-availability"
    struct Mutex
    {
        /// Constructs a non-recursive mutex (the default).
        Mutex () : Mutex (false) {}
        
        ~Mutex () {
            AS_POSIX_ASSERT_NOERR(pthread_mutex_destroy (&_m));
#if CHECK_LOCKING_SAFETY
            _owner = 0;
            _count = 0;
#endif
        }
        
        Mutex (const Mutex&) = delete;
        Mutex &operator=(const Mutex&) = delete;
        
        bool tryLock() {
            let result = pthread_mutex_trylock(&_m);
            if (result == 0) {
                return true;
            } else if (result == EBUSY) {
                return false;
            } else {
                ASDisplayNodeCFailAssert(@"Locking error: %s", strerror(result));
                return true; // if we return false we may enter an infinite loop.
            }
        }
        void lock() {
            AS_POSIX_ASSERT_NOERR(pthread_mutex_lock(&_m));
#if CHECK_LOCKING_SAFETY
            mach_port_t thread_id = pthread_mach_thread_np(pthread_self());
            if (thread_id != _owner) {
                // New owner. Since this mutex can't be acquired by another thread if there is an existing owner, _owner and _count must be 0.
                ASDisplayNodeCAssertTrue(0 == _owner);
                ASDisplayNodeCAssertTrue(0 == _count);
                _owner = thread_id;
            } else {
                // Existing owner tries to reacquire this (recursive) mutex. _count must already be positive.
                ASDisplayNodeCAssertTrue(_count > 0);
            }
            ++_count;
#endif
        }
        
        void unlock () {
#if CHECK_LOCKING_SAFETY
            mach_port_t thread_id = pthread_mach_thread_np(pthread_self());
            // Unlocking a mutex on an unowning thread causes undefined behaviour. Assert and fail early.
            ASDisplayNodeCAssertTrue(thread_id == _owner);
            // Current thread owns this mutex. _count must be positive.
            ASDisplayNodeCAssertTrue(_count > 0);
            --_count;
            if (0 == _count) {
                // Current thread is no longer the owner.
                _owner = 0;
            }
#endif
            AS_POSIX_ASSERT_NOERR(pthread_mutex_unlock(&_m));
        }
        
        pthread_mutex_t *mutex () { return &_m; }
        
#if CHECK_LOCKING_SAFETY
        bool locked() {
            return _count > 0 && pthread_mach_thread_np(pthread_self()) == _owner;
        }
#endif
        
    protected:
        explicit Mutex (bool recursive) {
            
            _recursive = recursive;
            
            if (!recursive) {
                AS_POSIX_ASSERT_NOERR(pthread_mutex_init (&_m, NULL));
            } else {
                // Fall back to recursive mutex.
                static pthread_mutexattr_t attr;
                static dispatch_once_t onceToken;
                dispatch_once(&onceToken, ^{
                    AS_POSIX_ASSERT_NOERR(pthread_mutexattr_init (&attr));
                    AS_POSIX_ASSERT_NOERR(pthread_mutexattr_settype (&attr, PTHREAD_MUTEX_RECURSIVE));
                });
                AS_POSIX_ASSERT_NOERR(pthread_mutex_init(&_m, &attr));
            }
#if CHECK_LOCKING_SAFETY
            _owner = 0;
            _count = 0;
#endif
        }
        
    private:
        BOOL _recursive;
        union {
            pthread_mutex_t _m;
        };
#if CHECK_LOCKING_SAFETY
        mach_port_t _owner;
        uint32_t _count;
#endif
    };
#pragma clang diagnostic pop // ignored "-Wunguarded-availability"
    
    /**
     Obj-C doesn't allow you to pass parameters to C++ ivar constructors.
     Provide a convenience to change the default from non-recursive to recursive.
     
     But wait! Recursive mutexes are a bad idea. Think twice before using one:
     
     http://www.zaval.org/resources/library/butenhof1.html
     http://www.fieryrobot.com/blog/2008/10/14/recursive-locks-will-kill-you/
     */
    struct RecursiveMutex : Mutex
    {
        RecursiveMutex () : Mutex (true) {}
    };
    
    typedef Locker<Mutex> MutexLocker;
    typedef SharedLocker<Mutex> MutexSharedLocker;
    typedef Unlocker<Mutex> MutexUnlocker;
    
    /**
     If you are creating a static mutex, use StaticMutex. This avoids expensive constructor overhead at startup (or worse, ordering
     issues between different static objects). It also avoids running a destructor on app exit time (needless expense).
     
     Note that you can, but should not, use StaticMutex for non-static objects. It will leak its mutex on destruction,
     so avoid that!
     */
    struct StaticMutex
    {
        StaticMutex () : _m (PTHREAD_MUTEX_INITIALIZER) {}
        
        // non-copyable.
        StaticMutex(const StaticMutex&) = delete;
        StaticMutex &operator=(const StaticMutex&) = delete;
        
        void lock () {
            AS_POSIX_ASSERT_NOERR(pthread_mutex_lock (this->mutex()));
        }
        
        void unlock () {
            AS_POSIX_ASSERT_NOERR(pthread_mutex_unlock (this->mutex()));
        }
        
        pthread_mutex_t *mutex () { return &_m; }
        
    private:
        pthread_mutex_t _m;
    };
    
    typedef Locker<StaticMutex> StaticMutexLocker;
    typedef Unlocker<StaticMutex> StaticMutexUnlocker;
    
} // namespace ASDN

#endif /* __cplusplus */
