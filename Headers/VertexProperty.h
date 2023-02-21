/*
 *
 *	VertexProperty.h
 * 	Author: Behnam
 *
 */

#ifndef VERTEXPROPERTY_H_
#define VERTEXPROPERTY_H_

/* Should really change this to an interior property map
 * with just a boost::vertex_index_t.
 * Then use exterior property maps for drawing.
 * */
struct VertexProperty{
	int id;
	std::string pos; //for drawing.
	double height; //for drawing.
	double width; //for drawing.
    std::string other; //for drawing.
};

#endif
