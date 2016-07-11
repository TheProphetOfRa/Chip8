//
//  opcodes.h
//  Chip8 Emulator
//
//  Created by David Hodgkinson on 24/11/2015.
//  Copyright © 2015 David Hodgkinson. All rights reserved.
//

#ifndef opcodes_h
#define opcodes_h

#include <functional>
#include <map>

namespace Chip8
{
    void registerOpCodes(std::map<unsigned short, std::function<void(unsigned short)>>& ops);
}

#endif /* opcodes_h */
