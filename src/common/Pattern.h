#pragma once

#include "Node.h"

namespace common {

	class Pattern : public Expression {
	public:
		Pattern(Location Loc);
		Pattern(Type Ty, Location Loc);

		virtual void accept(Visitor &V) = 0;
	};

	class Id : public Pattern {
	public:
		string Val;
		Scope* Scp;

		Id(string Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Int : public Pattern {
	public:
		long Val;

		Int(long Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Float : public Pattern {
	public:
		double Val;

		Float(double Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Bool : public Pattern {
	public:
		bool Val;

		Bool(bool Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class Char : public Pattern {
	public:
		char Val;

		Char(char Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class String : public Pattern {
	public:
		string Val;

		String(string Val, Location Loc);

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

		TuplePattern(vector<unique_ptr<Pattern>> Patterns, Location loc);

		virtual void accept(Visitor &V);
		string str();
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> Left;
		unique_ptr<Pattern> Right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern> Patterns, Location loc);

		virtual void accept(Visitor &V);
		string str();
	};
}
