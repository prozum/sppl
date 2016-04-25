#pragma once

#include "Node.h"

namespace common {
	class Pattern : public Node {
	public:
		Type RetTy;
		Pattern(Type Ty, Location Loc);

		virtual void accept(Visitor &V) = 0;
		unique_ptr<Pattern> clone() const;
	private:
		Pattern *doClone() const = 0;
	};

	class IdPattern : public Pattern {
	public:
		string Val;
		Scope* Scp;

		IdPattern(string Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class IntPattern : public Pattern {
	public:
		long Val;

		IntPattern(long Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class FloatPattern : public Pattern {
	public:
		double Val;

		FloatPattern(double Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class CharPattern : public Pattern {
	public:
		char Val;

		CharPattern(char Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class BoolPattern : public Pattern {
	public:
		bool Val;

		BoolPattern(bool Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class StringPattern : public Pattern {
	public:
		string Val;

		StringPattern(string Val, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class ListPattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		ListPattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class TuplePattern : public Pattern {
	public:
		vector<unique_ptr<Pattern>> Patterns;

		TuplePattern(vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class WildPattern : public Pattern {
	public:
		WildPattern(Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class ListSplit : public Pattern {
	public:
		unique_ptr<Pattern> Left;
		unique_ptr<Pattern> Right;

		ListSplit(unique_ptr<Pattern>, unique_ptr<Pattern> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class AlgebraicPattern : public Pattern {
	public:
		string Constructor;
		vector<unique_ptr<Pattern>> Patterns;

		AlgebraicPattern(string Constructor, vector<unique_ptr<Pattern>> Patterns, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};

	class ParPattern : public Pattern {
	public:
		unique_ptr<Pattern> Pat;

		ParPattern(unique_ptr<Pattern> Pat, Location Loc);

		virtual void accept(Visitor &V);
		string str();
	private:
		Pattern *doClone() const;
	};
}
