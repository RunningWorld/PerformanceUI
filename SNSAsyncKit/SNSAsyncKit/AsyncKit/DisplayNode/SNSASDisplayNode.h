//
//  SNSASDisplayNode.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SNSASLocking.h"
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

#define VISIBILITY_NOTIFICATIONS_DISABLED_BITS 4

@interface SNSASDisplayNode : NSObject <SNSASLocking>

@end

@interface SNSASDisplayNode (UIViewBridge)

@property (nullable) id contents;

/**
 * Marks the view as needing display. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 */
- (void)setNeedsDisplay;

/**
 * Marks the node as needing layout. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 *
 * If the node determines its own desired layout size will change in the next layout pass, it will propagate this
 * information up the tree so its parents can have a chance to consider and apply if necessary the new size onto the node.
 *
 * Note: ASCellNode has special behavior in that calling this method will automatically notify
 * the containing ASTableView / ASCollectionView that the cell should be resized, if necessary.
 */
- (void)setNeedsLayout;

/**
 * Performs a layout pass on the node. Convenience for use whether the view / layer is loaded or not. Safe to call from a background thread.
 */
- (void)layoutIfNeeded;

@end

NS_ASSUME_NONNULL_END

