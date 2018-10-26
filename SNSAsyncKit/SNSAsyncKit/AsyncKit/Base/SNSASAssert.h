//
//  SNSASAssert.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/15.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/NSException.h>
#import "SNSASBaseDefines.h"
#import <pthread.h>

AS_EXTERN BOOL ASMainThreadAssertionsAreDisabled(void);

#define ASDisplayNodeAssert(condition, desc, ...) NSAssert(condition, desc, ##__VA_ARGS__)
#define ASDisplayNodeCAssert(condition, desc, ...) NSCAssert(condition, desc, ##__VA_ARGS__)

#define ASDisplayNodeAssertFalse(condition) ASDisplayNodeAssert(!(condition), @"Expected %s to be false.", #condition)
#define ASDisplayNodeCAssertFalse(condition) ASDisplayNodeCAssert(!(condition), @"Expected %s to be false.", #condition)

#define ASDisplayNodeCFailAssert(desc, ...) ASDisplayNodeCAssert(NO, desc, ##__VA_ARGS__)
#define ASDisplayNodeCAssertTrue(condition) ASDisplayNodeCAssert((condition), @"Expected %s to be true.", #condition)

#define ASDisplayNodeAssertMainThread() ASDisplayNodeAssert(ASMainThreadAssertionsAreDisabled() || 0 != pthread_main_np(), @"This method must be called on the main thread")
