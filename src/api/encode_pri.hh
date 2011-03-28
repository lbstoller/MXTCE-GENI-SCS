#ifndef ENCODE_PRI_HH_
#define ENCODE_PRI_HH_

#include <iostream>
#include <string>
#include <fstream>
#include "types.hh"
#include "tag_id.hh"

using namespace std;


#define INIT_BUFF_SIZE 32

class Encode_Pri_Type
{
private:
	u_int8_t* buff;
	int current_cap;
	int offset;

	void create_buff();
	void buff_rem_check(int );
	void expand_buff();
	void buff_prune();


public:

	Encode_Pri_Type()
	{
		create_buff();
		offset=0;
	}

	void encodeHeader(u_int8_t , u_int8_t , int );
	void encodeInteger(u_int8_t, int );
	void encodeString(u_int8_t , string );
	void encodeFloat(u_int8_t , float );
	void encodeDouble(u_int8_t , double );

	u_int8_t* get_buff()
	{
		buff_prune();
		return buff;
	}

	int get_length()
	{
		return offset;
	}


};


#endif /* ENCODE_PRI_HH_ */
