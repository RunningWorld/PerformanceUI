//
//  SNSASBlockTypes.h
//  SNSAsyncKit
//
//  Created by caolidong on 2018/10/12.
//  Copyright © 2018年 Lee. All rights reserved.
//

#ifndef SNSASBlockTypes_h
#define SNSASBlockTypes_h

#import <Foundation/Foundation.h>

// Type for the cancellation checker block passed into the async display blocks. YES means the operation has been cancelled, NO means continue.
typedef BOOL(^asdisplaynode_iscancelled_block_t)(void);

#endif /* SNSASBlockTypes_h */
