//
//  SNSASDisplayLayer.m
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASDisplayLayer.h"
#import "SNSASAssert.h"
#import "SNSASDisplayNode.h"

@implementation SNSASDisplayLayer
{
    BOOL _attemptedDisplayWhileZeroSized;
    struct {
        BOOL delegateDidChangeBounds:1;
    } _delegateFlags;
}

//@dynamic displaysAsynchronously;

#pragma mark - Properties

- (void)setDelegate:(id)delegate
{
    [super setDelegate:delegate];
    _delegateFlags.delegateDidChangeBounds = [delegate respondsToSelector:@selector(layer:didChangeBoundsWithOldValue:newValue:)];
}

- (void)setBounds:(CGRect)bounds
{
    [super setBounds:bounds];
    self.asyncdisplaykit_node.threadSafeBounds = bounds;
    
    if (_attemptedDisplayWhileZeroSized && CGRectIsEmpty(bounds) == NO && self.needsDisplayOnBoundsChange == NO) {
        _attemptedDisplayWhileZeroSized = NO;
        [self setNeedsDisplay];
    }
}

- (void)layoutSublayers
{
    ASDisplayNodeAssertMainThread();
    [super layoutSublayers];
    
    [self.asyncdisplaykit_node __layout];
}

- (void)setNeedsDisplay
{
    ASDisplayNodeAssertMainThread();
    
    // FIXME: Reconsider whether we should cancel a display in progress.
    // We should definitely cancel a display that is scheduled, but unstarted display.
    [self cancelAsyncDisplay];
    
    // Short circuit if display is suspended. When resumed, we will setNeedsDisplay at that time.
    [super setNeedsDisplay];
}

#pragma mark -

+ (dispatch_queue_t)displayQueue
{
    static dispatch_queue_t displayQueue = NULL;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        displayQueue = dispatch_queue_create("org.AsyncDisplayKit.ASDisplayLayer.displayQueue", DISPATCH_QUEUE_CONCURRENT);
        // we use the highpri queue to prioritize UI rendering over other async operations
        dispatch_set_target_queue(displayQueue, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0));
    });
    
    return displayQueue;
}

+ (id)defaultValueForKey:(NSString *)key
{
    if ([key isEqualToString:@"displaysAsynchronously"]) {
        return @YES;
    } else if ([key isEqualToString:@"opaque"]) {
        return @YES;
    } else {
        return [super defaultValueForKey:key];
    }
}

#pragma mark - Display
- (void)display
{
    ASDisplayNodeAssertMainThread();
    [self _hackResetNeedsDisplay];

    [self display:self.displaysAsynchronously];
}

- (void)display:(BOOL)asynchronously
{
    if (CGRectIsEmpty(self.bounds)) {
        _attemptedDisplayWhileZeroSized = YES;
    }
    
    [self.asyncDelegate displayAsyncLayer:self asynchronously:asynchronously];
}

- (void)_hackResetNeedsDisplay
{
//    ASDisplayNodeAssertMainThread();
    // Don't listen to our subclasses crazy ideas about setContents by going through super
    super.contents = super.contents;
}

- (void)cancelAsyncDisplay
{
    ASDisplayNodeAssertMainThread();
    
    [self.asyncDelegate cancelDisplayAsyncLayer:self];
}

// e.g. <MYTextNodeLayer: 0xFFFFFF; node = <MYTextNode: 0xFFFFFFE; name = "Username node for user 179">>
- (NSString *)description
{
    NSMutableString *description = [[super description] mutableCopy];
    SNSASDisplayNode *node = self.asyncdisplaykit_node;
    if (node != nil) {
        NSString *classString = [NSString stringWithFormat:@"%s-", object_getClassName(node)];
        [description replaceOccurrencesOfString:@"_ASDisplay" withString:classString options:kNilOptions range:NSMakeRange(0, description.length)];
        NSUInteger insertionIndex = [description rangeOfString:@">"].location;
        if (insertionIndex != NSNotFound) {
            NSString *nodeString = [NSString stringWithFormat:@"; node = %@", node];
            [description insertString:nodeString atIndex:insertionIndex];
        }
    }
    return description;
}

@end
