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

#include "tedb.hh"
#include "exception.hh"

// TODO: Generate unique nodeID-number (for xml) ?

void DBDomain::UpdateToXML(bool populateSubLevels)
{
    //$$$$ loop-call node->UpdateToXML()
    //$$$$ update content of xmlElem from local variables
}


void DBDomain::UpdateFromXML(bool populateSubLevels)
{
    // update local variables from content of xmlElem
    gettimeofday (&this->updateTime, NULL);

    //match up node level elements
    assert(this->xmlElem->children);
    xmlNodePtr nodeLevel;
    for (nodeLevel = this->xmlElem->children; nodeLevel != NULL; nodeLevel = nodeLevel->next)
    {
        if (nodeLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)nodeLevel->name, "node", 4) == 0)
        {
            bool newNode = false;
            string nodeName = (const char*)xmlGetProp(nodeLevel, (const xmlChar*)"id");
            if (strstr(nodeName.c_str(), "node=") != NULL)
                nodeName = GetUrnField(nodeName, "node");
            DBNode* node = NULL;
            if (this->nodes.find(nodeName) != this->nodes.end())
            {
                node = (DBNode*)this->nodes[nodeName];
            }
            if (node == NULL)
            {
                node = new DBNode(tedb, 0, nodeName);
                node->SetXmlElement(nodeLevel);            
                node->SetDomain(this);
                nodes[nodeName] = node;
                newNode = true;
            }
            // populate XML update to node level
            if (populateSubLevels)
                node->UpdateFromXML(true);
        }
        else if (nodeLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)nodeLevel->name, "address", 7) == 0)
        {
           StripXmlString(this->address, xmlNodeGetContent(nodeLevel));
        }

        // TODO: parse NodeIfAdaptMatrix?        
    }
    // cleanup nodes that no longer exist in XML
    map<string, Node*, strcmpless>::iterator itn = nodes.begin();
    while (itn != nodes.end())
    {
        if(((DBNode*)(*itn).second)->GetXmlElement() == NULL)
        {
            delete (*itn).second;
            map<string, Node*, strcmpless>::iterator toerase = itn;
            ++itn;
            nodes.erase(toerase);
        }
        else
            ++itn;
    }
}


TDomain* DBDomain::Checkout(TGraph* tg)
{
    TDomain* td = new TDomain(this->_id, this->name, this->address);
    map<string, Node*, strcmpless>::iterator itn = this->nodes.begin();
    for (; itn != this->nodes.end(); itn++) 
    {
        TNode* tn = ((DBNode*)(*itn).second)->Checkout(tg);
        tg->AddNode(td, tn);
    }
    tg->AddDomain(td);
    return td;
}

DBDomain::~DBDomain()
{
    // delete sublevels
    map<string, Node*, strcmpless>::iterator itn = nodes.begin();
    for (; itn != nodes.end(); itn++)
    {
        delete (*itn).second;
    }
}


void DBNode::UpdateToXML(bool populateSubLevels)
{
    //$$$$ loop-call node->UpdateToXML()
    //$$$$ update content of xmlElem from local variables
}


void DBNode::UpdateFromXML(bool populateSubLevels)
{
    // update local variables from content of xmlElem
    gettimeofday (&this->updateTime, NULL);

    //match up node level elements
    assert(this->xmlElem->children);
    xmlNodePtr portLevel;
    for (portLevel = this->xmlElem->children; portLevel != NULL; portLevel = portLevel->next)
    {
        if (portLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)portLevel->name, "port", 4) == 0)
        {
            bool newPort = false;
            string portName = (const char*)xmlGetProp(portLevel, (const xmlChar*)"id");
            if (strstr(portName.c_str(), "port=") != NULL)
                portName = GetUrnField(portName, "port");
            DBPort* port = NULL;
            if (this->ports.find(portName) != this->ports.end())
            {
                port = (DBPort*)this->ports[portName];
            }
            if (port == NULL)
            {
                port = new DBPort(tedb, 0, portName);
                port->SetXmlElement(portLevel);            
                port->SetNode(this);
                ports[portName] = port;
                newPort = true;
            }
            // populate XML update to node level
            if (populateSubLevels)
                port->UpdateFromXML(true);
        }
        else if (portLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)portLevel->name, "address", 7) == 0)
        {
           StripXmlString(this->address, xmlNodeGetContent(portLevel));
        }
    }
    // cleanup ports that no longer exist in XML
    
    map<string, Port*, strcmpless>::iterator itp = ports.begin();
    while (itp != ports.end())
    {
        if(((DBPort*)(*itp).second)->GetXmlElement() == NULL)
        {
            delete (*itp).second;
            map<string, Port*, strcmpless>::iterator toerase = itp;
            ++itp;
            ports.erase(toerase);
        }
        else
            ++itp;
    }
}


TNode* DBNode::Checkout(TGraph* tg)
{
    TNode* tn = new TNode(this->_id, this->name, this->address);
    map<string, Port*, strcmpless>::iterator itp = this->ports.begin();
    for (; itp != this->ports.end(); itp++)
    {
        TPort* tp = ((DBPort*)(*itp).second)->Checkout(tg);
        tg->AddPort(tn, tp);
    }
    return tn;
}


DBNode::~DBNode()
{
    // delete sublevels
    map<string, Port*, strcmpless>::iterator itp = ports.begin();
    for (; itp != ports.end(); itp++)
    {
        delete (*itp).second;
    }
}


void DBPort::UpdateToXML(bool populateSubLevels)
{
    //$$$$ loop-call node->UpdateToXML()
    //$$$$ update content of xmlElem from local variables
}


void DBPort::UpdateFromXML(bool populateSubLevels)
{
    // update local variables from content of xmlElem
    gettimeofday (&this->updateTime, NULL);

    //match up node level elements
    assert(this->xmlElem->children);
    xmlNodePtr linkLevel;
    for (linkLevel = this->xmlElem->children; linkLevel != NULL; linkLevel = linkLevel->next)
    {
        if (linkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)linkLevel->name, "link", 4) == 0)
        {
            bool newLink = false;
            string linkName = (const char*)xmlGetProp(linkLevel, (const xmlChar*)"id");
            if (strstr(linkName.c_str(), "link=") != NULL)
                linkName = GetUrnField(linkName, "link");
            DBLink* link = NULL;
            if (this->links.find(linkName) != this->links.end())
            {
                link = (DBLink*)this->links[linkName];
            }
            if (link == NULL)
            {
                link = new DBLink(tedb, 0, linkName);
                link->SetXmlElement(linkLevel);
                link->SetPort(this);
                links[linkName] = link;
                newLink = true;
            }
            // populate XML update to node level
            if (populateSubLevels)
                link->UpdateFromXML();
        }
        else if (linkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)linkLevel->name, "capacity", 8) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(linkLevel));
            this->maxBandwidth = StringToBandwidth(bwStr);
        }
        else if (linkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)linkLevel->name, "maximumReservableCapacity", 17) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(linkLevel));
            this->maxReservableBandwidth= StringToBandwidth(bwStr);
            //?
            for (int i = 0; i < 8; i++)
                this->unreservedBandwidth[i] = this->maxReservableBandwidth;
        }
        else if (linkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)linkLevel->name, "minimumReservableCapacity", 17) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(linkLevel));
            this->minReservableBandwidth= StringToBandwidth(bwStr);
        }
        else if (linkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)linkLevel->name, "granularity", 10) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(linkLevel));
            this->bandwidthGranularity = StringToBandwidth(bwStr);
        }
    }
    // cleanup links that no longer exist in XML
    map<string, Link*, strcmpless>::iterator itl = links.begin();
    while (itl != links.end())
    {
        if(((DBLink*)(*itl).second)->GetXmlElement() == NULL)
        {
            if ((*itl).second->GetRemoteLink() != NULL)
                (*itl).second->GetRemoteLink()->SetRemoteLink(NULL);
            delete (*itl).second;
            map<string, Link*, strcmpless>::iterator toerase = itl;
            ++itl;
            links.erase(toerase);
        }
        else
            ++itl;
    }
}


TPort* DBPort::Checkout(TGraph* tg)
{
    TPort* tp = new TPort(this->_id, this->name, this->address);
    tp->SetMaxBandwidth(this->maxBandwidth);
    tp->SetMaxReservableBandwidth(this->maxReservableBandwidth);
    for (int i = 0; i < 8; i++)
        tp->GetUnreservedBandwidth()[i] = this->unreservedBandwidth[i];
    tp->SetMinReservableBandwidth(this->minReservableBandwidth);
    tp->SetBandwidthGranularity(this->bandwidthGranularity);
    map<string, Link*, strcmpless>::iterator itl = this->links.begin();
    for (; itl != this->links.end(); itl++)
    {
        TLink* tl = ((DBLink*)(*itl).second)->Checkout(tg);
        tg->AddLink(tp, tl);
    }
    return tp;
}


DBPort::~DBPort()
{
    // delete sublevels
    map<string, Link*, strcmpless>::iterator itl = links.begin();
    for (; itl != links.end(); itl++)
    {
        delete (*itl).second;
    }
}


void DBLink::UpdateToXML(bool populateSubLevels)
{
    //$$$$ loop-call node->UpdateToXML()
    //$$$$ update content of xmlElem from local variables
}


void DBLink::UpdateFromXML(bool populateSubLevels)
{
    // update local variables from content of xmlElem
    gettimeofday (&this->updateTime, NULL);

    //match up node level elements
    assert(this->xmlElem->children);
    swCapDescriptors.clear();
    xmlNodePtr sublinkLevel;
    for (sublinkLevel = this->xmlElem->children; sublinkLevel != NULL; sublinkLevel = sublinkLevel->next)
    {
        if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "address", 8) == 0)
        {
            StripXmlString(this->address, xmlNodeGetContent(sublinkLevel));
        }
        if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "capacity", 8) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(sublinkLevel));
            this->maxBandwidth = StringToBandwidth(bwStr);
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "maximumReservableCapacity", 17) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(sublinkLevel));
            this->maxReservableBandwidth= StringToBandwidth(bwStr);
            //?
            for (int i = 0; i < 8; i++)
                this->unreservedBandwidth[i] = this->maxReservableBandwidth;
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "minimumReservableCapacity", 17) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(sublinkLevel));
            this->minReservableBandwidth= StringToBandwidth(bwStr);
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "granularity", 10) == 0)
        {
            string bwStr;
            StripXmlString(bwStr, xmlNodeGetContent(sublinkLevel));
            this->bandwidthGranularity = StringToBandwidth(bwStr);
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "trafficEngineeringMetric", 10) == 0)
        {
            string metricStr;
            StripXmlString(metricStr, xmlNodeGetContent(sublinkLevel));
            sscanf(metricStr.c_str(), "%d", &this->metric);
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "remoteLinkId", 10) == 0)
        {
            string rmtLinkStr;
            StripXmlString(rmtLinkStr, xmlNodeGetContent(sublinkLevel));
            DBLink* rmtLink = tedb->LookupLinkByURN(rmtLinkStr);
            if (rmtLink != NULL)
            {
                this->remoteLink = rmtLink;
                rmtLink->SetRemoteLink(this);
            }
        }
        else if (sublinkLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)sublinkLevel->name, "switchingCapabilityDescriptors", 30) == 0)
        {
            ISCD* iscd = GetISCDFromXML(sublinkLevel);
            swCapDescriptors.push_back(iscd); 
        }
        // TODO: parse IACD?
    }
}


ISCD* DBLink::GetISCDFromXML(xmlNodePtr xmlNode)
{
    ISCD* iscd = NULL;
    u_char swType = 0, encType = 0;
    xmlNodePtr specLevel = NULL;
    int mtu = 0;
    string vlanRange = "";
    bool vlanTranslation = false;
    long capacity = 0;
    string timeslotRange = "";
    long minBandwidth = 0;
    string wavelengthRange = "";
    bool wavelengthTranslation = false;
    for (xmlNode = xmlNode->children; xmlNode != NULL; xmlNode = xmlNode->next)
    {
        if (xmlNode->type == XML_ELEMENT_NODE && strncasecmp((const char*)xmlNode->name, "switchingType", 12) == 0)
        {
            string swTypeStr;
            StripXmlString(swTypeStr, xmlNodeGetContent(xmlNode));
            if (strncasecmp((const char*)swTypeStr.c_str(), "l2sc", 4) == 0)
                swType = LINK_IFSWCAP_L2SC;
            else if (strncasecmp((const char*)swTypeStr.c_str(), "psc", 3) == 0)
                swType = LINK_IFSWCAP_PSC1;
            else if (strncasecmp((const char*)swTypeStr.c_str(), "tdm", 4) == 0)
                swType = LINK_IFSWCAP_TDM;
            else if (strncasecmp((const char*)swTypeStr.c_str(), "lsc", 4) == 0)
                swType = LINK_IFSWCAP_LSC;
        }
        else if (xmlNode->type == XML_ELEMENT_NODE && strncasecmp((const char*)xmlNode->name, "encodingType", 12) == 0)
        {
            string encTypeStr;
            StripXmlString(encTypeStr, xmlNodeGetContent(xmlNode));
            if (strncasecmp((const char*)encTypeStr.c_str(), "ethernet", 6) == 0)
                encType = LINK_IFSWCAP_ENC_ETH;
            else if (strncasecmp((const char*)encTypeStr.c_str(), "packet", 6) == 0)
                encType = LINK_IFSWCAP_ENC_PKT;
            else if (strncasecmp((const char*)encTypeStr.c_str(), "sonet", 5) == 0 || strncasecmp((const char*)encTypeStr.c_str(), "sdh", 3) == 0)
                encType = LINK_IFSWCAP_ENC_SONETSDH;
            else if (strncasecmp((const char*)encTypeStr.c_str(), "lambda", 6) == 0)
                encType = LINK_IFSWCAP_ENC_LAMBDA;
        }
        else if (xmlNode->type == XML_ELEMENT_NODE && strncasecmp((const char*)xmlNode->name, "switchingCapabilitySpecificInfo", 30) == 0)
        {
            for (specLevel = specLevel->children; specLevel != NULL; specLevel = specLevel->next)
            {
                if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "capacity", 12) == 0)
                {
                    string bwStr;
                    StripXmlString(bwStr, xmlNodeGetContent(specLevel));
                    capacity = StringToBandwidth(bwStr);
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "interfaceMTU", 12) == 0)
                {
                    string mtuStr;
                    StripXmlString(mtuStr, xmlNodeGetContent(specLevel));
                    sscanf(mtuStr.c_str(), "%d", &mtu);
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "vlanRangeAvailability", 12) == 0)
                {
                    StripXmlString(vlanRange, xmlNodeGetContent(specLevel));
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "vlanTranslation", 12) == 0)
                {
                    string translationStr;
                    StripXmlString(translationStr, xmlNodeGetContent(specLevel));
                    if (strncasecmp(translationStr.c_str(), "true", 4))
                        vlanTranslation = true;
                    else
                        vlanTranslation = false;
                }
                if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "minimumReservableBandwidth", 12) == 0)
                {
                    string bwStr;
                    StripXmlString(bwStr, xmlNodeGetContent(specLevel));
                    minBandwidth = StringToBandwidth(bwStr);
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "timeslotRangeAvailability", 12) == 0)
                {
                    StripXmlString(timeslotRange, xmlNodeGetContent(specLevel));
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "wavelengthRangeAvailability", 12) == 0)
                {
                    StripXmlString(wavelengthRange, xmlNodeGetContent(specLevel));
                }
                else if (specLevel->type == XML_ELEMENT_NODE && strncasecmp((const char*)specLevel->name, "wavelengthTranslation", 12) == 0)
                {
                    string translationStr;
                    StripXmlString(translationStr, xmlNodeGetContent(specLevel));
                    if (strncasecmp(translationStr.c_str(), "true", 4))
                        wavelengthTranslation = true;
                    else
                        wavelengthTranslation = false;
                }
            }
        }
    }
    switch (swType)
    {
        case LINK_IFSWCAP_L2SC:
            iscd = new ISCD_L2SC(mtu);
            ((ISCD_L2SC*)iscd)->availableVlanTags.LoadRangeString(vlanRange);
            ((ISCD_L2SC*)iscd)->vlanTranslation = vlanTranslation;
            break;
        case LINK_IFSWCAP_PSC1:
            iscd = new ISCD_PSC(1, mtu);
            break;
        case LINK_IFSWCAP_TDM:
            iscd = new ISCD_TDM(minBandwidth);
            ((ISCD_TDM*)iscd)->availableTimeSlots.LoadRangeString(timeslotRange);
            break;
        case LINK_IFSWCAP_LSC:
            iscd = new ISCD_LSC();
            ((ISCD_LSC*)iscd)->availableWavelengths.LoadRangeString(wavelengthRange);
            ((ISCD_LSC*)iscd)->wavelengthTranslation = wavelengthTranslation;
            break;
        default:
            // type not supported 
            return NULL;
    }
    iscd->switchingType = swType;
    iscd->encodingType = encType;
    iscd->capacity = capacity;
    return iscd;
}


TLink* DBLink::Checkout(TGraph* tg)
{
    TLink* tl = new TLink(this->_id, this->name, this->address);
    tl->SetMetric(this->metric);
    tl->SetMaxBandwidth(this->maxBandwidth);
    tl->SetMaxReservableBandwidth(this->maxReservableBandwidth);
    tl->SetMinReservableBandwidth(this->minReservableBandwidth);
    tl->SetBandwidthGranularity(this->bandwidthGranularity);
    for (int i = 0; i < 8; i++)
        tl->GetUnreservedBandwidth()[i] = this->unreservedBandwidth[i];
    list<ISCD*>::iterator its = this->GetSwCapDescriptors().begin();
    for (; its != this->GetSwCapDescriptors().end(); its++)
        tl->GetSwCapDescriptors().push_back((*its)->Duplicate());
    list<IACD>::iterator ita = this->GetSwAdaptDescriptors().begin();
    for (; ita != this->GetSwAdaptDescriptors().end(); ita++)
        tl->GetSwAdaptDescriptors().push_back(*ita);
    list<Link*>::iterator itl = this->GetContainerLinks().begin();
    //$$$$ update pointers to copy version?
    for (; itl != this->GetContainerLinks().end(); itl++)
        tl->GetContainerLinks().push_back(*itl);
    itl = this->GetComponentLinks().begin();
    //$$$$ update pointers to copy version?
    for (; itl != this->GetComponentLinks().end(); itl++)
        tl->GetComponentLinks().push_back(*itl);
    // correct remoteLink references
    tl->SetRemoteLink(this->remoteLink);
    list<TLink*>::iterator itl2 = tg->GetLinks().begin();
    for (; itl2 != tg->GetLinks().end(); itl2++)
    {
        TLink* tl2 = *itl2;
        if ((Link*)tl2 == (Link*)this)
        {
            tl->SetRemoteLink(tl2);
            tl2->SetRemoteLink(tl);
            if (tl->GetPort() && tl->GetPort()->GetNode())
                ((TNode*)tl->GetPort()->GetNode())->AddRemoteLink(tl2);
            if (tl2->GetPort() && tl2->GetPort()->GetNode())
                ((TNode*)tl2->GetPort()->GetNode())->AddRemoteLink(tl);
            break;
        }
    }
    return tl;
}


DBLink::~DBLink()
{
    for (list<ISCD*>::iterator it = swCapDescriptors.begin(); it != swCapDescriptors.end(); it++)
        delete (*it);
}


void TEDB::ClearXmlTree()
{
    if (xmlTree == NULL)
        return;
    xmlFreeDoc(xmlTree);
    list<DBDomain*>::iterator itd = dbDomains.begin();
    for (; itd != dbDomains.end(); itd++)
    {
        (*itd)->SetXmlElement(NULL);
    }
    list<DBNode*>::iterator itn = dbNodes.begin();
    for (; itn != dbNodes.end(); itn++)
    {
        (*itn)->SetXmlElement(NULL);
    }
    list<DBPort*>::iterator itp = dbPorts.begin();
    for (; itp != dbPorts.end(); itp++)
    {
        (*itp)->SetXmlElement(NULL);
    }    
    list<DBLink*>::iterator itl = dbLinks.begin();
    for (; itl != dbLinks.end(); itl++)
    {
        (*itl)->SetXmlElement(NULL);
    }    
    xmlTree = NULL;
}


void TEDB::PopulateXmlTree()
{
    assert(xmlTree != NULL);

    xmlNodePtr node;
    xmlNodePtr rootLevel;
    xmlNodePtr domainLevel;

    rootLevel = xmlDocGetRootElement(xmlTree);
    if (rootLevel->type != XML_ELEMENT_NODE || strncasecmp((const char*)rootLevel->name, "topology", 8) != 0)
    {
        throw TEDBException((char*)"TEDB::PopulateXmlTree failed to locate root <topology> element");
    }

    //match up Domain level elements
    for (domainLevel = rootLevel->children; domainLevel != NULL; domainLevel = domainLevel->next)
    {
        if (domainLevel->type != XML_ELEMENT_NODE || strncasecmp((const char*)domainLevel->name, "domain", 6) != 0)
            continue;
        bool newDomain = false;
        string domainName = (const char*)xmlGetProp(domainLevel, (const xmlChar*)"id");
        if (strstr(domainName.c_str(), "domain=") != NULL)
            domainName = GetUrnField(domainName, "domain");
        DBDomain* domain = LookupDomainByName(domainName);
        if (domain == NULL)
        {
            domain = new DBDomain(this, 0, domainName);
            domain->SetXmlElement(domainLevel);            
            dbDomains.push_back(domain);
            newDomain = true;
        }

        domain->UpdateFromXML(true);
    }

    // cleanup domains that no longer exist in XML
    list<DBDomain*>::iterator itd = dbDomains.begin();
    for (; itd != dbDomains.end(); itd++)
    {
        if((*itd)->GetXmlElement() == NULL)
        {
            delete (*itd);
            itd = dbDomains.erase(itd);
        }
    }

    // clean up dbNodes, dbPorts and dbLinks lists
    dbNodes.clear();
    dbPorts.clear();
    dbLinks.clear();    
    // then re-add the updated elements to the lists
    for (; itd != dbDomains.end(); itd++)
    {
        map<string, Node*, strcmpless>::iterator itn = (*itd)->GetNodes().begin();
        for (; itn != (*itd)->GetNodes().end(); itn++)
        {
            dbNodes.push_back((DBNode*)(*itn).second);
            map<string, Port*, strcmpless>::iterator itp = (*itn).second->GetPorts().begin();
            for (; itp != (*itn).second->GetPorts().end(); itp++)
            {
                dbPorts.push_back((DBPort*)(*itp).second);
                map<string, Link*, strcmpless>::iterator itl = (*itp).second->GetLinks().begin();
                for (; itl != (*itp).second->GetLinks().end(); itl++)
                {
                    dbLinks.push_back((DBLink*)(*itl).second);
                }
            }

        }
    }
}


TGraph* TEDB::GetSnapshot(string& name)
{
    TGraph* tg = new TGraph(name);
    if (dbDomains.empty())
        return NULL;

    list<DBDomain*>::iterator itd = dbDomains.begin();
    for (; itd != dbDomains.end(); itd++)
        (*itd)->Checkout(tg);

    return tg;
}


DBDomain* TEDB::LookupDomainByName(string& name)
{
    list<DBDomain*>::iterator itd = dbDomains.begin();
    for (; itd != dbDomains.end(); itd++)
    {
        DBDomain* dbd = *itd;
        if (strcasecmp(dbd->GetName().c_str(), name.c_str()) == 0)
            return dbd;
    }
    return NULL;
}



DBDomain* TEDB::LookupDomainByURN(string& urn)
{
    string domainName = GetUrnField(urn, "domain");
    if (domainName.empty())
        return NULL;
    return LookupDomainByName(domainName);
}


DBNode* TEDB::LookupNodeByURN(string& urn)
{
    DBDomain* dbd = LookupDomainByURN(urn);
    if (dbd == NULL)
        return NULL;
    string nodeName = GetUrnField(urn, "node");
    map<string, Node*, strcmpless>::iterator itn = dbd->GetNodes().find(nodeName);
    if (itn == dbd->GetNodes().end())
        return NULL;
    return (DBNode*)(*itn).second;
}


DBPort* TEDB::LookupPortByURN(string& urn)
{
    DBNode* dbn = LookupNodeByURN(urn);
    if (dbn == NULL)
        return NULL;
    string portName = GetUrnField(urn, "port");
    map<string, Port*, strcmpless>::iterator itp = dbn->GetPorts().find(portName);
    if (itp == dbn->GetPorts().end())
        return NULL;
    return (DBPort*)(*itp).second;
}


DBLink* TEDB::LookupLinkByURN(string& urn)
{
    DBPort* dbp = LookupPortByURN(urn);
    if (dbp == NULL)
        return NULL;
    string linkName = GetUrnField(urn, "link");
    map<string, Link*, strcmpless>::iterator itl = dbp->GetLinks().find(linkName);
    if (itl == dbp->GetLinks().end())
        return NULL;
    return (DBLink*)(*itl).second;
}

