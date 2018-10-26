//
//  SNSASEqualityHelpers.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/11.
//  Copyright © 2018年 Lee. All rights reserved.
//

#ifndef SNSASEqualityHelpers_h
#define SNSASEqualityHelpers_h

#import "SNSASBaseDefines.h"

/**
 @abstract Correctly equates two objects, including cases where both objects are nil. The latter is a case where `isEqual:` fails.
 @param obj The first object in the comparison. Can be nil.
 @param otherObj The second object in the comparison. Can be nil.
 @result YES if the objects are equal, including cases where both object are nil.
 */
ASDISPLAYNODE_INLINE BOOL ASObjectIsEqual(id<NSObject> obj, id<NSObject> otherObj)
{
    return obj == otherObj || [obj isEqual:otherObj];
}

#endif /* SNSASEqualityHelpers_h */
