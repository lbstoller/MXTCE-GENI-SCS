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
#include "tewg.hh"
#include "user_constraint.hh"

using namespace std;

class ComputeWorkflowData: public WorkData
{
public:
    Apimsg_user_constraint* userConstraint;
    list<Apimsg_user_constraint*>* userConsList;
    TEWG* tewg;
    string errMsg;
    vector<TPath*>* ksp;
    vector<TPath*>* feasiblePaths;
    void _Init() {
        userConstraint = NULL;
        userConsList = NULL;
        tewg = NULL;
        ksp = NULL;
        feasiblePaths = NULL;
    }

public:
    ComputeWorkflowData(): WorkData() { _Init(); }
    ~ComputeWorkflowData() {}
    void Cleanup() {
        // TODO: 
    }
};

class ComputeWorker: public ThreadPortScheduler
{
protected:
    ComputeWorkflowData workflowData; //  data global for the workflow.
    list<Action*> actions;//workflow-actions

    void HandleException(ComputeThreadException& e);

public:
    ComputeWorker(string n):ThreadPortScheduler(n) { }
    virtual ~ComputeWorker();
    string& GetName() { return portName;}
    list<Action*>& GetActions() { return actions; }
    Action* LookupAction(string& context, string& name);
    void* Run();
    virtual void* hookRun();
    virtual void hookHandleMessage();
    virtual void* GetWorkflowData(const char* paramName);
    virtual void* GetWorkflowData(string& paramName);
    virtual void SetWorkflowData(const char* paramName, void* paramPtr);
    virtual void SetWorkflowData(string& paramName, void* paramPtr);
    virtual void* GetContextActionData(string& contextName, string& actionName, string& dataName);
    virtual void* GetContextActionData(const char* contextName, const char* actionName, const char* dataName);
    virtual void* GetContextActionData(string& contextName, string& actionName, const char* dataName);
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

class ComputeResult 
{
protected:
    string gri;
    string pathId;
    TPath* pathInfo;
    list<TPath*> alterPaths;
    list<TPath*> flexAlterPaths;
    TGraph* graphInfo;
    string errMsg;

public:
    ComputeResult(string& g): gri(g), pathInfo(NULL), graphInfo(NULL) { }
    virtual ~ComputeResult() { delete pathInfo; }
    string& GetGri() { return gri; }
    void SetGri(string& g) { gri = g; }
    string& GetPathId() { return pathId; }
    void SetPathId(string& s) { pathId = s; }
    TPath* GetPathInfo() { return pathInfo; }
    void SetPathInfo(TPath* p) { pathInfo = p; }
    list<TPath*>& GetAlterPaths() { return alterPaths; }
    list<TPath*>& GetFlexAlterPaths() { return flexAlterPaths; }
    string& GetErrMessage() { return errMsg; }
    TGraph* GetGraphInfo() { return graphInfo; }
    void SetGraphInfo(TGraph* g) { graphInfo = g; }
    void SetErrMessage(string& s) {errMsg = s; }
    static void RegulatePathInfo(TPath* path);
};

#endif
