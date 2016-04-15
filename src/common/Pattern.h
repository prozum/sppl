#pragma once

#include "Node.h"

namespace common {

	class Pattern : public Expr {
	public:

		Pattern(Location);
		Pattern(Type, Location);

		virtual void accept(Visitor &) = 0;
	};

	class Id : public Pattern {
	public:
		string id;
		Scope* scope;

		Id(string, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Int : public Pattern {
	public:
		long value;

		Int(long, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Float : public Pattern {
	public:
		double value;

		Float(double, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Bool : public Pattern {
	public:
		bool value;

		Bool(bool, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class Char : public Pattern {
	public:
		char value;

		Char(char, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class String : public Pattern {
	public:
		string value;

		String(string, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListPattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> patterns;

		ListPattern(vector<unique_ptr<Pattern>>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class TuplePattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> patterns;

		TuplePattern(vector<unique_ptr<Pattern>>, Location);

		virtual void accept(Visitor &);

		string str();
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> left;
		unique_ptr<Pattern> right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern>, Location);

		virtual void accept(Visitor &);

		string str();
	};
}
