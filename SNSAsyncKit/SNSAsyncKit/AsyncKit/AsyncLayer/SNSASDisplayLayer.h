//
//  SNSASDisplayLayer.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SNSASBlockTypes.h"
#import "SNSASBaseDefines.h"

NS_ASSUME_NONNULL_BEGIN

@class SNSASDisplayNode;

@protocol SNSDisplayLayerDelegate;

@interface SNSASDisplayLayer : CALayer

/**
 @discussion This property overrides the CALayer category method which implements this via associated objects.
 This should result in much better performance for _ASDisplayLayers.
 */
@property (nullable, nonatomic, weak) SNSASDisplayNode *asyncdisplaykit_node;

/**
 @summary Set to YES to enable asynchronous display for the receiver.

 @default YES (note that this might change for subclasses)
 */
@property (nonatomic) BOOL displaysAsynchronously;

/**
 @summary Cancels any pending async display.
 
 @desc If the receiver has had display called and is waiting for the dispatched async display to be executed, this will
 cancel that dispatched async display.  This method is useful to call when removing the receiver from the window.
 */
- (void)cancelAsyncDisplay;

/**
 @summary Delegate for asynchronous display of the layer. This should be the node (default) unless you REALLY know what you're doing.
 
 @desc The asyncDelegate will have the opportunity to override the methods related to async display.
 */
@property (nullable, weak) id<SNSDisplayLayerDelegate> asyncDelegate;

@end

@protocol SNSDisplayLayerDelegate <NSObject>

@optional

// Called on the display queue and/or main queue (MUST BE THREAD SAFE)

/**
 @summary Delegate method to draw layer contents into a CGBitmapContext. The current UIGraphics context will be set to an appropriate context.
 @param parameters An object describing all of the properties you need to draw. Return this from -drawParametersForAsyncLayer:
 @param isCancelledBlock Execute this block to check whether the current drawing operation has been cancelled to avoid unnecessary work. A return value of YES means cancel drawing and return.
 @param isRasterizing YES if the layer is being rasterized into another layer, in which case drawRect: probably wants to avoid doing things like filling its bounds with a zero-alpha color to clear the backing store.
 */
+ (void)drawRect:(CGRect)bounds
  withParameters:(nullable id)parameters
     isCancelled:(AS_NOESCAPE asdisplaynode_iscancelled_block_t)isCancelledBlock
   isRasterizing:(BOOL)isRasterizing;

///**
// @summary Delegate override to provide new layer contents as a UIImage.
// @param parameters An object describing all of the properties you need to draw. Return this from -drawParametersForAsyncLayer:
// @param isCancelledBlock Execute this block to check whether the current drawing operation has been cancelled to avoid unnecessary work. A return value of YES means cancel drawing and return.
// @return A UIImage with contents that are ready to display on the main thread. Make sure that the image is already decoded before returning it here.
// */
+ (UIImage *)displayWithParameters:(nullable id<NSObject>)parameters
                       isCancelled:(AS_NOESCAPE asdisplaynode_iscancelled_block_t)isCancelledBlock;

// Called on the main thread only

/**
 @summary Delegate override for drawParameters
 */
- (NSObject *)drawParametersForAsyncLayer:(SNSASDisplayLayer *)layer;

/**
 @summary Delegate override for willDisplay
 */
- (void)willDisplayAsyncLayer:(SNSASDisplayLayer *)layer asynchronously:(BOOL)asynchronously;

/**
 @summary Delegate override for didDisplay
 */
- (void)didDisplayAsyncLayer:(SNSASDisplayLayer *)layer;

/**
 @summary Delegate callback to display a layer, synchronously or asynchronously.  'asyncLayer' does not necessarily need to exist (can be nil).  Typically, a delegate will display/draw its own contents and then set .contents on the layer when finished.
 */
- (void)displayAsyncLayer:(SNSASDisplayLayer *)asyncLayer asynchronously:(BOOL)asynchronously;

/**
 @summary Delegate callback to handle a layer which requests its asynchronous display be cancelled.
 */
- (void)cancelDisplayAsyncLayer:(SNSASDisplayLayer *)asyncLayer;

@end

NS_ASSUME_NONNULL_END
