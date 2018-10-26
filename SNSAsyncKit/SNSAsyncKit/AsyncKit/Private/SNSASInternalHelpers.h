//
//  SNSASInternalHelpers.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/11.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SNSASBaseDefines.h"

NS_ASSUME_NONNULL_BEGIN

/// For deallocation of objects on a background thread without GCD overhead / thread explosion
AS_EXTERN void ASPerformBackgroundDeallocation(id __strong _Nullable * _Nonnull object);

NS_ASSUME_NONNULL_END
