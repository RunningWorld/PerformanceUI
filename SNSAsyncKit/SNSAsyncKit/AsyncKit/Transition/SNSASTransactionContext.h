//
//  SNSASTransactionContext.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/12.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>

@class SNSASTransactionContext;

@protocol SNSASTransitionContextCompletionDelegate <NSObject>

- (void)transitionContext:(SNSASTransactionContext *)context didComplete:(BOOL)didComplete;

@end

@interface SNSASTransactionContext : NSObject

@end


