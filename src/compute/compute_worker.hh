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


#ifndef __COMPUTE_WORKER_HH__
#define __COMPUTE_WORKER_HH__
#include <list>
#include "types.hh"
#include "event.hh"
#include "exception.hh"
#include "thread.hh"
#include "api.hh"
#include "action.hh"

using namespace std;

class ComputeWorker: public ThreadPortScheduler
{
protected:
    list<Action*> actions;//workflow-actions

public:
    ComputeWorker(string n):ThreadPortScheduler(n){ }
    virtual ~ComputeWorker();
    string& GetName() { return this->msgPort->GetName();}
    list<Action*>& GetActions() { return actions; }
    void* Run();
    virtual void* hookRun();
    virtual void hookHandleMessage();
};


class ComputeWorkerFactory
{
private:
    static list<ComputeWorker*> workers;
    static int serialNum;
    static int NewWorkerNum() { return ++serialNum; }
    static Lock cwfLock;

public:
    static int GetWorkerNum() { return serialNum; }
    static list<ComputeWorker*>& GetComputeWorkers() { return workers; }
    static ComputeWorker* CreateComputeWorker(string type);
    static ComputeWorker* LookupComputeWorker(string name);
    static void RemoveComputeWorker(string name);
};

#endif
