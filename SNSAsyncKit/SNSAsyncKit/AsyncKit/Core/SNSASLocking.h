//
//  SNSASLocking.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/12.
//  Copyright © 2018年 Lee. All rights reserved.
//

#ifndef SNSASLocking_h
#define SNSASLocking_h

#import <Foundation/Foundation.h>

/**
 * An extension of NSLocking that supports -tryLock.
 */
@protocol SNSASLocking <NSLocking>

/// Try to take lock without blocking. Returns whether the lock was taken.
- (BOOL)tryLock;

@end

#endif /* SNSASLocking_h */
