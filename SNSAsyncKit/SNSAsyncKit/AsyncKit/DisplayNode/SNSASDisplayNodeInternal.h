//
//  SNSASDisplayNodeInternal.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/5.
//  Copyright © 2018年 Lee. All rights reserved.
//

#import "SNSASDisplayNode.h"
#import "SNSASThread.h"
#import "SNSASTransactionContext.h"

#define VISIBILITY_NOTIFICATIONS_DISABLED_BITS 4

@interface SNSASDisplayNode () <SNSASTransitionContextCompletionDelegate>
{
@package
    ASDN::RecursiveMutex __instanceLock__;
    UIView *_view;
    CALayer *_layer;
    
    struct ASDisplayNodeFlags {
        // public properties
        unsigned viewEverHadAGestureRecognizerAttached:1;
        unsigned layerBacked:1;
        unsigned displaysAsynchronously:1;
        unsigned rasterizesSubtree:1;
        unsigned shouldBypassEnsureDisplay:1;
        unsigned displaySuspended:1;
        unsigned shouldAnimateSizeChanges:1;

        // Wrapped view handling

        // The layer contents should not be cleared in case the node is wrapping a UIImageView.UIImageView is specifically
        // optimized for performance and does not use the usual way to provide the contents of the CALayer via the
        // CALayerDelegate method that backs the UIImageView.
        unsigned canClearContentsOfLayer:1;

        // Prevent calling setNeedsDisplay on a layer that backs a UIImageView. Usually calling setNeedsDisplay on a CALayer
        // triggers a recreation of the contents of layer unfortunately calling it on a CALayer that backs a UIImageView
        // it goes through the normal flow to assign the contents to a layer via the CALayerDelegate methods. Unfortunately
        // UIImageView does not do recreate the layer contents the usual way, it actually does not implement some of the
        // methods at all instead it throws away the contents of the layer and nothing will show up.
        unsigned canCallSetNeedsDisplayOfLayer:1;

        unsigned implementsDrawRect:1;
        unsigned implementsImageDisplay:1;
        unsigned implementsDrawParameters:1;

        // internal state
        unsigned isEnteringHierarchy:1;
        unsigned isExitingHierarchy:1;
        unsigned isInHierarchy:1;
        unsigned visibilityNotificationsDisabled:VISIBILITY_NOTIFICATIONS_DISABLED_BITS;
        unsigned isDeallocating:1;
    } _flags;
@protected
    SNSASDisplayNode * __weak _supernode;
    NSMutableArray<SNSASDisplayNode *> *_subnodes;
}

@end
