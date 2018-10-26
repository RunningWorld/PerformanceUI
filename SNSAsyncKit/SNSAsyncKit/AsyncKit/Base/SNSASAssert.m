//
//  SNSASAseert.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/15.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASAssert.h"
#import <pthread.h>
#import <Foundation/NSException.h>

static _Thread_local int tls_mainThreadAssertionsDisabledCount;
BOOL ASMainThreadAssertionsAreDisabled() {
    return tls_mainThreadAssertionsDisabledCount > 0;
}
