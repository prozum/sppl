#pragma once

#include "Node.h"

namespace common {

	class Pattern : public Node {
	public:
		Pattern(Location Loc);
		Pattern(Type Ty, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	class IdPattern : public Pattern {
	public:
		string Val;
		Scope* Scp;

		IdPattern(string Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class IntPattern : public Pattern {
	public:
		long Val;

		IntPattern(long Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class FloatPattern : public Pattern {
	public:
		double Val;

		FloatPattern(double Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class CharPattern : public Pattern {
	public:
		char Val;

		CharPattern(char Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class ListPattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		ListPattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class TuplePattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		TuplePattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class WildPattern : public Pattern {
		WildPattern(Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> Left;
		unique_ptr<Pattern> Right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};
}
