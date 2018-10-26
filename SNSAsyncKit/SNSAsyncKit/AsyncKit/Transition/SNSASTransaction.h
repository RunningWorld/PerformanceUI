//
//  SNSASTransaction.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class SNSASTransaction;

typedef void(^asyncdisplaykit_async_transaction_completion_block_t)(SNSASTransaction *completedTransaction, BOOL canceled);
typedef id<NSObject> _Nullable(^asyncdisplaykit_async_transaction_operation_block_t)(void);
typedef void(^asyncdisplaykit_async_transaction_operation_completion_block_t)(id _Nullable value, BOOL canceled);

@interface SNSASTransaction : NSObject



@end

NS_ASSUME_NONNULL_END
