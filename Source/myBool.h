#ifndef __MY_BOOL__
#define __MY_BOOL__

enum mBool {mFALSE = 0x00, mTRUE = !mFALSE};

enum mFlagStatus {mRESET = 0x00, mSET = !mRESET};

enum mFunctionalState {mDISABLE = 0x00, mENABLE = !mDISABLE};

#endif
// end of file
