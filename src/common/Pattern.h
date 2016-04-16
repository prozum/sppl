#pragma once

#include "Node.h"

namespace common {

	class Pattern : public Expression {
	public:
		Pattern(Location);
		Pattern(Type, Location);

		virtual void accept(Visitor &) = 0;
	};

	class Id : public Pattern {
	public:
		string Val;
		Scope* Scp;

		Id(string, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class Int : public Pattern {
	public:
		long Val;

		Int(long, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class Float : public Pattern {
	public:
		double Val;

		Float(double, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class Bool : public Pattern {
	public:
		bool Val;

		Bool(bool, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class Char : public Pattern {
	public:
		char Val;

		Char(char, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class String : public Pattern {
	public:
		string Val;

		String(string, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class ListPattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		ListPattern(vector<unique_ptr<Pattern>>, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class TuplePattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		TuplePattern(vector<unique_ptr<Pattern>>, Location);

		virtual void accept(Visitor &);
		string str();
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> Left;
		unique_ptr<Pattern> Right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern>, Location);

		virtual void accept(Visitor &);
		string str();
	};
}
