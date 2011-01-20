/*
 * Copyright (c) 2010-2011
 * ARCHSTONE Project.
 * University of Southern California/Information Sciences Institute.
 * All rights reserved.
 *
 * Created by Xi Yang 2011
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
 
#ifndef __ACTION_HH__
#define __ACTION_HH__
#include <list>
#include "types.hh"
#include "event.hh"

using namespace std;

typedef enum {
    _Idle = 0,
    _Started = 1,
    _PendingMsg,
    _ReceivedMsg,
    _PendingChildren,
    _Cancelled,
    _Failed,
    _Finished
} ActionState;

class Message;
class ComputeWorker;
class Action: public Event
{
protected:
    ActionState state;
    ComputeWorker* worker;
    list<Action*> children;
    list<Message*> messages;

public:
    Action(ComputeWorker* w):state(_Idle), worker(w) { assert(w != NULL); }
    virtual ~Action() { }
    ActionState GetState() { return state; }
    void SetState(ActionState s) { this->state = s; }
    list<Message*>& GetMessages() { return messages; }

    virtual void Run();
    virtual void Process();
    virtual void Next();
    virtual void Wait();
    virtual void Finish();
};


#endif
