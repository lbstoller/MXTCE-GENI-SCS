/*
 * user_constraint.hh
 *
 *  Created on: Mar 24, 2011
 *      Author: wind
 */

#ifndef USER_CONSTRAINT_HH_
#define USER_CONSTRAINT_HH_

#include "api_constraint.hh"

class Hop_req
{
protected:
	string hop_id;
	string link_id;
	string switching_cap_type;
	string encoding_type;
	string vlan_range_availability;

public:
	Hop_req()
	{
		this->initVar();
	}

	void initVar()
	{
		this->hop_id="";
		this->link_id="";
		this->switching_cap_type="";
		this->encoding_type="";
		this->vlan_range_availability="";
	}

	void setHopid(string& t_hop_id) {this->hop_id = t_hop_id;}
	string& getHopid() {return this->hop_id;}

	void setLinkid(string& t_link_id) {this->link_id = t_link_id;}
	string& getLinkid() {return this->link_id;}

	void setSwitchingcaptype(string& t_switching_cap_type) {this->switching_cap_type = t_switching_cap_type;}
	string& getSwitchingcaptype() {return this->switching_cap_type;}

	void setEncodingtype(string& t_encoding_type) {this->encoding_type = t_encoding_type;}
	string& getEncodingtype() {return this->encoding_type;}

	void setVlanrangeavailability(string& t_vlan_range_availability) {this->vlan_range_availability = t_vlan_range_availability;}
	string& getVlanrangeavailability() {return this->vlan_range_availability;}

};

class Path_req
{
protected:
	string path_id;
	int path_length;
	Hop_req* hops_ptr;

public:
	Path_req()
	{
		this->initVar();
	}
	void initVar()
	{
		this->path_id="";
		path_length=0;
		hops_ptr=NULL;
	}
	void setPathid(string& t_path_id) {this->path_id = t_path_id;}
	string& getPathid() {return this->path_id;}

	void setPathlength(int t_length) {this->path_length = t_length;}
	int getPathlength() {return this->path_length;}

	void setHops(Hop_req* t_hops_ptr) {this->hops_ptr = t_hops_ptr;}
	Hop_req* getHops() {return this->hops_ptr;}

};

class Apimsg_user_constraint:public Apimsg_constraint
{
protected:
	string gri;
	u_int32_t start_time;
	u_int32_t end_time;
	u_int32_t bandwidth;
	string layer;
	string path_setup_model;
	string path_type;
	string src_vlan_tag;
	string dest_vlan_tag;
	string src_end_point;
	string dest_end_point;
	u_int32_t src_ip_port;
	u_int32_t dest_ip_port;
	string protocol;
	string dscp;
	u_int32_t burst_limit;
	string lsp_class;
	Path_req* path;

public:
	Apimsg_user_constraint()
	{
		this->initVar();
	}
	virtual ~Apimsg_user_constraint(){}

	void initVar()
	{
		this->gri = "";
		this->start_time = 0;
		this->end_time = 0;
		this->bandwidth = 0;
		this->layer = "";
		this->path_setup_model = "";
		this->path_type = "";
		this->src_vlan_tag = "";
		this->dest_vlan_tag = "";
		this->src_end_point = "";
		this->dest_end_point = "";
		this->src_ip_port = 0;
		this->dest_ip_port =0;
		this->protocol = "";
		this->dscp = "";
		this->burst_limit = 0;
		this->lsp_class = "";
		this->path = NULL;

	}

	void setGri(string t_gri) {this->gri = t_gri;}
	string& getGri() {return this->gri;}

	void setStarttime(u_int32_t t_start_time) {this->start_time = t_start_time;}
	u_int32_t getStarttime() {return this->start_time;}

	void setEndtime(u_int32_t t_end_time) {this->end_time = t_end_time;}
	u_int32_t getEndtime() {return this->end_time;}

	void setBandwidth(u_int32_t t_bandwidth) {this->bandwidth = t_bandwidth;}
	u_int32_t getBandwidth() {return this->bandwidth;}

	void setLayer(string& t_layer) {this->layer = t_layer;}
	string& getLayer() {return this->layer;}

	void setPathsetupmodel(string& t_path_setup_model) {this->path_setup_model = t_path_setup_model;}
	string& getPathsetupmodel() {return this->path_setup_model;}

	void setPathtype(string& t_path_type) {this->path_type = t_path_type;}
	string& getPathtype() {return this->path_type;}

	void setSrcvlantag(string& t_src_vlan_tag) {this->src_vlan_tag = t_src_vlan_tag;}
	string& getSrcvlantag() {return this->src_vlan_tag;}

	void setDestvlantag(string& t_dest_vlan_tag) {this->dest_vlan_tag = t_dest_vlan_tag;}
	string& getDestvlantag() {return this->dest_vlan_tag;}

	void setSrcendpoint(string& t_src_end_point) {this->src_end_point = t_src_end_point;}
	string& getSrcendpoint() {return this->src_end_point;}

	void setDestendpoint(string& t_dest_end_point) {this->dest_end_point = t_dest_end_point;}
	string& getDestendpoint() {return this->dest_end_point;}

	void setSrcipport(u_int32_t t_src_ip_port) {this->src_ip_port = t_src_ip_port;}
	u_int32_t getSrcipport() {return this->src_ip_port;}

	void setDestipport(u_int32_t t_dest_ip_port) {this->dest_ip_port = t_dest_ip_port;}
	u_int32_t getDestipport() {return this->dest_ip_port;}

	void setProtocol(string& t_protocol) {this->protocol = t_protocol;}
	string& getProtocol() {return this->protocol;}

	void setDscp(string& t_dscp) {this->dscp = t_dscp;}
	string& getDscp() {return this->dscp;}

	void setBurstlimit(u_int32_t t_burst_limit) {this->burst_limit = t_burst_limit;}
	u_int32_t getBurstlimit() {return this->burst_limit;}

	void setLspclass(string& t_lsp_class) {this->lsp_class = t_lsp_class;}
	string& getLspclass() {return this->lsp_class;}

	void setPath(Path_req* t_path) {this->path = t_path;}
	Path_req* getPath() {return this->path;}


};



#endif /* USER_CONSTRAINT_HH_ */
