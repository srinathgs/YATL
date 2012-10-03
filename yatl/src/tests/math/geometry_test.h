/*
 * geometry_test.h
 *
 *  Created on: Sep 14, 2012
 *      Author: arietal
 */

#ifndef GEOMETRY_TEST_H_
#define GEOMETRY_TEST_H_

#include "../test.h"
#include "../../include/math/geometry.h"

using namespace yatl;

template <typename peak>
struct triangle {
	typedef P< sub_op< peak, LR<1> >, LR<0> > low;
	typedef P< peak, LR<1> > p;
	typedef P< add_op< peak, LR<1> >, LR<0> > high;
	typedef segment< low, p > s1;
	typedef segment< p, high > s2;
};

template <int start, int end>
struct comb_ : public list<list_< typename triangle< LR< start > >::s1, list_< typename triangle< LR< start > >::s2, typename comb_<start+1,end>::List_> > > {};

template <int end>
struct comb_<end,end> : public list<_NIL> {};

template <int start, int end>
struct comb : public multi_segment< comb_<start,end>, false > {};

class GeometryTest : public Test {
public:
	const char *getName() const { return "Geometry Test"; }
	bool runTest() {

		typedef segment< P< LR<0>, LR<0> >, P< LR<6>, LR<3> > > s1;
		typedef segment< P< LR<1,2>, LR<0> >, P< LR<3>, LR<3> > > s2;
		typedef segment< P< LR<0>, LR<1> >, P< LR<6>, LR<0> > > s3;
		typedef segment< P< LR<0>, LR<3> >, P< LR<6>, LR<0> > > s4;
		typedef segment< P< LR<4>, LR<2> >, P< LR<7>, LR<5> > > s5;
		typedef segment< P< LR<1>, LR<0> >, P< LR<7>, LR<2> > > s6;

		Assume((s1::a::eq< LR<1,2> >::result && s1::b::eq< LR<0> >::result), ("Expected a: 1/2, b: 0"));
		Assume((s2::a::eq< LR<6,5> >::result && s2::b::eq< LR<-3,5> >::result), ("Expected a: 6/5, b: -3/5"));
		Assume((s3::a::eq< LR<-1,6> >::result && s3::b::eq< LR<1> >::result), ("Expected a: -1/6, b: 1"));
		Assume((s4::a::eq< LR<-1,2> >::result && s4::b::eq< LR<3> >::result), ("Expected a: -1/2, b: 3"));


		Assume((s1::at< LR<3> >::eq< LR<3,2> >::result), ("Expected 3/2"));

		typedef seg_intersect<s1,s2> s1s2;
		Assume((s1s2::valid), ("Expected s1 to intersect s2"));
		Assume((s1s2::x::eq< LR<6,7> >::result && s1::at<s1s2::x>::eq<s2::at<s1s2::x> >::result), ("Expected 6/7, 3/7, 3/7"));

		typedef seg_intersect<s1,s3> s1s3;
		Assume((s1s3::valid), ("Expected s1 to intersect s3"));
		Assume((s1s3::x::eq< LR<3,2> >::result && s1::at<s1s3::x>::eq<s3::at<s1s3::x> >::result), ("Expected 3/2, 3/4, 3/4"));

		typedef seg_intersect<s1,s4> s1s4;
		Assume((s1s4::valid), ("Expected s1 to intersect s4"));
		Assume((s1s4::x::eq< LR<3> >::result && s1::at<s1s4::x>::eq<s4::at<s1s4::x> >::result), ("Expected 3, 3/2, 3/2"));

		typedef seg_intersect<s1,s5> s1s5;
		Assume((s1s5::valid), ("Expected s1 to intersect s5"));
		Assume((s1s5::x::eq< LR<4> >::result && s1::at<s1s5::x>::eq<s5::at<s1s5::x> >::result), ("Expected 4,2,2"));


		typedef seg_intersect<s1,s6> s1s6;
		Assume((!s1s6::valid), ("Expected s1 to not intersect s6"));

		Assume((!s2::overlaps<s5>::result), ("Expected s2 to not overlaps s5"));

		Assume((s2::overlaps<s6>::result), ("Expected s2 to overlap s6"));


		typedef multi_segment< list< s5, s2 > > mseg;
		Assume((mseg::segments::compare< list< segment< P< LR<1,2>, LR<0> >, P< LR<3>, LR<3> > >, segment< P< LR<4>, LR<2> >, P< LR<7>, LR<5> > > > >::result == 0),
				("Expected (sorted) [(1/2, 0) - (3, 3), a: 6/5, b: -3/5, minx: 1/2, miny: 0], [(4, 2) - (7, 5), a: 1, b: -2, minx: 4, miny: 2]"));

		typedef comb<1, 3> mseg2;

		Assume((mseg2::segments::compare< list<
				segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > >, segment< P< LR<1>, LR<0> >, P< LR<2>, LR<1> > >,
				segment< P< LR<1>, LR<1> >, P< LR<2>, LR<0> > >, segment< P< LR<2>, LR<1> >, P< LR<3>, LR<0> > > > >::result == 0),
				("Expected [(0, 0) - (1, 1), a: 1, b: 0, minx: 0, miny: 0], [(1, 0) - (2, 1), a: 1, b: -1, minx: 1, miny: 0], [(1, 1) - (2, 0), a: -1, b: 2, minx: 1, miny: 1], [(2, 1) - (3, 0), a: -1, b: 3, minx: 2, miny: 1]"));


		typedef segment< P< LR<0>, LR<0> >, P< LR<1>, LR<1> > > ns1;
		typedef segment< P< LR<1,4>, LR<1> >, P< LR<1>, LR<0> > > ns2;

		Assume((maxfunc<ns1,ns2>::result::compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,4>, LR<1,4> > >, segment< P< LR<1,4>, LR<1> >, P< LR<4,7>, LR<4,7> > >,
				segment< P< LR<4,7>, LR<4,7> >, P< LR<1>, LR<1> > > > >::result == 0),
				("Expected [(0, 0) - (1/4, 1/4), a: 1, b: 0, minx: 0, miny: 0], [(1/4, 1) - (4/7, 4/7), a: -4/3, b: 4/3, minx: 1/4, miny: 1], [(4/7, 4/7) - (1, 1), a: 1, b: 0, minx: 4/7, miny: 4/7]"));

		Assume((maxfunc< segment< P< LR<0>, LR<0> >, P< LR<1,2>, LR<1,3> > >,
			     segment< P< LR<1,4>, LR<1,2> >, P< LR<1>, LR<1> > > >::result::
			     compare< list< segment< P< LR<0>, LR<0> >, P< LR<1,4>, LR<1,6> > >, segment< P< LR<1,4>, LR<1,2> >, P< LR<1>, LR<1> > > > >::result == 0),
				("Expeted [(0, 0) - (1/4, 1/6), a: 2/3, b: 0, minx: 0, miny: 0], [(1/4, 1/2) - (1, 1), a: 2/3, b: 1/3, minx: 1/4, miny: 1/2]"));

		Assume((maxfunc< segment< P< LR<0>, LR<2,3> >, P< LR<3,4>, LR<1,3> > >,
			     segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > >::result::
			     compare< list< segment< P< LR<0>, LR<2,3> >, P< LR<1,2>, LR<4,9> > >, segment< P< LR<1,2>, LR<2,3> >, P< LR<1>, LR<3,4> > > > >::result == 0),
				("Expected [(0, 2/3) - (1/2, 4/9), a: -4/9, b: 2/3, minx: 0, miny: 2/3], [(1/2, 2/3) - (1, 3/4), a: 1/6, b: 7/12, minx: 1/2, miny: 2/3]"));

		Assume((maxfunc< segment< P< LR<0>, LR<2,3> >, P< LR<1>, LR<1,3> > >,
			     segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<3,4> > > >::result::
			     compare< list< segment< P< LR<0>, LR<2,3> >, P< LR<1,2>, LR<1,2> > >, segment< P< LR<1,2>, LR<2,3> >, P< LR<3,4>, LR<3,4> > >,
			     segment< P< LR<3,4>, LR<5,12> >, P< LR<1>, LR<1,3> > > > >::result == 0),
				("Expected [(0, 2/3) - (1/2, 1/2), a: -1/3, b: 2/3, minx: 0, miny: 2/3], [(1/2, 2/3) - (3/4, 3/4), a: 1/3, b: 1/2, minx: 1/2, miny: 2/3], [(3/4, 5/12) - (1, 1/3), a: -1/3, b: 2/3, minx: 3/4, miny: 5/12]"));

		return true;
	}
};



#endif /* GEOMETRY_TEST_H_ */