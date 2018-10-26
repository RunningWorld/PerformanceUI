//
//  SNSASImageNode.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SNSASControlNode.h"

NS_ASSUME_NONNULL_BEGIN

/**
 * Image modification block.  Use to transform an image before display.
 *
 * @param image The image to be displayed.
 *
 * @return A transformed image.
 */
typedef UIImage * _Nullable (^asimagenode_modification_block_t)(UIImage *image);

@interface SNSASImageNode : SNSASControlNode

@property (nullable) UIImage *image;

@end

NS_ASSUME_NONNULL_END
