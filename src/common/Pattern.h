#pragma once

#include "Node.h"

namespace common {
	class Pattern : public Node {
	public:
		Type RetTy;
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
		unique_ptr<Node> clone();
	};

	class IntPattern : public Pattern {
	public:
		long Val;

		IntPattern(long Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class FloatPattern : public Pattern {
	public:
		double Val;

		FloatPattern(double Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class CharPattern : public Pattern {
	public:
		char Val;

		CharPattern(char Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class BoolPattern : public Pattern {
	public:
		bool Val;

		BoolPattern(bool Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class StringPattern : public Pattern {
	public:
		string Val;

		StringPattern(string Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class ListPattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		ListPattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class TuplePattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		TuplePattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class WildPattern : public Pattern {
	public:
		WildPattern(Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> Left;
		unique_ptr<Pattern> Right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class AlgebraicPattern : public Pattern {
	public:
		string Constructor;
		vector<unique_ptr<Pattern>> Patterns;

		AlgebraicPattern(string Constructor, vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};

	class ParPattern : public Pattern {
	public:
		unique_ptr<Pattern> Pat;

		ParPattern(unique_ptr<Pattern> Pat, Location Loc);

		virtual void accept(Visitor &V);
		string str();
		unique_ptr<Node> clone();
	};
}
