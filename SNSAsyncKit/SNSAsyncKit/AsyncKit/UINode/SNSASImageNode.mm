//
//  SNSASImageNode.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASImageNode.h"
#import "SNSASDisplayNodeInternal.h"
#import "SNSASEqualityHelpers.h"
#import "SNSASThread.h"
#import "SNSASInternalHelpers.h"

static const CGSize kMinReleaseImageOnBackgroundSize = {20.0, 20.0};

@implementation SNSASImageNode
{
@private
    UIImage *_image;
}

#pragma mark - Setter / Getter

- (void)setImage:(UIImage *)image
{
    ASDN::MutexLocker l(__instanceLock__);
    [self _locked_setImage:image];
}

- (void)_locked_setImage:(UIImage *)image
{
    ASAssertLocked(__instanceLock__);
    if (ASObjectIsEqual(_image, image)) {
        return;
    }
    
    UIImage *oldImage = _image;
    _image = image;
    
    if (image != nil) {
        // We explicitly call setNeedsDisplay in this case, although we know setNeedsDisplay will be called with lock held.
        // Therefore we have to be careful in methods that are involved with setNeedsDisplay to not run into a deadlock
        [self setNeedsDisplay];
    } else {
        self.contents = nil;
    }
    
    // Destruction of bigger images on the main thread can be expensive
    // and can take some time, so we dispatch onto a bg queue to
    // actually dealloc.
    CGSize oldImageSize = oldImage.size;
    BOOL shouldReleaseImageOnBackgroundThread = oldImageSize.width > kMinReleaseImageOnBackgroundSize.width
    || oldImageSize.height > kMinReleaseImageOnBackgroundSize.height;
    if (shouldReleaseImageOnBackgroundThread) {
        ASPerformBackgroundDeallocation(&oldImage);
    }
}

- (UIImage *)image
{
    return ASLockedSelf(_image);
}

@end
