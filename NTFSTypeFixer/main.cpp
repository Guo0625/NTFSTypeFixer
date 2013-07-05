//
//  main.cpp
//  NTFSTypeFixer
//
//  Created by Sun Junwen on 13-7-5.
//  Copyright (c) 2013 Sun Junwen. All rights reserved.
//

#include <string>

#include "FSWatcher.h"

using namespace std;

int main(int argc, const char * argv[])
{
    string strPath("/Volumes/Windows/");
    
    StartFsEvent(strPath);
    
    // Start fix thread.
    
    
    EndFsEvent();
    
    return 0;
}

