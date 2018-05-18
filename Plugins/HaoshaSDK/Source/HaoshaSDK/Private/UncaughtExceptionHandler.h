//
//  UncaughtExceptionHandler.h
//  UncaughtExceptions
//
//  Created by Matt Gallagher on 2010/05/25.
//  Copyright 2010 Matt Gallagher. All rights reserved.
//
//  Permission is given to use this source code file, free of charge, in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//
#if PLATFORM_IOS
#import <UIKit/UIKit.h>

#define APIVersion [[NSUserDefaults standardUserDefaults] objectForKey:@"APIVersion"]

#define USERID   [[NSUserDefaults standardUserDefaults] objectForKey:@"UserID"]

#define ErrorLog [[NSUserDefaults standardUserDefaults] objectForKey:@"ErrorLog"]

#define AppVersion [[NSUserDefaults standardUserDefaults] objectForKey:@"AppVersion"]

@interface UncaughtExceptionHandler : NSObject{
	BOOL dismissed;
}

@end
void HandleException(NSException *exception);
void SignalHandler(int signal);


void InstallUncaughtExceptionHandler(void);
#endif
