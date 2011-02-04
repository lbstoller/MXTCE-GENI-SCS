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

#ifndef __RESOURCE_HH__
#define __RESOURCE_HH__

#include "types.hh"
#include "utils.hh"
#include <list>
#include <map>

using namespace std;


enum ResourceType
{
    RTYPE_DOMAIN = 0x01,
    RTYPE_NODE,
    RTYPE_PORT,
    RTYPE_LINK,
    RTYPE_POINT,
};

class ResourceMapByString;
class Resource
{
protected:
    ResourceType type;
    u_int32_t _id;  // unique resource ID 
    string name;    // topology identification name
    string address; // IPv address (with /slash netmask if applicable)

    //map<u_int32_t, Resource*>* IdIndex;
    //ResourceMapByName* NameIndex;
    //ResourceMapByAddress* NameIndex;    

public:
    Resource(ResourceType t, u_int32_t i, string& n, string& a): type(t), _id(i), name(n), address(a) { }
    Resource(ResourceType t, u_int32_t i, string& n): type(t), _id(i), name(n) { address = ""; }
    Resource(ResourceType t, u_int32_t i): type(t), _id(i) { name= ""; address = ""; }
    virtual ~Resource() { }
    ResourceType GetType() {return type;}
    void SetType(ResourceType t) { type = t;}
    u_int32_t GetId() { return _id; }
    void SetId(u_int32_t i) { _id = i; }
    string& GetName() { return name; }
    void SetName(string& n) { name = n; }
    string& GetAddress() { return address; }
    void SetAddress(string& a) { address = a; }
};


struct strcmpless {
    bool operator() (const string& ls, const string& rs) const 
    {   return (ls.compare(rs) < 0); }
};


class ResourceMapByString: public multimap<string, Resource*, strcmpless>
{
public:
    const list<Resource*> operator[](string& name) {
        multimap<string, Resource*, strcmpless>::iterator it, itlow, itup;
        itlow = this->lower_bound(name);
        itup = this->upper_bound(name);
        list<Resource*> rList;
        for ( it = itlow; it != itup; it++)
            rList.push_back((*it).second);
        return rList;
    }
};


class Link;
class Port;
class Node;
class ISCD;

class Domain: public Resource
{
protected:
    map<string, Node*, strcmpless> nodes;

public:
    Domain(u_int32_t id, string& name): Resource(RTYPE_DOMAIN, id, name) { }
    Domain(u_int32_t id, string& name, string& address): Resource(RTYPE_DOMAIN, id, name, address) { }
    virtual ~Domain() { }    
};

class NodeIfAdaptMatrix;

class Node: public Resource
{
protected:
    map<string, Port*, strcmpless> ports;
    Domain* domain;
    NodeIfAdaptMatrix* ifAdaptMatrix;

public:
    Node(u_int32_t id, string& name);
    Node(u_int32_t id, string& name, string& address);
    virtual ~Node();
    Domain* GetDomain() { return domain; }
    void SetDomain(Domain* d) { domain = d; }
    NodeIfAdaptMatrix* GetIfAdaptMatrix() { return ifAdaptMatrix; }
    void SetIfAdaptMatrix(NodeIfAdaptMatrix* matrix) { ifAdaptMatrix = matrix; }
};


class Port: public Resource
{
protected:
    map<string, Link*, strcmpless> links;
    Node* node;
    float maxBandwidth;
    float maxReservableBandwidth;
    float minReservableBandwidth;
    float unreservedBandwidth[8];   // 8 priorities: use unreservedBandwidth[7] by default
    void _Init() {
        node = NULL;
        maxBandwidth = maxReservableBandwidth = minReservableBandwidth = 0;
        for (int i = 0; i < 8; i++) unreservedBandwidth[i] = 0;
    }

public:
    Port(u_int32_t id, string& name): Resource(RTYPE_PORT, id, name) { _Init(); }
    Port(u_int32_t id, string& name, string& address): Resource(RTYPE_PORT, id, name, address) { _Init(); }
    virtual ~Port() { }    
    Node* GetNode() { return node; }
    void SetNode(Node* n) { node = n; }
    float GetMaxBandwidth() {return maxBandwidth;}
    void SetMaxBandwidth(float bw) { maxBandwidth = bw;}
    float GetMaxReservableBandwidth() {return maxReservableBandwidth;}
    void SetMaxReservableBandwidth(float bw) { maxReservableBandwidth = bw;}
    float* GetUnreservedBandwidth() { return unreservedBandwidth; }
};

#define _INF_ 2147483647

class Link: public Resource
{
protected:
    Port* port;
    int metric;
    float maxBandwidth;
    float maxReservableBandwidth;
    float minReservableBandwidth;
    float unreservedBandwidth[8];   // 8 priorities: use unreservedBandwidth[7] by default
    Link* remoteLink;
    list<ISCD> swCapDescriptors;
    list<Link*> containerLinks;      // the lower-layer links that this (virtual) link depends on (optional)
    list<Link*> componentLinks;     // the upper-layer (virtual) links that depends on this link (optional)
    void _Init() {
        port = NULL;
        metric = _INF_;
        maxBandwidth = maxReservableBandwidth = minReservableBandwidth = 0;
        for (int i = 0; i < 8; i++) unreservedBandwidth[i] = 0;
        remoteLink = NULL;
    }

public:
    Link(u_int32_t id, string& name): Resource(RTYPE_LINK, id, name) { _Init(); }
    Link(u_int32_t id, string& name, string& address): Resource(RTYPE_LINK, id, name, address) { _Init(); }
    virtual ~Link() { }
    Port* GetPort() { return port; }
    void SetPort(Port* p) { port = p; }
    int GetMetric() {return metric;}
    void SetMetric(int x) { metric = x;}
    float GetMaxBandwidth() {return maxBandwidth;}
    void SetMaxBandwidth(float bw) { maxBandwidth = bw;}
    float GetMaxReservableBandwidth() {return maxReservableBandwidth;}
    void SetMaxReservableBandwidth(float bw) { maxReservableBandwidth = bw;}
    float* GetUnreservedBandwidth() { return unreservedBandwidth; }
    Link* GetRemoteLink() {return remoteLink;}
    void SetRemoteLink(Link* rmt) { remoteLink = rmt;}
    list<ISCD>& GetSwCapDescriptors() { return swCapDescriptors; }
    list<Link*>& GetContainerLinks() { return containerLinks; }
    list<Link*>& GetComponentLinks() { return componentLinks; }
};


//$$$$ place holder for measurement, monitoring and compute resource attachment points
class Point: public Resource
{
protected:
    Port* port;

public:
    Point(u_int32_t id, string& name): Resource(RTYPE_POINT, id, name), port(NULL) { }
    Point(u_int32_t id, string& name, string& address): Resource(RTYPE_POINT, id, name, address), port(NULL) { }
    ~Point() { }
    Port* GetPort() { return port; }
    void SetPort(Port* p) { port = p; }
};


// Interface Switching Capability Descriptor (ISCD)  base classes

#define LINK_IFSWCAP_PSC1		1
#define LINK_IFSWCAP_PSC2		2
#define LINK_IFSWCAP_PSC3		3 
#define LINK_IFSWCAP_PSC4		4
#define LINK_IFSWCAP_L2SC		51
#define LINK_IFSWCAP_TDM		100
#define LINK_IFSWCAP_LSC		150
#define LINK_IFSWCAP_FSC		200

#define LINK_IFSWCAP_ENC_PKT		1
#define LINK_IFSWCAP_ENC_ETH	    2
#define LINK_IFSWCAP_ENC_PDH		3
#define LINK_IFSWCAP_ENC_RESV1		4
#define LINK_IFSWCAP_ENC_SONETSDH	5
#define LINK_IFSWCAP_ENC_RESV2		6
#define LINK_IFSWCAP_ENC_DIGIWRAP	7
#define LINK_IFSWCAP_ENC_LAMBDA		8
#define LINK_IFSWCAP_ENC_FIBER		9
#define LINK_IFSWCAP_ENC_RESV3		10
#define LINK_IFSWCAP_ENC_FIBRCHNL	11
#define LINK_IFSWCAP_ENC_G709ODUK	12
#define LINK_IFSWCAP_ENC_G709OCH	13


class ISCD
{
public:
    u_char	switchingType;
    u_char	encodingType;
    float unreservedBandwidth[8];   // 8 priorities: use unreservedBandwidth[7] by default
    ISCD (u_char swType, u_char enc): switchingType(swType), encodingType(enc)  { for (int i = 0; i < 8; i++) unreservedBandwidth[i] = 0; }
    virtual ~ISCD() { }
};


class ISCD_PSC: public ISCD
{
public:
    int mtu;
    ISCD_PSC(int level, int m): ISCD(LINK_IFSWCAP_PSC1+level-1, LINK_IFSWCAP_ENC_PKT), mtu(m) { }
    virtual ~ISCD_PSC() { }
};


#ifndef MAX_VLAN_NUM
#define MAX_VLAN_NUM 4096
#endif

class ISCD_L2SC: public ISCD
{
public:
    int mtu;
    ConstraintTagSet availableVlanTags;
    ConstraintTagSet assignedVlanTags;

    ISCD_L2SC(int m): ISCD(LINK_IFSWCAP_L2SC, LINK_IFSWCAP_ENC_PKT), mtu(m), availableVlanTags(MAX_VLAN_NUM), assignedVlanTags(MAX_VLAN_NUM) { }
    virtual ~ISCD_L2SC() { }
};


#ifndef MAX_TIMESLOTS_NUM
#define MAX_TIMESLOTS_NUM 192
#endif

class ISCD_TDM: public ISCD
{
public:
    float minReservableBandwidth;
    ConstraintTagSet availableTimeSlots;
    ConstraintTagSet assignedTimeSlots;

    ISCD_TDM(float min): ISCD(LINK_IFSWCAP_TDM, LINK_IFSWCAP_ENC_SONETSDH), minReservableBandwidth(min), availableTimeSlots(MAX_TIMESLOTS_NUM), assignedTimeSlots(MAX_TIMESLOTS_NUM) { }
    ~ISCD_TDM() { }
};


#ifndef MAX_OTNX_CHAN_NUM
#define MAX_OTNX_CHAN_NUM 256 //64: 10G with OPVCX; 40: WDM 
#endif

class ISCD_LSC: public ISCD
{
public:
    ConstraintTagSet availableWavelengs;
    ConstraintTagSet assignedWavelengs;

    ISCD_LSC(float min): ISCD(LINK_IFSWCAP_LSC, LINK_IFSWCAP_ENC_LAMBDA), availableWavelengs(MAX_OTNX_CHAN_NUM), assignedWavelengs(MAX_OTNX_CHAN_NUM) { }
    ~ISCD_LSC() { }
};



// Node-Interface Switching Maxtrix

#define MATRIX_SIZE 1024  //1024 x 1024 matrix

struct portcmpless {
    bool operator() (Port* lp, Port* rp) const 
    {   return (lp->GetName().compare(rp->GetName()) < 0); }
};

class NodeIfAdaptMatrix
{
private:
    float* bwCaps;
    int size;
    int portSN;
    map<Port*, int, portcmpless> portMap;

    NodeIfAdaptMatrix() { }

public:
    NodeIfAdaptMatrix(int n): size(n), portSN(0) { bwCaps = new float[n*n]; }
    ~NodeIfAdaptMatrix() { delete[] bwCaps; }
    void AddPort(Port* port) { assert(port && portSN < size); portMap[port] = portSN++; }
    float GetAdaptCap(int n1, int n2) { 
        assert(n1 >= 0 && n1 < size && n2 >= 0 && n2 < size); 
        return bwCaps[n1*size+n2]; 
    }
    float GetAdaptCap(Port* p1, Port* p2) { 
        if (portMap.find(p1) != portMap.end() && portMap.find(p2) != portMap.end()) 
        {
            int n1 = portMap[p1];
            int n2 = portMap[p2];
            return GetAdaptCap(n1, n2); 
        }
    }
    list<Port*> GetAdaptToPorts(Port* port, float bw=0.0);
    list<Port*> GetAdaptFromPorts(Port* port, float bw=0.0);
};


#endif
